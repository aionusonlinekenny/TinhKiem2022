#include "KCore.h"
#include <math.h>
#include "KNpc.h"
#include "KSubWorld.h"
#include "KRegion.h"
#include "GameDataDef.h"
#include "KNpcSet.h"
#include "KPlayer.h"
#ifndef _SERVER
#include "CoreShell.h"
#include "Scene\KScenePlaceC.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "KOption.h"
#ifndef TOOLVERSION
#include "../../Headers/IClient.h"
#endif
#endif
#include "Scene/SceneDataDef.h"
#include "KMath.h"

extern INT GetRandomNumber(INT nMin, INT nMax);
//#define	ENCHANT_SETTING_PATH	"settings\\npc"
//#define	ENCHANT_NORMAL_FILE		"normalunique.txt"
//#define	ENCHANT_SPECIAL_FILE	"speicalunique.txt"
extern KLuaScript* g_pNpcLevelScript;

KNpcSet	NpcSet;

KNpcSet::KNpcSet()
{
	m_dwIDCreator = 1000;
}

// »Æ½ð¹ÖÎïÉè¶¨ÎÄ¼þµÄ³õÊ¼»¯Ã»ÓÐ·ÅÔÚÕâÀï£¬Ö±½Ó·ÅÔÚ core µÄ³õÊ¼»¯ÀïÃæ
void KNpcSet::Init()
{
	// Add by Freeway Chen in 2003.6.29    
	GenRelationTable();
	m_FreeIdx.Init(MAX_NPC);
	m_UseIdx.Init(MAX_NPC);

	// ¿ªÊ¼Ê±ËùÓÐµÄÊý×éÔªËØ¶¼Îª¿Õ
	INT i = 0;
	for (INT i = MAX_NPC - 1; i > 0; i--)
	{
		m_FreeIdx.Insert(i);
		Npc[i].m_Node.m_nIndex = i;
#ifdef _SERVER
		Npc[i].m_cDeathCalcExp.Init(i);
#endif
	}

	LoadPlayerBaseValue(PLAYER_BASE_VALUE);

#ifdef _SERVER
	KIniFile	cPKIni;
	//	g_SetFilePath("\\");
	if (cPKIni.Load(PLAYER_PK_SET_FILE))
	{
		cPKIni.GetInteger("PK", "Rate", 20, &m_nPKDamageRate);
		cPKIni.GetInteger("PK", "LevelRate", 0, &m_nLevelPKDamageRate);
		cPKIni.GetInteger("PK", "SpecialRate", 50, &m_nNpcSpecialDamageRate);
		cPKIni.GetInteger("PK", "FactionPKFaction", 1, &m_nFactionPKFactionAddPKValue);
		cPKIni.GetInteger("PK", "KillerPKFaction", 1, &m_nKillerPKFactionAddPKValue);
		cPKIni.GetInteger("PK", "EnmityPK", 2, &m_nEnmityAddPKValue);
		cPKIni.GetInteger("PK", "BeKilled", -1, &m_nBeKilledAddPKValue);
		cPKIni.GetInteger("PK", "LevelDistance", 25, &m_nLevelDistance);
		cPKIni.GetInteger("PK", "LevelBoundaryPKPunish", 130, &m_nLevelBoundaryPKPunish);
		cPKIni.GetInteger("PK", "ButcherPKExercise", 10, &m_nButcherPKExercise);
		cPKIni.GetInteger("PK", "NotSubPKExpPercent", -20, &m_nNotSubPKExpPercent);
		cPKIni.GetInteger("PK", "NotEnmityExpPercent", -30, &m_nNotEnmityExpPercent);
		cPKIni.GetInteger("PK", "NotFightExpPercent", -80, &m_nNotFightExpPercent);
		cPKIni.GetInteger("PK", "MurderPK", 1, &m_nMurderAddPKValue);
		cPKIni.GetInteger("PK", "PKWarOpen", 1, &m_nPKWarOpen);
		cPKIni.GetInteger("PK", "PKMurderOpen", 1, &m_nPKMurderOpen);
		cPKIni.GetInteger("PK", "PKTongOpen", 0, &m_nPKTongOpen);
		cPKIni.GetInteger("PK", "PKTongOpenCamp", 0, &m_nPKTongOpenCamp);
		cPKIni.GetInteger("PK", "NormalPKTimeLong", 180000, &m_nNormalPKTimeLong);
		cPKIni.GetInteger("PK", "FightPKTimeLong", 5000, &m_nFightPKTimeLong);
		cPKIni.GetInteger("PK", "NotFightWhenHightPK", 6, &m_nNotFightWhenHightPK);
		cPKIni.GetInteger("PK", "PKNotSwitchPKWhenLock", 3, &m_nPKNotSwitchPKWhenLock);
		cPKIni.GetInteger("PK", "SparPacific", 0, &m_nSparPacific);
	}
	else
	{
		m_nPKDamageRate = 20;
		m_nNpcSpecialDamageRate = 50;
		m_nFactionPKFactionAddPKValue = 1;
		m_nKillerPKFactionAddPKValue = 1;
		m_nEnmityAddPKValue = 2;
		m_nBeKilledAddPKValue = -1;
		m_nLevelDistance = 25;
		m_nLevelBoundaryPKPunish = 130;
		m_nButcherPKExercise = 10;
		m_nNotSubPKExpPercent = -20;
		m_nNotEnmityExpPercent = -30;
		m_nNotFightExpPercent = -80;
		m_nMurderAddPKValue = 1;
		m_nPKWarOpen = 1;
		m_nPKMurderOpen = 1;
		m_nPKTongOpen = 0;
		m_nPKTongOpenCamp = 0;
		m_nNormalPKTimeLong = 180000;
		m_nFightPKTimeLong = 5000;
		m_nNotFightWhenHightPK = 6;
		m_nPKNotSwitchPKWhenLock = 3;
		m_nSparPacific = 0;
	}
#endif

#ifndef _SERVER
	m_nShowPateFlag = 0;
	m_nShowPateFlag |= PATE_NAME;
	m_nShowPateFlag |= PATE_LIFE;
	m_nShowPateFlag |= PATE_OBJ;
	ZeroMemory(m_RequestNpc, sizeof(m_RequestNpc));
	m_RequestFreeIdx.Init(MAX_NPC_REQUEST);
	m_RequestUseIdx.Init(MAX_NPC_REQUEST);

	for (i = MAX_NPC_REQUEST - 1; i > 0; i--)
	{
		m_RequestFreeIdx.Insert(i);
	}
#endif

}

void KNpcSet::LoadPlayerBaseValue(LPSTR szFile)
{
	KIniFile	File;

	File.Load(szFile);

	File.GetInteger("Common", "HurtFrame", 12, &m_cPlayerBaseValue.nHurtFrame);
	File.GetInteger("Common", "RunSpeed", 10, &m_cPlayerBaseValue.nRunSpeed);
	File.GetInteger("Common", "WalkSpeed", 5, &m_cPlayerBaseValue.nWalkSpeed);
	File.GetInteger("Common", "AttackFrame", 20, &m_cPlayerBaseValue.nAttackFrame);
#ifndef _SERVER
	File.GetInteger("Male", "WalkFrame", 15, &m_cPlayerBaseValue.nWalkFrame[0]);
	File.GetInteger("Female", "WalkFrame", 15, &m_cPlayerBaseValue.nWalkFrame[1]);
	File.GetInteger("Male", "RunFrame", 15, &m_cPlayerBaseValue.nRunFrame[0]);
	File.GetInteger("Female", "RunFrame", 15, &m_cPlayerBaseValue.nRunFrame[1]);
	File.GetInteger("Male", "StandFrame", 15, &m_cPlayerBaseValue.nStandFrame[0]);
	File.GetInteger("Female", "StandFrame", 15, &m_cPlayerBaseValue.nStandFrame[1]);
#endif
	File.Clear();
}

BOOL KNpcSet::IsNpcExist(INT nIdx, DWORD dwId)
{
	if (Npc[nIdx].m_dwID == dwId)
		return TRUE;
	else
		return FALSE;
}

// Add by Freeway Chen in 2003.6.29
NPC_RELATION KNpcSet::GenOneRelation(NPCKIND Kind1, NPCKIND Kind2, NPCCAMP Camp1, NPCCAMP Camp2)
{
	// Â·ÈËNPCÃ»ÓÐÕ½¶·¹ØÏµ
	if (Kind1 == kind_dialoger || Kind2 == kind_dialoger)
		return relation_dialog;

	if (Kind1 >= kind_bird || Kind2 >= kind_bird)
		return relation_none;

	// Â·ÈËÕóÓªÃ»ÓÐÕ½¶·¹ØÏµ
	if (Camp1 == camp_event || Camp2 == camp_event)
		return relation_none;

	// ÐÂÊÖºÍ¶¯Îï»¹ÊÇÕ½¶·¹ØÏµ
	if ((Camp1 == camp_begin && Camp2 == camp_animal)
		|| (Camp1 == camp_animal && Camp2 == camp_begin))
		return relation_enemy;

	// Ö»ÒªÓÐÒ»¸öÐÂÊÖ£¬¾Í²»´æÔÚÕ½¶·¹ØÏµ(ÊÇÍ¬ÃË¹ØÏµ£¬´ó¼Ò°ïÐÂÊÖ)
	if (Camp1 == camp_begin || Camp2 == camp_begin)
		return relation_ally;

	// Á½¸ö¶¼ÊÇÍæ¼Ò
	if (Kind1 == kind_player && Kind2 == kind_player)
	{
		// Ö»ÒªÓÐÒ»¸öÍæ¼Ò¿ªÁËPK£¬¾ÍÒ»¶¨´æÔÚÕ½¶·¹ØÏµ
		if (Camp1 == camp_free || Camp2 == camp_free)
		{
			return relation_enemy;
		}
	}
	// Í¬ÕóÓªÎª»ï°é¹ØÏµ
	if (Camp1 == Camp2)
		return relation_ally;

	// ÆäËûÇé¿öÎªÕ½¶·¹ØÏµ
	return relation_enemy;
}



// Add by Freeway Chen in 2003.7.14
INT KNpcSet::GenRelationTable()
{
	INT nKind1 = 0;
	INT nKind2 = 0;
	INT nCamp1 = 0;
	INT nCamp2 = 0;

	for (nKind1 = 0; nKind1 < kind_num; nKind1++)
	{
		for (nKind2 = 0; nKind2 < kind_num; nKind2++)
		{
			for (nCamp1 = 0; nCamp1 < camp_num; nCamp1++)
			{
				for (nCamp2 = 0; nCamp2 < camp_num; nCamp2++)
				{
					m_RelationTable[nKind1][nKind2][nCamp1][nCamp2] = GenOneRelation(
						(NPCKIND)nKind1,
						(NPCKIND)nKind2,
						(NPCCAMP)nCamp1,
						(NPCCAMP)nCamp2);
				}
			}
		}
	}
	return true;
}




