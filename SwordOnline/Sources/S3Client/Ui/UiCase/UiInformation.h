/*****************************************************************************************
//	界面--消息窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-14
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndText.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndShowAnimate.h"

class KUiInformation : protected KWndShowAnimate
{
public:
	void	Initialize();							//初始化
	void	LoadScheme(const char* pScheme);		//载入界面方案
	void	Show(const char* pInformation,
		const char* pszFirstBtnText = "X竎 nh薾",
		const char* pszSecondBtnText = 0,
		KWndWindow* pCallerWnd = 0,
		UINT uParam = 0,
		INT nInformationLen = -1,
		INT nLeftTime = -1);			//显示窗口
	void	Close();								//关闭窗口，不通知调用窗口
	KUiInformation();

private:
	void	Hide(INT nBtnIndex);					//隐藏窗口
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	void	Breathe();
private:
	KWndText256			m_Information;
	KWndPureTextBtn		m_FirstBtn;
	KWndPureTextBtn		m_SecondBtn;
	KWndWindow* m_pCallerWnd;
	UINT		m_uCallerParam;
	INT					m_nOrigFirstBtnXPos;
	INT					m_nCentreBtnXPos;
	UINT		m_uLastShowMessage;
	INT					m_nLeftTime;

	INT                 m_nTipModeFlagFlag;
};

void UIMessageBox(const char* pMsg, KWndWindow* pCaller = 0,
	const char* pszFirstBtnText = "X竎 nh薾",
	const char* pszSecondBtnText = 0,
	UINT uParam = 0,
	INT nLeftTime = -1);
void UiCloseMessageBox();

extern KUiInformation	g_UiInformation;