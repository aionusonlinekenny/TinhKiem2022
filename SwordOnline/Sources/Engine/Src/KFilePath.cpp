//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPath.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	File Path Utility
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include "KFilePath.h"

//#ifndef _SERVER
#include "KPakList.h"
//#endif

#ifndef WIN32
#include <unistd.h>
#else
#include <direct.h>
#endif
#include <string.h>
//---------------------------------------------------------------------------
#ifdef WIN32
static char szRootPath[MAXPATH] = "C:";		// Æô¶¯Â·¾¶
static char szCurrPath[MAXPATH] = "\\";		// µ±Ç°Â·¾¶
#else
static char szRootPath[MAXPATH] = "/";		// Æô¶¯Â·¾¶
static char szCurrPath[MAXPATH] = "/";		// µ±Ç°Â·¾¶
#endif

int RemoveTwoPointPath(LPTSTR szPath, int nLength)
{
	int nRemove = 0;
	KASSERT(szPath);
#ifdef WIN32
	LPCTSTR lpszOld = "\\..\\";
#else
	LPCTSTR lpszOld = "/../";
#endif
	LPTSTR lpszTarget = strstr(szPath, lpszOld);
	if (lpszTarget)
	{
		LPTSTR lpszAfter = lpszTarget + 3;
		while(lpszTarget > szPath)
		{
			lpszTarget--;
			if ((*lpszTarget) == '\\' ||(*lpszTarget) == '/')
				break;
		}
		memmove(lpszTarget, lpszAfter, (nLength - (lpszAfter - szPath) + 1) * sizeof(char));
		nRemove = (lpszAfter - lpszTarget);
		return RemoveTwoPointPath(szPath, nLength - nRemove);
	}

	return nLength - nRemove;
}

int RemoveOnePointPath(LPTSTR szPath, int nLength)
{
	int nRemove = 0;
	KASSERT(szPath);
#ifdef WIN32
	LPCTSTR lpszOld = "\\.\\";
#else
	LPCTSTR lpszOld = "/./";
#endif
	LPTSTR lpszTarget = strstr(szPath, lpszOld);
	if (lpszTarget)
	{
		LPTSTR lpszAfter = lpszTarget + 2;
		memmove(lpszTarget, lpszAfter, (nLength - (lpszAfter - szPath) + 1) * sizeof(char));
		nRemove = (lpszAfter - lpszTarget);
		return RemoveOnePointPath(szPath, nLength - nRemove);
	}

	return nLength - nRemove;
}

int RemoveAllPointPath(LPTSTR szPath, int nLength)
{
	return RemoveOnePointPath(szPath, RemoveTwoPointPath(szPath, nLength));
}