INT	KNpcSet::SearchID(DWORD dwID)
{
	//	INT nMaxNpc = m_BinTree.GetCount();
	//	for (INT i = 0; i < MAX_NPC; i++)
	//	{
	//		if (Npc[i].m_dwID == dwID)
	//			return Npc[i].m_Index;
	//	}
	INT nIdx = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;
		if (Npc[nIdx].m_dwID == dwID)
			return nIdx;
	}

	return 0;
}

#ifndef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º²éÕÒÄ³¸öClientIDµÄnpcÊÇ·ñ´æÔÚ
//---------------------------------------------------------------------------
INT		KNpcSet::SearchClientID(KClientNpcID sClientID)
{
	INT nIdx = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;
		if (Npc[nIdx].m_sClientNpcID.m_dwRegionID == sClientID.m_dwRegionID &&
			Npc[nIdx].m_sClientNpcID.m_nNo == sClientID.m_nNo)
			return nIdx;
	}

	return 0;
}
#endif

INT KNpcSet::SearchName(LPSTR szName)
{
	//	for (INT i = 0; i < MAX_NPC; i ++)
	//	{
	//		if (!strcmp(Npc[i].Name, szName))
	//			return i ;
	//	}
	INT nIdx = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;
		if (g_StrCmp(Npc[nIdx].Name, szName))
			return nIdx;
	}
	return 0;
}

INT KNpcSet::SearchUUID(DWORD dwID)
{
	INT nIdx = 0;
	DWORD	dwNameID = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;
		if (Npc[nIdx].m_Kind != kind_player)
			continue;
		DWORD dwNameID = 0;
#ifdef _SERVER
		dwNameID = Player[Npc[nIdx].GetPlayerIdx()].m_dwID;
#else
		dwNameID = g_FileName2Id(Npc[nIdx].Name);
#endif
		if (dwID == dwNameID)
			return nIdx;
	}
	return 0;
}

INT KNpcSet::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

#ifndef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÌí¼ÓÒ»¸ö¿Í»§¶Ënpc£¨ÐèÒªÉè¶¨ClientNpcID£©
//---------------------------------------------------------------------------
INT		KNpcSet::AddClientNpc(INT nTemplateID, INT nRegionX, INT nRegionY, INT nMpsX, INT nMpsY, INT nNo)
{
	INT nNpcNo, nNpcSettingIdxInfo, nMapX, nMapY, nOffX, nOffY, nRegion;

	nNpcSettingIdxInfo = MAKELONG(1, nTemplateID);
	SubWorld[0].Mps2Map(nMpsX, nMpsY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion < 0)
		return 0;

	nNpcNo = this->AddServerNpcB(nNpcSettingIdxInfo, 0, nRegion, nMapX, nMapY, nOffX, nOffY);

	if (nNpcNo > 0)
	{
		Npc[nNpcNo].m_sClientNpcID.m_dwRegionID = MAKELONG(nRegionX, nRegionY);
		Npc[nNpcNo].m_sClientNpcID.m_nNo = nNo;
		Npc[nNpcNo].m_RegionIndex = nRegion;
		Npc[nNpcNo].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
		Npc[nNpcNo].m_bClientOnly = TRUE;
		Npc[nNpcNo].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
		SubWorld[0].m_Region[nRegion].DecNpcRef(Npc[nNpcNo].m_MapX, Npc[nNpcNo].m_MapY);
	}

	return nNpcNo;
}
#endif

INT KNpcSet::Add(INT nSubWorld, void* pNpcInfo, KMapList* pMapListInfo)
{
	if (NULL == pNpcInfo || NULL == pMapListInfo)
		return false;

	KSPNpc* pKSNpcInfo = (KSPNpc*)pNpcInfo;

	INT nMpsX = pKSNpcInfo->nPositionX;
	INT nMpsY = pKSNpcInfo->nPositionY;
	INT nNpcSettingIdxInfo = MAKELONG(pKSNpcInfo->nLevel, pKSNpcInfo->nTemplateID);
	INT nRet = AddServerNpcA(nNpcSettingIdxInfo, nSubWorld, nMpsX, nMpsY, 0, 0, 0, 0, 0, 0);
	if (nRet)
	{
		Npc[nRet].m_TrapScriptID = 0;
		g_StrCpyLen(Npc[nRet].Name, pKSNpcInfo->szName, sizeof(Npc[nRet].Name)); // 按照字节数 复制名字

		char nTempName[64];
		ZeroMemory(nTempName, sizeof(nTempName));
		g_NpcChangeName.GetString(pKSNpcInfo->nTemplateID + 2, 2, "", nTempName, sizeof(nTempName));
		g_StrCpyLen(Npc[nRet].Name, nTempName, sizeof(Npc[nRet].Name));
		// 修改NPC的为地图默认数据
		/*Npc[nRet].m_Camp        = pKSNpcInfo->cCamp;    //阵营
		  Npc[nRet].m_CurrentCamp = pKSNpcInfo->cCamp;    //阵营
		*/

		/*if (SubWorld[nSubWorld].m_SubWorldID==42)
		{
			printf("--地图%d,刷怪:%s 成功---\n",SubWorld[nSubWorld].m_SubWorldID,Npc[nRet].Name);
		}*/

		Npc[nRet].m_Series = (BYTE)pKSNpcInfo->cSeries; // 五行
		if (pKSNpcInfo->shKind >= kind_normal && pKSNpcInfo->shKind < kind_num)
			Npc[nRet].m_Kind = pKSNpcInfo->shKind; // 种类

		if (pKSNpcInfo->cCamp >= camp_begin && pKSNpcInfo->cCamp < camp_num)
		{
			Npc[nRet].m_Camp = pKSNpcInfo->cCamp;		 // 阵营
			Npc[nRet].m_CurrentCamp = pKSNpcInfo->cCamp; // 阵营
		}

		char nMapNpc[32] = { 0 };
		INT nIsCreat = 0;
		ZeroMemory(nMapNpc, sizeof(nMapNpc));
		sprintf(nMapNpc, "%d_IsScript", SubWorld[nSubWorld].m_SubWorldID);
		g_NpcMapDropRate.GetInteger("List", nMapNpc, 0, &nIsCreat);
		Npc[nRet].IsExeGoldScript = nIsCreat;
		sprintf(nMapNpc, "%d_IsCreat", SubWorld[nSubWorld].m_SubWorldID);
		g_NpcMapDropRate.GetInteger("List", nMapNpc, 0, &nIsCreat); // 默认执行全局脚本
		Npc[nRet].IsCreatBoss = nIsCreat;

		// 如果是黄金怪物，备份数据
		if (pKSNpcInfo->bSpecialNpc) // 特殊NPC
		{
			Npc[nRet].m_cGold.SetGoldTypeAndBackData();
		}
		else
		{
			Npc[nRet].m_cGold.SetGoldType(FALSE);
		}

#ifdef _SERVER // 开始加载爆率
		if (Npc[nRet].m_Kind == kind_normal)
		{
			INT nSet = 0;
			nSet = CheckThisNpc(Npc[nRet].m_NpcSettingIdx, "NpcsetID"); // 检查是否在禁止列表中

			if (!nSet)
			{
				KNpcTemplate* pNpcTemp = NULL;
				if (pNpcTemp = NpcSet.GetTemplate(Npc[nRet].m_NpcSettingIdx, 0))
				{
					if (Npc[nRet].m_cGold.GetGoldType() > 1 && Npc[nRet].m_cGold.GetGoldType() <= 4 && pKSNpcInfo->nGDropFile[0])
					{
						pNpcTemp->InitDropRate(nRet, pKSNpcInfo->nGDropFile); // 蓝怪绿怪爆率修改
					}
					else if (Npc[nRet].m_cGold.GetGoldType() == 0 && pKSNpcInfo->nNDropFile[0])
					{
						pNpcTemp->InitDropRate(nRet, pKSNpcInfo->nNDropFile); // 白怪爆率修改
					}
				}
				else if (pNpcTemp = NpcSet.GetTemplate(Npc[nRet].m_NpcSettingIdx, Npc[nRet].m_Level))
				{
					if (Npc[nRet].m_cGold.GetGoldType() > 1 && Npc[nRet].m_cGold.GetGoldType() <= 4 && pKSNpcInfo->nGDropFile[0])
					{
						pNpcTemp->InitDropRate(nRet, pKSNpcInfo->nGDropFile); // 蓝怪绿怪爆率修改
					}
					else if (Npc[nRet].m_cGold.GetGoldType() == 0 && pKSNpcInfo->nNDropFile[0])
					{
						pNpcTemp->InitDropRate(nRet, pKSNpcInfo->nNDropFile); // 白怪爆率修改
					}
				}

				//--------------------开始转变 加强怪物
				INT npMapGoldRate = 0;
				char nMapGoldRate[32] = { 0 };
				sprintf(nMapGoldRate, "%d_AutoGoldenNpc", SubWorld[nSubWorld].m_SubWorldID); // 38_AutoGoldenNpc
				g_NpcMapDropRate.GetInteger("List", nMapGoldRate, 0, &npMapGoldRate);		 // 生成加强怪物的概率

				if (npMapGoldRate < 0 && Npc[nRet].m_cGold.GetGoldType() <= 0)
				{
					Npc[nRet].m_cGold.RandChangeGold(1, 0, nSubWorld);
				}
			}
			else
			{
				Npc[nRet].m_pDropRate = NULL;
			}
			// 开始调整防御
			if (pMapListInfo->nNpcDefense > 0)
			{
				Npc[nRet].m_CurrentFireResist = pMapListInfo->nNpcDefense;	  // Npc的当前火抗性
				Npc[nRet].m_CurrentColdResist = pMapListInfo->nNpcDefense;	  // Npc的当前冰抗性
				Npc[nRet].m_CurrentPoisonResist = pMapListInfo->nNpcDefense;  // Npc的当前毒抗性
				Npc[nRet].m_CurrentLightResist = pMapListInfo->nNpcDefense;	  // Npc的当前电抗性
				Npc[nRet].m_CurrentPhysicsResist = pMapListInfo->nNpcDefense; // Npc的当前物理抗性

				Npc[nRet].m_CurrentFireResistMax = MAX_RESIST;	  // Npc的当前最大火抗性
				Npc[nRet].m_CurrentColdResistMax = MAX_RESIST;	  // Npc的当前最大冰抗性
				Npc[nRet].m_CurrentPoisonResistMax = MAX_RESIST;  // Npc的当前最大毒抗性
				Npc[nRet].m_CurrentLightResistMax = MAX_RESIST;	  // Npc的当前最大电抗性
				Npc[nRet].m_CurrentPhysicsResistMax = MAX_RESIST; // Npc的当前最大物理抗性

				Npc[nRet].m_FireResist = pMapListInfo->nNpcDefense; // 原始的五防
				Npc[nRet].m_ColdResist = pMapListInfo->nNpcDefense;
				Npc[nRet].m_PoisonResist = pMapListInfo->nNpcDefense;
				Npc[nRet].m_LightResist = pMapListInfo->nNpcDefense;
				Npc[nRet].m_PhysicsResist = pMapListInfo->nNpcDefense;

				if (Npc[nRet].m_CurrentFireResist > MAX_RESIST)
					Npc[nRet].m_CurrentFireResist = MAX_RESIST;
				if (Npc[nRet].m_CurrentColdResist > MAX_RESIST)
					Npc[nRet].m_CurrentColdResist = MAX_RESIST;
				if (Npc[nRet].m_CurrentPoisonResist > MAX_RESIST)
					Npc[nRet].m_CurrentPoisonResist = MAX_RESIST;
				if (Npc[nRet].m_CurrentLightResist > MAX_RESIST)
					Npc[nRet].m_CurrentLightResist = MAX_RESIST;
				if (Npc[nRet].m_CurrentPhysicsResist > MAX_RESIST)
					Npc[nRet].m_CurrentPhysicsResist = MAX_RESIST;

				if (Npc[nRet].m_CurrentFireResistMax > MAX_RESIST)
					Npc[nRet].m_CurrentFireResistMax = MAX_RESIST;
				if (Npc[nRet].m_CurrentColdResistMax > MAX_RESIST)
					Npc[nRet].m_CurrentColdResistMax = MAX_RESIST;
				if (Npc[nRet].m_CurrentPoisonResistMax > MAX_RESIST)
					Npc[nRet].m_CurrentPoisonResistMax = MAX_RESIST;
				if (Npc[nRet].m_CurrentLightResistMax > MAX_RESIST)
					Npc[nRet].m_CurrentLightResistMax = MAX_RESIST;
				if (Npc[nRet].m_CurrentPhysicsResistMax > MAX_RESIST)
					Npc[nRet].m_CurrentPhysicsResistMax = MAX_RESIST;
			}
			// 设置生命
			if (pMapListInfo->nNpcMaxLife > 0)
			{
				Npc[nRet].m_CurrentLife = pMapListInfo->nNpcMaxLife;	// Npc的当前生命
				Npc[nRet].m_CurrentLifeMax = pMapListInfo->nNpcMaxLife; // Npc的当前生命最大值
			}
			// 设置经验倍率
			if (pMapListInfo->nExpSale > 0)
			{
				Npc[nRet].IsJinYan = pMapListInfo->nExpSale; // m_Experience
			}
		}
		if (Npc[nRet].m_Kind == kind_dialoger)
		{
			Npc[nRet].m_CurrentLifeReplenish = 200000000;
		}
#endif
		if (pKSNpcInfo->szScript[0]) // 脚本不等空
		{
			INT zVal = CheckThisNpc(Npc[nRet].m_NpcSettingIdx, "NoScript"); // 检查是否在禁止列表中
			if (zVal)
			{ // 在禁止列表中，就不加载脚本
				return nRet;
			}
			char nMsg[256] = { 0 };
			/*
			if (pKSNpcInfo->szScript[0] == '.')
				g_StrCpyLen(Npc[nRet].ActionScript, &pKSNpcInfo->szScript[1], sizeof(Npc[nRet].ActionScript));
			else
				g_StrCpyLen(Npc[nRet].ActionScript, pKSNpcInfo->szScript, sizeof(Npc[nRet].ActionScript));
			*/
			if (pKSNpcInfo->szScript[0] == '.')
				g_StrCpyLen(nMsg, &pKSNpcInfo->szScript[1], sizeof(nMsg));
			else
				g_StrCpyLen(nMsg, pKSNpcInfo->szScript, sizeof(nMsg));
			// 保持小写，保证脚本对应关系

			_strlwr(nMsg);
			// g_StrLower(nMsg);
			Npc[nRet].SetstrInfo(STR_ACTION_SCRIPT, nMsg);
			Npc[nRet].m_ActionScriptID = g_FileName2Id(nMsg);
		}
		else
		{
			Npc[nRet].m_ActionScriptID = 0;
		}
	}
	else
	{ // 产生怪物失败
		/*if (SubWorld[nSubWorld].m_SubWorldID==53)
		{
			printf("--地图%d,刷怪:%s 成功---\n",SubWorld[nSubWorld].m_SubWorldID,Npc[nRet].Name);
		}*/
	}
	return nRet;
}

