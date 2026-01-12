#include "KEngine.h"
#include "KCore.h"
#include "SkillDef.h"
#include "KSkillList.h"
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#include "crtdbg.h"
#else
#include "S3DBInterface.h"
#endif
#include "KSkills.h"
#include "KThiefSkill.h"
#include "KPlayer.h"
#include "KPlayerSet.h"

#ifndef _SERVER
#include "cOREsHELL.H"
#endif

KSkillList::KSkillList()
{
	ZeroMemory(m_Skills, sizeof(NPCSKILL) * MAX_NPCSKILL);
	m_nAllSkillV = 0;
	m_nNpcIndex = 0;
}

KSkillList::~KSkillList()
{
}


static INT g_CallCount = 0;

INT	KSkillList::FindSame(INT nSkillID)
{
	g_CallCount++;

	if (!nSkillID)
		return 0;

	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId == nSkillID)
		{
			return i;
		}
	}
	return 0;
}

INT KSkillList::FindFree()
{
	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		if (!m_Skills[i].SkillId)
		{
			return i;
		}
	}
	return 0;
}

#ifdef _SERVER
void KSkillList::SetNpcSkill(INT nSkillNo, INT nSkillId, INT nSkillLevel)
{
	if (nSkillNo <= 0 || nSkillLevel <= 0 || nSkillId <= 0)
		return;
	if (nSkillNo >= MAX_NPCSKILL)
		return;

	m_Skills[nSkillNo].SkillId = nSkillId;
	m_Skills[nSkillNo].SkillLevel = nSkillLevel;
	m_Skills[nSkillNo].AddLevel = m_nAllSkillV;
	m_Skills[nSkillNo].CurrentSkillLevel = nSkillLevel;
	m_Skills[nSkillNo].SkillExp = 0;
	m_Skills[nSkillNo].NextSkillExp = 0;
	m_Skills[nSkillNo].TempSkill = FALSE;
	m_Skills[nSkillNo].MaxTimes = 0;
	m_Skills[nSkillNo].RemainTimes = 0;
	m_Skills[nSkillNo].NextCastTime = 0;
	m_Skills[nSkillNo].WaitCastTime = 0;

	_ASSERT(nSkillId > 0 && nSkillLevel > 0);
	KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nSkillId, nSkillLevel);
	if (!pOrdinSkill)
		return;

	if (pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState || pOrdinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
	{
		pOrdinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex);
	}

	if (pOrdinSkill->IsAura())
	{
		Npc[m_nNpcIndex].SetAuraSkill(m_Skills[nSkillNo].SkillId);
	}
}

void KSkillList::RemoveNpcSkill(INT nSkillNo)
{
	if (nSkillNo <= 0)
		return;
	if (nSkillNo >= MAX_NPCSKILL)
		return;

	_ASSERT(m_Skills[nSkillNo].SkillId > 0);
	KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[nSkillNo].SkillId, 1);
	if (!pOrdinSkill)
		return;

	if (pOrdinSkill->IsAura() && Npc[m_nNpcIndex].m_ActiveAuraID == m_Skills[nSkillNo].SkillId)
	{
		Npc[m_nNpcIndex].SetAuraSkill(0);
	}

	m_Skills[nSkillNo].SkillId = 0;
	m_Skills[nSkillNo].SkillLevel = 0;
	m_Skills[nSkillNo].AddLevel = m_nAllSkillV;
	m_Skills[nSkillNo].CurrentSkillLevel = 0;
	m_Skills[nSkillNo].SkillExp = 0;
	m_Skills[nSkillNo].NextSkillExp = 0;
	m_Skills[nSkillNo].TempSkill = FALSE;
	m_Skills[nSkillNo].MaxTimes = 0;
	m_Skills[nSkillNo].RemainTimes = 0;
	m_Skills[nSkillNo].NextCastTime = 0;
	m_Skills[nSkillNo].WaitCastTime = 0;
}

#endif

INT KSkillList::GetCount()
{
	INT nCount = 0;

	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId)
		{
			nCount++;
		}
	}
	return nCount;
}

