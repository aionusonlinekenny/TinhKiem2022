//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcGold.h
// Date:	2003.07.23
// Code:	边城浪子
// Desc:	KNpcGold Class
//---------------------------------------------------------------------------

#ifndef _KNPCGOLD_H
#define	_KNPCGOLD_H

#define	defMAX_NPC_GOLD_TYEP	30  //蓝怪NPC的种类限制

class KNpcGold
{
	friend class KNpc;
private:
	INT		m_nNpcIdx;
	INT		m_nIsGold;			// 是否是黄金怪物类
	INT		m_nIsGolding;		// 当前是否是黄金怪物
	INT		m_nGoldType;		// 如果当前是黄金怪物，变化的类型

// 备份的 npc 数据
	UINT	m_dwSkill5ID;				// 技能名转成的id  SkillString2Id()
	INT		m_nSkill5Level;				// 技能等级
	INT		m_nFireResist;				// 火抗性
	INT		m_nFireResistMax;			// 火抗性最大值
	INT		m_nColdResist;				// 冰抗性
	INT		m_nColdResistMax;			// 冰抗性最大值
	INT		m_nLightingResist;			// 电抗性
	INT		m_nLightingResistMax;		// 电抗性最大值
	INT		m_nPoisonResist;			// 毒抗性
	INT		m_nPoisonResistMax;			// 毒抗性最大值
	INT		m_nPhycicsResist;			// 物理抗性
	INT		m_nPhycicsResistMax;		// 物理抗性最大值
//////////////////////////////////////////////////////
	/*INT		m_nAiMode;
	INT		m_nAiParam1;
	INT		m_nAiParam2;
	INT		m_nAiParam3;
	INT		m_nAiParam4;
	INT		m_nAiParam5;
	INT		m_nAiParam6;
	INT		m_nAiParam7;
	INT		m_nAiParam8;
	INT		m_nAiParam9;
	INT		m_nAiParam10;
	INT		m_nAiMaxTime;
	INT		m_nPhysicalDamageBase;
	INT		m_nPhysicalMagicBase;
	INT		m_nPoisonDamageBase;
	INT		m_nPoisonMagicBase;
	INT		m_nColdDamageBase;
	INT		m_nColdMagicBase;
	INT		m_nFireDamageBase;
	INT		m_nFireMagicBase;
	INT		m_nLightingDamageBase;
	INT		m_nLightingMagicBase;
	//char	m_nDeathScript[128]; //死亡脚本
	//char	m_nDropRate[128]; //爆率
	//INT   m_nRate;
	*/
	INT     GetGoldNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, INT nLevel, char * szParam,INT nSeries);
public:
	void	Init(INT nIdx);
	void	SetGoldType(BOOL bFlag);	// 设定是否为黄金怪物
	void	SetGoldTypeAndBackData();	// 设定类型为黄金怪物，同时备份相应数据
	void	SetBossTypeAndBackData(INT nBoss);
	void	RecoverBackData();			// 恢复旧数据，变成普通npc
	void	RandChangeGold(BOOL bGold = 0,INT BossType=0,INT nSubWorld=0);			// 按一定概率变成黄金怪物
	INT		GetGoldType();				// 获得当前黄金类型，0 非黄金怪
	void	SetGoldCurrentType(INT nType,INT nSubWorld=0);
#ifndef _SERVER
	void	ClientClearState();
#endif

#ifdef _SERVER
	BOOL	CheckThisNpc(INT nSetings,char *nKey);
#endif

};

struct	KNpcGoldTemplateInfo
{
// 按整数倍变化，可反向恢复
	INT		m_nExp;						// 经验
	INT		m_nLife;					// 生命
	INT		m_nLifeReplenish;			// 回血
	INT		m_nAttackRating;			// 命中
	INT		m_nDefense;					// 防御
	INT		m_nMinDamage;
	INT		m_nMaxDamage;

// 加加减减的东西，可反向恢复
	INT		m_nTreasure;				// 装备
	INT		m_nWalkSpeed;
	INT		m_nRunSpeed;
	INT		m_nAttackSpeed;
	INT		m_nCastSpeed;

// 需要备份的东西，不可反向恢复
	INT	    m_dwSkill5ID;				// 技能名转成的id  SkillString2Id()
	//char	m_szSkill5Level[32];		// 技能等级
	INT 	m_szSkill5Level;		    // 技能等级
	INT		m_nFireResist;				// 火抗性
	INT		m_nFireResistMax;			// 火抗性最大值
	INT		m_nColdResist;				// 冰抗性
	INT		m_nColdResistMax;			// 冰抗性最大值
	INT		m_nLightingResist;			// 电抗性
	INT		m_nLightingResistMax;		// 电抗性最大值
	INT		m_nPoisonResist;			// 毒抗性
	INT		m_nPoisonResistMax;			// 毒抗性最大值
	INT		m_nPhycicsResist;			// 物理抗性
	INT		m_nPhycicsResistMax;		// 物理抗性最大值
//////////////////////////////////////////////////////
	/*INT		m_nAiMode;
	INT		m_nAiParam1;
	INT		m_nAiParam2;
	INT		m_nAiParam3;
	INT		m_nAiParam4;
	INT		m_nAiParam5;
	INT		m_nAiParam6;
	INT		m_nAiParam7;
	INT		m_nAiParam8;
	INT		m_nAiParam9;
	INT		m_nAiParam10;
	INT		m_nAiMaxTime;
	INT		m_nPhysicalDamageBase;
	INT		m_nPhysicalMagicBase;
	INT		m_nPoisonDamageBase;
	INT		m_nPoisonMagicBase;
	INT		m_nColdDamageBase;
	INT		m_nColdMagicBase;
	INT		m_nFireDamageBase;
	INT		m_nFireMagicBase;
	INT		m_nLightingDamageBase;
	INT		m_nLightingMagicBase;
	//char	m_nDeathScript[128]; //死亡脚本
	//char	m_nDropRate[128]; //爆率
	INT     m_nRate;
	*/
};

class KNpcGoldTemplate
{
	friend class KNpcSet;
public:
	KNpcGoldTemplateInfo m_sInfo[defMAX_NPC_GOLD_TYEP];
	INT	 m_nEffectTypeNum; //蓝怪的最大种类
public:
	KNpcGoldTemplate();
	BOOL	Init();
};
//extern 	KNpcGoldTemplateInfo *m_sInfo;
#endif