INT KNpcSet::Add(INT nNpcSettingIdxInfo, INT nSubWorld, INT nMpsX, INT nMpsY, BOOL bBarrier/* = FALSE*/, INT nBoss)
{
	INT nRegion, nMapX, nMapY, nOffX, nOffY;
	if (nSubWorld < 0 || nSubWorld >= MAX_SUBWORLD)
		return 0;

	POINT ptLocal;
	ptLocal.x = nMpsX;
	ptLocal.y = nMpsY;

	SubWorld[nSubWorld].GetFreeObjPos(ptLocal); // 获取周围没有障碍物的坐标(位置矫正)

	SubWorld[nSubWorld].Mps2Map(ptLocal.x, ptLocal.y, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	//SubWorld[nSubWorld].Mps2Map(nMpsX, nMpsY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion < 0)
		return 0;

	if (bBarrier)
	{
		if (SubWorld[nSubWorld].m_Region[nRegion].GetBarrierMin(nMapX, nMapY, nOffX, nOffY, TRUE) != Obstacle_NULL)
		{
			g_DebugLog("[AddNpc]Npc in Barrier %d,%d,%d", nSubWorld, nMpsX, nMpsY);
			return 0;
		}
	}
	INT nIndex = Add(nNpcSettingIdxInfo, nSubWorld, nRegion, nMapX, nMapY, nOffX, nOffY, nBoss);

	// 如果是黄金怪物
	if (nBoss > 0 && Npc[nIndex].m_Kind == kind_normal) // 特殊NPC  g_NpcMapDropRate
	{													  // if(!IsPlayer() && m_Kind!=kind_dialoger && m_Kind!=kind_partner)
		Npc[nIndex].m_cGold.SetGoldCurrentType(nBoss, nSubWorld);
		// Npc[nIndexID].m_cGold.RandChangeGold(0,0,nBoss,nSubWorld);  //同步怪物类型
	}

	return nIndex;
}

INT KNpcSet::Add(INT nNpcSettingIdxInfo, INT nSubWorld, INT nRegion, INT nMapX, INT nMapY, INT nOffX /* = 0 */, INT nOffY /* = 0 */, INT nBoss)
{
	INT i = FindFree();

	if (i == 0)
	{
		g_DebugLog("[Error!]AddNpc Error KNpcSet.cpp");
		return 0;
	}

#ifndef _SERVER
	Npc[i].m_sClientNpcID.m_dwRegionID = 0;
	Npc[i].m_sClientNpcID.m_nNo = -1;
	Npc[i].Remove();
#endif

	INT nNpcSettingIdx = (int16_t)HIWORD(nNpcSettingIdxInfo);// >> 7; //³ýÓÚ128
	INT nLevel = LOWORD(nNpcSettingIdxInfo);// & 0x7f; 

	Npc[i].m_Index = i;
	Npc[i].Load(nNpcSettingIdx, nLevel, nSubWorld, nBoss);
	Npc[i].m_SkillList.m_nNpcIndex = i;
	Npc[i].m_SubWorldIndex = nSubWorld;
	Npc[i].m_RegionIndex = nRegion;
	ZeroMemory(Npc[i].m_nNpcParam, sizeof(Npc[i].m_nNpcParam));
	Npc[i].m_bNpcRemoveDeath = FALSE;
	Npc[i].m_nNpcTimeout = 0;
	Npc[i].m_bNpcFollowFindPath = FALSE;
	Npc[i].m_uFindPathTime = 0;
	Npc[i].m_uFindPathMaxTime = 0;
	Npc[i].m_uLastFindPathTime = 0;
#ifndef _SERVER
	if (nRegion >= 0 && nRegion < 9)
		Npc[i].m_dwRegionID = SubWorld[nSubWorld].m_Region[nRegion].m_RegionID;
#endif
	Npc[i].m_MapX = nMapX;
	Npc[i].m_MapY = nMapY;
	Npc[i].m_OffX = nOffX;
	Npc[i].m_OffY = nOffY;

	SubWorld[nSubWorld].Map2Mps(nRegion, nMapX, nMapY, nOffX, nOffY, &Npc[i].m_OriginX, &Npc[i].m_OriginY);

#ifdef _SERVER
	SetID(i);
	Npc[i].m_cDeathCalcExp.Clear();
#endif
	// ÐÞ¸Ä¿ÉÓÃÓëÊ¹ÓÃ±í
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);

	if (nSubWorld >= 0 && nSubWorld < MAX_SUBWORLD)
	{
		SubWorld[nSubWorld].m_Region[nRegion].AddNpc(i);//m_WorldMessage.Send(GWM_NPC_ADD, nRegion, i);
		SubWorld[nSubWorld].m_Region[nRegion].AddNpcRef(nMapX, nMapY, obj_npc);
	}

#ifndef _SERVER
	Npc[i].m_dwRegionID = SubWorld[nSubWorld].m_Region[nRegion].m_RegionID;
#endif
	return i;
}


void KNpcSet::Remove(INT nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_NPC)
		return;
#ifdef _SERVER
	NPC_REMOVE_SYNC	NetCommand;

	NetCommand.ProtocolType = (BYTE)s2c_npcremove;
	NetCommand.ID = Npc[nIdx].m_dwID;
	NetCommand.Rv = TRUE;

	INT nSubWorld = Npc[nIdx].m_SubWorldIndex;
	INT nRegion = Npc[nIdx].m_RegionIndex;

	if (nSubWorld >= 0 && nSubWorld <= MAX_SUBWORLD && nRegion >= 0 && nRegion <= SubWorld[nSubWorld].m_nTotalRegion)
	{
		POINT	POff[8] =
		{
			{0, 32},
			{-16, 32},
			{-16, 0},
			{-16, -32},
			{0, -32},
			{16, -32},
			{16, 0},
			{16, 32},
		};
		INT nMaxCount = MAX_BROADCAST_COUNT;
		SubWorld[nSubWorld].m_Region[nRegion].BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, Npc[nIdx].m_MapX, Npc[nIdx].m_MapY);

		INT nConRegion;
		for (INT i = 0; i < 8; i++)
		{
			nConRegion = SubWorld[nSubWorld].m_Region[nRegion].m_nConnectRegion[i];
			if (nConRegion == -1)
				continue;
			SubWorld[nSubWorld].m_Region[nConRegion].BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, Npc[nIdx].m_MapX - POff[i].x, Npc[nIdx].m_MapY - POff[i].y);
		}
	}
