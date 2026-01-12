// Bishop.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "Application.h"

#include "Macro.h"
#include <minidumpapiset.h>

LONG WINAPI UnhandledExceptionFilterCallback(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	// Tạo tên file dựa trên thời gian
	TCHAR szDumpFileName[MAX_PATH];
	_stprintf_s(szDumpFileName, MAX_PATH, _T("CrashDump_Bishop_%04d%02d%02d_%02d%02d%02d.dmp"),
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

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR     lpCmdLine, INT       nCmdShow)
{
	// Đăng ký hàm xử lý ngoại lệ
	SetUnhandledExceptionFilter(UnhandledExceptionFilterCallback);
	/*
	 * Add this funtion by liupeng on 2003
	 * We can find some error when start a console tracer
	 *
	 * Please check 'project-setting'
	 */
#ifdef	CONSOLE_DEBUG

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

	cprintf("Startup application!\n");

#endif

	CBishopApp app(hInstance);

	INT nRet = app.Run();

#ifdef CONSOLE_DEBUG

	cprintf("End of application!\n");

	if (bOpenTracer)
	{
		FreeConsole();
	}

#endif

	return nRet;
}



