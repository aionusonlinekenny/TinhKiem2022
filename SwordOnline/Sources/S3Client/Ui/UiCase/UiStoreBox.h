/*****************************************************************************************
//	界面--储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"

struct KUiObjAtRegion;

class KUiStoreBox : protected KWndShowAnimate
{
public:
	static KUiStoreBox* OpenWindow(INT nToPage);		//打开窗口，返回唯一的一个类对象实例
	static KUiStoreBox* GetIfVisible();		//如果窗口正被显示，则返回实例指针
	static void			CloseWindow();		//关闭窗口
	static void			LoadScheme(const char* pScheme);//载入界面方案
	void			UpdateItem(INT nToPage, KUiObjAtRegion* pItem, INT bAdd);//物品变化更新
private:
	KUiStoreBox() {}
	~KUiStoreBox() {}
	void	Initialize();							//初始化
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	void	UpdateData(INT nCurPage);
	void	OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void	OnClickItem(KUiDraggedObject* pItem);
	void	OnGetMoney(INT nMoney);
	void	PaintWindow();
private:
	static KUiStoreBox* m_pSelf;
private:
	INT					m_nMoney;
	KWndText32			m_Money;
	KWndImageTextButton			m_GetMoneyBtn;
	KWndImageTextButton			m_CloseBtn;
	KWndImageTextButton	m_UnlockBtn;
	KWndObjectMatrix	m_ItemBox;
	KWndText32			m_CurPage;
	KWndButton			m_PreBtn;
	KWndButton			m_NextBtn;

	INT			m_nExpandBoxNum;
	INT			m_nCurPage;
};