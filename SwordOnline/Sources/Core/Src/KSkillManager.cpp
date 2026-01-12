
#include "KCore.h"
#include "KSkillManager.h"
#include "MyAssert.H"
#include "KThiefSkill.h"
#include "KPlayer.h"

ULONG g_ulSkillCount = 0;
KSkillManager g_SkillManager;

KSkillManager::KSkillManager()
{
	ZeroMemory(m_SkillInfo, sizeof(m_SkillInfo));

	// Khởi tạo mảng với giá trị mặc định (ví dụ: nullptr)
	for (INT i = 0; i < MAX_SKILL; ++i) {
		for (INT j = 0; j < MAX_SKILLLEVEL; ++j) {
			m_pOrdinSkill[i][j] = nullptr;
		}
	}
}

// Add By Freeway Chen in 2003.6.6
KSkillManager::~KSkillManager()
{
	ULONG i = 0;
	ULONG j = 0;

	for (i = 0; i < MAX_SKILL; i++)
	{
		for (j = 0; j < MAX_SKILLLEVEL; j++)
		{
			if (m_pOrdinSkill[i][j])
			{
				delete m_pOrdinSkill[i][j];
				m_pOrdinSkill[i][j] = NULL;
			}
		}
	}
}

BOOL KSkillManager::Init()
{
	INT i = 0;
	INT j = 0;
	for (i = 0; i < MAX_SKILL; i++)
	{
		for (j = 0; j < MAX_SKILLLEVEL; j++)
		{
			m_pOrdinSkill[i][j] = NULL;
		}
	}

	memset(m_SkillInfo, 0, sizeof(m_SkillInfo));

	//	Load OrdinSkill Info 
	INT nSkillNum = g_OrdinSkillsSetting.GetHeight() - 1;
	if (nSkillNum <= 0) return FALSE;

	for (i = 0; i < nSkillNum; i++)
	{
		INT nSkillId = -1;
		INT nSkillStyle = -1;
		INT nSkillMaxLevel = 0;
		g_OrdinSkillsSetting.GetInteger(i + 2, "SkillId", -1, &nSkillId);
		g_OrdinSkillsSetting.GetInteger(i + 2, "SkillStyle", -1, &nSkillStyle);
		g_OrdinSkillsSetting.GetInteger(i + 2, "MaxLevel", 20, &nSkillMaxLevel);
		_ASSERT(nSkillMaxLevel >= 0);
		if (nSkillId > 0 && nSkillStyle >= 0)
		{
			m_SkillInfo[nSkillId - 1].m_nSkillStyle = nSkillStyle;
			m_SkillInfo[nSkillId - 1].m_nTabFileRowId = i + 2;
			m_SkillInfo[nSkillId - 1].m_ulMaxSkillLevel = nSkillMaxLevel;
		}
		else
		{
			if (nSkillId > 0 && nSkillId <= MAX_SKILL) {
				m_SkillInfo[nSkillId - 1].m_nSkillStyle = -1;
				m_SkillInfo[nSkillId - 1].m_nTabFileRowId = 0;
				m_SkillInfo[nSkillId - 1].m_ulMaxSkillLevel = 0;
			}
		}
	}
	KTabFile ThiefSkillTab;
	if (ThiefSkillTab.Load(THIEFSKILL_SETTINGFILE))
	{
		INT nSkillId = 0;
		ThiefSkillTab.GetInteger(2, "SkillId", 400, &nSkillId);
		_ASSERT(nSkillId > 0);
		m_SkillInfo[nSkillId - 1].m_nSkillStyle = SKILL_SS_Thief;
		m_SkillInfo[nSkillId - 1].m_nTabFileRowId = 2;
		m_SkillInfo[nSkillId - 1].m_ulMaxSkillLevel = 1;
	}
	return TRUE;
}

ISkill* KSkillManager::InstanceSkill(ULONG ulSkillID, ULONG ulSkillLevel)
{
	ISkill* pRetSkill = NULL;

	INT nStyle = GetSkillStyle(ulSkillID);

	switch (nStyle) // eSkillStyle
	{
	case SKILL_SS_Missles:			        //	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
	case SKILL_SS_Melee:
	case SKILL_SS_InitiativeNpcState:	    //	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
	case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
	{
		KSkill* pNewOrdinSkill = NULL;
		ULONG ulFirstLoadLevel = 0;

		if (m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1])
		{
			pRetSkill = m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1];
			goto Exit1;
		}

		pNewOrdinSkill = new KSkill;
		ulFirstLoadLevel = m_SkillInfo[ulSkillID - 1].m_ulFirstLoadLevel;

		if (!ulFirstLoadLevel)
		{
			pNewOrdinSkill->GetInfoFromTabFile(m_SkillInfo[ulSkillID - 1].m_nTabFileRowId);
			pNewOrdinSkill->LoadSkillLevelData(ulSkillLevel, m_SkillInfo[ulSkillID - 1].m_nTabFileRowId);
			m_SkillInfo[ulSkillID - 1].m_ulFirstLoadLevel = ulSkillLevel;
		}
		else
		{
			_ASSERT(m_pOrdinSkill[ulSkillID - 1][ulFirstLoadLevel - 1]);
			if (m_pOrdinSkill[ulSkillID - 1] && m_pOrdinSkill[ulSkillID - 1][ulFirstLoadLevel - 1]) {
				*pNewOrdinSkill = *(KSkill*)m_pOrdinSkill[ulSkillID - 1][ulFirstLoadLevel - 1];
				pNewOrdinSkill->LoadSkillLevelData(ulSkillLevel, m_SkillInfo[ulSkillID - 1].m_nTabFileRowId);
			}
		}

		pNewOrdinSkill->SetSkillId(ulSkillID);
		pNewOrdinSkill->SetSkillLevel(ulSkillLevel);

		m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1] = pNewOrdinSkill;
		pRetSkill = pNewOrdinSkill;
		pNewOrdinSkill = NULL;

	}
	break;
	case SKILL_SS_Thief:
	{
		if (!m_pOrdinSkill[ulSkillID - 1][0])
		{
			m_pOrdinSkill[ulSkillID - 1][0] = (ISkill*)new KThiefSkill;
		}

		pRetSkill = m_pOrdinSkill[ulSkillID - 1][0];

		((KThiefSkill*)pRetSkill)->LoadSetting(THIEFSKILL_SETTINGFILE);

	}break;

	default:
		goto Exit0;
	}

Exit1:
Exit0:
	return pRetSkill;
}


