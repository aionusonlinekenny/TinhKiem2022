/*****************************************************************************************
//	界面窗口体系结构--列表窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-8
------------------------------------------------------------------------------------------
    列表窗口只列出文字。但是每项都可以关联其他数据。
	列表窗口设置的内容为一个数组，数组的每一个元素对应一个列表项。每个数组原素的存储空间大
小相同。每个数组元素的结构为一个定长的字符串或者为表含一个定长字符串成员的结构，字符串可以
处于结构中的任意位置。列表窗口类提供访问函数可以获取这些数组元素结构中的整数形成员的值。
    列表窗口所能列出的列表项数目小于数组元素数目时，可以设置列表窗口顶部从哪个数组元素开始
显示。
*****************************************************************************************/
#pragma once
#include "WndWindow.h"

#define	WNDLIST_ES_HALIGN_CENTRE		0x0001		//水平居中
#define	WNDLIST_ES_HALIGN_RIGHT			0x0002		//水平靠右对齐
#define	WNDLIST_ES_HALIGN_FILTER		(WNDLIST_ES_HALIGN_RIGHT | WNDLIST_ES_HALIGN_CENTRE)
#define	WNDLIST_ES_HIGHLIGHT_ENABLE		0x0004		//以不同颜色显示鼠标指针指向的项
#define WNDLIST_ES_MULTI_SEL			0x0008		//可以同时选中多项

#define	NO_CUSTOM_COLOR					0xffffffff

class KWndScrollBar;

class KWndList : public KWndWindow
{
public:
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);//初始化
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	virtual void	PaintWindow();							//绘制窗口
	//设置列表显示的内容
	void			SetContent(BYTE* pContent, INT nNumItem, INT nItemSize, INT nStringOffset);
	void			GetString(INT nItemIndex, char* pBuffer);//获取列表项的字符串内容
	INT				GetStrignLen(INT nItemIndex);			//获得列表项的字符串内容长度。
	//在列表中查找某个字符串。
	INT				FindString(INT nPrecedingStart, const char* pString);
	//获得列表项关联数据结构中某个整数型成员的值
	INT				GetItemDataInt(INT nItemIndex, INT nDataOffset);
	void			SetTopItemIndex(INT nTopItemIndex);		//设置列表窗口顶部显示的数据项的索引
	INT				GetTopItemIndex() { return m_nTopItemIndex; }//获得列表窗口顶部显示的数据项的索引
	INT				GetVisibleItemCount() const;			//获得列表框可以同时显示的项的数目
	INT				GetCount()  { return m_nNumItem; }		//获取数据项的总数目
	INT				GetCurSel() { return m_nSelItemIndex;  }//得到当前选择的数据项的索引
	INT				SetCurSel(INT nSel);
	INT				IsItemSel(INT nIndex);					//判断某项是否被选中
	void			SetCustomColorOffset(INT nOffset);
	void			EnableMultiSel(INT bEnable);			//启用/禁用多选
	void			SetScrollbar(KWndScrollBar* pScroll);
	void			SetSelItemBorderColor(UINT uColor)
	{
		m_SelItemBorderColor = uColor;
	}
	KWndList();
	~KWndList();
protected:
	void			Clone(KWndList* pCopy);
private:
	void			OnLButtonDown(INT x, INT y);			//响应鼠标左键按下
	void			OnRButtonDown(INT x, INT y);			//响应鼠标右键按下
	void			OnLButtonDClick(INT x, INT y);			//响应鼠标左键double click
	void			OnMouseMove(INT x, INT y);				//响应鼠标在列表窗口上移动
	INT				GetIemIndexAtPoint(INT x, INT y);		//获得的指定位置的选项的索引
	void			UpdateData();
private:
	KWndScrollBar*	m_pScrollbar;			//滚动条

	BYTE*			m_pContent;				//列表项内容数据
	INT				m_nNumItem;				//数据项的数目
	INT				m_nItemSize;			//数据项结构的存储空间宽度
	INT				m_nStringOffset;		//要显示的字符串在数据项结构中的偏移位置
	INT				m_nColorOffset;			//要显示的字符串的颜色在数据项结构中的偏移位置，
					//只当有WNDLIST_ES_CUSTOM_COLOR标记时，才有效果

	INT				m_nTopItemIndex;		//窗口顶部显示的数据项的索引
	INT				m_nSelItemIndex;		//选中的数据项的索引
	INT				m_nHighLightItemIndex;	//高亮度显示（鼠标指针下方）的数据项的索引

	UINT	m_ItemColor;			//显示数据项文本的颜色
	UINT	m_ItemBorderColor;		//显示数据项文本的边缘颜色
	UINT	m_SelItemColor;			//被选中的数据项文本的颜色
	UINT	m_SelItemBorderColor;	//被选中的数据项文本的边缘颜色
	UINT	m_HighLightColor;		//高亮度显示的数据项文本的颜色
	UINT	m_HighLightBorderColor;	//高亮度显示的数据项文本的边缘颜色
	UINT	m_uSelItemBgColor;		//选中项的背景色，如果值为0xffffffff表示无背景色
	INT				m_nFontSize;			//显示字体大小
	INT				m_nRowDis;
	
	INT				m_nNumColumn;			//可以同时显示几列数据
	UINT	m_uIsItemSelStatus;		//纪录每个项是否被选择
	UINT*	m_puItemSelStatusList;	//纪录序号32及以上的项是否被选择
	UINT	m_uNumSelStatusList;	//m_upItemSelStatusList包含的unsigned INT空间的数目
};