#ifndef	KItemListH
#define	KItemListH

#include "KLinkArray.h"
#include "KItem.h"
#define	MAX_ITEM_ACTIVE	2


//#define		EQUIPMENT_ROOM_WIDTH		6
//#define		EQUIPMENT_ROOM_HEIGHT		10
//#define		REPOSITORY_ROOM_WIDTH		6
//#define		REPOSITORY_ROOM_HEIGHT		10
//#define		TRADE_ROOM_WIDTH			6
//#define		TRADE_ROOM_HEIGHT			10


#define	REQUEST_EAT_MEDICINE	1
#define	REQUEST_EAT_OTHER		2
#define	REQUEST_EQUIP_ITEM 		3

typedef struct
{
	INT		nPlace;
	INT		nX;
	INT		nY;
} ItemPos;


class CORE_API KItemList
{
private:
	INT			m_PlayerIdx;
	INT			m_Hand;
	INT			m_nBackHand;
	INT			m_CompoundItem[MAX_COMPOUND_ITEM];

	static INT	ms_ActiveEquip[itempart_num][MAX_ITEM_ACTIVE];	// 某个部分的装备所激活的装备
	static INT	ms_ActivedEquip[itempart_num][MAX_ITEM_ACTIVE];	// 某个部分的装备被什么装备激活
	KLinkArray	m_FreeIdx;
	KLinkArray	m_UseIdx;
	INT			m_nListCurIdx;									// 用于 GetFirstItem 和 GetNextItem
	BOOL		m_bMaskLock;
	INT			m_BuildItem[MAX_PART_BUILD]; //TamLTM kham nam xanh
	//#ifndef _SERVER
	BOOL		m_bLockOperation;
	//#endif
public:
	PlayerItem	m_Items[MAX_PLAYER_ITEM];						// 玩家拥有的所有道具（包括装备着的和箱子里放的，对应游戏世界中道具数组的索引）
	PlayerItem	m_sBackItems[MAX_PLAYER_ITEM];					// 交易过程中 m_Items 的备份
	KInventory	m_Room[room_num];
	INT			m_EquipItem[itempart_num];						// 玩家装备的道具（对应游戏世界中道具数组的索引）
	INT			FindSame(INT nGameIdx);							// nGameIdx指游戏世界中道具数组的编号
	INT			FindSame(DWORD dwID);
private:
	INT			FindFree();
	BOOL		Fit(INT nIdx, INT nPlace);						// 检查是否可以装备上
	BOOL		Fit(KItem* pItem, INT nPlace);
	INT			GetEquipPlace(INT nType);						// 取得某类型装备应该放的位置
	INT			GetEquipEnhance(INT nPlace);
	INT			GetGoldEquipEnhance(INT nPlace);
	INT			GetActiveEquipPlace(INT nPlace, INT nCount);	// 取得nPlace的装备激活的第nCount个装备位置
	void		InfectionNextEquip(INT nPlace, BOOL bEquip = FALSE);
#ifdef _SERVER
	BOOL		FindSameToRemove(INT nItemNature, INT nItemGenre, INT nDetailType, INT nLevel, INT nSeries, INT nRoom, INT* pnIdx);
#endif
	friend	class KPlayer;

public:
	KItemList();
	~KItemList();
	INT			Init(INT nIdx);
	INT			GetEquipment(INT nIdx) { return m_EquipItem[nIdx]; }
	INT			GetActiveAttribNum(INT nIdx);
	INT			GetGoldActiveAttribNum(INT nIdx);
	INT			GetWeaponType();								// 取得武器的类型，近程武器(equip_meleeweapon)还是远程武器(equip_rangeweapon)
	INT			GetWeaponParticular();							// 取得武器的具体类型，用于计算不同的伤害加强
	void		GetWeaponDamage(INT* nMin, INT* nMax);			// 取得武器的伤害
	INT			Add(INT nIdx, INT nPlace, INT nX, INT nY, bool bAutoStack = true);		// nIdx指游戏世界中道具数组的编号
	INT			Add(INT nIdx, POINT ItemSize, bool bAutoStack = true);
	BOOL		SearchPosition(POINT ItemSize, ItemPos* pPos);
	BOOL		SearchPosition(INT nWidth, INT nHeight, ItemPos* pPos, bool bOverLookHand = false);
	BOOL		CanCombie(INT Source, INT Dest);