#ifndef _SERVER
void KSkillList::SetSkillLevel(INT nId/*¼¼ÄÜµÄIDºÅ*/, INT nLevel)
{
	INT i = FindSame(nId);

	if (!i)
	{
		Add(nId, nLevel);
		return;
	}
	else
		//m_Skills[i].SkillLevel = nLevel;
		SetLevel(i, nLevel);
}
#endif

#ifndef _SERVER
BOOL KSkillList::SetLevel(INT nIndex/*¼¼ÄÜÁÐ±íµÄË÷ÒýºÅ*/, INT nLevel)
{
	if (nIndex <= 0 || nIndex >= MAX_NPCSKILL)
		return FALSE;
	if (nLevel < 0 || nLevel >= MAX_SKILLLEVEL)
		return FALSE;

	INT nInc = nLevel - m_Skills[nIndex].SkillLevel;
	IncreaseLevel(nIndex, nInc);
	return TRUE;
}

BOOL KSkillList::SetExp(INT nIndex/*¼¼ÄÜÁÐ±íµÄË÷ÒýºÅ*/, INT nExp)
{
	if (nIndex <= 0 || nIndex >= MAX_NPCSKILL)
		return FALSE;

	m_Skills[nIndex].SkillExp = nExp;
	return TRUE;
}
#endif

BOOL KSkillList::IsBaseSkill(INT nSkillId)
{
	if (nSkillId < 0)
		return FALSE;

	if (nSkillId > 0)
	{
		KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nSkillId, 1);
		if (!pOrdinSkill)
			return FALSE;

		return pOrdinSkill->IsBase();
	}
	return FALSE;
}

