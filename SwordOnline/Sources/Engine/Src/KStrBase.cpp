//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KStrBase.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	String Utility Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include <string.h>

ENGINE_API int g_StrLen(LPCSTR lpStr)
{
#ifdef WIN32
	register int nLen;

	__asm
	{
		mov		edi, lpStr
		mov		ecx, 0xffffffff
		xor		al, al
		repne	scasb
		not		ecx
		dec		ecx
		mov		nLen, ecx
	}
	return nLen;
#else
     return strlen(lpStr);
#endif
}
//---------------------------------------------------------------------------
// º¯Êý:	StrEnd
// ¹¦ÄÜ:	·µ»Ø×Ö·û´®½áÎ²Ö¸Õë
// ²ÎÊý:	lpStr	:	×Ö·û´®¿ªÍ·µÄÖ¸Õë
// ·µ»Ø:	lpEnd	:	×Ö·û´®Ä©Î²µÄÖ¸Õë
//---------------------------------------------------------------------------
ENGINE_API LPSTR g_StrEnd(LPCSTR lpStr)
{
#ifdef WIN32
	register LPSTR lpEnd;

	__asm
	{
		mov		edi, lpStr
		mov		ecx, 0xffffffff
		xor		al, al
		repne	scasb
		lea		eax, [edi - 1]
		mov		lpEnd, eax
	}
	return lpEnd;
#else
     return (char *)lpStr + strlen(lpStr);
#endif
}
//---------------------------------------------------------------------------
// º¯Êý:	StrCpy
// ¹¦ÄÜ:	×Ö·û´®¿½±´
// ²ÎÊý:	lpDest	:	Ä¿±ê×Ö·û´®
//			lpSrc	:	Ô´×Ö·û´®
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_StrCpy(LPSTR lpDest, LPCSTR lpSrc)
{
#ifdef WIN32
	__asm
	{
		mov		edi, lpSrc
		mov		ecx, 0xffffffff
		xor		al, al
		repne	scasb
		not		ecx
		mov		edi, lpDest
		mov		esi, lpSrc
		mov		edx, ecx
		shr		ecx, 2
		rep		movsd
		mov		ecx, edx
		and		ecx, 3
		rep		movsb
	};
#else
    strcpy(lpDest, lpSrc);
#endif
}
//---------------------------------------------------------------------------
// º¯Êý:	StrCpyLen
// ¹¦ÄÜ:	×Ö·û´®¿½±´,ÓÐ×î´ó³¤¶ÈÏÞÖÆ
// ²ÎÊý:	lpDest	:	Ä¿±ê×Ö·û´®
//			lpSrc	:	Ô´×Ö·û´®
//			nMaxLen	:	×î´ó³¤¶È
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_StrCpyLen(LPSTR lpDest, LPCSTR lpSrc, int nMaxLen)
{
#ifdef WIN32
	__asm
	{
		xor		al, al
		mov		edx, nMaxLen
		dec		edx
		jg		copy_section

		jl		finished
		mov		edi, lpDest
		stosb
		jmp		finished

copy_section:
		mov		edi, lpSrc
		mov		ecx, 0xffffffff
		repne	scasb
		not		ecx
		dec		ecx
		cmp		ecx, edx
		jle		loc_little_equal
		mov		ecx, edx

loc_little_equal:

		mov		edi, lpDest
		mov		esi, lpSrc
		mov		edx, ecx
		shr		ecx, 2
		rep		movsd
		mov		ecx, edx
		and		ecx, 3
		rep		movsb
		stosb
		
finished:
	};
#else
    strncpy(lpDest, lpSrc, nMaxLen);
#endif
}
//---------------------------------------------------------------------------
// º¯Êý:	StrCat
// ¹¦ÄÜ:	×Ö·û´®Ä©Î²×·¼ÓÁíÒ»¸ö×Ö·û´®
// ²ÎÊý:	lpDest	:	Ä¿±ê×Ö·û´®
//			lpSrc	:	Ô´×Ö·û´®
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_StrCat(LPSTR lpDest, LPCSTR lpSrc)
{
	register LPSTR lpEnd;

	lpEnd = g_StrEnd(lpDest);
	g_StrCpy(lpEnd, lpSrc);
}
//---------------------------------------------------------------------------
// º¯Êý:	StrCatLen
// ¹¦ÄÜ:	×Ö·û´®Ä©Î²×·¼ÓÁíÒ»¸ö×Ö·û´®,ÓÐ×î´ó³¤¶ÈÏÞÖÆ
// ²ÎÊý:	lpDest	:	Ä¿±ê×Ö·û´®
//			lpSrc	:	Ô´×Ö·û´®
//			nMaxLen	:	×î´ó³¤¶È
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_StrCatLen(LPSTR lpDest, LPCSTR lpSrc, int nMaxLen)
{
	register LPSTR lpEnd;

	lpEnd = g_StrEnd(lpDest);
	g_StrCpyLen(lpEnd, lpSrc, nMaxLen);
}
//---------------------------------------------------------------------------
// º¯Êý:	StrCmp
// ¹¦ÄÜ:	×Ö·û´®±È½Ï
// ²ÎÊý:	lpDest	:	×Ö·û´®1	
//			lpSrc	:	×Ö·û´®2
// ·µ»Ø:	TRUE	:	ÏàÍ¬
//			FALSE	:	²»Í¬
//---------------------------------------------------------------------------
ENGINE_API BOOL g_StrCmp(LPCSTR lpDest, LPCSTR lpSrc)
{
	register int nLen1, nLen2;

	nLen1 = g_StrLen(lpDest);
	nLen2 = g_StrLen(lpSrc);
	if (nLen1 != nLen2)
		return FALSE;
	return g_MemComp((void*)lpDest, (void*)lpSrc, nLen1);
}
//---------------------------------------------------------------------------
// º¯Êý:	StrCmpLen
// ¹¦ÄÜ:	×Ö·û´®±È½Ï,ÏÞ¶¨³¤¶È
// ²ÎÊý:	lpDest	:	×Ö·û´®1	
//			lpSrc	:	×Ö·û´®2
//			nLen	:	³¤¶È
// ·µ»Ø:	TRUE	:	ÏàÍ¬
//			FALSE	:	²»Í¬
//---------------------------------------------------------------------------
ENGINE_API BOOL g_StrCmpLen(LPCSTR lpDest, LPCSTR lpSrc, int nMaxLen)
{
	register int nLen1, nLen2;

	nLen1 = g_StrLen(lpDest);
	nLen2 = g_StrLen(lpSrc);
	if (nMaxLen > nLen1)
		nMaxLen = nLen1;
	if (nMaxLen > nLen2)
		nMaxLen = nLen2;
	return g_MemComp((void*)lpDest, (void*)lpSrc, nMaxLen);
}
//---------------------------------------------------------------------------
// º¯Êý:	StrUpper
// ¹¦ÄÜ:	Ð¡Ð´×ÖÄ¸×ª´óÐ´×ÖÄ¸
// ²ÎÊý:	lpDest	:	×Ö·û´®
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_StrUpper(LPSTR lpDest)
{
#ifdef WIN32
	__asm
	{
		mov		esi, lpDest
loc_lodsb:
		lodsb
		or		al, al
		je		loc_exit
		cmp		al, 'a'
		jb		loc_lodsb
		cmp		al, 'z'
		ja		loc_lodsb
		sub		al, 0x20
		mov		[esi - 1], al
		jmp		loc_lodsb
loc_exit:
	}
#else
     char *ptr = lpDest;
     while(*ptr) {
         if(*ptr >= 'a' && *ptr <= 'z') *ptr += 'A' - 'a';
//          *ptr = toupper(*ptr);
          ptr++;
     }
#endif
}
//---------------------------------------------------------------------------
// º¯Êý:	StrLower
// ¹¦ÄÜ:	´óÐ´×ÖÄ¸×ªÐ¡Ð´×ÖÄ¸
// ²ÎÊý:	lpDest	:	×Ö·û´®
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_StrLower(LPSTR lpDest)
{
#ifdef WIN32
	__asm
	{
		mov		esi, lpDest
loc_lodsb:
		lodsb
		or		al, al
		je		loc_exit
		cmp		al, 'A'
		jb		loc_lodsb
		cmp		al, 'Z'
		ja		loc_lodsb
		add		al, 0x20
		mov		[esi - 1], al
		jmp		loc_lodsb
loc_exit:
	}
#else
     char *ptr = lpDest;
     while(*ptr) {
         if(*ptr >= 'A' && *ptr <= 'Z') *ptr += 'a' - 'A';
//          *ptr = tolower(*ptr);
          ptr++;
     }
#endif
}
//---------------------------------------------------------------------------
ENGINE_API void g_StrRep(LPSTR lpDest, LPSTR lpSrc, LPSTR lpRep)
{
	int		nSrcLen = g_StrLen(lpSrc);
	int		nDestLen = g_StrLen(lpDest);
	int		nMaxLen = nDestLen - nSrcLen + g_StrLen(lpRep) + 1;
	char	*pStart = NULL;
        int i;
	for (i = 0; i < nDestLen - nSrcLen; i++)
	{
		if (g_StrCmpLen(&lpDest[i], lpSrc, nSrcLen))
			break;
	}
	if (i == nDestLen - nSrcLen)
		return;

	pStart = new char[nMaxLen];

	if (i != 0)
	{
		g_StrCpyLen(pStart, lpDest, i);
		g_StrCat(pStart, lpRep);
		g_StrCat(pStart, &lpDest[i + nSrcLen]);
	}
	else
	{
		g_StrCpy(pStart, lpRep);
		g_StrCat(pStart, &lpDest[nSrcLen]);
	}
	g_StrCpy(lpDest, pStart);
	if (pStart)
	{
		delete [] pStart;
		pStart = NULL;
	}
}

