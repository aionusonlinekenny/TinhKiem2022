// Goddess.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include <objbase.h>
#include <initguid.h>
#include <winsock2.h>
#include <process.h>

#include "kprotocoldef.h"

#include "IDBRoleServer.h"

#include "console.h"
#include "Event.h"
#include "macro.h"
#include "Inifile.h"
#include "Utils.h"
#include "tstring.h"
#include "Library.h"
#include "Buffer.h"
#include "CriticalSection.h"

#include "IServer.h"
#include "HeavenInterface.h"

#include "ClientNode.h"

#include <iostream>
#include <strstream>
#include <string>
#include <list>
#include <time.h>

#include "FilterTextLib.h"
#include "RoleNameFilter.h"
#include <minidumpapiset.h>

const char log_directory[] = "goddess_log";
const size_t log_threshold = 1024 * 1024;


using namespace std;

using OnlineGameLib::Win32::Console::clrscr;
using OnlineGameLib::Win32::Console::setcolor;
using OnlineGameLib::Win32::CEvent;
using OnlineGameLib::Win32::CIniFile;
using OnlineGameLib::Win32::GetAppFullPath;
using OnlineGameLib::Win32::_tstring;
using OnlineGameLib::Win32::CLibrary;
using OnlineGameLib::Win32::ToString;
using OnlineGameLib::Win32::CPackager;
using OnlineGameLib::Win32::CBuffer;
using OnlineGameLib::Win32::CCriticalSection;

/*
 * User data
 */

static CLibrary g_theHeavenLibrary("heaven.dll");

static CEvent g_hQuitEvent(NULL, true, false);
static CEvent g_hStartWorkEvent(NULL, true, false);

static size_t g_nMaxRoleCount = 3;

static uint16_t g_nServerPort = 5001;
static IServer* g_pServer = NULL;

static HANDLE g_hThread = NULL;

static CCriticalSection					g_csPlayer;

typedef map< size_t, CClientNode* >	stdPlayerMap;
static 	stdPlayerMap					g_thePlayer;

static CPackager						g_theRecv;
static CPackager						g_theSend;

UINT g_nDBEngineLoop = 0;
UINT g_nServiceLoop = 0;

extern HWND hListOutput;

#define WM_CREATE_ENGINE	WM_USER + 0x100
#define WM_ADD_CLIENT		WM_USER + 0x200
#define WM_DEL_CLIENT		WM_USER + 0x300
#define TID_BACKUP_WATCH  	1001

CFilterTextLib g_libFilterText;
CRoleNameFilter g_fltRoleName;

static size_t g_BackupSleepTime = 0;  //±¸·ÝÏß³Ì×Ô¶¯¹ÒÆðÊ±¼ä
static DWORD g_BackupSpaceTime = 0;
static bool g_IsBackupSuspend = false;  //±¸·ÝÏß³ÌÊÇ·ñÊÖ¹¤¹ÒÆð
static DWORD g_BackupBeginTime = 0;
// >>> ADD:
static BOOL   g_AutoBackupEnable  = FALSE;
static DWORD  g_AutoBackupMinutes = 30;
// <<< ADD
/*
 * Helper function
 */
BOOL CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool CreateDatabaseEngine(HWND hwnd);
void DestroyDatabaseEngine();

void LoadSetting();
void SaveSetting();

//==========¼àÊÓµÄÏß³Ì
DWORD WINAPI MonitorProc(LPVOID lpParameter) {
	while (true) {
		Sleep(60 * 1000);			//1´Î
	}
	return 0;
}


/*
 * Callback function
 */
typedef HRESULT(__stdcall* pfnCreateServerInterface)(
	REFIID	riid,
	void** ppv
	);