void KSkillList::AllSkillV(INT nSkillId, INT nLevel)
{
	if (nSkillId < 0)
		return;

	if (nSkillId > 0)
	{
		INT i = FindSame(nSkillId);
		if (i)
		{
			if (m_Skills[i].TempSkill)
			{
				m_Skills[i].SkillLevel += nLevel;
				m_Skills[i].AddLevel = m_nAllSkillV;
				m_Skills[i].CurrentSkillLevel = m_Skills[i].SkillLevel + m_Skills[i].AddLevel;
				if (m_Skills[i].SkillLevel <= 0)
					RemoveIdx(i);
			}
			else
			{
				m_Skills[i].AddLevel += nLevel;
				m_Skills[i].CurrentSkillLevel += nLevel;
			}
		}
		else
		{
			i = FindFree();
			if (i)
			{
				m_Skills[i].SkillId = nSkillId;
				m_Skills[i].SkillLevel += nLevel;
				m_Skills[i].AddLevel = m_nAllSkillV;
				m_Skills[i].CurrentSkillLevel = m_Skills[i].SkillLevel + m_Skills[i].AddLevel;
				m_Skills[i].SkillExp = 0;
				m_Skills[i].NextSkillExp = PlayerSet.m_cMagicLevelExp.GetNextExp(nSkillId, nLevel);
				m_Skills[i].TempSkill = TRUE;
				m_Skills[i].MaxTimes = 0;
				m_Skills[i].RemainTimes = 0;
				m_Skills[i].NextCastTime = 0;
				m_Skills[i].WaitCastTime = 0;

				if (m_Skills[i].SkillLevel > 0)
				{
					ISkill* pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
					if (!pSkill)
						return;
					if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
					{
						((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
					}
					if (!pSkill->IsAura())
					{
						eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
						DWORD dwCastTime = 0;
						if (eStyle == SKILL_SS_Missles
							|| eStyle == SKILL_SS_Melee
							|| eStyle == SKILL_SS_InitiativeNpcState
							|| eStyle == SKILL_SS_PassivityNpcState)
						{
							dwCastTime = pSkill->GetDelayPerCast(Npc[m_nNpcIndex].m_bRideHorse);
						}
						else
						{
							switch (eStyle)
							{
							case SKILL_SS_Thief:
							{
								dwCastTime = ((KThiefSkill*)pSkill)->GetDelayPerCast();
							}break;
							default:
								return;
							}
						}
						SetNextCastTime(m_Skills[i].SkillId, SubWorld[Npc[m_nNpcIndex].m_SubWorldIndex].m_dwCurrentTime, SubWorld[Npc[m_nNpcIndex].m_SubWorldIndex].m_dwCurrentTime + dwCastTime);
					}
				}
			}
		}
	}
	else
	{
		m_nAllSkillV += nLevel;
		if (m_nAllSkillV > 0)
			for (INT i = 0; i < MAX_NPCSKILL; i++)
			{
				if (!IsBaseSkill(m_Skills[i].SkillId))
				{
					m_Skills[i].AddLevel += nLevel;
					m_Skills[i].CurrentSkillLevel += nLevel;
				}

				/*if (m_Skills[i].SkillLevel > 0)*/
//				{
//					ISkill* pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
//					if (!pSkill)
//						continue;
//					if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
//					{
////#ifdef _SERVER
////						printf("AllSkillV: InputSkillID: %d, SkillID: %d, SkillLevel: %d\n", m_Skills[i].SkillId, pSkill->GetSkillId(), m_Skills[i].CurrentSkillLevel);
////#endif
//						((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
//					}
//				}
			}
	}
}

void KSkillList::SeriesSkillV(INT nSeries, INT nLevel)
{
	if (nSeries >= series_metal && nSeries < series_num)
	{
		for (INT i = 0; i < MAX_NPCSKILL; i++)
		{
			ISkill* pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
			if (!pSkill)
				continue;

			if (pSkill->IsBase())
				continue;

			if (pSkill->GetSkillSeries() == nSeries)
			{
				m_Skills[i].AddLevel += nLevel;
				m_Skills[i].CurrentSkillLevel += nLevel;
			}
		}
	}
}

BOOL KSkillList::IncreaseLevel(INT nIdx, INT nLvl)
{
	if (nIdx <= 0 || nIdx >= MAX_NPCSKILL)
		return FALSE;
	//if (nLvl <= 0)
	//	return FALSE;
	if (m_Skills[nIdx].TempSkill)
		return FALSE;

	m_Skills[nIdx].SkillLevel += nLvl;
	m_Skills[nIdx].CurrentSkillLevel += nLvl;
	m_Skills[nIdx].SkillExp = 0;
	m_Skills[nIdx].NextSkillExp = PlayerSet.m_cMagicLevelExp.GetNextExp(m_Skills[nIdx].SkillId, m_Skills[nIdx].SkillLevel);
	m_Skills[nIdx].TempSkill = FALSE;

	KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[nIdx].SkillId, m_Skills[nIdx].CurrentSkillLevel);
	if (!pOrdinSkill)
		return FALSE;

	if (pOrdinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
	{
		pOrdinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex);
	}
	return TRUE;
}

BOOL KSkillList::IncreaseExp(INT nIdx, INT nExp)
{
	if (nIdx <= 0 || nIdx >= MAX_NPCSKILL)
		return FALSE;
	if (m_Skills[nIdx].SkillLevel <= 0)
		return FALSE;
	if (m_Skills[nIdx].TempSkill)
		return FALSE;
	if (m_Skills[nIdx].NextSkillExp <= 0)
		return FALSE;

	m_Skills[nIdx].SkillExp += nExp;

	if (m_Skills[nIdx].SkillExp >= m_Skills[nIdx].NextSkillExp)
	{
		return IncreaseLevel(nIdx, 1);
	}
	return FALSE;
}

BOOL KSkillList::SetNextExp(INT nIdx, INT nExp)
{
	if (nIdx <= 0 || nIdx >= MAX_NPCSKILL)
		return FALSE;

	if (nExp <= 0)
		return FALSE;

	m_Skills[nIdx].NextSkillExp = nExp;
	return TRUE;
}


BOOL KSkillList::SetTempSkill(INT nIdx, BOOL bTempSkill)
{
	if (nIdx <= 0 || nIdx >= MAX_NPCSKILL)
		return FALSE;

	m_Skills[nIdx].TempSkill = bTempSkill;
	return TRUE;
}

INT KSkillList::Add(INT nSkillID, INT nSkillLevel, INT nSkillExp, BOOL bTempSkill, INT nMaxTimes, INT RemainTimes)
{
	INT i;
	if (nSkillID <= 0 || nSkillLevel < 0)
		return 0;
	i = FindSame(nSkillID);
	if (i)
	{
		if (nSkillLevel > m_Skills[i].SkillLevel)
		{
			INT nInc = nSkillLevel - m_Skills[i].SkillLevel;
			IncreaseLevel(i, nInc);
		}
		m_Skills[i].MaxTimes = 0;
		m_Skills[i].RemainTimes = 0;

		if (nSkillLevel > 0)
		{
			ISkill* pSkill = g_SkillManager.GetSkill(nSkillID, m_Skills[i].CurrentSkillLevel);
			if (!pSkill)
				return 0;
			//Èç¹û¸Ä¼¼ÄÜÊôÓÚ±»¶¯¸¨Öú¼¼ÄÜÊ±£¬ÔòÉèÖÃNpc×´Ì¬
			if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			{
				((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
			}
		}

		return i;
	}

	i = FindFree();
	if (i)
	{
		m_Skills[i].SkillId = nSkillID;
		m_Skills[i].SkillLevel = nSkillLevel;
		m_Skills[i].AddLevel = m_nAllSkillV;
		m_Skills[i].CurrentSkillLevel = m_Skills[i].SkillLevel + m_Skills[i].AddLevel;
		m_Skills[i].SkillExp = nSkillExp;
		m_Skills[i].NextSkillExp = PlayerSet.m_cMagicLevelExp.GetNextExp(nSkillID, nSkillLevel);
		m_Skills[i].TempSkill = bTempSkill;
		m_Skills[i].MaxTimes = 0;
		m_Skills[i].RemainTimes = 0;
		m_Skills[i].NextCastTime = 0;
		m_Skills[i].WaitCastTime = 0;

		if (nSkillLevel > 0)
		{
			ISkill* pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
			if (!pSkill)
				return 0;
			if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			{
				((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex);
			}
		}
		return i;
	}

	return 0;
}

INT KSkillList::GetLevel(INT nSkillID)
{
	INT i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);
	if (i)
	{
		_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		return m_Skills[i].SkillLevel;
	}

	return 0;
}

ULONG KSkillList::GetExp(INT nSkillID)
{
	INT i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);
	if (i)
	{
		return m_Skills[i].SkillExp;
	}

	return 0;
}

ULONG KSkillList::GetNextExp(INT nSkillID)
{
	INT i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);
	if (i)
	{
		return m_Skills[i].NextSkillExp;
	}

	return 0;
}

BOOL KSkillList::IsTempSkill(INT nSkillID)
{
	INT i;

	if (!nSkillID)
		return FALSE;

	i = FindSame(nSkillID);
	if (i)
	{
		return m_Skills[i].TempSkill;
	}

	return FALSE;
}

INT KSkillList::GetCurrentLevel(INT nSkillID)
{
	INT i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);

	if (i)
	{
		_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		if (m_Skills[i].SkillLevel > 0)
			return m_Skills[i].CurrentSkillLevel;
	}

	return 0;
}

