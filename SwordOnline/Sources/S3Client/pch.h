#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>					// Windows Header Files
#ifndef _WIN32_WINNT
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#define _WIN32_WINNT 0x0600					// Win 2k8, Vista
#define _WIN32_WINNT_WIN7 0x0601			// Windows 7
#define _WIN32_WINNT_WIN8  0x0602			// Windows 8
#define _WIN32_WINNT_WINBLUE 0x0603			// Windows 8.1
#define _WIN32_WINNT_WINTHRESHOLD 0x0A00	// Windows 10
#define _WIN32_WINNT_WIN10 0x0A00			// Windows 10
#endif
#endif

#ifdef MAKING_DLL
#define FOO_API __declspec(dllexport)
#else
#define FOO_API
#endif

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#define CORE_API_C extern "C" __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#define CORE_API_C extern "C" __declspec(dllimport)
#endif

#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NEXT_RESOURCE_VALUE	1000
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		1000
#define _APS_NEXT_COMMAND_VALUE		32771
#endif
#endif

#if !defined(AFX_STDAFX_H__3BC60975_014C_4318_99D7_EFC8C5B76DE4__INCLUDED_)
#define AFX_STDAFX_H__3BC60975_014C_4318_99D7_EFC8C5B76DE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers


#ifndef _AFX_NO_OLE_SUPPORT
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#endif // _AFX_NO_AFXCMN_SUPPORT

#endif // !defined(AFX_STDAFX_H__3BC60975_014C_4318_99D7_EFC8C5B76DE4__INCLUDED_)
