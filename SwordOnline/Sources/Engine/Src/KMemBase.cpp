//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMemBase.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Memory base functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KNode.h"
#include "KList.h"
#include "KMemBase.h"
#include <string.h>
#include <cstdint>
//---------------------------------------------------------------------------
class KMemNode : public KNode
{
public:
	DWORD	m_dwMemSize;//ÄÚ´æ´óÐ¡
	DWORD	m_dwMemSign;//ÄÚ´æ±êÖ¾
};
//---------------------------------------------------------------------------
class KMemList : public KList
{
public:
	~KMemList()
	{
		KMemNode* pNode = (KMemNode*)GetHead();
		while (pNode)
		{
			//			g_DebugLog("KMemList::Leak Detected, Size = %d", pNode->m_dwMemSize);
			pNode = (KMemNode*)pNode->GetNext();
		}
	};
	void ShowUsage()
	{
		KMemNode* pNode = (KMemNode*)GetHead();
		DWORD dwMemSize = 0;
		while (pNode)
		{
			dwMemSize += pNode->m_dwMemSize;
			pNode = (KMemNode*)pNode->GetNext();
		}
		g_DebugLog("Memory Usage Size = %d KB", dwMemSize / 1024);
	}
};
static KMemList m_MemList;
//---------------------------------------------------------------------------
#define MEMSIGN 1234567890
//---------------------------------------------------------------------------
// º¯Êý:	g_MemInfo
// ¹¦ÄÜ:	Memory Infomation
// ²ÎÊý:	void
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemInfo()
{
	//	MEMORYSTATUS stat;

	//	GlobalMemoryStatus(&stat);

	//	g_DebugLog("Total Physical Memory = %d MB", stat.dwTotalPhys >> 20);
	//	g_DebugLog("Total Virtual Memory = %d MB", stat.dwTotalVirtual >> 20);
	//	g_DebugLog("%d percent of memory is in use.", stat.dwMemoryLoad);
}
//---------------------------------------------------------------------------
// º¯Êý:	g_MemAlloc
// ¹¦ÄÜ:	·ÖÅäÄÚ´æ
// ²ÎÊý:	dwSize		ÄÚ´æ¿é´óÐ¡
// ·µ»Ø:	lpMem (lpMem = NULL ±íÊ¾·ÖÅäÊ§°Ü)
//---------------------------------------------------------------------------
ENGINE_API LPVOID g_MemAlloc(DWORD dwSize)
{
	//	HANDLE hHeap = GetProcessHeap();
	PBYTE  lpMem = NULL;
	DWORD  dwHeapSize = dwSize + sizeof(KMemNode);

	//	lpMem = (PBYTE)HeapAlloc(hHeap, 0, dwHeapSize);
	lpMem = (PBYTE)new char[dwHeapSize];
	if (NULL == lpMem)
	{
		g_MessageBox("g_MemAlloc() Failed, Size = %d", dwSize);
		return NULL;
	}

	KMemNode* pNode = (KMemNode*)lpMem;
	pNode->m_pPrev = NULL;
	pNode->m_pNext = NULL;
	pNode->m_dwMemSize = dwSize;
	pNode->m_dwMemSign = MEMSIGN;
	m_MemList.AddHead(pNode);

	return (lpMem + sizeof(KMemNode));
	//	return 0;
}
//---------------------------------------------------------------------------
// º¯Êý:	g_MemFree
// ¹¦ÄÜ:	ÊÍ·ÅÄÚ´æ
// ²ÎÊý:	lpMem		ÒªÊÍ·ÅµÄÄÚ´æÖ¸Õë
// ·µ»Ø:	void
//---------------------------------------------------------------------------
ENGINE_API void g_MemFree(LPVOID lpMem)
{
	//	HANDLE hHeap = GetProcessHeap();
	if (lpMem == NULL)
		return;
	lpMem = (PBYTE)lpMem - sizeof(KMemNode);
	KMemNode* pNode = (KMemNode*)lpMem;
	if (pNode->m_dwMemSign != MEMSIGN)
	{
		g_MessageBox("g_MemFree() Failed, Size = %d", pNode->m_dwMemSize);
		return;
	}
	pNode->Remove();
	//	HeapFree(hHeap, 0, lpMem);
	delete[] lpMem;
}

