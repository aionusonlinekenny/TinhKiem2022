/*
 * File:     UiFaceSelector.h
 * Desc:     表情选择窗口
 * Author:   flying
 * Creation: 2003/7/8
 * 今天是偶老婆的生日哦，呵呵
 */
 //-----------------------------------------------------------------------------
#pragma once

#if !defined _FACESELECTOR
#define _FACESELECTOR

#include "../Elem/WndButton.h"

class KUiFaceSelector : public KWndImage
{
public:
	//打开窗口，返回唯一的一个类对象实例
	static KUiFaceSelector* OpenWindow(KWndWindow* pCaller, void* pvParam);
	//关闭窗口，同时可以选则是否删除对象实例
	static void				CloseWindow(bool bDestroy);
	static KUiFaceSelector* GetIfVisible();
	void					LoadScheme(const char* pScheme);	//载入界面方案

	static void				LoadFaceList();
	static void				Clear();

	static INT				ConvertFaceText(char* pDest, const char* pSrc, INT nCount);

private:
	KUiFaceSelector();
	virtual		~KUiFaceSelector() {}
	static INT	ConvertFace(char* pDest, INT& nConvertCount, const char* pSrc, INT nCount, INT& nReadPos);
	void		PaintWindow();
	INT			WndProc(UINT uMsg, UINT uParam, INT nParam);
	void		Show();
	void		Hide();

	void		Initialize();
	INT			GetFaceAtPos(INT x, INT y);
	void		UpdateFaceTip(INT x, INT y);
	void		OnSelFace();

private:
	struct	KFaceItem
	{
		char	szFaceText[32];
		char	szFaceTip[32];
		char	szImage[128];
		WORD	wFaceID;
	};
	static KUiFaceSelector* m_pSelf;
	static KFaceItem* ms_pFaceList;
	static INT			ms_nNumFaces;

private:
	KWndButton			m_PrevBtn;
	KWndButton			m_NextBtn;

	INT					m_nNormColuCount;
	INT                 m_nExpColuCount;
	INT					m_nRowCount;

	INT                 m_nIndentH, m_nIndentV;
	INT					m_nBtnWidth, m_nBtnHeight;	//表情符号按钮的长宽

	UINT		m_nBgColor;
	UINT		m_nBgOver;
	UINT		m_nBgAlpha;
	UINT		m_nBorderColor;

	INT					m_nCurrIndex;		//当前选中/指向的表情图标
	INT					m_nCurrPage;
	INT                	m_nFacesPageCount;	//是否在扩展状态
	INT                	m_nMaxFacesPage;	//是否在扩展状态

	KWndWindow* m_pCallerWnd;		//调用窗口
	void* m_pvCallerParam;	//调用窗口调用时的自定义参数
};

#endif