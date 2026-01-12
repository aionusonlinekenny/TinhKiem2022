#ifndef	KNpcSetH
#define	KNpcSetH

//#include "KFindBinTree.h"
#include "KLinkArray.h"
#include "KNpc.h"
#include "Scene/SceneDataDef.h"
#include "GameDataDef.h"
#include <map>

#ifndef _SERVER
#define		MAX_NPC_REQUEST	20
#endif
#include "KNpcTemplate.h"
#include "KNpcGold.h"

/*
enum NPC_ENCHANT
{
	enchant_treasure = 0,				// 掉的装备更多
	enchant_aura,						// 光环技能
	enchant_lifeenhance,				// 生命更长
	enchant_lifereplenish,				// 自动回血
	enchant_attackratingenhance,		// 打得更准
	enchant_defenseenhance,				// 防御加强
	enchant_damageenhance,				// 伤害更高
	enchant_speedenhance,				// 速度更快
	enchant_selfresist,					// 自己的五行属性的抗性为100%
	enchant_conquerresist,				// 相生的五行属性的抗性为100%
	enchant_num,
};
*/

#ifndef _SERVER
#define		MAX_INSTANT_STATE	20
#define		MAX_INSTANT_SOUND	20
class	KInstantSpecial
{
private:
	INT		m_nLoadFlag;
	char	m_szSprName[MAX_INSTANT_STATE][FILE_NAME_LENGTH];
	char	m_szSoundName[MAX_INSTANT_SOUND][FILE_NAME_LENGTH];

	KCacheNode* m_pSoundNode;	// 声效指针
	KWavSound* m_pWave;		// 声效wav指针

private:
	void	LoadSprName();
	void	LoadSoundName();

public:
	KInstantSpecial();
	void	GetSprName(INT nNo, char* lpszName, INT nLength);
	void	PlaySound(INT nNo);
};
#endif

typedef struct
{
	DWORD	dwRequestId;
	DWORD	dwRequestTime;
} RequestNpc;

typedef struct
{
#ifndef _SERVER
	INT		nStandFrame[2];
	INT		nWalkFrame[2];
	INT		nRunFrame[2];
#endif
	INT		nWalkSpeed;
	INT		nRunSpeed;
	INT		nAttackFrame;
	INT		nHurtFrame;
} PlayerBaseValue;

