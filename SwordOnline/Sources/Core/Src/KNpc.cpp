//-----------------------------------------------------------------------
//	Sword3 KNpc.cpp
//-----------------------------------------------------------------------
#include "KCore.h"
//#include <crtdbg.h>
#include "KNpcAI.h"
#include "KSkills.h"
#include "KObj.h"
#include "KObjSet.h"
#include "KMath.h"
#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KNpc.h"
#include "GameDataDef.h"
#include "KSubWorldSet.h"
#include "KRegion.h"
#include "KTaskFuns.h"
#include "KNpcTemplate.h"
#include "KNpcRes.h"
#include "KItemSet.h"
#include "KItemChangeRes.h"
#include "KTongData.h"
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/Interface/iServer.h"
#include "KPlayerSet.h"
#include "KSkillManager.h"
#include "../../../lib/S3DBInterface.h"
#else
#include "../../Headers/IClient.h"
#include "CoreShell.h"
#include "Scene/KScenePlaceC.h"
#include "KIme.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "ImgRef.h"
#include "Text.h"
#include "KOption.h"
#endif
#include "KNpcAttribModify.h"
#include "CoreUseNameDef.h"
#include "KSubWorld.h"
#include "Scene/ObstacleDef.h"
#include "KThiefSkill.h"
#ifdef _STANDALONE
#include "KThiefSkill.cpp"
#endif
#include <iostream>

#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

extern KLuaScript* g_pNpcLevelScript;

#define	ATTACKACTION_EFFECT_PERCENT		60	// ·¢¼¼ÄÜ¶¯×÷Íê³É°Ù·ÖÖ®¶àÉÙ²ÅÕæÕý·¢³öÀ´
#define	MIN_DOMELEE_RANGE				20
#define	MIN_BLURMOVE_SPEED				1
#define	ACCELERATION_OF_GRAVITY			10
#define MIN_JUMP_RANGE 0

#define FIND_PATH_DISTANCE 1

#define		SHOW_CHAT_WIDTH				24
#define		SHOW_CHAT_COLOR				0xffffffff

#define		BLOOD_EVENTTIME				400
#define		BLOOD_MOVESPEED				1
#define		BLOOD_FONTSIZE				16

#define		SHOW_LIFE_WIDTH				38
#define		SHOW_LIFE_HEIGHT			3

#define		SHOW_SPACE_HEIGHT			5
//-----------------------------------------------------------------------
#define	STAMINA_RECOVER_SCALE	4
// ÇøÓòµÄ¿í¸ß£¨¸ñ×Óµ¥Î»£©
#define	REGIONWIDTH			SubWorld[m_SubWorldIndex].m_nRegionWidth
#define	REGIONHEIGHT		SubWorld[m_SubWorldIndex].m_nRegionHeight
// ¸ñ×ÓµÄ¿í¸ß£¨ÏñËØµ¥Î»£¬·Å´óÁË1024±¶£©
#define	CELLWIDTH			(SubWorld[m_SubWorldIndex].m_nCellWidth << 10)
#define	CELLHEIGHT			(SubWorld[m_SubWorldIndex].m_nCellHeight << 10)
// µ±Ç°ÇøÓò
#define	CURREGION			SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex]
// ÏàÁÚÇøÓòµÄË÷Òý
#define	LEFTREGIONIDX		CURREGION.m_nConnectRegion[2]
#define	RIGHTREGIONIDX		CURREGION.m_nConnectRegion[6]
#define	UPREGIONIDX			CURREGION.m_nConnectRegion[4]
#define	DOWNREGIONIDX		CURREGION.m_nConnectRegion[0]
#define	LEFTUPREGIONIDX		CURREGION.m_nConnectRegion[3]
#define	LEFTDOWNREGIONIDX	CURREGION.m_nConnectRegion[1]
#define	RIGHTUPREGIONIDX	CURREGION.m_nConnectRegion[5]
#define	RIGHTDOWNREGIONIDX	CURREGION.m_nConnectRegion[7]

#define	LEFTREGION			SubWorld[m_SubWorldIndex].m_Region[LEFTREGIONIDX]
#define	RIGHTREGION			SubWorld[m_SubWorldIndex].m_Region[RIGHTREGIONIDX]
#define	UPREGION			SubWorld[m_SubWorldIndex].m_Region[UPREGIONIDX]
#define	DOWNREGION			SubWorld[m_SubWorldIndex].m_Region[DOWNREGIONIDX]
#define	LEFTUPREGION		SubWorld[m_SubWorldIndex].m_Region[LEFTUPREGIONIDX]
#define	LEFTDOWNREGION		SubWorld[m_SubWorldIndex].m_Region[LEFTDOWNREGIONIDX]
#define	RIGHTUPREGION		SubWorld[m_SubWorldIndex].m_Region[RIGHTUPREGIONIDX]
#define	RIGHTDOWNREGION		SubWorld[m_SubWorldIndex].m_Region[RIGHTDOWNREGIONIDX]

#define	CONREGION(x)		SubWorld[m_SubWorldIndex].m_Region[CURREGION.m_nConnectRegion[x]]
#define	CONREGIONIDX(x)		CURREGION.m_nConnectRegion[x]
// µ±Ç°¼¼ÄÜ
#define BROADCAST_REGION(pBuff,uSize,uMaxCount)	 if(m_SubWorldIndex >= 0 && m_SubWorldIndex < MAX_SUBWORLD && SubWorld[m_SubWorldIndex].m_SubWorldID != -1) SubWorld[m_SubWorldIndex].BroadCastRegion((pBuff), (uSize), (uMaxCount), m_RegionIndex, m_MapX, m_MapY);
//-----------------------------------------------------------------------
// Npc[0]²»ÔÚÓÎÏ·ÊÀ½çÖÐÊ¹ÓÃ£¬×öÎªÒ»¸öNpcSetÓÃÓÚÌí¼ÓÐÂµÄNPC¡£
KNpc	Npc[MAX_NPC];


KNpcTemplate* g_pNpcTemplate[MAX_NPCSTYLE]; //0,0ÎªÆðµã

//-----------------------------------------------------------------------

KNpc::KNpc()
{
#ifdef _SERVER
	m_AiSkillRadiusLoadFlag = 0;	// Ö»ÐèÒªÔÚ¹¹ÔìµÄÊ±ºò³õÊ¼»¯Ò»´Î
#endif
	Init();
}

void KNpc::Init()
{
	memset(m_btStateInfo, 0, sizeof(m_btStateInfo));
	m_dwID = 0;
	m_Index = 0;
	m_nPlayerIdx = 0;
	m_ProcessAI = 1;
	m_Kind = kind_normal;
	m_Series = series_metal;
	m_btSpecial = npc_normal;
	m_Camp = camp_free;
	m_CurrentCamp = camp_free;
	m_Doing = do_stand;
	m_Height = 0;
	m_Frames.nCurrentFrame = 0;
	m_Frames.nTotalFrame = 0;
	m_SubWorldIndex = 0;
	m_RegionIndex = 0;
	m_Experience = 0;
	m_ActiveSkillID = 0;
	m_SkillParam1 = 0;
	m_SkillParam2 = 0;

	m_bNpcRemoveDeath = FALSE;
	m_nNpcTimeout = -1;
	ZeroMemory(m_nNpcParam, sizeof(m_nNpcParam));
	m_bNpcFollowFindPath = FALSE;
	m_uFindPathTime = 0;
	m_uFindPathMaxTime = 0;
	m_uLastFindPathTime = 0;
	m_nIsOver = FALSE;

#ifndef _SERVER
	m_ClientDoing = cdo_stand;
	m_nChatContentLen = 0;
	m_nCurChatTime = 0;
	m_nChatNumLine = 0;
	m_nChatFontWidth = 0;
	m_nStature = 0;
	m_dwTongNameID = 0;
	ZeroMemory(m_szTongName, sizeof(m_szTongName));
	ZeroMemory(m_szTongAgname, sizeof(m_szTongAgname));
	m_nTongNationalEmblem = 0;
	m_nFigure = -1;
	m_nTeamServerID = -1;
	m_nCheckAutoMoveBarrier = 0;
#endif

	m_CurrentLife = 100;			// NpcµÄµ±Ç°ÉúÃü
	m_CurrentLifeMax = 100;		// NpcµÄµ±Ç°ÉúÃü×î´óÖµ
	m_CurrentLifeReplenish = 0;	// NpcµÄµ±Ç°ÉúÃü»Ø¸´ËÙ¶È
	m_CurrentLifeReplenishPercent = 0;
	m_CurrentMana = 100;			// NpcµÄµ±Ç°ÄÚÁ¦
	m_CurrentManaMax = 100;		// NpcµÄµ±Ç°×î´óÄÚÁ¦
	m_CurrentManaReplenish = 0;	// NpcµÄµ±Ç°ÄÚÁ¦»Ø¸´ËÙ¶È
	m_CurrentStamina = 100;		// NpcµÄµ±Ç°ÌåÁ¦
	m_CurrentStaminaMax = 100;	// NpcµÄµ±Ç°×î´óÌåÁ¦
	m_CurrentStaminaGain = 0;	// NpcµÄµ±Ç°ÌåÁ¦»Ø¸´ËÙ¶È
	m_CurrentStaminaLoss = 0;	// NpcµÄµ±Ç°ÌåÁ¦ÏÂ½µËÙ¶È
	m_CurrentAttackRating = 100;	// NpcµÄµ±Ç°ÃüÖÐÂÊ
	m_CurrentDefend = 10;		// NpcµÄµ±Ç°·ÀÓù
	m_CurrentWalkSpeed = 5;		// NpcµÄµ±Ç°×ß¶¯ËÙ¶È
	m_CurrentRunSpeed = 10;		// NpcµÄµ±Ç°ÅÜ¶¯ËÙ¶È
	m_CurrentJumpSpeed = 12;	// NpcµÄµ±Ç°ÌøÔ¾ËÙ¶È
	m_CurrentJumpFrame = 40;	// NpcµÄµ±Ç°ÌøÔ¾Ê±¼ä
	m_CurrentAttackSpeed = 0;	// NpcµÄµ±Ç°¹¥»÷ËÙ¶È
	m_CurrentCastSpeed = 0;		// NpcµÄµ±Ç°Ê©·¨ËÙ¶È
	m_CurrentVisionRadius = 40;	// NpcµÄµ±Ç°ÊÓÒ°·¶Î§
	m_CurrentAttackRadius = 30;	// NpcµÄµ±Ç°¹¥»÷·¶Î§
	m_CurrentHitRecover = 0;	// NpcµÄµ±Ç°ÊÜ»÷»Ø¸´ËÙ¶È
	m_CurrentAddPhysicsDamage = 0;	// NpcµÄµ±Ç°ÎïÀíÉËº¦Ö±½Ó¼ÓµÄµãÊý
	m_CurrentAddPhysicsMagic = 0;
	m_CurrentAddEngergyDamage = 0;
	m_CurrentAddPhysicsDamageP = 0; // 当前被动外普百分比

	m_Dir = 0;					// NpcµÄ·½Ïò
	m_JumpStep = 0;
	m_JumpDir = 0;
	m_MapZ = 0;					// NpcµÄ¸ß¶È
	m_HelmType = 0;				// NpcµÄÍ·¿øÀàÐÍ
	m_ArmorType = 0;			// NpcµÄ¿ø¼×ÀàÐÍ
	m_WeaponType = 0;			// NpcµÄÎäÆ÷ÀàÐÍ
	m_HorseType = -1;			// NpcµÄÆïÂíÀàÐÍ
	m_bRideHorse = FALSE;		// NpcÊÇ·ñÆïÂí
	m_dwNextSwitchHorseTime = 0;
	m_MaskType = 0;					// Npc Ãæ¾ß¹¦ÄÜ
	m_bMaskFeature = FALSE;
	m_MantleType = 0;
	m_nPKFlag = enumPKNormal;
	m_nMissionGroup = -1;

	ZeroMemory(Name, sizeof(Name));		// NpcµÄÃû³Æ
	ZeroMemory(Owner, sizeof(Owner));		// NpcµÄÃû³Æ
	ZeroMemory(MateName, sizeof(MateName));		// NpcµÄÃû³Æ

	m_NpcSettingIdx = 0;		// NpcµÄÉè¶¨ÎÄ¼þË÷Òý
	m_CorpseSettingIdx = 0;		// BodyµÄÉè¶¨ÎÄ¼þË÷Òý
	ZeroMemory(ActionScript, sizeof(ActionScript));
	m_ActionScriptID = 0;
	m_DropScriptID = 0;
	m_TrapScriptID = 0;

	m_RankID = 0;
	m_ExpandRank.Release();
	m_CurExpandRank.Release();
	m_byTranslife = 0;
	m_byViprank = 0;
	m_nRepute = 0;
	m_nFuYuan = 0;
	m_nPKValue = 0;
	m_ImagePlayer = 0;
	m_byMantleLevel = 0;
	m_nFactionNumber = -1;

	m_LifeMax = 100;		// NpcµÄ×î´óÉúÃü
	m_LifeReplenish = 0;		// NpcµÄÉúÃü»Ø¸´ËÙ¶È
	m_ManaMax = 100;		// NpcµÄ×î´óÄÚÁ¦
	m_ManaReplenish = 0;		// NpcµÄÄÚÁ¦»Ø¸´ËÙ¶È
	m_StaminaMax = 100;		// NpcµÄ×î´óÌåÁ¦
	m_StaminaGain = 0;		// NpcµÄÌåÁ¦»Ø¸´ËÙ¶È
	m_StaminaLoss = 0;		// NpcµÄÌåÁ¦ÏÂ½µËÙ¶È
	m_AttackRating = 100;		// NpcµÄÃüÖÐÂÊ
	m_Defend = 10;		// NpcµÄ·ÀÓù
	m_WalkSpeed = 6;		// NpcµÄÐÐ×ßËÙ¶È
	m_RunSpeed = 10;		// NpcµÄÅÜ¶¯ËÙ¶È
	m_JumpSpeed = 12;		// NpcµÄÌøÔ¾ËÙ¶È
	m_AttackSpeed = 0;		// NpcµÄ¹¥»÷ËÙ¶È
	m_CastSpeed = 0;		// NpcµÄÊ©·¨ËÙ¶È
	m_VisionRadius = 40;		// NpcµÄÊÓÒ°·¶Î§
	m_DialogRadius = 124;		// NpcµÄ¶Ô»°·¶Î§
	m_HitRecover = 12;		// NpcµÄÊÜ»÷»Ø¸´ËÙ¶È
	m_nPeopleIdx = 0;

	m_LoopFrames = 0;
	m_WalkFrame = 12;
	m_RunFrame = 15;
	m_StandFrame = 15;
	m_DeathFrame = 15;
	m_HurtFrame = 10;
	m_AttackFrame = 20;
	m_CastFrame = 20;
	m_SitFrame = 15;
	m_JumpFrame = 40;
	m_AIMAXTime = 25;
	m_NextAITime = 0;
	m_ProcessState = 1;
	m_ReviveFrame = 100;
	m_bExchangeServer = FALSE;
	m_bActivateFlag = FALSE;
	m_FightMode = enumFightNone;
	m_OldFightMode = enumFightNone;

#ifdef _SERVER
	m_nCurPKPunishState = 0;
	m_bReviveNow = FALSE;
#else
	m_SyncSignal = 0;
	m_sClientNpcID.m_dwRegionID = 0;
	m_sClientNpcID.m_nNo = -1;
	m_ResDir = 0;
	m_nSleepFlag = 0;

	m_btCurBlood = 0;
	memset(m_nBlood, 0, sizeof(m_nBlood));
	memset(m_szBlood, 0, sizeof(m_szBlood));

	m_nHurtHeight = 0;
	m_nHurtDesX = 0;
	m_nHurtDesY = 0;

	m_nPacePercent = 0;

	m_bTongFlag = 0;
	m_MarkMask = 0;
	m_PTrade.Release();
#endif

	m_nLastPoisonDamageIdx = 0;
	m_nLastDamageIdx = 0;
	m_bHaveLoadedFromTemplate = FALSE;
	m_bClientOnly = FALSE;
}

ISkill* KNpc::GetActiveSkill()
{
	_ASSERT(m_ActiveSkillID < MAX_SKILL);
	if (m_SkillList.GetLevel(m_ActiveSkillID) > 0)
		return g_SkillManager.GetSkill(m_ActiveSkillID, m_SkillList.GetCurrentLevel(m_ActiveSkillID));
	else
		return NULL;
};

#ifdef _SERVER
void KNpc::SetTempCurrentCamp(INT nCamp)
{
	if (Player[m_nPlayerIdx].m_cTeam.m_nFlag)
	{
		if (Player[m_nPlayerIdx].m_cTeam.m_nFigure == TEAM_CAPTAIN)
		{
			Npc[Player[m_nPlayerIdx].m_nIndex].ChangeCurrentCamp(nCamp);
			for (INT i = 0; i < g_Team[Player[m_nPlayerIdx].m_cTeam.m_nID].m_nMemNum; i++)
			{
				if (g_Team[Player[m_nPlayerIdx].m_cTeam.m_nID].m_nMember[i] > 0)
					Npc[Player[g_Team[Player[m_nPlayerIdx].m_cTeam.m_nID].m_nMember[i]].m_nIndex].ChangeCurrentCamp(nCamp);
			}
		}
		else
			Npc[Player[m_nPlayerIdx].m_nIndex].ChangeCurrentCamp(
				Npc[Player[g_Team[Player[m_nPlayerIdx].m_cTeam.m_nID].m_nCaptain].m_nIndex].m_CurrentCamp);
	}
}
#endif

void KNpc::SetCurrentCamp(INT nCamp)
{
	if (IsPlayer())
	{
#ifdef _SERVER
		if (Player[m_nPlayerIdx].m_bForbidCamp)
			return;
#endif
		if (Player[m_nPlayerIdx].m_cTeam.m_nFlag)
		{
#ifdef _SERVER
			SetTempCurrentCamp(nCamp);
#endif
			return;
		}
		else
			m_CurrentCamp = nCamp;
	}
	else
		m_CurrentCamp = nCamp;

#ifdef _SERVER
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

	if (m_RegionIndex < 0)
		return;

	NPC_CHGCURCAMP_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcchgcurcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_CurrentCamp;

	INT	nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

void KNpc::ChangeCurrentCamp(INT nCamp)
{
	m_CurrentCamp = nCamp;

#ifdef _SERVER
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

	if (m_RegionIndex < 0)
		return;

	NPC_CHGCURCAMP_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcchgcurcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_CurrentCamp;

	INT	nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

void KNpc::SetCamp(INT nCamp)
{
	m_Camp = nCamp;
#ifdef _SERVER
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
	if (m_RegionIndex < 0)
		return;

	NPC_CHGCAMP_SYNC	NetCommand;

	NetCommand.ProtocolType = (BYTE)s2c_npcchgcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_Camp;

	INT nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

void KNpc::RestoreCurrentCamp()
{
#ifdef _SERVER
	if (IsPlayer() && Player[m_nPlayerIdx].m_bForbidCamp)
		return;
#endif
	m_CurrentCamp = m_Camp;
#ifdef _SERVER
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
	if (m_RegionIndex < 0)
		return;

	NPC_CHGCURCAMP_SYNC	NetCommand;

	NetCommand.ProtocolType = (BYTE)s2c_npcchgcurcamp;
	NetCommand.ID = m_dwID;
	NetCommand.Camp = (BYTE)m_CurrentCamp;
	INT nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

#define		NPC_SHOW_CHAT_TIME		15000
INT		IR_IsTimePassed(UINT UINTerval, UINT& uLastTimer);

void KNpc::Activate()
{
	// ²»´æÔÚÕâ¸öNPC
	if (!m_Index)
	{
		//g_DebugLog("[DEATH] No Index: %d", m_Index);
		return;
	}

	// ÇÐ»»µØÍ¼ÖÐ£¬²»´¦Àí
	if (m_bExchangeServer)
	{
		//g_DebugLog("[DEATH] Change Server: %d", m_bExchangeServer);
		return;
	}
	// Check here
	if (m_bActivateFlag)
	{
		m_bActivateFlag = FALSE;	// restore flag
		return;
	}
	//	m_bActivateFlag = TRUE;

	m_LoopFrames++;
	// Process npc special state, such as curse, etc.
	//g_DebugLog("[DEATH] m_ProcessState: %d", m_ProcessState);
#ifdef _SERVER
	this->CheckTrap(m_MapX, m_MapY);

	if (m_nNpcTimeout && g_SubWorldSet.GetGameTime() >= m_nNpcTimeout)
	{
		//	    g_DebugLog("den gio xoa");
		m_nNpcTimeout = 0;
		if (m_ActionScriptID)
		{
			NpcSet.ExecuteScript(m_Index, m_ActionScriptID, "Timeout", m_Index);
		}
	}

	this->m_cDeathCalcExp.Active();
#endif

	if (m_ProcessState)
	{
		if (ProcessState())
			return;
	}
	if (m_ProcessAI)
	{
		NpcAI.Activate(m_Index);
	}

	ProcCommand(m_ProcessAI);
	ProcStatus();

#ifndef _SERVER

	if (m_RegionIndex == -1)
		return;

	INT		nMpsX, nMpsY;

	if (m_MarkMask)
		GetNpcCopyFromTemplate(m_MaskType, m_Level);

	m_DataRes.SetAction(m_ClientDoing);
	if (m_bRideHorse && m_ClientDoing == cdo_jump)
		m_DataRes.SetRideHorse(FALSE);
	else
		m_DataRes.SetRideHorse(m_bRideHorse);
	m_DataRes.SetArmor(m_ArmorType, m_MantleType);
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	m_DataRes.SetWeapon(m_WeaponType);
	// ´¦Àí¼¼ÄÜ²úÉúµÄ×´Ì¬µÄÌØÐ§
	m_DataRes.SetState(m_btStateInfo, &g_NpcResList);

	if (Player[CLIENT_PLAYER_INDEX].m_nIndex == m_Index)
	{
		SubWorld[0].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		m_DataRes.SetPos(m_Index, nMpsX, nMpsY, m_Height, TRUE);
	}
	else
	{
		SubWorld[0].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		m_DataRes.SetPos(m_Index, nMpsX, nMpsY, m_Height, FALSE);
	}

	// client npc Ê±¼ä¼ÆÊý´¦Àí£º²»ÍùºóÌø
	if (m_Kind == kind_bird || m_Kind == kind_mouse)
		m_SyncSignal = SubWorld[0].m_dwCurrentTime;

	if (m_nChatContentLen > 0)
	{
		if (IR_GetCurrentTime() - m_nCurChatTime > NPC_SHOW_CHAT_TIME)
		{
			m_nChatContentLen = 0;
			m_nChatNumLine = 0;
			m_nChatFontWidth = 0;
			m_nCurChatTime = 0;
		}
	}

	if (IsPlayer())
	{
		if (m_PTrade.nTrade)
		{
			if (m_Doing != do_sit)
				SendClientCmdSit(true);
		}
	}
#endif
}

void KNpc::ProcStatus()
{
	//g_DebugLog("[DEATH] m_bExchangeServer: %d", m_bExchangeServer);
	if (m_bExchangeServer)
		return;

	switch (m_Doing)
	{
	case do_stand:
		OnStand();
		break;
	case do_run:
		OnRun();
		break;
	case do_walk:
		OnWalk();
		break;
	case do_attack:
	case do_magic:
		OnSkill();
		break;
	case do_sit:
		OnSit();
		break;
	case do_jump:
		OnJump();
		break;
	case do_hurt:
		OnHurt();
		break;
	case do_revive:
		OnRevive();
		break;
	case do_death:
		OnDeath();
		break;
	case do_defense:
		OnDefense();
		break;
	case do_special1:
		OnSpecial1();
		break;
	case do_special2:
		OnSpecial2();
		break;
	case do_special3:
		OnSpecial3();
		break;
	case do_blurmove:
		OnBlurMove();
		break;
	case do_manyattack:
		OnManyAttack();
		break;
	case do_runattack:
		OnRunAttack();
		break;
	case do_jumpattack:
		OnJumpAttack();
		break;
	case do_idle:
		OnIdle();
		break;
	default:
		break;
	}
#ifndef _SERVER
	if (m_MaskType)
	{
		if (m_MarkMask)
		{
			if (m_MarkMask != m_MaskType)
			{
				ResetNpcTypeName(1);
				m_MarkMask = 0;
			}
		}
		else
		{
			ResetNpcTypeName(0);
			m_MarkMask = m_MaskType;
		}
	}
	else
	{
		if (m_MarkMask)
		{
			ResetNpcTypeName(1);
			m_MarkMask = 0;
		}
	}
#endif

}

void KNpc::ProcCommand(INT nAI)
{
	// CmdKind < 0 ±íÊ¾Ã»ÓÐÖ¸Áî	½»»»µØÍ¼Ò²²»´¦Àí
	if (m_Command.CmdKind == do_none || m_bExchangeServer)
		return;

	if (nAI)
	{
		if (m_RegionIndex < 0)
			return;

		switch (m_Command.CmdKind)
		{
		case do_stand:
			DoStand();
			break;
		case do_walk:
			Goto(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_run:
			RunTo(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_jump:
			JumpTo(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_skill:
			if (INT nSkillIdx = m_SkillList.FindSame(m_Command.Param_X))
			{
				SetActiveSkill(nSkillIdx);
				DoSkill(m_Command.Param_Y, m_Command.Param_Z);
			}
			else
			{
				DoStand();
			}
			break;
		case do_sit:
			DoSit();
			break;
		case do_defense:
			DoDefense();
			break;
		case do_idle:
			DoIdle();
			break;
		case do_hurt:
			DoHurt(m_Command.Param_X, m_Command.Param_Y, m_Command.Param_Z);
			break;
			// ÒòÎª¿çµØÍ¼ÄÜ°ÑaiÉèÎª1
		case do_revive:
			DoStand();
			m_ProcessAI = 1;
			m_ProcessState = 1;
#ifndef _SERVER
			this->SetInstantSpr(enumINSTANT_STATE_REVIVE);
#endif
			break;
		}
	}
	else
	{
		switch (m_Command.CmdKind)
		{
		case do_hurt:
			if (m_RegionIndex >= 0)
				DoHurt(m_Command.Param_X, m_Command.Param_Y, m_Command.Param_Z);
			break;
		case do_revive:
			DoStand();
			m_ProcessAI = 1;
			m_ProcessState = 1;
#ifndef _SERVER
			this->SetInstantSpr(enumINSTANT_STATE_REVIVE);
#endif
			break;
		case do_walk:
			if (m_RandMove.nTime > 0)
				Goto(m_Command.Param_X, m_Command.Param_Y);
			break;
		default:
			break;
		}
	}
	m_Command.CmdKind = do_none;
}

BOOL KNpc::ProcessState()
{
	INT nRet = FALSE;
	if (m_RegionIndex < 0)
		return FALSE;

#ifdef _SERVER

	if (!(g_SubWorldSet.GetGameTime() % (GAME_FPS * 3))) //(FRAME2TIME *3)//Perform status synchronization every three seconds GAME_UPDATE_TIME));//
		// if (m_LoopFrames -(m_LoopFrames>>4<<4)==0)
	{													   // Find the remainder
		UpdateNpcStateInfo();
		BroadCastState();
	}
	//if (!(g_SubWorldSet.GetGameTime() % GAME_FPS)) // Execute once per second
	//{
	//	if (m_nNpcTimeout >= g_SubWorldSet.GetGameTime())
	//	{
	//		m_nNpcTimeout = -1;
	//		if (m_RegionIndex >= 0)
	//		{
	//			SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].RemoveNpc(m_Index);
	//			SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY, obj_npc);
	//			NpcSet.Remove(m_Index);
	//		}
	//	}
	//}

#endif

	if (m_CurrentIgnoreNegativeStateP > 0)
	{
		m_PoisonState.nTime = 0;
		m_FreezeState.nTime = 0;
		m_BurnState.nTime = 0;
		m_StunState.nTime = 0;
		m_FrozenAction.nTime = 0;
		m_RandMove.nTime = 0;
		m_LoseMana.nTime = 0;

		ClearStateSkillEffect(TRUE);
	}

	if (this != NULL && !(m_LoopFrames % GAME_UPDATE_TIME))
	{
		// ÉúÃü¡¢ÄÚÁ¦¡¢ÌåÁ¦±ä»¯Ö»ÓÉ·þÎñÆ÷¼ÆËã
#ifdef _SERVER
		// ´ò×øÖÐ
		if (m_Doing == do_sit)
		{
			INT nLifeAdd = m_CurrentLifeMax * 3 / 1000;
			if (nLifeAdd <= 0)
				nLifeAdd = 1;
			m_CurrentLife += nLifeAdd;
			if (m_CurrentLife > m_CurrentLifeMax)
				m_CurrentLife = m_CurrentLifeMax;

			INT nManaAdd = m_CurrentManaMax * 3 / 1000;
			if (nManaAdd <= 0)
				nManaAdd = 1;

			if (m_CurrentMana < 0)
				m_CurrentMana = 0;

			m_CurrentMana += nManaAdd;
			if (m_CurrentMana > m_CurrentManaMax)
				m_CurrentMana = m_CurrentManaMax;

			m_CurrentStamina += PlayerSet.m_cPlayerStamina.m_nSitAdd;
			if (m_CurrentStamina > m_CurrentStaminaMax)
				m_CurrentStamina = m_CurrentStaminaMax;
		}
		// ÉúÃü×ÔÈ»»Ø¸´
		if (m_StunState.nTime <= 0)
		{
			m_CurrentLife += m_CurrentLifeReplenish + (m_CurrentLifeReplenish * m_CurrentLifeReplenishPercent / MAX_PERCENT);
			if (m_CurrentLife > m_CurrentLifeMax)
				m_CurrentLife = m_CurrentLifeMax;
			// ÄÚÁ¦×ÔÈ»»Ø¸´
			m_CurrentMana += m_CurrentManaReplenish;
			if (m_CurrentMana > m_CurrentManaMax)
				m_CurrentMana = m_CurrentManaMax;

			// ÌåÁ¦×ÔÈ»»Ø¸´
			if (m_Doing == do_stand)
			{
				m_CurrentStamina += m_CurrentStaminaGain;
			}
			else
			{
				if (m_nPKFlag < enumPKMurder)
					m_CurrentStamina += m_CurrentStaminaGain / STAMINA_RECOVER_SCALE;
			}
			if (m_CurrentStamina > m_CurrentStaminaMax)
				m_CurrentStamina = m_CurrentStaminaMax;
		}
#endif
		// ¹â»·¼¼ÄÜ

		if (m_ActiveAuraID)
		{
			if (m_SkillList.GetLevel(m_ActiveAuraID) > 0)
			{
				INT nCurLevel = m_SkillList.GetCurrentLevel(m_ActiveAuraID);

				INT nMpsX, nMpsY;
				SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
				if (m_ActiveAuraID < MAX_SKILL && nCurLevel < MAX_SKILLLEVEL);
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_ActiveAuraID, nCurLevel);
#ifdef _SERVER
				NPC_SKILL_SYNC SkillCmd;
				SkillCmd.ID = this->m_dwID;
				if (this != NULL && pOrdinSkill)
				{
					//		if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles)
					SkillCmd.nSkillID = pOrdinSkill->GetChildSkillId();
				}
				else
				{
					SkillCmd.nSkillID = 0;
				}

				SkillCmd.nSkillLevel = nCurLevel;
				SkillCmd.nMpsX = -1;
				SkillCmd.nMpsY = m_dwID;
				SkillCmd.ProtocolType = s2c_castskilldirectly;
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
				CURREGION.BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX, m_MapY);
				INT i;
				for (i = 0; i < 8; i++)
				{
					if (CONREGIONIDX(i) == -1)
						continue;
					CONREGION(i).BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
				}
#endif				
				KSkill* pOrdinSkill1 = (KSkill*)g_SkillManager.GetSkill(m_ActiveAuraID, nCurLevel);
				INT nChildSkillId = 0;
				if (this != NULL && pOrdinSkill1)
				{
					nChildSkillId = pOrdinSkill1->GetChildSkillId();

					KSkill* pOrdinSkill2 = (KSkill*)g_SkillManager.GetSkill(nChildSkillId, nCurLevel);
					if (pOrdinSkill2)
					{
						pOrdinSkill2->Cast(m_Index, nMpsX, nMpsY);
					}
				}
				if (this != NULL && pOrdinSkill->GetAppendSkillNum())
				{
					for (INT j = 0; j < pOrdinSkill->GetAppendSkillNum(); j++)
					{
						INT nAppendId = pOrdinSkill->GetAppendSkillId(j);
						INT nAppendLv = m_SkillList.GetCurrentLevel(nAppendId);
						if (nAppendLv > nCurLevel)
							nAppendLv = nCurLevel;
						if (nAppendId < MAX_SKILL && nAppendLv > 0 && nAppendLv < MAX_SKILLLEVEL);
						{
#ifdef _SERVER
							KSkill* pOrdinSkill1 = (KSkill*)g_SkillManager.GetSkill(nAppendId, nAppendLv);
							if (pOrdinSkill1)
							{
								SkillCmd.nSkillID = pOrdinSkill1->GetChildSkillId();
							}
							else
							{
								SkillCmd.nSkillID = 0;
							}
							SkillCmd.nSkillLevel = nAppendLv;
							SkillCmd.nMpsX = -1;
							SkillCmd.nMpsY = m_dwID;
							SkillCmd.ProtocolType = s2c_castskilldirectly;

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
							CURREGION.BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX, m_MapY);
							INT i;
							for (i = 0; i < 8; i++)
							{
								if (CONREGIONIDX(i) == -1)
									continue;
								CONREGION(i).BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
							}
#endif				
							KSkill* pOrdinSkill2 = (KSkill*)g_SkillManager.GetSkill(nAppendId, nAppendLv);
							INT nChildSkillId = 0;
							if (pOrdinSkill2)
							{
								nChildSkillId = pOrdinSkill2->GetChildSkillId();

								KSkill* pOrdinSkill3 = (KSkill*)g_SkillManager.GetSkill(nChildSkillId, nAppendLv);
								if (pOrdinSkill3)
								{
									pOrdinSkill3->Cast(m_Index, nMpsX, nMpsY);
								}
							}
						}
					}
				}
			}
		}
	}
#ifdef _SERVER

	if (m_PoisonState.nTime > 0)
	{
		m_PoisonState.nTime--;
		if (m_PoisonState.nValue[1] == 0)
		{
			m_PoisonState.nValue[1] = 1;
		}
		if (!(m_PoisonState.nTime % m_PoisonState.nValue[1]))
		{
			CalcDamage(m_nLastPoisonDamageIdx, -1, m_PoisonState.nValue[0], m_PoisonState.nValue[0], damage_poison, FALSE, 0, TRUE);
		}
		if (m_PoisonState.nTime == 0)
		{
			m_PoisonState.nValue[0] = 0;
			m_PoisonState.nValue[1] = 0;
			m_PoisonState.nValue[2] = 0;
		}
	}
	// ±ù¶³×´Ì¬
	if (m_FreezeState.nTime > 0)
	{
		m_FreezeState.nTime--;
		if (m_FreezeState.nTime & 1)
		{
			nRet = TRUE;
		}
	}
	// È¼ÉÕ×´Ì¬
	if (m_BurnState.nTime > 0)
	{
		m_BurnState.nTime--;
		if (m_BurnState.nValue[1] == 0)
		{
			m_BurnState.nValue[1] = 1;
		}
		if (!(m_BurnState.nTime % m_BurnState.nValue[1]))
		{
			CalcDamage(m_Index, -1, m_BurnState.nValue[0], m_BurnState.nValue[0], damage_fire, TRUE, 0);
		}
	}

	if (m_FrozenAction.nTime > 0)
	{
		m_FrozenAction.nTime--;
	}
	// »ìÂÒ×´Ì¬
	if (m_RandMove.nTime > 0)
	{
		m_ProcessAI = 0;
		if (!(g_SubWorldSet.GetGameTime() % GAME_UPDATE_TIME))
		{
			INT nDesX, nDesY;
			GetMpsPos(&nDesX, &nDesY);
			//INT nRan = ::GetRandomNumber(0, 1);
			//if (nRan)
			//	nDesX -= g_Random(100);
			//else
			//	nDesX += g_Random(100);

			//nRan = ::GetRandomNumber(0, 1);
			//if (nRan)
			//	nDesY -= g_Random(100);
			//else
			//	nDesY += g_Random(100);
			SendCommand(do_walk, nDesX + _GetRandomNumber(0, 100), nDesY + _GetRandomNumber(0, 100));
		}
		m_RandMove.nTime--;
		if (m_RandMove.nTime == 0)
			m_ProcessAI = 1;
	}

	// Ñ£ÔÎ×´Ì¬
	if (m_StunState.nTime > 0)
	{
		m_StunState.nTime--;
		nRet = TRUE;
	}

	// ²¹Ñª×´Ì¬
	if (m_LifeState.nTime > 0)
	{
		m_LifeState.nTime--;
		if (!(m_LifeState.nTime % GAME_UPDATE_TIME))
		{
			m_CurrentLife += m_LifeState.nValue[0];
			if (m_CurrentLife > m_CurrentLifeMax)
			{
				m_CurrentLife = m_CurrentLifeMax;
			}
		}
	}
	// ²¹MANA×´Ì¬
	if (m_ManaState.nTime > 0)
	{
		m_ManaState.nTime--;
		if (!(m_ManaState.nTime % GAME_UPDATE_TIME))
		{
			m_CurrentMana += m_ManaState.nValue[0];
			if (m_CurrentMana > m_CurrentManaMax)
			{
				m_CurrentMana = m_CurrentManaMax;
			}
		}
	}
	// ×í¾Æ×´Ì¬
	if (m_LoseMana.nTime > 0)
	{
		m_LoseMana.nTime--;
		if (!(m_LoseMana.nTime % GAME_FPS))
		{
			m_CurrentMana -= m_LoseMana.nValue[0];
			if (m_CurrentMana < 0)
				m_CurrentMana = 0;
		}
	}
	if (m_HideState.nTime > 0)
	{
		m_HideState.nTime--;
	}
	if (m_SilentState.nTime > 0)
	{
		m_SilentState.nTime--;
	}
	if (m_WalkRun.nTime > 0)
	{
		m_WalkRun.nTime--;
	}
#endif

#ifndef _SERVER
	bool bAdjustColorId = false;

	if (m_FreezeState.nTime > 0)
	{
		if (SubWorld[0].m_dwCurrentTime & 1)
			nRet = TRUE;
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_freeze);
		bAdjustColorId = true;
	}

	/*if (m_Index == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
		if (m_RandMove.nTime > 0)
		{
			m_ProcessAI = 0;
			if (!(g_SubWorldSet.GetGameTime() % GAME_UPDATE_TIME))
			{
				INT nDesX, nDesY;
				GetMpsPos(&nDesX, &nDesY);
				INT nRan = ::GetRandomNumber(0, 1);
				if (nRan)
					nDesX -= g_Random(100);
				else
					nDesX += g_Random(100);

				nRan = ::GetRandomNumber(0, 1);
				if (nRan)
					nDesY -= g_Random(100);
				else
					nDesY += g_Random(100);
				SendCommand(do_walk, nDesX, nDesY);
			}
			m_RandMove.nTime--;
		}

		if (m_RandMove.nTime <= 0)
			m_ProcessAI = 1;
	}*/

	if (m_StunState.nTime > 0)
	{
		m_DataRes.SetSpecialSpr("\\spr\\skill\\²¹³ä\\mag_spe_Ñ£ÔÎ.spr");
		nRet = TRUE;
	}

	if (m_PoisonState.nTime > 0)
	{
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_poison);
		bAdjustColorId = true;
	}

	if (m_BurnState.nTime > 0)
	{
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_burn);
		bAdjustColorId = true;
	}

	if (!bAdjustColorId)
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_physics);
#endif

	KStateNode* pNode;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		KStateNode* pTempNode = pNode;
		pNode = (KStateNode*)pNode->GetNext();

		if (pTempNode->m_LeftTime == -1)	// ±»¶¯¼¼ÄÜ
			continue;

		if (pTempNode->m_LeftTime == 0)
		{
			INT i;
			for (i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pTempNode->m_State[i].nAttribType)
				{
					ModifyAttrib(m_Index, &pTempNode->m_State[i]);
				}
			}
			_ASSERT(pTempNode != NULL);
			pTempNode->Remove();
			delete pTempNode;

			pTempNode = NULL;

#ifdef _SERVER
			UpdateNpcStateInfo();
#endif
			continue;
		}
		else
			pTempNode->m_LeftTime--;
	}

	return nRet;
}

