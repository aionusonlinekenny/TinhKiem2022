//////////////////////////////////////////////////////////////////////////////////////
//	ÎÄ¼þÃû			:	TextPic.cpp
//	´´½¨ÈË			:	ÍõÎ÷±´
//	´´½¨Ê±¼ä		:	2003-6-22 23:13:12
//	ÎÄ¼þËµÃ÷		:	ÎÄ×ÖÇ¶ÈëÊ½Í¼Æ¬
//////////////////////////////////////////////////////////////////////////////////////

#include "KWin32.h"
#include "TextPic.h"
#include "KIniFile.h"
#include "CrtDbg.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"

KInlinePicSink::KInlinePicSink()
{
	m_pRepShell = NULL;
}
	
BOOL KInlinePicSink::Init(iRepresentShell* pShell)
{
	m_pRepShell = pShell;
	if (!m_pRepShell)
		return FALSE;
	KIniFile ini;
#define CHAR_BUFFER_LEN	32
	char szBuffer[CHAR_BUFFER_LEN], szPath[CHAR_BUFFER_LEN],
		szIndex[4], szBuffer2[CHAR_BUFFER_LEN * 2];
	INT nPicCount = 0, i = 0;
	KUiImageRef CurrentImg;
	KImageParam	Param;

	if (!ini.Load("\\Ui\\ChatPics.ini"))
		return FALSE;
	ini.GetString("Main", "Path", "\\Spr\\Ui3\\ChatPics", szPath, CHAR_BUFFER_LEN);
	ini.GetInteger("Main", "Count", 0, &nPicCount);

	if (nPicCount > MAX_SYSTEM_INLINE_PICTURES)
		nPicCount = MAX_SYSTEM_INLINE_PICTURES;

	for (i = 0; i < nPicCount; i++)
	{
		sprintf(szIndex, "%d", i);
		if (!ini.GetString("Main", szIndex, "", szBuffer, CHAR_BUFFER_LEN))
		{
			_ASSERT(0);
			break;
		}
		strcpy(szBuffer2, szPath);
		strcat(szBuffer2, "\\");
		strcat(szBuffer2, szBuffer);
		IR_InitUiImageRef(CurrentImg);
		CurrentImg.nFlipTime = 0;
		CurrentImg.nInterval = 0;
		CurrentImg.nNumFrames = 0;
		strncpy(CurrentImg.szImage, szBuffer2, sizeof(CurrentImg.szImage));
		CurrentImg.szImage[sizeof(CurrentImg.szImage) - 1] = 0;
		CurrentImg.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		CurrentImg.nType = ISI_T_SPR;
		memset(&Param, 0, sizeof(Param));
		pShell->GetImageParam(CurrentImg.szImage, &Param, CurrentImg.nType);
		m_Images.push_back(CurrentImg);
		m_ImageParams.push_back(Param);
	}

	if (FAILED(m_pRepShell->AdviseRepresent(this)))
		return FALSE;
	if (FAILED(AdviseEngine(this)))
		return FALSE;
	return TRUE;
}

BOOL KInlinePicSink::UnInit()
{
	if (!m_pRepShell)
		return FALSE;
	if (FAILED(m_pRepShell->UnAdviseRepresent(this)))
		return FALSE;
	if (FAILED(UnAdviseEngine(this)))
		return FALSE;
	return TRUE;
}

LONG KInlinePicSink::GetPicSize(uint16_t wIndex, INT& cx, INT& cy)
{
	_ASSERT(m_Images.size() == m_ImageParams.size() &&
			m_CustomImages.size() == m_CustomImageParams.size());
	if (!m_pRepShell ||
		wIndex < MAX_SYSTEM_INLINE_PICTURES && wIndex >= m_Images.size() ||
		wIndex >= MAX_SYSTEM_INLINE_PICTURES &&
			(wIndex - MAX_SYSTEM_INLINE_PICTURES >= m_CustomImages.size() ||
			!m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES].szImage[0]))
		return E_FAIL;

	if (wIndex < MAX_SYSTEM_INLINE_PICTURES)
	{
		cx = m_ImageParams[wIndex].nWidth;
		cy = m_ImageParams[wIndex].nHeight;
	}
	else
	{
		cx = m_CustomImageParams[wIndex - MAX_SYSTEM_INLINE_PICTURES].nWidth;
		cy = m_CustomImageParams[wIndex - MAX_SYSTEM_INLINE_PICTURES].nHeight;
	}

	return S_OK;
}

