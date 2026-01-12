//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KBitmap16.h
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KBitmap16_H
#define KBitmap16_H
//---------------------------------------------------------------------------
#include "KMemClass.h"
//---------------------------------------------------------------------------
typedef struct {
	char		Id[4];	// "BM16"
	int32_t			Width;
	int32_t			Height;
	int32_t			RGBMask;
} KBMPHEADER16;
//---------------------------------------------------------------------------
class ENGINE_API KBitmap16
{
protected:
	KMemClass	m_Bitmap;
	int32_t			m_nWidth;
	int32_t			m_nHeight;
public:
	KBitmap16();
	BOOL		Init(int32_t nWidth, int32_t nHeight);
	void		Free();
	BOOL		Load(LPSTR lpFileName);
	BOOL		Save(LPSTR lpFileName);
	void		Draw(int32_t nX, int32_t nY);
	void		DrawLum(int32_t nX, int32_t nY, int32_t nLum);
	void		Clear();
	void		PutPixel(int32_t nX, int32_t nY, WORD wColor);
	WORD		GetPixel(int32_t nX, int32_t nY);
	LPWORD		GetPointer(int32_t nX, int32_t nY);

public:
	void* GetBitmap() { return m_Bitmap.GetMemPtr(); };
	int32_t			GetWidth() { return m_nWidth; };
	int32_t			GetHeight() { return m_nHeight; };
};
//---------------------------------------------------------------------------
#endif