#ifdef _SERVER
INT	KNpc::UpdateDBStateList(BYTE* pStateBuffer)
{
	if (!pStateBuffer)
		return -1;

	INT nCount = 0;

	TDBSkillData* pStateData = (TDBSkillData*)pStateBuffer;
	KStateNode* pNode;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		KStateNode* pTempNode = pNode;
		pNode = (KStateNode*)pNode->GetNext();

		if (pTempNode->m_SkillID > 0 && pTempNode->m_SkillID < MAX_SKILL &&
			pTempNode->m_Level > 0 && pTempNode->m_Level < MAX_SKILLLEVEL &&
			pTempNode->m_bOverLook)
		{
			pStateData->m_nSkillId = pTempNode->m_SkillID;
			pStateData->m_nSkillLevel = pTempNode->m_Level;
			pStateData->m_nSkillVal = pTempNode->m_LeftTime;
			pStateData++;
			nCount++;
		}
	}
	return nCount;
}
#endif

void KNpc::DoDeath(INT nMode/* = 0*/)
{
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (m_Doing == do_death)
		return;

	if (IsPlayer() && !m_FightMode)	// ³ÇÕòÄÚ²»»áËÀÍö
	{
		m_CurrentLife = 1;
		return;
	}

	ClearStateSkillEffect(TRUE);

#ifndef _SERVER
	if (this->m_Kind == kind_normal)
		this->AddBlood(this->m_CurrentLife);
#endif

#ifdef _SERVER
	if (IsPlayer() && nMode == enumDEATH_MODE_PLAYER_SPAR_NO_PUNISH)
	{
		Player[m_nPlayerIdx].Revive(LOCAL_REVIVE_TYPE);
		return;
	}
#endif

	m_Doing = do_death;
	m_ProcessAI = 0;
	m_ProcessState = 0;

	m_Frames.nTotalFrame = m_DeathFrame;
	m_Frames.nCurrentFrame = 0;

	m_Height = 0;

#ifdef _SERVER
	INT nPlayer = 0;
	// É±ËÀÍæ¼Ò²»µÃ¾­Ñé
	if (this->m_Kind != kind_player)
	{
		nPlayer = m_cDeathCalcExp.CalcExp();
	}

	//¶ªÎïÆ·
	DeathPunish(nMode, nPlayer);

	if (this->m_Kind == kind_normal)
	{
		if (m_DropScriptID && nPlayer) // drop nay cua Player là đúng rồi 
			Player[nPlayer].ExecuteScript(m_DropScriptID, "DropRate", m_Index);

		if (m_ActionScriptID) // cái này của NPC. thì tạo riêng ra rồi gắn 
		{
			if (nPlayer)
				Player[nPlayer].ExecuteScript(m_ActionScriptID, "LastDamage", m_Index, SCRIPT_NPCID);
		}
	}
	else if (this->m_Kind == kind_player)
	{
		if (Player[m_nPlayerIdx].m_dwDeathScriptId)
			Player[m_nPlayerIdx].ExecuteScript(Player[m_nPlayerIdx].m_dwDeathScriptId, "OnDeath", m_nLastDamageIdx);
	}

	NPC_DEATH_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcdeath;
	NetCommand.ID = m_dwID;

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
	//if (CURREGION.m_PlayerList.GetHead())
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		//if (CONREGION(i).m_PlayerList.GetHead())
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
#ifndef _SERVER
	m_ClientDoing = cdo_death;
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx == m_Index)
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
	}

	this->m_cGold.ClientClearState(); // 删除黄金怪状态

	if (IsPlayer())
	{ // 自动离队
		if (Player[m_nPlayerIdx].m_cTeam.m_nFlag)
		{
			Player[m_nPlayerIdx].LeaveTeam();
		}
	}

#else
	if (this->m_Kind == kind_player)
	{
		if (Npc[Player[m_nPlayerIdx].m_nIndex].m_bReviveNow)
			return;

		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		if (m_nLastDamageIdx && Npc[m_nLastDamageIdx].m_Kind == kind_player)
		{
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
			sMsg.m_lpBuf = (LPVOID)Npc[m_nLastDamageIdx].m_dwID;
			sMsg.m_wMsgID = enumMSG_ID_NPC_RENASCENCE_SOMEONE;
		}
		else
		{
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
			sMsg.m_wMsgID = enumMSG_ID_NPC_RENASCENCE;
		}
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	}
#endif
}

void KNpc::OnDeath()
{
	if (WaitForFrame())
	{
		m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;		// ±£Ö¤²»»áÓÐÖØ»ØµÚÒ»Ö¡µÄÇé¿ö
#ifndef _SERVER
		if (!IsPlayer())
		{
			INT		nTempX, nTempY;
			KObjItemInfo	sInfo;

			SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nTempX, &nTempY);
			sInfo.m_nItemID = 0;
			sInfo.m_nItemWidth = 0;
			sInfo.m_nItemHeight = 0;
			sInfo.m_nMoneyNum = 0;
			sInfo.m_nColorID = 0;
			sInfo.m_nGenre = 0;
			sInfo.m_nDetailType = 0;
			sInfo.m_nMovieFlag = 0;
			sInfo.m_nSoundFlag = 0;
			sInfo.m_bOverLook = FALSE;
			sInfo.m_szName[0] = 0;
			ObjSet.ClientAdd(0, m_CorpseSettingIdx, 0, m_Dir, 0, nTempX, nTempY, sInfo);
			m_ProcessAI = 1;
		}
#endif

		// ÖØÉúµã
		if (m_Kind != kind_partner)//Õ½¶·NpcÊ±
		{
			DoRevive();
#ifdef _SERVER
			if (this->m_Kind == kind_normal)
				NpcSet.ExecuteScript(m_Index, m_ActionScriptID, "DeathSelf", m_Index);
			else if (this->m_Kind == kind_player && Npc[Player[m_nPlayerIdx].m_nIndex].m_bReviveNow)
				Player[m_nPlayerIdx].Revive(REMOTE_REVIVE_TYPE);
#else
			// ¿Í»§¶Ë°ÑNPCÉ¾³ý
			if (m_Kind != kind_player)
			{
				SubWorld[0].m_WorldMessage.Send(GWM_NPC_DEL, m_Index);
				return;
			}
#endif		
		}
		else	// Í¬°éÀà£¿ÒÔºóÔÙËµ°É
		{
			// ÒÔºóÔÙËµNot Finish
		}
	}
	else
	{
	}
}

void KNpc::DoDefense()
{
	m_ProcessAI = 0;
}

void KNpc::OnDefense()
{
}

void KNpc::DoIdle()
{
	if (m_Doing == do_idle)
		return;
	m_Doing = do_idle;
}

void KNpc::OnIdle()
{
}

void KNpc::DoHurt(INT nHurtFrames, INT nX, INT nY)
{
	_ASSERT(m_RegionIndex >= 0);
#ifndef _SERVER
	m_DataRes.SetBlur(FALSE);
#endif
	if (m_RegionIndex < 0)
		return;
	if (m_Doing == do_hurt || m_Doing == do_death)
		return;

	// ÊÜ»÷»Ø¸´ËÙ¶ÈÒÑ¾­´ïµ½100%ÁË£¬²»×öÊÜÉË¶¯×÷
#ifdef _SERVER
	if (m_CurrentHitRecover >= MAX_PERCENT)
		return;
#endif

	m_Doing = do_hurt;
	m_ProcessAI = 0;

#ifdef _SERVER
	m_Frames.nTotalFrame = m_HurtFrame * (MAX_PERCENT - m_CurrentHitRecover) / MAX_PERCENT;
#else
	m_ClientDoing = cdo_hurt;
	m_Frames.nTotalFrame = nHurtFrames;
	m_nHurtDesX = nX;
	m_nHurtDesY = nY;
	if (m_Height > 0)
	{
		// ÁÙÊ±¼ÇÂ¼ÏÂÀ´×öÎª¸ß¶È±ä»¯£¬ÔÚOnHurtÖÐÊ¹ÓÃ
		m_nHurtHeight = m_Height;
	}
	else
	{
		m_nHurtHeight = 0;
	}
#endif
	if (m_Frames.nTotalFrame <= 0)
		m_Frames.nTotalFrame = 1;
	m_Frames.nCurrentFrame = 0;

#ifdef _SERVER	// ÏòÖÜÎ§9¸öRegion¹ã²¥·¢¼¼ÄÜ
	NPC_HURT_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npchurt;
	NetCommand.ID = m_dwID;
	NetCommand.nFrames = m_Frames.nTotalFrame;
	GetMpsPos(&NetCommand.nX, &NetCommand.nY);

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
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

void KNpc::OnHurt()
{
	if (m_RegionIndex < 0)
	{
		return;
	}
	INT nX, nY, nMap;
	GetMpsPos(&nX, &nY);
#ifdef _SERVER
	m_Height = 0;
#endif
#ifndef _SERVER

	_ASSERT(m_Frames.nTotalFrame > 0);

	m_Height = m_nHurtHeight * (m_Frames.nTotalFrame - m_Frames.nCurrentFrame - 1) / m_Frames.nTotalFrame;
	nX = nX + (m_nHurtDesX - nX) * m_Frames.nCurrentFrame / m_Frames.nTotalFrame;
	nY = nY + (m_nHurtDesY - nY) * m_Frames.nCurrentFrame / m_Frames.nTotalFrame;

	INT nOldRegion = m_RegionIndex;
	// SetPos(nX, nY);
	CURREGION.DecNpcRef(m_MapX, m_MapY);

	INT nRegion, nMapX, nMapY, nOffX, nOffY;
	nRegion = -1;
	nMapX = nMapY = nOffX = nOffY = 0;
	SubWorld[m_SubWorldIndex].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion == -1)
	{
		SubWorld[0].m_Region[nOldRegion].RemoveNpc(m_Index);
		m_dwRegionID = 0;
	}
	else if (nOldRegion != nRegion || nRegion < 0)
	{
		m_RegionIndex = nRegion;
		m_MapX = nMapX;
		m_MapY = nMapY;
		m_OffX = nOffX;
		m_OffY = nOffY;
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
	}
	if (nRegion >= 0)
		CURREGION.AddNpcRef(m_MapX, m_MapY);
	// CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
	// SubWorld[m_SubWorldIndex].m_Region[nRegion].AddNpcRef(m_MapX,m_MapY);
#endif

	if (WaitForFrame()) // 等待的帧数
	{
		//	g_DebugLog("[DEATH]On Hurt Finished");                 //受到伤害的动作
		if (m_Kind == kind_player)
		{
#ifdef _SERVER
			Player[m_nPlayerIdx].UpdataCurData(); // UpdataCurData();                 //更新自身的属性参数
#else
			Player[CLIENT_PLAYER_INDEX].UpdataCurData();
#endif
		}
		DoStand();
		m_ProcessAI = 1; // 完成受伤动作
	}
}

void KNpc::DoSpecial1()
{
	DoBlurAttack();
}

void KNpc::OnSpecial1()
{
	if (WaitForFrame() && m_Frames.nTotalFrame != 0)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		KSkill* pSkill = (KSkill*)GetActiveSkill();
		if (pSkill)
		{
			INT nChildSkill = pSkill->GetChildSkillId();
			INT nChildSkillLevel = pSkill->m_ulLevel;

			if (nChildSkill > 0)
			{
				KSkill* pChildSkill = (KSkill*)g_SkillManager.GetSkill(nChildSkill, nChildSkillLevel);
				if (pChildSkill)
				{
					pChildSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
				}
			}
		}

		if (m_Frames.nTotalFrame <= 0)
		{
			m_ProcessAI = 1;
		}
	}
}

void KNpc::DoSpecial2()
{
}

void KNpc::OnSpecial2()
{
	if (WaitForFrame() && m_Frames.nTotalFrame != 0)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		ISkill* pSkill = GetActiveSkill();
		eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
		switch (eStyle)
		{
		case SKILL_SS_Thief:
		{
			((KThiefSkill*)pSkill)->OnSkill(this);
		}
		break;
		}

		if (m_Frames.nTotalFrame <= 0)
		{
			m_ProcessAI = 1;
		}
	}

}

void KNpc::DoSpecial3()
{
}

void KNpc::OnSpecial3()
{
}

void KNpc::DoStand()
{
#ifndef _SERVER
	m_DataRes.SetBlur(FALSE);
#endif
	m_Frames.nTotalFrame = m_StandFrame;
	if (m_Doing == do_stand)
	{
		return;
	}

	FixPos();
	m_Doing = do_stand;
	GetMpsPos(&m_DesX, &m_DesY);

	//m_Frames.nCurrentFrame = 0;
#ifndef _SERVER
	if (m_FightMode)
		m_ClientDoing = cdo_fightstand;
	else if (g_Random(6) != 1)
		m_ClientDoing = cdo_stand;
	else
		m_ClientDoing = cdo_stand1;

	m_DataRes.StopSound();
#endif

}

void KNpc::OnStand()
{
	if (WaitForFrame())
	{
#ifndef _SERVER
		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightstand;
		}
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}
#endif
	}
}

void KNpc::DoRevive()
{
	if (m_RegionIndex < 0)
	{
		g_DebugLog("[error]%s Region Index < 0 when dorevive", Name);
		return;
	}
#ifndef _SERVER
	m_DataRes.SetBlur(FALSE);
#endif
	if (m_Doing == do_revive)
	{
		return;
	}
	else
	{
		m_Doing = do_revive;
		m_ProcessAI = 0;
		m_ProcessState = 0;

		ClearStateSkillEffect();
		ClearNormalState();

#ifdef _SERVER
		if (IsPlayer())
		{
			return;
		}
		m_Frames.nTotalFrame = m_ReviveFrame;
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
		SubWorld[m_SubWorldIndex].NpcChangeRegion(m_RegionIndex, VOID_REGION, m_Index);	// spe 03/06/28
		m_Frames.nCurrentFrame = 0;
#else
		m_Frames.nTotalFrame = m_DeathFrame;
		m_ClientDoing = cdo_death;
#endif
	}
}

void KNpc::OnRevive()
{
#ifdef _SERVER
	if (!IsPlayer() && WaitForFrame())
	{
		Revive();
	}
#else	// ¿Í»§¶Ë
	m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
#endif
}

void KNpc::DoRun()
{
	if (m_SubWorldIndex < 0 || m_SubWorldIndex >= MAX_SUBWORLD || m_RegionIndex < 0)
		return;

	_ASSERT(m_RegionIndex >= 0);

	if (m_CurrentRunSpeed) // Current running speed m_RunSpeed ​​original running speed m_CurrentRunSpeed ​​+= m_RunSpeed*percentage
	{
		if (m_CurrentRunSpeed < 10)
			m_CurrentRunSpeed = 10;

		if (m_CurrentRunSpeed > m_RunFrame * m_RunSpeed)
			m_Frames.nTotalFrame = m_RunFrame;
		else
			m_Frames.nTotalFrame = (m_RunFrame * m_RunSpeed) / m_CurrentRunSpeed; // 15*跑速百分比
	}
	else
		m_Frames.nTotalFrame = m_RunFrame;

#ifndef _SERVER
	if (m_FightMode)
	{
		m_ClientDoing = cdo_fightrun;
	}
	else
	{
		m_ClientDoing = cdo_run;
	}
#endif

#ifdef _SERVER

	NPC_RUN_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcrun;
	NetCommand.ID = m_dwID;
	NetCommand.nMpsX = m_DesX;
	NetCommand.nMpsY = m_DesY;

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
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

	if (m_Doing == do_run)
	{
		return;
	}

	m_Doing = do_run;

	m_Frames.nCurrentFrame = 0;
}

void KNpc::OnRun()
{
	if (m_Doing == do_hurt || m_Doing == do_death || m_Doing == do_revive)
		DoStand();

	WaitForFrame();

#ifndef _SERVER
	if (m_WalkRun.nTime)
		m_DataRes.SetBlur(TRUE);
#endif

#ifdef _SERVER
	if (!(m_LoopFrames % GAME_UPDATE_TIME))
	{
		if (Npc[Player[m_nPlayerIdx].m_nIndex].m_nCurPKPunishState == enumDEATH_MODE_TOURNAMENTS_PUNISH)
		{
		}
		else
		{
			switch (m_nPKFlag)
			{
			case enumPKMurder:
				m_CurrentStamina -= PlayerSet.m_cPlayerStamina.m_nKillRunSub;
				break;
			case enumPKTongWar:
				m_CurrentStamina -= PlayerSet.m_cPlayerStamina.m_nTongWarRunSub;
				break;
			}
		}
		if (m_CurrentStamina <= 0)
			m_CurrentStamina = 0;
	}
#endif

	if (m_CurrentStamina == 0) // 没有体力值
		ServerMove(m_CurrentWalkSpeed);
	else if (m_Doing == do_runattack)
	{
		m_CurrentRunSpeed += 50;
		ServerMove(m_CurrentRunSpeed);
		m_CurrentRunSpeed -= 50;
	}
	else
	{
		ServerMove(m_CurrentRunSpeed);
	}
}

void KNpc::DoSit()
{
	_ASSERT(m_RegionIndex >= 0);

	if (m_Doing == do_sit)
		return;

	m_Doing = do_sit;

#ifdef _SERVER	// ÏòÖÜÎ§9¸öRegion¹ã²¥·¢¼¼ÄÜ
	NPC_SIT_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcsit;
	NetCommand.ID = m_dwID;

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
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

#ifndef _SERVER
	m_ClientDoing = cdo_sit;
#endif


	m_Frames.nTotalFrame = m_SitFrame;
	m_Frames.nCurrentFrame = 0;

}

void KNpc::OnSit()
{
	// ÌåÁ¦»»ÄÚÁ¦£¨Ã»ÓÐÉè¶¨£©
	if (WaitForFrame())
	{
		m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
	}
}

void KNpc::DoSkill(INT nX, INT nY)
{
	_ASSERT(m_RegionIndex >= 0);

	if (m_SubWorldIndex < 0 || m_SubWorldIndex >= MAX_SUBWORLD || m_RegionIndex < 0)
		return;

	if (Player[m_nPlayerIdx].CheckTrading())
		return;

	if (m_Doing == do_skill || m_Doing == do_hurt)
		return;

	m_HideState.nTime = 0;

	if (IsPlayer())
	{
		if (!m_FightMode)
			return;
#ifdef _SERVER
		if (m_nPlayerIdx > 0)
			Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeAttack);
#endif
	}

#ifdef _SERVER
	if (m_RandMove.nValue[0] && m_RandMove.nTime > 0)//Unable to attack in a chaotic state
	{
		return;
	}
#else
	if (m_RandMove.nTime > 0) // 混乱状态不能发招
		return;
#endif

	// ·ÇÕ½¶·×´Ì¬²»ÄÜ·¢¼¼ÄÜ
	ISkill* pSkill = GetActiveSkill();
	if (pSkill)
	{
		eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();

		if (m_SkillList.GetNextCastTime(m_ActiveSkillID) > 0 && (m_SkillList.GetNextCastTime(m_ActiveSkillID) > SubWorld[m_SubWorldIndex].m_dwCurrentTime))
		{//Not defrosted yet
			return;
		}

		if (m_HorseType)
		{//Riding time limit
			if (m_SkillList.GetHorseNextCastTime(m_ActiveSkillID) > 0 && (m_SkillList.GetHorseNextCastTime(m_ActiveSkillID) > SubWorld[m_SubWorldIndex].m_dwCurrentTime))
			{
				return;
			}
		}

		if (m_SkillList.CanCast(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime) // Whether it is possible to send skills
			&& pSkill->CanCastSkill(m_Index, nX, nY) && (/*m_Kind != kind_player || */ Cost(pSkill->GetSkillCostType(), pSkill->GetSkillCost(this))))
		{
			/*----------------------------------------------------------------------------------
			When casting a skill, when the target object needs to be specified, the first parameter of the two parameters passed to Skill.Cast is -1, and the second parameter is Npc index.
			In S2C, the second parameter must be converted from Server's NpcIndex to NpcdwID.
			When C receives this instruction, it converts NpcdwID to the local NpcIndex.
			------------------------------------------------------------------------------------*/
			if (m_HorseType)
				m_SkillList.SetHorseNextCastTimeByIndex(m_ActiveSkListIndex, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCast(TRUE)); // 设置骑马使用限制时间
			else
			{
				m_SkillList.SetNextCastTimeByIndex(m_ActiveSkListIndex, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCast(FALSE)); // 发该技能的最小间阁时间
			}
#ifdef _SERVER	// ÏòÖÜÎ§9¸öRegion¹ã²¥·¢¼¼ÄÜ
			NPC_SKILL_SYNC	NetCommand;

			NetCommand.ProtocolType = (BYTE)s2c_skillcast;
			NetCommand.ID = m_dwID;
			NetCommand.nSkillID = m_ActiveSkillID;
			NetCommand.nSkillLevel = m_SkillList.GetCurrentLevel(m_ActiveSkillID);

			if (nY <= 0)
			{
				DoStand();
				return;
			}

			NetCommand.nMpsX = nX;
			if (nX == -1) //m_nDesX == -1 means attack someone whose id is DesY , and if m_nDesX == -2 means attack at somedir
			{
				NetCommand.nMpsY = Npc[nY].m_dwID;
				if (0 == NetCommand.nMpsY || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
					return;
			}
			else
			{
				NetCommand.nMpsY = nY;
			}

			m_SkillParam1 = nX;
			m_SkillParam2 = nY;
			m_DesX = nX;
			m_DesY = nY;

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
			CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
			INT i;
			for (i = 0; i < 8; i++)
			{
				if (CONREGIONIDX(i) == -1)
					continue;
				CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
			}
#endif		
			if (eStyle == SKILL_SS_Missles || eStyle == SKILL_SS_Melee || eStyle == SKILL_SS_InitiativeNpcState || eStyle == SKILL_SS_PassivityNpcState)
			{
				DoOrdinSkill((KSkill*)pSkill, nX, nY);
			}
			else
			{
				switch (eStyle)
				{
				case SKILL_SS_Thief:
				{
					((KThiefSkill*)pSkill)->DoSkill(this, nX, nY);

				}break;
				default:
					return;
				}
			}
		}
		else
		{
			m_nPeopleIdx = 0;
			m_nObjectIdx = 0;
			DoStand();
		}
	}
	else
	{
		_ASSERT(pSkill);
		return;
	}
}

INT KNpc::DoOrdinSkill(KSkill* pSkill, INT nX, INT nY)
{
	_ASSERT(pSkill);

#ifndef _SERVER		
	m_DataRes.StopSound();
	INT x, y, tx, ty;
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);

	if (nY < 0)
		return 0;

	if (nX < 0)
	{
		if (nX != -1)
			return 0;

		if (nY >= MAX_NPC || Npc[nY].m_dwID == 0 || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
			return 0;
		Npc[nY].GetMpsPos(&tx, &ty);
	}
	else
	{
		tx = nX;
		ty = nY;
	}

	m_SkillParam1 = nX;
	m_SkillParam2 = nY;
	m_DesX = nX;
	m_DesY = nY;

	m_Dir = g_GetDirIndex(x, y, tx, ty);
	if (pSkill->GetPreCastEffectFile()[0])
		m_DataRes.SetSpecialSpr((char*)pSkill->GetPreCastEffectFile());

	if (IsPlayer())
		pSkill->PlayPreCastSound(m_nSex, x, y);

	if (pSkill->IsNeedShadow())
		m_DataRes.SetBlur(TRUE);
	else
		m_DataRes.SetBlur(FALSE);
#endif

	CLIENTACTION ClientDoing = pSkill->GetActionType();

#ifndef _SERVER
	if (ClientDoing >= cdo_count)
		m_ClientDoing = cdo_magic;
	else if (ClientDoing != cdo_none)
		m_ClientDoing = ClientDoing;
#endif
	if (pSkill->GetSkillStyle() == SKILL_SS_Melee)
	{
		if (CastMeleeSkill(pSkill) == FALSE)
		{
			m_nPeopleIdx = 0;
			m_nObjectIdx = 0;
			m_ProcessAI = 1;
			DoStand();

			return 1;
		}
		if (!pSkill->IsAura())
		{
			DWORD dwCastTime = 0;
			eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
			if (eStyle == SKILL_SS_Missles
				|| eStyle == SKILL_SS_Melee
				|| eStyle == SKILL_SS_InitiativeNpcState
				|| eStyle == SKILL_SS_PassivityNpcState)
			{
				dwCastTime = pSkill->GetDelayPerCast(m_bRideHorse);
			}
			else
			{
				switch (eStyle)
				{
				case SKILL_SS_Thief:
				{
					dwCastTime = ((KThiefSkill*)pSkill)->GetDelayPerCast();
				}break;
				}
			}
			m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime, SubWorld[m_SubWorldIndex].m_dwCurrentTime + dwCastTime);
		}
	}
	//ÎïÀí¼¼ÄÜµÄ¼¼ÄÜÊÍ·ÅÊ±¼äÓëÆÕÍ¨¼¼ÄÜ²»Í¬£¬Ò»¸öÊÇAttackFrame,Ò»¸öÊÇCastFrame
	else if (pSkill->IsPhysical())
	{
		if (ClientDoing == cdo_none)
			m_Frames.nTotalFrame = 0;
		else
		{
			INT nTotalFrame = m_AttackFrame * MAX_PERCENT / (m_CurrentAttackSpeed + MAX_PERCENT);
			m_Frames.nTotalFrame = nTotalFrame - nTotalFrame % 2;
			if (m_Frames.nTotalFrame <= 0)
				m_Frames.nTotalFrame = 1;
		}

#ifndef _SERVER
		if (g_Random(3))
			m_ClientDoing = cdo_attack;
		else
			m_ClientDoing = cdo_attack1;
#endif
		m_Doing = do_attack;
	}
	else
	{
		if (ClientDoing == cdo_none)
			m_Frames.nTotalFrame = 0;
		else
		{
			INT nTotalFrame = m_CastFrame * MAX_PERCENT / (m_CurrentCastSpeed + MAX_PERCENT);
			m_Frames.nTotalFrame = nTotalFrame - nTotalFrame % 2;
			if (m_Frames.nTotalFrame <= 0)
				m_Frames.nTotalFrame = 1;
		}
		m_Doing = do_magic;
	}
	m_ProcessAI = 0;
	m_Frames.nCurrentFrame = 0;
	return 1;
}

BOOL	KNpc::CastMeleeSkill(KSkill* pSkill)
{
	BOOL bSuceess = FALSE;
	_ASSERT(pSkill);

	switch (pSkill->GetMeleeType())
	{
	case Melee_AttackWithBlur:
	{
		bSuceess = DoBlurAttack();
	}break;
	case Melee_Jump:
	{
		if (NewJump(m_DesX, m_DesY))
		{
			DoJump();
			bSuceess = TRUE;
		}

	}break;
	case Melee_JumpAndAttack:
	{
		if (m_DesX < 0 && m_DesY > 0)
		{
			INT x, y;
			SubWorld[m_SubWorldIndex].Map2Mps
			(
				Npc[m_DesY].m_RegionIndex,
				Npc[m_DesY].m_MapX,
				Npc[m_DesY].m_MapY,
				Npc[m_DesY].m_OffX,
				Npc[m_DesY].m_OffY,
				&x,
				&y
			);

			m_DesX = x + 1;
			m_DesY = y;
		}

		if (NewJump(m_DesX, m_DesY))
		{
			DoJumpAttack();
			bSuceess = TRUE;
		}

	}break;
	case Melee_RunAndAttack:
	{
		bSuceess = DoRunAttack();

	}break;
	case Melee_ManyAttack:
	{
		bSuceess = DoManyAttack();
	}break;
	case Melee_MoveWithBlur:
	{
		m_SkillParam1 = pSkill->GetParam1();
		bSuceess = DoBlurMove();
	}break;
	default:
		m_ProcessAI = 1;
		break;
	}
	return bSuceess;

}

BOOL KNpc::DoBlurAttack()// DoSpecail1
{
	if (m_Doing == do_special1)
		return FALSE;

	KSkill* pSkill = (KSkill*)GetActiveSkill();
	if (!pSkill)
		return FALSE;

	_ASSERT(pSkill->GetSkillStyle() == SKILL_SS_Melee);

#ifndef _SERVER
	m_ClientDoing = pSkill->GetActionType();
	m_DataRes.SetBlur(TRUE);
#endif

	m_Frames.nTotalFrame = m_AttackFrame * MAX_PERCENT / (m_CurrentAttackSpeed + MAX_PERCENT);
	m_Frames.nCurrentFrame = 0;
	m_Doing = do_special1;
	return TRUE;
}