#endif
	Npc[nIdx].ClearNpcState();
	Npc[nIdx].m_SkillList.Clear();
	Npc[nIdx].Remove();

	m_FreeIdx.Insert(nIdx);
	m_UseIdx.Remove(nIdx);
}

void KNpcSet::RemoveAll()
{
	INT nIdx = m_UseIdx.GetNext(0);
	INT nIdx1 = 0;
	while (nIdx)
	{
		nIdx1 = m_UseIdx.GetNext(nIdx);
		Npc[nIdx].ClearNpcState();
		Npc[nIdx].m_SkillList.Clear();
		Npc[nIdx].Remove();
		m_FreeIdx.Insert(nIdx);
		m_UseIdx.Remove(nIdx);
		nIdx = nIdx1;
	}
}


#ifndef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º´ÓnpcÊý×éÖÐÑ°ÕÒÊôÓÚÄ³¸öregionµÄ client npc £¬Ìí¼Ó½øÈ¥
//---------------------------------------------------------------------------
void	KNpcSet::InsertNpcToRegion(INT nRegionIdx)
{
	if (nRegionIdx < 0 || nRegionIdx >= MAX_REGION)
		return;
	INT nIdx = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;

		if (Npc[nIdx].m_sClientNpcID.m_dwRegionID > 0 && Npc[nIdx].m_dwRegionID == (DWORD)SubWorld[0].m_Region[nRegionIdx].m_RegionID)
		{
			SubWorld[0].m_Region[nRegionIdx].AddNpc(nIdx);
			Npc[nIdx].m_RegionIndex = nRegionIdx;
			Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nRegionIdx].m_RegionID;
			Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
			Npc[nIdx].SendCommand(do_stand);
		}
	}
}
#endif


void KNpcSet::SetID(INT m_nIndex)
{
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;

	Npc[m_nIndex].m_dwID = m_dwIDCreator;
	m_dwIDCreator++;
}

