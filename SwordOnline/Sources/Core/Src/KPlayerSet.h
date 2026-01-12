#ifndef KPlayerSetH
#define	KPlayerSetH

#include "KLinkArray.h"

typedef struct
{
	INT		m_nExpP;
	INT		m_nExpV;
	INT		m_nMoney;
	INT		m_nItem;
	INT		m_nEquip;
	INT		m_nAbradeP;
} KPK_DEATH_PUNISH_PARAM;

class KLevelAdd
{
private:
	INT		m_nLevelExp[MAX_LEVEL];				// 升级经验
	INT		m_nLevelExpAdd[MAX_TRANSLIFE_VALUE][MAX_LEVEL];				// 升级经验
	INT		m_nLifePerLevel[MAX_SERIES];			// 每个系升级加生命点
	INT		m_nStaminaMalePerLevel[MAX_SERIES];			// 每个系升级加体力点
	INT		m_nStaminaFemalePerLevel[MAX_SERIES];			// 每个系升级加体力点
	INT		m_nManaPerLevel[MAX_SERIES];			// 每个系升级加内力点
	INT		m_nLifePerVitality[MAX_SERIES];			// 每个系活力点增加一点后生命点增长
	INT		m_nStaminaPerVitality[MAX_SERIES];		// 每个系活力点增加一点后体力点增长
	INT		m_nManaPerEnergy[MAX_SERIES];			// 每个系精力点增加一点后内力点增长
	INT		m_nLeadExpShare[MAX_SERIES];			// 每个系获得战斗经验时分配统率力经验的参数
	INT		m_nFireResistPerLevel[MAX_SERIES];		// 每个系升级加火抗性
	INT		m_nColdResistPerLevel[MAX_SERIES];		// 每个系升级加冰抗性
	INT		m_nPoisonResistPerLevel[MAX_SERIES];	// 每个系升级加毒抗性
	INT		m_nLightResistPerLevel[MAX_SERIES];		// 每个系升级加电抗性
	INT		m_nPhysicsResistPerLevel[MAX_SERIES];	// 每个系升级加物理抗性
	INT		m_nStaminaMaleBase[MAX_SERIES];	// 每个系升级加物理抗性
	INT		m_nStaminaFemaleBase[MAX_SERIES];	// 每个系升级加物理抗性
public:
	KLevelAdd();
	BOOL		Init();
	INT			GetLevelExp(INT nLevel, INT nTranslife);
	INT			GetLifePerLevel(INT nSeries);
	INT			GetStaminaPerLevel(INT nSex, INT nSeries);
	INT			GetManaPerLevel(INT nSeries);
	INT			GetLifePerVitality(INT nSeries);
	INT			GetStaminaPerVitality(INT nSeries);
	INT			GetManaPerEnergy(INT nSeries);
	INT			GetLeadExpShare(INT nSeries);
	INT			GetFireResist(INT nSeries, INT nLevel);
	INT			GetColdResist(INT nSeries, INT nLevel);
	INT			GetPoisonResist(INT nSeries, INT nLevel);
	INT			GetLightResist(INT nSeries, INT nLevel);
	INT			GetPhysicsResist(INT nSeries, INT nLevel);
	INT			GetStaminaBase(INT nSex, INT nSeries);
};

class KMagicLevelExp
{
private:
	KTabFile	m_MagicLevelExpFile;
public:
	KMagicLevelExp();
	BOOL		Init();

	INT			GetNextExp(INT nSkillId, INT nLevel);
};

class KTeamLeadExp
{
public:
	struct LEADLEVEL
	{
		DWORD	m_dwExp;								// 统率力升级经验
		DWORD	m_dwMemNum;								// 统率等级决定能统率的队员人数
	}	m_sPerLevel[MAX_LEAD_LEVEL];
public:
	KTeamLeadExp();
	BOOL		Init();									// 初始化
	INT			GetLevel(DWORD dwExp, INT nCurLeadLevel = 0);// 传入经验获得等级数 (如果传入当前等级，可以加快搜索速度)
	INT			GetMemNumFromExp(DWORD dwExp);			// 传入经验获得可带队员数
	INT			GetMemNumFromLevel(INT nLevel);			// 传入等级获得可带队员数
	INT			GetLevelExp(INT nLevel);				// 传入等级获得升级所需经验值
};