void KNpc::OnSkill()
{
	if (WaitForFrame() && m_Frames.nTotalFrame != 0)
	{
		DoStand();
		m_ProcessAI = 1;
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		KSkill* pSkill = NULL;
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif

		if (m_DesX == -1)
		{
			if (m_DesY <= 0)
				goto Label_ProcessAI;

			//´ËÊ±¸Ã½ÇÉ«ÒÑ¾­ÎÞÐ§Ê±
			if (Npc[m_DesY].m_RegionIndex < 0)
				goto Label_ProcessAI;
		}

		pSkill = (KSkill*)GetActiveSkill();

		if (pSkill)
		{
			pSkill->Cast(m_Index, m_DesX, m_DesY);

			DWORD dwCastTime = 0;
			eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
			if (eStyle == SKILL_SS_Missles
				|| eStyle == SKILL_SS_Melee
				|| eStyle == SKILL_SS_InitiativeNpcState
				|| eStyle == SKILL_SS_PassivityNpcState)
			{
				dwCastTime = pSkill->GetDelayPerCast(m_bRideHorse);
			}
			else if (eStyle == SKILL_SS_Thief)
				dwCastTime = ((KThiefSkill*)pSkill)->GetDelayPerCast();

			m_SkillList.SetNextCastTime(m_ActiveSkillID, SubWorld[m_SubWorldIndex].m_dwCurrentTime, SubWorld[m_SubWorldIndex].m_dwCurrentTime + dwCastTime);
		}

	Label_ProcessAI:
		if (m_Frames.nTotalFrame <= 0)
		{

			m_ProcessAI = 1;
		}
	}
}

void KNpc::JumpTo(INT nMpsX, INT nMpsY)
{
	if (NewJump(nMpsX, nMpsY))
		DoJump();
	else
	{
		RunTo(nMpsX, nMpsY);
	}
}

void KNpc::RunTo(INT nMpsX, INT nMpsY)
{
	if (NewPath(nMpsX, nMpsY))
		DoRun();
}

void KNpc::Goto(INT nMpsX, INT nMpsY)
{
	if (NewPath(nMpsX, nMpsY))
		DoWalk();
}

void KNpc::DoWalk()
{
	_ASSERT(m_RegionIndex >= 0);

	if (m_CurrentWalkSpeed)
		m_Frames.nTotalFrame = (m_WalkFrame * m_WalkSpeed) / m_CurrentWalkSpeed + 1;
	else
		m_Frames.nTotalFrame = m_WalkFrame;

#ifdef _SERVER		// Server¶ËµÄ´úÂë
	NPC_WALK_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcwalk;
	NetCommand.ID = m_dwID;
	NetCommand.nMpsX = m_DesX;
	NetCommand.nMpsY = m_DesY;

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
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

	if (m_Doing == do_walk)
	{
		return;
	}
	m_Doing = do_walk;
	m_Frames.nCurrentFrame = 0;

#ifndef _SERVER
	if (m_FightMode)
	{
		m_ClientDoing = cdo_fightwalk;
	}
	else
	{
		m_ClientDoing = cdo_walk;
	}
#endif

}

void KNpc::OnWalk()
{
#ifndef	_SERVER
	if (m_WalkRun.nTime)
		m_DataRes.SetBlur(TRUE);
#endif
	WaitForFrame();
	ServerMove(m_CurrentWalkSpeed);
}

void KNpc::ModifyAttrib(INT nAttacker, void* pData)
{
	if (pData != NULL)
	{
		g_NpcAttribModify.ModifyAttrib(this, pData);
	}
}

#ifdef _SERVER
BOOL KNpc::CalcDamage(INT nAttacker, INT nMissleSeries, INT nMin, INT nMax, DAMAGE_TYPE nType, BOOL bIsMelee, BOOL bReturn /* = FALSE */, INT nFiveElements_DamageP /*0*/, INT nStolen_Life/*0*/, INT nStolen_Mana/*0*/, INT nStolen_Stamina/*0*/, BOOL bIsDS /*= FALSE*/, BOOL bIsFS /*= FALSE*/)
{
	if (m_Doing == do_death || m_Doing == do_revive)
	{
		Npc[nAttacker].m_nPeopleIdx = 0;
		m_cDeathCalcExp.Clear();
		return FALSE;
	}

	if (m_Doing == do_death || m_Doing == do_revive || m_RegionIndex < 0)
	{
		return FALSE;
	}

	if (nMin < 0)
		nMin = 0;

	if (nMax < 0)
		nMax = 0;

	if (nMin + nMax <= 0)
		return FALSE;

	INT nDamage = 0, nCurDamage = 0;
	if (m_Series == series_minus)
	{
		nDamage = 1;
	}
	else
	{
		INT	nRes = 0;
		INT nDamageRange = nMax - nMin;
		INT nFiveElements_ResistRate = 30;
		INT nFiveElements_EnhanceRate = 70;

		if (nDamageRange < 0)
		{
			nDamage = nMax + g_Random(-nDamageRange);
		}
		else
		{
			nDamage = nMin + g_Random(nMax - nMin);
		}

		if ((nMissleSeries == series_metal && m_Series == series_wood) ||
			(nMissleSeries == series_water && m_Series == series_fire) ||
			(nMissleSeries == series_wood && m_Series == series_earth) ||
			(nMissleSeries == series_fire && m_Series == series_metal) ||
			(nMissleSeries == series_earth && m_Series == series_water))
		{
			nRes -= nFiveElements_DamageP;
			nDamage -= (Npc[nAttacker].m_CurrentFiveElementsEnhance - m_CurrentFiveElementsResist);
			nDamage += nDamage * nFiveElements_EnhanceRate / MAX_PERCENT;
		}
		else if ((nMissleSeries == series_metal && m_Series == series_fire) ||
			(nMissleSeries == series_water && m_Series == series_earth) ||
			(nMissleSeries == series_wood && m_Series == series_metal) ||
			(nMissleSeries == series_fire && m_Series == series_water) ||
			(nMissleSeries == series_earth && m_Series == series_wood))
		{
			nRes += nFiveElements_DamageP;
			nDamage -= (m_CurrentFiveElementsResist - Npc[nAttacker].m_CurrentFiveElementsEnhance);
			nDamage -= nDamage * nFiveElements_ResistRate / MAX_PERCENT;
		}

		if (bReturn)
		{
			if (bIsDS)
				nDamage = nDamage * MAX_DEATLY_STRIKE_ENHANCEP / MAX_PERCENT;

			if (bIsFS)
				nDamage = m_CurrentLife / MAX_PERCENT * GetRandomNumber(MIN_FATALLY_STRIKE_ENHANCEP, MAX_FATALLY_STRIKE_ENHANCEP);

			if (this->m_Kind == kind_normal)
			{
				if (this->m_btSpecial)
					nDamage = nDamage * NpcSet.m_nNpcSpecialDamageRate / MAX_PERCENT;
			}
		}
		else
		{
			if (m_FightMode)
			{
				ReplySkill(nAttacker);

				if (m_CurrentLife < (m_CurrentLifeMax * LIFE_EXPLOSIVE / MAX_PERCENT))
					RescueSkill();
			}

			Npc[nAttacker].AttackSkill(m_Index);
		}

		switch (nType)
		{
		case damage_physics:
			nRes += m_CurrentPhysicsResist;
			if (nRes > m_CurrentPhysicsResistMax)
			{
				nRes = m_CurrentPhysicsResistMax;
			}

			nDamage -= m_PhysicsArmor.nValue[0];
			if (bIsMelee)
			{
				nMax = m_CurrentMeleeDmgRetPercent;
			}
			else
			{
				nMax = m_CurrentRangeDmgRetPercent;
			}
			break;
		case damage_cold:
			nRes += m_CurrentColdResist;
			if (nRes > m_CurrentColdResistMax)
			{
				nRes = m_CurrentColdResistMax;
			}

			nDamage -= m_ColdArmor.nValue[0];
			if (bIsMelee)
			{
				nMax = m_CurrentMeleeDmgRetPercent;
			}
			else
			{
				nMax = m_CurrentRangeDmgRetPercent;
			}
			break;
		case damage_fire:
			nRes += m_CurrentFireResist;
			if (nRes > m_CurrentFireResistMax)
			{
				nRes = m_CurrentFireResistMax;
			}

			nDamage -= m_FireArmor.nValue[0];
			if (bIsMelee)
			{
				nMax = m_CurrentMeleeDmgRetPercent;
			}
			else
			{
				nMax = m_CurrentRangeDmgRetPercent;
			}
			break;
		case damage_light:
			nRes += m_CurrentLightResist;
			if (nRes > m_CurrentLightResistMax)
			{
				nRes = m_CurrentLightResistMax;
			}

			nDamage -= m_LightArmor.nValue[0];
			if (bIsMelee)
			{
				nMax = m_CurrentMeleeDmgRetPercent;
			}
			else
			{
				nMax = m_CurrentRangeDmgRetPercent;
			}
			break;
		case damage_poison:
			nRes += m_CurrentPoisonResist;
			if (nRes > m_CurrentPoisonResistMax)
			{
				nRes = m_CurrentPoisonResistMax;
			}

			nDamage -= m_PoisonArmor.nValue[0];
			if (bIsMelee)
			{
				nMax = m_CurrentMeleeDmgRetPercent;
			}
			else
			{
				nMax = m_CurrentRangeDmgRetPercent;
			}
			m_nLastPoisonDamageIdx = nAttacker;
			break;
		case damage_magic:	//No magic damage, attack that ignores defense
			nRes = 0;
			break;
		default:
			nRes = 0;
			break;
		}

		if (!nDamage)
			return 0;

		if (nRes < MAX_RESIST)
			nDamage = nDamage * (MAX_RESIST - nRes) / MAX_RESIST;
		else
			nDamage = nDamage * (MAX_PERCENT - GetRandomNumber(96, 98)) / MAX_PERCENT;

		if (nDamage > 0)
		{
			Npc[nAttacker].m_CurrentLife += nDamage * nStolen_Life / MAX_PERCENT;
			if (Npc[nAttacker].m_CurrentLife > Npc[nAttacker].m_CurrentLifeMax)
				Npc[nAttacker].m_CurrentLife = Npc[nAttacker].m_CurrentLifeMax;

			Npc[nAttacker].m_CurrentMana += nDamage * nStolen_Mana / MAX_PERCENT;
			if (Npc[nAttacker].m_CurrentMana > Npc[nAttacker].m_CurrentManaMax)
				Npc[nAttacker].m_CurrentMana = Npc[nAttacker].m_CurrentManaMax;

			Npc[nAttacker].m_CurrentStamina += nDamage * nStolen_Stamina / MAX_PERCENT;
			if (Npc[nAttacker].m_CurrentStamina > Npc[nAttacker].m_CurrentStaminaMax)
				Npc[nAttacker].m_CurrentStamina = Npc[nAttacker].m_CurrentStaminaMax;

			m_CurrentMana += nDamage * m_CurrentDamage2Mana / MAX_PERCENT;
			if (m_CurrentMana > m_CurrentManaMax)
				m_CurrentMana = m_CurrentManaMax;
		}

		if (m_CurrentManaShield > 0 && nDamage >= m_CurrentManaShield && nType != damage_poison)
		{
			nDamage -= m_CurrentManaShield;
		}

		if (m_ManaShield.nValue[0] > 0 && nType != damage_poison)
		{
			INT nManaShieldP = m_ManaShield.nValue[0];

			if (nManaShieldP > MAX_PERCENT)
				nManaShieldP = MAX_PERCENT;

			INT nManaDamage = nDamage * nManaShieldP / MAX_PERCENT;
			m_CurrentMana -= nManaDamage;
			nCurDamage += nManaDamage;
			if (m_CurrentMana < 0)
			{
				nDamage -= m_CurrentMana;
				nCurDamage += m_CurrentMana;
				m_CurrentMana = 0;
			}
			else
			{
				nDamage -= nManaDamage;
			}
		}

		if (nAttacker > 0 && !bReturn)
		{
			INT nRetPercent = 0, nRetPoint = 0;
			if (nType == damage_poison)
			{
				nRetPercent = m_CurrentReturnResPercent;
				nRetPoint = m_CurrentPoisonDamageReturnPercent;
			}
			else if (bIsMelee)
			{
				nRetPercent = m_CurrentMeleeDmgRetPercent - Npc[nAttacker].m_CurrentReturnResPercent;
				nRetPoint = m_CurrentMeleeDmgRet;
			}
			else
			{
				nRetPercent = m_CurrentRangeDmgRetPercent - Npc[nAttacker].m_CurrentReturnResPercent;
				nRetPoint = m_CurrentRangeDmgRet;
			}

			nRetPoint += nDamage * nRetPercent / MAX_PERCENT;

			if (nRetPoint > 0)
				Npc[nAttacker].CalcDamage(m_Index, -1, nRetPoint, nRetPoint, damage_magic, FALSE, 0, TRUE);
		}

		if (this->m_Kind == kind_player && Npc[nAttacker].m_Kind == kind_player)
		{
			nDamage = nDamage * NpcSet.m_nPKDamageRate / MAX_PERCENT;
		}

		if (this->m_Kind == kind_normal)
		{
			if (this->m_btSpecial)
				nDamage = nDamage * NpcSet.m_nNpcSpecialDamageRate / MAX_PERCENT;
		}

		if (m_Kind != kind_player && Npc[nAttacker].m_Kind == kind_player && Npc[nAttacker].m_nPlayerIdx > 0)
		{
			m_cDeathCalcExp.AddDamage(Npc[nAttacker].m_nPlayerIdx, (m_CurrentLife - nDamage > 0 ? nDamage : m_CurrentLife));
		}
		m_nLastDamageIdx = nAttacker;
	}

	m_CurrentLife -= nDamage;
	nCurDamage += nDamage;
	if (m_CurrentLife <= 0)
	{
		nCurDamage += m_CurrentLife;
		if (m_Doing != do_death && m_Doing != do_revive)
		{
			if ((m_DeathSkill[0].nSkillId > 0 && m_DeathSkill[0].nSkillId < MAX_SKILL) && m_Level >= LEVEL_EXPLOSIVE)
				DeathSkill();

			INT nMode = DeathCalcPKValue(m_nLastDamageIdx);
			DoDeath(nMode);

			if (m_Kind == kind_player)
				Player[m_nPlayerIdx].m_cPK.CloseAll();
		}
	}
	if (nCurDamage > 0 && (this->m_Kind == kind_player) && (Npc[nAttacker].m_Kind == kind_player))
	{
		if (Player[Npc[nAttacker].m_nPlayerIdx].m_dwDamageScriptId)
			Player[m_nPlayerIdx].ExecuteScript(Player[m_nPlayerIdx].m_dwDamageScriptId, "OnDamage", nCurDamage);
	}
	return TRUE;
}

void KNpc::ReplySkill(INT nAttacker)
{
	if (!m_Index)
		return;

	if (m_Doing == do_death || m_Doing == do_revive)
		return;

	for (INT i = 0; i < MAX_AUTOSKILL; i++)
	{
		if (m_ReplySkill[i].nSkillId > 0 && m_ReplySkill[i].nSkillId < MAX_SKILL && m_ReplySkill[i].nSkillLevel > 0 && m_ReplySkill[i].nSkillLevel < MAX_SKILLLEVEL)
		{
			if (m_ReplySkill[i].dwNextCastTime < SubWorld[m_SubWorldIndex].m_dwCurrentTime)
			{
				if (g_RandPercent(m_ReplySkill[i].nRate))
				{
					this->Cast(m_ReplySkill[i].nSkillId, m_ReplySkill[i].nSkillLevel, nAttacker);
					m_ReplySkill[i].dwNextCastTime = SubWorld[m_SubWorldIndex].m_dwCurrentTime + m_ReplySkill[i].nWaitCastTime;
				}
			}
		}
	}
}

void KNpc::RescueSkill()
{
	if (!m_Index)
		return;

	if (m_Doing == do_death || m_Doing == do_revive)
		return;

	for (INT i = 0; i < MAX_AUTOSKILL; i++)
	{
		if (m_RescueSkill[i].nSkillId > 0 && m_RescueSkill[i].nSkillId < MAX_SKILL && m_RescueSkill[i].nSkillLevel > 0 && m_RescueSkill[i].nSkillLevel < MAX_SKILLLEVEL)
		{
			if (m_RescueSkill[i].dwNextCastTime < SubWorld[m_SubWorldIndex].m_dwCurrentTime)
			{
				if (g_RandPercent(m_RescueSkill[i].nRate))
				{
					this->Cast(m_RescueSkill[i].nSkillId, m_RescueSkill[i].nSkillLevel, m_Index);
					m_RescueSkill[i].dwNextCastTime = SubWorld[m_SubWorldIndex].m_dwCurrentTime + m_RescueSkill[i].nWaitCastTime;
				}
			}
		}
	}
}

void KNpc::AttackSkill(INT nLauncher)
{
	if (!m_Index || !Npc[nLauncher].m_Index)
		return;

	if (m_Doing == do_death || m_Doing == do_revive)
		return;

	if (Npc[nLauncher].m_Doing == do_death || Npc[nLauncher].m_Doing == do_revive)
		return;

	for (INT i = 0; i < MAX_AUTOSKILL; i++)
	{
		if (m_AttackSkill[i].nSkillId > 0 && m_AttackSkill[i].nSkillId < MAX_SKILL && m_AttackSkill[i].nSkillLevel > 0 && m_AttackSkill[i].nSkillLevel < MAX_SKILLLEVEL)
		{
			if (m_AttackSkill[i].dwNextCastTime < SubWorld[m_SubWorldIndex].m_dwCurrentTime)
			{
				if (g_RandPercent(m_AttackSkill[i].nRate))
				{

					KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(m_AttackSkill[i].nSkillId, m_AttackSkill[i].nSkillLevel);
					if (pSkill)
					{
						if (pSkill->Cast(m_Index, -1, nLauncher))
						{
							this->Cast(m_AttackSkill[i].nSkillId, m_AttackSkill[i].nSkillLevel, nLauncher);
							m_AttackSkill[i].dwNextCastTime += SubWorld[m_SubWorldIndex].m_dwCurrentTime;
						}
					}
				}
			}
		}
	}
}

void KNpc::CastSkill()
{
	if (m_Doing == do_death || m_Doing == do_revive)
		return;

	for (INT i = 0; i < MAX_AUTOSKILL; i++)
	{
		if (m_CastSkill[i].nSkillId > 0 && m_CastSkill[i].nSkillId < MAX_SKILL && m_CastSkill[i].nSkillLevel > 0 && m_CastSkill[i].nSkillLevel < MAX_SKILLLEVEL)
		{
			if (m_CastSkill[i].dwNextCastTime < SubWorld[m_SubWorldIndex].m_dwCurrentTime)
			{
				//if (g_RandPercent(m_CastSkill[i].nRate))
				//{
				KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(m_CastSkill[i].nSkillId, m_CastSkill[i].nSkillLevel);
				if (pSkill)
					pSkill->Cast(m_Index, -1, 0);
				m_CastSkill[i].dwNextCastTime = SubWorld[m_SubWorldIndex].m_dwCurrentTime + m_CastSkill[i].nWaitCastTime;
				//}
			}
		}
	}
}

void KNpc::DeathSkill()
{
	if (!m_Index)
		return;

	if (m_Doing == do_death || m_Doing == do_revive)
		return;

	for (INT i = 0; i < MAX_AUTOSKILL; i++)
	{
		if (m_DeathSkill[i].nSkillId > 0 && m_ReplySkill[i].nSkillId < MAX_SKILL &&
			m_DeathSkill[i].nSkillLevel > 0 && m_DeathSkill[i].nSkillLevel < MAX_SKILLLEVEL)
		{
			if (m_DeathSkill[i].dwNextCastTime < SubWorld[m_SubWorldIndex].m_dwCurrentTime)
			{
				if (g_RandPercent(m_DeathSkill[i].nRate))
				{
					this->Cast(m_DeathSkill[i].nSkillId, m_DeathSkill[i].nSkillLevel, m_Index);
					m_DeathSkill[i].dwNextCastTime = SubWorld[m_SubWorldIndex].m_dwCurrentTime + m_DeathSkill[i].nWaitCastTime;
				}
			}
		}
	}
}
#endif

#ifdef _SERVER
BOOL KNpc::ReceiveDamage(INT nLauncher, INT nMissleSeries, BOOL bIsMelee, void* pData, BOOL bUseAR, INT nDoHurtP, INT nMissRate)
{
	if (!m_Index || !Npc[nLauncher].m_Index)
		return FALSE;

	if (!pData)
		return FALSE;

	if (m_Doing == do_death || m_Doing == do_revive)
		return TRUE;

	if (Npc[nLauncher].m_Doing == do_death || Npc[nLauncher].m_Doing == do_revive)
		return TRUE;

	INT nRdc;
	KMagicAttrib* pTemp = NULL;

	pTemp = (KMagicAttrib*)pData;

	INT nAr = pTemp->nValue[0];
	pTemp++;
	INT nIgnoreAr = pTemp->nValue[0];

	if (bUseAR)
	{
		if (!CheckHitTarget(nAr, m_CurrentDefend, nIgnoreAr))
			return FALSE;
	}

	if (Npc[nLauncher].IsPlayer() && Npc[nLauncher].m_FightMode)
	{
		if (Npc[nLauncher].m_ActiveSkillID > 0 && Npc[nLauncher].m_ActiveSkillID < MAX_SKILL)
		{
			if (Npc[nLauncher].m_SkillList.GetLevel(Npc[nLauncher].m_ActiveSkillID) < MAX_TRAIN_SKILLEXPLEVEL)
			{
				KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(Npc[nLauncher].m_ActiveSkillID, 1);
				if (pSkill->IsExp() && pSkill->IsTargetEnemy())
				{
					//INT nNextExp = Npc[nLauncher].m_SkillList.GetNextExp(Npc[nLauncher].m_ActiveSkillID);

					if (Npc[nLauncher].m_SkillList.IncreaseExp(Npc[nLauncher].m_SkillList.GetSkillIdx(Npc[nLauncher].m_ActiveSkillID), 5))//1 * g_SkillExpRate))
						Player[Npc[nLauncher].m_nPlayerIdx].UpdataCurData();

					PLAYER_SKILL_LEVEL_SYNC NewSkill;
					NewSkill.ProtocolType = s2c_playerskilllevel;
					NewSkill.m_nSkillID = Npc[nLauncher].m_ActiveSkillID;
					NewSkill.m_nSkillLevel = Npc[nLauncher].m_SkillList.GetLevel(Npc[nLauncher].m_ActiveSkillID);
					NewSkill.m_nAddLevel = Npc[nLauncher].m_SkillList.GetAddLevel(Npc[nLauncher].m_ActiveSkillID);
					NewSkill.m_nSkillExp = Npc[nLauncher].m_SkillList.GetExp(Npc[nLauncher].m_ActiveSkillID);
					NewSkill.m_bTempSkill = Npc[nLauncher].m_SkillList.IsTempSkill(Npc[nLauncher].m_ActiveSkillID);
					NewSkill.m_nLeavePoint = Player[Npc[nLauncher].m_nPlayerIdx].m_nSkillPoint;
					g_pServer->PackDataToClient(Player[Npc[nLauncher].m_nPlayerIdx].m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
				}
			}
		}
	}

	pTemp++;
	INT nMagicDamage = pTemp->nValue[0];

	pTemp++;
	INT nFiveElementsDamageP = pTemp->nValue[0];

	pTemp++;
	BOOL bIsDS = FALSE;
	if (g_RandPercent(pTemp->nValue[0]))
		bIsDS = TRUE;

	pTemp++;
	BOOL bIsFS = FALSE;
	if (g_RandPercent(pTemp->nValue[0] - m_CurrentFatallyStrikeResP))
		bIsFS = TRUE;

	pTemp++;
	INT nStolenLifeP = pTemp->nValue[0];

	pTemp++;
	INT nStolenManaP = pTemp->nValue[0];

	pTemp++;
	INT nStolenStaminaP = pTemp->nValue[0];

	pTemp++;
	CalcDamage(nLauncher, nMissleSeries, pTemp->nValue[0], pTemp->nValue[2], damage_physics, bIsMelee, FALSE, nFiveElementsDamageP, nStolenLifeP, nStolenManaP, nStolenStaminaP, bIsDS, bIsFS);

	pTemp++;
	if (CalcDamage(nLauncher, nMissleSeries, pTemp->nValue[0], pTemp->nValue[2], damage_cold, bIsMelee, FALSE, nFiveElementsDamageP, 0, 0, 0, FALSE, bIsFS))
	{
		nRdc = m_CurrentFreezeTimeReducePercent;
		if (nRdc > MAX_FREEZE_TIME_PERCENT_REDUCE)
			nRdc = MAX_FREEZE_TIME_PERCENT_REDUCE;

		if (m_FreezeState.nTime <= 0)
			m_FreezeState.nTime = pTemp->nValue[1] * (MAX_STATE_PERCENT - nRdc) / MAX_STATE_PERCENT;

		if (m_FreezeState.nTime > MAX_FREEZE_TIME)
			m_FreezeState.nTime = MAX_FREEZE_TIME;
	}
	pTemp++;
	CalcDamage(nLauncher, nMissleSeries, pTemp->nValue[0], pTemp->nValue[2], damage_fire, bIsMelee, FALSE, nFiveElementsDamageP, 0, 0, 0, FALSE, bIsFS);

	pTemp++;
	CalcDamage(nLauncher, nMissleSeries, pTemp->nValue[0], pTemp->nValue[2], damage_light, bIsMelee, FALSE, nFiveElementsDamageP, 0, 0, 0, FALSE, bIsFS);

	pTemp++;
	if (CalcDamage(nLauncher, nMissleSeries, pTemp->nValue[0], pTemp->nValue[2], damage_poison, bIsMelee, FALSE, nFiveElementsDamageP, 0, 0, 0, FALSE, bIsFS))
	{
		nRdc = m_CurrentPoisonTimeReducePercent;
		if (nRdc > MAX_POISON_TIME_PERCENT_REDUCE)	//MAX_PERCENT
			nRdc = MAX_POISON_TIME_PERCENT_REDUCE;	//MAX_RESIST

		if (m_PoisonState.nTime <= 0)
		{
			m_PoisonState.nTime = pTemp->nValue[1] * (MAX_STATE_PERCENT - nRdc) / MAX_STATE_PERCENT;
			m_PoisonState.nValue[0] = pTemp->nValue[0];
			m_PoisonState.nValue[1] = pTemp->nValue[2];
		}
		else
		{
			INT d1, d2, t1, t2, c1, c2;
			d1 = m_PoisonState.nValue[0];
			d2 = pTemp->nValue[0];
			t1 = m_PoisonState.nTime;
			t2 = pTemp->nValue[1] * (MAX_STATE_PERCENT - nRdc) / MAX_STATE_PERCENT;
			c1 = m_PoisonState.nValue[1];
			c2 = pTemp->nValue[2];
			if (c1 > 0 && c2 > 0 && d1 > 0 && d2 > 0)
			{
				m_PoisonState.nValue[0] = ((c1 + c2) * d1 / c1 + (c1 + c2) * d2 / c2) / 2;
				m_PoisonState.nTime = (t1 * d1 * c2 + t2 * d2 * c1) / (d1 * c2 + d2 * c1);
				m_PoisonState.nValue[1] = (c1 + c2) / 2;
			}
		}

		if (m_PoisonState.nTime > MAX_POISON_TIME)
			m_PoisonState.nTime = MAX_POISON_TIME;
	}
	pTemp++;
	if (m_StunState.nTime <= 0)
	{
		if (g_RandPercent(pTemp->nValue[0]))
		{
			nRdc = m_CurrentStunTimeReducePercent;
			if (nRdc > MAX_STUN_TIME_PERCENT_REDUCE)
				nRdc = MAX_STUN_TIME_PERCENT_REDUCE;

			m_StunState.nTime = pTemp->nValue[1] * (MAX_STATE_PERCENT - nRdc) / MAX_STATE_PERCENT;
		}

		if (m_StunState.nTime > MAX_STUN_TIME)
			m_StunState.nTime = MAX_STUN_TIME;
	}
	pTemp++;
	if (g_RandPercent(nMissRate))
	{
		this->ClearNormalState();
		this->IgnoreState(TRUE);
	}

	pTemp++;
	if (pTemp->nValue[0] && pTemp->nValue[1] && g_RandPercent(nMissRate))
		return FALSE;

	if (g_RandPercent(nDoHurtP))
		DoHurt();

	m_nPeopleIdx = nLauncher;
	return TRUE;
}
#endif

void KNpc::SetImmediatelySkillEffect(INT nLauncher, void* pData, INT nDataNum)
{
	if (!pData || !nDataNum)
		return;

	KMagicAttrib* pTemp = (KMagicAttrib*)pData;
	_ASSERT(nDataNum <= MAX_SKILL_STATE);
	for (INT i = 0; i < nDataNum; i++)
	{
		ModifyAttrib(nLauncher, pTemp);
		pTemp++;
	}
}

void KNpc::AppendSkillEffect(INT nSkillID, BOOL bIsPhysical, BOOL bIsMelee, void* pSrcData, void* pDesData)
{
	INT nMinDamage = m_PhysicsDamage.nValue[0] + m_CurrentAddPhysicsDamage;
	INT	nMaxDamage = m_PhysicsDamage.nValue[2] + m_CurrentAddPhysicsDamage;
	INT nAddDamageP = this->m_SkillList.GetAddSkillDamage(nSkillID) + this->m_CurrentSkillEnhancePercent;
	UINT nAddDamageV = this->m_SkillList.GetAddSkillDamageBonus(nSkillID, this->m_CurrentSkillEnhancePercent);
	//if (m_CurrentMana == m_CurrentManaMax)
	//	nAddDamageP += m_CurrentManaToSkillEnhanceP;

	KMagicAttrib* pTemp = (KMagicAttrib*)pSrcData;
	KMagicAttrib* pDes = (KMagicAttrib*)pDesData;
	if (pTemp->nAttribType == magic_attackrating_p)
	{
		pDes->nAttribType = magic_attackrating_v;
		pDes->nValue[0] = m_CurrentAttackRating + m_AttackRating * pTemp->nValue[0] / MAX_PERCENT;
	}
	else
	{
		pDes->nAttribType = magic_attackrating_v;
		pDes->nValue[0] = m_CurrentAttackRating;
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_ignoredefense_p)
	{
		pDes->nAttribType = magic_ignoredefense_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_magicdamage_v)
	{
		pDes->nAttribType = magic_magicdamage_v;
		pDes->nValue[0] = pTemp->nValue[0] + nAddDamageV;
		pDes->nValue[2] = pTemp->nValue[2] + nAddDamageV;
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_seriesdamage_p)
	{
		pDes->nAttribType = magic_seriesdamage_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_deadlystrike_p)
	{
		pDes->nAttribType = magic_deadlystrike_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	if (bIsPhysical)
		pDes->nValue[0] += m_CurrentDeadlyStrikeEnhanceP;
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_fatallystrike_p)
	{
		pDes->nAttribType = magic_fatallystrike_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	pDes->nValue[0] += m_CurrentFatallyStrikeEnhanceP;
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_steallife_p)
	{
		pDes->nAttribType = magic_steallife_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];

	}
	if (bIsPhysical)
		pDes->nValue[0] += m_CurrentLifeStolen;
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_stealmana_p)
	{
		pDes->nAttribType = magic_stealmana_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];

	}
	if (bIsPhysical)
		pDes->nValue[0] += m_CurrentManaStolen;
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_stealstamina_p)
	{
		pDes->nAttribType = magic_stealstamina_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];

	}
	if (bIsPhysical)
		pDes->nValue[0] += m_CurrentStaminaStolen;
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_physicsenhance_p)
	{
		pDes->nAttribType = magic_physicsdamage_v;
		pDes->nValue[0] = nMinDamage * (MAX_PERCENT + pTemp->nValue[0]) / MAX_PERCENT + nAddDamageV;
		pDes->nValue[2] = nMaxDamage * (MAX_PERCENT + pTemp->nValue[0]) / MAX_PERCENT + nAddDamageV;
		if (IsPlayer())
		{
			if (Player[m_nPlayerIdx].m_ItemList.GetWeaponType() == equip_meleeweapon)
			{
				pDes->nValue[0] += nMinDamage + ((nMinDamage * m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()]) / MAX_PERCENT);
				pDes->nValue[2] += nMaxDamage + ((nMaxDamage * m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()]) / MAX_PERCENT);

				g_DebugLog("CurrentPhysicP: %d; AddSkillDamageV: %d, MagicPhysic: %d; BaseDamage:[%d-%d]\n", m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()], nAddDamageV, pTemp->nValue[0], nMinDamage, nMaxDamage);
			}
			else if (Player[m_nPlayerIdx].m_ItemList.GetWeaponType() == equip_rangeweapon)
			{
				pDes->nValue[0] += nMinDamage + ((nMinDamage * m_CurrentRangeEnhance) / MAX_PERCENT);
				pDes->nValue[2] += nMaxDamage + ((nMaxDamage * m_CurrentRangeEnhance) / MAX_PERCENT);

				g_DebugLog("CurrentPhysicP: %d; AddSkillDamageV: %d, MagicPhysic: %d; BaseDamage:[%d-%d]\n", m_CurrentRangeEnhance, nAddDamageV, pTemp->nValue[0], nMinDamage, nMaxDamage);
			}
			else	// ¿ÕÊÖ
			{
				pDes->nValue[0] += nMinDamage + ((nMinDamage * m_CurrentHandEnhance) / MAX_PERCENT);
				pDes->nValue[2] += nMaxDamage + ((nMaxDamage * m_CurrentHandEnhance) / MAX_PERCENT);

				g_DebugLog("CurrentPhysicP: %d; AddSkillDamageV: %d, MagicPhysic: %d; BaseDamage:[%d-%d]\n", m_CurrentHandEnhance, nAddDamageV, pTemp->nValue[0], nMinDamage, nMaxDamage);
			}
		}
	}
	else if (pTemp->nAttribType == magic_physicsdamage_v)
	{
		pDes->nAttribType = magic_physicsdamage_v;
		pDes->nValue[0] = pTemp->nValue[0] + nAddDamageV;
		pDes->nValue[2] = pTemp->nValue[2] + nAddDamageV;

		if (!bIsPhysical)
		{
			pDes->nValue[0] += m_PhysicsMagic.nValue[0] + m_CurrentAddPhysicsMagic;
			pDes->nValue[2] += m_PhysicsMagic.nValue[2] + m_CurrentAddPhysicsMagic;
		}
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_colddamage_v)
	{
		pDes->nAttribType = magic_colddamage_v;
		pDes->nValue[0] = pTemp->nValue[0] * (MAX_PERCENT + m_CurrentColdEnhance) / MAX_PERCENT + nAddDamageV;
		pDes->nValue[1] = pTemp->nValue[1] + m_CurrentColdEnhance;
		pDes->nValue[2] = pTemp->nValue[2] * (MAX_PERCENT + m_CurrentColdEnhance) / MAX_PERCENT + nAddDamageV;

		if (!bIsPhysical)
		{
			pDes->nValue[0] += m_PhysicsMagic.nValue[0] + m_CurrentColdMagic.nValue[0];
			pDes->nValue[1] = max(pDes->nValue[1], m_CurrentColdMagic.nValue[1] + m_CurrentColdEnhance);
			pDes->nValue[2] += m_PhysicsMagic.nValue[0] + m_CurrentColdMagic.nValue[2];
		}
	}
	if (bIsPhysical)
	{
		pDes->nValue[0] += m_CurrentColdDamage.nValue[0];
		pDes->nValue[1] = max(pDes->nValue[1], m_CurrentColdDamage.nValue[1] + m_CurrentColdEnhance);
		pDes->nValue[2] += m_CurrentColdDamage.nValue[2];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_firedamage_v)
	{
		pDes->nAttribType = magic_firedamage_v;
		pDes->nValue[0] = pTemp->nValue[0] * (MAX_PERCENT + m_CurrentFireEnhance) / MAX_PERCENT + nAddDamageV;
		pDes->nValue[2] = pTemp->nValue[2] * (MAX_PERCENT + m_CurrentFireEnhance) / MAX_PERCENT + nAddDamageV;

		if (!bIsPhysical)
		{
			pDes->nValue[0] += m_PhysicsMagic.nValue[0] + m_CurrentFireMagic.nValue[0];
			pDes->nValue[2] += m_PhysicsMagic.nValue[2] + m_CurrentFireMagic.nValue[2];
		}
	}
	if (bIsPhysical)
	{
		pDes->nValue[0] += m_CurrentFireDamage.nValue[0];
		pDes->nValue[2] += m_CurrentFireDamage.nValue[2];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_lightingdamage_v)
	{
		pDes->nAttribType = magic_lightingdamage_v;
		pDes->nValue[0] = pTemp->nValue[0] + nAddDamageV;
		pDes->nValue[2] = pTemp->nValue[2] + nAddDamageV;

		if (!bIsPhysical)
		{
			pDes->nValue[0] += m_PhysicsMagic.nValue[0] + m_CurrentLightMagic.nValue[0];
			pDes->nValue[2] += m_PhysicsMagic.nValue[2] + m_CurrentLightMagic.nValue[2];
		}

	}
	else if (pTemp->nAttribType == magic_lightingdamage_p)
	{
		pDes->nAttribType = magic_lightingdamage_p;
		pDes->nValue[0] = pTemp->nValue[0] * (MAX_PERCENT + m_CurrentLightEnhance) / MAX_PERCENT + nAddDamageV;
		pDes->nValue[2] = pTemp->nValue[2] * (MAX_PERCENT + m_CurrentLightEnhance) / MAX_PERCENT + nAddDamageV;

		/*int skillMultiplier = (MAX_PERCENT + nAddDamageP) / MAX_PERCENT;
		int baseDamage = pTemp->nValue[0] * skillMultiplier;
		int maxDamage = pTemp->nValue[2] * skillMultiplier;
		int lightEnhancedDamage = (maxDamage - baseDamage) * m_CurrentLightEnhance / 100;

		pDes->nValue[0] = baseDamage + lightEnhancedDamage;
		pDes->nValue[2] = maxDamage;*/

		if (!bIsPhysical)
		{
			pDes->nValue[0] += m_PhysicsMagic.nValue[0] + m_CurrentLightMagic.nValue[0];
			pDes->nValue[2] += m_PhysicsMagic.nValue[2] + m_CurrentLightMagic.nValue[2];
		}
	}
	if (bIsPhysical)
	{
		pDes->nValue[0] += m_CurrentLightDamage.nValue[0];
		pDes->nValue[2] += m_CurrentLightDamage.nValue[2];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_poisondamage_v)
	{
		pDes->nAttribType = magic_poisondamage_v;
		pDes->nValue[0] = pTemp->nValue[0] * (MAX_PERCENT + m_CurrentPoisonEnhance) / 100 + nAddDamageV;
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2] * (MAX_PERCENT - m_CurrentPoisonEnhance) / MAX_PERCENT;
		if (pDes->nValue[2] <= 0)
			pDes->nValue[2] = 1;

		if (!bIsPhysical)
		{
			g_NpcAttribModify.MixPoisonDamage(pDes, &m_CurrentPoisonMagic);
			pDes->nValue[0] += m_PhysicsMagic.nValue[0];
			pDes->nValue[2] += m_PhysicsMagic.nValue[2];
		}
	}
	if (bIsPhysical)
		g_NpcAttribModify.MixPoisonDamage(pDes, &m_CurrentPoisonDamage);
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_stun_p)
	{
		pDes->nAttribType = magic_stun_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_ignorenegativestate_p)
	{
		pDes->nAttribType = magic_ignorenegativestate_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	pTemp++;
	pDes++;
	if (pTemp->nAttribType == magic_randmove)
	{
		pDes->nAttribType = magic_randmove;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	pTemp++;
	pDes++;											 // 16 自动释放技能
	if (pTemp->nAttribType == magic_autoattackskill) // Automatic skill 16
	{
		pDes->nAttribType = magic_autoattackskill;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
#ifdef _SERVER
		pDes->nAttribType = magic_autoattackskill;
		pDes->nValue[0] = m_AttackSkill[0].nSkillId;
		pDes->nValue[1] = m_AttackSkill[0].nSkillLevel;
		pDes->nValue[2] = m_AttackSkill[0].nRate;
#endif // _SERVER
	}
}

void KNpc::ServerMove(INT MoveSpeed)
{
	if (m_Doing != do_walk && m_Doing != do_run && m_Doing != do_hurt && m_Doing != do_runattack)
		return;

	if (MoveSpeed <= 0)
		return;

	if (MoveSpeed >= SubWorld[m_SubWorldIndex].m_nCellWidth)
	{
		MoveSpeed = SubWorld[m_SubWorldIndex].m_nCellWidth - 1;
	}

#ifndef _SERVER
	if (m_RegionIndex < 0 || m_RegionIndex >= 9)
	{
		g_DebugLog("Npc (%d) ServerMove RegionIdx = %d", m_Index, m_RegionIndex);
		_ASSERT(0);
		DoStand();
		return;
	}
#else
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;
#endif
	INT x, y;

	SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, 0, 0, &x, &y);
	x = (x << 10) + m_OffX;
	y = (y << 10) + m_OffY;

	INT nRet = m_PathFinder.GetDir(x, y, m_Dir, m_DesX, m_DesY, MoveSpeed, &m_Dir);

#ifndef _SERVER
	if (nRet == 1) // 有路可以走
	{

		if (m_Dir >= MaxMissleDir)
			// m_Dir -= MaxMissleDir;
			m_Dir = m_Dir % MaxMissleDir;

		x = g_DirCos(m_Dir, MaxMissleDir) * MoveSpeed * FIND_PATH_DISTANCE; // 下个要走的点
		y = g_DirSin(m_Dir, MaxMissleDir) * MoveSpeed * FIND_PATH_DISTANCE;

	}
	else if (nRet == 0) // 路不通
	{
		DoStand(); // 站着

		return;
	}
	else if (nRet == -1) // 到地图外面
	{					 // 删除NPC
		SubWorld[0].m_Region[m_RegionIndex].RemoveNpc(m_Index);
		SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);

		m_RegionIndex = -1;
		return;
	}
	else
	{
		return;
	}
