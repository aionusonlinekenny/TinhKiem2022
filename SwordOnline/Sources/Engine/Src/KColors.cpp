//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KColors.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Colors Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDDraw.h"
#include "KColors.h"
//---------------------------------------------------------------------------
ENGINE_API WORD (*g_RGB)(int nRed, int nGreen, int nBlue) = g_RGB565;
//---------------------------------------------------------------------------
// 函数:	Red
// 功能:	返回一个色彩值的红色分量
// 参数:	wColor	色彩值
// 返回:	红色分量
//---------------------------------------------------------------------------
//ENGINE_API BYTE g_Red(WORD wColor)
//{
//	BYTE Red;
//	__asm
//	{
//		mov		ax, wColor
//		and		ax, 0x0031
//		mov		Red, al
//	}
//	return Red;
//}

ENGINE_API BYTE g_Red(WORD wColor) {
	BYTE Red;
	Red = wColor & 0x31;
	return Red;
}


//---------------------------------------------------------------------------
// 函数:	Red
// 功能:	返回一个色彩值的绿色分量
// 参数:	wColor	色彩值
// 返回:	绿色分量
//---------------------------------------------------------------------------
//ENGINE_API BYTE g_Green(WORD wColor)
//{
//	long Mask16 = g_pDirectDraw->GetRGBBitMask16();
//	BYTE Green;
//	__asm
//	{
//		mov		ax, wColor
//		shr		ax, 5
//		mov		edx, Mask16
//		cmp		edx, 0xffff
//		je		loc_Green_0001
//		and		ax, 0x0031
//
//loc_Green_0001:
//
//		and		ax, 0x0071
//		mov		Green, al
//	}
//	return Green;
//}

ENGINE_API BYTE g_Green(WORD wColor) {
	long Mask16 = g_pDirectDraw->GetRGBBitMask16();
	BYTE Green;
	Green = (wColor >> 5) & 0x71;
	if (Mask16 != 0xffff) {
		Green &= 0x31;
	}
	return Green;
}

//---------------------------------------------------------------------------
// 函数:	Red
// 功能:	返回一个色彩值的兰色分量
// 参数:	wColor	色彩值
// 返回:	兰色分量
//---------------------------------------------------------------------------
//ENGINE_API BYTE g_Blue(WORD wColor)
//{
//	long Mask16 = g_pDirectDraw->GetRGBBitMask16();
//	BYTE Blue;
//	__asm
//	{
//		mov		ax, wColor
//		shr		ax, 10
//		mov		edx, Mask16
//		cmp		edx, 0xffff
//		jne		loc_Blue_0001
//		shr		ax, 1
//
//loc_Blue_0001:
//
//		and		ax, 0x0031
//		mov		Blue, al
//	}
//	return Blue;
//}

ENGINE_API BYTE g_Blue(WORD wColor) {
	long Mask16 = g_pDirectDraw->GetRGBBitMask16();
	BYTE Blue;
	Blue = (wColor >> 10) & 0x1F;
	if (Mask16 != 0xffff) {
		Blue &= 0x31;
	}
	return Blue;
}


//---------------------------------------------------------------------------
// 函数:	RGB
// 功能:	返回一个DWORD色彩值
// 参数:	red		红色分量
//			green	绿色分量
//			blue	蓝色分量
// 返回:	色彩值
//---------------------------------------------------------------------------
//ENGINE_API WORD g_RGB555(int nRed, int nGreen, int nBlue)
//{
//	WORD wColor;
//	__asm
//	{
//		xor		ecx, ecx
//		mov		ebx, 0xff
//		mov		eax, nRed
//		and		eax, ebx
//		shr		eax, 3
//		shl		eax, 10
//		or		ecx, eax
//		mov		eax, nGreen
//		and		eax, ebx
//		shr		eax, 3
//		shl		eax, 5
//		or		ecx, eax
//		mov		eax, nBlue
//		and		eax, ebx
//		shr		eax, 3
//		or		ecx, eax
//		mov		wColor, cx
//	}
//	return wColor;
//}

ENGINE_API WORD g_RGB555(int nRed, int nGreen, int nBlue) {
	WORD wColor;
	wColor = ((nRed & 0xff) >> 3) << 10;
	wColor |= ((nGreen & 0xff) >> 3) << 5;
	wColor |= (nBlue & 0xff) >> 3;
	return wColor;
}