//---------------------------------------------------------------------------
// º¯Êý:	MemoryCopy
// ¹¦ÄÜ:	ÄÚ´æ¿½±´
// ²ÎÊý:	lpDest	:	Ä¿±êÄÚ´æ¿é
//			lpSrc	:	Ô´ÄÚ´æ¿é
//			dwLen	:	¿½±´³¤¶È
// ·µ»Ø:	void
//---------------------------------------------------------------------------
//ENGINE_API void g_MemCopy(PVOID lpDest, PVOID lpSrc, DWORD dwLen)
//{	
//#ifdef WIN32
//	__asm
//	{
//		mov		edi, lpDest
//		mov		esi, lpSrc
//		mov		ecx, dwLen
//		mov     ebx, ecx
//		shr     ecx, 2
//		rep     movsd
//		mov     ecx, ebx
//		and     ecx, 3
//		rep     movsb
//	}
//#else
//     memcpy(lpDest, lpSrc, dwLen);
//#endif
//}


ENGINE_API void g_MemCopy(LPVOID lpDest, LPVOID lpSrc, DWORD dwLen)
{
#ifdef WIN32
	uint8_t* dest = static_cast<uint8_t*>(lpDest);
	const uint8_t* src = static_cast<const uint8_t*>(lpSrc);

	DWORD dwCount = dwLen / 4;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*reinterpret_cast<uint32_t*>(dest) = *reinterpret_cast<const uint32_t*>(src);
		dest += 4;
		src += 4;
	}

	dwCount = dwLen % 4;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*dest = *src;
		++dest;
		++src;
	}
#else
	std::memcpy(lpDest, lpSrc, dwLen);
#endif
}
//---------------------------------------------------------------------------
// º¯Êý:	MemoryCopyMmx
// ¹¦ÄÜ:	ÄÚ´æ¿½±´£¨MMX°æ±¾£©
// ²ÎÊý:	lpDest	:	Ä¿±êÄÚ´æ¿é
//			lpSrc	:	Ô´ÄÚ´æ¿é
//			dwLen	:	¿½±´³¤¶È
// ·µ»Ø:	void
//---------------------------------------------------------------------------

//ENGINE_API void g_MemCopyMmx(PVOID lpDest, PVOID lpSrc, DWORD dwLen)
//{
//#ifdef WIN32
//	__asm
//	{
//		mov		edi, lpDest
//		mov		esi, lpSrc
//		mov		ecx, dwLen
//		mov     ebx, ecx
//		shr     ecx, 3
//		jcxz	loc_copy_mmx2
//
//		loc_copy_mmx1 :
//
//		movq	mm0, [esi]
//			add		esi, 8
//			movq[edi], mm0
//			add		edi, 8
//			dec		ecx
//			jnz		loc_copy_mmx1
//
//			loc_copy_mmx2 :
//
//		mov     ecx, ebx
//			and ecx, 7
//			rep     movsb
//			emms
//	}
//#else
//	memcpy(lpDest, lpSrc, dwLen);
//#endif
//}

/*
Trong phiên bản này, chúng tôi sử dụng một vòng lặp để thực hiện sao chép các quadword (8-byte) khi mã nguồn và đích đều được căn chỉnh 8-byte.
Sau đó, chúng tôi sử dụng một vòng lặp khác để sao chép phần còn lại của dữ liệu,
nếu có, một byte mỗi lần. Nếu bạn sử dụng một kiến trúc khác nhau hoặc cần sửa đổi định dạng dữ liệu,
bạn có thể cần điều chỉnh mã nguồn tùy thuộc vào điều kiện cụ thể của bạn.
*/


ENGINE_API void g_MemCopyMmx(LPVOID lpDest, LPVOID lpSrc, DWORD dwLen)
{
#ifdef WIN32
	uint8_t* dest = static_cast<uint8_t*>(lpDest);
	const uint8_t* src = static_cast<const uint8_t*>(lpSrc);

	DWORD dwCount = dwLen / 8;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*reinterpret_cast<uint64_t*>(dest) = *reinterpret_cast<const uint64_t*>(src);
		dest += 8;
		src += 8;
	}

	dwCount = dwLen % 8;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*dest = *src;
		++dest;
		++src;
	}
