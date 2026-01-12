// S3Client.cpp : Defines the entry point for the application.
//

// ReSharper disable All
#include "KWin32.h"
#include "KCore.h"
#include "S3Client.h"
#include "KWin32Wnd.h"
#include "../../Represent/iRepresent/iRepresentShell.h"
#include "Ui/UiShell.h"
#include "NetConnect/NetConnectAgent.h"
#include "TextCtrlCmd/TextCtrlCmd.h"
#include "KPakList.h"
#include "Ui/Elem/TextPic.h"
#include "Ui/Elem/UiCursor.h"
#include "Ui/Elem/SpecialFuncs.h"
#include "Ui/FilterTextLib.h"
#include "Ui/ChatFilter.h"
#include "Ui/uibase.h"
#include "ErrorCode.h"
#include <tlhelp32.h>
#include <tchar.h>
#include "S3Config.h"
#include "Ui/UiCase/UiInit.h"
#include <dbghelp.h>
#include <cstdint>

//using std::byte;

#define ClientVersion
KMyApp		MyApp;
HINSTANCE	hInst;
KPakList	g_PakList;
CFilterTextLib g_libFilterText;
CChatFilter g_ChatFilter;

extern INT			g_bDisconnect;

#define	QUIT_QUESTION_ID	"22"
#define	GAME_TITLE			"23"

#define REPRESENT_MODULE_2			"Represent2.dll"
#define REPRESENT_MODULE_3			"Represent3.dll"
#define CREATE_REPRESENT_SHELL_FUN	"CreateRepresentShell"
#define	GAME_FPS			18

struct iRepresentShell* g_pRepresentShell = NULL;
struct IInlinePicEngineSink* g_pIInlinePicSink = NULL;
iCoreShell* g_pCoreShell = NULL;
KMusic* g_pMusic = NULL;

#define	DYNAMIC_LINK_REPRESENT_LIBRARY

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
static HMODULE		l_hRepresentModule = NULL;
INT					g_bRepresent3 = false;
#endif

INT					g_bScreen = true;
char				g_szGameName[128] = "Vo Lam Truyen Ky";

KClientCallback g_ClientCallback;

#define	SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

INT g_nScreenWidth = SCREEN_WIDTH;
INT g_nScreenHeight = SCREEN_HEIGHT;

/*
 * Add this macro by liupeng on 2003.3.20
 * This macro is helper that can judge some legal character
 */
#define _private_IS_SPACE(c)   ((c) == ' ' || (c) == '\r' || (c) == '\n' || (c) == '\t' || (c) == 'x')
#define IS_SPACE(c)	_private_IS_SPACE(c)

void GioiHanCuaSo();
LONG WINAPI UnhandledExceptionFilterCallback(EXCEPTION_POINTERS* ExceptionInfo);

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	// TODO: Place code here.
	// Đăng ký hàm xử lý ngoại lệ
	SetUnhandledExceptionFilter(UnhandledExceptionFilterCallback);

	GioiHanCuaSo();

	/*
	 * Add this funtion by liupeng on 2003.3.20
	 * We can find some error when start a console tracer
	 */
#ifdef	TRUE

	bool bOpenTracer = false;

	while (lpCmdLine[0] == '-' || lpCmdLine[0] == '/')
	{
		lpCmdLine++;

		switch (*lpCmdLine++)
		{
		case 'c':
		case 'C':
			bOpenTracer = true;
			break;
		}

		while (IS_SPACE(*lpCmdLine))
		{
			lpCmdLine++;
		}
	}

	if (bOpenTracer)
	{
		AllocConsole();
	}

#endif // End of this function

	hInst = hInstance;
	if (MyApp.Init(hInstance))
		MyApp.Run();

#ifdef TRUE

	if (bOpenTracer)
	{
		FreeConsole();
	}

#endif
	Error_Box();

	return 0;
}

KMyApp::KMyApp()
{
	m_GameCounter = 0;
	m_pInlinePicSink = NULL;
}

