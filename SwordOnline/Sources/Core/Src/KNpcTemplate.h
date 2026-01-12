#ifndef	_KNPCTEMPLATE_H
#define	_KNPCTEMPLATE_H

#include "KCore.h"
#include "KNpc.h"

class KNpcTemplate
{
public:
private:
public:
	char	Name[32];
	DWORD	m_Kind;
	INT		m_Camp;
	INT		m_Series;
	INT		m_HeadImage;
	INT		m_bClientOnly;
	INT		m_CorpseSettingIdx;
	INT		m_DeathFrame;
	INT		m_WalkFrame;
	INT		m_StandFrame;
	INT		m_StandFrame1;
	INT		m_RunFrame;
	INT		m_HurtFrame;
	INT		m_WalkSpeed;
	INT		m_AttackFrame;
	INT		m_CastFrame;
	INT		m_RunSpeed;
	INT			m_nLifeMax;
	BOOL		m_bHaveLoadedFromTemplate;

#ifdef _SERVER	
	DWORD		m_dwLevelSettingScript;
	INT			m_Treasure;
	INT		m_AiMode;
	INT		m_AiParam[MAX_AI_PARAM - 1];
	INT		m_FireResistMax;
	INT		m_ColdResistMax;
	INT		m_LightResistMax;
	INT		m_PoisonResistMax;
	INT		m_PhysicsResistMax;
	INT		m_ActiveRadius;
	INT		m_VisionRadius;
	BYTE		m_AIMAXTime;
	INT		m_HitRecover;
	INT		m_ReviveFrame;
	UINT		m_IsRevive;

	INT		m_Experience;
	INT		m_LifeReplenish;
	INT		m_AttackRating;
	INT		m_Defend;
	KMagicAttrib		m_PhysicsDamage;
	INT		m_RedLum;
	INT		m_GreenLum;
	INT		m_BlueLum;
	INT		m_FireResist;
	INT		m_ColdResist;
	INT		m_LightResist;
	INT		m_PoisonResist;
	INT		m_PhysicsResist;
	KSkillList	m_SkillList;
	KItemDropRate* m_pItemDropRate; // General explosion rate
#endif

#ifndef _SERVER	
	INT		m_ArmorType;
	INT		m_HelmType;
	INT		m_WeaponType;
	INT		m_HorseType;
	INT		m_bRideHorse;
	char		ActionScript[128];
	char		m_szLevelSettingScript[128];
#endif

	INT		m_NpcSettingIdx;
	INT		m_nStature;
	INT			m_nLevel;

public:
	BOOL		Init(INT nNpcTemplateId);
	BOOL		InitNpcLevelData(INT nNpcTemplateId, KLuaScript* pLevelScript, INT nLevel);
	static INT	GetNpcLevelDataFromScript(KLuaScript* pScript, char* szDataName, INT nLevel, char* szParam, INT nSeries);
	static INT	GetNpcKeyDataFromScript(KLuaScript* pScript, char* szDataName, INT nLevel, double nParam1, double nParam2, double nParam3, INT nSeries);
	static INT	SkillString2Id(char* szSkillString);
	INT			InitDropRate(INT nNpcIdex, char* nDropRateFiled);
	INT			gmUpdataDropRate(INT nNpcIdex, char* nDropRateFiled);
	KNpcTemplate() {
		m_bHaveLoadedFromTemplate = FALSE;
		Name[0] = 0;
		m_Kind = -1;
		m_Camp = -1;
		m_Series = -1;
		m_HeadImage = -1;
		m_bClientOnly = -1;
		m_CorpseSettingIdx = -1;
		m_DeathFrame = 0;
		m_WalkFrame = 0;
		m_StandFrame = 0;
		m_StandFrame1 = 0;
		m_RunFrame = 0;
		m_HurtFrame = 0;
		m_WalkSpeed = 0;
		m_AttackFrame = 0;
		m_CastFrame = 0;
		m_RunSpeed = 0;

#ifdef _SERVER	
		m_AiMode = 0;
		ZeroMemory(m_AiParam, sizeof(m_AiParam));
		m_FireResistMax = 0;
		m_ColdResistMax = 0;
		m_LightResistMax = 0;
		m_PoisonResistMax = 0;
		m_PhysicsResistMax = 0;
		m_ActiveRadius = 0;
		m_VisionRadius = 0;
		m_AIMAXTime = 0;
		m_HitRecover = 0;
		m_ReviveFrame = 0;
		m_Experience = 0;
		m_LifeReplenish = 0;
		m_AttackRating = 0;
		m_Defend = 0;
		m_PhysicsDamage = {};
		m_RedLum = 0;
		m_GreenLum = 0;
		m_BlueLum = 0;
		m_FireResist = 0;
		m_ColdResist = 0;
		m_LightResist = 0;
		m_PoisonResist = 0;
		m_PhysicsResist = 0;
		m_SkillList = {};
#endif

#ifndef _SERVER	
		m_ArmorType = 0;
		m_HelmType = 0;
		m_WeaponType = 0;
		m_HorseType = 0;
		m_bRideHorse = 0;
#endif
		m_NpcSettingIdx = 0;
		m_nStature = 0;
	};
};

extern KNpcTemplate* g_pNpcTemplate[MAX_NPCSTYLE];

#endif

