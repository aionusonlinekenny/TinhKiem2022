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

struct KUiInformationParam;

class KUiInformation2 : protected KWndShowAnimate
{
public:
	void	Initialize();							//初始化
	void	LoadScheme(const char* pScheme);		//载入界面方案
	void	Show(const char* pInformation, INT nInforLen, BOOL bNeedConfirmNotify = TRUE);//显示窗口

	void	SpeakWords(KUiInformationParam* pWordDataList, INT nCount);	//显示对话内容
	KWndWindow* TopChildFromPoint(INT x, INT y) {
		return this;
	}
	void	Hide();									//隐藏窗口
	void	Close();
	KUiInformation2();
	~KUiInformation2();
private:
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
private:
	KWndText512			m_Information;

	KUiInformationParam* m_pWordDataList;
	INT						m_nNumWordData;
	INT						m_nCurrentWord;
	BOOL					m_bNeedConfirmNotify;
};

void UIMessageBox2(const char* pMsg, INT nMsgLen = -1);

extern KUiInformation2	g_UiInformation2;