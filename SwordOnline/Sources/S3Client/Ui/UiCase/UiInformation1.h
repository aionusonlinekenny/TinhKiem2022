/*****************************************************************************************
//	½çÃæ--ÏûÏ¢´°¿Ú
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-14
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndText.h"
#include "../Elem/WndImage.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndShowAnimate.h"

class KUiInformation1 : protected KWndShowAnimate
{
public:
	void	Initialize();							//³õÊ¼»¯
	void	LoadScheme(const char* pScheme);		//ÔØÈë½çÃæ·½°¸
	void	Show(const char* pInformation, INT nMsgLen = -1,
		const char* pszFirstBtnText = "§ång ý",
		const char* pszSecondBtnText = 0,
		KWndWindow* pCallerWnd = 0,
		UINT uParam = 0,
		const char* pszImage = 0,
		INT nLeftTime = -1);			//ÏÔÊ¾´°¿Ú
	void	SpeakWords(KUiInformationParam* pWordDataList, INT nCount, INT nLeftTime = -1);	//ÏÔÊ¾¶Ô»°ÄÚÈÝ
	void	Close();								//¹Ø±Õ´°¿Ú£¬²»Í¨Öªµ÷ÓÃ´°¿Ú
	KUiInformation1();

private:
	void	Hide(INT nBtnIndex);					//Òþ²Ø´°¿Ú
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//´°¿Úº¯Êý
	void	PaintWindow();
	void	Breathe();
private:
	KUiInformationParam* m_pWordDataList;
	KWndText256			m_Information;
	KWndPureTextBtn		m_FirstBtn;
	KWndPureTextBtn		m_SecondBtn;
	KWndImage			m_MsgIcon;
	KWndWindow* m_pCallerWnd;
	UINT		m_uCallerParam;
	INT					m_nMsgIconFrame;
	UINT		m_uLastShowMessage;
	INT					m_nLeftTime;
};

void UIMessageBox1(const char* pMsg, INT nMsgLen = -1, KWndWindow* pCaller = 0,
	const char* pszFirstBtnText = "§ång ý",
	const char* pszSecondBtnText = 0,
	UINT uParam = 0,
	const char* pszImage = 0,
	INT nLeftTime = -1);
void UiCloseMessageBox1();

extern KUiInformation1	g_UiInformation1;