//----------------------------------------------------------------------------
//Function: SetRootPath
//Function: Set the root path of the program
//Parameters: lpPathName path name
//Return: void
//----------------------------------------------------------------------------
ENGINE_API void g_SetRootPath(LPSTR lpPathName)
{
	if (lpPathName)
	{
		//g_DebugLog("set path %s\n", lpPathName);
		g_StrCpy(szRootPath, lpPathName);
	}
	else
	{
//#ifdef WIN32
//		GetCurrentDirectory(MAXPATH, szRootPath);
//#else
//		g_DebugLog("set path NULL\n");
		getcwd(szRootPath, MAXPATH);
//#endif
	}

	// È¥µôÂ·¾¶Ä©Î²µÄ '\'
	int len = g_StrLen(szRootPath);
	//g_DebugLog("set path %s(%d)\n", szRootPath, len);
	if (szRootPath[len - 1] == '\\' || szRootPath[len - 1] == '/')
	{
		szRootPath[len - 1] = 0;
	}
//	g_DebugLog("RootPath = %s", szRootPath);
}
//----------------------------------------------------------------------------
//Function: GetRootPath
//Function: Get the root path of the program
//Parameters: lpPathName path name
//Return: void
//----------------------------------------------------------------------------
ENGINE_API void g_GetRootPath(LPSTR lpPathName)
{
	g_StrCpy(lpPathName, szRootPath);
}
//---------------------------------------------------------------------------
// º¯Êý:	SetFilePath
// ¹¦ÄÜ:	ÉèÖÃµ±Ç°ÎÄ¼þÂ·¾¶
// ²ÎÊý:	lpPathName	Â·¾¶Ãû
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_SetFilePath(LPSTR lpPathName)
{
	// È¥µôÇ°ÃæµÄ "\\"
	if (lpPathName[0] == '\\' ||lpPathName[0] == '/')
	{
		g_StrCpy(szCurrPath, lpPathName + 1);
	}
	else
	{
		g_StrCpy(szCurrPath, lpPathName);
	}

	// Ä©Î²¼ÓÉÏ "\\"
	int len = g_StrLen(szCurrPath);
	if (len > 0 && szCurrPath[len - 1] != '\\' && szCurrPath[len - 1] != '/')
	{
#ifdef WIN32
		szCurrPath[len] = '\\';
#else
		szCurrPath[len] = '/';
#endif
		szCurrPath[len + 1] = 0;
	}
	RemoveAllPointPath(szCurrPath, len + 1);
#ifndef WIN32
	//'\\' -> '/' [wxb 2003-7-29]
	for (len = 0; szCurrPath[len]; len++)
	{
		if (szCurrPath[len] == '\\')
			szCurrPath[len] = '/';
	}
#endif
}
//---------------------------------------------------------------------------
// º¯Êý:	GetFilePath
// ¹¦ÄÜ:	È¡µÃµ±Ç°ÎÄ¼þÂ·¾¶
// ²ÎÊý:	lpPathName	Â·¾¶Ãû
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API	void g_GetFilePath(LPSTR lpPathName)
{
	g_StrCpy(lpPathName, szCurrPath);
}
//---------------------------------------------------------------------------
// º¯Êý:	GetFullPath
// ¹¦ÄÜ:	È¡µÃÎÄ¼þµÄÈ«Â·¾¶Ãû
// ²ÎÊý:	lpPathName	Â·¾¶Ãû
//			lpFileName	ÎÄ¼þÃû
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_GetFullPath(LPSTR lpPathName, LPSTR lpFileName)
{
	// ÎÄ¼þ´øÓÐÈ«Â·¾¶
	if (lpFileName[1] == ':')
	{
		g_StrCpy(lpPathName, lpFileName);
		return;
	}

	// ÎÄ¼þ´øÓÐ²¿·ÖÂ·¾¶
	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		g_StrCpy(lpPathName, szRootPath);
		g_StrCat(lpPathName, lpFileName);
		return;
	}
	
	// µ±Ç°Â·¾¶ÎªÈ«Â·¾¶
#ifdef WIN32
	if (szCurrPath[1] == ':')
	{
		g_StrCpy(lpPathName, szCurrPath);
		g_StrCat(lpPathName, lpFileName);
		return;
	}