BOOL KSkillList::CanCast(INT nSkillID, DWORD dwTime)
{
	if (!nSkillID)
		return FALSE;

	INT i = FindSame(nSkillID);
	if (!i)
	{
		return FALSE;
	}

	if (m_Skills[i].NextCastTime > dwTime)
		return FALSE;


	if (m_Skills[i].SkillLevel <= 0)
		return FALSE;

	return TRUE;
}

void KSkillList::SetNextCastTime(INT nSkillID, DWORD dwCurrentTime, DWORD dwNextCastTime)
{
	if (!nSkillID)
		return;;

	INT i = FindSame(nSkillID);
	if (!i)
		return;

	m_Skills[i].NextCastTime = dwNextCastTime;
	m_Skills[i].WaitCastTime = dwNextCastTime - dwCurrentTime;
#ifndef _SERVER
	m_Skills[i].TotalTime = dwNextCastTime - dwCurrentTime;
#endif
}

#ifndef _SERVER
#define MAX_FIGHTSKILL_SORTLIST 50
#define MAX_LRSKILL_SORTLIST 65
INT	KSkillList::GetSkillSortList(KUiSkillData* pSkillList)
{
	//Ques
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * MAX_FIGHTSKILL_SORTLIST);
	INT nCount = 0;
	ISkill* pSkill = NULL;
	KSkill* pOrdinSkill = NULL;
	INT nSkillLevel = 0;
	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId)
		{
			_ASSERT(m_Skills[i].SkillLevel >= 0);
			nSkillLevel = m_Skills[i].SkillLevel + m_Skills[i].AddLevel;

			if (!m_Skills[i].SkillLevel)
			{
				pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
			}
			else
			{
				pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, nSkillLevel);
			}

			if (!pSkill)
				continue;
			eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();

			switch (eStyle)
			{
			case SKILL_SS_Missles:			//	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	//	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
			case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
			{
				pOrdinSkill = (KSkill*)pSkill;
				if (pOrdinSkill->IsBase())
					continue;
			}break;
			case SKILL_SS_Thief:					//	ÍµÇÔÀà
			{

			}
			break;
			default:
				continue;
			}

			KUiSkillData* pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_FIGHT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nLevel = m_Skills[i].SkillLevel;
			if ((++nCount) == MAX_FIGHTSKILL_SORTLIST)
			{
				break;
			}
		}
	}
	return nCount;
}