ENGINE_API std::string GbkToUtf8(const char* src_str)
{
	//ASSERT(src_str!=NULL);
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
	/*
	ASSERT(gb2312!=NULL);
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];//需要在外面析构，可以改成传指针进来的方式
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
	*/
}

ENGINE_API const char* Utf8ToGbk(const char* src_str, char* pcDes)
{
	/*int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
	*/
	enum { GB2312 = 936 };
	//unsigned long len;
	int len = -1;
	len = ::MultiByteToWideChar(CP_UTF8, NULL, src_str, -1, NULL, NULL);
	if (len == 0)
		return NULL;
	wchar_t* wide_char_buffer = new wchar_t[len];
	::MultiByteToWideChar(CP_UTF8, NULL, src_str, -1, wide_char_buffer, len);
	len = ::WideCharToMultiByte(GB2312, NULL, wide_char_buffer, -1, NULL, NULL, NULL, NULL);
	if (len == 0)
	{
		if (wide_char_buffer)
			delete[] wide_char_buffer;
		return NULL;
	}
	char* multi_byte_buffer = new char[len];
	::WideCharToMultiByte(GB2312, NULL, wide_char_buffer, -1, multi_byte_buffer, len, NULL, NULL);
	strcpy(pcDes, multi_byte_buffer);
	//std::string dest(multi_byte_buffer);
	if (multi_byte_buffer) delete[] multi_byte_buffer;
	if (wide_char_buffer) delete[] wide_char_buffer;

	return pcDes;
}