#endif
	// µ±Ç°Â·¾¶Îª²¿·ÖÂ·¾¶
	g_StrCpy(lpPathName, szRootPath);
        if(szCurrPath[0] != '\\' && szCurrPath[0] != '/') {
#ifdef WIN32
	g_StrCat(lpPathName, "\\");
#else
	g_StrCat(lpPathName, "/");
#endif
      }
	g_StrCat(lpPathName, szCurrPath);

	if (lpFileName[0] == '.' && (lpFileName[1] == '\\'||lpFileName[1] == '/') )
		g_StrCat(lpPathName, lpFileName + 2);
	else
		g_StrCat(lpPathName, lpFileName);
}
//---------------------------------------------------------------------------
// º¯Êý:	GetHalfPath
// ¹¦ÄÜ:	È¡µÃÎÄ¼þµÄ°ëÂ·¾¶Ãû£¬²»´ø¸ùÂ·¾¶
// ²ÎÊý:	lpPathName	Â·¾¶Ãû
//			lpFileName	ÎÄ¼þÃû
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_GetHalfPath(LPSTR lpPathName, LPSTR lpFileName)
{
	// ÎÄ¼þ´øÓÐ²¿·ÖÂ·¾¶
	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		g_StrCpy(lpPathName, lpFileName);
	}
	else
	{
#ifdef WIN32
		g_StrCpy(lpPathName, "\\");
#else
		g_StrCpy(lpPathName, "/");
#endif
		g_StrCat(lpPathName, szCurrPath);
		g_StrCat(lpPathName, lpFileName);
	}
}
//---------------------------------------------------------------------------
// º¯Êý:	GetPackPath
// ¹¦ÄÜ:	È¡µÃÎÄ¼þÔÚÑ¹Ëõ°üÖÐµÄÂ·¾¶Ãû
// ²ÎÊý:	lpPathName	Â·¾¶Ãû
//			lpFileName	ÎÄ¼þÃû
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_GetPackPath(LPSTR lpPathName, LPSTR lpFileName)
{
	// 文件带有部分路径
	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		g_StrCpy(lpPathName, lpFileName + 1);
	}
	else
	{
		g_StrCpy(lpPathName, szCurrPath);
		g_StrCat(lpPathName, lpFileName);
	}
	int len = g_StrLen(lpPathName);
	RemoveAllPointPath(lpPathName, len + 1);
	// Convert all to lowercase letters
	// g_StrLower(lpPathName);
	_strlwr(lpPathName);
}
//---------------------------------------------------------------------------
// º¯Êý:	GetDiskPath
// ¹¦ÄÜ:	È¡µÃCDROM¶ÔÓ¦µÄÎÄ¼þÂ·¾¶Ãû
// ²ÎÊý:	lpPathName	Â·¾¶Ãû
//			lpFileName	ÎÄ¼þÃû
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_GetDiskPath(LPSTR lpPathName, LPSTR lpFileName)
{
	g_StrCpy(lpPathName, "C:");
	for (int i = 0; i < 24; lpPathName[0]++, i++)
	{
//		if (GetDriveType(lpPathName) == DRIVE_CDROM)
//			break;
	}
	if (lpFileName[0] == '\\' || lpPathName[0] == '/')
	{
		g_StrCat(lpPathName, lpFileName);
	}
	else
	{
#ifdef WIN32
		g_StrCat(lpPathName, "\\");
#else
		g_StrCat(lpPathName, "/");
#endif
		g_StrCat(lpPathName, szCurrPath);
		g_StrCat(lpPathName, lpFileName);
	}
}

//---------------------------------------------------------------------------
// º¯Êý:	CreatePath
// ¹¦ÄÜ:	ÔÚÓÎÏ·¸úÄ¿Â¼ÏÂ½¨Á¢Ò»ÌõÂ·¾¶
// ²ÎÊý:	lpPathName	Â·¾¶Ãû
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API	void	g_CreatePath(LPSTR lpPathName)
{
	if (!lpPathName || !lpPathName[0])
		return;

	char	szFullPath[MAXPATH];
	int		i;
	// ÎÄ¼þ´øÓÐÈ«Â·¾¶
	if (lpPathName[1] == ':')
	{
		if (g_StrLen(lpPathName) < 4)
			return;
		g_StrCpy(szFullPath, lpPathName);
		i = 4;
	}
	else if (lpPathName[0] == '\\' || lpPathName[0] == '/')
	{
		g_StrCpy(szFullPath, szRootPath);
		g_StrCat(szFullPath, lpPathName);
		i = g_StrLen(szRootPath) + 1;
	}
	else
	{
		g_StrCpy(szFullPath, szRootPath);
#ifdef WIN32
		g_StrCat(szFullPath, "\\");
#else
		g_StrCat(szFullPath, "/");
#endif
		g_StrCat(szFullPath, lpPathName);
                
		i = g_StrLen(szRootPath) + 1;
	}

	for (; i < g_StrLen(szFullPath); i++)
	{
#ifdef WIN32
		if (szFullPath[i] == '\\') {
			szFullPath[i] = 0;
			CreateDirectory(szFullPath, NULL);
			szFullPath[i] = '\\';
		}
#else
		if (szFullPath[i] == '/') {
			szFullPath[i] = 0;
			szFullPath[i] = '/';
		}
#endif
	}
#ifdef WIN32
	CreateDirectory(szFullPath, NULL);
#else
	// flying comment
	// Öì´«¾¸ÊµÏÖÕâ¸öµ÷ÓÃ
	//mkdir();
#endif
}

