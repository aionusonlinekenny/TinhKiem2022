#ifndef KPLAYERAI_H
#define KPLAYERAI_H
#include "KLinkArray.h"

enum
{
	Blood,
	Mana,
	TownPortal,
	TownPortal_l,
	AntiPoison,
	EnchaseExp,
	MecidineBag,
};

enum
{
	OverLookNothing,
	OverLookAllBoss,
	OverLookBlueBoss,
	OverLookGoldBoss,
};

enum
{
	Step_Training,
	Step_SellTrash,
	Step_Repair,
	Step_InventoryMoney,
	Step_InventoryItem,
	Step_WithDraw,
	Step_Return,
};

#ifndef _SEVER
class KEquipmentArray
{
public:
	KLinkArray	m_Link;
	INT		FindSame(INT nIdx);
};

class KPlayerAI
{
	friend class KPlayer;
public:
	UINT			m_uTotalTime;
	BOOL			m_bIsActive;
	INT				m_nLastFightMode;
	BOOL			m_bPriorityUseMouse;
	BOOL			m_bFightUseSB;
	BOOL			m_bHoldSpaceBar;
	BOOL			m_bEatLife;
	BOOL			m_bEatMana;
	UINT			m_uLifeTimeUse;
	UINT			m_uManaTimeUse;
	INT				m_nLifeAutoV;
	INT				m_nLifeAutoP;
	INT				m_nManaAutoV;
	INT				m_nManaAutoP;
	BOOL			m_bTPLife;
	INT				m_nTPLifeV;
	BOOL			m_bTPMana;
	INT				m_nTPManaV;
	BOOL			m_bTPNotMedicineBlood;
	BOOL			m_bTPNotMedicineMana;
	BOOL			m_bTPHightMoney;
	INT				m_nTPHightMoneyV;
	BOOL			m_bTPDamageEquip;
	INT				m_nTPDamageEquipV;
	BOOL			m_bTPNotEquipment;
	INT				m_nTPNotEquipmentV;
	BOOL			m_bAntiPoison;
	BOOL			m_bEnchaseExp;
	BOOL			m_bLifeReplenish;
	INT				m_nLifeReplenishP;
	BOOL			m_bInventoryMoney;
	BOOL			m_bInventoryItem;
	BOOL			m_bReturnPortal;
	INT				m_nReturnPortalSec;
	INT				m_nCurReturnPortalSec;
	BOOL			m_bFightNear;
	INT				m_nRadiusAuto;
	INT				m_nDistanceAuto;
	INT				m_nSupportSkills[MAX_AUTO_STATESKILL];
	INT				m_nFightSkill;
	INT				m_nFightBack;
	INT				m_nFightBoss;
	INT				m_nActiveAuraID1;
	INT				m_nActiveAuraID2;
	INT				m_nTargetIdx;
	UINT			m_uLifeCountDown;
	UINT			m_uManaCountDown;
	UINT			m_uPortalCountDown;
	UINT			m_uAntiPoisonCountDown;
	UINT			m_uEnchaseExpCountDown;
	UINT			m_uOpenMedicineCountDown;
	INT				m_nArrayNpcNeast[MAX_AUTO_SIZE];
	INT				m_nArrayNpcOverLook[MAX_AUTO_SIZE];
	INT				m_nCount_Attack_OverLook;
	INT				m_nArrayTimeNpcOverLook[MAX_AUTO_SIZE];
	INT				m_nArrayTimeObjectOverLook[MAX_AUTO_SIZE];
	INT				m_nArrayInvitePlayer[MAX_AUTO_SIZE];
	INT				m_nArrayTimeInvitePlayer[MAX_AUTO_SIZE];
	INT				m_nLifeOverLook;
	INT				m_nCount_Run_OverLook;
	INT				m_nObject;
	INT				m_nArrayObjectNeast[MAX_AUTO_SIZE];
	INT				m_nArrayObjectOverLook[MAX_AUTO_SIZE];
	BOOL			m_bPickFightNone;
	BOOL			m_bPickMoney;
	BOOL			m_bPickNotEquip;
	BOOL			m_bPickEquip;
	BOOL			m_bFollowPick;
	INT				m_nPickEquipKind;
	BOOL			m_bFilterEquipment;
	INT				m_nFilterMagic[MAX_AUTO_FILTERL][2];
	BOOL			m_bSaveTrash;
	INT				m_nSaveTrashPrice;
	BOOL			m_bSaveJewelry;
	BOOL			m_bAttackNpc;
	BOOL			m_bFollowPeople;
	char			m_szFollowName[32];
	INT				m_nFollowRadius;
	BOOL			m_bFollowRadius; // them check chay toa do
	BOOL			m_bAutoParty;
	BOOL			m_bAutoInvite;
	BOOL			m_bAutoInviteList;
	char			m_szPartyList[MAX_AUTO_LIST][32];
	BOOL			m_bOverTarget;
	BOOL			m_bFollowTarget;
	BOOL			m_bQuanhDiem; //Chay quanh diem
	BOOL			m_bOpenMedicine;
	BOOL 			m_bRepairEquip;
	BOOL			m_bSortEquipment;
	BOOL 			m_bSellEquipment;
	BOOL			m_bWithDrawMoney;
	INT				m_nWithDrawMoneyV;
	KEquipmentArray	m_sListEquipment;
	INT				m_nMissCastTime;

