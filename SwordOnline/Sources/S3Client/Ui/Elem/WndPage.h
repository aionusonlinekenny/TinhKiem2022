/*****************************************************************************************
//	界面窗口体系结构--页面窗口\多个页面集合窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-8
------------------------------------------------------------------------------------------
    类似于M$ Win32里面的 PropertiePage
*****************************************************************************************/
#pragma once
#include "WndImage.h"
#include "WndShowAnimate.h"

class KWndPage : public KWndImage
{
public:
	virtual INT	WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
};

//------------------------------------------------------------------------------------------
//	多个页面集合窗口类似于M$ Win32里面的 PropertieSheet
//------------------------------------------------------------------------------------------

class KWndButton;

class KWndPageSet : public KWndShowAnimate
{
public:
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	bool			AddPage(KWndPage* pPage, KWndButton* pPageBtn);	//添加页面
	bool			ActivePage(INT nPageIndex);						//激活指定的页面
	INT				GetActivePageIndex() { return m_nAcitvePage; }	//取得当前激活页面的索引
	KWndPage*		GetActivePage();								//激活指定的页面
	KWndPageSet();
	~KWndPageSet();

private:
	void			OnPageBtnClick(KWndWindow* pBtn);		//响应按下切换页面按钮的操作

private:
	struct KWndPageBtnPair
	{
		KWndPage*	pPage;						//页面窗口指针
		KWndButton*	pPageBtn;					//页面窗口关联按钮指针
	}*				m_pPageBtnPairList;			//指向存储全部页面及其关联按钮指针的数组
	INT				m_nNumPage;					//页面的数目
	INT				m_nAcitvePage;				//当前激活的页面
};