INT KSkillList::GetLeftSkillSortList(KUiSkillData* pSkillList)
{
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * MAX_LRSKILL_SORTLIST);
	INT nCount = 1;

	pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
	pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();//ÎïÀí¼¼ÄÜÖÃÇ°!
	pSkillList->nData = 0;

	ISkill* pISkill = NULL;
	KSkill* pOrdinSkill = NULL;
	INT nSkillLevel = 0;
	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		nSkillLevel = m_Skills[i].SkillLevel + m_Skills[i].AddLevel;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel > 0)
		{
			_ASSERT(m_Skills[i].SkillId > 0);
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, nSkillLevel);
			if (!pISkill)
				continue;
			pOrdinSkill = (KSkill*)pISkill;

			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level < pOrdinSkill->GetSkillReqLevel())
				continue;

			eSkillStyle eStyle = (eSkillStyle)pISkill->GetSkillStyle();
			switch (eStyle)
			{
			case SKILL_SS_Missles:			//	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	//	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
			case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
			{
				if ((!pOrdinSkill->IsBase()) &&
					(pOrdinSkill->GetSkillLRInfo() == BothSkill) ||
					(pOrdinSkill->GetSkillLRInfo() == LeftOnlySkill)
					)
				{

				}
				else
					continue;
			}
			break;
			case SKILL_SS_Thief:
			{

				continue;

			}break;
			}

			KUiSkillData* pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_SHORTCUT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nData = nCount / 8;
			nCount++;
			if (nCount >= MAX_LRSKILL_SORTLIST)
				break;

		}
	}
	return nCount;
}

INT KSkillList::GetRightSkillSortList(KUiSkillData* pSkillList)
{
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * MAX_LRSKILL_SORTLIST);

	INT nCount = 1;
	pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
	//pSkillList->uId = (UINT)-1;//ÎïÀí¼¼ÄÜÖÃÇ°!
	pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();//ÎïÀí¼¼ÄÜÖÃÇ°!;	 
	pSkillList->nData = 0;
	ISkill* pISkill = NULL;
	KSkill* pOrdinSkill = NULL;
	INT nSkillLevel = 0;
	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		nSkillLevel = m_Skills[i].SkillLevel + m_Skills[i].AddLevel;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel > 0)
		{
			_ASSERT(m_Skills[i].SkillId > 0);
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, nSkillLevel);
			if (!pISkill)
				continue;

			eSkillStyle eStyle = (eSkillStyle)pISkill->GetSkillStyle();

			pOrdinSkill = (KSkill*)pISkill;

			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level < pOrdinSkill->GetSkillReqLevel())
				continue;

			switch (eStyle)
			{
			case SKILL_SS_Missles:			//	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	//	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
			case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
			{
				if ((!pOrdinSkill->IsBase()) &&
					(pOrdinSkill->GetSkillLRInfo() == BothSkill) ||
					(pOrdinSkill->GetSkillLRInfo() == RightOnlySkill)
					)
				{

				}
				else
					continue;
			}
			break;
			}

			KUiSkillData* pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_SHORTCUT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nData = nCount / 8;
			nCount++;
			if (nCount >= MAX_LRSKILL_SORTLIST)
				break;

		}
	}
	return nCount;
}