//---------------------------------------------------------------------------
// º¯Êý:	g_UnitePathAndName
// ¹¦ÄÜ:	Ò»¸öÂ·¾¶ºÍÒ»¸öÎÄ¼þÃû£¬ºÏ²¢µ½lpGetÖÐÐÎ³ÉÒ»¸öÍêÕûµÄÂ·¾¶ÎÄ¼þÃû
// ²ÎÊý:	lpPath ´«ÈëÂ·¾¶Ãû lpFile ´«ÈëÎÄ¼þÃû lpGet »ñµÃµÄ×îÖÕÍêÕûÎÄ¼þÃû
// ·µ»Ø:	void
// ×¢Òâ£º   ÕâÀïÃ»ÓÐ¿¼ÂÇ×Ö·û´®µÄ³¤¶È£¬Ê¹ÓÃµÄÊ±ºòÒª±£Ö¤×Ö·û´®µÄ³¤¶È×ã¹»
//---------------------------------------------------------------------------
ENGINE_API	void	g_UnitePathAndName(char *lpPath, char *lpFile, char *lpGet)
{
	if (!lpPath || !lpFile || !lpGet)
		return;
	strcpy(lpGet, lpPath);
	int	nSize = strlen(lpGet);
	if (lpGet[nSize] - 1 != '\\')
	{
		lpGet[nSize] = '\\';
		lpGet[nSize + 1] = 0;
	}
	if (lpFile[0] != '\\')
	{
		strcat(lpGet, lpFile);
	}
	else
	{
		strcat(lpGet, &lpFile[1]);
	}
}


//---------------------------------------------------------------------------
// º¯Êý:	find if file exists in pak or in hard disk
// ¹¦ÄÜ:	·µ»ØÖ¸¶¨µÄÎÄ¼þÊÇ·ñ´æÔÚ
// ²ÎÊý:	lpPathName	Â·¾¶Ãû£«ÎÄ¼þÃû
// ·µ»Ø:	TRUE£­³É¹¦£¬FALSE£­Ê§°Ü¡£
//---------------------------------------------------------------------------
ENGINE_API BOOL g_FileExists(LPSTR FileName)
{
	BOOL	bExist = FALSE;
	char	szFullName[MAX_PATH];
	if (FileName && FileName[0])
	{
#ifndef _SERVER
		//ÏÈ²éÊÇÊÇ·ñÔÚ´ò°üÎÄ¼þÖÐ
		if (g_pPakList)
		{
			XPackElemFileRef	PackRef;
			bExist = g_pPakList->FindElemFile(FileName, PackRef);
		}
#endif
		//ÔÚ¼ì²éÊÇ·ñµ¥¶À´æÔÚÎÄ¼þÏµÍ³Àï
		if (bExist == FALSE)
		{
			#ifdef	WIN32
				g_GetFullPath(szFullName, FileName);
				bExist = !(GetFileAttributes(szFullName) & FILE_ATTRIBUTE_DIRECTORY);// || dword == INVALID_FILE_ATTRIBUTES)
			#endif
		}
	}
	return bExist;
}
//---------------------------------------------------------------------------
// º¯Êý:	File Name to 32bit Id
// ¹¦ÄÜ:	ÎÄ¼þÃû×ª»»³É Hash 32bit ID
// ²ÎÊý:	lpFileName	ÎÄ¼þÃû
// ·µ»Ø:	FileName Hash 32bit ID
// 
// ×¢Òâ:	ÓÎÏ·ÊÀ½çºÍÖ÷Íø¹Ø½»»¥Êý¾ÝËùÓÃµÄ¹þÏ£²éÕÒË÷ÒýÒ²ÊÇÓÃ
//			µÄÕâ¸öº¯Êý£¬ËùÒÔÇëÐÞ¸ÄÕâ¸öº¯ÊýÊ±Ò²¶ÔÓ¦ÐÞ¸ÄÖ÷Íø¹Ü
//			ÖÐÏà¶ÔÓ¦µÄÄÇ¸öº¯Êý¡£Õâ¸öº¯Êý´æÔÚÓÚCommon.lib¹¤³ÌµÄUtils.h
//			ÖÐ£¬º¯ÊýÉùÃ÷Îª DWORD HashStr2ID( const char * const pStr );
//---------------------------------------------------------------------------
ENGINE_API DWORD g_FileName2Id(LPSTR lpFileName)
{
	if (lpFileName)
	{
		DWORD Id = 0;
		char c = 0;
		for (int i = 0; lpFileName[i]; i++)
		{
			c = lpFileName[i];
#ifndef WIN32
			//for linux path looking up
			if ('/' == c)
				c = '\\';
#endif
			Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef;
		}
		return (Id ^ 0x12345678);
	}

	return (DWORD)(-1);
}

