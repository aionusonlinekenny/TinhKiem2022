#ifndef	KSkillListH
#define	KSkillListH
//#include "Windows.h"
#include "GameDataDef.h"


typedef struct tagNpcSkill
{
	INT		SkillId;
	INT		SkillLevel;
	INT		AddLevel;
	INT		CurrentSkillLevel;
	ULONG		SkillExp;
	ULONG		NextSkillExp;
	BOOL		TempSkill;
	INT		MaxTimes;
	INT		RemainTimes;
	DWORD		NextCastTime;		//下次可发送时的最小时间
	DWORD		NextHorseCastTime; // The minimum time when the next ride can be sent
	INT		WaitCastTime;
#ifndef _SERVER
	DWORD	TotalTime;//tong thoi gian
#endif
} NPCSKILL, * LPNPCSKILL;
struct TDBPlayerData;
#ifndef TOOLVERSION
class KSkillList
#else 
class CORE_API KSkillList
#endif
{
public:
	NPCSKILL		m_Skills[MAX_NPCSKILL];//下标从1开始，0为无效index
	INT			m_nAllSkillV;
	INT			m_nNpcIndex;
public:
	KSkillList();
	~KSkillList();
	INT			Add(INT nSkillID, INT nLevel = 1, INT nExp = 0, BOOL bTempSkill = FALSE, INT nMaxTimes = 0, INT RemainTimes = 0);
	void		Remove(INT nSkillID)
	{
		if (nSkillID <= 0 || nSkillID >= MAX_SKILL)
			return;
		INT i = FindSame(nSkillID);
		if (i)
		{
			m_Skills[i].SkillId = 0;
			m_Skills[i].SkillLevel = 0;
			m_Skills[i].AddLevel = m_nAllSkillV;
			m_Skills[i].CurrentSkillLevel = 0;
			m_Skills[i].SkillExp = 0;
			m_Skills[i].NextSkillExp = 0;
			m_Skills[i].TempSkill = FALSE;
			m_Skills[i].MaxTimes = 0;
			m_Skills[i].RemainTimes = 0;
			m_Skills[i].NextCastTime = 0;
			m_Skills[i].WaitCastTime = 0;
		}

	};//Question :需要加！
	void		RemoveIdx(INT nIdx)
	{
		if (nIdx <= 0 || nIdx >= MAX_NPCSKILL)
			return;

		m_Skills[nIdx].SkillId = 0;
		m_Skills[nIdx].SkillLevel = 0;
		m_Skills[nIdx].AddLevel = m_nAllSkillV;
		m_Skills[nIdx].CurrentSkillLevel = 0;
		m_Skills[nIdx].SkillExp = 0;
		m_Skills[nIdx].NextSkillExp = 0;
		m_Skills[nIdx].TempSkill = FALSE;
		m_Skills[nIdx].MaxTimes = 0;
		m_Skills[nIdx].RemainTimes = 0;
		m_Skills[nIdx].NextCastTime = 0;
		m_Skills[nIdx].WaitCastTime = 0;

	};//Question :需要加！
	INT			GetSkillId(ULONG ulSkillIdx) const
	{
		if (ulSkillIdx >= MAX_NPCSKILL || ulSkillIdx <= 0)
			return 0;
		else
			return m_Skills[ulSkillIdx].SkillId;
	};


	INT	SetSkillLevelDirectlyUsingIndex(ULONG ulSkillIdx, ULONG ulLevel)/*请慎用该函数，因为目前未加任何被动技能降级对数值的影响*/
	{
		if (ulSkillIdx >= MAX_NPCSKILL || ulSkillIdx == 0 || ulLevel >= MAX_SKILLLEVEL)
			return 0;

		if (m_Skills[ulSkillIdx].SkillId)
		{
			m_Skills[ulSkillIdx].SkillLevel = ulLevel;
			m_Skills[ulSkillIdx].CurrentSkillLevel = m_Skills[ulSkillIdx].SkillLevel + m_Skills[ulSkillIdx].AddLevel;
			return 1;
		}
		return 0;
	}

	INT	SetSkillLevelDirectlyUsingId(ULONG ulSkillId, ULONG ulLevel)/*请慎用该函数，因为目前未加任何被动技能降级对数值的影响*/
	{
		if (ulSkillId >= MAX_SKILL || ulSkillId <= 0 || ulLevel >= MAX_SKILLLEVEL)
			return 0;
		ULONG  ulSkillIdx = FindSame(ulSkillId);
		if (ulSkillIdx)
		{
			if (m_Skills[ulSkillIdx].SkillId)
			{
				m_Skills[ulSkillIdx].SkillLevel = ulLevel;
				m_Skills[ulSkillIdx].CurrentSkillLevel = m_Skills[ulSkillIdx].SkillLevel + m_Skills[ulSkillIdx].AddLevel;
				return 1;
			}
		}
		return 0;
	}

	void		Clear() { memset(m_Skills, 0, sizeof(m_Skills)); };
#ifndef _SERVER
	void		SetSkillLevel(INT nId, INT nLevel);
	BOOL		SetLevel(INT nIndex, INT nLevel);		// 把某个编号技能设为某一级
	void		SetAddLevel(INT nId, INT nAdd);
	void		SetCurLevel(INT nId, INT nAdd);
	BOOL		SetExp(INT nIndex, INT nExp);		// 把某个编号技能设为某一级
	INT			GetSkillSortList(KUiSkillData*);		// 获得角色当前所有技能排序列表
	INT			GetSkillPosition(INT nSkillId);
	INT			GetLeftSkillSortList(KUiSkillData*);	// 获得角色当前左键技能排序列表
	INT			GetRightSkillSortList(KUiSkillData*);	// 获得角色当前右键技能排序列表
	INT			GetNextSkillState(INT nIndex = 0);
	INT			GetNextSkillFight(INT nIndex = 0);
	INT			GetNextSkillAura(INT nIndex = 0);
	INT			FindSkillLifeReplenish();
#endif
	BOOL			IsBaseSkill(INT nSkillId);
	void			AllSkillV(INT nSkillId, INT nLevel);
	void			SeriesSkillV(INT nSeries, INT nLevel);
	BOOL			IncreaseLevel(INT nIdx, INT nLvl);
	BOOL			IncreaseExp(INT nIdx, INT nExp);
	BOOL			SetNextExp(INT nIdx, INT nExp);
	BOOL			SetTempSkill(INT nIdx, BOOL bTempSkill);
	INT			GetLevel(INT nSkillID);
	ULONG			GetExp(INT nSkillID);
	ULONG			GetNextExp(INT nSkillID);
	BOOL			IsTempSkill(INT nSkillID);
	INT			GetSkillIdxLevel(INT nIdx)
	{
		if (nIdx <= 0) return 0;
		return m_Skills[nIdx].SkillLevel;
	};
	INT            GetSkillIdx(INT nIdx)
	{
		if (nIdx <= 0) return 0;
		for (INT i = 1; i < MAX_NPCSKILL; i++)
		{
			if (m_Skills[i].SkillId == nIdx)
				return i;
		}
		return 0;
	};
	INT			GetCurrentLevel(INT nSkillID);
	INT			GetCurrentLevelIndex(INT nListIndex)
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].CurrentSkillLevel;
		else
			return 0;
	};
	INT			GetCount();
	INT			FindSame(INT nSkillID);
	INT			FindFree();
	BOOL			CanCast(INT nSkillID, DWORD dwTime);
	void			SetNextCastTime(INT nSkillID, DWORD dwCurrentTime, DWORD dwNextCastTime);
	void			SetNextCastTimeByIndex(INT nSkListIndex, DWORD dwTime);
	void			SetHorseNextCastTime(INT nSkillID, DWORD dwTime);
	void			SetHorseNextCastTimeByIndex(INT nSkListIndex, DWORD dwTime);
	INT 			GetHorseNextCastTime(INT nSkillID);
	INT				GetHorseNextCastTimeByIndex(INT nSkListIndex);
	INT				GetNextCastTime(INT nSkillID);
	INT				GetNextCastTimeByIndex(INT nSkListIndex);

	INT				GetSkillIdByListidx(INT nListIndex)
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].SkillId;
		else
			return 0;
	};

	INT			GetSkillId(INT nListIndex)
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].SkillId;
		else
			return -1;
	};
	INT				GetAddSkillDamage(INT nSkillID);
	UINT			GetAddSkillDamageBonus(INT nSkillID, INT CurrentSkillEnhancePercent);

	INT GetAddLevel(INT SkillId)
	{
		for (INT i = 1; i < MAX_NPCSKILL; i++)
		{
			if (m_Skills[i].SkillId == SkillId)
			{
				return m_Skills[i].AddLevel;
			}
		}
		return 0;
	}
	INT	GetAddLevelIndex(INT nListIndex)
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].AddLevel;
		else
			return 0;
	};
#ifdef _SERVER
	INT		UpdateDBSkillList(BYTE*);
	void		SetNpcSkill(INT nSkillNo, INT nSkillId, INT nSkillLevel);
	void		RemoveNpcSkill(INT nSkillNo);
	INT		RollBackSkills(bool bRbAll);
	void		SetSkillList(KSkillList mSkillList) {
		for (INT i = 1; i < MAX_NPCSKILL; i++)
		{
			if (mSkillList.m_Skills[i].SkillId  >  0)
			{
				m_Skills[i].SkillId = mSkillList.m_Skills[i].SkillId;
				m_Skills[i].SkillLevel = mSkillList.m_Skills[i].SkillLevel;

				g_DebugLog("Copy Skill: [%d] - [%d]", mSkillList.m_Skills[i].SkillId, mSkillList.m_Skills[i].SkillLevel);
			}
		}
	};
#endif

};
#endif
