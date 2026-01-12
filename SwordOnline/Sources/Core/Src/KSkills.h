// KSkills.h: interface for the KSkills class.
//
//////////////////////////////////////////////////////////////////////
#ifndef KSkillsH
#define KSkillsH

#include "KMissle.h"
#include "KWorldMsg.h"
#include "KNpc.h"
#include "KNode.h"
#include "KMissleMagicAttribsData.h"
#include "Skill.h"
#define MAX_MISSLE_DAMAGEATTRIB 50

typedef  KSkill KFightSkill;
class KMissle;
class KSkill;
//Skill[0]无用，每个skill的[0]无用!!注意！
enum
{
	Missle_StartEvent = 1,
	Missle_FlyEvent,
	Missle_CollideEvent,
	Missle_VanishEvent,
};

#ifdef TOOLVERSION
class CORE_API KSkill :public ISkill
#else
class KSkill :public ISkill
#endif
{
private:

public:
	ULONG				m_ulLevel;

#ifdef _SERVER
	DWORD				m_dwSkillLevelUpScriptID;	//玩家点升级技能等级时触发的脚本ID
	DWORD				m_dwSkillLevelDataScriptId; //该技能升级的变动数据对应脚本ID
#endif

#ifndef _SERVER
	char				m_szSkillIcon[80];
	char				m_szPreCastEffectFile[100];
	char				m_szManPreCastSoundFile[100];
	char				m_szFMPreCastSoundFile[100];
	eSkillLRInfo		m_eLRSkillInfo;			//0 表示左右键皆可，1表示只可以作右键技能，2表示左右键都不可作
#endif
	uint16_t			m_usReqLevel;//		技能需要的最小等级
private:
	INT					m_nCharClass;			//	属哪个系的
	eSKillStyle			m_eSkillStyle;			//  当前的技能类型
	INT					m_nSkillTime;			//	该技能的作用时间
	BOOL				m_bClientSend;			//  该技能是否对服务器来说有效
	BOOL				m_bHaveLoad;			//	标识该技能是否已经加载，如果没有则加载先
	INT					m_nInteruptTypeWhenMove;//子弹的激活是否受发送者的移动而中止
	BOOL				m_bHeelAtParent;	//	当子弹实际激活时，位置根据父当前位置而确定,而不是由产生那刻parent位置决定

	//技能限制项
	CLIENTACTION		m_nCharActionId;		//	发这个技能时人物做什么动作
	INT					m_nWaitTime;			//	该技能正常情况下真正产生的时间
	BOOL				m_bIsAura;				//	是否为光环技能
	BOOL				m_bIsPassivity;			//	是否为被动技能
	BOOL				m_bIsMelee;				//	是否为近身攻击，近身攻击时
	DWORD 				nId;
	BOOL				m_bUseAttackRate;		//	是否考虑命中率
	BOOL				m_bTargetOnly;			//	只有选中目标的时候才能发出
	BOOL				m_bTargetEnemy;			//	技能的目标是否为敌人
	BOOL				m_bTargetAlly;			//	技能的目标是否为同伴
	BOOL				m_bTargetObj;			//	技能的目标是否为物件
	BOOL				m_bTargetNoNpc;
	BOOL				m_bTargetSelf;			//	技能的目标是否可以为自已
	INT					m_eRelation;
	char				m_szName[64];			//	技能名称
	INT					m_nAttrib;
	DWORD				m_nId;					//	技能Id
	BOOL				m_bBaseSkill;			//	是否为最基本技能
	BOOL				m_bByMissle;			//	当由父技能产生时，是否是根据玩家为基点还是以当前的子弹为基点
	BOOL				m_bIsPhysical;			//	是否为物理技能
	INT					m_nCost;				//	技能使用时所需要花费的内力、体力、精力、金钱的类型
	NPCATTRIB			m_nSkillCostType;		//	发该技能所需的内力、体力等的消耗
	INT					m_nMinTimePerCast;		//	发该技能的最小间阁时间
	INT					m_nMinTimePerCastOnHorse;
	INT					m_nChildSkillNum;		//	同时发射子技能的数量	
	eMisslesForm		m_eMisslesForm;			//	多个子弹的起始格式
	INT					m_nValue1;				//	附加整形数据1
	INT					m_nValue2;				//	附加整形数据2
	INT					m_nEventSkillLevel;
	KMagicAttrib		m_MissleAttribs[MAXSKILLLEVELSETTINGNUM];//生成子弹的相关数据。不同等级的技能，子弹的相关属性可以变化。
	INT					m_nMissleAttribsNum;
	KMagicAttrib		m_DamageAttribs[MAX_MISSLE_DAMAGEATTRIB];	//应用到非状态相关的数据
	INT					m_nDamageAttribsNum;
	KMagicAttrib		m_ImmediateAttribs[MAXSKILLLEVELSETTINGNUM];//非状态类数据,立即伤害，技能发的子弹打中对方就立即生效的伤害,不随技能等级变化而改变.
	INT					m_nImmediateAttribsNum;
	KMagicAttrib		m_StateAttribs[MAXSKILLLEVELSETTINGNUM];     //应用到状态相关的数据
	INT					m_nStateAttribsNum;		//个数，最大10
	eMissleFollowKind   m_eMissleFollowKind;
	INT					m_nFollowKindIndex;
	DWORD				m_dwFollowKindID;
	INT					m_nChildSkillId;		//	技能引发的子技能Id;	//当该技能为基本技能时，这项无用
	INT					m_nChildSkillLevel;
	BOOL				m_bFlyingEvent;			//	是否需要在飞行过程消息发生是，调用相关回调函数
	BOOL				m_bStartEvent;			//	是否需要在技能第一次Active时，调用相关回调函数
	BOOL				m_bCollideEvent;		//	是否需要在子技能魔法碰撞时，调用相关回调函数
	BOOL				m_bVanishedEvent;		//	是否需要在子技能消亡时，调用相关的回调函数
	INT					m_nFlySkillId;			//	整 个飞行的相关技能
	INT					m_nFlyEventTime;		//	每多少帧回调FlyEvent;
	INT					m_nStartSkillId;		//	技能刚刚才发出时所引发的事件时，所需要的相关技能id
	INT					m_nVanishedSkillId;		//	技能发出的子弹结束时引发的技能Id;
	INT					m_nCollideSkillId;		//	技能发出的子弹碰撞到物件引发的技能Id;
	BOOL				m_bMustBeHit;			//	是否为必中技能，即对其处理简单化以便降低服务器负荷
	eMisslesGenerateStyle m_eMisslesGenerateStyle;//同时生成的多个子弹，DoWait的时间顺序	
	INT					m_nMisslesGenerateData;	//相关数据
	INT					m_nMaxShadowNum;		//	最大保留的残影量
	BOOL				m_bNeedShadow;
	INT					m_nAttackRadius;		// 射程
	INT					m_nStateSpecialId;		//状态性魔法光环id号
	INT					m_nMaxTimes;			//某些如陷阱魔法，最大同时出现多少个陷阱
	INT					m_nShowEvent;

	INT					m_nEquiptLimited;		//发技能时对当前装备的限制

	INT					m_nHorseLimited;		//骑马限制 0表示没任何限制
	//		   1表示不能骑马
	//		   2表示必须骑马
	INT					m_nAppendSkillNum;
	INT					m_nAppendSkillId[MAX_APPENDSKILL];
	BOOL				m_bSkillReduceResist;
	BOOL				m_bSkillLifeReplenish;
	INT					m_nDoHurtP;
	INT					m_nSeries;
	INT					m_nIsExpSkill;
	INT					m_nShowAddition;
	KMagicAttrib        m_AddSkillDamage[MAX_ADDSKILLDAMAGE];
	INT					m_nAddSkillDamageNum;
	char				m_szMagicSkillDesc[512];
public:
	KSkill();
	virtual ~KSkill();
	//载入相对于g_OrdinSkillsSetting的列号
	//BOOL				
	BOOL				OnMissleEvent(uint16_t usEvent, KMissle* pMissle) const;
	BOOL				IsTargetOnly() const { return m_bTargetOnly; };
	BOOL				IsTargetEnemy() const { return m_bTargetEnemy; };
	BOOL				IsTargetAlly() const { return m_bTargetAlly; };
	BOOL				IsTargetObj() const { return m_bTargetObj; };
	BOOL				IsTargetNoNpc() const { return m_bTargetNoNpc; };
	BOOL				IsNeedShadow() const { return m_bNeedShadow; };
	BOOL				IsSkillReduceResist() const { return m_bSkillReduceResist; };
	BOOL				IsSkillLifeReplenish() const { return m_bSkillLifeReplenish; };
	INT					GetMaxTimes() const { return m_nMaxTimes; };
	static	INT			GetSkillIdFromName(char* szSkillName);
	uint16_t			GetSkillReqLevel() const { return m_usReqLevel; };
	BOOL				IsTargetSelf() const { return m_bTargetSelf; };
	INT					GetParam1()const { return m_nValue1; };
	INT					GetMissleForm() { return m_eMisslesForm; };

#ifdef _SERVER
	ULONG		GetSkillLevelUpScriptId()const { return m_dwSkillLevelUpScriptID; };
	KMissleMagicAttribsData* CreateMissleMagicAttribsData(INT nLauncher)const;
#endif

#ifndef _SERVER
	eSkillLRInfo		GetSkillLRInfo() const { return m_eLRSkillInfo; };
	const char* GetPreCastEffectFile() const { return m_szPreCastEffectFile; };
	const char* GetPreCastSoundFile(BOOL bIsFeMale) const { return bIsFeMale ? m_szFMPreCastSoundFile : m_szManPreCastSoundFile; };
#endif

	const char* GetSkillName() { return m_szName; };
	INT					GetSkillStyle() { return m_eSkillStyle; };
	BOOL				GetInfoFromTabFile(INT nCol);
	BOOL				GetInfoFromTabFile(KITabFile* pSkillsSettingFile, INT nRow);
	BOOL				Load(INT nCol);
	BOOL				Cast(INT nLauncher, INT nParam1, INT nParam2, INT nWaitTime = 0, eSkillLauncherType eLauncherType = SKILL_SLT_Npc) const;	//发出时调用
	INT					GetChildSkillNum(INT nLevel)const;
	INT					GetAttackRadius() const { return m_nAttackRadius; };
	void				LoadSkillLevelData(ULONG  ulLevel, INT nParam); //从技能升级文件中获得nLevel等级的属性变动，并设置
	friend	class		KMissle;
	INT					GetSkillCost(void*)const { return m_nCost; };
	NPCATTRIB			GetSkillCostType()const { return m_nSkillCostType; };
	BOOL				GetItemLimit(INT)const;
	CLIENTACTION		GetActionType()const { return m_nCharActionId; };
	INT					GetDelayPerCast(BOOL bRideHorse)const {
		if (bRideHorse)
			return m_nMinTimePerCastOnHorse;
		return m_nMinTimePerCast;
	};
	UINT				GetMissleGenerateTime(INT nNo) const;
	INT					GetChildSkillNum() const { return m_nChildSkillNum; };
	INT					GetChildSkillId() const { return m_nChildSkillId; };
	INT					GetChildSkillLevel() const { return m_nChildSkillLevel; };
	INT					GetAttribType() const { return m_nAttrib; };
	INT					GetSkillId() { return m_nId; };
	void				SetSkillId(INT nId) { m_nId = nId; };
	void				SetSkillLevel(ULONG ulLevel) { m_ulLevel = ulLevel; };
	INT					GetSkillLevel() { return m_ulLevel; };

	BOOL				IsAura() const { return m_bIsAura; };
	BOOL				IsPhysical() const { return m_bIsPhysical; };
	BOOL				IsBase() { return m_nAttrib <= 1; };
	INT					IsExp() { return m_nIsExpSkill; };
	INT					GetMeleeType() const { return m_eMisslesForm; };
	INT					GetEquipLimit() const { return m_nEquiptLimited; };
	INT					GetHorseLimit() const { return m_nHorseLimited; };
	INT					GetStateSpecailId() const { return m_nStateSpecialId; };
	INT					CanCastSkill(INT nLauncher, INT& nParam1, INT& nParam2)  const;
	void				PlayPreCastSound(BOOL bIsFeMale, INT nX, INT nY) const;
	KMagicAttrib* GetDamageAttribs() { return m_DamageAttribs; };
	KMagicAttrib* GetStateAttribs() { return m_StateAttribs; };

	BOOL				IsUseAR() const { return m_bUseAttackRate; };
	BOOL				CastStateSkill(INT nLauncher, INT nParam1, INT nParam2, INT nWaitTime, BOOL bOverLook = FALSE) const;
	BOOL				CastSkill(int nLauncher, int nSkillID, int nParam1, int nParam2, int nLevel) const;
	INT					GetAppendSkillNum() { return m_nAppendSkillNum; };
	INT					GetAppendSkillId(INT nIdx) {
		if (nIdx >= 0 && nIdx < MAX_APPENDSKILL)
			return m_nAppendSkillId[nIdx];
		return 0;
	};
	INT					GetShowAddition() { return m_nShowAddition; };
	KMagicAttrib* GetAddSkillDamage() { return m_AddSkillDamage; };    //get	
	INT					GetSkillSeries() const { return m_nSeries; };
private:
	void				Vanish(KMissle*) const;
	void				FlyEvent(KMissle*)  const;
	void				Collidsion(KMissle*) const;
	BOOL				CastMissles(INT nLauncher, INT nParam1, INT nParam2, INT nWaitTime = 0, eSkillLauncherType eLauncherType = SKILL_SLT_Npc) const;
	BOOL				CastInitiativeSkill(INT nLauncher, INT nParam1, INT nParam2, INT nWaitTime) const;
	BOOL				CastPassivitySkill(INT nLauncher, INT nParam1, INT nParam2, INT nWaitTime) const;

	INT					CastWall(TOrdinSkillParam* pSkillParam, INT nDir, INT nRefPX, INT nRefPY) const;
	INT					CastLine(TOrdinSkillParam* pSkillParam, INT nDir, INT nRefPX, INT nRefPY) const;
	INT					CastSpread(TOrdinSkillParam* pSkillParam, INT nDir, INT nRefPX, INT nRefPY) const;
	INT					CastCircle(TOrdinSkillParam* pSkillParam, INT nDir, INT nRefPX, INT nRefPY) const;
	INT					CastZone(TOrdinSkillParam* pSkillParam, INT nDir, INT nRefPX, INT nRefPY) const;
	INT					CastExtractiveLineMissle(TOrdinSkillParam* pSkillParam, INT nDir, INT nSrcX, INT nSrcY, INT nXOffset, INT nYOffset, INT nDesX, INT nDesY) const;
	INT					Param2PCoordinate(INT nLauncher, INT nParam1, INT nParam2, INT* npPX, INT* npPY, eSkillLauncherType eLauncherType = SKILL_SLT_Npc) const;


	void				CreateMissle(INT nLauncher, INT ChildSkillId, INT nMissleIndex) const;

	BOOL				ParseString2MagicAttrib(ULONG ulLevel, char* szMagicAttribName, char* szValue);
	inline const char* MagicAttrib2String(INT MagicAttrib) const;

#ifndef _SERVER
public:
	void				DrawSkillIcon(INT x, INT y, INT Width, INT Height);
	KRUImage			m_RUIconImage;
	char				m_szSkillDesc[256];//技能的描述
	static void			GetDesc(ULONG ulSkillId, ULONG ulCurLevel, char* pszMsg, INT nOwnerIndex, bool bNextLevelDesc);
	void				GetDescAboutLevel(ULONG ulSkillId, char* pszMsg, BOOL bNextLevel = FALSE, BOOL bAddSkillDamage = FALSE, BOOL bEventSkill = FALSE);
	INT					GetHorseLimited() { return m_nHorseLimited; };
#endif
public:
};

#define MAX_MELEEWEAPON_PARTICULARTYPE_NUM 100
#define MAX_RANGEWEAPON_PARTICULARTYPE_NUM 100
extern INT		g_nMeleeWeaponSkill[MAX_MELEEWEAPON_PARTICULARTYPE_NUM];
extern INT		g_nRangeWeaponSkill[MAX_RANGEWEAPON_PARTICULARTYPE_NUM];
extern INT		g_nHandSkill;

#endif