class KNpcSet
{
public:
	KNpcGoldTemplate		m_cGoldTemplate; // Gold monster template
	PlayerBaseValue			m_cPlayerBaseValue;					// 玩家标准数据

#ifdef _SERVER
	INT					m_nPKDamageRate;					// PK时伤害乘一个系数
	INT					m_nLevelPKDamageRate;					// PK时伤害乘一个系数
	INT					m_nNpcSpecialDamageRate;					// PK时伤害乘一个系数
	INT					m_nFactionPKFactionAddPKValue;		// 三大阵营之间PK，等级差太大时PK者PK值增加
	INT					m_nKillerPKFactionAddPKValue;		// 杀手与三大阵营PK，等级差太大时PK者PK值增加
	INT					m_nEnmityAddPKValue;				// 仇杀时PK者PK值增加
	INT					m_nBeKilledAddPKValue;				// 被PK致死着PK值增加，应该是个负数
	INT					m_nLevelDistance;					// 等级差多少算是PK新手
	INT					m_nLevelBoundaryPKPunish;					// 等级差多少算是PK新手
	INT					m_nButcherPKExercise;					// 等级差多少算是PK新手
	INT					m_nNotSubPKExpPercent;					// 等级差多少算是PK新手
	INT					m_nNotEnmityExpPercent;					// 等级差多少算是PK新手
	INT					m_nNotFightExpPercent;					// 等级差多少算是PK新手
	INT					m_nMurderAddPKValue;
	INT					m_nPKWarOpen;
	INT					m_nPKMurderOpen;
	INT					m_nPKTongOpen;
	INT					m_nPKTongOpenCamp;
	INT					m_nNormalPKTimeLong;
	INT					m_nFightPKTimeLong;
	INT					m_nNotFightWhenHightPK;
	INT					m_nPKNotSwitchPKWhenLock;
	INT					m_nSparPacific;
#endif
#ifndef _SERVER
	KInstantSpecial		m_cInstantSpecial;
#endif
private:
	DWORD				m_dwIDCreator;						//	游戏世界中的ID计数器
	KLinkArray			m_FreeIdx;							//	可用表
	KLinkArray			m_UseIdx;							//	已用表
	typedef std::map<DWORD, KNpcTemplate*> _KMapTemplate;
	_KMapTemplate m_mapTemplate;
#ifndef _SERVER
	enum
	{
		PATE_NAME = 0x01,
		PATE_NAMEF = 0x02,
		PATE_LIFE = 0x04,
		PATE_LIFEF = 0x08,
		PATE_OBJ = 0x10,
	};
	INT					m_nShowPateFlag;					// 是否全部显示玩家的名字在头顶上 zroc add
	RequestNpc			m_RequestNpc[MAX_NPC_REQUEST];		//	向服务器申请的ID表
	KLinkArray			m_RequestFreeIdx;					//	向服务器申请可用表
	KLinkArray			m_RequestUseIdx;					//	向服务器申请空闲表
#endif
public:
	KNpcSet();
	void				Init();
	INT					GetNpcCount(INT nKind = -1, INT nCamp = -1);
	INT					SearchName(LPSTR szName);
	INT					SearchID(DWORD dwID);
	INT					SearchUUID(DWORD dwID);
	BOOL				IsNpcExist(INT nIdx, DWORD dwId);
	INT					AddServerNpcB(INT nNpcSetingIdxInfo, INT nSubWorld, INT nRegion, INT nMapX, INT nMapY, INT nOffX = 0, INT nOffY = 0, INT nBoss = 0);
	INT					AddServerNpcA(INT nNpcSetingIdxInfo, INT nSubWorld, INT nMpsX, INT nMpsY, INT nBoss = 0, INT nRodom = 0, INT nOffwminx = 1, INT nOffwmaxx = 1, INT nOffhminy = 1, INT nOffhmaxy = 1);
	INT					Add(INT nNpcSetingIdxInfo, INT nSubWorld, INT nRegion, INT nMapX, INT nMapY, INT nOffX, INT nOffY, INT nBoss = 0);
	INT					Add(INT nNpcSetingIdxInfo, INT nSubWorld, INT nMpsX, INT nMpsY, BOOL bBarrier = FALSE, INT nBoss = 0);
	INT					Add(INT nSubWorld, void* pNpcInfo, KMapList* pMapListInfo);
	void				Remove(INT nIdx);
	void				RemoveAll();
	INT					GetNpcNumber() { return m_UseIdx.GetCount(); };
	NPC_RELATION		GetRelation(INT nIdx1, INT nIdx2);
	INT					GetNearestNpc(INT nMapX, INT nMapY, INT nId, INT nRelation);
	static INT			GetDistance(INT nIdx1, INT nIdx2);
	static INT			GetDistanceSquare(INT nIdx1, INT nIdx2);
	INT					GetNextIdx(INT nIdx);
	// 把所有npc的 bActivateFlag 设为 FALSE (每次游戏循环处理所有npc的activate之前做这个处理)
	void				ClearActivateFlagOfAllNpc();
	void				LoadPlayerBaseValue(LPSTR szFile);
	INT					GetPlayerWalkSpeed() { return m_cPlayerBaseValue.nWalkSpeed; };
	INT					GetPlayerRunSpeed() { return m_cPlayerBaseValue.nRunSpeed; };
	INT					GetPlayerAttackFrame() { return m_cPlayerBaseValue.nAttackFrame; };
	INT					GetPlayerHurtFrame() { return m_cPlayerBaseValue.nHurtFrame; };

	BOOL				CheckThisNpc(INT nSetings, char* nKey);