INT KSkillList::GetSkillPosition(INT nSkillId)//»ñµÃ¼¼ÄÜÔÚ¼¼ÄÜ½çÃæµÄÎ»ÖÃ
{
	if (nSkillId <= 0) return -1;
	KSkill* pOrdinSkill = NULL;

	ISkill* pISkill = NULL;
	INT nCount = 0;
	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		_ASSERT(m_Skills[i].SkillLevel >= 0);
		if (m_Skills[i].SkillId <= 0) continue;

		if (m_Skills[i].SkillLevel == 0)
		{
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
		}
		else
		{
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel + m_Skills[i].AddLevel);
		}

		if (!pISkill)
			continue;


		eSkillStyle eStyle = (eSkillStyle)pISkill->GetSkillStyle();
		switch (eStyle)
		{
		case SKILL_SS_Missles:			//	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
		case SKILL_SS_Melee:
		case SKILL_SS_InitiativeNpcState:	//	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
		case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
		{
			pOrdinSkill = (KSkill*)pISkill;
			if (
				(!m_Skills[i].SkillId) ||
				//(pOrdinSkill->IsPhysical())
				(pOrdinSkill->IsBase())
				)
			{
				continue;
			}
		}
		break;
		case SKILL_SS_Thief:
		{
		}break;
		}


		if (m_Skills[i].SkillId == nSkillId)
		{
			return nCount;
		}
		nCount++;

	}
	return nCount;
}

void KSkillList::SetAddLevel(INT nId, INT nAdd)
{
	INT i = FindSame(nId);

	if (i > 0 && i < MAX_NPCSKILL)
	{
		m_Skills[i].AddLevel = nAdd;
	}
}

void KSkillList::SetCurLevel(INT nId, INT nLevel)
{
	INT i = FindSame(nId);

	if (i > 0 && i < MAX_NPCSKILL)
	{
		m_Skills[i].CurrentSkillLevel = nLevel;
	}
}
#endif

#ifdef _SERVER
INT	KSkillList::UpdateDBSkillList(BYTE* pSkillBuffer)
{
	if (!pSkillBuffer)
		return -1;

	INT nCount = 0;

	TDBSkillData* pSkillData = (TDBSkillData*)pSkillBuffer;
	for (INT i = 0; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].TempSkill)
			continue;
		if (m_Skills[i].SkillId > 0)
		{
			pSkillData->m_nSkillId = m_Skills[i].SkillId;
			pSkillData->m_nSkillLevel = m_Skills[i].SkillLevel;
			pSkillData->m_nSkillVal = m_Skills[i].SkillExp;
			pSkillData++;
			nCount++;
		}
	}
	return nCount;
}

INT		KSkillList::RollBackSkills(bool bRbAll)
{
	INT nTotalSkillPoint = 0;

	for (INT i = 0; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillId > 0)
		{
			ISkill* pSkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
			if (pSkill)
			{
				if (pSkill->IsBase()) continue;
				if (!bRbAll && pSkill->IsExp()) continue;
				if (!pSkill->IsExp())
					nTotalSkillPoint += m_Skills[i].SkillLevel;
				m_Skills[i].SkillLevel = 0;
				m_Skills[i].AddLevel = m_nAllSkillV;
				m_Skills[i].CurrentSkillLevel = 0;
			}
		}
	}
	return nTotalSkillPoint;
}
#endif

INT KSkillList::GetAddSkillDamage(INT nSkillID)
{
	INT nAddP = 0;

	if (nSkillID <= 0 || nSkillID >= MAX_SKILL)
		return nAddP;

	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillLevel)
		{
			KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
			if (pSkill)
			{
				KMagicAttrib* pMagicData = NULL;
				pMagicData = pSkill->GetAddSkillDamage();
				for (INT j = 0; j < MAX_ADDSKILLDAMAGE; j++)
				{
					if (!pMagicData[j].nAttribType)
						continue;
					if (pMagicData[j].nValue[0] == nSkillID)
					{
						KSkill* nSkill = (KSkill*)g_SkillManager.GetSkill(nSkillID, GetLevel(nSkillID));


						nAddP += pMagicData[j].nValue[2];
					}
				}
			}
		}
	}
	return nAddP;
}