#else
	std::memcpy(lpDest, lpSrc, dwLen);
#endif
}

//---------------------------------------------------------------------------
// º¯Êý:	MemoryComp
// ¹¦ÄÜ:	ÄÚ´æ±È½Ï
// ²ÎÊý:	lpDest	:	ÄÚ´æ¿é1
//			lpSrc	:	ÄÚ´æ¿é2
//			dwLen	:	±È½Ï³¤¶È
// ·µ»Ø:	TRUE	:	ÏàÍ¬
//			FALSE	:	²»Í¬	
//---------------------------------------------------------------------------

//ENGINE_API BOOL g_MemComp(PVOID lpDest, PVOID lpSrc, DWORD dwLen)
//{
//#ifdef WIN32
//	__asm
//	{
//		mov		edi, lpDest
//		mov		esi, lpSrc
//		mov		ecx, dwLen
//		mov     ebx, ecx
//		shr     ecx, 2
//		rep     cmpsd
//		jne		loc_not_equal
//		mov     ecx, ebx
//		and ecx, 3
//		rep     cmpsb
//		jne		loc_not_equal
//	};
//	return TRUE;
//
//loc_not_equal:
//
//	return FALSE;
//#else
//	return (0 == memcmp(lpDest, lpSrc, dwLen));
//#endif
//}

/*
Trong phiên bản này, chúng tôi sử dụng một vòng lặp để so sánh từng quadword (4-byte) khi mã nguồn và đích đều được căn chỉnh 4-byte.
Sau đó, chúng tôi sử dụng một vòng lặp khác để so sánh phần còn lại của dữ liệu,
nếu có, một byte mỗi lần. Nếu bạn sử dụng một kiến trúc khác nhau hoặc cần sửa đổi định dạng dữ liệu,
bạn có thể cần điều chỉnh mã nguồn tùy thuộc vào điều kiện cụ thể của bạn.
*/


ENGINE_API BOOL g_MemComp(LPVOID lpDest, LPVOID lpSrc, DWORD dwLen)
{
#ifdef WIN32
	uint8_t* dest = static_cast<uint8_t*>(lpDest);
	const uint8_t* src = static_cast<const uint8_t*>(lpSrc);

	DWORD dwCount = dwLen / 4;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		if (*reinterpret_cast<uint32_t*>(dest) != *reinterpret_cast<const uint32_t*>(src))
			return FALSE;

		dest += 4;
		src += 4;
	}

	dwCount = dwLen % 4;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		if (*dest != *src)
			return FALSE;

		++dest;
		++src;
	}

	return TRUE;
#else
	return (0 == memcmp(lpDest, lpSrc, dwLen));
#endif
}

//---------------------------------------------------------------------------
// º¯Êý:	MemoryFill
// ¹¦ÄÜ:	ÄÚ´æÌî³ä
// ²ÎÊý:	lpDest	:	ÄÚ´æµØÖ·
//			dwLen	:	ÄÚ´æ³¤¶È
//			byFill	:	Ìî³ä×Ö½Ú
// ·µ»Ø:	void
//---------------------------------------------------------------------------

//ENGINE_API void g_MemFill(PVOID lpDest, DWORD dwLen, BYTE byFill)
//{
//#ifdef WIN32
//	__asm
//	{
//		mov		edi, lpDest
//		mov		ecx, dwLen
//		mov		al, byFill
//		mov		ah, al
//		mov		bx, ax
//		shl		eax, 16
//		mov		ax, bx
//		mov		ebx, ecx
//		shr		ecx, 2
//		rep     stosd
//		mov     ecx, ebx
//		and ecx, 3
//		rep     stosb
//	}
//#else
//	memset(lpDest, byFill, dwLen);
//#endif
//}

/*
Trong phiên bản này, chúng tôi sử dụng một vòng lặp để điền giá trị fillValue (4-byte) vào địa chỉ đích khi mã nguồn và đích đều được căn chỉnh 4-byte.
Sau đó, chúng tôi sử dụng một vòng lặp khác để điền giá trị byFill vào phần còn lại của dữ liệu, nếu có, một byte mỗi lần.
Nếu bạn sử dụng một kiến trúc khác nhau hoặc cần sửa đổi định dạng dữ liệu, bạn có thể cần điều chỉnh mã nguồn tùy thuộc vào điều kiện cụ thể của bạn.
*/