	KNpcTemplate* GetTemplate(INT nNpcTemplateId, INT nLevel);
#ifndef _SERVER
	INT					GetPlayerStandFrame(BOOL bMale)
	{
		if (bMale)
			return m_cPlayerBaseValue.nStandFrame[0];
		else
			return m_cPlayerBaseValue.nStandFrame[1];
	};
	INT				GetPlayerWalkFrame(BOOL bMale)
	{
		if (bMale)
			return m_cPlayerBaseValue.nWalkFrame[0];
		else
			return m_cPlayerBaseValue.nWalkFrame[1];
	};
	INT				GetPlayerRunFrame(BOOL bMale)
	{
		if (bMale)
			return m_cPlayerBaseValue.nRunFrame[0];
		else
			return m_cPlayerBaseValue.nRunFrame[1];
	};
	BOOL				IsNpcRequestExist(DWORD	dwID);
	void				InsertNpcRequest(DWORD dwID);
	void				RemoveNpcRequest(DWORD dwID);
	INT					GetRequestIndex(DWORD dwID);
	// 添加一个客户端npc（需要设定ClientNpcID）
	INT					AddClientNpc(INT nTemplateID, INT nRegionX, INT nRegionY, INT nMpsX, INT nMpsY, INT nNo);
	// 从npc数组中寻找属于某个region的 client npc ，添加进去
	void				InsertNpcToRegion(INT nRegionIdx);
	// 查找某个ClientID的npc是否存在
	INT					SearchClientID(KClientNpcID sClientID);
	// 某座标上精确查找Npc，客户端专用
	INT					SearchNpcAt(INT nX, INT nY, INT nRelation, INT nRange);
	void				CheckBalance();
	INT					GetAroundPlayerForTeamInvite(KUiPlayerItem* pList, INT nCount);	// 获得周围玩家列表(用于队伍邀请列表)
	void				GetAroundOpenCaptain(INT nCamp);		// 获得周围同阵营的已开放队伍队长列表
	INT					GetAroundPlayer(KUiPlayerItem* pList, INT nCount);	// 获得周围玩家列表(用于列表)

	// 设定是否全部显示玩家的名字  bFlag ==	TRUE 显示，bFlag == FALSE 不显示 zroc add
	void				SetShowNameFlag(BOOL bFlag);
	// 判断是否全部显示玩家的名字  返回值 TRUE 显示，FALSE 不显示
	BOOL				CheckShowName();
	// 设定是否全部显示玩家的血  bFlag ==	TRUE 显示，bFlag == FALSE 不显示 zroc add
	void				SetShowLifeFlag(BOOL bFlag);
	// 判断是否全部显示玩家的血  返回值 TRUE 显示，FALSE 不显示
	BOOL				CheckShowLife();
	// 设定是否全部显示玩家的内力  bFlag ==	TRUE 显示，bFlag == FALSE 不显示 zroc add
	void				SetShowObjFlag(BOOL bFlag);
	// 判断是否全部显示玩家的内力  返回值 TRUE 显示，FALSE 不显示
	BOOL				CheckShowObj();

#endif
#ifdef _SERVER
	BOOL				SyncNpc(DWORD dwID, INT nClientIdx);
	BOOL				ExecuteScript(INT nNpcIndex, char* szScriptName, char* szFuncName, INT nParam);
	BOOL				ExecuteScript(INT nNpcIndex, DWORD dwScriptId, char* szFuncName, INT nParam);
#endif
private:
	void				SetID(INT m_nIndex);
	INT					FindFree();

	// Add By Freeway Chen in 2003.7.14
private:
	unsigned char m_RelationTable[kind_num][kind_num][camp_num][camp_num];
	INT GenRelationTable();
	NPC_RELATION GenOneRelation(NPCKIND Kind1, NPCKIND Kind2, NPCCAMP Camp1, NPCCAMP Camp2);
};

// modify by Freeway Chen in 2003.7.14
// 确定两个NPC之间的战斗关系

extern KNpcSet NpcSet;
#endif