ENGINE_API DWORD Name2ID(LPSTR lpFileName)
{
	return g_FileName2Id(lpFileName);
}
//---------------------------------------------------------------------------
// º¯Êý:	change file extention
// ¹¦ÄÜ:	¸Ä±äÎÄ¼þµÄÀ©Õ¹Ãû
// ²ÎÊý:	lpFileName	ÎÄ¼þÃû
//			lpNewExt	ÐÂÀ©Õ¹Ãû£¬²»ÄÜÓÐ'.'
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_ChangeFileExt(LPSTR lpFileName, LPSTR lpNewExt)
{
	int  i;

	for (i = 0; lpFileName[i]; i++)
	{
		if (lpFileName[i] == '.')
			break;
	}
	
	if (lpFileName[i] == '.')
	{
		g_StrCpy(&lpFileName[i + 1], lpNewExt);
	}
	else
	{
		g_StrCat(lpFileName, ".");
		g_StrCat(lpFileName, lpNewExt);
	}
}
//---------------------------------------------------------------------------
// º¯Êý:	Extract File Name from path name
// ¹¦ÄÜ:	È¡µÃÎÄ¼þÃû£¨²»°üº¬Â·¾¶£©
// ²ÎÊý:	lpFileName	ÎÄ¼þÃû£¨²»°üº¬Â·¾¶£©
//			lpFilePath	ÎÄ¼þÃû£¨°üº¬Â·¾¶£©
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_ExtractFileName(LPSTR lpFileName, LPSTR lpFilePath)
{
	int nLen = g_StrLen(lpFilePath);
	if (nLen < 5)
		return;
	int nPos = nLen;
	while (--nPos > 0)
	{
		if (lpFilePath[nPos] == '\\'||lpFilePath[nPos] == '/')
			break;
	}
	g_StrCpy(lpFileName, &lpFilePath[nPos + 1]);
}
//---------------------------------------------------------------------------
// º¯Êý:	Extract File Path from path name
// ¹¦ÄÜ:	È¡µÃÂ·¾¶Ãû
// ²ÎÊý:	lpFileName	Â·¾¶Ãû
//			lpFilePath	Â·¾¶Ãû£«ÎÄ¼þÃû
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_ExtractFilePath(LPSTR lpPathName, LPSTR lpFilePath)
{
	int nLen = g_StrLen(lpFilePath);
	if (nLen < 5)
		return;
	int nPos = nLen;
	while (--nPos > 0)
	{
		if (lpFilePath[nPos] == '\\' ||lpFilePath[nPos] == '/')
			break;
	}
	g_StrCpyLen(lpPathName, lpFilePath, nPos);
}
//---------------------------------------------------------------------------
