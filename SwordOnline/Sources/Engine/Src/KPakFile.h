//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakFile.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KPakFile_H
#define KPakFile_H
//---------------------------------------------------------------------------
#include "KFile.h"

//#ifndef _SERVER
#include "XPackFile.h"
//#endif

//---------------------------------------------------------------------------
ENGINE_API void g_SetPakFileMode(int32_t nFileMode);
//---------------------------------------------------------------------------

class ENGINE_API KPakFile
{
public:
	KPakFile();
	~KPakFile();
	BOOL		Open(const char* pszFileName);
	void		Close();
	bool		IsFileInPak();
	DWORD		Read(void* pBuffer, uint32_t uSize);
	DWORD		Seek(int32_t nOffset, uint32_t uMethod);
	DWORD		Tell();
	DWORD		Size();
	BOOL		Save(const char* pszFileName);
private:
	KFile		m_File;			// 真实文件(不在包中)
//#ifndef _SERVER
	XPackElemFileRef	m_PackRef;
//#endif
};

//---------------------------------------------------------------------------
#ifndef _SERVER
struct KSGImageContent
{
	int32_t				nWidth;
	int32_t				nHeight;
	uint16_t	Data[1];
};
#define	KSG_IMAGE_CONTENT_SIZE(w, h)    ((uint32_t)((&((KSGImageContent *)0)->Data[0])) + w * h * 2)

ENGINE_API SPRHEAD* SprGetHeader(const char* pszFileName, SPROFFS*& pOffsetTable);
ENGINE_API void			SprReleaseHeader(SPRHEAD* pSprHeader);
ENGINE_API SPRFRAME* SprGetFrame(SPRHEAD* pSprHeader, int32_t nFrame);
ENGINE_API void			SprReleaseFrame(SPRFRAME* pFrame);

ENGINE_API KSGImageContent* get_jpg_image(const char cszName[], uint32_t uRGBMask16 = ((uint32_t)-1));
ENGINE_API void release_image(KSGImageContent* pImage);
#endif

#endif