#endif
#ifdef _SERVER
	if (nRet == 1)
	{
		if (m_Dir >= MaxMissleDir)
			// m_Dir -= MaxMissleDir;
			m_Dir = m_Dir % MaxMissleDir;

		x = g_DirCos(m_Dir, MaxMissleDir) * MoveSpeed * FIND_PATH_DISTANCE; // 下个要走的点
		y = g_DirSin(m_Dir, MaxMissleDir) * MoveSpeed * FIND_PATH_DISTANCE; // 下个要走的点
	}
	else
	{
		DoStand();
		/*if (m_Kind==kind_player)
		{
			Player[nPlayerIndex].m_ItemList.msgshow("S站着不动A");
		}*/
		return;
	}
#endif

	INT nOldRegion = m_RegionIndex;
	INT nOldMapX = m_MapX;
	INT nOldMapY = m_MapY;
	INT nOldOffX = m_OffX;
	INT nOldOffY = m_OffY;

	m_OffX += x;
	m_OffY += y;

	if (!m_bClientOnly)
		CURREGION.DecNpcRef(m_MapX, m_MapY, obj_npc);

	if (m_OffX < 0)
	{
		m_MapX--;
		m_OffX += CELLWIDTH;
	}
	else if (m_OffX > CELLWIDTH)
	{
		m_MapX++;
		m_OffX -= CELLWIDTH;
	}

	if (m_OffY < 0)
	{
		m_MapY--;
		m_OffY += CELLHEIGHT;
	}
	else if (m_OffY > CELLHEIGHT)
	{
		m_MapY++;
		m_OffY -= CELLHEIGHT;
	}

	if (m_MapX < 0)
	{
		m_RegionIndex = LEFTREGIONIDX;
		m_MapX += REGIONWIDTH;
	}
	else if (m_MapX >= REGIONWIDTH)
	{
		m_RegionIndex = RIGHTREGIONIDX;
		m_MapX -= REGIONWIDTH;
	}

	if (m_RegionIndex >= 0)
	{
		if (m_MapY < 0)
		{
			m_RegionIndex = UPREGIONIDX;
			m_MapY += REGIONHEIGHT;
		}
		else if (m_MapY >= REGIONHEIGHT)
		{
			m_RegionIndex = DOWNREGIONIDX;
			m_MapY -= REGIONHEIGHT;
		}

		if (!m_bClientOnly && m_RegionIndex >= 0)
			CURREGION.AddNpcRef(m_MapX, m_MapY, obj_npc);
	}

	if (m_RegionIndex == -1)
	{
		m_RegionIndex = nOldRegion;
		m_MapX = nOldMapX;
		m_MapY = nOldMapY;
		m_OffX = nOldOffX;
		m_OffY = nOldOffY;
		CURREGION.AddNpcRef(m_MapX, m_MapY, obj_npc);
		return;
	}

	if (nOldRegion != m_RegionIndex)
	{
#ifdef _SERVER
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, m_RegionIndex, m_Index);
		if (IsPlayer())
		{ // 转换地图时
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, m_RegionIndex, m_nPlayerIdx);

			if (m_nPlayerIdx > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove); // 移动磨损
			}
		}
#else
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
#endif
	}
}

VOID KNpc::FixPos()
{
	//Round off and omit the last ten digits
	m_OffX = (m_OffX + 512) & (~1023);
	m_OffY = (m_OffY + 512) & (~1023);
}

void KNpc::ServerJump(INT nSpeed)
{
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (!(m_Doing == do_jump || m_Doing == do_jumpattack))
		return;

	if (nSpeed <= 0)
		return;

	if (nSpeed >= SubWorld[m_SubWorldIndex].m_nCellWidth)
	{
		nSpeed = SubWorld[m_SubWorldIndex].m_nCellWidth - 1;
	}

	m_OffX += g_DirCos(m_JumpDir, 64) * nSpeed;
	m_OffY += g_DirSin(m_JumpDir, 64) * nSpeed;

	// s = vt - a * t * t / 2
	m_Height = (m_JumpFirstSpeed * m_Frames.nCurrentFrame - ACCELERATION_OF_GRAVITY * m_Frames.nCurrentFrame * m_Frames.nCurrentFrame / 2) / 8;
	if (m_Height < 0)
		m_Height = 0;

	INT nOldRegion = m_RegionIndex;
	INT nOldMapX = m_MapX;
	INT nOldMapY = m_MapY;
	INT nOldOffX = m_OffX;
	INT nOldOffY = m_OffY;
	CURREGION.DecNpcRef(m_MapX, m_MapY, obj_npc);

	if (m_OffX < 0)
	{
		m_MapX--;
		m_OffX += CELLWIDTH;
	}
	else if (m_OffX > CELLWIDTH)
	{
		m_MapX++;
		m_OffX -= CELLWIDTH;
	}

	if (m_OffY < 0)
	{
		m_MapY--;
		m_OffY += CELLHEIGHT;
	}
	else if (m_OffY > CELLHEIGHT)
	{
		m_MapY++;
		m_OffY -= CELLHEIGHT;
	}

	if (m_MapX < 0)
	{
		m_RegionIndex = LEFTREGIONIDX;
		m_MapX += REGIONWIDTH;
	}
	else if (m_MapX >= REGIONWIDTH)
	{
		m_RegionIndex = RIGHTREGIONIDX;
		m_MapX -= REGIONWIDTH;
	}

	if (m_RegionIndex >= 0)
	{
		if (m_MapY < 0)
		{
			m_RegionIndex = UPREGIONIDX;
			m_MapY += REGIONHEIGHT;
		}
		else if (m_MapY >= REGIONHEIGHT)
		{
			m_RegionIndex = DOWNREGIONIDX;
			m_MapY -= REGIONHEIGHT;
		}
		if (m_RegionIndex >= 0)
			CURREGION.AddNpcRef(m_MapX, m_MapY, obj_npc);
	}

	if (m_RegionIndex == -1)	// ²»¿ÉÄÜÒÆ¶¯µ½-1 Region£¬Èç¹û³öÏÖÕâÖÖÇé¿ö£¬»Ö¸´Ô­×ø±ê
	{
		m_RegionIndex = nOldRegion;
		m_MapX = nOldMapX;
		m_MapY = nOldMapY;
		m_OffX = nOldOffX;
		m_OffY = nOldOffY;
		CURREGION.AddNpcRef(m_MapX, m_MapY, obj_npc);
		return;
	}

	if (nOldRegion != m_RegionIndex)
	{
#ifdef _SERVER
		SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, m_RegionIndex, m_Index);
		if (IsPlayer())
		{
			SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, m_RegionIndex, m_nPlayerIdx);
			if (m_nPlayerIdx > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeMove);
			}
		}
#else
		if (m_RegionIndex >= 0)
		{
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
			m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
		}
#endif
	}
}

void KNpc::SendCommand(NPCCMD cmd, INT x, INT y, INT z)
{
	if (cmd == do_run)
	{
		if ((m_CurrentStamina < PlayerSet.m_cPlayerStamina.m_nKillRunSub && m_nPKFlag == enumPKMurder) || (m_CurrentStamina < PlayerSet.m_cPlayerStamina.m_nTongWarRunSub && m_nPKFlag == enumPKTongWar))
			cmd = do_walk;
	}

	if (m_FrozenAction.nTime > 0)
	{
		if (cmd == do_walk ||
			cmd == do_run ||
			cmd == do_runattack ||
			cmd == do_jump ||
			cmd == do_jumpattack ||
			cmd == do_skill ||
			cmd == do_magic ||
			cmd == do_attack ||
			cmd == do_blurmove)
		{
			return;
		}
	}

	if (m_RandMove.nTime > 0)
	{
		if (cmd == do_run ||
			cmd == do_runattack ||
			cmd == do_jump ||
			cmd == do_jumpattack ||
			cmd == do_skill ||
			cmd == do_magic ||
			cmd == do_attack ||
			cmd == do_blurmove)
		{
			return;
		}
	}

	m_Command.CmdKind = cmd;
	m_Command.Param_X = x;
	m_Command.Param_Y = y;
	m_Command.Param_Z = z;

}

BOOL KNpc::NewPath(INT nMpsX, INT nMpsY)
{
	m_DesX = nMpsX;
	m_DesY = nMpsY;
	return TRUE;
}

BOOL KNpc::NewJump(INT nMpsX, INT nMpsY)
{
	_ASSERT(m_CurrentJumpSpeed > 0);
	if (m_CurrentJumpSpeed <= 0)
		return FALSE;

	INT nX, nY;
	GetMpsPos(&nX, &nY);

	if (nX == nMpsX && nY == nMpsY)
		return FALSE;

	INT nDir = g_GetDirIndex(nX, nY, nMpsX, nMpsY);
	INT	nMaxLength = m_CurrentJumpSpeed * m_CurrentJumpFrame;
	INT	nWantLength = g_GetDistance(nX, nY, nMpsX, nMpsY);

	if (m_Dir >= MaxMissleDir)
		m_Dir -= MaxMissleDir;

	INT	nSin = g_DirSin(nDir, MaxMissleDir);
	INT	nCos = g_DirCos(nDir, MaxMissleDir);

	if (nWantLength > nMaxLength)
	{
		m_DesX = nX + ((nMaxLength * nCos) >> 10);
		m_DesY = nY + ((nMaxLength * nSin) >> 10);
		nWantLength = nMaxLength;
	}
	else if (nWantLength <= MIN_JUMP_RANGE)
	{
		m_DesX = nMpsX;
		m_DesY = nMpsY;
		return FALSE;
	}
	else if (nWantLength <= MIN_DOMELEE_RANGE)
	{
		m_DesX = nMpsX;
		m_DesY = nMpsY;
		return FALSE;
	}

	m_JumpStep = nWantLength / m_CurrentJumpSpeed;

	INT nTestX = 0;
	INT nTestY = 0;
	INT nSuccessStep = 0;

	for (INT i = 1; i < m_JumpStep + 1; i++)
	{
		nTestX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
		nTestY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
		INT nBarrier = SubWorld[m_SubWorldIndex].GetBarrier(nTestX, nTestY);
		DWORD	dwTrap = SubWorld[m_SubWorldIndex].GetTrap(nTestX, nTestY);
		if (Obstacle_NULL == nBarrier && dwTrap == 0)
		{
			g_DebugLog("Obstacle_NULL == nBarrier && dwTrap == 0");
			nSuccessStep = i;
		}
		if (Obstacle_Normal == nBarrier || Obstacle_Fly == nBarrier || dwTrap)
		{
			g_DebugLog("Obstacle_Normal == nBarrier || Obstacle_Fly == nBarrier || dwTrap");
			if (nSuccessStep <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return FALSE;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * nSuccessStep) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * nSuccessStep) >> 10);
			m_JumpStep = nSuccessStep;
			break;
		}

		INT nTrap = SubWorld[m_SubWorldIndex].GetTrap(nTestX, nTestY);

		if (nTrap)
		{ // 有Trap
			if (i <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return FALSE;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
			m_JumpStep = i;
			break;
		}
	}
	m_JumpDir = nDir;
	return TRUE;
}

BOOL KNpc::Cost(NPCATTRIB nType, INT nCost, BOOL bIsAudit, BOOL bNotShowMessage)
{
	if (!IsPlayer())
		return TRUE;

	INT* pSource = NULL;
	INT nCurCost = nCost;
	switch (nType)
	{
	case attrib_mana_v:
		pSource = &m_CurrentMana;
		break;
	case attrib_mana_p:
		pSource = &m_CurrentMana;
		nCurCost = m_CurrentManaMax * nCost / MAX_PERCENT;
		break;
	case attrib_life_v:
		pSource = &m_CurrentLife;
		break;
	case attrib_life_p:
		pSource = &m_CurrentLife;
		nCurCost = m_CurrentLifeMax * nCost / MAX_PERCENT;
		break;
	case attrib_stamina_v:
		pSource = &m_CurrentStamina;
		break;
	case attrib_stamina_p:
		pSource = &m_CurrentStamina;
		nCurCost = m_CurrentStaminaMax * nCost / MAX_PERCENT;
		break;
	default:
		break;
	}

	if (pSource)
	{
		if (Player[m_nPlayerIdx].GetFactionNo() == 10 && m_CurrentMana <= 0 && m_CurrentManaReplenish <= 0)
		{
			m_CurrentMana = 0;
			return TRUE;
		}

		//if ((nType == attrib_life_v || nType == attrib_life_p) ? ((*pSource - 1) < nCurCost) : (*pSource < nCurCost))
		if ((nType == attrib_life_v || nType == attrib_life_p) ? (static_cast<INT>(*pSource - 1) < nCurCost) : (static_cast<INT>(*pSource) < nCurCost))
		{
			if (!bNotShowMessage)
			{
#ifndef _SERVER
				KSystemMessage Msg;

				Msg.byConfirmType = SMCT_NONE;
				Msg.byParamSize = 0;
				Msg.byPriority = 1;
				Msg.eType = SMT_NORMAL;
				switch (nType)
				{
				case attrib_mana_v:
				case attrib_mana_p:
					g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_MANA, sizeof(Msg.szMessage));
					break;
				case attrib_life_v:
				case attrib_life_p:
					g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_LIFE, sizeof(Msg.szMessage));
					break;
				case attrib_stamina_v:
				case attrib_stamina_p:
					g_StrCpyLen(Msg.szMessage, MSG_NPC_NO_STAMINA, sizeof(Msg.szMessage));
					break;
				default:
					break;
				}
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (UINT)&Msg, NULL);
#endif
			}
			return FALSE;
		}
		else
		{
#ifdef _SERVER
			if (!bIsAudit)
				*pSource -= nCurCost;
#endif
			return TRUE;
		}
	}
	return FALSE;
}

#ifdef _SERVER
void KNpc::Cast(INT nSkillId, INT nSkillLevel, INT nTarget)
{
	if (nSkillId < MAX_SKILL && nSkillLevel < MAX_SKILLLEVEL)
	{
		INT nMpsX, nMpsY;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		_ASSERT(nSkillId < MAX_SKILL && nSkillLevel < MAX_SKILLLEVEL);

		NPC_SKILL_SYNC SkillCmd;
		SkillCmd.ID = this->m_dwID;
		SkillCmd.nSkillID = nSkillId;
		SkillCmd.nSkillLevel = nSkillLevel;
		SkillCmd.nMpsX = -1;
		SkillCmd.nMpsY = Npc[nTarget].m_dwID; // m_dwID;
		SkillCmd.ProtocolType = s2c_castskilldirectly;

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
		CURREGION.BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX, m_MapY);
		INT i;
		for (i = 0; i < 8; i++)
		{
			if (CONREGIONIDX(i) == -1)
				continue;
			CONREGION(i).BroadCast(&SkillCmd, sizeof(SkillCmd), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
		}

		KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nSkillId, nSkillLevel);
		pOrdinSkill->Cast(m_Index, nMpsX, nMpsY);

		if (pOrdinSkill != NULL && !pOrdinSkill->IsAura())
		{
			DWORD dwCastTime = 0;
			eSkillStyle eStyle = (eSkillStyle)pOrdinSkill->GetSkillStyle();
			if (eStyle == SKILL_SS_Missles
				|| eStyle == SKILL_SS_Melee
				|| eStyle == SKILL_SS_InitiativeNpcState
				|| eStyle == SKILL_SS_PassivityNpcState)
			{
				dwCastTime = pOrdinSkill->GetDelayPerCast(m_bRideHorse);
			}
			else
			{
				switch (eStyle)
				{
				case SKILL_SS_Thief:
				{
					dwCastTime = ((KThiefSkill*)pOrdinSkill)->GetDelayPerCast();
				}break;
				}
			}
			m_SkillList.SetNextCastTime(nSkillId, SubWorld[m_SubWorldIndex].m_dwCurrentTime, SubWorld[m_SubWorldIndex].m_dwCurrentTime + dwCastTime);
		}
	}
}
#endif

void KNpc::DoJump()
{
	if (m_SubWorldIndex < 0 || m_SubWorldIndex >= MAX_SUBWORLD || m_RegionIndex < 0)
		return;

	_ASSERT(m_RegionIndex >= 0);

	if (m_Doing == do_jump)
		return;

#ifndef _SERVER
	if (m_WalkRun.nTime)
		m_DataRes.SetBlur(TRUE);
#endif

	m_Doing = do_jump;
	m_Dir = m_JumpDir;
	m_ProcessAI = 0;
	m_JumpFirstSpeed = ACCELERATION_OF_GRAVITY * (m_JumpStep - 1) / 2;
#ifdef _SERVER	// ÏòÖÜÎ§9¸öRegion¹ã²¥·¢¼¼ÄÜ
	NPC_JUMP_SYNC	NetCommand;
	NetCommand.ProtocolType = (BYTE)s2c_npcjump;
	NetCommand.ID = m_dwID;
	NetCommand.nMpsX = m_DesX;
	NetCommand.nMpsY = m_DesY;

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
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	for (INT i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

#ifndef _SERVER
	m_ClientDoing = cdo_jump;
#endif

	m_Frames.nTotalFrame = m_JumpStep;
	m_Frames.nCurrentFrame = 0;
}

BOOL KNpc::OnJump()
{
	ServerJump(m_CurrentJumpSpeed);
	if (WaitForFrame())
	{
		DoStand();
		m_ProcessAI = 1;
		//#ifndef _SERVER	
		//		m_DataRes.SetBlur(FALSE);
		//#endif
		return FALSE;
	}
	return TRUE;
}

BOOL KNpc::WaitForFrame()
{
	m_Frames.nCurrentFrame++;
	if (m_Frames.nCurrentFrame < m_Frames.nTotalFrame)
	{
		return FALSE;
	}
	m_Frames.nCurrentFrame = 0;
	return TRUE;
}

BOOL KNpc::IsReachFrame(INT nPercent)
{
	if (m_Frames.nCurrentFrame == m_Frames.nTotalFrame * nPercent / MAX_PERCENT)
	{
		return TRUE;
	}
	return FALSE;
}

// spawn monsters
// The NpcSettingIdx obtained by the client from the network contains the high 16-bit Npc template number and the low-order 16 bits as the level.
void KNpc::Load(INT nNpcSettingIdx, INT nLevel, INT nSubWorld, INT nBoss)
{
	m_PathFinder.Init(m_Index);
	if (nLevel <= 0)
	{
		nLevel = 1;
	}
	m_NpcSettingIdx = nNpcSettingIdx;
	m_Level = nLevel;
	m_btSpecial = nBoss;

#ifndef _SERVER
	char	szNpcTypeName[32];
#endif
	if (nNpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID || nNpcSettingIdx == PLAYER_FEMALE_NPCTEMPLATEID)
	{
#ifndef _SERVER
		if (nNpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID)
		{
			strcpy(szNpcTypeName, "MainMan");
			m_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(TRUE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(TRUE);
		}
		else
		{
			strcpy(szNpcTypeName, "MainLady");
			m_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(FALSE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(FALSE);
		}
#endif
		//		TODO: Load Player Data;
		m_WalkSpeed = NpcSet.GetPlayerWalkSpeed();
		m_RunSpeed = NpcSet.GetPlayerRunSpeed();
		m_AttackFrame = NpcSet.GetPlayerAttackFrame();
		m_HurtFrame = NpcSet.GetPlayerHurtFrame();
	}
	else
	{
		GetNpcCopyFromTemplate(nNpcSettingIdx, nLevel);

#ifndef _SERVER	
		g_NpcSetting.GetString(nNpcSettingIdx + 2, "NpcResType", "enemy003", szNpcTypeName, sizeof(szNpcTypeName));
		if (!szNpcTypeName[0])
		{																							  // 人物类型表 npcres
			g_NpcKindFile.GetString(2, "人物名称", "enemy003", szNpcTypeName, sizeof(szNpcTypeName)); // 如果没找到，用第一个npc代替
		}
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIMode", 12, &m_AiMode);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam1", 12, &m_AiParam[0]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam2", 12, &m_AiParam[1]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam3", 12, &m_AiParam[2]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam4", 12, &m_AiParam[3]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam5", 12, &m_AiParam[4]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam6", 12, &m_AiParam[5]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam7", 12, &m_AiParam[6]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam8", 12, &m_AiParam[7]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam9", 12, &m_AiParam[8]);

		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "ActiveRadius", 12, &m_ActiveRadius); // 活动范围
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "ClientOnly", 0, &m_bClientOnly);
		INT m_nIsRevive;
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "IsRevive", 0, &m_nIsRevive);
		m_IsRevive = (DWORD)m_nIsRevive;

		if (nSubWorld > -1)
		{
			char nMapNpc[32] = { 0 };
			INT nIsCreat = 0;
			ZeroMemory(nMapNpc, sizeof(nMapNpc));
			sprintf(nMapNpc, "%d_IsScript", SubWorld[nSubWorld].m_SubWorldID);
			g_NpcMapDropRate.GetInteger("List", nMapNpc, 0, &nIsCreat);
			IsExeGoldScript = nIsCreat;
			sprintf(nMapNpc, "%d_IsCreat", SubWorld[nSubWorld].m_SubWorldID);
			g_NpcMapDropRate.GetInteger("List", nMapNpc, 0, &nIsCreat); // 默认执行全局脚本
			IsCreatBoss = nIsCreat;
		}
		// Flying category, 11, 12, 17, use AiParam[6] to save the planned setting altitude
		// add by flying
		if (m_AiMode == 11 || m_AiMode == 12 || m_AiMode == 17)
			m_AiParam[6] = m_AiMode;
#endif
	}
#ifndef _SERVER
	this->RemoveRes();
	m_DataRes.Init(szNpcTypeName, &g_NpcResList);

	m_DataRes.SetAction(m_ClientDoing);
	if (m_bRideHorse && m_ClientDoing == cdo_jump)
		m_DataRes.SetRideHorse(FALSE);
	else
		m_DataRes.SetRideHorse(m_bRideHorse);
	m_DataRes.SetArmor(m_ArmorType, m_MantleType);
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	m_DataRes.SetWeapon(m_WeaponType);
#endif
	m_CurrentCamp = m_Camp;

#ifdef _SERVER
	// 出现蓝装 率怪的概率
	if (m_Kind == kind_normal)
	{												 // 是动物
		m_cGold.RandChangeGold(1, nBoss, nSubWorld);//There is a certain probability of spawning monsters above blue monsters. 0 means canceling the spawning of blue monsters.

		/*if (CheckMaps("IsThisMap",SubWorld[nSubWorld].m_SubWorldID))
		{//执行地图全局脚本,设置重生NPC方案
			//M_SubWorldSet->SubExecuteScript("\\script\\Global\\goldrevive.lua","ReviveMain",Name,"",SubWorld[nSubWorld].m_SubWorldID,m_NpcSettingIdx,m_Index);
			  NpcExecuteScriptA("\\script\\global\\goldrevive.lua","ReviveMain",Name,Name,SubWorld[nSubWorld].m_SubWorldID,m_NpcSettingIdx,m_Index);
		}*/
	}
#endif
}

void KNpc::GetMpsPos(INT* pPosX, INT* pPosY)
{
#ifdef _SERVER
	SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, pPosX, pPosY);
#else
	SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, pPosX, pPosY);
#endif
}

BOOL	KNpc::SetActiveSkill(INT nSkillIdx)
{
	if (nSkillIdx <= 0 || nSkillIdx >= MAX_NPCSKILL)
		return FALSE;

	if (m_SkillList.m_Skills[nSkillIdx].SkillId <= 0 || m_SkillList.m_Skills[nSkillIdx].CurrentSkillLevel <= 0)
		return FALSE;

	m_ActiveSkillID = m_SkillList.m_Skills[nSkillIdx].SkillId;

	INT nCurLevel = m_SkillList.m_Skills[nSkillIdx].CurrentSkillLevel;

	_ASSERT(m_ActiveSkillID < MAX_SKILL && nCurLevel < MAX_SKILLLEVEL && nCurLevel > 0);

	ISkill* pISkill = g_SkillManager.GetSkill(m_ActiveSkillID, nCurLevel);
	if (pISkill)
	{
		m_CurrentAttackRadius = pISkill->GetAttackRadius();
	}
	return TRUE;
}

void KNpc::SetAuraSkill(INT nSkillID)
{
	INT nCurLevel = 0;
	if (nSkillID <= 0 || nSkillID >= MAX_SKILL)
	{
		nSkillID = 0;
	}
	else
	{
		nCurLevel = m_SkillList.GetCurrentLevel(nSkillID);
		//if (m_SkillList.GetLevel(nSkillID) <= 0) 
		if (nCurLevel <= 0)
		{
			nSkillID = 0;
		}
		else
		{
			_ASSERT(nSkillID < MAX_SKILL && nCurLevel < MAX_SKILLLEVEL);

			KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nSkillID, m_SkillList.GetCurrentLevel(nSkillID));
			if (!pOrdinSkill || !pOrdinSkill->IsAura())
			{
				nSkillID = 0;
			}
		}
	}
	m_ActiveAuraID = nSkillID;

#ifndef _SERVER
	SKILL_CHANGEAURASKILL_COMMAND ChangeAuraMsg;
	ChangeAuraMsg.ProtocolType = c2s_changeauraskill;
	ChangeAuraMsg.m_nAuraSkill = m_ActiveAuraID;
	if (g_pClient)
		g_pClient->SendPackToServer(&ChangeAuraMsg, sizeof(SKILL_CHANGEAURASKILL_COMMAND));
#else
	UpdateNpcStateInfo();
#endif
}

BOOL KNpc::SetPlayerIdx(INT nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_PLAYER)
		return FALSE;

	if (m_Kind != kind_player)
		return FALSE;

	m_nPlayerIdx = nIdx;
	return TRUE;
}

void KNpc::SwitchMaskFeature()
{
	m_bMaskFeature = !m_bMaskFeature;
#ifdef _SERVER
	INT nIdx = Player[m_nPlayerIdx].m_ItemList.GetEquipment(itempart_mask);
	if (m_bMaskFeature)
	{
		if (nIdx && !Player[m_nPlayerIdx].m_ItemList.GetMaskLock())
			m_MaskType = 0;
	}
	else
	{
		if (nIdx && !Player[m_nPlayerIdx].m_ItemList.GetMaskLock())
		{
			switch (Item[nIdx].m_CommonAttrib.nItemNature)
			{
			case NATURE_GOLD:
				m_MaskType = g_ItemChangeRes.GetGoldItemRes(Item[nIdx].GetDetailType(), Item[nIdx].GetRow());
				break;
			case NATURE_PLATINA:
				break;
			default:
				g_MaskChangeRes.GetInteger(Item[nIdx].GetParticular() + 2, 2, 0, &m_MaskType);
				break;
			}
		}
	}
#endif

#ifdef _SERVER
	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_lpBuf = 0;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_MASKFEATURE;
	g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
#endif
}

