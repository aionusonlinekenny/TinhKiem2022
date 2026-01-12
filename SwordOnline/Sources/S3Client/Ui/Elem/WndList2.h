/*****************************************************************************************
//	界面窗口体系结构--列表窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-11
------------------------------------------------------------------------------------------
    列表窗口只列出文字。但是每项都可以关联其他数据。
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "WndScrollBar.h"

#define	WNDLIST_ERROR					-1

class KWndList2 : public KWndWindow
{
public:
	virtual INT	Init(KIniFile* pIniFile, const char* pSection);//初始化
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	virtual void	PaintWindow();							//绘制窗口
	INT				AddString(INT nIndex, const char* pszString, bool bIsNode = false);//添加项
	INT				GetString(INT nIndex, char* pBuffer, UINT nSize);//获取列表项的字符串内容
	INT				DeleteString(INT nIndex);				//删除列表中的一项
	void			ResetContent();							//删除列表中全部的内容
	//在列表中查找某个字符串。
	INT				FindString(const char* pString);
	INT				SetItemData(INT nIndex, INT nData);		//设置列表项关联数据的值
	INT				GetItemData(INT nIndex);				//获得列表项关联数据的值
	INT 			SetItemColor(INT nIndex, UINT Color);
	UINT 	GetItemColor(INT nIndex);
	void			SetTopItemIndex(INT nTopItemIndex);		//获得列表窗口顶部显示的数据项的索引
	INT				GetTopItemIndex() { return m_nTopItemIndex; }//获得列表窗口顶部显示的数据项的索引
	INT				GetVisibleLineCount() const;			//获得列表框可以同时显示的项的数目
	INT				GetVisibleNodeCount(INT nTopIndex) const;
	INT				GetCount()  { return m_nNumItem; }		//获取数据项的总数目
	INT				GetCurCount();
	INT				GetCurSel() { return m_nSelItemIndex;  }//得到当前选择的数据项的索引
	INT				SetCurSel(INT nSel);
	void			SetScrollbar(KWndScrollBar* pScroll);
	
	virtual const char*	GetShortKey() {return NULL;}	//取快捷键,用于Tip显示
	static void		SetAllButtonTipTextColor(UINT uColor);	//设置所有按钮提示名称文字的颜色
	static void		EnableAllButtonTip(INT bEnable);
	virtual INT		GetToolTipInfo(INT nIndex, char* pBuffer, INT nSize);
	INT		SetToolTipInfo(INT nIndex, char* pBuffer, INT nSize);
	
	KWndList2();
	virtual ~KWndList2();
protected:
	void			Clone(KWndList2* pCopy);
private:
	void			OnLButtonDown(INT x, INT y);			//响应鼠标左键按下
	void			OnLButtonDClick(INT x, INT y);			//响应鼠标左键按下
	void			OnMouseMove(INT x, INT y);				//响应鼠标在列表窗口上移动
	
private:
	void**			m_pContent;				//列表项内容数据
	INT				m_nItemSpace;			//可以保存的item的数目
	INT				m_nNumItem;				//数据项的数目

	INT				m_nNodeList;
	INT				m_nTopItemIndex;		//窗口顶部显示的数据项的索引
	INT				m_nSelItemIndex;		//选中的数据项的索引
	INT				m_nRowDis;

	INT				m_nLastData;
	UINT	m_NodeItemColor;			//显示数据项文本的颜色
	UINT	m_NodeItemBorderColor;		//显示数据项文本的边缘颜色
	UINT	m_ItemColor;			//显示数据项文本的颜色
	UINT	m_ItemBorderColor;		//显示数据项文本的边缘颜色
	UINT	m_SelItemColor;			//被选中的数据项文本的颜色
	UINT	m_SelItemBorderColor;	//被选中的数据项文本的边缘颜色
	UINT	m_uSelItemBgColor;
	UINT	m_uIconLineColor;
	UINT	m_uNodeLineColor;
	INT				m_nFontSize;			//显示字体大小

	KWndScrollBar  *m_pScrollbar;
private:
	static UINT	ms_uBtnTipTextColor;	//按钮提示名称文字的颜色
	static INT			ms_nDisableBtnTip;		//是否禁止按钮的提示文字
};