	BOOL		CompareRemoveItem(INT Source, INT Dest);
	BOOL		Remove(INT nIdx);								// nIdx指游戏世界中道具数组的编号
	void		RemoveAll();
	BOOL		CanEquip(INT nIdx, INT nPlace = -1);			// nIdx指游戏世界中道具数组的编号
	BOOL		CanEquip(KItem* pItem, INT nPlace = -1);
	BOOL		EnoughAttrib(void* pData);
	BOOL		Equip(INT nIdx, INT nPlace = -1);				// nIdx指游戏世界中道具数组的编号
	BOOL		UnEquip(INT nIdx, INT nPlace = -1);				// nIdx指游戏世界中道具数组的编号
	BOOL		NowEatItem(INT nIdx);							// nIdx指游戏世界中道具数组的编号
	PlayerItem* GetFirstItem();
	PlayerItem* GetNextItem();
	INT			SearchID(INT nID);
	void		ExchangeMoney(INT nSrcRoom, INT DesRoom, INT nMoney);
	void		WithDrawaMoney(INT nSrcRoom, INT DesRoom, INT nMoney); // rut tien + pass ruong;
	void		ExchangeItem(ItemPos* SrcPos, ItemPos* DesPos);
	INT			GetMoneyAmount();					// 得到物品栏和储物箱的总钱数
	INT			GetRepositoryMoney();
	INT			GetEquipmentMoney();				// 得到物品栏和储物箱的钱数
	INT			GetTradeMoney();				// 得到物品栏和储物箱的钱数
	BOOL		AddMoney(INT nRoom, INT nMoney);
	BOOL		CostMoney(INT nMoney);
	BOOL		DecMoney(INT nMoney);
	void		SetMoney(INT nMoney1, INT nMoney2, INT nMoney3);
	void		SetRoomMoney(INT nRoom, INT nMoney);
	INT			GetMoney(INT nRoom) { return m_Room[nRoom].GetMoney(); }	// 取得空间的钱
	void		SetPlayerIdx(INT nIdx);				// 设定玩家索引
	INT			Hand() { return m_Hand; };
	void		ClearRoom(INT nRoom);
	void		BackupTrade();
	void		RecoverTrade();
	void		StartTrade();
	INT			GetHorseIdx() { return m_EquipItem[itempart_horse]; };
	BOOL		CompareItemList(const KBASICPROP_EQUIPMENT*);
	INT			GetItemNum(INT nItemGenre, INT nDetailType, INT nParticularType, INT nLevel);
	BOOL		GetMaskLock() { return m_bMaskLock; };
	void		SetMaskLock(BOOL bFlag);
	INT			GetCompoundItem(INT nIdx) { return m_CompoundItem[nIdx]; }
	BOOL		PutCompound(INT nIdx, INT nPlace = -1);
	void		DropCompound(INT nIdx, INT nPlace = -1);
	void    	ClearRoomItemOnly(INT nRoom)
	{
		if (nRoom >= 0 && nRoom < room_num)
			this->m_Room[nRoom].ClearItem();
	}
	void		RemoveRoom(INT nRoom);
	INT			CalcFreeItemCellCount(INT nWidth, INT nHeight, INT nRoom);
	INT			GetBuildItem(INT nIdx) { return m_BuildItem[nIdx]; } //TamLTM kham nam xanh
	BOOL		BuildItem(INT nIdx, INT nPlace = -1);	//TamLTM kham nam xanh
	void		UnBuildItem(INT nIdx, INT nPlace = -1); //TamLTM kham nam xanh
	BOOL		IsEnoughToActive();
	INT			GetPlayerFortune();
	INT			HaveDamageItem(INT nDur = 3);
	BOOL		CheckCanPlaceInEquipment(INT nWidth, INT nHeight, INT* pnP, INT* pnX, INT* pnY);// 判断一定长宽的物品能否放进物品栏
	INT			CountCommonItem(INT nItemNature, INT nItemGenre, INT nDetailType = -1, INT nLevel = -1, INT nSeries = -1, INT P = pos_equiproom);
	INT			GetItemCountRoom(INT P = pos_equiproom);
	INT			FindItem(INT nItemNature, INT nItemGenre, INT nDetailType = -1, INT nLevel = -1, INT nSeries = -1);
	INT			FindItemByIndex(INT nItemNature, INT nIndex);
#ifdef	_SERVER
	void		Abrade(INT nType);
	void		TradeMoveMoney(INT nMoney);			// 调用此接口必须保证传入的nMoney是一个有效数(正数且不超过所有钱数)
	void		SendMoneySync();					// 服务器发money同步信息给客户端
	INT			RemoveCommonItem(INT nCount, INT nItemNature, INT nItemGenre = -1, INT nDetailType = -1, INT nLevel = -1, INT nSeries = -1, INT P = pos_equiproom);
	void		GetTradeRoomItemInfo();				// 交易中把 trade room 中的 item 的 idx width height 信息写入 itemset 中的 m_psItemInfo 中去
	BOOL		TradeCheckCanPlace();				// 交易中判断买进的物品能不能完全放进自己的物品栏
	BOOL		EatItem(INT nPlace, INT nX, INT nY);		// 吃什么地方的药
	// 自动把一个药品从room_equipment移动到room_immediacy
	BOOL		AutoMoveItemFromEquipmentRoom(INT nItemIdx, INT nSrcX, INT nSrcY, INT nDestX, INT nDestY);
	// 丢失随身物品
	void		AutoLoseItemFromEquipmentRoom(INT nRate);
	// 丢失一个穿在身上的装备(马不会掉)
	void		AutoLoseEquip();