#ifdef _SERVER
BOOL KNpc::SendSyncData(INT nClient, BOOL bBroadCast/* = FALSE*/)
{
	BOOL	bRet = FALSE;
	NPC_SYNC	NpcSync;

	NpcSync.ProtocolType = (BYTE)s2c_syncnpc;
	NpcSync.m_btKind = (BYTE)m_Kind;
	NpcSync.Camp = (BYTE)m_Camp;
	NpcSync.CurrentCamp = (BYTE)m_CurrentCamp;
	NpcSync.m_bySeries = (BYTE)m_Series;
	NpcSync.m_Doing = (BYTE)m_Doing;

	NpcSync.CurrentLifeMax = (INT)m_CurrentLifeMax;
	if (m_CurrentLifeMax > 0)
		NpcSync.CurrentLife = (INT)m_CurrentLife;
	else
		NpcSync.CurrentLife = 0;

	if (this->IsPlayer())
		NpcSync.m_btMenuState = (BYTE)Player[this->m_nPlayerIdx].m_cMenuState.m_nState;
	else
		NpcSync.m_btMenuState = 0;
	NpcSync.Dir = m_Dir;
	GetMpsPos((INT*)&NpcSync.MapX, (INT*)&NpcSync.MapY);
	NpcSync.ID = m_dwID;
	NpcSync.NpcSettingIdx = MAKELONG(m_Level, m_NpcSettingIdx);

	NpcSync.Special = (BYTE)m_btSpecial;
	NpcSync.MissionGroup = (INT)m_nMissionGroup;
	strcpy(NpcSync.m_szName, m_Kind == kind_player ? (Player[m_nPlayerIdx].m_bForbidName ? "" : Name) : Name);
	NpcSync.m_wLength = sizeof(NPC_SYNC) - 1 - sizeof(NpcSync.m_szName) + strlen(NpcSync.m_szName);

	if (bBroadCast)
	{
		this->SendDataToNearRegion((BYTE*)&NpcSync, NpcSync.m_wLength + 1);
	}
	else
	{
		if (SUCCEEDED(g_pServer->PackDataToClient(nClient, (BYTE*)&NpcSync, NpcSync.m_wLength + 1)))
		{
			//printf("Packing sync data ok...\n");
			bRet = TRUE;
		}
		else
		{
			printf("Packing sync data failed...\n");
			return FALSE;
		}
	}
	//	g_DebugLog("[Sync]%d:%s<%d> request to %d. size:%d", SubWorld[m_SubWorldIndex].m_dwCurrentTime, Name, m_Kind, nClient, NpcSync.m_wLength + 1);

	if (IsPlayer())
	{
		PLAYER_SYNC	PlayerSync;

		PlayerSync.ProtocolType = (BYTE)s2c_syncplayer;
		PlayerSync.ID = m_dwID;
		PlayerSync.TeamServerID = Player[m_nPlayerIdx].m_cTeam.m_nFlag ? (INT)Player[m_nPlayerIdx].m_cTeam.m_nID : -1;
		PlayerSync.HelmType = (BYTE)m_HelmType;
		PlayerSync.ArmorType = (BYTE)m_ArmorType;
		PlayerSync.WeaponType = (BYTE)m_WeaponType;
		PlayerSync.MaskType = m_MaskType;
		PlayerSync.MantleType = (BYTE)m_MantleType;
		if (Player[m_nPlayerIdx].m_cTong.m_nFlag)
		{
			PlayerSync.TongNameID = Player[m_nPlayerIdx].m_cTong.m_dwTongNameID;
			PlayerSync.m_Figure = (BYTE)Player[m_nPlayerIdx].m_cTong.m_nFigure;
			PlayerSync.TongNationalEmblem = Player[m_nPlayerIdx].m_cTong.m_nTongNationalEmblem;
			strcpy(PlayerSync.TongName, Player[m_nPlayerIdx].m_cTong.m_szName);
			strcpy(PlayerSync.TongAgname, Player[m_nPlayerIdx].m_cTong.m_szAgname);
		}
		else
		{
			PlayerSync.TongNameID = 0;
			PlayerSync.m_Figure = -1;
			PlayerSync.TongNationalEmblem = 0;
			PlayerSync.TongName[0] = 0;
			PlayerSync.TongAgname[0] = 0;
		}
		strcpy(PlayerSync.MateName, Player[m_nPlayerIdx].m_cTask.GetSaveStr(TASKVALUE_BASEDATA_MATENAME));
		PlayerSync.HorseType = m_bRideHorse ? (BYTE)m_HorseType : -1;
		PlayerSync.RankID = (BYTE)m_RankID;
		memcpy(&PlayerSync.ExpandRank, &m_CurExpandRank, sizeof(m_CurExpandRank));
		PlayerSync.Translife = (BYTE)m_byTranslife;
		PlayerSync.Viprank = (BYTE)Player[m_nPlayerIdx].m_cTask.GetSaveVal(TASKVALUE_STATTASK_VIPRANK);
		PlayerSync.Repute = Player[m_nPlayerIdx].m_cTask.GetSaveVal(TASKVALUE_STATTASK_REPUTE);
		PlayerSync.FuYuan = Player[m_nPlayerIdx].m_cTask.GetSaveVal(TASKVALUE_STATTASK_FUYUAN);
		PlayerSync.PKFlag = (BYTE)Player[m_nPlayerIdx].m_cPK.GetNormalPKState();
		PlayerSync.PKValue = (BYTE)Player[m_nPlayerIdx].m_cPK.GetPKValue();
		PlayerSync.ImagePlayer = (BYTE)Player[m_nPlayerIdx].m_ImagePlayer;
		PlayerSync.MantleLevel = (BYTE)m_byMantleLevel;
		PlayerSync.RankInWorld = Player[m_nPlayerIdx].m_nWorldStat;
		PlayerSync.FactionNumber = Player[m_nPlayerIdx].m_cFaction.GetCurFactionNo();
		PlayerSync.Trade = (PLAYERTRADE)Player[m_nPlayerIdx].m_PTrade;
		if (Player[m_nPlayerIdx].m_nPaceBarTime)
			PlayerSync.PacePer = (BYTE)(Player[m_nPlayerIdx].m_nPaceBarTime * MAX_PERCENT / Player[m_nPlayerIdx].m_nPaceBarTimeMax);
		else
			PlayerSync.PacePer = 0;
		PlayerSync.m_btSomeFlag = 0;
		if (m_FightMode)
			PlayerSync.m_btSomeFlag |= 0x01;
		if (Player[m_nPlayerIdx].m_bSleepMode)
			PlayerSync.m_btSomeFlag |= 0x02;

		if (bBroadCast)
		{
			this->SendDataToNearRegion((BYTE*)&PlayerSync, sizeof(PLAYER_SYNC));
		}
		else
		{
			if (SUCCEEDED(g_pServer->PackDataToClient(nClient, (BYTE*)&PlayerSync, sizeof(PLAYER_SYNC))))
			{
				//printf("Packing player sync data ok...\n");
				bRet = TRUE;
			}
			else
			{
				printf("Packing player sync data failed...\n");
				return FALSE;
			}
		}
	}
	return bRet;
}

void KNpc::SyncPos(INT nNetConnectIdx /* = -1 */, BOOL bDoStand /*=FALSE*/)
{
	if ((IsDead() && !IsPlayer()) || !m_Index)
		return;
	if (nNetConnectIdx == -1) // 广播到周围的消息
	{
		if (m_RegionIndex < 0)
			return;
	}
	if (bDoStand)
	{
		DoStand();
	}
	NPC_POS_SYNC sNpcSync;
	sNpcSync.ProtocolType = s2c_npcpos;
	sNpcSync.ID = m_dwID;
	INT nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	sNpcSync.MapX = nMpsX;
	sNpcSync.MapY = nMpsY;
	//sNpcSync.m_dwExParam = bDoStand ? 1 : 0;
	// 由于同步数据每次同步的很少，所以上限就是MAX_PLAYER
	if (nNetConnectIdx == -1)
		SendDataToNearRegion(&sNpcSync, sizeof(sNpcSync));
	else
		g_pServer->SendData(nNetConnectIdx, (LPBYTE)&sNpcSync, sizeof(NPC_POS_SYNC));
}

// Æ½Ê±Êý¾ÝµÄÍ¬²½
void KNpc::NormalSync()
{
	if (m_Doing == do_revive || m_Doing == do_death || !m_Index || m_RegionIndex < 0)
		return;

	NPC_NORMAL_SYNC NpcSync;
	INT nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);

	NpcSync.ProtocolType = (BYTE)s2c_syncnpcmin;
	NpcSync.ID = m_dwID;

	memcpy(NpcSync.StateInfo, m_btStateInfo, sizeof(BYTE) * MAX_SKILL_STATE);

	NpcSync.CurrentLifeMax = m_CurrentLifeMax;
	if (m_CurrentLifeMax > 0)
		NpcSync.CurrentLife = m_CurrentLife;
	else
		NpcSync.CurrentLife = 0;

	NpcSync.MapX = nMpsX;
	NpcSync.MapY = nMpsY;

	NpcSync.RunSpeed = (BYTE)m_CurrentRunSpeed;
	NpcSync.WalkSpeed = (BYTE)m_CurrentWalkSpeed;
	NpcSync.AttackSpeed = (WORD)m_CurrentAttackSpeed;
	NpcSync.CastSpeed = (WORD)m_CurrentCastSpeed;

	NpcSync.Camp = (BYTE)m_CurrentCamp;
	NpcSync.State = 0;

	if (m_FreezeState.nTime > 0)
		NpcSync.State |= STATE_FREEZE;
	if (m_PoisonState.nTime > 0)
		NpcSync.State |= STATE_POISON;
	if (m_StunState.nTime > 0)
		NpcSync.State |= STATE_STUN;
	if (m_HideState.nTime > 0)
		NpcSync.State |= STATE_HIDE;
	if (m_FrozenAction.nTime > 0)
		NpcSync.State |= STATE_FROZEN;
	if (m_WalkRun.nTime > 0)
		NpcSync.State |= STATE_WALKRUN;
	NpcSync.Doing = (BYTE)m_Doing;

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
	CURREGION.BroadCast(&NpcSync, sizeof(NPC_NORMAL_SYNC), nMaxCount, m_MapX, m_MapY);
	INT j;
	for (j = 0; j < 8; j++)
	{
		INT nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		_ASSERT(m_SubWorldIndex >= 0 && nConRegion >= 0);
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcSync, sizeof(NPC_NORMAL_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}

	if (IsPlayer())
	{
		PLAYER_NORMAL_SYNC PlayerSync;

		PlayerSync.ProtocolType = (BYTE)s2c_syncplayermin;
		PlayerSync.ID = m_dwID;
		PlayerSync.TeamServerID = Player[m_nPlayerIdx].m_cTeam.m_nFlag ? (INT)Player[m_nPlayerIdx].m_cTeam.m_nID : -1;
		PlayerSync.HelmType = (BYTE)m_HelmType;
		PlayerSync.ArmorType = (BYTE)m_ArmorType;
		PlayerSync.WeaponType = (BYTE)m_WeaponType;
		PlayerSync.MaskType = m_MaskType;
		PlayerSync.MantleType = (BYTE)m_MantleType;
		if (Player[m_nPlayerIdx].m_cTong.m_nFlag)
		{
			PlayerSync.TongNameID = Player[m_nPlayerIdx].m_cTong.m_dwTongNameID;
			PlayerSync.m_Figure = (BYTE)Player[m_nPlayerIdx].m_cTong.m_nFigure;
			PlayerSync.TongNationalEmblem = Player[m_nPlayerIdx].m_cTong.m_nTongNationalEmblem;
			strcpy(PlayerSync.TongName, Player[m_nPlayerIdx].m_cTong.m_szName);
			strcpy(PlayerSync.TongAgname, Player[m_nPlayerIdx].m_cTong.m_szAgname);
		}
		else
		{
			PlayerSync.TongNameID = 0;
			PlayerSync.m_Figure = -1;
			PlayerSync.TongNationalEmblem = 0;
			PlayerSync.TongName[0] = 0;
			PlayerSync.TongAgname[0] = 0;
		}
		strcpy(PlayerSync.MateName, Player[m_nPlayerIdx].m_cTask.GetSaveStr(TASKVALUE_BASEDATA_MATENAME));
		PlayerSync.HorseType = m_bRideHorse ? (BYTE)m_HorseType : -1;
		PlayerSync.RankID = (BYTE)m_RankID;
		memcpy(&PlayerSync.ExpandRank, &m_CurExpandRank, sizeof(m_CurExpandRank));
		PlayerSync.Translife = (BYTE)m_byTranslife;
		PlayerSync.Viprank = (BYTE)Player[m_nPlayerIdx].m_cTask.GetSaveVal(TASKVALUE_STATTASK_VIPRANK);
		PlayerSync.Repute = Player[m_nPlayerIdx].m_cTask.GetSaveVal(TASKVALUE_STATTASK_REPUTE);
		PlayerSync.FuYuan = Player[m_nPlayerIdx].m_cTask.GetSaveVal(TASKVALUE_STATTASK_FUYUAN);
		PlayerSync.PKFlag = (BYTE)Player[m_nPlayerIdx].m_cPK.GetNormalPKState();
		PlayerSync.PKValue = (BYTE)Player[m_nPlayerIdx].m_cPK.GetPKValue();
		PlayerSync.ImagePlayer = (BYTE)Player[m_nPlayerIdx].m_ImagePlayer;
		PlayerSync.MantleLevel = (BYTE)m_byMantleLevel;
		PlayerSync.RankInWorld = Player[m_nPlayerIdx].m_nWorldStat;
		PlayerSync.FactionNumber = Player[m_nPlayerIdx].m_cFaction.GetCurFactionNo();
		PlayerSync.Trade = (PLAYERTRADE)Player[m_nPlayerIdx].m_PTrade;
		if (Player[m_nPlayerIdx].m_nPaceBarTime)
			PlayerSync.PacePer = (BYTE)(Player[m_nPlayerIdx].m_nPaceBarTime * MAX_PERCENT / Player[m_nPlayerIdx].m_nPaceBarTimeMax);
		else
			PlayerSync.PacePer = 0;
		PlayerSync.m_btSomeFlag = 0;
		if (m_FightMode)
			PlayerSync.m_btSomeFlag |= 0x01;
		if (Player[m_nPlayerIdx].m_bSleepMode)
			PlayerSync.m_btSomeFlag |= 0x02;

		INT nMaxCount = MAX_BROADCAST_COUNT;
		CURREGION.BroadCast(&PlayerSync, sizeof(PLAYER_NORMAL_SYNC), nMaxCount, m_MapX, m_MapY);
		for (j = 0; j < 8; j++)
		{
			INT nConRegion = CURREGION.m_nConnectRegion[j];
			if (nConRegion == -1)
				continue;
			SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&PlayerSync, sizeof(PLAYER_NORMAL_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
		}

		NPC_PLAYER_TYPE_NORMAL_SYNC	sSync;
		sSync.ProtocolType = s2c_syncnpcminplayer;
		sSync.ID = m_dwID;
		sSync.MapX = nMpsX;
		sSync.MapY = nMpsY;
		sSync.OffX = m_OffX;
		sSync.OffY = m_OffY;
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(sSync));
	}
}

void KNpc::BroadCastRevive(INT nType)
{
	if (!IsPlayer())
		return;

	if (m_RegionIndex < 0)
		return;

	NPC_REVIVE_SYNC	NpcReviveSync;
	NpcReviveSync.ProtocolType = s2c_playerrevive;
	NpcReviveSync.ID = m_dwID;
	NpcReviveSync.Type = (BYTE)nType;

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
	CURREGION.BroadCast((BYTE*)&NpcReviveSync, sizeof(NPC_REVIVE_SYNC), nMaxCount, m_MapX, m_MapY);
	INT j;
	for (j = 0; j < 8; j++)
	{
		INT nConRegion = CURREGION.m_nConnectRegion[j];
		if (nConRegion == -1)
			continue;
		SubWorld[m_SubWorldIndex].m_Region[nConRegion].BroadCast((BYTE*)&NpcReviveSync, sizeof(NPC_REVIVE_SYNC), nMaxCount, m_MapX - POff[j].x, m_MapY - POff[j].y);
	}
}

INT	KNpc::GetPlayerIdx()
{
	if (m_Kind != kind_player)
		return 0;
	return m_nPlayerIdx;
}

#endif

#ifndef _SERVER
#include "scene/KScenePlaceC.h"

INT KNpc::PaintInfo(INT nHeightOffset, INT nFontSize, DWORD dwBorderColor)
{
	if (m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex && m_HideState.nTime > 0)
	{
		return 0;
	}

	INT nMpsX, nMpsY, nXX, nYY;
	GetMpsPos(&nMpsX, &nMpsY);

	DWORD	dwColor = 0xffffffff;
	INT nHeightOff = nHeightOffset + nFontSize + 1;

	char pszTemp[128];

	switch (m_Kind)
	{
	case kind_player:
		switch (m_CurrentCamp)
		{
		case camp_begin:
			dwColor = 0xffffffff;
			break;
		case camp_justice:
			dwColor = 0xff000000 | (255 << 16) | (168 << 8) | 94;
			break;
		case camp_evil:
			dwColor = 0xff000000 | (255 << 16) | (146 << 8) | 255;
			break;
		case camp_balance:
			dwColor = 0xff000000 | (85 << 16) | (255 << 8) | 145;
			break;
		case camp_free:
			dwColor = 0xff000000 | (255 << 16);
			break;
		case camp_animal:
			dwColor = 0xffffffff;
			break;
		case camp_event:
			dwColor = 0xff000000 | (238 << 16) | (18 << 8) | 137;
			break;
		case camp_audience:
			dwColor = 0xffffffff;
			break;
		case camp_tongwar:
			dwColor = 0xff000000 | (104 << 16) | (38 << 8) | 174;
			break;
		}

		sprintf(pszTemp, "[CÊp:%d] %s", m_Level, Name);
		if (m_FreezeState.nTime || m_PoisonState.nTime || m_StunState.nTime)
		{
			strcat(pszTemp, " (");
			if (m_FreezeState.nTime)
				strcat(pszTemp, "ChËm");
			if (m_PoisonState.nTime)
				strcat(pszTemp, "§éc");
			if (m_StunState.nTime)
				strcat(pszTemp, "Mª");
			if (m_FrozenAction.nTime)
				strcat(pszTemp, "Cho¸ng");
			if (m_RandMove.nTime)
				strcat(pszTemp, "Lo¹n");
			if (m_HideState.nTime)
				strcat(pszTemp, "Èn");
			if (m_FrozenAction.nTime)
				strcat(pszTemp, "B¨ng");
			if (m_BurnState.nTime)
				strcat(pszTemp, "Báng");
			if (m_SilentState.nTime)
				strcat(pszTemp, "C©m");
			strcat(pszTemp, ")");
		}

		//sprintf(pszTemp, "%s [%d/%d]", pszTemp, nMpsX / 256, nMpsY / 512);
		nXX = nMpsX - nFontSize * g_StrLen(pszTemp) / 4 + ((m_byMantleLevel > 0 && m_byMantleLevel <= MAX_ITEM_LEVEL) ? 40 : 0);
		nYY = nMpsY;

		g_pRepresent->OutputText(nFontSize, pszTemp, KRF_ZERO_END, nXX, nYY, dwColor, 0, nHeightOff, dwBorderColor);

		if (m_byViprank > 0 && (m_byViprank <= MAX_VIPRANK_VALUE))
			nXX = PaintViprank(nHeightOff, nFontSize, nXX, nYY);
		if (m_byMantleLevel > 0 && (m_byMantleLevel <= MAX_ITEM_LEVEL))
			nXX = PaintMantle(nHeightOff, nFontSize, nXX, nYY);
		if (m_byTranslife > 0 && (m_byTranslife <= MAX_TRANSLIFE_VALUE))
			nXX = PaintTranslife(nHeightOff, nFontSize, nXX, nYY);

		nHeightOffset += nFontSize + 1;
		if (m_dwTongNameID)
		{
			if (m_szTongAgname[0])
				sprintf(pszTemp, "%s %s", m_szTongName, m_szTongAgname);
			else
			{
				switch (m_nFigure)
				{
				case enumTONG_FIGURE_MEMBER:
					sprintf(pszTemp, "%s %s", m_szTongName, defTONG_MEMBER_AGNAME);
					break;
				case enumTONG_FIGURE_MANAGER:
					sprintf(pszTemp, "%s %s", m_szTongName, defTONG_MANAGER_AGNAME);
					break;
				case enumTONG_FIGURE_DIRECTOR:
					sprintf(pszTemp, "%s %s", m_szTongName, defTONG_DIRECTOR_AGNAME);
					break;
				case enumTONG_FIGURE_MASTER:
					sprintf(pszTemp, "%s %s", m_szTongName, defTONG_MASTER_AGNAME);
					break;
				default:
					sprintf(pszTemp, "%s", m_szTongName);
					break;
				}
			}
			nXX = nMpsX - nFontSize * g_StrLen(pszTemp) / 4;
			nYY -= (nFontSize + 2) * 2;
			g_pRepresent->OutputText(nFontSize, pszTemp, KRF_ZERO_END, nXX, nYY, dwColor, 0, nHeightOff, dwBorderColor);

			if (m_nTongNationalEmblem)
			{
				KRUImage RUIconImage;
				RUIconImage.nType = ISI_T_SPR;
				RUIconImage.Color.Color_b.a = 255;
				RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
				RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
				RUIconImage.uImage = 0;
				RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
				strcpy(RUIconImage.szImage, PlayerSet.m_szNationalEmblemPic[m_nTongNationalEmblem]);
				RUIconImage.oPosition.nX = nXX - 18;
				RUIconImage.oPosition.nY = nYY - 12;
				RUIconImage.oPosition.nZ = nHeightOff;
				RUIconImage.nFrame = 0;
				g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, FALSE);
			}
			nHeightOffset += nFontSize + 1;

			/*char szTitleName[64] = "§éc C« CÇu B¹i";
			nXX = nMpsX - nFontSize * g_StrLen(szTitleName) / 4;
			nYY -= (nFontSize + 2) * 2;

			g_pRepresent->OutputText(nFontSize, szTitleName, KRF_ZERO_END, nXX, nYY, 0xffccaa00, 0, nHeightOff, dwBorderColor);*/
		}

		if (m_CurExpandRank.szName[0])
		{
			nYY -= (nFontSize + 4) * 2;

			g_pRepresent->OutputText(nFontSize, m_CurExpandRank.szName, KRF_ZERO_END,
				nMpsX - nFontSize * g_StrLen(m_CurExpandRank.szName) / 4, nYY,
				m_CurExpandRank.dwColor, 0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;
		}

		if (m_nRankInWorld > 0 && m_nRankInWorld <= 50)
		{
			switch (m_nRankInWorld)
			{
			case enumWORLD_RANK_TOP_1:
			{
				sprintf(pszTemp, "%s", defWorldRank_1);
				//nXX = PaintWorldRank(nHeightOff, nFontSize, nMpsX + 40, nYY + 200, enumWORLD_RANK_TOP_1);
			}
			break;
			case enumWORLD_RANK_TOP_2:
				sprintf(pszTemp, "%s", defWorldRank_2);
				//nXX = PaintWorldRank(nHeightOff, nFontSize, nMpsX + 43, nYY + 200, enumWORLD_RANK_TOP_2);
				break;
			case enumWORLD_RANK_TOP_3:
				sprintf(pszTemp, "%s", defWorldRank_3);
				//nXX = PaintWorldRank(nHeightOff, nFontSize, nMpsX + 40, nYY + 200, enumWORLD_RANK_TOP_3);
				break;
			default:
			{
				sprintf(pszTemp, "%s %d", defWorldRank_4, m_nRankInWorld);
			}
			break;
			}

			nXX = nMpsX - nFontSize * g_StrLen(pszTemp) / 4;
			nYY -= (nFontSize + 2) * 2;
			g_pRepresent->OutputText(nFontSize, pszTemp, KRF_ZERO_END, nXX, nYY, 0xffff0000, 0, nHeightOff, dwBorderColor);
		}

		if (m_MaskType)
		{
			pszTemp[0] = 0;

			if (m_bRideHorse)
			{
				g_GameSetting.GetString("Actions", "0", "", pszTemp, sizeof(pszTemp));
			}
			else if (m_Doing == do_sit)
			{
				g_GameSetting.GetString("Actions", "1", "", pszTemp, sizeof(pszTemp));
			}

			if (pszTemp[0])
			{
				KRUImage RUIconImage;
				RUIconImage.nType = ISI_T_SPR;
				RUIconImage.Color.Color_b.a = 255;
				RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
				RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
				RUIconImage.uImage = 0;
				RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
				strcpy(RUIconImage.szImage, pszTemp);
				RUIconImage.oPosition.nX = nMpsX + (nFontSize * g_StrLen(Name) / 4) + 10 + (m_byMantleLevel > 0 ? 40 : 0);
				RUIconImage.oPosition.nY = nMpsY + 20;
				RUIconImage.oPosition.nZ = nHeightOff;
				RUIconImage.nFrame = 0;
				g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, FALSE);
			}
		}
		break;
	case kind_dialoger:
		g_pRepresent->OutputText(nFontSize, Name, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
		nHeightOffset += nFontSize + 1;
		break;
	case kind_normal:
		g_GameSetting.GetString("Series", "Image", "", pszTemp, sizeof(pszTemp));

		KRUImage RUIconImageR;
		RUIconImageR.nType = ISI_T_SPR;
		RUIconImageR.Color.Color_b.a = 255;
		RUIconImageR.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		RUIconImageR.uImage = 0;
		RUIconImageR.nISPosition = IMAGE_IS_POSITION_INIT;
		RUIconImageR.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		sprintf(RUIconImageR.szImage, pszTemp, m_Series);
		RUIconImageR.oPosition.nX = nMpsX + nFontSize * g_StrLen(Name) / 4 + 5;
		RUIconImageR.oPosition.nY = nMpsY;
		RUIconImageR.oPosition.nZ = nHeightOff;
		RUIconImageR.nFrame = 0;
		g_pRepresent->DrawPrimitives(1, &RUIconImageR, RU_T_IMAGE, FALSE);
		switch (m_btSpecial)
		{
		case npc_normal:
			dwColor = 0xffffffff;
			break;
		case npc_blue:
			dwColor = 0x006569d7;
			break;
		case npc_gold:
			dwColor = 255 << 16 | 217 << 8 | 78;
			break;
		default:
			dwColor = 0xff000000;	// ½ðÉ«
			break;
		}
		g_pRepresent->OutputText(nFontSize, Name, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name) / 4, nMpsY, dwColor, 0, nHeightOff, dwBorderColor);
		break;
	}
	return nHeightOffset;
}

INT KNpc::PaintWorldRank(INT nHeightOff, INT nFontSize, INT nMpsX, INT nMpsY, INT nWorldRank)
{
	KRUImage RUIconImage;
	RUIconImage.nType = ISI_T_SPR;
	RUIconImage.Color.Color_b.a = 255;
	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImage.uImage = 0;
	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;

	if (nWorldRank == 1)
		strcpy(RUIconImage.szImage, "\\Spr\\Ui3\\vip\\thienhavs.spr");
	else if (nWorldRank == 2)
		strcpy(RUIconImage.szImage, "\\Spr\\Ui3\\vip\\hungbathienha.spr");
	else if (nWorldRank == 3)
		strcpy(RUIconImage.szImage, "\\Spr\\Ui3\\vip\\vangdanh.spr");
	KImageParam	Param = KImageParam();
	g_pRepresent->GetImageParam(RUIconImage.szImage, &Param, ISI_T_SPR);
	if (Param.nNumFrames > 0)
	{
		RUIconImage.oPosition.nX = nMpsX - Param.nWidth / 4 - 2;
		RUIconImage.oPosition.nY = nMpsY - Param.nHeight * 5;
		RUIconImage.oPosition.nZ = nHeightOff;
		RUIconImage.nFrame = g_SubWorldSet.GetGameTime() % Param.nNumFrames;
		g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	}

	return RUIconImage.oPosition.nX - Param.nWidth;
}

INT KNpc::PaintViprank(INT nHeightOff, INT nFontSize, INT nMpsX, INT nMpsY)
{
	KRUImage RUIconImage;
	RUIconImage.nType = ISI_T_SPR;
	RUIconImage.Color.Color_b.a = 255;
	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImage.uImage = 0;
	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	strcpy(RUIconImage.szImage, PlayerSet.m_szViprankPic[m_byViprank]);
	KImageParam	Param = KImageParam();
	g_pRepresent->GetImageParam(RUIconImage.szImage, &Param, ISI_T_SPR);
	if (Param.nNumFrames > 0)
	{
		RUIconImage.oPosition.nX = nMpsX - Param.nWidth / 4 - 2;
		RUIconImage.oPosition.nY = (nMpsY - Param.nHeight * 5);
		RUIconImage.oPosition.nZ = nHeightOff;
		RUIconImage.nFrame = g_SubWorldSet.GetGameTime() % Param.nNumFrames;
		g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	}

	return RUIconImage.oPosition.nX - Param.nWidth;
}

INT KNpc::PaintMantle(INT nHeightOff, INT nFontSize, INT nMpsX, INT nMpsY)
{
	KRUImage RUIconImage;
	RUIconImage.nType = ISI_T_SPR;
	RUIconImage.Color.Color_b.a = 255;
	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImage.uImage = 0;
	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	strcpy(RUIconImage.szImage, PlayerSet.m_szFortuneRankPic[m_byMantleLevel]);
	KImageParam	Param = KImageParam();;
	g_pRepresent->GetImageParam(RUIconImage.szImage, &Param, ISI_T_SPR);
	if (Param.nNumFrames > 0)
	{
		RUIconImage.oPosition.nX = nMpsX - Param.nWidth;
		RUIconImage.oPosition.nY = nMpsY - Param.nHeight / 2 - 4;
		RUIconImage.oPosition.nZ = nHeightOff;
		RUIconImage.nFrame = g_SubWorldSet.GetGameTime() % Param.nNumFrames;
		g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	}

	return RUIconImage.oPosition.nX;
}

INT KNpc::PaintTranslife(INT nHeightOff, INT nFontSize, INT nMpsX, INT nMpsY)
{
	KRUImage RUIconImage;
	RUIconImage.nType = ISI_T_SPR;
	RUIconImage.Color.Color_b.a = 255;
	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	RUIconImage.uImage = 0;
	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	strcpy(RUIconImage.szImage, PlayerSet.m_szTranlifePic[m_byTranslife]);
	KImageParam	Param = KImageParam();;
	g_pRepresent->GetImageParam(RUIconImage.szImage, &Param, ISI_T_SPR);
	if (Param.nNumFrames > 0)
	{
		RUIconImage.oPosition.nX = nMpsX - (Param.nWidth / 4) - 2;
		RUIconImage.oPosition.nY = nMpsY + Param.nHeight / 2;
		RUIconImage.oPosition.nZ = nHeightOff;
		RUIconImage.nFrame = g_SubWorldSet.GetGameTime() % Param.nNumFrames;
		g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 0);
	}

	return RUIconImage.oPosition.nX;
}

void KNpc::PaintTop(INT nHeightOffset, INT nnHeightOffset, INT nFontSize, DWORD dwBorderColor)
{
	if (m_Kind != kind_player)
		return;

	INT	nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	if (m_PTrade.nTrade)
	{
		INT nWid = nFontSize * g_StrLen(m_PTrade.cName) / 2 + 10;
		INT nHei = nFontSize + 12;

		KRUImage RUIconImageR;
		RUIconImageR.nType = ISI_T_SPR;
		RUIconImageR.Color.Color_b.a = 150;
		RUIconImageR.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		RUIconImageR.uImage = 0;
		RUIconImageR.nISPosition = IMAGE_IS_POSITION_INIT;
		RUIconImageR.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		strcpy(RUIconImageR.szImage, "\\spr\\Ui3\\°ÚÌ¯\\°ÚÌ¯Í·¶¥Ìõ£­ÖÐ.spr");
		RUIconImageR.oPosition.nX = nMpsX - nWid / 2;
		RUIconImageR.oPosition.nY = nMpsY - 40;
		RUIconImageR.oPosition.nZ = nHeightOffset + 10;
		RUIconImageR.nFrame = 0;
		for (INT i = 0; i < nWid; i++)
		{
			RUIconImageR.oPosition.nX = nMpsX - nWid / 2 + i;
			g_pRepresent->DrawPrimitives(1, &RUIconImageR, RU_T_IMAGE, FALSE);
		}

		DWORD dwColor = 0x00eed66a;
		g_pRepresent->OutputText(nFontSize, m_PTrade.cName, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(m_PTrade.cName) / 4, RUIconImageR.oPosition.nY + nFontSize * 2, dwColor, 0, RUIconImageR.oPosition.nZ + nFontSize - 1, dwBorderColor);

		strcpy(RUIconImageR.szImage, "\\spr\\Ui3\\°ÚÌ¯\\°ÚÌ¯Í·¶¥Ìõ£­ÓÒ.spr");
		RUIconImageR.nType = ISI_T_SPR;
		RUIconImageR.Color.Color_b.a = 255;
		RUIconImageR.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		RUIconImageR.uImage = 0;
		RUIconImageR.nISPosition = IMAGE_IS_POSITION_INIT;
		RUIconImageR.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		RUIconImageR.oPosition.nX = nMpsX + nWid / 2;
		RUIconImageR.oPosition.nY = nMpsY;
		RUIconImageR.oPosition.nZ += 23;
		g_pRepresent->DrawPrimitives(1, &RUIconImageR, RU_T_IMAGE, FALSE);

		strcpy(RUIconImageR.szImage, "\\spr\\Ui3\\°ÚÌ¯\\°ÚÌ¯Í·¶¥Ìõ£­×ó.spr");
		RUIconImageR.nType = ISI_T_SPR;
		RUIconImageR.Color.Color_b.a = 255;
		RUIconImageR.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		RUIconImageR.uImage = 0;
		RUIconImageR.nISPosition = IMAGE_IS_POSITION_INIT;
		RUIconImageR.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		RUIconImageR.oPosition.nX = nMpsX - nWid / 2 - 18;
		RUIconImageR.oPosition.nY = nMpsY;
		g_pRepresent->DrawPrimitives(1, &RUIconImageR, RU_T_IMAGE, FALSE);
	}
	if (m_nPacePercent)
	{
		INT nPercent = MAX_PERCENT - m_nPacePercent;
		if (nPercent < 0)
			nPercent = 0;
		KRUShadow Shadow;
		Shadow.Color.Color_b.a = 0;
		Shadow.Color.Color_b.r = 238;
		Shadow.Color.Color_b.g = 18;
		Shadow.Color.Color_b.b = 137;
		Shadow.oPosition.nX = nMpsX - SHOW_LIFE_WIDTH / 2;
		Shadow.oPosition.nY = nMpsY;
		Shadow.oPosition.nZ = nnHeightOffset + SHOW_LIFE_HEIGHT - 3;
		Shadow.oEndPos.nX = Shadow.oPosition.nX + SHOW_LIFE_WIDTH * nPercent / MAX_PERCENT;
		Shadow.oEndPos.nY = nMpsY - 2;
		Shadow.oEndPos.nZ = nnHeightOffset - 4;
		g_pRepresent->DrawPrimitives(1, &Shadow, RU_T_SHADOW, FALSE);

		Shadow.Color.Color_dw = 0x16000000;
		Shadow.oPosition.nX = Shadow.oEndPos.nX;
		Shadow.oEndPos.nX = nMpsX + SHOW_LIFE_WIDTH / 2;
		g_pRepresent->DrawPrimitives(1, &Shadow, RU_T_SHADOW, FALSE);
	}
}