//---------------------------------------------------------------------------
// 函数:	RGB
// 功能:	返回一个DWORD色彩值
// 参数:	red		红色分量
//			green	绿色分量
//			blue	蓝色分量
// 返回:	色彩值
//---------------------------------------------------------------------------
//ENGINE_API WORD g_RGB565(int nRed, int nGreen, int nBlue)
//{
//	WORD wColor;
//	__asm
//	{
//		xor		ecx, ecx
//		mov		ebx, 0xff
//		mov		eax, nRed
//		and		eax, ebx
//		shr		eax, 3
//		shl		eax, 11
//		or		ecx, eax
//		mov		eax, nGreen
//		and		eax, ebx
//		shr		eax, 2
//		shl		eax, 5
//		or		ecx, eax
//		mov		eax, nBlue
//		and		eax, ebx
//		shr		eax, 3
//		or		ecx, eax
//		mov		wColor, cx
//	}
//	return wColor;
//}

ENGINE_API WORD g_RGB565(int nRed, int nGreen, int nBlue) {
	WORD wColor;
	wColor = ((nRed & 0xff) >> 3) << 11;
	wColor |= ((nGreen & 0xff) >> 2) << 5;
	wColor |= (nBlue & 0xff) >> 3;
	return wColor;
}


//---------------------------------------------------------------------------
// 函数:	555 To 565
// 功能:	555格式转化为565格式
// 参数:	nWidth      宽度
//			nHeight	    高度
//			lpBitmap	位图
// 返回:	void
//---------------------------------------------------------------------------
//void g_555To565(int nWidth, int nHeight, void* lpBitmap)
//{
//	__asm
//	{
//		mov		esi, lpBitmap
//		mov		edx, nHeight
//
//loc_555to565_loop1:
//		mov		ecx, nWidth
//
//loc_555to565_loop2:
//		mov		ax, [esi]
//		mov		bx, ax
//		shr		ax, 5
//		shl		ax, 6
//		and		bx, 0x001f
//		or		ax, bx
//		mov		[esi], ax
//		add		esi, 2
//		dec		ecx
//		jnz		loc_555to565_loop2
//		dec		edx
//		jnz		loc_555to565_loop1
//	}
//}

void g_555To565(int nWidth, int nHeight, void* lpBitmap) {
	WORD* pPixel = (WORD*)lpBitmap;
	for (int i = 0; i < nWidth * nHeight; i++) {
		WORD wColor = *pPixel;
		WORD wRed = (wColor & 0x7C00) >> 10;
		WORD wGreen = (wColor & 0x03E0) >> 5;
		WORD wBlue = (wColor & 0x001F);
		wRed <<= 11;
		wGreen <<= 10;
		wBlue <<= 11;
		*pPixel = wRed | wGreen | wBlue;
		pPixel++;
	}
}


//---------------------------------------------------------------------------
// 函数:	565 To 555
// 功能:	565格式转化为555格式
// 参数:	nWidth      宽度
//			nHeight	    高度
//			lpBitmap	位图
// 返回:	void
//---------------------------------------------------------------------------
//void g_565To555(int nWidth, int nHeight, void* lpBitmap)
//{
//	__asm
//	{
//		mov		esi, lpBitmap
//		mov		edx, nHeight
//
//loc_565to555_loop1:
//		mov		ecx, nWidth
//
//loc_565to555_loop2:
//		mov		ax, [esi]
//		mov		bx, ax
//		shr		ax, 6
//		shl		ax, 5
//		and		bx, 0x001f
//		or		ax, bx
//		mov		[esi], ax
//		add		esi, 2
//		dec		ecx
//		jnz		loc_565to555_loop2
//		dec		edx
//		jnz		loc_565to555_loop1
//	}
//}

void g_565To555(int nWidth, int nHeight, void* lpBitmap) {
	WORD* pPixel = (WORD*)lpBitmap;
	for (int i = 0; i < nWidth * nHeight; i++) {
		WORD wColor = *pPixel;
		WORD wRed = (wColor & 0xF800) >> 11;
		WORD wGreen = (wColor & 0x07E0) >> 6;
		WORD wBlue = (wColor & 0x001F);
		wRed <<= 10;
		wGreen <<= 5;
		wBlue <<= 5;
		*pPixel = wRed | wGreen | wBlue;
		pPixel++;
	}
}


//---------------------------------------------------------------------------