void __stdcall ServerEventNotify(
	LPVOID lpParam,
	const ULONG& ulnID,
	const ULONG& ulnEventType)
{
	HWND hwnd = (HWND)lpParam;

	switch (ulnEventType)
	{
	case enumClientConnectCreate:
	{
		CClientNode* pNode = CClientNode::AddNode(g_pServer, ulnID);

		if (pNode)
		{
			CCriticalSection::Owner lock(g_csPlayer);

			g_thePlayer.insert(stdPlayerMap::value_type(ulnID, pNode));

			::PostMessage(hwnd, WM_ADD_CLIENT, ulnID, 0);
		}
	}
	break;

	case enumClientConnectClose:
	{
		{
			CCriticalSection::Owner lock(g_csPlayer);

			stdPlayerMap::iterator it;
			if (g_thePlayer.end() != (it = g_thePlayer.find(ulnID)))
			{
				g_thePlayer.erase(ulnID);

				::PostMessage(hwnd, WM_DEL_CLIENT, ulnID, 0);
			}
		}

		CClientNode::DelNode(ulnID);
	}
	break;
	}
}

DWORD WINAPI ThreadProcess(void* pParam)
{
	IServer* pServer = (IServer*)pParam;

	ASSERT(pServer);

	g_hStartWorkEvent.Wait();

	//try
	//{		
	while (!g_hQuitEvent.Wait(0))
	{
		{
			CCriticalSection::Owner lock(g_csPlayer);

			stdPlayerMap::iterator it;
			for (it = g_thePlayer.begin(); it != g_thePlayer.end(); it++)
			{
				UINT index = (*it).first;
				CClientNode* pClientNode = (*it).second;

				size_t datalength = 0;
				const char* pData = (const char*)pServer->GetPackFromClient(index, datalength);

				while (pClientNode && pData && datalength)
				{
					pClientNode->AppendData(pData, datalength);

					pData = (const char*)pServer->GetPackFromClient(index, datalength);
				}
			}
		}

		if (++g_nServiceLoop & 0x80000000)
		{
			g_nServiceLoop = 0;
		}

		if (g_nServiceLoop & 0x1)
		{
			::Sleep(1);
		}
	}
	//}
	//catch(...)
	//{
	//	::MessageBox( NULL, "ThreadProcess was error!", "Warning", MB_OK );
	//}

	return 0L;
}

LONG WINAPI UnhandledExceptionFilterCallback(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	// Tạo tên file dựa trên thời gian
	TCHAR szDumpFileName[MAX_PATH];
	_stprintf_s(szDumpFileName, MAX_PATH, _T("CrashDump_Goddess_%04d%02d%02d_%02d%02d%02d.dmp"),
		localTime.wYear, localTime.wMonth, localTime.wDay,
		localTime.wHour, localTime.wMinute, localTime.wSecond);

	// Tạo một file dump khi có lỗi
	HANDLE hFile = CreateFile(szDumpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION minidumpExceptionInfo;
		minidumpExceptionInfo.ThreadId = GetCurrentThreadId();
		minidumpExceptionInfo.ExceptionPointers = ExceptionInfo;
		minidumpExceptionInfo.ClientPointers = FALSE;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &minidumpExceptionInfo, NULL, NULL);

		CloseHandle(hFile);
	}

	// Chuyển xử lý lỗi về cho hệ thống
	return EXCEPTION_CONTINUE_SEARCH;
}