BOOL InitRepresentShell(BOOL bFullScreen, INT nWidth, INT nHeight)
{
	Error_SetErrorString(g_bRepresent3 ? REPRESENT_MODULE_3 : REPRESENT_MODULE_2);
	if (g_pRepresentShell == NULL)
	{
#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
		if (l_hRepresentModule == NULL &&
			(l_hRepresentModule = LoadLibrary(g_bRepresent3 ? REPRESENT_MODULE_3 : REPRESENT_MODULE_2)) == NULL)
		{
			Error_SetErrorCode(ERR_T_LOAD_MODULE_FAILED);
			return FALSE;
		}
		fnCreateRepresentShell pCreate = (fnCreateRepresentShell)GetProcAddress(
			l_hRepresentModule, CREATE_REPRESENT_SHELL_FUN);
		if (pCreate == NULL ||
			(g_pRepresentShell = pCreate()) == NULL)
		{
			Error_SetErrorCode((pCreate == NULL) ? ERR_T_MODULE_UNCORRECT : ERR_T_MODULE_INIT_FAILED);
			return FALSE;
		}
#else
		g_pRepresentShell = CreateRepresentShell();
#endif
	}
	if (g_pRepresentShell->Create(nWidth, nHeight, bFullScreen != 0))
	{
		return TRUE;
	}
	else
	{
		Error_SetErrorCode(g_bRepresent3 ? ERR_T_REPRESENT3_INIT_FAILED : ERR_T_REPRESENT2_INIT_FAILED);
		return FALSE;
	}
}

BOOL KMyApp::GameInit()
{
	Error_SetErrorString("KMyApp::GameInit");
#ifdef KUI_USE_HARDWARE_MOUSE

	//g_FindDebugWindow("#32770", "DebugWin");
	ShowMouse(TRUE);

#else   // KUI_USE_HARDWARE_MOUSE

	ShowMouse(FALSE);

#endif

	g_SetRootPath(NULL);
	g_SetFilePath("\\");

	KIniFile* pSetting = g_UiBase.GetCommConfigFile();
	//if (pSetting)
	//{
		//	g_DebugLog("Set Game Name");
		//pSetting->GetString("Main", "GameName", "JxOnline", g_szGameName, sizeof(g_szGameName));
	SetWindowText(g_GetMainHWnd(), g_szGameName);
	//}

#ifdef _DEBUG
	g_FindDebugWindow("#32770", "DebugWin");
#endif

	KIniFile	IniFile;
	if (!IniFile.Load("\\config.ini"))
	{
		Error_SetErrorCode(ERR_T_FILE_NO_FOUND);
		Error_SetErrorString("\\config.ini");
		return FALSE;
	}

#ifdef _DEBUG
	BOOL		bCursor = FALSE;
	if (IniFile.GetInteger("Client", "ShowCursor", 0, &bCursor))
		ShowMouse(TRUE);
#endif
	INT nWindowSize = 1;
	IniFile.GetInteger("Client", "FullScreen", FALSE, &g_bScreen);
	IniFile.GetInteger("Client", "WindowSize", 1, &nWindowSize);

	if (nWindowSize == 2)
	{
		g_nScreenWidth = 1024;
		g_nScreenHeight = 768;
	}

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	IniFile.GetInteger("Client", "Represent", 2, &g_bRepresent3);
	g_bRepresent3 = (g_bRepresent3 == 3);
#endif

	g_PakList.Open("\\package.ini");

	char	szPath[MAX_PATH];
	if (IniFile.GetString("Client", "CapPath", "", szPath, sizeof(szPath)))
	{
		if (szPath[0])
			SetScrPicPath(szPath);
	}

	IniFile.Clear();

	if (!g_libFilterText.Initialize() || !g_ChatFilter.Initialize())
		return FALSE;

	//if (!InitRepresentShell(g_bScreen, SCREEN_WIDTH, SCREEN_HEIGHT))
	if (!InitRepresentShell(g_bScreen, g_nScreenWidth, g_nScreenHeight))
	{
		return FALSE;
	}

	UiSetScreenSize(g_nScreenWidth, g_nScreenHeight);

	if (!UiInit())
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("UiInit");
		return FALSE;
	}

	//[wxb 2003-6-23]
	m_pInlinePicSink = new KInlinePicSink;
	if (m_pInlinePicSink)
	{
		m_pInlinePicSink->Init(g_pRepresentShell);
		_ASSERT(NULL == g_pIInlinePicSink);
		g_pIInlinePicSink = m_pInlinePicSink;
	}

	//UiSetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	UiPaint(0);

	// init dsound
	m_Sound.Init();

	SetMultiGame(TRUE);

	if ((g_pCoreShell = CoreGetShell()) == NULL)
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("CoreGetShell");
		return false;
	}
	g_pCoreShell->SetRepresentShell(g_pRepresentShell);
	g_pCoreShell->SetMusicInterface((KMusic*)&m_Music);
	g_pCoreShell->SetCallDataChangedNofify(&g_ClientCallback);
	//g_pCoreShell->SetRepresentAreaSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pCoreShell->SetRepresentAreaSize(g_nScreenWidth, g_nScreenHeight);
	g_pCoreShell->SetScreenSize(g_nScreenWidth, g_nScreenHeight);

	g_pMusic = &m_Music;

	if (g_NetConnectAgent.Initialize() == 0)
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("NetConnectAgent");
		return FALSE;
	}

	m_GameCounter = 0;
	m_Timer.Start();

	SetMouseHoverTime(400);

	if (UiStart())
	{
		return TRUE;
	}
	else
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("UiStart");
		return FALSE;
	}
}

