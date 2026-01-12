// -------------------------------------------------------------------------
//	文件名		：	ShowChatText.h
//	创建者		：	彭建波
//	创建时间	：	2002-9-16 16:22:52
//	功能描述	：	显示聊天内容
// -------------------------------------------------------------------------
#pragma once

#include "WndWindow.h"
#include "WndScrollbar.h"
#include "../../../core/src/gamedatadef.h"

struct KItemBtn
{
	char m_pItem[MAX_SENTENCE_LENGTH];
	INT	nPic;
	INT nLeft;
	INT nLine;
	void Release() {
		memset(m_pItem, 0, sizeof(m_pItem));
		nPic = 0;
		nLeft = 0;
		nLine = 0;
	};
};

struct KOneMsgInfo
{
	INT			 nLines;				//这条信息占了多少行
	INT			 nCharWidth;			//这条信息占了字符宽
	UINT uTextBKColor;			//这条信息字显示时的文字衬底的颜色,0为无
	INT			 nData;
	INT			 nMaxSize;				//Msg可以容纳的最大信息长度
	INT			 nLen;					//信息长度
	KItemBtn	 Item;
	char		 Msg[1];				//信息的内容
};

class KWndScrollBar;

class KWndMessageListBox : public KWndWindow
{
public:
	KWndMessageListBox();
	virtual ~KWndMessageListBox();
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);//初始化

	void	SetFirstShowLine(INT nLine);	//设置第一条被显示的文字是全部文字的第几行
	INT		GetFirstShowLine();				//获取第一条被显示的文字是全部文字的第几行
	void	SetScrollbar(KWndScrollBar* pScroll);

	INT		SetCapability(INT nNumMessage);
	INT		GetCapability() { return m_nCapability; }
	INT		GetCurSel() { return m_nSelMsgIndex; }
	INT		SetCurSel(INT nIndex);

	INT		AddOneMessage(const char* pText, INT nLen, UINT uTextBKColor = 0, INT nData = -1, KItemBtn* pIB = NULL);
	INT		GetOneMessage(INT nIndex, char* pBuffer, INT nLen, bool bExcludeCtrl);
	INT		GetOneMessageData(INT nIndex);
	void	RemoveAMessage(INT nIndex);
	INT		GetMsgCount() { return m_nNumMessage; }
	INT		HitTextAtPoint(INT x, INT y);

	void	SetSize(INT nWidth, INT nHeight);//设置窗口大小
	void	Clear();
	void	Clone(KWndMessageListBox* pCopy);
	INT		GetMaxShowLine() { return m_nNumMaxShowLine; }
	INT		GetHideNumLine() { return m_nHideNumLine; }
	INT		GetItemLineCount(INT nIndex);

	//从窗口分离出数据
	UINT	SplitData();
	//给窗口捆绑数据
	UINT	BindData(UINT	hData);
	//释放窗口数据句柄
	static void		FreeData(UINT	hData);

	virtual INT		PtInWindow(INT x, INT y);

	void ClearHideLine();
	void HideNextLine();
	void HideAllLine();

	INT	GetMinHeight();

	void SetFontSize(INT nFontSize);
	INT GetFontSize() { return m_nFontSize; }
	void SetTextColor(UINT MsgColor, UINT MsgBorderColor)
	{
		m_MsgColor = MsgColor;
		m_MsgBorderColor = MsgBorderColor;
	};
	void SetItemActived(BOOL bItemActived);
	INT GetMaxLineWidth() { return m_nNumBytesPerLine; }

private:
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);
	void	UpdateData();					//根据内容增删或者窗口尺寸变化重新作些参数计算以及滚动条容量调整
	virtual void	PaintWindow();			//绘制窗口
	void	OnMouseMove(INT x, INT y);
	void	OnLButtonDown(INT x, INT y);
	void	OnLButtonDClick(INT x, INT y);
	INT		GetMsgAtPoint(INT x, INT y);

private:
	struct KMessageListData
	{
		KOneMsgInfo** pMessages;			//信息数据
		INT				nNumMessage;		//信息条数目
		INT				nCapability;		//信息条最多允许数目
		INT				nStartShowMsg;		//首条显示的信息
		INT				nStartMsgSkipLine;	//首条显示的信息上方忽略的行数目
		INT				nSelMsgIndex;		//当前选择的消息
	};

private:
	KWndScrollBar* m_pScrollbar;			//滚动条
	KOneMsgInfo** m_pMessages;			//信息数据

	INT				m_nNumMessage;			//信息条数目
	INT				m_nCapability;			//信息条最多允许数目

	INT				m_nNumMaxShowLine;		//最多可显示的行数目
	INT				m_nNumBytesPerLine;		//每一行字符的数目
	INT				m_nNumVisibleTextLine;	//可以看见的有文字的行的数目

	INT				m_nHideNumLine;		//被强迫隐藏的行数,追加/删除/选中/滚动行/改尺寸时会无效

	INT				m_nFontSize;			//字体大小
	INT				m_nStartShowMsg;		//首条显示的信息
	INT				m_nStartMsgSkipLine;	//首条显示的信息上方忽略的行数目

	INT				m_nSelMsgIndex;			//当前选择的消息
	INT				m_nHLMsgIndex;			//鼠标所指消息的索引
	UINT	m_MsgColor;				//默认的文字颜色
	UINT	m_MsgBorderColor;		//默认的文字边缘颜色
	UINT	m_SelMsgColor;			//被选中文字的颜色
	UINT	m_SelMsgBorderColor;	//被选中文字的边缘颜色

	UINT	m_SelMsgBgColor;		//被选中消息的背景颜色
	UINT	m_HLMsgColor;			//鼠标所指消息的颜色
	UINT	m_HLMsgBorderColor;		//鼠标所指消息的边缘颜色
	UINT	m_uTextLineShadowColor;	//有文字的行拥有的背景颜色，如果为0，表示无此背景色
	INT				m_nRowDis;

	BOOL			m_bHitText;
	BOOL			m_bItemActived;
	UINT	GetOffsetTextHeight();
};

class KScrollMessageListBox : public KWndWindow
{
public:
	KScrollMessageListBox();

	virtual INT		Init(KIniFile* pIniFile, const char* pSection);//初始化
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);

	KWndMessageListBox* GetMessageListBox()
	{
		return &m_MsgList;
	}
	KWndScrollBar* GetScrollBar()
	{
		return &m_Scroll;
	}
	INT		HeightToLineHeight(INT nHeight);
	INT		WidthToCharWidth(INT nWidth);
	INT		GetMinHeight();
	INT		GetFontSize() { return m_MsgList.GetFontSize(); };

protected:
	KWndMessageListBox	m_MsgList;
	KWndScrollBar		m_Scroll;
	INT m_nLineHeight;
	INT m_nMinLineCount;
	INT	m_nMaxLineCount;
	INT m_nCurrentLineCount;

	void	SetMsgLineCount(INT nCount);
	void	InitMinMaxLineCount(INT nMin, INT nMax);//初始化
};

bool	MsgListBox_LoadContent(KWndMessageListBox* pBox, KIniFile* pFile, const char* pszSection);