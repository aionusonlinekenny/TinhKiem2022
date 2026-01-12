/*****************************************************************************************
//	界面窗口体系结构--滚动条窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-22
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "WndButton.h"

#define WNDSCROLL_ES_VERTICAL		0x0001		//垂直型的滚动条
#define	WNDSCROLL_F_DRAGGING_SLIDE	0x0100		//滚动条的滑块正在被拖动

class KWndScrollBar : public KWndImage
{
private:
	UINT	m_Flag;
	INT			m_nMinValue;	//最小值
	INT			m_nMaxValue;	//最大值
	INT			m_nCurValue;	//当前值
	//	INT			m_nLineSize;	//一行大小
	INT			m_nPageSize;	//一页大小
	INT			m_nMinPosition;	//滑动活动起始位置
	INT			m_nMaxPosition;	//滑动活动终止位置
	INT			m_nImgRange;	//重复贴图时，贴图的宽度或高度
public:
	KWndButton	m_SlideBtn;		//滑动按钮
public:
	KWndScrollBar();
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);	//初始化
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	virtual void	PaintWindow();							//窗体绘制
	void			SetSize(INT nWidth, INT nHeight);		//设置窗口大小

	INT				ScrollLine(bool bPre);
	INT				ScrollPage(bool bPre);
	void			SetScrollPos(INT nPosition);				//设置位置
	INT				GetScrollPos() { return m_nCurValue; }		//获取位置
	void			SetValueRange(INT nMinValue, INT nMaxValue);	//设置取值范围
	INT				GetMaxValue() { return m_nMaxValue; }	//取值范围
	INT				GetMinValue() { return m_nMinValue; }	//取值范围
	void			Clone(KWndScrollBar* pCopy);
	INT				GetMinHeight();
	BOOL			IsDraggingSlide() { return (m_Flag & WNDSCROLL_F_DRAGGING_SLIDE); };
private:
	void			OnLButtonDown(INT x, INT y);					//响应鼠标左键按下
	void			OnSlideBtnPressed();						//响应滑动按钮被按下
	void			OnDraggingSlide(INT x, INT y);				//正在拖动滑动按钮
	void			SetSlideBtnPos();							//设置滑动块窗口位置
};