BOOL KMyApp::GameExit()
{
	if (m_pInlinePicSink)
	{
		//[wxb 2003-6-23]
		m_pInlinePicSink->UnInit();
		delete m_pInlinePicSink;
		m_pInlinePicSink = NULL;
		g_pIInlinePicSink = NULL;
	}

	UiExit();

	g_pMusic = NULL;
	if (g_pCoreShell)
	{
		g_pCoreShell->SetRepresentShell(NULL);
		g_pCoreShell->SetClient(NULL);
		g_pCoreShell->SetMusicInterface(NULL);
		g_pCoreShell->Release();
		g_pCoreShell = NULL;
	}

	if (g_pRepresentShell)
	{
		g_pRepresentShell->Release();
		g_pRepresentShell = NULL;
	}

	g_NetConnectAgent.Exit();

	m_Music.Close();
	m_Sound.Exit();

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	if (l_hRepresentModule)
	{
		FreeLibrary(l_hRepresentModule);
		l_hRepresentModule = NULL;
	}
#endif

	::ShowCursor(TRUE);

	g_ChatFilter.Uninitialize();
	g_libFilterText.Uninitialize();
	return TRUE;
}

BOOL KMyApp::GameLoop()
{
	static INT nGameFps = 0;
	g_NetConnectAgent.Breathe();
	if (m_GameCounter * 1000 <= m_Timer.GetElapse() * GAME_FPS)
	{
		if (g_pCoreShell->Breathe(g_bDisconnect) && UiHeartBeat())
		{
			m_GameCounter++;
			INT	nElapse = m_Timer.GetElapse();
			if (nElapse)
				nGameFps = m_GameCounter * 1000 / nElapse;
		}
		else
		{
			return false;
		}
	}
	if (m_GameCounter * 1000 >= m_Timer.GetElapse() * GAME_FPS)
	{
		UiPaint(nGameFps);
		Sleep(1);
	}
	else if ((m_GameCounter % 8) == 0)
	{
		Sleep(1);
	}

	return true;
}

