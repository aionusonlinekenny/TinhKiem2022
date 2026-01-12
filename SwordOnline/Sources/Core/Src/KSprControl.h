
#pragma once

#ifndef _SERVER

#define		SPR_X_OFF	160
#define		SPR_Y_OFF	190

class KSprControl
{
public:
	INT			m_bChange;							// ÊÇ·ñ·¢Éú±ä»¯
	INT			m_nTotalFrame;						// ×ÜÖ¡Êý
	INT			m_nCurFrame;						// µ±Ç°Ö¡
	INT			m_nTotalDir;						// ×Ü·½ÏòÊý
	INT			m_nCurDir;							// µ±Ç°·½Ïò
	DWORD		m_dwTimer;							// Ê±¼ä¼ÆÊýÆ÷
	DWORD		m_dwInterval;						// Ö¡¼ä¸ô£¨¶¯»­ËÙ¶È£©
	INT			m_nXpos;							// ×ø±ê x
	INT			m_nYpos;							// ×ø±ê y
	INT			m_nCgXpos;							// ÖØÐÄ×ø±ê x
	INT			m_nCgYpos;							// ÖØÐÄ×ø±ê y
	char		m_szName[80];						// ÎÄ¼þÃû
	DWORD		m_dwNameID;							// ÎÄ¼þÃûid
	DWORD		m_dwChangeColor;								// 换色
	INT			m_nBlendStyle;									// 图形的绘制混合模式（0：Alpha，1：Screen）

public:
	KSprControl();									// ¹¹Ôìº¯Êý
	void		Release();							// Çå¿Õ
	void		SetFileName(char* lpszName);		// Éè¶¨ spr ÎÄ¼þÃû
	void		SetTotalFrame(INT nTotalFrame);		// Éè¶¨×ÜÖ¡Êý
	void		SetCurFrame(INT nCurFrame);			// Éè¶¨µ±Ç°Ö¡
	void		SetTotalDir(INT nTotalDir);			// Éè¶¨×Ü·½ÏòÊý
	BOOL		SetCurDir(INT nDir);				// Éè¶¨µ±Ç°·½Ïò
	void		SetInterVal(DWORD dwInterval);		// Éè¶¨Ö¡¼ä¸ô
	void		SetPos(INT x, INT y);				// Éè¶¨×ø±ê
	void		SetCenterPos(INT x, INT y);			// Éè¶¨ÖØÐÄÎ»ÖÃ
	void		SetDirFrame(INT nDir, INT nFrame);	// Éè¶¨Ä³·½ÏòÎªµÚ¼¸Ö¡
	void		Set64DirFrame(INT nDir, INT nFrame);// Éè¶¨Ä³·½ÏòÎªµÚ¼¸Ö¡(·½ÏòÐè´Ó64·½Ïò×ª»»µ½ÕæÕýµÄ·½Ïò)
	BOOL		SetCurDir64(INT nDir);				// Éè¶¨µ±Ç°·½Ïò(·½ÏòÐè´Ó64·½Ïò×ª»»µ½ÕæÕýµÄ·½Ïò)
	INT			GetOneDirFrames();					// »ñµÃµ¥·½ÏòÖ¡Êý

	void		SetSprFile(							// »ñµÃÒ»¸ö spr ÎÄ¼þµÄ¸÷ÖÖÐÅÏ¢
		char* lpszName,
		INT nTotalFrame,
		INT nTotalDir,
		INT nInterVal);

	BOOL		GetNextFrame(BOOL bLoop = TRUE);	// ×Ô¶¯Ïòºó»»Ö¡
	BOOL		GetPrevFrame(BOOL bLoop = TRUE);	// ×Ô¶¯ÏòÇ°»»Ö¡
	BOOL		CheckExist();						// ÅÐ¶ÏÎÄ¼þÊÇ·ñ´æÔÚ
	BOOL		CheckEnd();							// ÅÐ¶Ï¶¯»­ÊÇ·ñ²¥·Åµ½×îºó
	void		SetDirStart();						// Éè¶¨µ±Ç°·½ÏòµÄµ±Ç°Ö¡ÎªµÚÒ»Ö¡
	void		SetDirEnd();						// Éè¶¨µ±Ç°·½ÏòµÄµ±Ç°Ö¡Îª×îºóÒ»Ö¡
	INT			GetCurDirFrameNo();					// »ñµÃµ±Ç°·½ÏòµÚ¼¸Ö¡
	char*		GetName(void) { return m_szName; }
	DWORD		GetNameId(void) const { return m_dwNameID; }
	DWORD		GetChangeColor(void) const { return m_dwChangeColor; }
	INT			GetCurFrame(void) const { return m_nCurFrame; }
	INT			GetTotalFrame(void) const { return m_nTotalFrame; }
	INT			GetCurDir(void) const { return m_nCurDir; }
	INT			GetTotalDir(void) const { return m_nTotalDir; }
	INT			GetBlendStyle(void) const { return m_nBlendStyle; }

	// »æÖÆ
	void		DrawAlpha(
		INT nX,						// ÆÁÄ»×ø±ê x
		INT nY,						// ÆÁÄ»×ø±ê y
		INT nExAlpha = 32);			// alpha ¶È
	// »æÖÆ±ß¿ò
	void		DrawBorder(
		INT nX,						// ÆÁÄ»×ø±ê x
		INT nY,						// ÆÁÄ»×ø±ê y
		INT nColor);				// ÑÕÉ«
};

//---------------------------------------------------------------------------
//	¹¦ÄÜ£º	ÅÐ¶Ï¿ØÖÆ¶ÔÏóÊÇ·ñ´æÔÚ
//---------------------------------------------------------------------------
inline BOOL KSprControl::CheckExist()
{
	if ((m_nTotalFrame > 0) && m_szName[0])
		return TRUE;

	return FALSE;
}


#endif