int32_t APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR     lpCmdLine, int32_t       nCmdShow)
{
	// Đăng ký hàm xử lý ngoại lệ
	SetUnhandledExceptionFilter(UnhandledExceptionFilterCallback);

	TCHAR szExePath[MAX_PATH + 1];

	if (szExePath)
	{
		memset(szExePath, 0, MAX_PATH);
		GetModuleFileName(NULL, szExePath, MAX_PATH);
		DWORD dwEndAt;
		dwEndAt = strlen(szExePath) - 1;
		while (dwEndAt >= 1)
		{
			if (szExePath[dwEndAt - 1] == '\\')
			{
				szExePath[dwEndAt] = 0;
				break;
			}
			dwEndAt--;
		}
	}

	TCHAR szLogDir[MAX_PATH + 1];
	_tcscpy(szLogDir, szExePath);
	_tcscat(szLogDir, log_directory);
	::CreateDirectory(szLogDir, NULL);

	//role name filter
	if (!g_libFilterText.Initialize()
		|| !g_fltRoleName.Initialize())
	{
		MessageBox(NULL, "text filter's initing has failed", "error", MB_OK | MB_ICONERROR);
		return -1;
	}



	HWND hwndMain = ::CreateDialog(hInstance,
		MAKEINTRESOURCE(IDD_DLG_GODDESS),
		NULL,
		(DLGPROC)MainWndProc);

	ASSERT(hwndMain);

	::ShowWindow(hwndMain, SW_SHOWNORMAL);
	::UpdateWindow(hwndMain);

	hListOutput = ::GetDlgItem(hwndMain, IDC_OUTPUT);//Êä³ölist

	BOOL bRet;
	MSG msg;

	while ((bRet = ::GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (!IsWindow(hwndMain) || !IsDialogMessage(hwndMain, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}


	//rolename filter
	g_fltRoleName.Uninitialize();
	g_libFilterText.Uninitialize();


	return msg.wParam;
}

/*
 *
 * MainWndProc() - Main window callback procedure.
 *
 */

BOOL CALLBACK MainWndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	static const UINT g_nIDEvent = 0x100;
	static const UINT g_nElapse = 500;

	static const char g_szBaseInfo[] = "....................";
	static const int32_t g_nDots = sizeof(g_szBaseInfo) - 1;

	static UINT g_nLastServiceLoop = g_nServiceLoop;
	static int32_t g_nServiceStep = g_nDots;
	static UINT g_nLastDBEngineLoop = g_nDBEngineLoop;
	static int32_t g_nDBEngineStep = g_nDots;

	BOOL bTranslated = TRUE;

	switch (msg)
	{
	/*case WM_TIMER:
	{
		if (g_nLastServiceLoop != g_nServiceLoop)
		{
			g_nServiceStep = (--g_nServiceStep > 0) ? g_nServiceStep : g_nDots;

			::SetDlgItemText(hwnd, IDC_STATIC_SERVICE, (const char*)g_szBaseInfo + g_nServiceStep);

			g_nLastServiceLoop = g_nServiceLoop;
		}

		if (g_nLastDBEngineLoop != g_nDBEngineLoop)
		{
			g_nDBEngineStep = (--g_nDBEngineStep > 0) ? g_nDBEngineStep : g_nDots;

			::SetDlgItemText(hwnd, IDC_STATIC_DATABASE, (const char*)g_szBaseInfo + g_nDBEngineStep);

			g_nLastDBEngineLoop = g_nDBEngineLoop;
		}

		if (IsBackupThreadWorking())
		{
			::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), TRUE);
			::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS),
				"Backup thread status: Sleeping");
			::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), TRUE);
			if (IsBackupWorking())
			{
				::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), FALSE);
				::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS),
					"Backup thread status: Running");
				::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), FALSE);
			}
			else if (g_IsBackupSuspend)
			{
				::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), TRUE);
				::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS),
					"Backup thread status: Suspended");
				::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), TRUE);
			}

		}
		else
		{
			::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES), FALSE);
			::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS),
				"Backup thread status: Stop");
			::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), FALSE);
		}
	}
	break;
	*/case WM_TIMER:
	{
		if (wParam == TID_BACKUP_WATCH)
		{
			if (!IsBackupWorking())
			{
				KillTimer(hwnd, TID_BACKUP_WATCH);

				char path[1024] = {0};
				bool ok = WasLastBackupOk();
				GetLastBackupPath(path, sizeof(path));

				// Bật lại nút Manual
				::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), TRUE);
				::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), ok ? "Backup: Idle" : "Backup: Failed");

				if (ok && path[0])
				{
					std::string msg = std::string("Backup completed.\r\nFile:\r\n") + path;
					MessageBox(hwnd, msg.c_str(), "Backup", MB_OK | MB_ICONINFORMATION);
					AddOutputString(hListOutput, (char*)msg.c_str());
				}
				else
				{
					MessageBox(hwnd, "Backup finished, but no output path.\r\nPlease check logs.",
							   "Backup", MB_OK | MB_ICONWARNING);
					AddOutputString(hListOutput, "[Manual] Finished but no output path.");
				}
			}
			return TRUE;
		}
		break;
	}

	case WM_INITDIALOG:

		LoadSetting();

		::SetDlgItemInt(hwnd, IDC_EDIT_PORT, g_nServerPort, FALSE);
		::SetDlgItemInt(hwnd, IDC_EDIT_MAXNUM_ROLE, g_nMaxRoleCount, FALSE);
		::SetDlgItemInt(hwnd, IDC_EDIT_BACKUP_SLEEP_TIME, g_BackupSleepTime, FALSE);
		::SetDlgItemInt(hwnd, IDC_EDIT_BACKUP_SPACE_TIME, g_BackupSpaceTime, FALSE);
		::SetDlgItemInt(hwnd, IDC_EDIT_BACKUP_BEGIN_TIME, g_BackupBeginTime, FALSE);
		::CheckDlgButton(hwnd, IDC_CHK_AUTO_BACKUP, g_AutoBackupEnable ? BST_CHECKED : BST_UNCHECKED);
		::SetDlgItemInt(hwnd, IDC_EDIT_AUTO_MINUTES, g_AutoBackupMinutes, FALSE);
		::EnableWindow(GetDlgItem(hwnd, IDC_EDIT_AUTO_MINUTES), g_AutoBackupEnable);
		
		::SetTimer(hwnd, g_nIDEvent, g_nElapse, NULL);

		return TRUE;
		break;

	case WM_CLOSE:
	{
		if (IDYES == ::MessageBox(hwnd, "Are you sure quit?", "Info", MB_YESNO | MB_ICONQUESTION))
		{

			DestroyDatabaseEngine();

			::KillTimer(hwnd, g_nIDEvent);

			g_nServerPort = ::GetDlgItemInt(hwnd, IDC_EDIT_PORT, &bTranslated, FALSE);
			g_nMaxRoleCount = ::GetDlgItemInt(hwnd, IDC_EDIT_MAXNUM_ROLE, &bTranslated, FALSE);
			g_BackupSleepTime = ::GetDlgItemInt(hwnd, IDC_EDIT_BACKUP_SLEEP_TIME, &bTranslated, FALSE);
			g_BackupSpaceTime = ::GetDlgItemInt(hwnd, IDC_EDIT_BACKUP_SPACE_TIME, &bTranslated, FALSE);
			g_BackupBeginTime = ::GetDlgItemInt(hwnd, IDC_EDIT_BACKUP_BEGIN_TIME, &bTranslated, FALSE);

			SaveSetting();

			::DestroyWindow(hwnd);
			::PostQuitMessage(0);
		}
	}

	return TRUE;
	break;

	case WM_COMMAND:

		switch (wParam)
		{
		case IDOK:
			::EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
			::EnableWindow(GetDlgItem(hwnd, IDC_EDIT_PORT), FALSE);
			::EnableWindow(GetDlgItem(hwnd, IDC_EDIT_MAXNUM_ROLE), FALSE);
			::EnableWindow(GetDlgItem(hwnd, IDC_EDIT_BACKUP_SLEEP_TIME), FALSE);
			::EnableWindow(GetDlgItem(hwnd, IDC_EDIT_BACKUP_SPACE_TIME), FALSE);
			::EnableWindow(GetDlgItem(hwnd, IDC_EDIT_BACKUP_BEGIN_TIME), FALSE);

			::SetWindowText(hwnd, "Goddess - [Start up...]");

			::PostMessage(hwnd, WM_CREATE_ENGINE, 0L, 0L);

			break;

		case IDCANCEL:

			::PostMessage(hwnd, WM_CLOSE, 0L, 0L);
			break;
/*
		case IDC_BTN_BACKUP_MANUAL:

			if (!IsBackupThreadWorking())
				MessageBox(hwnd, "Backup thread is not started yet.",
					"Information", MB_OK | MB_ICONEXCLAMATION);
			if (!DoManualBackup())
				MessageBox(hwnd, "Unable to start manual backup.\nPlease try later.",
					"Information", MB_OK | MB_ICONEXCLAMATION);

			break;*/
		case IDC_BTN_BACKUP_MANUAL:
		{
			// Engine luôn sẵn sàng (kể cả Auto OFF)
			InitBackupEngine();

			// Khoá nút để tránh bấm lặp
			::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), FALSE);

			// Gọi manual
			if (!DoManualBackup())
			{
				MessageBox(hwnd,
						   "Unable to start manual backup.\r\nPlease try later.",
						   "Information",
						   MB_OK | MB_ICONEXCLAMATION);

				::EnableWindow(GetDlgItem(hwnd, IDC_BTN_BACKUP_MANUAL), TRUE);
			}
			else
			{
				// Cập nhật trạng thái và **BẮT ĐẦU TIMER** theo dõi hoàn tất
				::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), "Backup: Running (manual)");
				SetTimer(hwnd, TID_BACKUP_WATCH, 200, NULL);  // <-- DÒNG QUAN TRỌNG (4.2)
			}
			break;
		}

		case IDC_RESTART:
			CClientNode::End();
			ReleaseDBInterface();
			InitDBInterface(g_nMaxRoleCount);
			CClientNode::Start(g_pServer);
			break;
		case IDC_CHK_AUTO_BACKUP:
			{
			g_AutoBackupEnable = (::IsDlgButtonChecked(hwnd, IDC_CHK_AUTO_BACKUP) == BST_CHECKED);
			::EnableWindow(GetDlgItem(hwnd, IDC_EDIT_AUTO_MINUTES), g_AutoBackupEnable);
			break;
			}
		case IDC_BTN_BACKUP_SUS_RES:

			if (!IsBackupThreadWorking())
				MessageBox(hwnd, "Backup thread is not started yet.",
					"Information", MB_OK | MB_ICONEXCLAMATION);
			else if (g_IsBackupSuspend)
			{
				if (ResumeBackupTimer())
				{
					g_IsBackupSuspend = false;
					::SetWindowText(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES),
						"Suspend");
				}
				else
					MessageBox(hwnd, "Unable to resume backup thread.\nPlease try later.",
						"Information", MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				if (SuspendBackupTimer())
				{
					g_IsBackupSuspend = true;
					::SetWindowText(GetDlgItem(hwnd, IDC_BTN_BACKUP_SUS_RES),
						"Resume");
				}
				else
					MessageBox(hwnd, "Unable to suspend backup thread.\nPlease try later.",
						"Information", MB_OK | MB_ICONEXCLAMATION);
			}
			break;
		}

		break;

	case WM_CREATE_ENGINE:
	{
		time_t rawtime;
		struct tm* timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		char szText[64];
		sprintf(szText, "%04d%02d%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
		if ((int32_t)(atoi(szText)) > KPROTOCOL_EXPIRATION_DATE)
		{
			MessageBox(hwnd, "Protocol Version is illegal",
				"Information", MB_OK | MB_ICONERROR);
			break;
		}

		g_nServerPort = ::GetDlgItemInt(hwnd, IDC_EDIT_PORT, &bTranslated, FALSE);
		g_nMaxRoleCount = ::GetDlgItemInt(hwnd, IDC_EDIT_MAXNUM_ROLE, &bTranslated, FALSE);
		g_BackupSleepTime = ::GetDlgItemInt(hwnd, IDC_EDIT_BACKUP_SLEEP_TIME, &bTranslated, FALSE);
		g_BackupSpaceTime = ::GetDlgItemInt(hwnd, IDC_EDIT_BACKUP_SPACE_TIME, &bTranslated, FALSE);
		g_BackupBeginTime = ::GetDlgItemInt(hwnd, IDC_EDIT_BACKUP_BEGIN_TIME, &bTranslated, FALSE);
		g_AutoBackupEnable  = (::IsDlgButtonChecked(hwnd, IDC_CHK_AUTO_BACKUP) == BST_CHECKED);
		g_AutoBackupMinutes = ::GetDlgItemInt(hwnd, IDC_EDIT_AUTO_MINUTES, &bTranslated, FALSE);

		CreateDatabaseEngine(hwnd);

		::SetWindowText(hwnd, "Goddess - [Enable]");
	}
	break;

	case WM_ADD_CLIENT:
	{
		//std::string szIpAddress = ;
		const char* pText = g_pServer->GetClientInfo(wParam);

		if (pText && pText[0])
		{
			HWND hCtrl = ::GetDlgItem(hwnd, IDC_LIST_CLIENT);

			if (hCtrl && ::IsWindow(hCtrl))
			{
				int32_t nIndex = ::SendMessage(hCtrl, LB_ADDSTRING, 0, (LPARAM)pText);

				if (LB_ERR != nIndex)
				{
					::SendMessage(hCtrl, LB_SETITEMDATA, nIndex, wParam);
				}
			}
		}
	}
	break;

	case WM_DEL_CLIENT:
	{
		HWND hCtrl = ::GetDlgItem(hwnd, IDC_LIST_CLIENT);

		if (hCtrl && ::IsWindow(hCtrl))
		{
			int32_t nCount = ::SendMessage(hCtrl, LB_GETCOUNT, 0, 0);

			for (int32_t i = 0; i < nCount && LB_ERR != nCount; i++)
			{
				UINT nSearchID = 0;

				if (wParam == (nSearchID = ::SendMessage(hCtrl, LB_GETITEMDATA, i, 0)))
				{
					::SendMessage(hCtrl, LB_DELETESTRING, i, 0);

					return TRUE;
				}
			}
		}
	}
	break;

	default:
		break;
	}

	/*
	 * Clean up any unused messages by calling DefWindowProc
	 */
	return FALSE;
}