ENGINE_API void g_MemFill(LPVOID lpDest, DWORD dwLen, BYTE byFill)
{
#ifdef WIN32
	uint8_t* dest = static_cast<uint8_t*>(lpDest);
	uint32_t fillValue = (static_cast<uint32_t>(byFill) << 24) |
		(static_cast<uint32_t>(byFill) << 16) |
		(static_cast<uint32_t>(byFill) << 8) |
		static_cast<uint32_t>(byFill);

	DWORD dwCount = dwLen / 4;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*reinterpret_cast<uint32_t*>(dest) = fillValue;
		dest += 4;
	}

	dwCount = dwLen % 4;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*dest = byFill;
		++dest;
	}
#else
	memset(lpDest, byFill, dwLen);
#endif
}


//---------------------------------------------------------------------------
// º¯Êý:	MemoryFill
// ¹¦ÄÜ:	ÄÚ´æÌî³ä
// ²ÎÊý:	lpDest	:	ÄÚ´æµØÖ·
//			dwLen	:	ÄÚ´æ³¤¶È
//			wFill	:	Ìî³ä×Ö
// ·µ»Ø:	void
//---------------------------------------------------------------------------

//ENGINE_API void g_MemFill(PVOID lpDest, DWORD dwLen, WORD wFill)
//{
//#ifdef WIN32
//	__asm
//	{
//		mov		edi, lpDest
//		mov		ecx, dwLen
//		mov		ax, wFill
//		mov		bx, ax
//		shl		eax, 16
//		mov		ax, bx
//		mov		ebx, ecx
//		shr		ecx, 1
//		rep     stosd
//		mov     ecx, ebx
//		and ecx, 1
//		rep     stosw
//	}
//#else
//	memset(lpDest, wFill & 0xff, dwLen);
//#endif
//}

/*
Trong phiên bản này, chúng tôi sử dụng một vòng lặp để điền giá trị fillValue (4-byte) vào địa chỉ đích khi mã nguồn và đích đều được căn chỉnh 4-byte.
Sau đó, chúng tôi sử dụng một vòng lặp khác để điền giá trị wFill vào phần còn lại của dữ liệu, nếu có, hai byte mỗi lần.
Nếu bạn sử dụng một kiến trúc khác nhau hoặc cần sửa đổi định dạng dữ liệu, bạn có thể cần điều chỉnh mã nguồn tùy thuộc vào điều kiện cụ thể của bạn.
*/


ENGINE_API void g_MemFill(LPVOID lpDest, DWORD dwLen, WORD wFill)
{
#ifdef WIN32
	uint8_t* dest = static_cast<uint8_t*>(lpDest);
	uint32_t fillValue = (static_cast<uint32_t>(wFill) << 16) | static_cast<uint32_t>(wFill);

	DWORD dwCount = dwLen / 2;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*reinterpret_cast<uint32_t*>(dest) = fillValue;
		dest += 4;
	}

	dwCount = dwLen % 2;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*reinterpret_cast<uint16_t*>(dest) = wFill;
		dest += 2;
	}
#else
	memset(lpDest, wFill & 0xFF, dwLen);
#endif
}


//---------------------------------------------------------------------------
// º¯Êý:	MemoryFill
// ¹¦ÄÜ:	ÄÚ´æÌî³ä
// ²ÎÊý:	lpDest	:	ÄÚ´æµØÖ·
//			dwLen	:	ÄÚ´æ³¤¶È
//			dwFill	:	Ìî³ä×Ö
// ·µ»Ø:	void
//---------------------------------------------------------------------------

//ENGINE_API void g_MemFill(PVOID lpDest, DWORD dwLen, DWORD dwFill)
//{
//#ifdef WIN32
//	__asm
//	{
//		mov		edi, lpDest
//		mov		ecx, dwLen
//		mov		eax, dwFill
//		rep     stosd
//	}
//#else
//	memset(lpDest, dwFill & 0xff, dwLen);
//#endif
//}


