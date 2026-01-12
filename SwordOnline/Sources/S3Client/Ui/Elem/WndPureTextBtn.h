/*****************************************************************************************
//	界面窗口体系结构-纯文字按钮
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-1-19
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "../../../core/src/gamedatadef.h"

#define	WNDPTB_MAX_TEXT_LEN	64

#define	WNDPTB_S_CENTRE_ALIGN	0x00008000

#define	WNDPTB_F_BE_PRESSEDDOWN	0x00000001
#define	WNDPTB_F_OVER			0x00000002
#define	WNDPTB_F_CHECK			0x00000004

class KWndPureTextBtn : public KWndWindow
{
public:
	KWndPureTextBtn();
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);	//初始化
	virtual void	PaintWindow();									//绘制窗口
	void			SetText(const char* pText, INT nLen = -1);		//设置文本文字
	INT				GetTextLen();
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	void			CheckButton(INT bChecked);
	void			Clone(KWndPureTextBtn* pCopy);
	BOOL            IsChecked();
	void			Set3IntText(INT nNumber1, INT nNumber2, char Separator, char* Find);
	virtual INT		GetToolTipInfo(char* szTip, INT nMax);
	INT				SetToolTipInfo(char* szTip, INT nMax);
	virtual const char* GetShortKey() { return NULL; }	//取快捷键,用于Tip显示
	void SetTextColor(UINT Color)
	{
		m_NormalColor = Color;
	}
	void SetColor(UINT NormalColor, UINT OverColor, UINT PressedColor)
	{
		m_NormalColor = NormalColor;
		m_OverColor = OverColor;
		m_PressedColor = PressedColor;
	}
private:
	void			AdjustPosition();
private:
	UINT	m_Flag;
	char			m_sText[WNDPTB_MAX_TEXT_LEN];
	INT				m_nTextLen;			//字符串的存储长度
	UINT	m_NormalColor;
	UINT	m_OverColor;
	UINT	m_PressedColor;
	UINT	m_NormalBorderColor;
	UINT	m_OverBorderColor;
	UINT	m_PressedBorderColor;
	INT				m_nFontSize;
	char			m_szTip[256];
	INT				m_nTipLen;
	static UINT	ms_uBtnTipTextColor;	//按钮提示名称文字的颜色
public:
	char			m_pItem[MAX_SENTENCE_LENGTH];
	INT				m_X;
	INT				m_Y;
	bool			m_bHaveItem;//
	void			SetItem(const char* pItem)
	{
		strcpy(m_pItem, pItem);
	};
};
