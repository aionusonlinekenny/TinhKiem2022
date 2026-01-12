//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KDrawSpriteAlpha.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#include <minwindef.h>
#ifndef KDrawSpriteAlpha_H
#define KDrawSpriteAlpha_H
//---------------------------------------------------------------------------
void g_DrawSpriteAlpha(void* node, void* canvas);
void g_DrawSpriteAlpha(void* node, void* canvas, int nExAlpha);
void g_DrawSprite3LevelAlpha(void* node, void* canvas);	//Èý¼¶alpha»æÖÆ
void g_DrawSpriteScreenBlendMMX(BYTE byInputAlpha, DWORD dwMask32, void* pBuffer, INT width, INT height,
	INT nPitch, INT nX, INT nY, void* pPalette, void* pSprite, INT nWidth, INT nHeight, const RECT* pSrcRect);
void ScreenBlend4Pixel(void* pDest, void* pColor, void* pAlpha);
//---------------------------------------------------------------------------
#endif
