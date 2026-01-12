#ifndef KSKILLMANAGER_H
#define KSKILLMANAGER_H

#include "KCore.h"
#ifndef _STANDALONE
#include "crtdbg.h"
#endif
#include "KSkills.h"
typedef eSKillStyle eSkillStyle;

class KSkillManager
{
	friend class KSkill;

	typedef struct
	{
		INT m_nTabFileRowId;
		INT m_nSkillStyle;
		ULONG	m_ulFirstLoadLevel;
		ULONG	m_ulMaxSkillLevel;
	}	TSkillInfo;

private:

	TSkillInfo m_SkillInfo[MAX_SKILL];
	//TOrdinSkillCommonData * m_pOrdinSkillCommonData[MAX_SKILL];
	ISkill* m_pOrdinSkill[MAX_SKILL][MAX_SKILLLEVEL];// 

private:
	ISkill* InstanceSkill(ULONG ulSkillID, ULONG ulSkillLevel);
	INT		GetSkillStyle(ULONG ulSkill);

public:
	KSkillManager();
	// Add By Freeway Chen in 2003.6.6
	~KSkillManager();

	BOOL Init();
	ISkill* GetSkill(INT nSkill, INT nSkillLevel);
	ISkill* GetSkill(INT nSkillID, INT nSkillLevel, eSkillStyle eStyle);
	ULONG	GetSkillMaxLevel(INT nSkill)
	{
		if (nSkill <= 0) return 0;

		if (nSkill <= MAX_SKILL)
			return m_SkillInfo[nSkill - 1].m_ulMaxSkillLevel;
		else
			return 0;
	};

};

inline INT KSkillManager::GetSkillStyle(ULONG ulSkillID)
{
	if (m_SkillInfo[ulSkillID - 1].m_nTabFileRowId > 0)
		return m_SkillInfo[ulSkillID - 1].m_nSkillStyle;
	else
		return -1;
}

inline ISkill* KSkillManager::GetSkill(INT nSkillID, INT nSkillLevel, eSkillStyle eStyle)
{
	ISkill* pSkill = GetSkill(nSkillID, nSkillLevel);
	if (pSkill)
	{
		if (pSkill->GetSkillStyle() == eStyle)
			return pSkill;
	}

	return NULL;
}


inline ISkill* KSkillManager::GetSkill(INT nSkillID, INT nSkillLevel)
{
	if (nSkillID > MAX_SKILL || nSkillID <= 0 || nSkillLevel <= 0 || nSkillLevel > MAX_SKILLLEVEL)
		return NULL;

	//g_DebugLog("SkillID: %d - SkillLevel: %d", nSkillID, nSkillLevel);
	_ASSERT((nSkillID <= MAX_SKILL) && (nSkillID > 0));
	_ASSERT((nSkillLevel <= MAX_SKILLLEVEL) && (nSkillLevel > 0));

	ULONG ulSkillID = nSkillID;
	ULONG ulSkillLevel = nSkillLevel;

	if (m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1])
	{
		return m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1];
	}

	return InstanceSkill(ulSkillID, ulSkillLevel);
}

extern KSkillManager g_SkillManager;
#endif