bool CreateDatabaseEngine(HWND hwnd)
{
	if (!InitDBInterface(g_nMaxRoleCount))
	{
		::MessageBox(NULL, "Setup dbserver is failed!", "Warning", MB_OK | MB_ICONSTOP);

		return false;
	}
/*
	StartBackupTimer(g_BackupSleepTime, g_BackupBeginTime); //¿ªÊ¼ÔËÐÐ±¸·ÝÏß³Ì

	::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), "Backup thread status: Running");*/
	if (g_AutoBackupEnable && g_AutoBackupMinutes > 0) {
    StartBackupTimerByMinutes(g_AutoBackupMinutes);
    ::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), "Backup thread status: Running");
	} else {
		::SetWindowText(GetDlgItem(hwnd, IDC_LAB_BACKUP_STATUS), "Backup thread status: Stop");
	}

	/*
	 * Open this server to client
	 */
	pfnCreateServerInterface pFactroyFun = (pfnCreateServerInterface)(g_theHeavenLibrary.GetProcAddress("CreateInterface"));

	IServerFactory* pServerFactory = NULL;

	if (pFactroyFun && SUCCEEDED(pFactroyFun(IID_IServerFactory, reinterpret_cast<void**>(&pServerFactory))))
	{
		pServerFactory->SetEnvironment(10, 10, 20, 4 * 1024 * 1024);

		pServerFactory->CreateServerInterface(IID_IIOCPServer, reinterpret_cast<void**>(&g_pServer));

		pServerFactory->Release();
	}

	if (!g_pServer)
	{
		::MessageBox(NULL, "Initialization failed! Don't find a correct heaven.dll", "Warning", MB_OK | MB_ICONSTOP);

		return false;
	}

	g_pServer->Startup();

	g_pServer->RegisterMsgFilter((void*)hwnd, ServerEventNotify);

	if (FAILED(g_pServer->OpenService(INADDR_ANY, g_nServerPort)))
	{
		return false;
	}

	DWORD dwThreadID = 0L;

	IServer* pCloneServer = NULL;
	g_pServer->QueryInterface(IID_IIOCPServer, (void**)&pCloneServer);

	g_hThread = ::CreateThread(NULL, 0, ThreadProcess, (void*)pCloneServer, 0, &dwThreadID);

	if (!g_hThread)
	{
		return false;
	}

	CClientNode::Start(g_pServer);

	g_hStartWorkEvent.Set();

	return true;
}