ENGINE_API void g_MemFill(LPVOID lpDest, DWORD dwLen, DWORD dwFill)
{
#ifdef WIN32
	uint32_t* dest = static_cast<uint32_t*>(lpDest);

	DWORD dwCount = dwLen / 4;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*dest = dwFill;
		++dest;
	}

	dwCount = dwLen % 4;
	uint8_t* destByte = reinterpret_cast<uint8_t*>(dest);
	for (DWORD i = 0; i < dwCount; ++i)
	{
		*destByte = static_cast<uint8_t>(dwFill);
		++destByte;
	}
#else
	{
		memset(lpDest, dwFill & 0xFF, dwLen);
	}
#endif
}


//---------------------------------------------------------------------------
// º¯Êý:	MemoryZero
// ¹¦ÄÜ:	ÄÚ´æÇåÁã
// ²ÎÊý:	lpDest	:	ÄÚ´æµØÖ·
//			dwLen	:	ÄÚ´æ³¤¶È
// ·µ»Ø:	void
//---------------------------------------------------------------------------


//ENGINE_API void g_MemZero(PVOID lpDest, DWORD dwLen)
//{
//#ifdef WIN32
//	__asm
//	{
//		mov		ecx, dwLen
//		mov		edi, lpDest
//		xor eax, eax
//		mov		ebx, ecx
//		shr		ecx, 2
//		rep     stosd
//		mov     ecx, ebx
//		and ecx, 3
//		rep     stosb
//	}
//#else
//	memset(lpDest, 0, dwLen);
//#endif
//}



ENGINE_API void g_MemZero(LPVOID lpDest, DWORD dwLen)
{
#ifdef WIN32
	DWORD dwCount = dwLen / 4;
	DWORD* pDestDword = static_cast<DWORD*>(lpDest);

	for (DWORD i = 0; i < dwCount; ++i)
	{
		pDestDword[i] = 0;
	}

	DWORD dwRemainder = dwLen % 4;
	if (dwRemainder > 0)
	{
		auto destByte = reinterpret_cast<uint8_t*>(pDestDword + dwCount);
		for (DWORD i = 0; i < dwRemainder; ++i)
		{
			destByte[i] = 0;
		}
	}
#else
	{
		memset(lpDest, 0, dwLen);
	}
#endif
}



//---------------------------------------------------------------------------
// º¯Êý:	MemoryXore
// ¹¦ÄÜ:	ÄÚ´æÒì»ò
// ²ÎÊý:	lpDest	:	ÄÚ´æµØÖ·
//			dwLen	:	ÄÚ´æ³¤¶È
//			dwXor	:	Òì»ò×Ö½Ú
// ·µ»Ø:	void
//---------------------------------------------------------------------------

//ENGINE_API void g_MemXore(PVOID lpDest, DWORD dwLen, DWORD dwXor)
//{
//#ifdef WIN32
//	__asm
//	{
//		mov		edi, lpDest
//		mov		ecx, dwLen
//		mov		eax, dwXor
//		shr		ecx, 2
//		cmp		ecx, 0
//		jle		loc_xor_exit
//		loc_xor_loop :
//		xor [edi], eax
//			add		edi, 4
//			dec		ecx
//			jnz		loc_xor_loop
//			loc_xor_exit :
//	}
//#else
//	unsigned long* ptr = (unsigned long*)lpDest;
//	while ((long)dwLen > 0) {
//		*ptr++ ^= dwXor;
//		dwLen -= sizeof(unsigned long);
//	}
//#endif
//}


ENGINE_API void g_MemXore(LPVOID lpDest, DWORD dwLen, DWORD dwXor)
{
#ifdef WIN32
	DWORD* pDest = static_cast<DWORD*>(lpDest);
	DWORD dwCount = dwLen / 4;

	for (DWORD i = 0; i < dwCount; ++i)
	{
		pDest[i] ^= dwXor;
	}

	DWORD dwRemainder = dwLen % 4;
	if (dwRemainder > 0)
	{
		auto destByte = reinterpret_cast<uint8_t*>(pDest + dwCount);
		for (DWORD i = 0; i < dwRemainder; ++i)
		{
			destByte[i] ^= static_cast<uint8_t>(dwXor >> (8 * i));
		}
	}
#else
	unsigned long* ptr = static_cast<unsigned long*>(lpDest);
	while (static_cast<long>(dwLen) > 0)
	{
		*ptr++ ^= dwXor;
		dwLen -= sizeof(unsigned long);
	}
#endif
}