INT	KNpc::PaintChat(INT nHeightOffset)
{
	if (m_Kind != kind_player && m_Kind != kind_dialoger && m_Kind != kind_normal)
		return nHeightOffset;
	if (m_nChatContentLen <= 0)
		return nHeightOffset;
	if (m_nChatNumLine <= 0)
		return nHeightOffset;
	if (m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex &&
		m_HideState.nTime > 0)
		return 0;

	INT nFontSize = 12;
	INT					nWidth, nHeight;
	INT					nMpsX, nMpsY;
	KOutputTextParam	sParam;
	sParam.BorderColor = 0;

	sParam.nNumLine = m_nChatNumLine;

	nWidth = m_nChatFontWidth * nFontSize / 2;
	nHeight = sParam.nNumLine * (nFontSize + 1);

	nWidth += 6;	//ÎªÁËºÃ¿´
	nHeight += 10;	//ÎªÁËºÃ¿´

	GetMpsPos(&nMpsX, &nMpsY);
	sParam.nX = nMpsX - nWidth / 2;
	sParam.nY = nMpsY;
	sParam.nZ = nHeightOffset + nHeight;
	sParam.Color = SHOW_CHAT_COLOR;
	sParam.nSkipLine = 0;
	sParam.nVertAlign = 0;

	sParam.bPicPackInSingleLine = true;
	g_pRepresent->OutputRichText(nFontSize, &sParam, m_szChatBuffer, m_nChatContentLen, nWidth);

	return sParam.nZ;
}

#include "../../Engine/Src/Text.h"
INT	KNpc::SetChatInfo(const char* Name, const char* pMsgBuff, uint16_t nMsgLength)
{
	INT nFontSize = 12;

	char szChatBuffer[MAX_SENTENCE_LENGTH];

	memset(szChatBuffer, 0, sizeof(szChatBuffer));

	if (nMsgLength)
	{
		INT nOffset = 0;
		if (pMsgBuff[0] != KTC_TAB)
		{
			szChatBuffer[nOffset] = (char)KTC_COLOR;
			nOffset++;
			szChatBuffer[nOffset] = (char)0xFF;
			nOffset++;
			szChatBuffer[nOffset] = (char)0xFF;
			nOffset++;
			szChatBuffer[nOffset] = (char)0x00;
			nOffset++;
			strncpy(szChatBuffer + nOffset, Name, 32);
			nOffset += strlen(Name);
			szChatBuffer[nOffset] = ':';
			nOffset++;
			szChatBuffer[nOffset] = (char)0x20;
			nOffset++;
			szChatBuffer[nOffset] = (char)KTC_COLOR_RESTORE;
			nOffset++;
		}
		else
		{
			pMsgBuff++;
			nMsgLength--;
		}

		if (nMsgLength)
		{
			memcpy(szChatBuffer + nOffset, pMsgBuff, nMsgLength);
			nOffset += nMsgLength;

			memset(m_szChatBuffer, 0, sizeof(m_szChatBuffer));
			m_nChatContentLen = MAX_SENTENCE_LENGTH;
			TGetLimitLenEncodedString(szChatBuffer, nOffset, nFontSize, SHOW_CHAT_WIDTH,
				m_szChatBuffer, m_nChatContentLen, m_Kind == kind_player ? 2 : 6, true);

			m_nChatNumLine = TGetEncodedTextLineCount(m_szChatBuffer, m_nChatContentLen, SHOW_CHAT_WIDTH, m_nChatFontWidth, nFontSize, 0, 0, true);
			if (m_Kind == kind_player && m_nChatNumLine >= 2)
				m_nChatNumLine = 2;
			m_nCurChatTime = IR_GetCurrentTime();
			return true;
		}
	}
	return false;
}

INT	KNpc::PaintLife(INT nHeightOffset, bool bSelect)
{
	if (!bSelect && (m_Kind != kind_player && m_Kind != kind_partner))
		return nHeightOffset;

	if (m_CurrentLifeMax <= 0)
		return nHeightOffset;

	if (m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex &&
		m_HideState.nTime > 0)
		return nHeightOffset;

	/*	if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
			(m_Kind == kind_player ||
			 m_Kind == kind_partner)
			)
			return nHeightOffset;*/

	INT	nMpsX, nMpsY;
	GetMpsPos(&nMpsX, &nMpsY);
	INT nWid = SHOW_LIFE_WIDTH;
	//if (Npc[m_Index].m_btSpecial >= npc_gold)
	//	nWid += 12;
	INT nHei = SHOW_LIFE_HEIGHT;
	//if (Npc[m_Index].m_btSpecial >= npc_gold)
	//	nHei += 2;
	KRUShadow	Shadow;
	INT nPercent = static_cast<INT>(static_cast<double>(m_CurrentLife) / m_CurrentLifeMax * MAX_PERCENT);

	if (nPercent > MAX_PERCENT)
		nPercent = MAX_PERCENT;
	else if (nPercent < 0)
		return nHeightOffset;

	if (nPercent >= 50)
	{
		Shadow.Color.Color_b.r = 0;
		Shadow.Color.Color_b.g = 255;
		Shadow.Color.Color_b.b = 0;
	}
	else if (nPercent >= 25)
	{
		Shadow.Color.Color_b.r = 255;
		Shadow.Color.Color_b.g = 255;
		Shadow.Color.Color_b.b = 0;
	}
	else
	{
		Shadow.Color.Color_b.r = 255;
		Shadow.Color.Color_b.g = 0;
		Shadow.Color.Color_b.b = 0;
	}

	if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
		(m_Kind == kind_player || m_Kind == kind_partner))
	{
		Shadow.Color.Color_b.r = 231;
		Shadow.Color.Color_b.g = 194;
		Shadow.Color.Color_b.b = 0;
	}

	if (Npc[m_Index].m_nPKFlag == enumPKMurder ||
		Player[Npc[m_Index].m_nPlayerIdx].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING)
	{
		Shadow.Color.Color_b.r = 238;
		Shadow.Color.Color_b.g = 18;
		Shadow.Color.Color_b.b = 137;
	}
	if (Npc[m_Index].m_nPKFlag == enumPKTongWar)
	{
		Shadow.Color.Color_b.r = 170;
		Shadow.Color.Color_b.g = 30;
		Shadow.Color.Color_b.b = 255;
	}
	if (Npc[m_Index].m_nPKValue >= 10)
	{
		Shadow.Color.Color_b.r = 255;
		Shadow.Color.Color_b.g = 0;
		Shadow.Color.Color_b.b = 66;
	}

	Shadow.Color.Color_b.a = 0;
	Shadow.oPosition.nX = nMpsX - nWid / 2;
	Shadow.oPosition.nY = nMpsY;
	Shadow.oPosition.nZ = nHeightOffset + nHei;
	Shadow.oEndPos.nX = Shadow.oPosition.nX + nWid * nPercent / MAX_PERCENT;
	Shadow.oEndPos.nY = nMpsY;
	Shadow.oEndPos.nZ = nHeightOffset;
	g_pRepresent->DrawPrimitives(1, &Shadow, RU_T_SHADOW, FALSE);

	//Shadow.Color.Color_dw = 0x16000000;
	Shadow.Color.Color_b.r = 128;
	Shadow.Color.Color_b.g = 128;
	Shadow.Color.Color_b.b = 128;
	Shadow.oPosition.nX = Shadow.oEndPos.nX;
	Shadow.oEndPos.nX = nMpsX + nWid / 2;
	g_pRepresent->DrawPrimitives(1, &Shadow, RU_T_SHADOW, FALSE);

	return nHeightOffset + nHei;
}

void KNpc::Paint()
{
	if (m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex && (m_CurrentCamp == camp_audience || m_HideState.nTime > 0))
	{
		return;
	}

	BOOL bPaintBody = TRUE;

	if (Option.GetLow(LowNpc))
	{
		if (m_Kind == kind_normal || m_Kind == kind_dialoger)
		{
			if (m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{
				bPaintBody = FALSE;
			}
		}
	}

	if (m_ResDir != m_Dir)
	{
		INT nDirOff = m_Dir - m_ResDir;
		if (nDirOff > 32)
			nDirOff -= 64;
		else if (nDirOff < -32)
			nDirOff += 64;
		m_ResDir += nDirOff / 2;
		if (m_ResDir >= 64)
			m_ResDir -= 64;
		if (m_ResDir < 0)
			m_ResDir += 64;
	}
	m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame, FALSE, bPaintBody);

	INT nHeight = GetNpcPate() + GetNpcPatePeopleInfo();
	if (m_CurExpandRank.szName[0])
		nHeight += NORMAL_FONTSIZE + 1;
	if (m_dwTongNameID)
		nHeight += NORMAL_FONTSIZE + 1;
	DrawMenuState(nHeight);
}
#endif

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó»ù±¾×î´óÉúÃüµã
//--------------------------------------------------------------------------
void	KNpc::AddBaseLifeMax(INT nLife)
{
	m_LifeMax += nLife;
	m_CurrentLifeMax = m_LifeMax;
}

void	KNpc::SetBaseLifeMax(INT nLifeMax)
{
	m_LifeMax = nLifeMax;
	m_CurrentLifeMax = m_LifeMax;
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Óµ±Ç°×î´óÉúÃüµã
//--------------------------------------------------------------------------
void	KNpc::AddCurLifeMax(INT nLife)
{
	m_CurrentLifeMax += nLife;
}


//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó»ù±¾×î´óÌåÁ¦µã
//--------------------------------------------------------------------------
void	KNpc::AddBaseStaminaMax(INT nStamina)
{
	m_StaminaMax += nStamina;
	m_CurrentStaminaMax = m_StaminaMax;
}

void	KNpc::SetBaseStaminaMax(INT nStamina)
{
	m_StaminaMax = nStamina;
	m_CurrentStaminaMax = m_StaminaMax;
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Óµ±Ç°×î´óÌåÁ¦µã
//--------------------------------------------------------------------------
void	KNpc::AddCurStaminaMax(INT nStamina)
{
	m_CurrentStaminaMax += nStamina;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó»ù±¾×î´óÄÚÁ¦µã
//--------------------------------------------------------------------------
void	KNpc::AddBaseManaMax(INT nMana)
{
	m_ManaMax += nMana;
	m_CurrentManaMax = m_ManaMax;
}

void	KNpc::SetBaseManaMax(INT nMana)
{
	m_ManaMax = nMana;
	m_CurrentManaMax = m_ManaMax;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Óµ±Ç°×î´óÄÚÁ¦µã
//--------------------------------------------------------------------------
void	KNpc::AddCurManaMax(INT nMana)
{
	m_CurrentManaMax += nMana;
}

/*
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÖØÐÂ¼ÆËãÉúÃü»Ø¸´ËÙ¶È
//--------------------------------------------------------------------------
void	KNpc::ResetLifeReplenish()
{
	m_LifeReplenish = (m_Level + 5) / 6;
	m_CurrentLifeReplenish = m_LifeReplenish;
}
*/

/*
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°×î´óÉúÃüµã
//--------------------------------------------------------------------------
void	KNpc::CalcCurLifeMax()
{
}
*/

/*
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°×î´óÌåÁ¦µã
//--------------------------------------------------------------------------
void	KNpc::CalcCurStaminaMax()
{
	m_CurrentStaminaMax = m_StaminaMax;		// »¹ÐèÒª¼ÓÉÏ ×°±¸¡¢¼¼ÄÜ¡¢Ò©Îï£¨ÁÙÊ±£©µÈµÄÓ°Ïì
}
*/

/*
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°×î´óÄÚÁ¦µã
//--------------------------------------------------------------------------
void	KNpc::CalcCurManaMax()
{
	m_CurrentManaMax = m_ManaMax;			// »¹ÐèÒª¼ÓÉÏ ×°±¸¡¢¼¼ÄÜ¡¢Ò©Îï£¨ÁÙÊ±£©µÈµÄÓ°Ïì
}
*/

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°ÉúÃü»Ø¸´ËÙ¶È
//--------------------------------------------------------------------------
void	KNpc::CalcCurLifeReplenish()
{
	m_CurrentLifeReplenish = m_LifeReplenish;	// Óë½ÇÉ«Ïµ±ð¡¢½ÇÉ«µÈ¼¶ºÍÊÇ·ñÊ¹ÓÃÒ©¼Á¡¢¼¼ÄÜºÍÄ§·¨×°±¸ÓÐ¹Ø
}

void	KNpc::CalcCurLucky()
{
	m_CurrentLucky = Player[m_nPlayerIdx].m_nLucky;	// Óë½ÇÉ«Ïµ±ð¡¢½ÇÉ«µÈ¼¶ºÍÊÇ·ñÊ¹ÓÃÒ©¼Á¡¢¼¼ÄÜºÍÄ§·¨×°±¸ÓÐ¹Ø
}

void	KNpc::Remove()
{
	/*	m_LoopFrames = 0;
		m_Index = 0;
		m_PlayerIdx = -1;
		m_Kind = 0;
		m_dwID = 0;
		Name[0] = 0;*/
	Init();
#ifndef _SERVER
	m_DataRes.Remove(m_Index);
#endif
}

#ifndef _SERVER
void	KNpc::RemoveRes()
{
	m_DataRes.Remove(m_Index);
}
#endif
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨´Ë npc µÄÎåÐÐÊôÐÔ£¨ÄÚÈÝ»¹Ã»Íê³É£©not end
//--------------------------------------------------------------------------
void	KNpc::SetSeries(INT nSeries)
{
	m_Series = nSeries;
}
/*!*****************************************************************************
// Function		: KNpc::SetStateSkill
// Purpose		:
// Return		: void
// Argumant		: INT nSkillID
// Argumant		: INT nLevel
// Argumant		: void *pData
// Argumant		: INT nDataNum
// Argumant		: INT nTime -1±íÊ¾±»¶¯¼¼ÄÜ£¬Ê±¼äÎÞÏÞ
// Comments		:
// Author		: Spe
*****************************************************************************/
void KNpc::SetStateSkillEffect(INT nLauncher, INT nSkillID, INT nLevel, void* pData, INT nDataNum, INT nTime/* = -1*/, BOOL bOverLook/* = FALSE*/)
{
	if (nLevel <= 0 || nSkillID <= 0)
		return;

	_ASSERT(nSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
	KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nSkillID, nLevel);

	_ASSERT(nDataNum < MAX_SKILL_STATE);
	if (nDataNum >= MAX_SKILL_STATE)
		nDataNum = MAX_SKILL_STATE;
#ifdef _SERVER
	if (pData && nDataNum >= 0)
	{
		STATE_EFFECT_SYNC	Sync;
		Sync.ProtocolType = s2c_syncstateeffect;
		Sync.m_dwSkillID = nSkillID;
		Sync.m_nLevel = nLevel;
		Sync.m_nTime = nTime;
		Sync.m_bOverLook = bOverLook;
		memcpy(Sync.m_MagicAttrib, pData, sizeof(KMagicAttrib) * nDataNum);
		Sync.m_wLength = sizeof(Sync) - sizeof(KMagicAttrib) * (MAX_SKILL_STATE - nDataNum) - 1;
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &Sync, Sync.m_wLength + 1);
	}
#endif

	KStateNode* pNode;
	KMagicAttrib* pTemp = NULL;

	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		if (pNode->m_SkillID == nSkillID)
		{
			if (pNode->m_Level == nLevel)
			{
				pNode->m_LeftTime = nTime;
				pNode->m_bOverLook = bOverLook;
				pNode->m_bTempStateGraphics = FALSE;
			}
			else if (pNode->m_Level < nLevel)
			{
				pTemp = (KMagicAttrib*)pData;
				for (INT i = 0; i < nDataNum; i++)
				{
					// Çå³ýÔ­¼¼ÄÜµÄÓ°Ïì
					ModifyAttrib(nLauncher, &pNode->m_State[i]);
					// °ÑÐÂµÈ¼¶ÏÂ¼¼ÄÜµÄÓ°Ïì¼ÆËãµ½NPCÉíÉÏ
					ModifyAttrib(nLauncher, pTemp);
					pNode->m_State[i].nAttribType = pTemp->nAttribType;
					pNode->m_State[i].nValue[0] = -pTemp->nValue[0];
					pNode->m_State[i].nValue[1] = -pTemp->nValue[1];
					pNode->m_State[i].nValue[2] = -pTemp->nValue[2];
					pTemp++;
				}
			}
			return;
		}
		pNode = (KStateNode*)pNode->GetNext();
	}
	// Ã»ÓÐÔÚÑ­»·ÖÐ·µ»Ø£¬ËµÃ÷ÊÇÐÂ¼¼ÄÜ
	pNode = new KStateNode;
	pNode->m_SkillID = nSkillID;
	pNode->m_Level = nLevel;
	pNode->m_LeftTime = nTime;
	pNode->m_bOverLook = bOverLook;
	pNode->m_bTempStateGraphics = FALSE;
	if (pOrdinSkill)
		pNode->m_StateGraphics = pOrdinSkill->GetStateSpecailId();
	else
		pNode->m_StateGraphics = 0;
	pTemp = (KMagicAttrib*)pData;
	for (INT i = 0; i < nDataNum; i++)
	{
		// µ÷ÕûNPCÊôÐÔ
		ModifyAttrib(nLauncher, pTemp);
		// °ÑÏà·´Öµ¼ÓÈëÁ´±íÖÐÒÔ¹©ÒÆ³ýÊ±Ê¹ÓÃ
		pNode->m_State[i].nAttribType = pTemp->nAttribType;
		pNode->m_State[i].nValue[0] = -pTemp->nValue[0];
		pNode->m_State[i].nValue[1] = -pTemp->nValue[1];
		pNode->m_State[i].nValue[2] = -pTemp->nValue[2];
		pTemp++;
	}
	m_StateSkillList.AddTail(pNode);
#ifdef _SERVER	
	UpdateNpcStateInfo();
#endif
}

//Synchronize client usage skills
void KNpc::SysnCastSkillEffect(int nLauncher, int nSkillID, int nParam1, int nParam2, int nLevel)
{
	if (nLevel <= 0 || nSkillID <= 0)
		return;
	if (m_SubWorldIndex < 0 || m_SubWorldIndex >= MAX_SUBWORLD || m_RegionIndex < 0)
		return;
#ifdef _SERVER
	// 如果是玩家 就发协议
	if (IsPlayer())
	{
		NPC_SKILL_SYNC SkillCmd;
		SkillCmd.ProtocolType = s2c_castskilldirectly; // 立即使用技能
		SkillCmd.ID = this->m_dwID;
		SkillCmd.nSkillID = nSkillID;
		SkillCmd.nSkillLevel = nLevel;
		SkillCmd.nMpsX = nParam1; //-1
		SkillCmd.nMpsY = nParam2; // this->m_dwID

		// BroadCastState();             //同步状态给周围玩家

		// g_pServer->PackDataToClient(Player[GetPlayerIdx()].m_nNetConnectIdx,  (BYTE*)&SkillCmd, sizeof(SkillCmd));
		POINT POff[8] =
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
		int nMaxCount = MAX_BROADCAST_COUNT; //;MAX_PLAYER    CURREGION MAX_BROADCAST_COUNT
		CURREGION.BroadCast(&SkillCmd, sizeof(NPC_SKILL_SYNC), nMaxCount, m_MapX, m_MapY);

		int i;
		for (i = 0; i < 8; ++i)
		{
			if (CONREGIONIDX(i) == -1)
				continue;
			CONREGION(i).BroadCast(&SkillCmd, sizeof(NPC_SKILL_SYNC), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
		}
	}

#endif
}

/*!*****************************************************************************
// Function		: KNpc::ModifyMissleCollsion
// Purpose		:
// Return		: BOOL
// Argumant		: BOOL bCollsion
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KNpc::ModifyMissleCollsion(BOOL bCollsion)
{
	if (bCollsion)
		return TRUE;

	//if (g_RandPercent(m_CurrentPiercePercent))
	//	return TRUE;
	else
		return FALSE;
}

INT KNpc::ModifyMissleLifeTime(INT nLifeTime)
{
	if (IsPlayer())
	{
		//return Player[m_PlayerIdx].GetWeapon().GetRange();
		return nLifeTime;
	}
	else
	{
		return nLifeTime;
	}
}

INT	KNpc::ModifyMissleSpeed(INT nSpeed)
{
	if (m_CurrentSlowMissle)
	{
		return nSpeed / 2;
	}
	return nSpeed;
}

BOOL KNpc::DoBlurMove()
{
	if (m_Doing == do_hurt || m_Doing == do_death)
		return FALSE;

	if (m_SkillParam1 <= 0)
		return FALSE;

#ifndef _SERVER
	if (m_RegionIndex < 0 || m_RegionIndex >= 9)
	{
		_ASSERT(0);
		DoStand();
		return FALSE;
	}
#else
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return FALSE;
#endif

	KSkill* pSkill = (KSkill*)GetActiveSkill();
	if (!pSkill)
		return FALSE;

	m_ProcessAI = 0;

	switch (m_SpecialSkillStep)
	{
	case 0:
	{
		INT nX, nY;
		GetMpsPos(&nX, &nY);

		if (nX == m_DesX && nY == m_DesY)
		{
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
			return FALSE;
		}

		INT nDir = g_GetDirIndex(nX, nY, m_DesX, m_DesY);
		INT	nMaxLength = m_SkillParam1;
		INT	nWantLength = SubWorld[m_SubWorldIndex].GetDistance(nX, nY, m_DesX, m_DesY);
		INT	nSin = g_DirSin(nDir, 64);
		INT	nCos = g_DirCos(nDir, 64);

		if (nWantLength > nMaxLength)
		{
			m_DesX = nX + ((nMaxLength * nCos) >> 10);
			m_DesY = nY + ((nMaxLength * nSin) >> 10);
			nWantLength = nMaxLength;
		}
		else if (nWantLength <= MIN_DOMELEE_RANGE)
		{
			return FALSE;
		}

		INT nStep = nWantLength / MIN_BLURMOVE_SPEED;

		INT nTestX = 0;
		INT nTestY = 0;
		INT nSuccessStep = 0;

		for (INT i = 1; i < nStep + 1; i++)
		{
			nTestX = nX + ((MIN_BLURMOVE_SPEED * nCos * i) >> 10);
			nTestY = nY + ((MIN_BLURMOVE_SPEED * nSin * i) >> 10);
			INT nBarrier = SubWorld[m_SubWorldIndex].GetBarrier(nTestX, nTestY);
			DWORD dwTrap = SubWorld[m_SubWorldIndex].GetTrap(nTestX, nTestY);
			if (Obstacle_NULL == nBarrier && dwTrap == 0)
			{
				nSuccessStep = i;
			}
			if (Obstacle_Normal == nBarrier || Obstacle_Fly == nBarrier || dwTrap)
			{
				if (nSuccessStep <= MIN_DOMELEE_RANGE / MIN_BLURMOVE_SPEED)
				{
					DoStand();
					m_ProcessAI = 1;
					m_SpecialSkillStep = 0;
					return FALSE;
				}
				m_DesX = nX + ((MIN_BLURMOVE_SPEED * nCos * nSuccessStep) >> 10);
				m_DesY = nY + ((MIN_BLURMOVE_SPEED * nSin * nSuccessStep) >> 10);
				nStep = nSuccessStep;
				break;
			}
		}
	}
	m_Doing = do_blurmove;
	break;
	case 1:
		m_ProcessAI = 0;
		m_Frames.nTotalFrame = pSkill->GetMissleGenerateTime(0);
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_blurmove;
		break;
	case 2:
	{
#ifndef _SERVER
		INT nX, nY;
		GetMpsPos(&nX, &nY);
#endif
		INT nOldRegion = m_RegionIndex;
		INT nOldMapX = m_MapX;
		INT nOldMapY = m_MapY;
		INT nOldOffX = m_OffX;
		INT nOldOffY = m_OffY;

		if (!m_bClientOnly)
			CURREGION.DecNpcRef(m_MapX, m_MapY, obj_npc);

		SubWorld[m_SubWorldIndex].Mps2Map(m_DesX, m_DesY, &m_RegionIndex, &m_MapX, &m_MapY, &m_OffX, &m_OffY);

		if (!m_bClientOnly && m_RegionIndex >= 0)
			CURREGION.AddNpcRef(m_MapX, m_MapY, obj_npc);


		if (m_RegionIndex == -1)
		{
			m_RegionIndex = nOldRegion;
			m_MapX = nOldMapX;
			m_MapY = nOldMapY;
			m_OffX = nOldOffX;
			m_OffY = nOldOffY;
			CURREGION.AddNpcRef(m_MapX, m_MapY, obj_npc);
			return FALSE;
		}

		if (nOldRegion != m_RegionIndex)
		{
#ifdef _SERVER
			SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, m_RegionIndex, m_Index);
			if (IsPlayer())
			{
				SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, m_RegionIndex, m_nPlayerIdx);
			}
#else
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
			m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
#endif
		}
#ifndef _SERVER
		m_DataRes.CreateBlur(m_Index, g_GetDistance(nX, nY, m_DesX, m_DesY), m_Dir);
#endif
	}
	break;
	}
	return TRUE;
}

void KNpc::OnBlurMove()
{
	if (m_SpecialSkillStep == 0)
	{
		m_SpecialSkillStep++;
		DoBlurMove();
	}
	else if (m_SpecialSkillStep == 1)
	{
		if (WaitForFrame())
		{
			m_SpecialSkillStep++;
			DoBlurMove();
		}
	}
	else
	{
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
	}
}

BOOL KNpc::DoManyAttack()
{
	m_ProcessAI = 0;

	KSkill* pSkill = (KSkill*)GetActiveSkill();
	if (!pSkill)
		return FALSE;

	if (pSkill->GetChildSkillNum() <= m_SpecialSkillStep)
		goto ExitManyAttack;
#ifndef _SERVER
	m_DataRes.SetBlur(TRUE);
#endif

	m_Frames.nTotalFrame = pSkill->GetMissleGenerateTime(m_SpecialSkillStep);

	INT x, y;
	SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
	//	m_DesX = x;
	//	m_DesY = y;


#ifndef _SERVER
	if (m_nPlayerIdx > 0)
		pSkill->PlayPreCastSound(m_nSex, x, y);
	if (g_Random(2))
		m_ClientDoing = cdo_attack;
	else
		m_ClientDoing = cdo_attack1;
#endif


	m_Doing = do_manyattack;

	m_Frames.nCurrentFrame = 0;

	return TRUE;

ExitManyAttack:

#ifndef _SERVER
	m_DataRes.SetBlur(FALSE);
#endif
	DoStand();
	m_ProcessAI = 1;
	m_SpecialSkillStep = 0;

	return TRUE;
}

void KNpc::OnManyAttack()
{
	if (WaitForFrame())
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		KSkill* pSkill = (KSkill*)GetActiveSkill();
		if (!pSkill)
			return;

		INT nPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill(); Changed 

		if (nPhySkillId > 0)
		{
			KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
			if (pOrdinSkill)
			{
				pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
			}
		}
		m_SpecialSkillStep++;
		DoManyAttack();

	}
}

BOOL	KNpc::DoRunAttack()
{
	m_ProcessAI = 0;

#ifdef _SERVER
	Npc[Player[m_nPlayerIdx].m_nIndex].SendSyncData(Player[m_nPlayerIdx].m_nNetConnectIdx, TRUE);
#endif

	switch (m_SpecialSkillStep)
	{
	case 0:
		m_Frames.nTotalFrame = m_RunSpeed;
		m_ProcessAI = 0;

#ifndef _SERVER
		m_DataRes.SetBlur(TRUE);

		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightrun;
		}
		else
		{
			m_ClientDoing = cdo_run;
		}
#endif

		if (m_DesX < 0 && m_DesY > 0)
		{
			INT x, y;
			SubWorld[m_SubWorldIndex].Map2Mps
			(
				Npc[m_DesY].m_RegionIndex,
				Npc[m_DesY].m_MapX,
				Npc[m_DesY].m_MapY,
				Npc[m_DesY].m_OffX,
				Npc[m_DesY].m_OffY,
				&x,
				&y
			);

			m_DesX = x;
			m_DesY = y;
		}

		m_Frames.nCurrentFrame = 0;
		m_Doing = do_runattack;
		break;

	case 1:
#ifndef _SERVER
		if (g_Random(2))
			m_ClientDoing = cdo_attack;
		else
			m_ClientDoing = cdo_attack1;

		INT x, y, tx, ty;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
#endif
		m_Frames.nTotalFrame = 0;
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_runattack;
		break;

	case 2:
	case 3:
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}

	m_Frames.nCurrentFrame = 0;

	return TRUE;
}

void	KNpc::OnRunAttack()
{
	if (m_SpecialSkillStep == 0)
	{
		OnRun();
		KSkill* pSkill = (KSkill*)GetActiveSkill();
		if (!pSkill)
			return;

		if (m_Doing == do_stand || (DWORD)m_nCurrentMeleeTime > pSkill->GetMissleGenerateTime(0))
		{
			m_SpecialSkillStep++;
			m_nCurrentMeleeTime = 0;

			DoRunAttack();

		}
		else
		{
			KSkill* pSkill = (KSkill*)GetActiveSkill();
			if (!pSkill)
				return;

			INT nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			if (nCurPhySkillId > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
				{
					pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
				}
			}
			m_nCurrentMeleeTime++;
		}

		m_ProcessAI = 0;
	}
	else if (m_SpecialSkillStep == 1)
	{
		if (WaitForFrame() && m_Frames.nTotalFrame != 0)
		{
			DoStand();
			m_ProcessAI = 1;
		}
		else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
		{
			KSkill* pSkill = (KSkill*)GetActiveSkill();
			if (!pSkill)
				return;

			INT nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			if (nCurPhySkillId > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
				{
					pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
				}
			}
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		}
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
	}
	else
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
	}
}

BOOL KNpc::DoJumpAttack()
{
	m_ProcessAI = 0;

	switch (m_SpecialSkillStep)
	{
	case 0:
	{
		DoJump();

#ifndef _SERVER
		m_DataRes.SetBlur(TRUE);
		m_ClientDoing = cdo_jump;
#endif
		m_Doing = do_jumpattack;
		break;

	case 1:
#ifndef _SERVER
		if (g_Random(2))
			m_ClientDoing = cdo_attack;
		else
			m_ClientDoing = cdo_attack1;
		INT x, y, tx, ty;
		SubWorld[m_SubWorldIndex].Map2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
#endif
		m_Frames.nTotalFrame = m_AttackFrame * MAX_PERCENT / (MAX_PERCENT + m_CurrentAttackSpeed);
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_jumpattack;
		break;
	}
	case 2:
	case 3:
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}

	m_Frames.nCurrentFrame = 0;

	return TRUE;

}

BOOL KNpc::OnJumpAttack()
{
	if (m_SpecialSkillStep == 0)
	{
		if (!OnJump())
		{
			m_SpecialSkillStep++;
			m_nCurrentMeleeTime = 0;
			DoJumpAttack();
		}
		m_ProcessAI = 0;
	}
	else if (m_SpecialSkillStep == 1)
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		if (WaitForFrame() && m_Frames.nTotalFrame != 0)
		{
			DoStand();
			m_ProcessAI = 1;
		}
		else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
		{
			KSkill* pSkill = (KSkill*)GetActiveSkill();
			if (!pSkill)
				return FALSE;

			INT nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			if (nCurPhySkillId > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
				{
					pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
				}
			}
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		}
	}
	else
	{
#ifndef _SERVER
		m_DataRes.SetBlur(FALSE);
#endif
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL KNpc::CheckHitTarget(INT nAR, INT nDf, INT nIngore/* = 0*/)
{
	INT nDefense = 0;
	if (nIngore < MAX_PERCENT)
		nDefense = nDf * (MAX_PERCENT - nIngore) / MAX_PERCENT;
	INT nPercent = 0;
	if (nAR < 0)
		return FALSE;

	if (nDf < 0)
		nPercent = MAX_HIT_PERCENT;
	else if ((nAR + nDefense) == 0)
		nPercent = 50;
	else
		nPercent = nAR * MAX_PERCENT / (nAR + nDefense);

	if (nPercent > MAX_HIT_PERCENT + 4)
		nPercent = MAX_HIT_PERCENT;

	if (nPercent < 40)
		nPercent = 40;

	BOOL bRet = g_RandPercent(nPercent);
	return bRet;
}

void KNpc::GetNpcCopyFromTemplate(INT nNpcTemplateId, INT nLevel)
{
	if (nNpcTemplateId < 0 || nLevel < 1)
		return;

	KNpcTemplate* pNpcTemp = NULL;
	if (pNpcTemp = NpcSet.GetTemplate(nNpcTemplateId, nLevel))
		//if (g_pNpcTemplate[nNpcTemplateId]) //Êý¾ÝÓÐÐ§Ôò¿½±´£¬·ñÔòÖØÐÂÉú³É
		LoadDataFromTemplate(pNpcTemp);
	/*else
	{
		g_pNpcTemplate[nNpcTemplateId] = new KNpcTemplate;
		g_pNpcTemplate[nNpcTemplateId]->m_NpcSettingIdx = nNpcTemplateId;
		g_pNpcTemplate[nNpcTemplateId]->Init(nNpcTemplateId);
		g_pNpcTemplate[nNpcTemplateId]->m_bHaveLoadedFromTemplate = TRUE;
		LoadDataFromTemplate(nNpcTemplateId);
	}*/
}

void	KNpc::LoadDataFromTemplate(void* nNpcTemp/*INT nNpcTemplateId*/)
{
	//if (nNpcTemplateId < 0)
	//{
	//	g_DebugLog("NpcTemplateId < 0");
	//	return;
	//}

	KNpcTemplate* pNpcTemp = (KNpcTemplate*)nNpcTemp;

	if (!pNpcTemp)
		return;

	strcpy(Name, pNpcTemp->Name);
	m_HeadImage = pNpcTemp->m_HeadImage;
	m_CorpseSettingIdx = pNpcTemp->m_CorpseSettingIdx;
	m_DeathFrame = pNpcTemp->m_DeathFrame;
	m_WalkFrame = pNpcTemp->m_WalkFrame;
	m_RunFrame = pNpcTemp->m_RunFrame;
	m_HurtFrame = pNpcTemp->m_HurtFrame;
	m_WalkSpeed = pNpcTemp->m_WalkSpeed;
	m_StandFrame = pNpcTemp->m_StandFrame;
	m_StandFrame1 = pNpcTemp->m_StandFrame1;
	m_LifeMax = pNpcTemp->m_nLifeMax;

#ifndef _SERVER
	m_ArmorType = pNpcTemp->m_ArmorType;
	m_HelmType = pNpcTemp->m_HelmType;
	m_WeaponType = pNpcTemp->m_WeaponType;

	m_HorseType = pNpcTemp->m_HorseType;
	m_bRideHorse = pNpcTemp->m_bRideHorse;
#endif

	if (m_Kind != kind_player)
	{
		m_AttackFrame = pNpcTemp->m_AttackFrame;
		m_CastFrame = pNpcTemp->m_CastFrame;
		m_RunSpeed = pNpcTemp->m_RunSpeed;

		//strcpy(Name, pNpcTemp->Name);
		m_Kind = pNpcTemp->m_Kind;
		m_Camp = pNpcTemp->m_Camp;
		m_Series = pNpcTemp->m_Series;
		m_bClientOnly = pNpcTemp->m_bClientOnly;
		m_NpcSettingIdx = pNpcTemp->m_NpcSettingIdx;
		m_nStature = pNpcTemp->m_nStature;

#ifdef _SERVER	
		m_SkillList = pNpcTemp->m_SkillList;
		m_AiMode = pNpcTemp->m_AiMode;
		m_AiAddLifeTime = 0;

		if (!m_AiSkillRadiusLoadFlag)
		{
			m_AiSkillRadiusLoadFlag = 1;
			INT i;
			for (i = 0; i < MAX_AI_PARAM - 1; i++)
				m_AiParam[i] = pNpcTemp->m_AiParam[i];

			INT		nMaxRadius = 0, nTempRadius;
			KSkill* pSkill;
			for (i = 1; i < MAX_NPC_USE_SKILL + 1; i++)
			{
				pSkill = (KSkill*)g_SkillManager.GetSkill(m_SkillList.m_Skills[i].SkillId, m_SkillList.m_Skills[i].CurrentSkillLevel);
				if (!pSkill)
					continue;
				nTempRadius = pSkill->GetAttackRadius();
				if (nTempRadius > nMaxRadius)
					nMaxRadius = nTempRadius;
			}
			m_AiParam[MAX_AI_PARAM - 1] = nMaxRadius * nMaxRadius;
		}

		m_FireResistMax = pNpcTemp->m_FireResistMax;
		m_ColdResistMax = pNpcTemp->m_ColdResistMax;
		m_LightResistMax = pNpcTemp->m_LightResistMax;
		m_PoisonResistMax = pNpcTemp->m_PoisonResistMax;
		m_PhysicsResistMax = pNpcTemp->m_PhysicsResistMax;
		m_ActiveRadius = pNpcTemp->m_ActiveRadius;
		m_VisionRadius = pNpcTemp->m_VisionRadius;
		m_AIMAXTime = pNpcTemp->m_AIMAXTime;
		m_HitRecover = pNpcTemp->m_HitRecover;
		m_ReviveFrame = pNpcTemp->m_ReviveFrame;
		m_Experience = pNpcTemp->m_Experience;
		m_CurrentExperience = m_Experience;
		m_LifeReplenish = pNpcTemp->m_LifeReplenish;
		m_AttackRating = pNpcTemp->m_AttackRating;
		m_Defend = pNpcTemp->m_Defend;
		m_PhysicsDamage = pNpcTemp->m_PhysicsDamage;
		m_RedLum = pNpcTemp->m_RedLum;
		m_GreenLum = pNpcTemp->m_GreenLum;
		m_BlueLum = pNpcTemp->m_BlueLum;
		m_FireResist = pNpcTemp->m_FireResist;
		m_ColdResist = pNpcTemp->m_ColdResist;
		m_LightResist = pNpcTemp->m_LightResist;
		m_PoisonResist = pNpcTemp->m_PoisonResist;
		m_PhysicsResist = pNpcTemp->m_PhysicsResist;
#endif
		RestoreNpcBaseInfo();
	}
}

//-----------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÎïÀí¹¥»÷µÄ×î´ó×îÐ¡Öµ not end ÐèÒª¿¼ÂÇµ÷ÓÃµÄµØ·½
//-----------------------------------------------------------------------
void	KNpc::SetPhysicsDamage(INT nMinDamage, INT nMaxDamage)
{
	m_PhysicsDamage.nValue[0] = nMinDamage;
	m_PhysicsDamage.nValue[2] = nMaxDamage;
}

void	KNpc::SetEngergyDamage(INT nEngergyDamage)
{
	m_CurrentAddEngergyDamage += nEngergyDamage;
}

void	KNpc::SetReviveFrame(INT nReviveFrame)
{
	m_ReviveFrame = nReviveFrame;
}


//-----------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨¹¥»÷ÃüÖÐÂÊ
//-----------------------------------------------------------------------
void	KNpc::SetBaseAttackRating(INT nAttackRating)
{
	m_AttackRating = nAttackRating;
	// ´Ë´¦»¹ÐèÒª¼ÓÉÏ×°±¸¡¢¼¼ÄÜµÄÓ°Ïì£¬¼ÆËã³öµ±Ç°Öµ
	m_CurrentAttackRating = m_AttackRating;
}

//-----------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨·ÀÓùÁ¦
//-----------------------------------------------------------------------
void	KNpc::SetBaseDefence(INT nDefence)
{
	m_Defend = nDefence;
	// ´Ë´¦»¹ÐèÒª¼ÓÉÏ×°±¸¡¢¼¼ÄÜµÄÓ°Ïì£¬¼ÆËã³öµ±Ç°Öµ
	m_CurrentDefend = m_Defend;
}

/*
//-----------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÐÐ×ßËÙ¶È
//-----------------------------------------------------------------------
void	KNpc::SetBaseWalkSpeed(INT nSpeed)
{
	m_WalkSpeed = nSpeed;
	// ´Ë´¦»¹ÐèÒª¼ÓÉÏ×°±¸¡¢¼¼ÄÜµÄÓ°Ïì£¬¼ÆËã³öµ±Ç°Öµ (not end)
	m_CurrentWalkSpeed = m_WalkSpeed;
}
*/

/*
//-----------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÅÜ²½ËÙ¶È
//-----------------------------------------------------------------------
void	KNpc::SetBaseRunSpeed(INT nSpeed)
{
	m_RunSpeed = nSpeed;
	// ´Ë´¦»¹ÐèÒª¼ÓÉÏ×°±¸¡¢¼¼ÄÜµÄÓ°Ïì£¬¼ÆËã³öµ±Ç°Öµ (not end)
	m_CurrentRunSpeed = m_RunSpeed;
}
*/

#ifdef _SERVER
void KNpc::DeathPunish(INT nMode, INT nBelongPlayer)
{
#define	LOSE_EXP_SCALE		10

	if (IsPlayer())
	{
		// ±»npc kill
		if (nMode == enumDEATH_MODE_NPC_KILL)
		{
			Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeDefend);
			// ¾­Ñé¼õÉÙ
			//if (Player[m_nPlayerIdx].m_nExp > 0)
			//{
			INT nSubExp;
			if (m_Level <= 10)
				nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level, m_byTranslife) / MAX_PERCENT) * 2;
			else
				nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level, m_byTranslife) / MAX_PERCENT) * 4;

			Player[m_nPlayerIdx].DirectAddExp(-nSubExp);
			//}
			// Ç®¼õÉÙ
			INT nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney() / 2;
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				// ËðÊ§½ðÇ®ÏûÏ¢
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void*)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;

				if (nMoney / 2 > 0)
					PlayerDeadCreateMoneyObj(nMoney / 2);
			}
		}
		// ÇÐ´è£¬Ã»ÓÐ³Í·£
		else if (nMode == enumDEATH_MODE_PLAYER_NO_PUNISH)
		{
			return;
		}
		else if (nMode == enumDEATH_MODE_PLAYER_SPAR_NO_PUNISH)
		{
			return;
		}
		else if (nMode == enumDEATH_MODE_PKBATTLE_PUNISH || nMode == enumDEATH_MODE_TOURNAMENTS_PUNISH)
		{
			return;
		}
		// PKÖÂËÀ£¬°´PKÖµ¼ÆËã³Í·£
		else //if (nMode == enumDEATH_MODE_PLAYER_PUNISH)
		{
			Player[m_nPlayerIdx].m_ItemList.Abrade(enumAbradeDefend);

			INT	nPKValue;
			nPKValue = Player[this->m_nPlayerIdx].m_cPK.GetPKValue();
			if (nPKValue < 0)
				nPKValue = 0;
			if (nPKValue > MAX_DEATH_PUNISH_PK_VALUE)
				nPKValue = MAX_DEATH_PUNISH_PK_VALUE;

			// ¾­Ñé¼õÉÙ
			if (m_Level < NpcSet.m_nLevelBoundaryPKPunish)
			{
				DWORD		dwLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(m_Level, m_byTranslife);
				Player[m_nPlayerIdx].DirectAddExp(-(dwLevelExp / MAX_PERCENT * PlayerSet.m_sPKPunishParam[nPKValue].m_nExpP));
			}
			else
				Player[m_nPlayerIdx].DirectAddExp(-PlayerSet.m_sPKPunishParam[nPKValue].m_nExpV);

			// Ç®¼õÉÙ
			INT nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney() * PlayerSet.m_sPKPunishParam[nPKValue].m_nMoney / MAX_PERCENT;
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				// ËðÊ§½ðÇ®ÏûÏ¢
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void*)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;

				if (nMoney / 2 > 0)
					PlayerDeadCreateMoneyObj(nMoney / 2);
			}

			// ¶ªÊ§ÎïÆ·
			Player[m_nPlayerIdx].m_ItemList.AutoLoseItemFromEquipmentRoom(PlayerSet.m_sPKPunishParam[nPKValue].m_nItem);

			// ¶ªÊ§´©ÔÚÉíÉÏµÄ×°±¸
			if (g_Random(MAX_PERCENT) < PlayerSet.m_sPKPunishParam[nPKValue].m_nEquip)
			{
				Player[m_nPlayerIdx].m_ItemList.AutoLoseEquip();
			}
			Player[m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nBeKilledAddPKValue);
			if (m_nLastDamageIdx)
			{
				if (Npc[m_nLastDamageIdx].IsPlayer())
				{
					KPlayerChat::MakeEnemy(Name, Npc[m_nLastDamageIdx].Name);
				}
			}
		}
	}
}