LONG KInlinePicSink::DrawPic(uint16_t wIndex, INT x, INT y)
{
	_ASSERT(m_Images.size() == m_ImageParams.size() &&
			m_CustomImages.size() == m_CustomImageParams.size());
	if (!m_pRepShell ||
		wIndex < MAX_SYSTEM_INLINE_PICTURES && wIndex >= m_Images.size() ||
		wIndex >= MAX_SYSTEM_INLINE_PICTURES &&
			(wIndex - MAX_SYSTEM_INLINE_PICTURES >= m_CustomImages.size() ||
			!m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES].szImage[0]))
		return E_FAIL;

	KUiImageRef* pPic = NULL;
	if (wIndex < MAX_SYSTEM_INLINE_PICTURES)
		pPic = &(m_Images[wIndex]);
	else
		pPic = &(m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES]);
	pPic->oPosition.nX = x;
	pPic->oPosition.nY = y;

	m_pRepShell->DrawPrimitives(1, pPic, RU_T_IMAGE, true);
	IR_NextFrame(*pPic);
	return S_OK;
}


//¶¯Ì¬¼ÓÔØÍ¼Æ¬,»ñÈ¡Ò»¸öWORD,¼´Í¼Æ¬µÄË÷Òý
LONG KInlinePicSink::AddCustomInlinePic(uint16_t& rwIndex, const char* szSprPathName)
{
	KUiImageRef CurrentImg;
	KImageParam	Param;
	INT i = 0, nSize = 0;

	rwIndex = -1;
	if (!szSprPathName || !szSprPathName[0])
		return E_FAIL;

	IR_InitUiImageRef(CurrentImg);
	CurrentImg.nFlipTime = 0;
	CurrentImg.nInterval = 0;
	CurrentImg.nNumFrames = 0;
	strncpy(CurrentImg.szImage, szSprPathName, sizeof(CurrentImg.szImage));
	CurrentImg.szImage[sizeof(CurrentImg.szImage) - 1] = 0;
	CurrentImg.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	CurrentImg.nType = ISI_T_SPR;
	memset(&Param, 0, sizeof(Param));
	if (!m_pRepShell->GetImageParam(CurrentImg.szImage, &Param, CurrentImg.nType))
		return E_FAIL;
	for (i = 0, nSize = (INT)m_CustomImages.size(); i < nSize; i++)
	{
		if (0 == m_CustomImages[i].szImage[0])
		{
			m_CustomImages[i] = CurrentImg;
			m_CustomImageParams[i] = Param;
			break;
		}
	}
	if (i >= nSize)
	{
		m_CustomImages.push_back(CurrentImg);
		m_CustomImageParams.push_back(Param);
	}
	rwIndex = i + MAX_SYSTEM_INLINE_PICTURES;
	//return S_OK;
	return rwIndex;
}

//¶¯Ì¬Ð¶ÔØÍ¼Æ¬
LONG KInlinePicSink::RemoveCustomInlinePic(uint16_t wIndex)
{
	if (wIndex < MAX_SYSTEM_INLINE_PICTURES ||
		wIndex >= MAX_SYSTEM_INLINE_PICTURES &&
			(wIndex - MAX_SYSTEM_INLINE_PICTURES >= m_CustomImages.size() ||
			!m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES].szImage[0]))
		return E_FAIL;
	m_CustomImages[wIndex - MAX_SYSTEM_INLINE_PICTURES].szImage[0] = 0;
	return S_OK;
}