UINT KSkillList::GetAddSkillDamageBonus(INT nSkillID, INT CurrentSkillEnhancePercent)
{
	INT nAddP = 0;

	if (nSkillID <= 0 || nSkillID >= MAX_SKILL)
		return nAddP;

	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_Skills[i].SkillLevel)
		{
			KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].CurrentSkillLevel);
			if (pSkill)
			{
				KMagicAttrib* pMagicData = NULL;
				pMagicData = pSkill->GetAddSkillDamage();
				for (INT j = 0; j < MAX_ADDSKILLDAMAGE; j++)
				{
					if (!pMagicData[j].nAttribType)
						continue;
					if (pMagicData[j].nValue[0] == nSkillID)
					{
						KSkill* nSkill = (KSkill*)g_SkillManager.GetSkill(pMagicData[j].nValue[1], GetCurrentLevel(pMagicData[j].nValue[1]));
						if (nSkill)
						{
							KMagicAttrib* nMagicData = NULL;
							nMagicData = nSkill->GetDamageAttribs();

							if (nMagicData[9].nAttribType == magic_physicsdamage_v || nMagicData[9].nAttribType == magic_physicsenhance_p)
							{
								nAddP += (pMagicData[j].nValue[2] * (nMagicData[9].nValue[0] + CurrentSkillEnhancePercent)) / MAX_PERCENT;
							}
							if (nMagicData[10].nAttribType == magic_colddamage_v)
							{
								nAddP += (pMagicData[j].nValue[2] * (nMagicData[10].nValue[0] + CurrentSkillEnhancePercent)) / MAX_PERCENT;
							}
							if (nMagicData[11].nAttribType == magic_firedamage_v)
							{
								nAddP += (pMagicData[j].nValue[2] * (nMagicData[11].nValue[0] + CurrentSkillEnhancePercent)) / MAX_PERCENT;
							}
							if (nMagicData[12].nAttribType == magic_lightingdamage_v || nMagicData[12].nAttribType == magic_lightingdamage_p)
							{
								nAddP += (pMagicData[j].nValue[2] * (nMagicData[12].nValue[0] + CurrentSkillEnhancePercent)) / MAX_PERCENT;
							}
							if (nMagicData[13].nAttribType == magic_poisondamage_v)
							{
								nAddP += (pMagicData[j].nValue[2] * (nMagicData[13].nValue[0] + CurrentSkillEnhancePercent)) / MAX_PERCENT;
							}
						}
					}
				}
			}
		}
	}
	return nAddP;
}

#ifndef _SERVER
INT KSkillList::GetNextSkillState(INT nIndex)
{
	if (nIndex > 0)
	{
		INT i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			INT nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if (nCurLevel > 0)
				{
					KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill)
						continue;

					if (pOrdinSkill->IsAura())
						continue;

					if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles ||
						pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState) && pOrdinSkill->IsTargetSelf() && pOrdinSkill->GetSkillReqLevel() <= Npc[m_nNpcIndex].m_Level)
					{
						j++;
						if (j == nIndex)
							return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	INT _nCount = 0;
	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		INT nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if (nCurLevel > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill)
					continue;

				if (pOrdinSkill->IsAura())
					continue;

				if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles ||
					pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState) && pOrdinSkill->IsTargetSelf() && pOrdinSkill->GetSkillReqLevel() <= Npc[m_nNpcIndex].m_Level)
					_nCount++;
			}
			else
				continue;
		}
	}
	return _nCount;
}

INT KSkillList::GetNextSkillFight(INT nIndex)
{
	if (nIndex > 0)
	{
		INT i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			INT nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if (nCurLevel > 0)
				{
					KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill)
						continue;

					if (pOrdinSkill->IsBase())
						continue;

					if (pOrdinSkill->IsAura())
						continue;

					if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles ||
						pOrdinSkill->GetSkillStyle() == SKILL_SS_Melee) && pOrdinSkill->IsTargetEnemy() && pOrdinSkill->GetSkillReqLevel() <= Npc[m_nNpcIndex].m_Level)
					{
						j++;
						if (j == nIndex)
							return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	INT _nCount = 0;

	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		INT nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if (nCurLevel > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill)
					continue;

				if (pOrdinSkill->IsBase())
					continue;

				if (pOrdinSkill->IsAura())
					continue;

				if ((pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles ||
					pOrdinSkill->GetSkillStyle() == SKILL_SS_Melee) && pOrdinSkill->IsTargetEnemy() && pOrdinSkill->GetSkillReqLevel() <= Npc[m_nNpcIndex].m_Level)
					_nCount++;
			}
			else
				continue;
		}
	}
	return _nCount;
}