// Íæ¼ÒËÀµÄÊ±ºòµô³öÀ´µÄÇ®Éú³ÉÒ»¸öobject
void	KNpc::PlayerDeadCreateMoneyObj(INT nMoneyNum)
{
	INT		nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);

	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y,
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY,
		&Pos.nOffX, &Pos.nOffY);

	INT nObjIdx = ObjSet.AddMoneyObj(Pos, nMoneyNum);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		Object[nObjIdx].SetItemBelong(-1);
	}
}

void KNpc::Revive()
{
	RestoreNpcBaseInfo();
	INT nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[m_SubWorldIndex].Mps2Map(m_OriginX, m_OriginY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;
	if (m_RegionIndex < 0)
		return;
	SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].AddNpcRef(m_MapX, m_MapY, obj_npc);
#ifdef _SERVER
	//SubWorld[m_SubWorldIndex].m_WorldMessage.Send(GWM_NPC_CHANGE_REGION, VOID_REGION, nRegion, m_Index);
	SubWorld[m_SubWorldIndex].NpcChangeRegion(VOID_REGION, nRegion, m_Index);	// spe 03/06/28

	if (m_ActionScriptID)
		NpcSet.ExecuteScript(m_Index, m_ActionScriptID, "Revive", m_Index);
#else
	SubWorld[0].NpcChangeRegion(VOID_REGION, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);
#endif
	DoStand();
	m_ProcessAI = 1;
	m_ProcessState = 1;
	m_AiAddLifeTime = 0;
}

void KNpc::RestoreLiveData()
{

}
#endif



#ifdef	_SERVER
// ÏòÖÜÎ§¾ÅÆÁ¹ã²¥
void	KNpc::SendDataToNearRegion(void* pBuffer, DWORD dwSize)
{
	_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

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
	SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].BroadCast(pBuffer, dwSize, nMaxCount, m_MapX, m_MapY);
	for (INT i = 0; i < 8; i++)
	{
		if (SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i] < 0)
			continue;
		SubWorld[m_SubWorldIndex].m_Region[SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i]].BroadCast(pBuffer, dwSize, nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
}
#endif



#ifdef	_SERVER
//-----------------------------------------------------------------------------
//	¹¦ÄÜ£ºËÀÍöÊ±ºò¼ÆËãPKÖµ
//-----------------------------------------------------------------------------
INT		KNpc::DeathCalcPKValue(INT nKiller)
{
	// ³ö´í
	if (nKiller <= 0 || nKiller >= MAX_NPC)
		return enumDEATH_MODE_NPC_KILL;

	if (m_nCurPKPunishState == enumDEATH_MODE_PKBATTLE_PUNISH ||
		m_nCurPKPunishState == enumDEATH_MODE_TOURNAMENTS_PUNISH)
		return m_nCurPKPunishState;

	// Íæ¼ÒÖ®¼ä£¬³ÇÕòÄÚ
	if (this->m_Kind != kind_player || Npc[nKiller].m_Kind != kind_player || !m_FightMode)
		return enumDEATH_MODE_NPC_KILL;
	// Èç¹ûÊÇÇÐ´è£¬²»¼ÆËã
	if (Player[m_nPlayerIdx].m_cPK.GetExercisePKAim() == Npc[nKiller].m_nPlayerIdx)
	{
		if (Player[m_nPlayerIdx].m_cPK.IsEnmitySpar())
		{
			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_SPAR_DEFEAT;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
			g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			sMsg.m_wMsgID = enumMSG_ID_SPAR_VICTORY;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
			g_pServer->PackDataToClient(Player[Npc[nKiller].m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			return enumDEATH_MODE_PLAYER_SPAR_NO_PUNISH;
		}

		return enumDEATH_MODE_PLAYER_NO_PUNISH;
	}
	// Èç¹ûÊÇ³ðÉ±
	if (Player[m_nPlayerIdx].m_cPK.GetEnmityPKState() == enumPK_ENMITY_STATE_PKING &&
		Player[m_nPlayerIdx].m_cPK.GetEnmityPKAim() == Npc[nKiller].m_nPlayerIdx)
	{
		if (Player[Npc[nKiller].m_nPlayerIdx].m_cPK.IsEnmitySpar())
		{
			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_SPAR_DEFEAT;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
			g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			sMsg.m_wMsgID = enumMSG_ID_SPAR_VICTORY;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
			g_pServer->PackDataToClient(Player[Npc[nKiller].m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			return enumDEATH_MODE_PLAYER_SPAR_NO_PUNISH;
		}
		else
		{
			if (Player[Npc[nKiller].m_nPlayerIdx].m_cPK.IsEnmityPKLauncher())
				Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nEnmityAddPKValue);

			Player[Npc[nKiller].m_nPlayerIdx].m_nKillPeopleNumber++;

			return enumDEATH_MODE_PLAYER_PUNISH;
		}
	}
	if (Player[m_nPlayerIdx].m_cPK.GetNormalPKState() != enumPKMurder && Player[Npc[nKiller].m_nPlayerIdx].m_cPK.GetNormalPKState() == enumPKMurder)
	{
		Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nMurderAddPKValue);

		Player[Npc[nKiller].m_nPlayerIdx].m_nKillPeopleNumber++;

		return enumDEATH_MODE_PLAYER_PUNISH;
	}
	if (m_Level <= 50 && Npc[nKiller].m_Level * 2 >= m_Level * 3)
	{
		if (!Player[m_nPlayerIdx].m_cPK.GetNormalPKState())
		{
			if (Npc[nKiller].m_CurrentCamp == camp_free)
				Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nKillerPKFactionAddPKValue);
			else
				Player[Npc[nKiller].m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nFactionPKFactionAddPKValue);
		}
		Player[Npc[nKiller].m_nPlayerIdx].m_nKillPeopleNumber++;

		return enumDEATH_MODE_PLAYER_PUNISH;
	}

	return enumDEATH_MODE_PLAYER_PUNISH;
}
#endif

#ifdef	_SERVER
//-----------------------------------------------------------------------------
//	¹¦ÄÜ£º²éÕÒÖÜÎ§9¸öRegionÖÐÊÇ·ñÓÐÖ¸¶¨µÄ player
//-----------------------------------------------------------------------------
INT	KNpc::FindAroundPlayer(const char* Name)
{
	INT nNpc = 0;
	if (Name[0] <= 0 || m_RegionIndex < 0)
		return nNpc;
	nNpc = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].SearchNpcName(Name);
	if (nNpc)
		return nNpc;
	INT		nRegionNo;
	for (INT i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_nConnectRegion[i];
		if (nRegionNo < 0)
			continue;
		nNpc = SubWorld[m_SubWorldIndex].m_Region[nRegionNo].SearchNpcName(Name);
		if (nNpc)
			return nNpc;
	}
	return nNpc;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨Í·¶¥×´Ì¬
//-------------------------------------------------------------------------
void	KNpc::SetMenuState(INT nState, char* lpszSentence, INT nLength)
{
	this->m_DataRes.SetMenuState(nState, lpszSentence, nLength);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñµÃÍ·¶¥×´Ì¬
//-------------------------------------------------------------------------
INT		KNpc::GetMenuState()
{
	return this->m_DataRes.GetMenuState();
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º²éÕÒÖÜÎ§9¸öRegionÖÐÊÇ·ñÓÐÖ¸¶¨ ID µÄ npc
//-------------------------------------------------------------------------
DWORD	KNpc::SearchAroundID(DWORD dwID)
{
	INT		nIdx, nRegionNo;
	nIdx = SubWorld[0].m_Region[m_RegionIndex].SearchNpc(dwID);
	if (nIdx)
		return nIdx;
	for (INT i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[0].m_Region[m_RegionIndex].m_nConnectRegion[i];
		if (nRegionNo < 0)
			continue;
		nIdx = SubWorld[0].m_Region[nRegionNo].SearchNpc(dwID);
		if (nIdx)
			return nIdx;
	}
	return 0;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÌØÊâµÄÖ»²¥·ÅÒ»±éµÄËæÉísprÎÄ¼þ
//-------------------------------------------------------------------------
void	KNpc::SetSpecialSpr(char* lpszSprName)
{
	m_DataRes.SetSpecialSpr(lpszSprName);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨Ë²¼äÌØÐ§
//-------------------------------------------------------------------------
void	KNpc::SetInstantSpr(INT nNo)
{
	char	szName[FILE_NAME_LENGTH];
	szName[0] = 0;
	NpcSet.m_cInstantSpecial.GetSprName(nNo, szName, sizeof(szName));
	if (szName[0])
		this->SetSpecialSpr(szName);
}
#endif

#ifndef _SERVER
INT		KNpc::GetNormalNpcStandDir(INT nFrame)
{
	return m_DataRes.GetNormalNpcStandDir(nFrame);
}

void KNpc::GetNpcResFile(INT nNpcSettingIdx, char* pszResPath)
{
	if (nNpcSettingIdx < 0)
		return;

	char szNpcTypeName[32];
	g_NpcSetting.GetString(nNpcSettingIdx + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
	m_DataRes.m_pcResTemp = g_NpcResList.AddNpcRes(szNpcTypeName);
	if (m_DataRes.m_pcResTemp == NULL)
	{
		strcpy(pszResPath, UNKNOWNITEM_SPR);
		return;
	}
	m_DataRes.GetResFile(::GetRandomNumber(1, 12), pszResPath);
	if (!pszResPath[0])
		m_DataRes.GetResFile(0, pszResPath);
}

void KNpc::KeyToImage(char* szKey, INT nAction, KUiImage* pImage)
{
	if (szKey[0] == 0)
	{
		memset(pImage->Name, 0, sizeof(pImage->Name));
		pImage->Name[0] = 0x20;
	}
	else
	{
		KImageParam sImage;
		if (g_pRepresent->GetImageParam(szKey, &sImage, ISI_T_SPR) == true)
		{
			strcpy(pImage->Name, szKey);
			pImage->Frame = sImage.nNumFrames;
		}
		else
		{
			if (strcmp(szKey, NPCNAME_KEY) != 0)
			{
				m_DataRes.m_pcResTemp = g_NpcResList.AddNpcRes(szKey);
				if (m_DataRes.m_pcResTemp == NULL)
				{
					strcpy(pImage->Name, UNKNOWNITEM_SPR);
					pImage->Frame = 0;
				}
				else
				{
					for (INT i = 0; i < MAX_PART; i++)
					{
						m_DataRes.m_pcResTemp->GetFileName(i, nAction, 0, "", pImage->Name, sizeof(pImage->Name));
						if (pImage->Name[0])
						{
							pImage->Frame = (m_DataRes.m_pcResTemp->GetTotalFrames(i, nAction, 0, MAX_PART)) /
								(m_DataRes.m_pcResTemp->GetTotalDirs(i, nAction, 0, MAX_PART)); return;
						}
					}
					for (INT j = 0; j < MAX_PART; j++)
					{
						m_DataRes.m_pcResTemp->GetFileName(j, 0, 0, "", pImage->Name, sizeof(pImage->Name));
						if (pImage->Name[0])
						{
							pImage->Frame = (m_DataRes.m_pcResTemp->GetTotalFrames(j, 0, 0, MAX_PART)) /
								(m_DataRes.m_pcResTemp->GetTotalDirs(j, 0, 0, MAX_PART)); return;
						}
					}
				}
			}
			else
			{
				if (Player[m_nPlayerIdx].m_nLastNpcIndex)
				{
					for (INT i = 0; i < MAX_PART; i++)
					{
						Npc[Player[m_nPlayerIdx].m_nLastNpcIndex].GetNpcRes()->m_pcResNode->GetFileName(i, nAction, 0, "", pImage->Name, sizeof(pImage->Name));
						if (pImage->Name[0])
						{
							pImage->Frame = (Npc[Player[m_nPlayerIdx].m_nLastNpcIndex].GetNpcRes()->m_pcResNode->GetTotalFrames(i, nAction, 0, MAX_PART)) /
								(Npc[Player[m_nPlayerIdx].m_nLastNpcIndex].GetNpcRes()->m_pcResNode->GetTotalDirs(i, nAction, 0, MAX_PART)); return;
						}
					}
					for (INT j = 0; j < MAX_PART; j++)
					{
						Npc[Player[m_nPlayerIdx].m_nLastNpcIndex].GetNpcRes()->m_pcResNode->GetFileName(j, 0, 0, "", pImage->Name, sizeof(pImage->Name));
						if (pImage->Name[0])
						{
							pImage->Frame = (Npc[Player[m_nPlayerIdx].m_nLastNpcIndex].GetNpcRes()->m_pcResNode->GetTotalFrames(j, 0, 0, MAX_PART)) /
								(Npc[Player[m_nPlayerIdx].m_nLastNpcIndex].GetNpcRes()->m_pcResNode->GetTotalDirs(j, 0, 0, MAX_PART)); return;
						}
					}
				}
			}
		}
	}
}
#endif

#ifdef _SERVER
//ÖØÐÂ¸üÐÂ½ÇÉ«×´Ì¬ÐÅÏ¢Êý¾Ý
void	KNpc::UpdateNpcStateInfo()
{
	INT i = 0, j = 0;
	memset(m_btStateInfo, 0, sizeof(BYTE) * MAX_SKILL_STATE);
	KStateNode* pNode = (KStateNode*)m_StateSkillList.GetTail();

	if (m_ActiveAuraID)
	{
		if (m_SkillList.GetLevel(m_ActiveAuraID) > 0)
		{
			KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_ActiveAuraID, 1);
			if (pOrdinSkill)
			{
				if (pOrdinSkill->GetStateSpecailId())
					m_btStateInfo[i++] = pOrdinSkill->GetStateSpecailId();

				if (pOrdinSkill->GetAppendSkillNum())
				{
					for (j = 0; j < pOrdinSkill->GetAppendSkillNum(); j++)
					{
						if (m_SkillList.GetLevel(pOrdinSkill->GetAppendSkillId(j)) <= 0)
							continue;
						pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(pOrdinSkill->GetAppendSkillId(j), 1);
						if (pOrdinSkill)
						{
							if (pOrdinSkill->GetStateSpecailId())
								m_btStateInfo[i++] = pOrdinSkill->GetStateSpecailId();
						}
					}
				}
			}
		}
	}

	while (pNode && i < MAX_SKILL_STATE)
	{
		if (pNode->m_StateGraphics > 0)
			m_btStateInfo[i++] = pNode->m_StateGraphics;
		pNode = (KStateNode*)pNode->GetPrev();
	}

	for (i; i < MAX_SKILL_STATE; i++)
		m_btStateInfo[i] = 0;
}
#endif

#ifndef _SERVER
void	KNpc::SetNpcState(BYTE* pNpcState)
{
	if (!pNpcState)
		return;

	memcpy(m_btStateInfo, pNpcState, sizeof(BYTE) * MAX_SKILL_STATE);
}
#endif

void	KNpc::ClearNpcState()
{
	KStateNode* pNode = (KStateNode*)m_StateSkillList.GetHead();
	KStateNode* pTempNode = NULL;

	while (pNode)
	{
		pTempNode = pNode;
		pNode = (KStateNode*)pNode->GetNext();
		pTempNode->Remove();
		delete pTempNode;
	}
	return;
}


void	KNpc::RestoreNpcBaseInfo()
{
	m_CurrentCamp = m_Camp;
	m_ActiveSkillID = 0;
	m_ActiveAuraID = 0;

	m_nPeopleIdx = 0;
	m_nLastDamageIdx = 0;
	m_nLastPoisonDamageIdx = 0;
	m_nObjectIdx = 0;

	m_CurrentLife = m_LifeMax;
	m_CurrentLifeMax = m_LifeMax;
	m_CurrentLifeReplenish = m_LifeReplenish;
	m_CurrentLifeReplenishPercent = 0;
	m_CurrentMana = m_ManaMax;
	m_CurrentManaMax = m_ManaMax;
	m_CurrentManaReplenish = m_ManaReplenish;
	m_CurrentStamina = m_StaminaMax;
	m_CurrentStaminaMax = m_StaminaMax;
	m_CurrentStaminaGain = m_StaminaGain;
	m_CurrentStaminaLoss = m_StaminaLoss;

	memset(&m_CurrentFireDamage, 0, sizeof(m_CurrentFireDamage));
	memset(&m_CurrentColdDamage, 0, sizeof(m_CurrentColdDamage));
	memset(&m_CurrentLightDamage, 0, sizeof(m_CurrentLightDamage));
	memset(&m_CurrentPoisonDamage, 0, sizeof(m_CurrentPoisonDamage));

	memset(&m_CurrentFireMagic, 0, sizeof(m_CurrentFireMagic));
	memset(&m_CurrentColdMagic, 0, sizeof(m_CurrentColdMagic));
	memset(&m_CurrentLightMagic, 0, sizeof(m_CurrentLightMagic));
	memset(&m_CurrentPoisonMagic, 0, sizeof(m_CurrentPoisonMagic));

	m_CurrentAttackRating = m_AttackRating;
	m_CurrentDefend = m_Defend;

	m_CurrentFireResist = m_FireResist;
	m_CurrentColdResist = m_ColdResist;
	m_CurrentPoisonResist = m_PoisonResist;
	m_CurrentLightResist = m_LightResist;
	m_CurrentPhysicsResist = m_PhysicsResist;
	m_CurrentFireResistMax = m_FireResistMax;
	m_CurrentColdResistMax = m_ColdResistMax;
	m_CurrentPoisonResistMax = m_PoisonResistMax;
	m_CurrentLightResistMax = m_LightResistMax;
	m_CurrentPhysicsResistMax = m_PhysicsResistMax;

	m_CurrentWalkSpeed = m_WalkSpeed;
	m_CurrentRunSpeed = m_RunSpeed;
	m_CurrentAttackSpeed = m_AttackSpeed;
	m_CurrentCastSpeed = m_CastSpeed;
	m_CurrentVisionRadius = m_VisionRadius;
	m_CurrentActiveRadius = m_ActiveRadius;
	m_CurrentHitRecover = m_HitRecover;

	m_CurrentDamage2Mana = 0;
	m_CurrentLifeStolen = 0;
	m_CurrentManaStolen = 0;
	m_CurrentStaminaStolen = 0;
	m_CurrentPiercePercent = 0;
	m_CurrentFreezeTimeReducePercent = 0;
	m_CurrentPoisonTimeReducePercent = 0;
	m_CurrentStunTimeReducePercent = 0;
	m_CurrentFireEnhance = 0;
	m_CurrentColdEnhance = 0;
	m_CurrentPoisonEnhance = 0;
	m_CurrentLightEnhance = 0;
	m_CurrentRangeEnhance = 0;
	m_CurrentHandEnhance = 0;
	m_CurrentDeadlyStrikeEnhanceP = 0;
	m_CurrentFatallyStrikeEnhanceP = 0;
	m_CurrentFatallyStrikeResP = 0;
	m_CurrentManaShield = 0;
	m_CurrentStaticMagicShieldP = 0;
	m_CurrentLucky = 0;
	m_CurrentExpEnhance = 0;
	m_CurrentPoisonDamageReturnPercent = 0;
	m_CurrentReturnSkillPercent = 0;
	m_CurrentIgnoreSkillPercent = 0;
	ZeroMemory(m_CurrentMeleeEnhance, sizeof(m_CurrentMeleeEnhance));
	memset(&m_ReplySkill, 0, sizeof(m_ReplySkill));
	memset(&m_RescueSkill, 0, sizeof(m_RescueSkill));
	memset(&m_AttackSkill, 0, sizeof(m_AttackSkill));
	memset(&m_CastSkill, 0, sizeof(m_CastSkill));
	memset(&m_DeathSkill, 0, sizeof(m_DeathSkill));
	m_CurrentIgnoreNegativeStateP = 0;

	m_CurrentSkillEnhancePercent = 0;
	m_CurrentFiveElementsEnhance = 0;
	m_CurrentFiveElementsResist = 0;
	m_CurrentManaToSkillEnhanceP = 0;

	ClearStateSkillEffect();
	ClearNormalState();
}

#ifndef _SERVER
void KNpc::DrawBorder()
{
	if (m_Index <= 0)
		return;

	m_DataRes.DrawBorder();
}

INT KNpc::DrawMenuState(INT n)
{
	if (m_Index <= 0)
		return n;

	return m_DataRes.DrawMenuState(n);
}

void KNpc::DrawBlood()
{
	if (m_Kind != kind_normal)
		return;

	INT nFontSize = 12;


	INT nHeightOff = GetNpcPate();
	{
		nHeightOff = PaintLife(nHeightOff, true);
		nHeightOff += SHOW_SPACE_HEIGHT;
	}
	{
		nHeightOff = PaintInfo(nHeightOff, true);
	}
}
#endif

#ifdef _SERVER
BOOL KNpc::SetPos(INT nX, INT nY, BOOL bSync2Client /* = TRUE*/)
#else
BOOL KNpc::SetPos(INT nX, INT nY)
#endif
{
	BOOL bResult = FALSE;
	INT nMpsX = 0;
	INT nMpsY = 0;

	if (!(m_SubWorldIndex >= 0))
		goto Exit0;

	GetMpsPos(&nMpsX, &nMpsY);
	if (nMpsX == nX && nMpsY == nY)
		goto Exit1;

	{
		KSubWorld& rcWorld = SubWorld[m_SubWorldIndex];
		INT x = nX / (rcWorld.m_nRegionWidth * rcWorld.m_nCellWidth);
		INT y = nY / (rcWorld.m_nRegionHeight * rcWorld.m_nCellHeight);
		INT nRegionId = MAKELONG(x, y);
		INT nRegion = -1;
#ifdef _SERVER
		nRegion = rcWorld.GetRegionIndex(MAKELONG(x, y));
#else
		nRegion = rcWorld.FindRegion(nRegionId);
#endif

#ifdef _SERVER
		if (!(nRegion >= 0))
			goto Exit0;
#else
		if (!(nRegion >= 0 || IsPlayer()))
			goto Exit0;
#endif

		if (m_RegionIndex >= 0)
		{
			SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY);
#ifdef _SERVER
			if (bSync2Client)
			{
				NPC_REMOVE_SYNC RemoveSync;
				RemoveSync.ProtocolType = s2c_npcremove;
				RemoveSync.ID = m_dwID;
				SendDataToNearRegion(&RemoveSync, sizeof(NPC_REMOVE_SYNC));
			}
#endif
		}

		if (m_RegionIndex != nRegion || nRegion < 0)
		{
			INT nOldRegion = m_RegionIndex;
#ifdef _SERVER
			SubWorld[m_SubWorldIndex].NpcChangeRegion(nOldRegion, nRegion, m_Index);
			if (IsPlayer())
				SubWorld[m_SubWorldIndex].PlayerChangeRegion(nOldRegion, nRegion, m_nPlayerIdx);
			if (!(m_RegionIndex >= 0))
				goto Exit0;
#else
			if (m_RegionIndex >= 0)
			{
				SubWorld[m_SubWorldIndex].NpcChangeRegion(
					SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].m_RegionID, nRegionId, m_Index);
			}
			else
			{
				SubWorld[m_SubWorldIndex].NpcChangeRegion(-1, nRegionId, m_Index);
			}
			if (!(m_RegionIndex >= 0))
				goto Exit0;
#endif
		}

		INT nMapX, nMapY, nOffX, nOffY;
		SubWorld[m_SubWorldIndex].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

		_ASSERT(nRegion == m_RegionIndex);

		m_MapX = nMapX;
		m_MapY = nMapY;
		m_MapZ = 0;
		m_OffX = nOffX;
		m_OffY = nOffY;
		m_Height = 0;

		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].AddNpcRef(m_MapX, m_MapY);
	}

Exit1:
	bResult = TRUE;

Exit0:
	return bResult;
}

#ifdef _SERVER
INT KNpc::ChangeWorld(DWORD dwSubWorldID, INT nX, INT nY)
{
	INT nTargetSubWorld = g_SubWorldSet.SearchWorld(dwSubWorldID);

	if (IsPlayer())
	{
		if (-1 == nTargetSubWorld)
		{
			TobeExchangeServer(dwSubWorldID, nX, nY);
			g_DebugLog("MapID %d haven't been loaded!", dwSubWorldID);
			return 0;
		}
	}

	if (nTargetSubWorld < 0)
		return 0;

	if (IsPlayer())
		Player[m_nPlayerIdx].m_nPrePayMoney = 0;// ²»ÊÇ¿ç·þÎñÆ÷£¬²»ÓÃ»¹Ç®
	// ÇÐ»»µÄÊÀ½ç¾ÍÊÇ±¾Éí
	if (nTargetSubWorld == m_SubWorldIndex)
	{
		if (SetPos(nX, nY))
		{
			SyncPos(-1, TRUE);
			m_ProcessState = 1;
			return 1;
		}
		return 0;
	}

	INT nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[nTargetSubWorld].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	// ÇÐ»»µ½µÄ×ø±ê·Ç·¨
	if (nRegion < 0)
	{
		g_DebugLog("[Map]Change Pos(%d,%d) Invalid!", nX, nY);
		return 0;
	}

	if (m_SubWorldIndex >= 0 && m_SubWorldIndex < MAX_SUBWORLD && m_RegionIndex >= 0)
	{
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].RemoveNpc(m_Index);
		SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);

		NPC_REMOVE_SYNC	RemoveSync;
		RemoveSync.ProtocolType = s2c_npcremove;
		RemoveSync.ID = m_dwID;
		RemoveSync.Rv = TRUE;
		SendDataToNearRegion(&RemoveSync, sizeof(NPC_REMOVE_SYNC));
	}

	INT nSourceSubWorld = m_SubWorldIndex;
	INT nSourceRegion = m_RegionIndex;

	m_SubWorldIndex = nTargetSubWorld;
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;
	SubWorld[nTargetSubWorld].Map2Mps(nRegion, nMapX, nMapY, nOffX, nOffY, &m_OriginX, &m_OriginY);
	SubWorld[nTargetSubWorld].m_Region[nRegion].AddNpc(m_Index);
	SubWorld[nTargetSubWorld].m_Region[nRegion].AddNpcRef(m_MapX, m_MapY, obj_npc);
	DoStand();
	m_ProcessAI = 1;

	if (IsPlayer())
	{
		if (nTargetSubWorld >= 0 && nTargetSubWorld < MAX_SUBWORLD)
			SubWorld[nTargetSubWorld].SendSyncData(m_Index, Player[m_nPlayerIdx].m_nNetConnectIdx);

		if (nSourceSubWorld >= 0 && nSourceSubWorld < MAX_SUBWORLD)
		{
			SubWorld[nSourceSubWorld].RemovePlayer(nSourceRegion, m_nPlayerIdx);

			if (strcmp(SubWorld[nSourceSubWorld].m_szNewWorldScript, "") != 0)
				Player[m_nPlayerIdx].ExecuteScript(SubWorld[nSourceSubWorld].m_szNewWorldScript, "OnLeaveWorld", SubWorld[nTargetSubWorld].m_szNewWorldParam);
		}

		if (nTargetSubWorld >= 0 && nTargetSubWorld < MAX_SUBWORLD)
		{
			SubWorld[nTargetSubWorld].AddPlayer(nRegion, m_nPlayerIdx);

			if (strcmp(SubWorld[nTargetSubWorld].m_szNewWorldScript, "") != 0)
				Player[m_nPlayerIdx].ExecuteScript(SubWorld[nTargetSubWorld].m_szNewWorldScript, "OnNewWorld", SubWorld[nTargetSubWorld].m_szNewWorldParam);
		}

		TRADE_DECISION_COMMAND	sTrade;
		sTrade.ProtocolType = c2s_tradedecision;
		sTrade.m_btDecision = 0;
		sTrade.m_btFolkGame = 0;
		Player[m_nPlayerIdx].TradeDecision((BYTE*)&sTrade);
	}
	return 1;
}
#endif