INT KNpcSet::GetDistance(INT nIdx1, INT nIdx2)
{
	INT nRet = 0;
	if (Npc[nIdx1].m_SubWorldIndex != Npc[nIdx2].m_SubWorldIndex)
		return -1;

	if (Npc[nIdx1].m_RegionIndex == Npc[nIdx2].m_RegionIndex)
	{
		INT XOff = (Npc[nIdx1].m_MapX - Npc[nIdx2].m_MapX) * SubWorld[Npc[nIdx1].m_SubWorldIndex].m_nCellWidth;
		XOff += (Npc[nIdx1].m_OffX - Npc[nIdx2].m_OffX) >> 10;

		INT YOff = (Npc[nIdx1].m_MapY - Npc[nIdx2].m_MapY) * SubWorld[Npc[nIdx1].m_SubWorldIndex].m_nCellHeight;
		YOff += (Npc[nIdx1].m_OffY - Npc[nIdx2].m_OffY) >> 10;

		nRet = (INT)sqrt(double(XOff * XOff) + YOff * YOff);
	}
	else
	{
		INT X1, Y1;
		SubWorld[Npc[nIdx1].m_SubWorldIndex].NewMap2Mps(Npc[nIdx1].m_RegionIndex,
			Npc[nIdx1].m_MapX,
			Npc[nIdx1].m_MapY,
			Npc[nIdx1].m_OffX,
			Npc[nIdx1].m_OffY,
			&X1,
			&Y1);
		INT X2, Y2;
		SubWorld[Npc[nIdx2].m_SubWorldIndex].NewMap2Mps(Npc[nIdx2].m_RegionIndex,
			Npc[nIdx2].m_MapX,
			Npc[nIdx2].m_MapY,
			Npc[nIdx2].m_OffX,
			Npc[nIdx2].m_OffY,
			&X2,
			&Y2);

		nRet = (INT)sqrt(double((X2 - X1)) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
	}
	return nRet;
}

INT		KNpcSet::GetDistanceSquare(INT nIdx1, INT nIdx2)
{
	INT nRet = 0;
	if (Npc[nIdx1].m_SubWorldIndex != Npc[nIdx2].m_SubWorldIndex)
		return -1;

	if (Npc[nIdx1].m_RegionIndex == Npc[nIdx2].m_RegionIndex)
	{
		INT XOff = (Npc[nIdx1].m_MapX - Npc[nIdx2].m_MapX) * SubWorld[Npc[nIdx1].m_SubWorldIndex].m_nCellWidth;
		XOff += (Npc[nIdx1].m_OffX - Npc[nIdx2].m_OffX) >> 10;

		INT YOff = (Npc[nIdx1].m_MapY - Npc[nIdx2].m_MapY) * SubWorld[Npc[nIdx1].m_SubWorldIndex].m_nCellHeight;
		YOff += (Npc[nIdx1].m_OffY - Npc[nIdx2].m_OffY) >> 10;

		nRet = (INT)(XOff * XOff + YOff * YOff);
	}
	else
	{
		INT X1, Y1;
		SubWorld[Npc[nIdx1].m_SubWorldIndex].NewMap2Mps(Npc[nIdx1].m_RegionIndex,
			Npc[nIdx1].m_MapX,
			Npc[nIdx1].m_MapY,
			Npc[nIdx1].m_OffX,
			Npc[nIdx1].m_OffY,
			&X1,
			&Y1);
		INT X2, Y2;
		SubWorld[Npc[nIdx2].m_SubWorldIndex].NewMap2Mps(Npc[nIdx2].m_RegionIndex,
			Npc[nIdx2].m_MapX,
			Npc[nIdx2].m_MapY,
			Npc[nIdx2].m_OffX,
			Npc[nIdx2].m_OffY,
			&X2,
			&Y2);

		nRet = (INT)((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
	}
	return nRet;
}

INT		KNpcSet::GetNextIdx(INT nIdx)
{
	if (nIdx < 0 || nIdx >= MAX_NPC)
		return 0;
	return m_UseIdx.GetNext(nIdx);
}

#ifdef _SERVER
BOOL KNpcSet::SyncNpc(DWORD dwID, INT nClientIdx)
{
	INT		nFindIndex;

	nFindIndex = SearchID(dwID);
	if (nFindIndex <= 0)
	{
		NPC_REQUEST_FAIL	RequestFail;

		RequestFail.ProtocolType = s2c_requestnpcfail;
		RequestFail.ID = dwID;

		if (g_pServer)
			g_pServer->PackDataToClient(nClientIdx, &RequestFail, sizeof(NPC_REQUEST_FAIL));
		return FALSE;
	}
	Npc[nFindIndex].SendSyncData(nClientIdx);
	return TRUE;
}

BOOL KNpcSet::ExecuteScript(INT nNpcIndex, char* szScriptName, char* szFuncName, INT nParam)
{
	DWORD dwScriptId = g_FileName2Id(szScriptName);
	return ExecuteScript(nNpcIndex, dwScriptId, szFuncName, nParam);
}

BOOL KNpcSet::ExecuteScript(INT nNpcIndex, DWORD dwScriptId, char* szFuncName, INT nParam)
{
	if (nNpcIndex > 0 && nNpcIndex < MAX_NPC)
	{
		try
		{
			KLuaScript* pScript = (KLuaScript*)g_GetScript(dwScriptId);
			if (pScript)
			{
				Lua_PushNumber(pScript->m_LuaState, Npc[nNpcIndex].m_SubWorldIndex);
				pScript->SetGlobalName(SCRIPT_NPCINDEX); //SCRIPT_SUBWORLDINDEX

				INT nTopIndex = 0;

				pScript->SafeCallBegin(&nTopIndex);
				pScript->CallFunction(szFuncName, 0, "d", nParam);
				pScript->SafeCallEnd(nTopIndex);

			}
			return TRUE;
		}
		catch (...)
		{
			printf("-->Error Execute: %u [%s]\n", dwScriptId, szFuncName);
			return FALSE;
		}
	}
	return FALSE;
}
#endif

#ifndef _SERVER
void KNpcSet::CheckBalance()
{
	INT nIdx;
	nIdx = m_UseIdx.GetNext(0);
	while (nIdx)
	{
		INT nTmpIdx = m_UseIdx.GetNext(nIdx);
		if (SubWorld[0].m_dwCurrentTime - Npc[nIdx].m_SyncSignal > 1000)
		{
			if (nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{
				if (Npc[nIdx].m_RegionIndex >= 0)
				{
					SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
					SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecNpcRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
				}
				Remove(nIdx);
			}
		}
		nIdx = nTmpIdx;
	}
	nIdx = m_RequestUseIdx.GetNext(0);
	while (nIdx)
	{
		INT nTmpIdx = m_RequestUseIdx.GetNext(nIdx);
		if (SubWorld[0].m_dwCurrentTime - m_RequestNpc[nIdx].dwRequestTime > 100)
		{
			DWORD	dwID = m_RequestNpc[nIdx].dwRequestId;
			m_RequestNpc[nIdx].dwRequestId = 0;
			m_RequestNpc[nIdx].dwRequestTime = 0;

			m_RequestUseIdx.Remove(nIdx);
			m_RequestFreeIdx.Insert(nIdx);
			//			g_DebugLog("[Request]Remove %d from %d on %d timeout", dwID, nIdx, SubWorld[0].m_dwCurrentTime);
		}
		nIdx = nTmpIdx;
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñµÃÖÜÎ§Íæ¼ÒÁÐ±í£¬ÓÃÓÚ½çÃæ£¬¶ÓÎéÑûÇëÁÐ±í
//-------------------------------------------------------------------------
INT		KNpcSet::GetAroundPlayerForTeamInvite(KUiPlayerItem* pList, INT nCount)
{
	INT nCamp = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp;
	INT nNum = 0, i;

	if (nCount == 0)
	{
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (nIdx == 0)
				break;
			if (Npc[nIdx].m_Kind != kind_player)
				continue;
			if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
				continue;
			if (Npc[nIdx].m_Camp != camp_begin && nCamp == camp_begin)
				continue;
			if (Npc[nIdx].m_RegionIndex < 0)
				continue;
			if (Npc[nIdx].Name[0] == 0)
				continue;
			for (i = 0; i < MAX_TEAM_MEMBER; i++)
			{
				if ((DWORD)g_Team[0].m_nMember[i] == Npc[nIdx].m_dwID)
					break;
			}
			if (i < MAX_TEAM_MEMBER)
				continue;
			if ((DWORD)g_Team[0].m_nCaptain == Npc[nIdx].m_dwID)
				continue;
			nNum++;
		}

		return nNum;
	}

	if (!pList)
		return 0;

	INT nIdx = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;
		if (Npc[nIdx].m_Kind != kind_player)
			continue;
		if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
			continue;
		if (Npc[nIdx].m_Camp != camp_begin && nCamp == camp_begin)
			continue;
		if (Npc[nIdx].m_RegionIndex < 0)
			continue;
		if (Npc[nIdx].Name[0] == 0)
			continue;
		for (i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if ((DWORD)g_Team[0].m_nMember[i] == Npc[nIdx].m_dwID)
				break;
		}
		if (i < MAX_TEAM_MEMBER)
			continue;
		if ((DWORD)g_Team[0].m_nCaptain == Npc[nIdx].m_dwID)
			continue;
		pList[nNum].nIndex = nIdx;
		pList[nNum].uId = Npc[nIdx].m_dwID;
		strcpy(pList[nNum].Name, Npc[nIdx].Name);
		nNum++;
	}

	return nNum;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñµÃÖÜÎ§Íæ¼ÒÁÐ±í(ÓÃÓÚÁÐ±í)
//-------------------------------------------------------------------------
INT		KNpcSet::GetAroundPlayer(KUiPlayerItem* pList, INT nCount)
{
	INT nNum = 0;

	if (nCount <= 0)
	{
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (nIdx == 0)
				break;
			if (Npc[nIdx].m_Kind != kind_player ||
				nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex ||
				Npc[nIdx].m_RegionIndex < 0)
			{
				continue;
			}
			nNum++;
		}

		return nNum;
	}

	if (!pList)
		return 0;

	INT nIdx = 0;
	while (nNum < nCount)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;
		if (Npc[nIdx].m_Kind != kind_player ||
			nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex ||
			Npc[nIdx].m_RegionIndex < 0)
		{
			continue;
		}
		pList[nNum].nIndex = nIdx;
		pList[nNum].uId = Npc[nIdx].m_dwID;
		strcpy(pList[nNum].Name, Npc[nIdx].Name);
		pList[nNum].nData = Npc[nIdx].GetMenuState();
		nNum++;
	}

	return nNum;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÊÇ·ñÈ«²¿ÏÔÊ¾Íæ¼ÒµÄÃû×Ö
//			bFlag ==	TRUE ÏÔÊ¾£¬bFlag == FALSE ²»ÏÔÊ¾ zroc add
//-------------------------------------------------------------------------
void	KNpcSet::SetShowNameFlag(BOOL bFlag)
{
	if (bFlag)
		m_nShowPateFlag |= PATE_NAME;
	else
		m_nShowPateFlag &= ~PATE_NAME;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÅÐ¶ÏÊÇ·ñÈ«²¿ÏÔÊ¾Íæ¼ÒµÄÃû×Ö  ·µ»ØÖµ TRUE ÏÔÊ¾£¬FALSE ²»ÏÔÊ¾
//-------------------------------------------------------------------------
BOOL	KNpcSet::CheckShowName()
{
	return m_nShowPateFlag & PATE_NAME;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÊÇ·ñÈ«²¿ÏÔÊ¾Íæ¼ÒµÄÑª
//			bFlag ==	TRUE ÏÔÊ¾£¬bFlag == FALSE ²»ÏÔÊ¾ zroc add
//-------------------------------------------------------------------------
void	KNpcSet::SetShowLifeFlag(BOOL bFlag)
{
	if (bFlag)
		m_nShowPateFlag |= PATE_LIFE;
	else
		m_nShowPateFlag &= ~PATE_LIFE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÅÐ¶ÏÊÇ·ñÈ«²¿ÏÔÊ¾Íæ¼ÒµÄÑª  ·µ»ØÖµ TRUE ÏÔÊ¾£¬FALSE ²»ÏÔÊ¾
//-------------------------------------------------------------------------
BOOL	KNpcSet::CheckShowLife()
{
	return m_nShowPateFlag & PATE_LIFE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÊÇ·ñÈ«²¿ÏÔÊ¾Íæ¼ÒµÄÄÚÁ¦
//			bFlag ==	TRUE ÏÔÊ¾£¬bFlag == FALSE ²»ÏÔÊ¾ zroc add
//-------------------------------------------------------------------------
void	KNpcSet::SetShowObjFlag(BOOL bFlag)
{
	if (bFlag)
		m_nShowPateFlag |= PATE_OBJ;
	else
		m_nShowPateFlag &= ~PATE_OBJ;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÅÐ¶ÏÊÇ·ñÈ«²¿ÏÔÊ¾Íæ¼ÒµÄÄÚÁ¦  ·µ»ØÖµ TRUE ÏÔÊ¾£¬FALSE ²»ÏÔÊ¾
//-------------------------------------------------------------------------
BOOL	KNpcSet::CheckShowObj()
{
	return m_nShowPateFlag & PATE_OBJ;
}
#endif

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º°ÑËùÓÐnpcµÄ bActivateFlag ÉèÎª FALSE
//		(Ã¿´ÎÓÎÏ·Ñ­»·´¦ÀíËùÓÐnpcµÄactivateÖ®Ç°×öÕâ¸ö´¦Àí)
//-------------------------------------------------------------------------
void	KNpcSet::ClearActivateFlagOfAllNpc()
{
	INT nIdx = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;
		Npc[nIdx].m_bActivateFlag = FALSE;
	}
}

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñµÃÖÜÎ§Í¬ÕóÓªµÄÒÑ¿ª·Å¶ÓÎé¶Ó³¤ÁÐ±í ²»Í¬ÕóÓªÏÖÔÚ¿ÉÒÔ×é¶Ó
//-------------------------------------------------------------------------
void	KNpcSet::GetAroundOpenCaptain(INT nCamp)
{
	INT		nIdx, nNum, nNo;

	nIdx = 0;
	nNum = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (nIdx == 0)
			break;
		if (Npc[nIdx].m_Kind != kind_player)
			continue;
		if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
			continue;
		// ²»Í¬ÕóÓªÏÖÔÚ¿ÉÒÔ×é¶Ó£¬ÀÏÊÖ²»ÄÜ¼ÓÈëÐÂÈË¶ÓÎé£¬ÐÂÈË¿ÉÒÔ¼ÓÈëÀÏÊÖ¶ÓÎé
		if (Npc[nIdx].m_Camp == camp_begin && nCamp != camp_begin)
			continue;
		//		if (Npc[nIdx].m_Camp != nCamp)
		//			continue;
		if (Npc[nIdx].m_RegionIndex < 0)
			continue;
		if (Npc[nIdx].GetMenuState() == PLAYER_MENU_STATE_TEAMOPEN)
			nNum++;
	}

	if (nNum > 0)
	{
		KUiTeamItem* const pTeamList = new KUiTeamItem[nNum];
		nIdx = 0;
		nNo = 0;
		while (1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (nIdx == 0)
				break;
			if (Npc[nIdx].m_Kind != kind_player)
				continue;
			if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
				continue;
			// ²»Í¬ÕóÓªÏÖÔÚ¿ÉÒÔ×é¶Ó£¬ÀÏÊÖ²»ÄÜ¼ÓÈëÐÂÈË¶ÓÎé£¬ÐÂÈË¿ÉÒÔ¼ÓÈëÀÏÊÖ¶ÓÎé
			if (Npc[nIdx].m_Camp == camp_begin && nCamp != camp_begin)
				continue;
			//			if (Npc[nIdx].m_Camp != nCamp)
			//				continue;
			if (Npc[nIdx].m_RegionIndex < 0)
				continue;
			if (Npc[nIdx].GetMenuState() == PLAYER_MENU_STATE_TEAMOPEN)
			{
				pTeamList[nNo].Leader.nIndex = nIdx;
				pTeamList[nNo].Leader.uId = Npc[nIdx].m_dwID;
				strcpy(pTeamList[nNo].Leader.Name, Npc[nIdx].Name);
				nNo++;
				if (nNo >= nNum)
					break;
			}
		}
		CoreDataChanged(GDCNI_TEAM_NEARBY_LIST, (UINT)pTeamList, nNo);
		delete[]pTeamList;
	}
}
#endif

#ifndef _SERVER	// ÓÃÓÚ¿Í»§¶Ë
INT	KNpcSet::SearchNpcAt(INT nX, INT nY, INT nRelation, INT nRange)
{
	INT nIdx;
	INT	nMin = nRange;
	INT nMinIdx = 0;
	INT	nLength = 0;
	INT nSrcX[2];
	INT	nSrcY[2];

	nSrcX[0] = nX;
	nSrcY[0] = nY;
	g_ScenePlace.ViewPortCoordToSpaceCoord(nSrcX[0], nSrcY[0], 0);

	nSrcX[1] = nX;
	nSrcY[1] = nY;
	g_ScenePlace.ViewPortCoordToSpaceCoord(nSrcX[1], nSrcY[1], 120);

	INT nDx = nSrcX[0] - nSrcX[1];
	INT nDy = nSrcY[0] - nSrcY[1];

	nIdx = 0;
	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);

		if (nIdx == 0)
			break;

		if (Npc[nIdx].m_RegionIndex < 0)
			continue;

		if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
			continue;

		if (Npc[nIdx].m_bClientOnly)
			continue;

		if (!(GetRelation(Player[CLIENT_PLAYER_INDEX].m_nIndex, nIdx) & nRelation))
			continue;

		INT x, y;
		SubWorld[0].Map2Mps(Npc[nIdx].m_RegionIndex, Npc[nIdx].m_MapX, Npc[nIdx].m_MapY,
			Npc[nIdx].m_OffX, Npc[nIdx].m_OffY, &x, &y);

		if (nSrcY[0] > y)
			continue;

		if (nSrcY[0] < y - 120)
			continue;

		nLength = abs(nDx * (nSrcY[0] - y) / nDy + nSrcX[0] - x);
		if (nLength < nMin)
		{
			nMin = nLength;
			nMinIdx = nIdx;
		}
	}

	return nMinIdx;
}
#endif

#ifndef _SERVER
BOOL KNpcSet::IsNpcRequestExist(DWORD dwID)
{
	return (GetRequestIndex(dwID) > 0);
}

void KNpcSet::InsertNpcRequest(DWORD dwID)
{
	if (IsNpcRequestExist(dwID))
	{
		return;
	}

	INT nIndex = m_RequestFreeIdx.GetNext(0);
	if (!nIndex)
		return;

	m_RequestNpc[nIndex].dwRequestId = dwID;
	m_RequestNpc[nIndex].dwRequestTime = SubWorld[0].m_dwCurrentTime;
	m_RequestFreeIdx.Remove(nIndex);
	m_RequestUseIdx.Insert(nIndex);
	//	g_DebugLog("[Request]Insert %d at %d on %d", dwID, nIndex, SubWorld[0].m_dwCurrentTime);
}

void KNpcSet::RemoveNpcRequest(DWORD dwID)
{
	if (!IsNpcRequestExist(dwID))
	{
		return;
	}
	INT nIndex = GetRequestIndex(dwID);

	// because _ASSERT(IsNpcRequestExist()); so nIndex > 0;
	m_RequestNpc[nIndex].dwRequestId = 0;
	m_RequestNpc[nIndex].dwRequestTime = 0;

	m_RequestUseIdx.Remove(nIndex);
	m_RequestFreeIdx.Insert(nIndex);
	//	g_DebugLog("[Request]Remove %d from %d on %d", dwID, nIndex, SubWorld[0].m_dwCurrentTime);
}

INT KNpcSet::GetRequestIndex(DWORD dwID)
{
	INT nIndex = m_RequestUseIdx.GetNext(0);

	while (nIndex)
	{
		if (m_RequestNpc[nIndex].dwRequestId == dwID)
		{
			return nIndex;
		}
		nIndex = m_RequestUseIdx.GetNext(nIndex);
	}
	return 0;
}
#endif

NPC_RELATION KNpcSet::GetRelation(INT nId1, INT nId2)
{
	// Í¬Ò»¸öÈË
	if (nId1 == nId2)
		return relation_self;

#ifndef _SERVER
	if (Npc[nId1].m_bClientOnly || Npc[nId2].m_bClientOnly)
		return relation_none;
#endif

	_ASSERT(
		((Npc[nId1].m_Kind >= 0) && (Npc[nId1].m_Kind < kind_num)) &&
		((Npc[nId2].m_Kind >= 0) && (Npc[nId2].m_Kind < kind_num)) &&
		((Npc[nId1].m_CurrentCamp >= 0) && (Npc[nId1].m_CurrentCamp < camp_num)) &&
		((Npc[nId2].m_CurrentCamp >= 0) && (Npc[nId2].m_CurrentCamp < camp_num))
	);

#ifndef _SERVER
	if (Player[CLIENT_PLAYER_INDEX].m_nIndex != nId1 && Player[CLIENT_PLAYER_INDEX].m_nIndex != nId2)
	{
		if (!Npc[nId1].IsAlive() || !Npc[nId2].IsAlive())
			return relation_none;

		NPC_RELATION nRelation = (NPC_RELATION)m_RelationTable
			[Npc[nId1].m_Kind]
			[Npc[nId2].m_Kind]
			[Npc[nId1].m_CurrentCamp]
			[Npc[nId2].m_CurrentCamp];
		if (nRelation == relation_enemy)
		{
			if (Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_player &&
				(!Npc[nId1].m_nPKFlag ||
					!Npc[nId2].m_nPKFlag ||
					Npc[nId1].m_FightMode == enumFightNone ||
					Npc[nId2].m_FightMode == enumFightNone)
				)
				return relation_none;
			if ((Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_normal && Npc[nId1].m_FightMode == enumFightNone) ||
				(Npc[nId1].m_Kind == kind_normal &&
					Npc[nId2].m_Kind == kind_player && Npc[nId2].m_FightMode == enumFightNone)
				)
				return relation_none;
		}
		return nRelation;
	}
	else if (Player[CLIENT_PLAYER_INDEX].m_nIndex == nId1)
	{
		if (/*!Npc[nId1].IsAlive() || */!Npc[nId2].IsAlive())
			return relation_none;

		if (Player[CLIENT_PLAYER_INDEX].m_cPK.GetExercisePKAim() == Npc[nId2].m_dwID)
			return relation_enemy;

		if (Player[CLIENT_PLAYER_INDEX].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING &&
			Player[CLIENT_PLAYER_INDEX].m_cPK.GetEnmityPKAimNpcID() == Npc[nId2].m_dwID
			)
			return relation_enemy;

		if ((Npc[nId2].m_Kind == kind_player && Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState() == enumPKMurder) ||
			(Npc[nId2].m_Kind == kind_player &&
				Npc[nId2].m_nPKFlag == enumPKMurder &&
				Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState())
			)
		{
			if (Npc[nId1].m_FightMode && Npc[nId2].m_FightMode &&
				Npc[nId2].m_CurrentCamp != camp_begin &&
				Npc[nId1].m_CurrentCamp != camp_begin &&
				Npc[nId2].m_CurrentCamp != Npc[nId1].m_CurrentCamp
				)
			{
				return relation_enemy;
			}
		}

		if (Npc[nId1].m_nTeamServerID >= 0 &&
			Npc[nId2].m_nTeamServerID >= 0 &&
			Npc[nId1].m_nTeamServerID == Npc[nId2].m_nTeamServerID
			)
			return relation_ally;

		if (Npc[nId2].m_Kind == kind_player &&
			Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState() == enumPKTongWar &&
			Npc[nId2].m_nPKFlag == enumPKTongWar)
		{
			if (Npc[nId1].m_FightMode && Npc[nId2].m_FightMode)
			{
				if (Npc[nId1].m_dwTongNameID == Npc[nId2].m_dwTongNameID)
				{
					if (Npc[nId1].m_CurrentCamp != Npc[nId2].m_CurrentCamp)
						return relation_none;
					else
						return relation_ally;
				}
				else
					return relation_enemy;

			}
			else
			{
				if (Npc[nId1].m_dwTongNameID == Npc[nId2].m_dwTongNameID)
					return relation_ally;
				else
					return relation_none;
			}
		}

		NPC_RELATION nRelation = (NPC_RELATION)m_RelationTable
			[Npc[nId1].m_Kind]
			[Npc[nId2].m_Kind]
			[Npc[nId1].m_CurrentCamp]
			[Npc[nId2].m_CurrentCamp];
		if (nRelation == relation_enemy)
		{
			if (Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_player &&
				(!Npc[nId1].m_nPKFlag ||
					!Npc[nId2].m_nPKFlag ||
					Npc[nId1].m_FightMode == enumFightNone ||
					Npc[nId2].m_FightMode == enumFightNone)
				)
				return relation_none;
			if ((Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_normal && Npc[nId1].m_FightMode == enumFightNone) ||
				(Npc[nId1].m_Kind == kind_normal &&
					Npc[nId2].m_Kind == kind_player && Npc[nId2].m_FightMode == enumFightNone)
				)
				return relation_none;
		}
		/*else if(nRelation == relation_ally)
		{
			if (Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_player &&
				(Npc[nId1].m_FightMode != Npc[nId2].m_FightMode)
				)
				return relation_none;
		}*/
		return nRelation;
	}
	else
	{
		if (!Npc[nId1].IsAlive()/* || !Npc[nId2].IsAlive()*/)
			return relation_none;

		if (Player[CLIENT_PLAYER_INDEX].m_cPK.GetExercisePKAim() == Npc[nId1].m_dwID)
			return relation_enemy;

		if (Player[CLIENT_PLAYER_INDEX].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING &&
			Player[CLIENT_PLAYER_INDEX].m_cPK.GetEnmityPKAimNpcID() == Npc[nId1].m_dwID)
			return relation_enemy;

		if ((Npc[nId1].m_Kind == kind_player && Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState() == enumPKMurder) ||
			(Npc[nId1].m_Kind == kind_player &&
				Npc[nId1].m_nPKFlag == enumPKMurder &&
				!Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState())
			)
		{
			if (Npc[nId1].m_FightMode && Npc[nId2].m_FightMode &&
				(Npc[nId1].m_CurrentCamp != camp_begin &&
					Npc[nId2].m_CurrentCamp != camp_begin &&
					Npc[nId1].m_CurrentCamp != Npc[nId2].m_CurrentCamp)
				)
			{
				return relation_enemy;
			}
		}

		if (Npc[nId1].m_nTeamServerID >= 0 &&
			Npc[nId2].m_nTeamServerID >= 0 &&
			Npc[nId1].m_nTeamServerID == Npc[nId2].m_nTeamServerID
			)
			return relation_ally;

		if (Npc[nId1].m_Kind == kind_player &&
			Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState() == enumPKTongWar &&
			Npc[nId1].m_nPKFlag == enumPKTongWar
			)
		{
			if (Npc[nId1].m_FightMode && Npc[nId2].m_FightMode)
			{
				if (Npc[nId2].m_dwTongNameID == Npc[nId1].m_dwTongNameID)
				{
					if (Npc[nId1].m_CurrentCamp != Npc[nId2].m_CurrentCamp)
						return relation_none;
					else
						return relation_ally;
				}
				else
					return relation_enemy;
			}
			else
			{
				if (Npc[nId2].m_dwTongNameID == Npc[nId1].m_dwTongNameID)
					return relation_ally;
				else
					return relation_none;
			}
		}


		NPC_RELATION nRelation = (NPC_RELATION)m_RelationTable
			[Npc[nId1].m_Kind]
			[Npc[nId2].m_Kind]
			[Npc[nId1].m_CurrentCamp]
			[Npc[nId2].m_CurrentCamp];

		if (nRelation == relation_enemy)
		{
			if (Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_player &&
				(!Npc[nId1].m_nPKFlag ||
					!Npc[nId2].m_nPKFlag ||
					Npc[nId1].m_FightMode == enumFightNone ||
					Npc[nId2].m_FightMode == enumFightNone)
				)
				return relation_none;
			if ((Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_normal && Npc[nId1].m_FightMode == enumFightNone) ||
				(Npc[nId1].m_Kind == kind_normal &&
					Npc[nId2].m_Kind == kind_player && Npc[nId2].m_FightMode == enumFightNone)
				)
				return relation_none;
		}
		/*else if(nRelation == relation_ally)
		{
			if (Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_player &&
				(Npc[nId1].m_FightMode != Npc[nId2].m_FightMode)
				)
				return relation_none;
		}*/
		return nRelation;
	}
#endif

#ifdef _SERVER
	if (Npc[nId1].m_Kind != kind_player ||
		Npc[nId2].m_Kind != kind_player
		)
	{
		return (NPC_RELATION)m_RelationTable
			[Npc[nId1].m_Kind]
			[Npc[nId2].m_Kind]
			[Npc[nId1].m_CurrentCamp]
			[Npc[nId2].m_CurrentCamp];

	}
	else
	{
		//if (!Npc[nId1].IsAlive() || !Npc[nId2].IsAlive())
		//	return relation_none;

		if (Player[Npc[nId1].m_nPlayerIdx].m_cPK.GetExercisePKAim() == Npc[nId2].m_nPlayerIdx)
			return relation_enemy;

		if (Player[Npc[nId1].m_nPlayerIdx].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING &&
			Player[Npc[nId1].m_nPlayerIdx].m_cPK.GetEnmityPKAim() == Npc[nId2].m_nPlayerIdx
			)
			return relation_enemy;

		if ((Npc[nId2].m_Kind == kind_player && Player[Npc[nId1].m_nPlayerIdx].m_cPK.GetNormalPKState() == enumPKMurder) ||
			(Npc[nId2].m_Kind == kind_player &&
				Player[Npc[nId1].m_nPlayerIdx].m_cPK.GetNormalPKState() == enumPKMurder &&
				!Player[Npc[nId1].m_nPlayerIdx].m_cPK.GetNormalPKState())
			)
		{
			if (Npc[nId1].m_FightMode && Npc[nId2].m_FightMode &&
				Npc[nId1].m_CurrentCamp != camp_begin &&
				Npc[nId2].m_CurrentCamp != camp_begin &&
				Npc[nId1].m_CurrentCamp != Npc[nId2].m_CurrentCamp
				)
			{
				return relation_enemy;
			}
		}

		if (Player[Npc[nId1].m_nPlayerIdx].m_cTeam.m_nID >= 0 &&
			Player[Npc[nId2].m_nPlayerIdx].m_cTeam.m_nID >= 0 &&
			Player[Npc[nId1].m_nPlayerIdx].m_cTeam.m_nID == Player[Npc[nId2].m_nPlayerIdx].m_cTeam.m_nID
			)
			return relation_ally;

		if (Npc[nId2].m_Kind == kind_player &&
			Player[Npc[nId1].m_nPlayerIdx].m_cPK.GetNormalPKState() == enumPKTongWar &&
			Npc[nId2].m_nPKFlag == enumPKTongWar
			)
		{
			if (Npc[nId1].m_FightMode && Npc[nId2].m_FightMode)
			{
				if (Player[nId1].m_cTong.m_dwTongNameID == Player[nId2].m_cTong.m_dwTongNameID)
				{
					if (Npc[nId1].m_CurrentCamp != Npc[nId2].m_CurrentCamp)
						return relation_none;
					else
						return relation_ally;
				}
				else
					return relation_enemy;
			}
			else
			{
				if (Player[nId1].m_cTong.m_dwTongNameID == Player[nId2].m_cTong.m_dwTongNameID)
					return relation_ally;
				else
					return relation_none;
			}
		}
		NPC_RELATION nRelation = (NPC_RELATION)m_RelationTable
			[Npc[nId1].m_Kind]
			[Npc[nId2].m_Kind]
			[Npc[nId1].m_CurrentCamp]
			[Npc[nId2].m_CurrentCamp];

		if (nRelation == relation_enemy)
		{
			if (!Player[Npc[nId1].m_nPlayerIdx].m_cPK.GetNormalPKState() ||
				!Player[Npc[nId2].m_nPlayerIdx].m_cPK.GetNormalPKState() ||
				Npc[nId1].m_FightMode == enumFightNone ||
				Npc[nId2].m_FightMode == enumFightNone
				)
				return relation_none;
			if ((Npc[nId1].m_Kind == kind_player &&
				Npc[nId2].m_Kind == kind_normal && Npc[nId1].m_FightMode == enumFightNone) ||
				(Npc[nId1].m_Kind == kind_normal &&
					Npc[nId2].m_Kind == kind_player && Npc[nId2].m_FightMode == enumFightNone)
				)
				return relation_none;
		}
		/*else if(nRelation == relation_ally)
		{
			if (Npc[nId1].m_FightMode != Npc[nId2].m_FightMode)
				return relation_none;
		}*/
		return nRelation;
	}
#endif
}

//------------------------ class KInstantSpecial start -------------------------
#ifndef _SERVER
KInstantSpecial::KInstantSpecial()
{
	INT		i;
	this->m_nLoadFlag = FALSE;
	for (i = 0; i < MAX_INSTANT_STATE; i++)
		this->m_szSprName[i][0] = 0;
	for (i = 0; i < MAX_INSTANT_SOUND; i++)
		this->m_szSoundName[i][0] = 0;

	m_pSoundNode = NULL;
	m_pWave = NULL;
}

void	KInstantSpecial::LoadSprName()
{
	INT		i;
	for (i = 0; i < MAX_INSTANT_STATE; i++)
		m_szSprName[i][0] = 0;

	KTabFile	cSprName;
	//	g_SetFilePath("\\");
	if (!cSprName.Load(PLAYER_INSTANT_SPECIAL_FILE))
		return;
	for (i = 0; i < MAX_INSTANT_STATE; i++)
		cSprName.GetString(i + 2, 3, "", m_szSprName[i], sizeof(m_szSprName[i]));
}

void	KInstantSpecial::LoadSoundName()
{
	INT		i;
	for (i = 0; i < MAX_INSTANT_SOUND; i++)
		m_szSoundName[i][0] = 0;

	KIniFile	cSoundName;
	char		szTemp[32];
	//	g_SetFilePath("\\");
	if (!cSoundName.Load(defINSTANT_SOUND_FILE))
		return;
	for (i = 0; i < MAX_INSTANT_SOUND; i++)
	{
		sprintf(szTemp, "%d", i);
		cSoundName.GetString("Game", szTemp, "", this->m_szSoundName[i], sizeof(m_szSoundName[i]));
	}
}

void	KInstantSpecial::GetSprName(INT nNo, char* lpszName, INT nLength)
{
	if (!lpszName || nLength <= 0)
		return;
	if (nNo < 0 || nNo >= MAX_INSTANT_STATE)
	{
		lpszName[0] = 0;
		return;
	}
	if (this->m_nLoadFlag == FALSE)
	{
		this->LoadSprName();
		this->LoadSoundName();
		m_nLoadFlag = TRUE;
	}

	if (strlen(this->m_szSprName[nNo]) < (DWORD)nLength)
		strcpy(lpszName, m_szSprName[nNo]);
	else
		lpszName[0] = 0;
}

void	KInstantSpecial::PlaySound(INT nNo)
{
	if (this->m_nLoadFlag == FALSE)
	{
		this->LoadSprName();
		this->LoadSoundName();
		m_nLoadFlag = TRUE;
	}
	if (nNo < 0 || nNo >= MAX_INSTANT_SOUND)
		return;
	if (!m_szSoundName[nNo][0])
		return;

	m_pSoundNode = (KCacheNode*)g_SoundCache.GetNode(m_szSoundName[nNo], (KCacheNode*)m_pSoundNode);
	m_pWave = (KWavSound*)m_pSoundNode->m_lpData;
	if (m_pWave)
	{
		if (m_pWave->IsPlaying())
			return;
		m_pWave->Play(0, -10000 + Option.GetSndVolume() * 100, 0);
	}
}

#endif
//------------------------- class KInstantSpecial end --------------------------

INT KNpcSet::AddServerNpcA(INT nNpcSettingIdxInfo, INT nSubWorld, INT nMpsX, INT nMpsY, INT nBoss, INT nRodom, INT nOffwminx, INT nOffwmaxx, INT nOffhminy, INT nOffhmaxy)
{
	INT nRegion, nmMapX, nmMapY, nOffX, nOffY;
	if (nSubWorld < 0 || nSubWorld >= MAX_SUBWORLD)
		return 0;

	INT nBarrier;

	/*if (nRodom>0)
	{
		INT mMpsPosX,mMpsPosY;
			SubWorld[nSubWorld].Mps2MapnRodom(&nRegion, &nmMapX, &nmMapY, &nOffX, &nOffY,&mMpsPosX,&mMpsPosY,nOffwminx,nOffwmaxx,nOffhminy,nOffhmaxy);
			nBarrier = SubWorld[nSubWorld].GetBarrier(mMpsPosX, mMpsPosY);  ///需要像素坐标障碍物判断
		if (Obstacle_NULL!=nBarrier)
			return 0;
	}
	else*/
	{
		// SubWorld[nSubWorld].Mps2Map(nMpsX, nMpsY, &nRegion, &nmMapX, &nmMapY, &nOffX, &nOffY);
		// 障碍物判断

		POINT ptLocal;
		// KMapPos	Pos;
		// GetMpsPos(&nX, &nY,&nMap);                          // 怪物的坐标=转换后的坐标
		ptLocal.x = nMpsX;
		ptLocal.y = nMpsY;

		SubWorld[nSubWorld].GetFreeObjPos(ptLocal); // 获取周围没有障碍物的坐标(位置矫正)

		SubWorld[nSubWorld].Mps2Map(ptLocal.x, ptLocal.y, &nRegion, &nmMapX, &nmMapY, &nOffX, &nOffY);

		// if (nRegion < 0)
		//{
		//  printf("[Map]传送非法坐标 Pos(%d,%d,m:%d,行:%d) 无效坐标!\n", nX, nY,dwSubWorldID,nTargetSubWorld);
		//  return 0;
		//}
	}

	if (nRegion < 0)
	{
		/*if (SubWorld[nSubWorld].m_SubWorldID==42)
		{
			printf("--地图%d,刷怪:失败---\n",SubWorld[nSubWorld].m_SubWorldID);
		}*/
		return 0;
	}

	INT nIndexID = 0;
	nIndexID = AddServerNpcB(nNpcSettingIdxInfo, nSubWorld, nRegion, nmMapX, nmMapY, nOffX, nOffY, nBoss);

	if (nIndexID > 0)
	{
		// 如果是黄金怪物
		if (nBoss > 0 && Npc[nIndexID].m_Kind == kind_normal) // 特殊NPC  g_NpcMapDropRate
		{													  // if(!IsPlayer() && m_Kind!=kind_dialoger && m_Kind!=kind_partner)
			Npc[nIndexID].m_cGold.SetGoldCurrentType(nBoss, nSubWorld);
			// Npc[nIndexID].m_cGold.RandChangeGold(0,0,nBoss,nSubWorld);  //同步怪物类型
		}
		//-----------------------------------------------------------------------------------------
#ifndef _SERVER																	  // 普通NPC 和特殊NPC 通用
		Npc[nIndexID].m_DataRes.Init(Npc[nIndexID].szNpcTypeName, &g_NpcResList); // 外观初始化
		Npc[nIndexID].m_DataRes.SetAction(Npc[nIndexID].m_ClientDoing);			  // 设定外观行为对接
		Npc[nIndexID].m_DataRes.SetRideHorse(Npc[nIndexID].m_bRideHorse);		  // 设定是否骑马接帧数
		Npc[nIndexID].m_DataRes.SetArmor(Npc[nIndexID].m_ArmorType, Npc[nIndexID].m_MantleType);			  // 衣服
		Npc[nIndexID].m_DataRes.SetHelm(Npc[nIndexID].m_HelmType);				  // 头部
		Npc[nIndexID].m_DataRes.SetHorse(Npc[nIndexID].m_HorseType);			  // 马位置
		Npc[nIndexID].m_DataRes.SetWeapon(Npc[nIndexID].m_WeaponType);			  // 武器
#endif
		//-----------------------------------------------------------------------------------------
	}
	return nIndexID;
}

INT KNpcSet::AddServerNpcB(INT nNpcSettingIdxInfo, INT nSubWorld, INT nRegion, INT nMapX, INT nMapY, INT nOffX /* = 0 */, INT nOffY, INT nBoss)
{
	INT nNpcSettingIdx = (short)HIWORD(nNpcSettingIdxInfo); // >> 7; //除于128

	if (nNpcSettingIdx > g_NpcSetting.GetHeight() - 2) // NPC.txt 的行数
	{
		printf(" -------------(超出范围)增加NPC失败-------------- \n");
		return 0;
	}

	INT i = FindFree();
	if (i <= 0)
	{ // 查找空闲节点
		printf(" -------------NPC已经满员(刷怪失败) OK------------ \n");
		return 0;
	}
#ifndef _SERVER
	Npc[i].m_sClientNpcID.m_dwRegionID = 0;
	Npc[i].m_sClientNpcID.m_nNo = -1;
	Npc[i].Remove(); // 初始化 删除外观？
#endif
	// INT nNpcSettingIdx = (short)HIWORD(nNpcSettingIdxInfo);// >> 7; //除于128
	/*if (SubWorld[nSubWorld].m_SubWorldID==42)
	{
		printf("--地图%d,Npc[%d].Load():---\n",SubWorld[nSubWorld].m_SubWorldID,i);
	}*/

	INT nLevel = LOWORD(nNpcSettingIdxInfo); // & 0x7f;
	Npc[i].m_Index = i;						 // 节点的索引
	Npc[i].m_SkillList.m_nNpcIndex = i;
	Npc[i].Load(nNpcSettingIdx, nLevel, nSubWorld, nBoss); // 加载NPC信息等  人物类型 等
	Npc[i].m_SubWorldIndex = nSubWorld;
	Npc[i].m_RegionIndex = nRegion;

#ifndef _SERVER
	if (nRegion >= 0 && nRegion < 9)
		Npc[i].m_dwRegionID = SubWorld[nSubWorld].m_Region[nRegion].m_RegionID;
#endif
	Npc[i].m_MapX = nMapX;
	Npc[i].m_MapY = nMapY;
	Npc[i].m_OffX = nOffX;
	Npc[i].m_OffY = nOffY;
	SubWorld[nSubWorld].NewMap2Mps(nRegion, nMapX, nMapY, nOffX, nOffY, &Npc[i].m_OriginX, &Npc[i].m_OriginY);

#ifdef _SERVER
	SetID(i);						// i为index值设定NPC的DWid(全局)= 数量 第几个NPC
	Npc[i].m_cDeathCalcExp.Clear(); // 清空经验
#endif
	// 修改可用与使用表
	m_FreeIdx.Remove(i);						 // 可用表删除一个
	m_UseIdx.Insert(i);						 // 已用表增加一个
	SubWorld[nSubWorld].m_Region[nRegion].AddNpc(i); // 增加节点m_WorldMessage.Send(GWM_NPC_ADD, nRegion, i);
	// SubWorld[nSubWorld].m_Region[nRegion].AddRef(nMapX, nMapY, obj_npc);  //NPC 出现的位置
	SubWorld[nSubWorld].m_Region[nRegion].AddNpcRef(nMapX, nMapY);

#ifndef _SERVER
	Npc[i].m_dwRegionID = SubWorld[nSubWorld].m_Region[nRegion].m_RegionID;
	//---------------------------------------------------------------------------
	Npc[i].m_DataRes.Init(Npc[i].szNpcTypeName, &g_NpcResList); // 外观初始化
	Npc[i].m_DataRes.SetAction(Npc[i].m_ClientDoing);			// 设定外观行为对接Npc[i].m_ClientDoing
	Npc[i].m_DataRes.SetRideHorse(Npc[i].m_bRideHorse);			// 设定是否骑马接帧数
	Npc[i].m_DataRes.SetArmor(Npc[i].m_ArmorType, Npc[i].m_MantleType);				// 衣服
	Npc[i].m_DataRes.SetHelm(Npc[i].m_HelmType);				// 头部
	Npc[i].m_DataRes.SetHorse(Npc[i].m_HorseType);				// 马位置
	Npc[i].m_DataRes.SetWeapon(Npc[i].m_WeaponType);			// 武器
	if (Npc[i].m_Kind == kind_player)
	{
		// if (Npc[i].m_PifengType>0)
		//    Npc[i].m_DataRes.SetPifeng(Npc[i].m_PifengType);             //披风
		//if (Npc[i].m_ChiBangType > 0)
		//	Npc[i].m_DataRes.SetChiBang(Npc[i].m_ChiBangType); // 翅膀
	}
	//---------------------------------------------------------------------------
#endif
	return i;
}

BOOL KNpcSet::CheckThisNpc(INT nSetings, char* nKey)
{

	INT nRow = g_ForbitMap.GetHeight() + 1, nReg = FALSE;

	for (INT i = 2; i < nRow; ++i)
	{
		INT nSkilid = 0;
		g_ForbitMap.GetInteger(i, nKey, 0, &nSkilid);
		if (nSkilid == nSetings)
		{
			nReg = TRUE;
			break;
		}
	}
	return nReg;
}

//------------------------Optimized new function----------------------------------------
KNpcTemplate* KNpcSet::GetTemplate(INT nNpcTemplateId, INT nLevel)
{
	if (nNpcTemplateId < 0 || nNpcTemplateId > MAX_NPCSTYLE - 1 || nLevel < 0 || nLevel > MAX_LEVEL - 1)
	{
		return NULL;
	}

	char nFlag[32] = { 0 };
	sprintf(nFlag, "idx_%d_level_%d", nNpcTemplateId, nLevel); //Data at this level of this IDX

	DWORD dwKey = g_FileName2Id(nFlag);

	if (nNpcTemplateId >= 0 && nNpcTemplateId <= MAX_NPCSTYLE - 1 /*&& eSeries >= series_metal && eSeries < series_num*/ && nLevel >= 0 && nLevel <= MAX_LEVEL - 1)
	{
		// 尝试查找已有数据
		// DWORD dwKey	= nKeyInfo;//((DWORD)nNpcTemplateId << 16) | ((DWORD)eSeries << 8) | (DWORD)nLevel; //((DWORD)nLevel << 8); /*
		_KMapTemplate::iterator it = m_mapTemplate.find(dwKey);
		if (it != m_mapTemplate.end())
		{
			// #ifdef _SERVER
			//	printf("---[已经存在]找到NPC(%s):%d 等级节点:%d---\n",it->second->Name,nNpcTemplateId,it->second->m_nLevel);
			// #endif

			return it->second; // 找到了
		}

		// 先载入0级基础数据
		sprintf(nFlag, "idx_%d_level_0", nNpcTemplateId);
		DWORD dwBaseKey = g_FileName2Id(nFlag); // dwKey & 0xffffff00;
		it = m_mapTemplate.find(dwBaseKey);
		if (it == m_mapTemplate.end())
		{
			KNpcTemplate* pTemplate = new KNpcTemplate;
			if (!pTemplate->Init(nNpcTemplateId))
			{
				// SAFE_DELETE(pTemplate);
				if (pTemplate)
				{
					delete pTemplate;
					pTemplate = NULL;
				}
				return NULL;
			}
			// pTemplate->m_Series		= eSeries;//不修改 基本值
			m_mapTemplate[dwBaseKey] = pTemplate;

			// #ifdef _SERVER
			//			printf("---[新建立基本数据]NPC(%s):%d 等级节点:0---\n",pTemplate->Name,nNpcTemplateId);
			// #endif
		}

		// 如果要的就是0级的，直接返回
		KNpcTemplate* pBaseTemplate = m_mapTemplate[dwBaseKey];

		if (nLevel == 0)
		{
			// #ifdef _SERVER
			// printf("---[要求返回基本数据]找到NPC(%s):%d 等级节点:%d---\n",pBaseTemplate->Name,nNpcTemplateId,nLevel);
			// #endif
			return pBaseTemplate;
		}
		// 开始加载脚本数据--------------------------------------

		KLuaScript* pLevelScript = NULL;

#ifdef _SERVER
		pLevelScript = (KLuaScript*)g_GetScript(pBaseTemplate->m_dwLevelSettingScript);

		if (pLevelScript == NULL)			  // 如果没有脚本的 就设置默认的脚本
			pLevelScript = g_pNpcLevelScript; // npcLevelscript.lua
#else
		KLuaScript LevelScript;
		if (!pBaseTemplate->m_szLevelSettingScript[0])
			pLevelScript = g_pNpcLevelScript;
		else
		{
			LevelScript.Init(); // 初始化 脚本函数
			if (!LevelScript.Load(pBaseTemplate->m_szLevelSettingScript))
			{
				pLevelScript = g_pNpcLevelScript;
				LevelScript.Exit();
			}
			else
				pLevelScript = &LevelScript;
		}

#endif
		// 载入等级相关数据-------------------------------------
		KNpcTemplate* pTemplate = new KNpcTemplate;
		*pTemplate = *pBaseTemplate;
		pTemplate->m_nLevel = nLevel;
		// #ifdef _SERVER
		//		printf("---[创建新等级节点]找到NPC(%s):%d 新等级节点:%d---\n",pBaseTemplate->Name,nNpcTemplateId,nLevel);
		// #endif

		if (!pTemplate->InitNpcLevelData(nNpcTemplateId, pLevelScript, nLevel))
		{
			// SAFE_DELETE(pTemplate);
			if (pTemplate)
			{
				delete pTemplate;
				pTemplate = NULL;
			}
			return NULL;
		}
		m_mapTemplate[dwKey] = pTemplate; // 赋值给这个节点

		return pTemplate;
	}
	else
		return NULL;
}