INT KSkillList::GetNextSkillAura(INT nIndex)
{
	if (nIndex > 0)
	{
		INT i = 0, j = 0;
		for (i = 1; i < MAX_NPCSKILL; i++)
		{
			INT nCurLevel = 0;
			if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
			{
				nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
				if (nCurLevel > 0)
				{
					KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
					if (!pOrdinSkill)
						continue;

					if (pOrdinSkill->IsAura() && pOrdinSkill->GetSkillReqLevel() <= Npc[m_nNpcIndex].m_Level)
					{
						j++;
						if (j == nIndex)
							return m_Skills[i].SkillId;
					}
				}
				else
					continue;
			}
		}
		return 0;
	}
	INT _nCount = 0;
	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		INT nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if (nCurLevel > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill)
					continue;

				if (pOrdinSkill->IsAura() && pOrdinSkill->GetSkillReqLevel() <= Npc[m_nNpcIndex].m_Level)
					_nCount++;
			}
			else
				continue;
		}
	}
	return _nCount;
}

INT KSkillList::FindSkillLifeReplenish()
{
	INT i = 0, j = 0;
	for (i = 1; i < MAX_NPCSKILL; i++)
	{
		INT nCurLevel = 0;
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel)
		{
			nCurLevel = this->GetCurrentLevel(m_Skills[i].SkillId);
			if (nCurLevel > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_Skills[i].SkillId, nCurLevel);
				if (!pOrdinSkill)
					continue;

				if (pOrdinSkill->IsSkillLifeReplenish())
					return m_Skills[i].SkillId;
			}
			else
				continue;
		}
	}
	return 0;
}
#endif

INT KSkillList::GetHorseNextCastTimeByIndex(INT nSkListIndex)
{
	if (nSkListIndex <= 0 || nSkListIndex >= MAX_NPCSKILL)
		return -1;

	return	m_Skills[nSkListIndex].NextHorseCastTime;
}

INT KSkillList::GetHorseNextCastTime(INT nSkillID)
{
	if (!nSkillID)
		return -1;

	INT i = FindSame(nSkillID);
	if (!i)
		return -1;

	return	m_Skills[i].NextHorseCastTime;
}

INT KSkillList::GetNextCastTimeByIndex(INT nSkListIndex)
{
	//	m_Skills[nSkillID].NextCastTime = dwTime;
	//	return;
	if (nSkListIndex <= 0 || nSkListIndex >= MAX_NPCSKILL)
		return -1;

	return	m_Skills[nSkListIndex].NextCastTime;
}

INT KSkillList::GetNextCastTime(INT nSkillID)
{
	//	m_Skills[nSkillID].NextCastTime = dwTime;
	//	return;
	if (!nSkillID)
		return -1;

	INT i = FindSame(nSkillID);
	if (!i)
		return -1;
	return	m_Skills[i].NextCastTime;
}

//设置骑马技能解冻时间
void	KSkillList::SetHorseNextCastTimeByIndex(INT nSkListIndex, DWORD dwTime)
{
	if (nSkListIndex <= 0 && nSkListIndex >= MAX_NPCSKILL) return;
	m_Skills[nSkListIndex].NextHorseCastTime = dwTime;
}
//下次发技能时间
void KSkillList::SetNextCastTimeByIndex(INT nSkListIndex, DWORD dwTime)
{
	if (nSkListIndex <= 0 && nSkListIndex >= MAX_NPCSKILL) return;
	m_Skills[nSkListIndex].NextCastTime = dwTime;
}

//设置骑马技能解冻时间
void	KSkillList::SetHorseNextCastTime(INT nSkillID, DWORD dwTime)
{
	if (!nSkillID)
		return;
	INT i = FindSame(nSkillID);
	if (!i)
		return;
	m_Skills[i].NextHorseCastTime = dwTime;
}