#ifdef _SERVER
void KNpc::TobeExchangeServer(DWORD dwMapID, INT nX, INT nY)
{
	if (!IsPlayer())
	{
		return;
	}

	m_OldFightMode = m_FightMode;
	m_bExchangeServer = TRUE;
	if (m_nPlayerIdx > 0 && m_nPlayerIdx <= MAX_PLAYER)
	{
		Player[m_nPlayerIdx].TobeExchangeServer(dwMapID, nX, nY);
	}
}
#endif

BOOL KNpc::IsPlayer()
{
#ifdef _SERVER
	return m_Kind == kind_player;
#else
	return m_Index == Player[CLIENT_PLAYER_INDEX].m_nIndex;
#endif
}

// Çå³ýNPCÉíÉÏµÄ·Ç±»¶¯ÀàµÄ¼¼ÄÜ×´Ì¬
void KNpc::ClearStateSkillEffect(bool bSkillCast)
{
	KStateNode* pNode;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		KStateNode* pTempNode = pNode;
		pNode = (KStateNode*)pNode->GetNext();

		if (pTempNode == NULL)
			continue;

		if (pTempNode->m_bOverLook)	// ±»¶¯¼¼ÄÜ
			continue;

		if (pTempNode->m_LeftTime == -1)	// ±»¶¯¼¼ÄÜ
			continue;

		if (pTempNode->m_LeftTime > 0)
		{
			if (!bSkillCast)
			{
				for (INT i = 0; i < MAX_SKILL_STATE; i++)
				{
					if (pTempNode->m_State[i].nAttribType)
						ModifyAttrib(m_Index, &pTempNode->m_State[i]);
				}
				_ASSERT(pTempNode != NULL);
				pTempNode->Remove();
				delete pTempNode;
				pTempNode = NULL;
				continue;
			}
			else
			{
				INT nDelCount = 0;
				KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(pTempNode->m_SkillID, pTempNode->m_Level);
				if (pSkill->IsTargetEnemy())
				{
					for (INT i = 0; i < MAX_SKILL_STATE; i++)
					{
						if (pTempNode->m_State[i].nAttribType && pTempNode->m_State[i].nValue[1] < 0)
						{
							ModifyAttrib(m_Index, &pTempNode->m_State[i]);
							nDelCount++;
						}
					}

					if (nDelCount > 0)
					{
						_ASSERT(pTempNode != NULL);
						pTempNode->Remove();
						delete pTempNode;
						pTempNode = NULL;
					}
				}
				continue;
			}
		}
	}
#ifdef _SERVER
	UpdateNpcStateInfo();
#endif
}

void KNpc::IgnoreState(BOOL bNegative)
{
	KStateNode* pNode;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		KStateNode* pTempNode = pNode;
		pNode = (KStateNode*)pNode->GetNext();

		if (pTempNode->m_bOverLook)	// ±»¶¯¼¼ÄÜ
			continue;

		if (pTempNode->m_LeftTime >= 0)
		{
			if (bNegative)
			{
				KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(pTempNode->m_SkillID, pTempNode->m_Level);
				if (!pSkill->IsTargetOnly() && !pSkill->IsTargetEnemy())
					continue;
			}

			INT i;
			for (i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pTempNode->m_State[i].nAttribType)
				{
					ModifyAttrib(m_Index, &pTempNode->m_State[i]);
				}
			}
			_ASSERT(pTempNode != NULL);
			pTempNode->Remove();
			delete pTempNode;

			pTempNode = NULL;

#ifdef _SERVER
			UpdateNpcStateInfo();
#endif
			continue;
		}
	}
#ifdef _SERVER
	IGNORE_STATE_SYNC	Sync;
	Sync.ProtocolType = s2c_ignorestate;
	Sync.bNegative = bNegative;
	g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &Sync, sizeof(IGNORE_STATE_SYNC));
#endif
}

void KNpc::ClearNormalState()
{
	ZeroMemory(&m_PhysicsArmor, sizeof(m_PhysicsArmor));
	ZeroMemory(&m_ColdArmor, sizeof(m_ColdArmor));
	ZeroMemory(&m_FireArmor, sizeof(m_FireArmor));
	ZeroMemory(&m_PoisonArmor, sizeof(m_PoisonArmor));
	ZeroMemory(&m_LightArmor, sizeof(m_LightArmor));
	ZeroMemory(&m_ManaShield, sizeof(m_ManaShield));
	ZeroMemory(&m_PoisonState, sizeof(m_PoisonState));
	ZeroMemory(&m_FreezeState, sizeof(m_FreezeState));
	ZeroMemory(&m_BurnState, sizeof(m_BurnState));
	ZeroMemory(&m_FrozenAction, sizeof(m_FrozenAction));
	ZeroMemory(&m_RandMove, sizeof(m_RandMove));
	ZeroMemory(&m_StunState, sizeof(m_StunState));
	ZeroMemory(&m_LifeState, sizeof(m_LifeState));
	ZeroMemory(&m_ManaState, sizeof(m_ManaState));
	ZeroMemory(&m_LoseMana, sizeof(m_LoseMana));
	ZeroMemory(&m_HideState, sizeof(m_HideState));
	ZeroMemory(&m_SilentState, sizeof(m_SilentState));
	ZeroMemory(&m_WalkRun, sizeof(m_WalkRun));
}

BOOL KNpc::IsNpcStateExist(INT nId)
{
	if (nId <= 0)
		return FALSE;
	KStateNode* pNode;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		if (pNode->m_SkillID == nId)
			return TRUE;

		pNode = (KStateNode*)pNode->GetNext();
	}
	return FALSE;
}

BOOL KNpc::IsNpcSkillExist(INT nId)
{
	if (nId <= 0)
		return FALSE;
	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		if (m_SkillList.m_Skills[i].SkillId)
		{
			if (m_SkillList.m_Skills[i].SkillId == nId)
				return TRUE;
		}
	}
	return FALSE;
}

BOOL KNpc::CheckTrap(INT nMapX, INT nMapY)
{
	if (m_Kind != kind_player)
		return FALSE;

	if (m_Index <= 0)
		return FALSE;

	if (m_SubWorldIndex < 0 || m_RegionIndex < 0)
		return FALSE;

	DWORD dwTrap = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].GetTrap(nMapX, nMapY);

	if (m_TrapScriptID == dwTrap)
		return FALSE;
	else
		m_TrapScriptID = dwTrap;

	if (!m_TrapScriptID)
		return FALSE;
	Player[m_nPlayerIdx].ExecuteScript(m_TrapScriptID, NORMAL_FUNCTION_NAME, 0);
	return TRUE;
}

void KNpc::SetFightMode(BOOL bFightMode)
{
#ifdef _SERVER
	if (this->m_Kind == kind_player)
		Player[this->m_nPlayerIdx].m_cPK.CloseAll();
#endif

	m_FightMode = bFightMode;
}

void KNpc::TurnTo(INT nIdx)
{
	if (!Npc[nIdx].m_Index || !m_Index)
		return;

	INT nX1, nY1, nX2, nY2;

	GetMpsPos(&nX1, &nY1);
	Npc[nIdx].GetMpsPos(&nX2, &nY2);

	m_Dir = g_GetDirIndex(nX1, nY1, nX2, nY2);
}

void KNpc::ReCalcStateEffect()
{
	KStateNode* pNode;
	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		if (pNode->m_LeftTime != 0)	// °üÀ¨±»¶¯(-1)ºÍÖ÷¶¯(>0)
		{
			INT i;
			for (i = 0; i < MAX_SKILL_STATE; i++)
			{
				if (pNode->m_State[i].nAttribType)
				{
					KMagicAttrib	MagicAttrib;
					MagicAttrib.nAttribType = pNode->m_State[i].nAttribType;
					MagicAttrib.nValue[0] = -pNode->m_State[i].nValue[0];
					MagicAttrib.nValue[1] = -pNode->m_State[i].nValue[1];
					MagicAttrib.nValue[2] = -pNode->m_State[i].nValue[2];
					ModifyAttrib(m_Index, &MagicAttrib);
				}
			}
		}
		pNode = (KStateNode*)pNode->GetNext();
	}
}

#ifndef _SERVER
extern KTabFile g_ClientWeaponSkillTabFile;
#endif

INT		KNpc::GetCurActiveWeaponSkill()
{
	INT nSkillId = 0;
	if (IsPlayer())
	{

		INT nDetailType = Player[m_nPlayerIdx].m_ItemList.GetWeaponType();
		INT nParticularType = Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular();

		//½üÉíÎäÆ÷
		if (nDetailType == 0)
		{
			nSkillId = g_nMeleeWeaponSkill[nParticularType];
		}//Ô¶³ÌÎäÆ÷
		else if (nDetailType == 1)
		{
			nSkillId = g_nRangeWeaponSkill[nParticularType];
		}//¿ÕÊÖ
		else if (nDetailType == -1)
		{
			nSkillId = g_nHandSkill;
		}
	}
	else
	{
#ifdef _SERVER
		//Real Npc
		return 0;
#else
		if (m_Kind == kind_player) // No Local Player
		{
			g_ClientWeaponSkillTabFile.GetInteger(m_WeaponType + 1, "SkillId", 0, &nSkillId);
		}
		else						//Real Npc
		{
			return 0;//
		}
#endif
	}
	return nSkillId;
}

#ifndef _SERVER
void KNpc::ProcNetCommand(NPCCMD cmd, INT x /* = 0 */, INT y /* = 0 */, INT z /* = 0 */)
{
	switch (cmd)
	{
	case do_death:
		DoDeath();
		break;
	case do_hurt:
		DoHurt(x, y, z);
		break;
	case do_revive:
		DoStand();
		m_ProcessAI = 1;
		m_ProcessState = 1;
		SetInstantSpr(enumINSTANT_STATE_REVIVE);
		break;
	case do_stand:
		DoStand();
		m_ProcessAI = 1;
		m_ProcessState = 1;
	default:
		break;
	}
}
#endif

//Fix move lag pos xy
#ifndef _SERVER
//HurtAutoMove()
//AutoFixXY()
void	KNpc::AutoFixXY()
{
	if (Player[CLIENT_PLAYER_INDEX].m_nIndex != m_Index)
	{
		if ((m_sSyncPos.m_nDoing == do_stand
			|| m_sSyncPos.m_nDoing == do_magic
			|| m_sSyncPos.m_nDoing == do_attack
			|| m_sSyncPos.m_nDoing == do_runattack
			|| m_sSyncPos.m_nDoing == do_manyattack
			|| m_sSyncPos.m_nDoing == do_jumpattack
			|| m_sSyncPos.m_nDoing == do_goattack
			)
			&& (m_Doing == do_run || m_Doing == do_walk))
		{
			INT	nRegionIdx;

			if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
			{
				SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
				m_MapX = m_sSyncPos.m_nMapX;
				m_MapY = m_sSyncPos.m_nMapY;
				m_OffX = m_sSyncPos.m_nOffX;
				m_OffY = m_sSyncPos.m_nOffY;
				memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
				SubWorld[0].m_Region[m_RegionIndex].AddNpcRef(m_MapX, m_MapY, obj_npc);
			}
			else
			{
				nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
				if (nRegionIdx < 0)
					return;
				SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
				SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
				m_RegionIndex = nRegionIdx;
				m_dwRegionID = m_sSyncPos.m_dwRegionID;
				m_MapX = m_sSyncPos.m_nMapX;
				m_MapY = m_sSyncPos.m_nMapY;
				m_OffX = m_sSyncPos.m_nOffX;
				m_OffY = m_sSyncPos.m_nOffY;
				memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
			}
		}

	}
	else if (m_Doing == do_sit)
	{
		INT	nRegionIdx;

		if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
			SubWorld[0].m_Region[m_RegionIndex].AddNpcRef(m_MapX, m_MapY, obj_npc);
		}
		else
		{
			nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
			if (nRegionIdx < 0)
				return;
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
		}
	}
}

void	KNpc::HurtAutoMove()
{
	if (this->m_Doing != do_hurt)
		return;

	if (m_sSyncPos.m_nDoing != do_hurt && m_sSyncPos.m_nDoing != do_stand)
		return;

	INT	nFrames, nRegionIdx;

	nFrames = m_Frames.nTotalFrame - m_Frames.nCurrentFrame;
	if (nFrames <= 1)
	{
		if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
			SubWorld[0].m_Region[m_RegionIndex].AddNpcRef(m_MapX, m_MapY, obj_npc);
		}
		else
		{
			nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
			if (nRegionIdx < 0)
				return;
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
		}
	}
	else
	{
		nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
		if (nRegionIdx < 0)
			return;
		INT		nNpcX, nNpcY, nSyncX, nSyncY;
		INT		nNewX, nNewY, nMapX, nMapY, nOffX, nOffY;
		SubWorld[0].Map2Mps(m_RegionIndex,
			m_MapX, m_MapY,
			m_OffX, m_OffY,
			&nNpcX, &nNpcY);
		SubWorld[0].Map2Mps(nRegionIdx,
			m_sSyncPos.m_nMapX, m_sSyncPos.m_nMapY,
			m_sSyncPos.m_nOffX, m_sSyncPos.m_nOffY,
			&nSyncX, &nSyncY);
		nNewX = nNpcX + (nSyncX - nNpcX) / nFrames;
		nNewY = nNpcY + (nSyncY - nNpcY) / nFrames;
		SubWorld[0].Mps2Map(nNewX, nNewY, &nRegionIdx, &nMapX, &nMapY, &nOffX, &nOffY);
		_ASSERT(nRegionIdx >= 0);
		if (nRegionIdx < 0)
			return;
		if (nRegionIdx != m_RegionIndex)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
		}
		else
		{
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY, obj_npc);
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
			SubWorld[0].m_Region[m_RegionIndex].AddNpcRef(m_MapX, m_MapY, obj_npc);
		}
	}
}

#endif
//End code

//TamLTM AutoMoveBarrier(INT x, INT y)
//void KNpc::CheckMoveBarrier(BOOL Obstacle_LT /*12h*/, BOOL Obstacle_RT /*6h*/, BOOL Obstacle_LB /*9h*/, BOOL Obstacle_RB /*3h*/)
INT m_nIndexPlayerRun = 1;
INT timerCountCheckAutoBarrier = 1;

void KNpc::CheckTimerMoveBarrier()
{
	// Chay lien tuc
}

void KNpc::CheckMoveBarrier()
{
#ifndef _SERVER

#endif
}

//void	KNpc::ActiveAutoMoveBarrier(INT moveX, INT moveY)
//{
//#ifndef _SERVER
//
//#endif
//}

//Code chay
INT isCheckAuto = false;
INT countSlowFrameAutoMove = 1;
INT checkPosIndexMove = 0;

void KNpc::DoAutoMoveBarrier(INT arrowMove)
{
	//	g_DebugLog("arrowMove %d ", arrowMove);
#ifdef _SERVER
	INT nX, nY;
	GetMpsPos(&nX, &nY);
	SetPos(nX, nY);
#endif // _SERVER

#ifndef _SERVER

	g_DebugLog("va cham DoAutoMoveBarrier");

	if (m_Doing == do_skill || m_Doing == do_magic ||
		m_Doing == do_attack || m_Doing == do_goattack)
	{
		arrowMove = 0;
		return;
	}

	if (m_Doing == do_stand || m_Doing == do_sit ||
		m_Doing == do_death || m_Doing == do_jump)
	{
		//g_DebugLog("CheckMoveBarrier TamLTM stop hanh dong player");
		INT nX, nY;
		GetMpsPos(&nX, &nY);
		//	MoveToBarrierPlayer(nX, nY, 0);
		arrowMove = 0;
		return;
	}

	m_nCheckAutoMoveBarrier = 1;

	if (GetCheckAutoMoveBarr(isCheckAuto) == false)
		return;

	INT rands; //Random khi va cham va move huong khac

	//1 left //2 right //3 bottom //4 top
	if (Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
	{
		// Huong 1 Move Left top
		if (arrowMove == 1)
		{
			INT nX, nY;
			GetMpsPos(&nX, &nY);

			INT nDir = g_GetDirIndex(nX, nY, m_DesX, m_DesY);

			rands = rand() % 4 + 1;
			//	DoRun();
			//	DoStand(); // Xem lai cho nay

				//Chay qua trai va xuong duoi, duong ngan hon va cham  == 4
			if (m_Doing == do_walk)
			{
				MoveToBarrierPlayer(nX + 100 * 2, nY - 540 * 2, 1);
				checkPosIndexMove = 1;
				Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
				return;
			}
			else
			{
				if (rands == 2)
				{
					if (nDir >= 25 && nDir <= 30)
					{
						MoveToBarrierPlayer(nX - 100, nY - 540, 0);
						checkPosIndexMove = 1;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}
					else if (nDir >= 32 && nDir <= 42)
					{
						MoveToBarrierPlayer(nX + 400, nY - 540, 0);
						checkPosIndexMove = 1;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}
					else
					{
						MoveToBarrierPlayer(nX - 100, nY + 540, 0);
						checkPosIndexMove = 1;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}

					return;
				}
				else
				{
					if (nDir >= 0 && nDir <= 15)
					{
						MoveToBarrierPlayer(nX - 100, nY + 540, 0);
						checkPosIndexMove = 1;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}
					else if (nDir >= 15 && nDir <= 25)
					{
						MoveToBarrierPlayer(nX - 100, nY + 540, 0);
						checkPosIndexMove = 1;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}
					else
					{
						MoveToBarrierPlayer(nX + 100, nY - 540, 0);
						checkPosIndexMove = 1;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}

					return;
				}
			}
		}

		// Huong 2  rand() % 100
		if (arrowMove == 2)
		{
			INT nX, nY;
			GetMpsPos(&nX, &nY);

			INT nDir = g_GetDirIndex(nX, nY, m_DesX, m_DesY);

			rands = rand() % 4 + 1;

			//Chay qua trai va xuong duoi, duong ngan hon va cham  == 2
			if (m_Doing == do_walk)
			{
				MoveToBarrierPlayer(nX - 50 * 2, nY + 540 * 2, 1);
				checkPosIndexMove = 2;
				Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
				return;
			}
			else
			{
				if (rands == 2)
				{
					if (nDir >= 25 && nDir <= 35)
					{
						MoveToBarrierPlayer(nX + 480, nY - 640, 0);
						checkPosIndexMove = 2;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}
					else if (nDir >= 35 && nDir <= 40)
					{
						MoveToBarrierPlayer(nX - 480, nY - 640, 0);
						checkPosIndexMove = 2;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}
					else
					{
						MoveToBarrierPlayer(nX + 480, nY + 640, 0);
						checkPosIndexMove = 2;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}

					return;
				}
				else
				{
					if (nDir >= 63 && nDir <= 55)
					{
						MoveToBarrierPlayer(nX - 400, nY + 640, 0);
						checkPosIndexMove = 2;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}
					else if (nDir >= 63 && nDir <= 55)
					{
						MoveToBarrierPlayer(nX + 400, nY - 640, 0);
						checkPosIndexMove = 2;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}
					else
					{
						MoveToBarrierPlayer(nX + 400, nY + 640, 0);
						checkPosIndexMove = 2;
						Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
						return;
					}

					return;
				}
			}
		}

		// Huong 3
		if (arrowMove == 3)
		{
			INT nX, nY;
			GetMpsPos(&nX, &nY);

			rands = rand() % 4 + 1;

			// Chay len tren va cham == 3
			if (m_Doing == do_walk)
			{
				MoveToBarrierPlayer(nX - 400 * 2, nY - 440 * 2, 1);
				checkPosIndexMove = 3;
				Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
				return;
			}
			else
			{
				if (rands == 3)
				{
					MoveToBarrierPlayer(nX + 420, nY + 680, 0);
					checkPosIndexMove = 3;
					Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
					return;
				}
				else
				{
					MoveToBarrierPlayer(nX - 420, nY - 680, 0);
					checkPosIndexMove = 3;
					Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
					return;
				}
			}
		}

		// Huong 4
		if (arrowMove == 4)
		{
			INT nX, nY;
			GetMpsPos(&nX, &nY);

			rands = rand() % 4 + 1;

			//Chay qua trai va xuong duoi, duong ngan hon va cham  == 4
			if (m_Doing == do_walk)
			{
				MoveToBarrierPlayer(nX + 410 * 2, nY + 540 * 2, 1);
				checkPosIndexMove = 4;
				Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
				return;
			}
			else
			{
				if (rands == 4)
				{
					MoveToBarrierPlayer(nX - 450, nY + 650, 0);
					checkPosIndexMove = 4;
					Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
					return;
				}
				else
				{
					MoveToBarrierPlayer(nX + 480, nY + 680, 0);
					checkPosIndexMove = 4;
					Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
					return;
				}
			}
		}
	}

	arrowMove = 0;
#endif

}

#ifndef _SERVER
void KNpc::MiniMapXY(INT nX, INT nY)
{
	m_nMoveToFlagMiniMapX = nX;
	m_nMoveToFlagMiniMapY = nY;

	//	g_DebugLog("%d + %d dfas", m_nMoveToFlagMiniMapX, m_nMoveToFlagMiniMapY);
}

void KNpc::MoveToBarrierPlayer(INT nX, INT nY, INT isCheckMoveCalculator)
{
	if (m_Doing == do_skill || m_Doing == do_magic ||
		m_Doing == do_attack || m_Doing == do_goattack)
	{
		return;
	}

	// Di bo
	if (isCheckMoveCalculator == 1)
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_walk, nX, nY);
		SendClientCmdWalk(nX, nY);
	}
	else // Chay
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_run, nX, nY);
		SendClientCmdRun(nX, nY);
	}
}

BOOL KNpc::GetCheckAutoMoveBarr(BOOL isCheck)
{
	isCheckAuto = isCheck;
	return isCheckAuto;
}
#endif

//end code chay

#ifndef _SERVER
void	KNpc::AddBlood(INT nNo)
{
	if (nNo > 0)
	{
		m_nBlood[m_btCurBlood] = BLOOD_EVENTTIME;
		itoa(nNo, m_szBlood[m_btCurBlood], 10);

		m_btCurBlood++;
		if (m_btCurBlood >= BLOOD_COUNT)
			m_btCurBlood = 0;
	}
}

INT	KNpc::PaintBlood(INT nHeightOffset)
{
	INT i = 0;
	while (i < BLOOD_COUNT)
	{
		if (m_szBlood[i][0] == 0)
		{
			i++;
			continue;
		}

		INT	nMpsX, nMpsY;
		GetMpsPos(&nMpsX, &nMpsY);

		g_pRepresent->OutputText(BLOOD_FONTSIZE, m_szBlood[i], KRF_ZERO_END, nMpsX - BLOOD_FONTSIZE * g_StrLen(m_szBlood[i]) / 4, nMpsY,
			0x00ff0000, 0, nHeightOffset + (BLOOD_EVENTTIME - m_nBlood[i]) * BLOOD_MOVESPEED / 5, 0xff000000);

		m_nBlood[i]--;
		if (m_nBlood[i] <= 0)
		{
			m_szBlood[i][0] = 0;
			break;
		}
		i++;
	}
	return nHeightOffset;
}

#endif

#ifndef _SERVER
INT	KNpc::GetNpcPate()
{
	INT nHeight = m_Height + m_nStature;
	if (m_Kind == kind_player)
	{
		if (m_nSex)
			nHeight += 84;	//Å®
		else
			nHeight += 84;	//ÄÐ

		if (m_MaskType <= 0)
		{
			if (m_DataRes.IgnoreShowRes() == FALSE && m_Doing == do_sit && MulDiv(10, m_Frames.nCurrentFrame, m_Frames.nTotalFrame) >= 8)
				nHeight -= MulDiv(30, m_Frames.nCurrentFrame, m_Frames.nTotalFrame);

			if (m_bRideHorse)
				nHeight += 38;	//ÆïÂí
		}
		else
		{
			nHeight += 16;
		}
	}

	return nHeight;
}
#endif

#ifndef _SERVER
INT	KNpc::GetNpcPatePeopleInfo()
{
	INT nFontSize = 12;
	if (m_nChatContentLen > 0 && m_nChatNumLine > 0)
		return m_nChatNumLine * (nFontSize + 1);

	INT nHeight = 0;
	if (NpcSet.CheckShowLife())
	{
		if (m_Kind == kind_player || m_Kind == kind_partner)
		{
			if (m_CurrentLifeMax > 0 && (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex)) &&
				Npc[m_Index].m_nPKFlag != enumPKMurder
				)
				nHeight += SHOW_LIFE_HEIGHT;
		}
	}
	if (NpcSet.CheckShowName())
	{
		if (nHeight != 0)
			nHeight += SHOW_SPACE_HEIGHT;//ºÃ¿´

		if (m_Kind == kind_player || m_Kind == kind_dialoger)
			nHeight += nFontSize + 1;
	}
	return nHeight;
}
#endif


void KNpc::SwitchRideHorse(BOOL bRideHorse)
{
	INT nIdx = Player[m_nPlayerIdx].m_ItemList.GetEquipment(itempart_horse);
	if (nIdx <= 0)
		return;

	m_bRideHorse = bRideHorse;

	if (m_bRideHorse)
		Item[nIdx].ApplyMagicAttribToNPC(&Npc[m_Index], MAX_ITEM_MAGICATTRIB / 2);
	else
		Item[nIdx].RemoveMagicAttribFromNPC(&Npc[m_Index], MAX_ITEM_MAGICATTRIB / 2);

#ifdef _SERVER

	NPC_HORSE_SYNC	NetCommand;
	NetCommand.ProtocolType = BYTE(s2c_npchorsesync);
	NetCommand.m_dwID = m_dwID;
	NetCommand.m_bRideHorse = bRideHorse;
	if (m_RegionIndex < 0)
		return;
	INT nMaxCount = MAX_BROADCAST_COUNT;
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
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif
}

#ifdef _SERVER
#define		MAX_SWITCH_HORSE_FIGHT_ACTIVE		90
#define		MAX_SWITCH_HORSE_FIGHT_NONE			36
BOOL KNpc::CanSwitchRideHorse()
{
	if (Player[m_nPlayerIdx].CheckTrading())
		return FALSE;

	if (Player[m_nPlayerIdx].m_ItemList.GetEquipment(itempart_horse) <= 0)
		return FALSE;

	if (Npc[Player[m_nPlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_HORSE_CANT_SWITCH2;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	if (m_dwNextSwitchHorseTime <= 0 ||
		(g_SubWorldSet.GetGameTime() >= m_dwNextSwitchHorseTime))
	{
		m_dwNextSwitchHorseTime = g_SubWorldSet.GetGameTime() + (m_FightMode ? MAX_SWITCH_HORSE_FIGHT_ACTIVE : MAX_SWITCH_HORSE_FIGHT_NONE);
		return TRUE;
	}
	else
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_HORSE_CANT_SWITCH1;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	return FALSE;
}
#endif

void	KNpc::ResetNpcTypeName(INT nMark)
{

#ifndef _SERVER
	char	szNpcTypeName[32];
#endif
	if (nMark == 1)
	{
#ifndef _SERVER
		if (m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID)
		{
			strcpy(szNpcTypeName, "MainMan");
			m_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(TRUE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(TRUE);
		}
		else
		{
			strcpy(szNpcTypeName, "MainLady");
			m_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(FALSE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(FALSE);
		}
#endif
		m_WalkSpeed = NpcSet.GetPlayerWalkSpeed();
		m_RunSpeed = NpcSet.GetPlayerRunSpeed();
		m_AttackFrame = NpcSet.GetPlayerAttackFrame();
		m_HurtFrame = NpcSet.GetPlayerHurtFrame();
	}
	else
	{
		GetNpcCopyFromTemplate(m_MaskType, m_Level);
#ifndef _SERVER	
		g_NpcSetting.GetString(m_MaskType + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
		if (!szNpcTypeName[0])
		{
			g_NpcKindFile.GetString(2, "CharacterName", "", szNpcTypeName, sizeof(szNpcTypeName));//Èç¹ûÃ»ÕÒµ½£¬ÓÃµÚÒ»¸önpc´úÌæ
		}
#endif
	}
#ifndef _SERVER
	this->RemoveRes();
	m_DataRes._Init(szNpcTypeName, &g_NpcResList);

	m_DataRes.SetAction(m_ClientDoing);
	if (m_bRideHorse && m_ClientDoing == cdo_jump)
		m_DataRes.SetRideHorse(FALSE);
	else
		m_DataRes.SetRideHorse(m_bRideHorse);
	m_DataRes.SetArmor(m_ArmorType, m_MantleType);
	m_DataRes.SetHelm(m_HelmType);
	m_DataRes.SetHorse(m_HorseType);
	m_DataRes.SetWeapon(m_WeaponType);
#endif
}

void KNpc::SetRank(INT nRank)
{
	m_RankID = nRank;
#ifdef _SERVER
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
	TITLE_SYNC	NetCommand;
	NetCommand.ProtocolType = BYTE(s2c_titlename);
	NetCommand.ID = m_dwID;
	NetCommand.Rank = m_RankID;
	if (m_RegionIndex < 0)
		return;
	INT nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);
	INT i;
	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}

#endif
}

void KNpc::SetExpandRank(KExpandRank* ExpandRank)
{
	INT i;
	KStateNode* pNode;
	// Ã»ÓÐÔÚÑ­»·ÖÐ·µ»Ø£¬ËµÃ÷ÊÇÐÂ¼¼ÄÜ
	BOOL bAddNewStateGraphics = TRUE;

	INT nCurStateGraphics = m_CurExpandRank.nStateGraphics;

	m_CurExpandRank = *ExpandRank;
	if (m_CurExpandRank.dwLeftTime > KSG_GetCurSec())
		m_ExpandRank = *ExpandRank;

	pNode = (KStateNode*)m_StateSkillList.GetHead();
	while (pNode)
	{
		if (pNode->m_bTempStateGraphics &&
			(pNode->m_StateGraphics == nCurStateGraphics))
		{
			bAddNewStateGraphics = FALSE;
			pNode->m_StateGraphics = m_CurExpandRank.nStateGraphics;
		}
		pNode = (KStateNode*)pNode->GetNext();
	}

	if (bAddNewStateGraphics)
	{
		pNode = new KStateNode;
		pNode->m_SkillID = 0;
		pNode->m_Level = 0;
		pNode->m_LeftTime = -1;
		pNode->m_bOverLook = TRUE;
		pNode->m_bTempStateGraphics = TRUE;
		pNode->m_StateGraphics = m_CurExpandRank.nStateGraphics;
		m_StateSkillList.AddTail(pNode);
	}

#ifdef _SERVER
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
	EXPANDTITLE_SYNC	NetCommand;
	NetCommand.ProtocolType = BYTE(s2c_expandrank);
	NetCommand.ID = m_dwID;
	memcpy(&NetCommand.ExpandRank, &m_CurExpandRank, sizeof(m_CurExpandRank));
	if (m_RegionIndex < 0)
		return;

	INT nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX, m_MapY);

	for (i = 0; i < 8; i++)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&NetCommand, sizeof(NetCommand), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
#endif

#ifdef _SERVER	
	UpdateNpcStateInfo();
#endif
}

#ifdef _SERVER	
// 广播状态数据  光环持续状态的同步
void KNpc::BroadCastState()
{
	if (m_SubWorldIndex < 0 || m_SubWorldIndex >= MAX_SUBWORLD || m_RegionIndex < 0)
		return;

	NPC_SYNC_STATEINFO StateInfo;
	StateInfo.ProtocolType = (BYTE)s2c_syncnpcstate;
	StateInfo.m_ID = m_dwID; // 状态持有者
	INT i;
	for (i = 0; i < MAX_NPC_RECORDER_STATE; ++i)
		StateInfo.m_btStateInfo[i] = m_btStateInfo[i]; // 需需要同步的光环，技能状态ID号

	POINT POff[8] =
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
	INT nMaxCount = MAX_BROADCAST_COUNT; //;
	CURREGION.BroadCast(&StateInfo, sizeof(NPC_SYNC_STATEINFO), nMaxCount, m_MapX, m_MapY);

	for (i = 0; i < 8; ++i)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(&StateInfo, sizeof(NPC_SYNC_STATEINFO), nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
}
#endif