	//TamLTM get pos npc
	INT				m_nMovePosX;
	INT				m_nMovePosY;

	BOOL			m_bMoveMps;
	INT				m_nMoveMps[MAX_AUTO_LIST][3];
	INT				m_nMoveStep;
	INT				m_nMoveCount;

	INT				m_nReturnPortalStep;

	//TamLTM check Attack miss npc
	INT				m_AttackNumberMiss; // thoi gian tan cong
	INT				m_IgnoreAttackMiss; // Lo di khi tan cong miss
	INT				m_RadiusAttackMapLag; // Check lai pham vi goc lag
	INT				m_IgnoreCornerLag; // ne' goc lag auto
	BOOL			m_IsIgnoreMoveMpsLag; //Lo goc lag
	INT				m_nTargetHP;
	//Check Auto Run
	BOOL			m_bAutoRunMap;
	INT				m_nMapX;
	INT				m_nMapY;
	BOOL			m_bAutoRunFlagMap;
	INT				m_nFlagMapX;
	INT				m_nFlagMapY;
	//end code

	INT				m_nWayPointId;
	INT				m_nStand;

public:
	KPlayerAI() {};
	~KPlayerAI() {}
	void	Init();
	void	Release();
	void	Active();

	//TamLTM Move To Run
	void	MoveToRunPlayer();
	void	MoveQuanhDiem();

	void			ClearArrayNpcOverLook() { memset(m_nArrayNpcOverLook, 0, sizeof(m_nArrayNpcOverLook)); };
	void			ClearArrayObjectOverLook() { memset(m_nArrayObjectOverLook, 0, sizeof(m_nArrayObjectOverLook)); };
	void			ClearArrayTimeNpcOverLook() { memset(m_nArrayTimeNpcOverLook, 0, sizeof(m_nArrayTimeNpcOverLook)); };
	void			ClearArrayTimeObjectOverLook() { memset(m_nArrayTimeObjectOverLook, 0, sizeof(m_nArrayTimeObjectOverLook)); };
	void			ClearArrayNpcNeast() { memset(m_nArrayNpcNeast, 0, sizeof(m_nArrayNpcNeast)); };
	void			ClearArrayObjectNeast() { memset(m_nArrayObjectNeast, 0, sizeof(m_nArrayObjectNeast)); };
	void			ClearArrayInvitePlayer() { memset(m_nArrayInvitePlayer, 0, sizeof(m_nArrayInvitePlayer)); };
	void			ClearArrayTimeInvitePlayer() { memset(m_nArrayTimeInvitePlayer, 0, sizeof(m_nArrayTimeInvitePlayer)); };

	INT			FindFreeInvitePlayer()
	{
		for (INT i = 0; i < MAX_AUTO_SIZE; i++)
		{
			if (m_nArrayInvitePlayer[i] <= 0)
				return i;
		}
		return -1;
	};

	BOOL			FindSameInvitePlayer(INT uID)
	{
		for (INT i = 0; i < MAX_AUTO_SIZE; i++)
		{
			if (m_nArrayInvitePlayer[i] == uID)
				return TRUE;
		}
		return FALSE;
	};
	void			MoveTo(INT nX, INT nY);
	BOOL			FollowAttack(INT i);
	BOOL			CheckNpc(INT nIndex);
	BOOL			CheckObject(INT nObject);
	BOOL			CheckEquip(BYTE btDetail);
	BOOL			CheckPTList(const char* Name);
	BOOL			CheckEquipMagic(INT nIdx);
	void			SetHorse();
	void			SwitchAura();
	void			EatItem();
	BOOL			CastStateSkill();
	void			SetSortEquipment(BOOL bFlag);
	void			SortEquipment();
	void			FilterEquip();
	BOOL			MoveMps();
	void			InviteParty();
	BOOL			AddNpc2Array(INT nRelation);
	INT				GetNearNpcArray(INT nRelation);
	BOOL			AddObject2Array();
	INT				GetNearObjectArray();
	BOOL			FollowObject(INT nObject);
	BOOL			UseItem(INT type);
	BOOL			ReturnFromPortal();
	BOOL			InventoryItem();
	BOOL			SellItem();
	void			RepairEquip();
};
#endif
#endif