void DestroyDatabaseEngine()
{
	CClientNode::End();

	g_hQuitEvent.Set();

	DWORD result = ::WaitForSingleObject(g_hThread, 50000);

	if (result == WAIT_TIMEOUT)
	{
		::TerminateThread(g_hThread, (DWORD)(-1));
	}

	SAFE_CLOSEHANDLE(g_hThread);

	if (g_pServer)
	{
		g_pServer->CloseService();
		g_pServer = NULL;
	}

	SAFE_RELEASE(g_pServer);

	ReleaseDBInterface();
}

void LoadSetting()
{
	CIniFile theConfigFile;

	_tstring sAppPath = GetAppFullPath(NULL);

	_tstring sConfigFile;

	sConfigFile = sAppPath + "Goddess.cfg";

	theConfigFile.SetFile(sConfigFile.c_str());

	g_nServerPort = theConfigFile.ReadInteger("Setting", "Port", 5001);

	g_nMaxRoleCount = theConfigFile.ReadInteger("Setting", "MaxRoleCount", 3);

	g_BackupSleepTime = theConfigFile.ReadInteger("Setting", "BackupSleepTime", 0);

	g_BackupSpaceTime = theConfigFile.ReadInteger("Setting", "BackupSpaceTime", 0);

	g_BackupBeginTime = theConfigFile.ReadInteger("Setting", "BackupBeginTime", 0);
	// >>> ADD:
	g_AutoBackupEnable  = theConfigFile.ReadInteger("Setting", "AutoBackupEnable", 0) ? TRUE : FALSE;
	
	g_AutoBackupMinutes = theConfigFile.ReadInteger("Setting", "AutoBackupMinutes", 60);
	// <<< ADD
}

void SaveSetting()
{
	CIniFile theConfigFile;

	_tstring sAppPath = GetAppFullPath(NULL);

	_tstring sConfigFile;

	sConfigFile = sAppPath + "Goddess.cfg";

	theConfigFile.SetFile(sConfigFile.c_str());

	theConfigFile.WriteInteger("Setting", "Port", g_nServerPort);

	theConfigFile.WriteInteger("Setting", "MaxRoleCount", g_nMaxRoleCount);

	theConfigFile.WriteInteger("Setting", "BackupSleepTime", g_BackupSleepTime);

	theConfigFile.WriteInteger("Setting", "BackupSpaceTime", g_BackupSpaceTime);

	theConfigFile.WriteInteger("Setting", "BackupBeginTime", g_BackupBeginTime);
	// >>> ADD:
	theConfigFile.WriteInteger("Setting", "AutoBackupEnable",  g_AutoBackupEnable ? 1 : 0);
	
	theConfigFile.WriteInteger("Setting", "AutoBackupMinutes", g_AutoBackupMinutes);
	// <<< ADD
}