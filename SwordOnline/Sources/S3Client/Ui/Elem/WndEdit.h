/*****************************************************************************************
//	界面窗口体系结构--输入窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-22
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "WndText.h"

#define	WNDEDIT_ES_FILTER				0x00ff
#define	WNDEDIT_ES_ENGLISH_AVAILABLE	0x0001		//可以输入西文字符，符号、标点等
#define	WNDEDIT_ES_IME_AVAILABLE		0x0002		//可以输入ime字符（中文字符）等
#define	WNDEDIT_ES_CHAR_TYPE_FILTER		(WNDEDIT_ES_ENGLISH_AVAILABLE | WNDEDIT_ES_IME_AVAILABLE)
#define	WNDEDIT_ES_MASK_CHARACTER		0x0010		//显示时以特殊符号替换要显示的字符
#define	WNDEDIT_ES_MULTI_LINE			0x0020		//多行编辑
#define	WNDEDIT_F_HAVE_FOCUS			0x0100
#define	WNDEDIT_F_HAVE_SHADOW			0x0200

class KWndEdit : public KWndWindow
{
public:
	KWndEdit();
	~KWndEdit();
	virtual INT	Init(KIniFile* pIniFile, const char* pSection);//初始化
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	virtual void	PaintWindow();							//绘制窗口
	void			SetColor(UINT uColor);
	void			SetText(const char* pText, INT nLen = -1);				//设置文本文字
	INT				GetText(char* pBuffer, INT nSize, bool bExcludeSpace);//获取字符串内容
	void			SetIntText(INT nNumber);				//设置文本串的内容为与表述所整数的字符串
	DWORD			GetIntNumber();
	void			Enable(INT bEnable);					//禁止或者允许使窗口被操作
	void			Clone(KWndEdit* pCopy);
	void			Clear(bool bSafe = false);
	INT				InsertString(const char* pString, INT nLen);
	void			GetCaretPosition(INT* nx, INT* ny);
	UINT	GetFocusBkColor()	{return m_FocusBkColor;}
	void			SetFocusBkColor(UINT nC)	{m_FocusBkColor = nC;}
	virtual const char*	GetShortKey() {return NULL;}	//取快捷键,用于Tip显示
	static void		SetAllButtonTipTextColor(UINT uColor);	//设置所有按钮提示名称文字的颜色
	static void		EnableAllButtonTip(INT bEnable);					//禁止/允许所有按钮的提示文字
	virtual void	OnButtonClick(){}
	virtual INT		GetToolTipInfo(char* szTip, INT nMax);
	INT		SetToolTipInfo(char* szTip, INT nMax);
	void	SetToolTipHover(bool bFlag) {m_bTipHover = bFlag;};
	INT		GetFontSize() {return m_nFontSize;};
	void	SetMaskCharacter(bool bMask)
	{
		if (bMask)
		{
			m_Flag |= WNDEDIT_ES_MASK_CHARACTER;
			m_Flag &= ~WNDEDIT_ES_MULTI_LINE;
		}
		else
		{
			m_Flag &= ~WNDEDIT_ES_MASK_CHARACTER;
		}
	};
	INT	GetMaskCharacter()
	{
		return (m_Flag & WNDEDIT_ES_MASK_CHARACTER);
	};
	void	SetLimitText(INT nLimit)
	{
		m_nLimitText = nLimit;
	};
	
protected:
	void			SetTextPtr(char* pText, INT nMaxLen);	//设置文本缓冲区指针
	INT				InsertChar(INT nChar);					//
	INT				InsertChar(char cLow, char cHigh);		//
	INT				OnKeyDown(INT nKeyCode, INT nModifiers);//响应键盘按键操作

private:
	INT				CharFromPos(INT x, INT y);				//retrieves information about the character closest to a specified point in the client area of an edit control
	void			UpdateData();
	void			FmtForShow();
	INT				SelectAll();
	INT				Copy();
	INT				Paste();
	
private:
	UINT	m_Flag;
	INT			m_nFontSize;				//字体大小
	char*		m_pText;					//文本缓冲区指针
	INT			m_nBufferSize;				//文本缓冲区指针的长度
	INT			m_nLimitText;				//字符串长度（BYTE）限制，不包括字符串结束符号。
	INT			m_nTextLen;					//字符串实际长度

	INT			m_nSkipAhead;				//显示时忽略前端超出的字符，前端被忽略字符的容量
	INT			m_nSkipBehind;				//显示时忽略后端超出的字符，后端被忽略字符的容量

	INT			m_nCaretLineIndex;			//插入符所在的行位置
	INT			m_nCaretPos;				//插入符的位置
	INT			m_nCaretTime;				//控制插入符显示的时钟

	INT         m_nCaretX;
	INT         m_nCaretY;

    //HWND        m_hEdit;

	UINT m_TextColor;				//字体颜色
	UINT m_TextBorderColor;			//字体边缘颜色
	UINT m_FocusBkColor;			//有焦点时的背景颜色
	
	char			m_szTip[256];
	INT				m_nTipLen;
	bool			m_bTipHover;
private:
	static UINT	ms_uBtnTipTextColor;	//按钮提示名称文字的颜色
	static INT			ms_nDisableBtnTip;		//是否禁止按钮的提示文字
};

class KWndEdit80 : public KWndEdit
{
private:
	char			m_Text[80];
public:
	KWndEdit80();
};
//=====================================
//	文本缓冲区长度为32的文本输入窗口类
//=====================================
class KWndEdit32 : public KWndEdit
{
private:
	char			m_Text[32];
public:
	KWndEdit32();
};

//=====================================
//	文本缓冲区长度为32的文本输入窗口类
//=====================================
class KWndEdit512 : public KWndEdit
{
private:
	char			m_Text[512];
public:
	KWndEdit512();
};
