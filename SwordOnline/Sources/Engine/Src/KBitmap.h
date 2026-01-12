//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KBitmap.h
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KBitmap_H
#define KBitmap_H
//---------------------------------------------------------------------------
#include "KMemClass.h"
#include "KPalette.h"
//---------------------------------------------------------------------------
typedef struct {
	char	Id[4];
	LONG	Width;
	LONG	Height;
	LONG	Colors;
} KBMPHEADER;
//---------------------------------------------------------------------------
class ENGINE_API KBitmap
{
private:
	KMemClass	m_Bitmap;
	KPAL32		m_Pal32[256];
	KPAL16		m_Pal16[256];
	int32_t			m_nWidth;
	int32_t			m_nHeight;
	int32_t			m_nColors;
public:
	KBitmap();
	BOOL		Init(int32_t nWidth, int32_t nHeight, int32_t nColors);
	BOOL		Load(LPSTR lpFileName);
	BOOL		Save(LPSTR lpFileName);
	void		Draw(int32_t nX, int32_t nY);
	void		Clear(BYTE byColor);
	void		MakePalette();
	void		PutPixel(int32_t nX, int32_t nY, BYTE byColor);
	BYTE		GetPixel(int32_t nX, int32_t nY);
public:
	void*		GetBitmap()	{ return m_Bitmap.GetMemPtr(); };
	KPAL32*		GetPal32()	{ return m_Pal32; };
	KPAL16*		GetPal16()	{ return m_Pal16; };
	int32_t			GetWidth()	{ return m_nWidth; };
	int32_t			GetHeight()	{ return m_nHeight; };
	int32_t			GetColors()	{ return m_nColors; };
};
//---------------------------------------------------------------------------
#endif
