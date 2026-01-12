/*****************************************************************************************
//	界面--道具界面
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndImage.h"
#include "../elem/WndText.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"
#include "../../../core/src/GameDataDef.h"

struct KUiObjAtRegion;

class KUiItem : public KWndShowAnimate
{
public:
	static KUiItem* OpenWindow(bool bFlag = true);				//打开窗口，返回唯一的一个类对象实例
	static KUiItem* GetIfVisible();				//如果窗口正被显示，则返回实例指针
	static void		CloseWindow(bool bDestroy);	//关闭窗口，同时可以选则是否删除对象实例
	static void		LoadScheme(const char* pScheme);//载入界面方案
	static void		OnNpcTradeMode(bool bTrue);
	void			UpdateItem(KUiObjAtRegion* pItem, INT bAdd);//物品变化更新
	void			Breathe();									//活动函数
private:
	KUiItem() {}
	~KUiItem() {}
	void	Initialize();							//初始化
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	void	UpdateData();
	void	OnClickItem(KUiDraggedObject* pItem, bool bDoImmed);
	void	OnRepairItem(KUiDraggedObject* pItem);
	void	OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void	OnGetMoney(INT nMoney);
	void	OnBreakItem(KUiDraggedObject* pItem, bool bDoImmed);
	void	OnSetPrice(KUiDraggedObject* pItem);
private:
	static KUiItem* m_pSelf;
private:
	INT					m_nMoney;
	KWndText256			m_Money;
	KWndText256			m_Gold;
	KWndImageTextButton			m_GetMoneyBtn;
	KWndImage			m_TitleIcon, m_MoneyIcon, m_GoldIcon;
	KWndImageTextButton			m_CloseBtn;
	KWndObjectMatrix	m_ItemBox;
	KWndImageTextButton			m_OpenStatusPadBtn;
	KWndImageTextButton			m_MakeAdvBtn;
	KWndImageTextButton			m_MarkPriceBtn;
	KWndImageTextButton			m_MakeStallBtn;
	BYTE				m_byMark;

	char				m_szAdvStr[16];
};