class KNewPlayerAttribute
{
public:
	INT			m_nStrength[series_num];
	INT			m_nDexterity[series_num];
	INT			m_nVitality[series_num];
	INT			m_nEngergy[series_num];
	INT			m_nLucky[series_num];
public:
	KNewPlayerAttribute();
	BOOL		Init();
};

class KPlayerStamina
{
public:
	INT			m_nNormalAdd;
	INT			m_nExerciseRunSub;
	INT			m_nFightRunSub;
	INT			m_nKillRunSub;
	INT			m_nTongWarRunSub;
	INT			m_nSitAdd;
public:
	KPlayerStamina();
	BOOL		Init();
};

class KPlayerSet
{
private:	// 用于优化查找速度
	KLinkArray		m_FreeIdx;				//	可用表
	KLinkArray		m_UseIdx;				//	已用表
	INT				m_nListCurIdx;			// 用于 GetFirstPlayer 和 GetNextPlayer
#ifdef _SERVER
	ULONG	m_ulNextSaveTime;
	ULONG	m_ulDelayTimePerSave;	//1mins
	ULONG	m_ulMaxSaveTimePerPlayer; //30mins
#endif

public:
	KLevelAdd				m_cLevelAdd;
	KMagicLevelExp			m_cMagicLevelExp;
	KTeamLeadExp			m_cLeadExp;
	KNewPlayerAttribute		m_cNewPlayerAttribute;
	KPlayerStamina			m_cPlayerStamina;
#ifndef _SERVER
	char					m_szNationalEmblemPic[MAX_TONG_NATIONALEMBLEM][MAX_PATH];
	char					m_szViprankPic[MAX_VIPRANK_VALUE + 1][MAX_PATH];
	char					m_szFortuneRankPic[MAX_ITEM_LEVEL + 1][MAX_PATH];
	char					m_szTranlifePic[MAX_TRANSLIFE_VALUE + 1][MAX_PATH];
#endif

#ifdef _SERVER
	KPK_DEATH_PUNISH_PARAM	m_sPKPunishParam[MAX_DEATH_PUNISH_PK_VALUE + 1];	// PK惩罚参数
#endif

public:
	KPlayerSet();
	~KPlayerSet();
	BOOL	Init();
	INT		FindSame(DWORD dwID);
	INT		GetFirstPlayer();				// 遍历所有玩家第一步
	INT		GetNextPlayer();				// 遍历所有玩家下一步(这支函数必须在上一支调用之后才能调用)
	INT		GetOnlinePlayerCount() { return m_UseIdx.GetCount(); }
	INT		FindClient(INT nClient);

#ifdef	_SERVER
	void	Activate();
	INT		Add(LPSTR szPlayerID, void* pGuid);
	void	PrepareRemove(INT nClientIdx);
	void	PrepareExchange(INT nIndex);
	void	PrepareLoginFailed(INT nIndex);
	void	RemoveQuiting(INT nIndex);
	void	RemoveLoginTimeOut(INT nIndex);
	void	RemoveExchanging(INT nIndex);
	void	AutoSave();
	void	ProcessClientMessage(INT nClient, const char* pChar, INT nSize);
	INT		GetPlayerNumber() { return m_UseIdx.GetCount(); }
	BOOL	GetPlayerName(INT nClient, char* szName);
	BOOL	GetPlayerAccount(INT nClient, char* szName);
	INT		Broadcasting(char* pMessage, INT nLen);
	void	SetSaveBufferPtr(void* pData);
	INT		AttachPlayer(const ULONG lnID, GUID* pGuid);
	INT		GetPlayerIndexByGuid(GUID* pGuid);
	INT		ExecuteScript(INT nPlayerIndex, char* szScriptName, char* szFuncName, INT nParam);
	INT		ExecuteScript(INT nPlayerIndex, DWORD dwScriptId, char* szFuncName, INT nParam);
#endif

private:
	INT		FindFree();
};

extern KPlayerSet PlayerSet;
#endif //KPlayerSetH
