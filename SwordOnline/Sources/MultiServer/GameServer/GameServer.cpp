// GameServer.cpp : Defines the entry point for the console application.
//

/*#ifdef _STANDALONE
#ifndef __linux
//#include <winsock2.h>
#endif
#endif*/

#include "StdAfx.h"
#include "KSOServer.h"
#include <cwchar>
#include <wincon.h>
#include <dbghelp.h>
#include <tchar.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <thread>
#include <fstream>

extern KSwordOnLineSever g_SOServer;
#ifdef __linux
#include <unistd.h>
#include <signal.h> 
#include <sys/param.h> 
#include <sys/types.h> 
#include <sys/stat.h> 

void init_daemon(void) {
	INT pid;
	INT i;
	if (pid = fork()) exit(0);			//ÊÇ¸¸½ø³Ì£¬½áÊø¸¸½ø³Ì 
	else if (pid < 0) exit(1);		//forkÊ§°Ü£¬ÍË³ö 
	//ÊÇµÚÒ»×Ó½ø³Ì£¬ºóÌ¨¼ÌÐøÖ´ÐÐ 
	setsid();//µÚÒ»×Ó½ø³Ì³ÉÎªÐÂµÄ»á»°×é³¤ºÍ½ø³Ì×é³¤ 
	//²¢Óë¿ØÖÆÖÕ¶Ë·ÖÀë 
	if (pid = fork()) exit(0);//ÊÇµÚÒ»×Ó½ø³Ì£¬½áÊøµÚÒ»×Ó½ø³Ì 
	else if (pid < 0)
		exit(1);//forkÊ§°Ü£¬ÍË³ö 
	//ÊÇµÚ¶þ×Ó½ø³Ì£¬¼ÌÐø 
	//µÚ¶þ×Ó½ø³Ì²»ÔÙÊÇ»á»°×é³¤ 

	for (i = 0; i < NOFILE; ++i) close(i);
	umask(0);//ÖØÉèÎÄ¼þ´´½¨ÑÚÄ£ 
	return;
}

#endif

// Hàm để ghi log vào file
void LogToFile(const std::string& message) {
	std::ofstream logFile("dump_creation.log", std::ios_base::app); // Mở file log ở chế độ ghi thêm
	if (logFile.is_open()) {
		logFile << message << std::endl;
	}
}

std::string GetTimestamp() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	std::ostringstream timeStream;
	timeStream << (now->tm_year + 1900)
		<< '-' << (now->tm_mon + 1)
		<< '-' << now->tm_mday
		<< ' ' << now->tm_hour
		<< ':' << now->tm_min
		<< ':' << now->tm_sec;
	return timeStream.str();
}

MINIDUMP_TYPE GetNextMiniDumpType(MINIDUMP_TYPE currentType) {
	switch (currentType) {
	case MiniDumpWithFullMemory:
		return MiniDumpNormal;
	case MiniDumpNormal:
		return MiniDumpWithDataSegs;
	case MiniDumpWithDataSegs:
		return MiniDumpWithHandleData;
	case MiniDumpWithHandleData:
		return MiniDumpWithPrivateReadWriteMemory;
	case MiniDumpWithPrivateReadWriteMemory:
		return MiniDumpScanMemory;
	case MiniDumpScanMemory:
		return MiniDumpFilterMemory;
	default:
		return (MINIDUMP_TYPE)0;
	}
}

void CreateMiniDump(EXCEPTION_POINTERS* pep) {
	// Tạo tên file dump với thời gian hiện tại
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	TCHAR szFileName[MAX_PATH];
	_stprintf_s(szFileName, _T("CrashDump_GameServer-%04d%02d%02d-%02d%02d%02d.dmp"),
		now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
		now->tm_hour, now->tm_min, now->tm_sec);

	// Số lần retry và khoảng thời gian giữa các lần retry (ms)
	const int maxRetries = 5;
	const int retryDelayMs = 1000; // 1 giây

	MINIDUMP_TYPE mdt = MiniDumpWithFullMemory;

	for (int attempt = 1; attempt <= maxRetries; ++attempt) {
		// Tạo file dump
		HANDLE hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
			0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE)) {
			MINIDUMP_EXCEPTION_INFORMATION mdei;
			mdei.ThreadId = GetCurrentThreadId();
			mdei.ExceptionPointers = pep;
			mdei.ClientPointers = FALSE;

			BOOL result = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
				hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0);

			if (result) {
				std::string logMessage = GetTimestamp() + " - Dump file created successfully with type " + std::to_string(mdt) + ": " + szFileName;
				std::cout << logMessage << std::endl;
				LogToFile(logMessage);
				CloseHandle(hFile);
				break; // Thoát khỏi vòng lặp nếu thành công
			}
			else {
				DWORD dwError = GetLastError();
				std::string logMessage = GetTimestamp() + " - MiniDumpWriteDump failed with type " + std::to_string(mdt) + ". Attempt " + std::to_string(attempt) + " of " + std::to_string(maxRetries) + ". Error: " + std::to_string(dwError);
				std::cerr << logMessage << std::endl;
				LogToFile(logMessage);
				CloseHandle(hFile);

				// Thử loại dump tiếp theo nếu gặp lỗi
				mdt = GetNextMiniDumpType(mdt);
				if (mdt == 0) {
					logMessage = GetTimestamp() + " - No more dump types to try.";
					std::cerr << logMessage << std::endl;
					LogToFile(logMessage);
					break;
				}
			}
		}
		else {
			DWORD dwError = GetLastError();
			std::string logMessage = GetTimestamp() + " - CreateFile failed. Attempt " + std::to_string(attempt) + " of " + std::to_string(maxRetries) + ". Error: " + std::to_string(dwError);
			std::cerr << logMessage << std::endl;
			LogToFile(logMessage);
		}

		if (attempt < maxRetries) {
			std::this_thread::sleep_for(std::chrono::milliseconds(retryDelayMs)); // Đợi trước khi thử lại
		}
		else {
			std::string logMessage = GetTimestamp() + " - Max retry attempts reached. Could not create dump file.";
			std::cerr << logMessage << std::endl;
			LogToFile(logMessage);
		}
	}
}

LONG WINAPI UnhandledExceptionFilterCallback(EXCEPTION_POINTERS* pep) {
	CreateMiniDump(pep);
	return EXCEPTION_EXECUTE_HANDLER;
}

INT g_nPort = 0;
INT main(INT argc, char* argv[])
{
	// Đăng ký hàm xử lý ngoại lệ
	//SetUnhandledExceptionFilter(UnhandledExceptionFilterCallback);
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	SetConsoleOutputCP(CP_UTF8);

	//g_FindDebugWindow("#32770", "DebugWin");
#ifdef _DEBUG
	g_FindDebugWindow("#32770", "DebugWin");
#endif

	BOOL bRunning = TRUE;
	if (argc == 2)
	{
		g_nPort = atoi(argv[1]);
	}
#ifdef __linux
	init_daemon();
#endif

	if (!g_SOServer.Init())
		return 0;

	while (bRunning)
	{
		bRunning = g_SOServer.Breathe();
	}

	g_SOServer.Release();
	return 1;
}

#ifdef _STANDALONE
extern "C"
{
	void lua_outerrmsg(const char* szerrmsg)
	{
		fprintf(stderr, szerrmsg);
	}
};
#endif