INT KMyApp::HandleInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT nRet = 0;
	if (uMsg != WM_CLOSE)
	{
		UiProcessInput(uMsg, wParam, lParam);
	}
	else if (g_bScreen == false && UiIsAlreadyQuit() == false)
	{
		KIniFile* pSetting = g_UiBase.GetCommConfigFile();
		if (pSetting)
		{
			char	szMsg[128], szTitle[64];
			pSetting->GetString("InfoString", QUIT_QUESTION_ID, "", szMsg, sizeof(szMsg));
			pSetting->GetString("InfoString", GAME_TITLE, "", szTitle, sizeof(szTitle));
			if (szMsg[0] && szTitle[0])
			{
				nRet = (MessageBox(g_GetMainHWnd(), szMsg, szTitle,
					MB_YESNO | MB_ICONQUESTION) != IDYES);
			}
		}
	}
	return nRet;
}

#include "api.h"
#include <iostream>
#include <cctype>

/*
 * Hàm này đếm số lượng tiến trình có tên là "GAME.EXE".
 * Đây là một chức năng hợp lệ và không có ý định gây hại.
 */
INT CountProcessesWithFileName()
{
	HANDLE Handle;
	PROCESSENTRY32 ProcI;
	Handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Handle == INVALID_HANDLE_VALUE)
	{
		// Xử lý lỗi, có thể là thông báo hoặc trả về giá trị đặc biệt
		return 0;
	}

	ProcI.dwSize = sizeof(PROCESSENTRY32);
	INT sl = 0;
	do
	{
		CHAR* strMessage = ProcI.szExeFile;

		// Chuyển đổi chuỗi ProcI_szExeFile sang chữ hoa
		TCHAR uppercaseProcI_szExeFile[256];
		for (INT i = 0; strMessage[i] != '\0'; i++) {
			uppercaseProcI_szExeFile[i] = std::toupper(strMessage[i]);
		}
		uppercaseProcI_szExeFile[_tcslen(strMessage)] = '\0';

		// So sánh chuỗi đã chuyển đổi với chuỗi "GAME.EXE"
		if (_tcscmp(uppercaseProcI_szExeFile, _T("GAME.EXE")) == 0) {
			sl = sl + 1;
		};

	} while (Process32Next(Handle, &ProcI));
	CloseHandle(Handle);
	return sl;
}

void DU_MessageBoxTimeout(HWND hWnd, const WCHAR* sText, const WCHAR* sCaption, UINT uType, DWORD dwMilliseconds)
{
	// Displays a message box, and dismisses it after the specified timeout.
	typedef INT(__stdcall* MSGBOXWAPI)(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

	INT iResult;

	HMODULE hUser32 = LoadLibraryA("user32.dll");
	if (hUser32)
	{
		auto MessageBoxTimeoutW = (MSGBOXWAPI)GetProcAddress(hUser32, "MessageBoxTimeoutW");

		iResult = MessageBoxTimeoutW(hWnd, sText, sCaption, uType, 0, dwMilliseconds);

		FreeLibrary(hUser32);
	}
	else
		iResult = MessageBoxW(hWnd, sText, sCaption, uType);         // oups, fallback to the standard function!

	//return iResult;
}

void GioiHanCuaSo()
{
	if (CountProcessesWithFileName() > MAX_LIMIT_ONE_PC) // Gioi Han Cua So
	{
		string szMsg = "Game chi cho phep chay toi da " + to_string(MAX_LIMIT_ONE_PC) + " cua so\nThoat game!";

		DU_MessageBoxTimeout(NULL, wstring(szMsg.begin(), szMsg.end()).c_str(), L"Thông Báo", MB_SERVICE_NOTIFICATION | MB_ICONWARNING, 3000);
		exit(EXIT_FAILURE);
		ExitProcess(1);
	}
}

LONG WINAPI UnhandledExceptionFilterCallback(EXCEPTION_POINTERS* ExceptionInfo)
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	// Tạo tên file dựa trên thời gian
	TCHAR szDumpFileName[MAX_PATH];
	_stprintf_s(szDumpFileName, MAX_PATH, _T("CrashDump_%04d%02d%02d_%02d%02d%02d.dmp"),
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