	BOOL		RemoveItem(INT nItemGenre, INT nDetailType, INT nParticularType, INT nLevel);
	BOOL		RemoveItem(INT nIdx, INT nNum);
	BOOL		Lock(INT nIdx, BOOL bLock);
	void		SyncItem(INT nIdx, BOOL bIsNew = FALSE, INT nPlace = 0, INT nX = 0, INT nY = 0, INT nPlayerIndex = 0);
	void		SyncItemMagicAttrib(INT nIdx);

	void		BackLocal();
	void		InsertEquipment(INT nIdx, bool bAutoStack = false);
	void		ExecuteScript(INT nIndex);
	BOOL		IsLockOperation() { return m_bLockOperation; };
	void        MsgShow(INT MsgID); //47
#endif
	INT			PositionToIndex(INT P, INT i);


#ifndef	_SERVER
	INT			UseItem(INT nIdx);					// nIdx指游戏世界中道具数组的编号
	BOOL		SearchEquipment(INT nWidth, INT nHeight);
	BOOL		SearchStoreBox(INT nRepositoryNum, INT nWidth, INT nHeight, ItemPos* pPos);
	BOOL		AutoMoveItem(ItemPos SrcPos, ItemPos DesPos);
	void		MenuSetMouseItem();
	INT			GetSameDetailItemNum(INT nImmediatePos);
	void		LockOperation();										// 锁定客户端对装备的操作
	void		UnlockOperation();
	BOOL		IsLockOperation() { return m_bLockOperation; };
	INT			ChangeItemInPlayer(INT nIdx);	//edit by phong kieu mac trang bi vao nguoi	
#endif
};
#endif
