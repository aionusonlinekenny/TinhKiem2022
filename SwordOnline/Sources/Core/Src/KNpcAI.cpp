#include "KCore.h"
#include "KNpc.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KMath.h"
#include "KObj.h"
#include "KPlayer.h"
#include "KNpcAI.h"
#include "KSellItem.h"

// flying add here, to use math lib
#include <math.h>
extern INT GetRandomNumber(INT nMin, INT nMax);

#define		MAX_FOLLOW_DISTANCE		48
#define		FOLLOW_WALK_DISTANCE	100

KNpcAI NpcAI;

KNpcAI::KNpcAI()
{
	m_nIndex = 0;
	m_bActivate = TRUE;
}

// flying modified this function.
// Jun.4.2003
void KNpcAI::Activate(INT nIndex)
{
	m_nIndex = nIndex;
	if (Npc[m_nIndex].IsPlayer() && Npc[m_nIndex].m_nFollowIndex == 0)
	{
		// 新增的Player AI在这里调用实现。
		ProcessPlayer();
		return;
	}
#ifdef _SERVER
	if (Npc[m_nIndex].m_CurrentLifeMax == 0)
		return;

	if (Npc[m_nIndex].m_nOwnerIndex > 0 && Npc[m_nIndex].m_bNpcFollowFindPath)
	{
		FindPathNpc();
		return;
	}

	if (Npc[m_nIndex].m_nFollowIndex > 0 && Npc[m_nIndex].m_SubWorldIndex != Npc[Npc[m_nIndex].m_nFollowIndex].m_SubWorldIndex)
	{
		INT nPosX, nPosY;
		Npc[m_nIndex].GetMpsPos(&nPosX, &nPosY);

		Npc[Npc[m_nIndex].m_nFollowIndex].ChangeWorld(SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID, nPosX, nPosY);

		return;
	}

	INT nCurTime = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwCurrentTime;
	if (/*Npc[m_nIndex].m_nPeopleIdx ||*/Npc[m_nIndex].m_NextAITime <= nCurTime)
	{
		Npc[m_nIndex].m_NextAITime = nCurTime + Npc[m_nIndex].m_AIMAXTime;
		switch (Npc[m_nIndex].m_AiMode)
		{
		case 1:
			ProcessAIType01();
			break;
		case 2:
			ProcessAIType02();
			break;
		case 3:
			ProcessAIType03();
			break;
		case 4:
			ProcessAIType04();
			break;
		case 5:
			ProcessAIType05();
			break;
		case 6:
			ProcessAIType06();
			break;
			/*		case 7:
						ProcessAIType7();
						break;
					case 8:
						ProcessAIType8();
						break;
					case 9:
						ProcessAIType9();
						break;
					case 10:
						ProcessAIType10();
						break;*/
		default:
			break;
		}
	}
	// flying add the code for the macro such as "_CLIENT".
	// because this code only run at client.
#else
	/*	if (Npc[m_nIndex].m_Kind >= kind_bird && Npc[m_nIndex].m_AiMode > 10)
		{
			if (CanShowNpc())
			{
				// 让NPC随机休息一下，是个好建议。
				if (GetRandomNumber(0, 1))
				{
					Npc[m_nIndex].m_AiParam[5] = 0;
					Npc[m_nIndex].m_AiParam[4] = 5;
					return;
				}
				if (!KeepActiveRange())
					ProcessShowNpc();
			}
		}*/
#endif
}


#define	MAX_FIND_PATH_NPC_DISTANCE 750
#define	MIN_FIND_PATH_NPC_DISTANCE 32
#define	MAX_WAIT_PATH_NPC_TIME 5*60*20 //5 minute
#define	MAX_FIND_PATH_NPC_TIME 30*60*20 //30 minute

#ifdef _SERVER
void	KNpcAI::FindPathNpc()
{
	INT nIdx = Npc[m_nIndex].m_nOwnerIndex; //Npc[m_nIndex].FindAroundPlayer(Npc[m_nIndex].Owner);

	if (nIdx <= 0 || Npc[nIdx].m_dwID <= 0)
		return;

	if (Npc[m_nIndex].m_SubWorldIndex != Npc[nIdx].m_SubWorldIndex)
	{
		return;
	}

	if (Npc[nIdx].m_Doing == do_death || Npc[nIdx].m_Doing == do_revive)
		return;

	if (Npc[m_nIndex].m_uFindPathTime)
	{
		if (Npc[m_nIndex].m_uFindPathMaxTime != -1)
		{
			DWORD dwTime = MAX_FIND_PATH_NPC_TIME;

			if (Npc[m_nIndex].m_uFindPathMaxTime > 0)
				dwTime = Npc[m_nIndex].m_uFindPathMaxTime;

			if (g_SubWorldSet.GetGameTime() - Npc[m_nIndex].m_uFindPathTime > dwTime)
			{
				if (Npc[m_nIndex].m_RegionIndex >= 0)
				{
					SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].RemoveNpc(m_nIndex);
					SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].DecNpcRef(Npc[m_nIndex].m_MapX, Npc[m_nIndex].m_MapY, obj_npc);
				}
				NpcSet.Remove(m_nIndex);
				return;
			}
		}
	}
	// 取得到目标的距离
	INT distance = NpcSet.GetDistance(nIdx, m_nIndex);
	if (distance <= MAX_FIND_PATH_NPC_DISTANCE)
	{
		if (Npc[m_nIndex].m_CurrentCamp != camp_event)
			Npc[m_nIndex].SetCurrentCamp(camp_event);

		if (distance > MIN_FIND_PATH_NPC_DISTANCE)
		{
			if (Npc[m_nIndex].m_uLastFindPathTime)
				Npc[m_nIndex].m_uLastFindPathTime = 0;

			INT nDesX, nDesY;
			Npc[nIdx].GetMpsPos(&nDesX, &nDesY);
			Npc[m_nIndex].SendCommand(do_walk, nDesX, nDesY);
		}
	}
	else
	{
		if (Npc[m_nIndex].m_CurrentCamp != camp_animal)
			Npc[m_nIndex].SetCurrentCamp(camp_animal);

		if (Npc[m_nIndex].m_uLastFindPathTime <= 0)
			Npc[m_nIndex].m_uLastFindPathTime = g_SubWorldSet.GetGameTime();
		else
		{
			if (g_SubWorldSet.GetGameTime() - Npc[m_nIndex].m_uLastFindPathTime > MAX_WAIT_PATH_NPC_TIME)
			{
				if (Npc[m_nIndex].m_RegionIndex >= 0)
				{
					SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].RemoveNpc(m_nIndex);
					SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].DecNpcRef(Npc[m_nIndex].m_MapX, Npc[m_nIndex].m_MapY, obj_npc);
				}
				NpcSet.Remove(m_nIndex);
			}
		}
	}
}
#endif
//---------------------------------------------------------------------
// flying add these functions
// Run at client.
#ifndef _SERVER
// 仅仅有画面效果的NPC
INT KNpcAI::ProcessShowNpc()
{
	INT nResult = false;
	INT nRetCode = false;

	switch (Npc[m_nIndex].m_AiMode)
	{
		// 飞鸟型
	case 11:
		nRetCode = ShowNpcType11();
		if (!nRetCode)
			goto Exit0;
		break;
		// 蜻蜓型
	case 12:
		nRetCode = ShowNpcType12();
		if (!nRetCode)
			goto Exit0;
		break;
		// 鱼类型
	case 13:
		nRetCode = ShowNpcType13();
		if (!nRetCode)
			goto Exit0;
		break;
		// 老鼠型
	case 14:
		nRetCode = ShowNpcType14();
		if (!nRetCode)
			goto Exit0;
		break;
		// 鸡犬型
	case 15:
		nRetCode = ShowNpcType15();
		if (!nRetCode)
			goto Exit0;
		break;
		// 兔子型
	case 16:
		nRetCode = ShowNpcType16();
		if (!nRetCode)
			goto Exit0;
		break;
		// 蝴蝶型
	case 17:
		nRetCode = ShowNpcType17();
		if (!nRetCode)
			goto Exit0;
		break;
	default:
		break;
	}

	nResult = true;
Exit0:
	return nResult;
}
// 飞鸟型
INT KNpcAI::ShowNpcType11()
{
	INT nResult = false;
	INT nRetCode = false;

	KNpc& aNpc = Npc[m_nIndex];
	// Go the distance between P1 to P2	
	INT nDistance = 0;
	INT nDesX = 0;
	INT nDesY = 0;
	INT nCurX = 0;
	INT nCurY = 0;
	INT nOffX = 0;
	INT nOffY = 0;
	INT nOffsetDir = 0;

	// 效果加强 随机调整高度
	aNpc.m_Height = GetRandomNumber(aNpc.m_AiParam[6] - 4, aNpc.m_AiParam[6]);

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// 计算新角度 和 距离
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];

	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;

	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	// 取得运动所需的时间，保存在参数表中
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (INT)nDistance / (INT)aNpc.m_CurrentWalkSpeed;
		aNpc.m_AiParam[5] = 0;
	}
	//if (KeepActiveShowRange())
	//	aNpc.m_Dir += 32;
	aNpc.m_Dir += nOffsetDir;
	if (aNpc.m_Dir < 0)
		aNpc.m_Dir += 64;
	else
		aNpc.m_Dir %= 64;

	// 根据三角函数计算偏移的X、Y数值
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;

	// 获取目标坐标
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// 蜻蜓型
// done
INT KNpcAI::ShowNpcType12()
{
	INT nResult = false;
	INT nRetCode = false;

	// Go the distance between P1 to P2	
	INT nDistance = 0;
	INT nDesX = 0;
	INT nDesY = 0;
	INT nCurX = 0;
	INT nCurY = 0;
	INT nOffX = 0;
	INT nOffY = 0;
	INT nOffsetDir = 0;
	KNpc& aNpc = Npc[m_nIndex];

	// 效果加强 随机调整高度
	aNpc.m_Height = GetRandomNumber(aNpc.m_AiParam[6] - 4, aNpc.m_AiParam[6]);

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// 计算新角度 和 距离
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	// 取得运动所需的时间，保存在参数表中
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (INT)nDistance / (INT)aNpc.m_CurrentWalkSpeed;
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}
	aNpc.m_Dir += nOffsetDir;
	if (aNpc.m_Dir < 0)
		aNpc.m_Dir += 64;
	else
		aNpc.m_Dir %= 64;

	// 根据三角函数计算偏移的X、Y数值
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;
	// 获取目标坐标
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// 鱼类型
// done
INT KNpcAI::ShowNpcType13()
{
	INT nResult = false;
	INT nRetCode = false;
	// Go the distance between P1 to P2	
	INT nDistance = 0;
	INT nDesX = 0;
	INT nDesY = 0;
	INT nCurX = 0;
	INT nCurY = 0;
	INT nOffX = 0;
	INT nOffY = 0;
	INT nOffsetDir = 0;
	INT nIndex = 0;
	KNpc& aNpc = Npc[m_nIndex];

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// 计算新角度 和 距离
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	// 取得运动所需的时间，保存在参数表中
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (INT)nDistance / (INT)aNpc.m_CurrentWalkSpeed;
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}
	//if (KeepActiveShowRange())
	//	aNpc.m_Dir += 32;
	// 附近有玩家
	nIndex = IsPlayerCome();
	if (nIndex > 0)
	{
		// do flee
		DoShowFlee(nIndex);
		goto Exit0;
	}
	// 根据三角函数计算偏移的X、Y数值
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	// 获取目标坐标
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// 老鼠型
// done
INT KNpcAI::ShowNpcType14()
{
	INT nResult = false;
	INT nRetCode = false;

	INT nDistance = 0;
	INT nDesX = 0;
	INT nDesY = 0;
	INT nCurX = 0;
	INT nCurY = 0;
	INT nOffX = 0;
	INT nOffY = 0;
	INT nRandom = 0;
	INT nOffsetDir = 0;
	KNpc& aNpc = Npc[m_nIndex];

	nRandom = GetRandomNumber(1, 10);
	// 掉头就跑
	if (nRandom < 4)
		nDistance = -nDistance;
	// 嗷嗷发呆
	else if (nRandom < 7)
	{
		aNpc.SendCommand(do_stand);
		goto Exit0;
	}
	aNpc.GetMpsPos(&nCurX, &nCurY);
	// 计算新角度 和 距离
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);
	// 取得运动所需的时间，保存在参数表中
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (INT)nDistance / (INT)aNpc.m_CurrentWalkSpeed;
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}

	aNpc.m_Dir += nOffsetDir;
	if (aNpc.m_Dir < 0)
		aNpc.m_Dir += 64;
	else
		aNpc.m_Dir %= 64;
	// 根据三角函数计算偏移的X、Y数值
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;
	// 获取目标坐标
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// 鸡犬型
INT KNpcAI::ShowNpcType15()
{
	INT nResult = false;
	INT nRetCode = false;
	// Go the distance between P1 to P2	
	INT nDistance = 0;
	INT nDesX = 0;
	INT nDesY = 0;
	INT nCurX = 0;
	INT nCurY = 0;
	INT nOffX = 0;
	INT nOffY = 0;
	INT nOffsetDir = 0;
	INT nIndex = 0;
	KNpc& aNpc = Npc[m_nIndex];

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// 计算新角度 和 距离
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	// 取得运动所需的时间，保存在参数表中
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (INT)nDistance / (INT)aNpc.m_CurrentWalkSpeed;
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}
	//if (KeepActiveShowRange())
	//	aNpc.m_Dir += 32;
	// 附近有玩家
	nIndex = IsPlayerCome();
	if (nIndex > 0)
	{
		// do flee
		DoShowFlee(nIndex);
		goto Exit0;
	}
	// 根据三角函数计算偏移的X、Y数值
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	// 获取目标坐标
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}

// 兔子型
INT KNpcAI::ShowNpcType16()
{
	INT nResult = false;
	INT nRetCode = false;

	// Go the distance between P1 to P2
	register INT nDistance = 0;
	INT nDesX = 0;
	INT nDesY = 0;
	INT nCurX = 0;
	INT nCurY = 0;
	INT nOffX = 0;
	INT nOffY = 0;
	INT nOffsetDir = 0;
	INT nIndex = 0;
	KNpc& aNpc = Npc[m_nIndex];

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// 计算新角度 和 距离
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);
	// 附近有玩家
	nIndex = IsPlayerCome();
	if (nIndex > 0)
	{
		// do flee
		nRetCode = DoShowFlee(nIndex);
		if (!nRetCode)
			goto Exit0;
		goto Exit1;
	}

	// 计算距离
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (INT)nDistance / (INT)aNpc.m_CurrentWalkSpeed;
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}

	// 计算新角度
	//if (KeepActiveShowRange())
	//	aNpc.m_Dir += 32;
	aNpc.m_Dir += GetRandomNumber(0, 6);
	aNpc.m_Dir %= 64;
	// 根据三角函数计算偏移的X、Y数值
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;
	// 获取目标坐标
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

Exit1:
	nResult = true;
Exit0:
	return nResult;
}

// 蝴蝶型
INT KNpcAI::ShowNpcType17()
{
	INT nResult = false;
	INT nRetCode = false;

	// Go the distance between P1 to P2
	INT nDistance = 0;
	INT nDesX = 0;
	INT nDesY = 0;
	INT nCurX = 0;
	INT nCurY = 0;
	INT nOffX = 0;
	INT nOffY = 0;
	INT nOffsetDir = 0;
	KNpc& aNpc = Npc[m_nIndex];

	// 效果加强 随机调整高度
	aNpc.m_Height = GetRandomNumber(aNpc.m_AiParam[6] - 4, aNpc.m_AiParam[6]);

	aNpc.GetMpsPos(&nCurX, &nCurY);

	// 计算新角度 和 距离
	if (aNpc.m_AiParam[3] > 0)
		nOffsetDir = GetRandomNumber(aNpc.m_AiParam[3], aNpc.m_AiParam[2]);
	else
		nOffsetDir = aNpc.m_AiParam[2];
	if (GetRandomNumber(0, 1))
		nOffsetDir = -nOffsetDir;
	nDistance = GetRandomNumber(aNpc.m_AiParam[0] - aNpc.m_AiParam[1], aNpc.m_AiParam[0]);

	// 取得运动所需的时间，保存在参数表中
	if (aNpc.m_CurrentWalkSpeed > 0)
	{
		aNpc.m_AiParam[4] = (INT)nDistance / (INT)aNpc.m_CurrentWalkSpeed;
		aNpc.m_AiParam[5] = 0;
	}
	else
	{
		aNpc.m_AiParam[4] = 0;
		aNpc.m_AiParam[5] = 0;
	}
	if (KeepActiveRange())
	{
		//aNpc.SendCommand(do_walk, aNpc.m_OriginX, aNpc.m_OriginY);
		goto Exit0;
		//aNpc.m_Dir += 32;
	}
	aNpc.m_Dir += nOffsetDir;
	//aNpc.m_Dir += GetRandomNumber(32, 64);
	if (aNpc.m_Dir < 0)
		aNpc.m_Dir += 64;
	else
		aNpc.m_Dir %= 64;
	// 根据三角函数计算偏移的X、Y数值
	nRetCode = GetNpcMoveOffset(aNpc.m_Dir, nDistance, &nOffX, &nOffY);
	if (!nRetCode)
		goto Exit0;
	// 获取目标坐标
	nDesX = nCurX + nOffX;
	nDesY = nCurY + nOffY;
	aNpc.SendCommand(do_walk, nDesX, nDesY);

	nResult = true;
Exit0:
	return nResult;
}
#endif
//---------------------------------------------------------------------
// Player AI add here.
// flying comment
void KNpcAI::ProcessPlayer()
{
#ifndef _SERVER
	INT i = Npc[m_nIndex].m_nPeopleIdx;

	if (i > 0)
	{
		FollowPeople(i);
	}
	i = Npc[m_nIndex].m_nObjectIdx;
	if (i > 0)
	{
		FollowObject(i);
	}
#endif
}

#ifndef _SERVER
void KNpcAI::FollowObject(INT nIdx)
{
	INT nX1, nY1, nX2, nY2;
	Npc[m_nIndex].GetMpsPos(&nX1, &nY1);
	Object[nIdx].GetMpsPos(&nX2, &nY2);

	//if ((nX1 - nX2) * (nX1 - nX2) + (nY1 - nY2) * (nY1 - nY2) < PLAYER_PICKUP_CLIENT_DISTANCE * PLAYER_PICKUP_CLIENT_DISTANCE)
	{
		//#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].CheckObject(nIdx);
		//#endif
	}
}
#endif

BOOL KNpcAI::CheckNpc(INT nIndex)
{
	if (nIndex < 0 || /*Npc[nIndex].m_SubWorldIndex != Npc[m_nIndex].m_SubWorldIndex || */
		Npc[nIndex].m_RegionIndex < 0 ||
		Npc[nIndex].m_CurrentLifeMax <= 0 ||
		Npc[nIndex].m_HideState.nTime > 0 ||
		!Npc[nIndex].IsAlive())
	{
		return TRUE;
	}
	return FALSE;
}

#ifndef _SERVER
void KNpcAI::FollowPeople(INT nIdx)
{
	if (CheckNpc(nIdx))
	{
		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}

	// 取得到目标的距离
	INT distance = NpcSet.GetDistance(nIdx, m_nIndex);
	INT	nRelation = NpcSet.GetRelation(m_nIndex, nIdx);

	// 小于对话半径就开始对话
	if ((Npc[nIdx].m_Kind == kind_dialoger))
	{
		if (distance <= Npc[nIdx].m_DialogRadius)
		{
			INT x, y;
			SubWorld[Npc[m_nIndex].m_SubWorldIndex].Map2Mps(Npc[m_nIndex].m_RegionIndex, Npc[m_nIndex].m_MapX, Npc[m_nIndex].m_MapY, Npc[m_nIndex].m_OffX, Npc[m_nIndex].m_OffY, &x, &y);
			Npc[m_nIndex].SendCommand(do_walk, x, y);
			SendClientCmdWalk(x, y);
			Player[CLIENT_PLAYER_INDEX].DialogNpc(nIdx);
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
			Npc[nIdx].TurnTo(Player[CLIENT_PLAYER_INDEX].m_nIndex);
			return;
		}
	}
	// 距离小于攻击范围就开始攻击
	if (nRelation == relation_enemy)
	{
		if (distance <= Npc[m_nIndex].m_CurrentAttackRadius)
		{
			INT nNpcX, nNpcY;
			Npc[m_nIndex].GetMpsPos(&nNpcX, &nNpcY);

			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, nIdx);
			SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, -1, Npc[nIdx].m_dwID, nNpcX, nNpcY);
		}
		// 嗷嗷追
		else
		{
			INT nDesX, nDesY;
			Npc[nIdx].GetMpsPos(&nDesX, &nDesY);
			// modify by spe 2003/06/13
			if (Player[CLIENT_PLAYER_INDEX].m_RunStatus)
			{
				Npc[m_nIndex].SendCommand(do_run, nDesX, nDesY);
				SendClientCmdRun(nDesX, nDesY);
			}
			else
			{
				Npc[m_nIndex].SendCommand(do_walk, nDesX, nDesY);
				SendClientCmdWalk(nDesX, nDesY);
			}
		}
		return;
	}
	// 跟随
	if (Npc[nIdx].m_Kind == kind_player)
	{
		// flow
		INT nDesX, nDesY;
		if (distance < MAX_FOLLOW_DISTANCE)
		{
			Npc[this->m_nIndex].GetMpsPos(&nDesX, &nDesY);
			Npc[m_nIndex].SendCommand(do_walk, nDesX, nDesY);
			SendClientCmdWalk(nDesX, nDesY);
		}
		else
		{
			Npc[nIdx].GetMpsPos(&nDesX, &nDesY);
			if (distance < FOLLOW_WALK_DISTANCE ||
				!Player[CLIENT_PLAYER_INDEX].m_RunStatus)
			{
				Npc[m_nIndex].SendCommand(do_walk, nDesX, nDesY);
				SendClientCmdWalk(nDesX, nDesY);
			}
			else
			{
				Npc[m_nIndex].SendCommand(do_run, nDesX, nDesY);
				SendClientCmdRun(nDesX, nDesY);
			}
		}
	}
	return;
}
#endif

INT KNpcAI::GetNearestNpc(INT nRelation)
{
	INT nRangeX = Npc[m_nIndex].m_VisionRadius;
	INT	nRangeY = nRangeX;
	INT	nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	INT	nRegion = Npc[m_nIndex].m_RegionIndex;
	INT	nMapX = Npc[m_nIndex].m_MapX;
	INT	nMapY = Npc[m_nIndex].m_MapY;
	INT	nRet;
	INT	nRMx, nRMy, nSearchRegion;

	nRangeX = nRangeX / SubWorld[nSubWorld].m_nCellWidth;
	nRangeY = nRangeY / SubWorld[nSubWorld].m_nCellHeight;

	// 检查视野范围内的格子里的NPC
	for (INT i = 0; i < nRangeX; i++)	// i, j由0开始而不是从-range开始是要保证Nearest
	{
		for (INT j = 0; j < nRangeY; j++)
		{
			// 去掉边角几个格子，保证视野是椭圆形
			if ((i * i + j * j) > nRangeX * nRangeX)
				continue;

			// 确定目标格子实际的REGION和坐标确定
			nRMx = nMapX + i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			// 从REGION的NPC列表中查找满足条件的NPC			
			nRet = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			if (Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			if (nRet > 0)
				return nRet;
			// 确定目标格子实际的REGION和坐标确定
			nRMx = nMapX - i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			// 从REGION的NPC列表中查找满足条件的NPC			
			nRet = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			if (Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			if (nRet > 0)
				return nRet;
			// 确定目标格子实际的REGION和坐标确定
			nRMx = nMapX - i;
			nRMy = nMapY - j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			// 从REGION的NPC列表中查找满足条件的NPC			
			nRet = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			if (Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			if (nRet > 0)
				return nRet;
			// 确定目标格子实际的REGION和坐标确定
			nRMx = nMapX + i;
			nRMy = nMapY - j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			// 从REGION的NPC列表中查找满足条件的NPC
			nRet = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			if (Npc[nRet].m_HideState.nTime > 0)
				nRet = 0;
			if (nRet > 0)
				return nRet;
		}
	}
	return 0;
}

#ifndef _SERVER
// flying add this
// 查找离某个NPC最近的玩家
INT KNpcAI::IsPlayerCome()
{
	INT nResult = 0;
	INT nPlayer = 0;
	INT X1 = 0;
	INT Y1 = 0;
	INT X2 = 0;
	INT Y2 = 0;
	INT distance = 0;

	nPlayer = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	distance = NpcSet.GetDistance(nPlayer, m_nIndex);
	// 鸡看的到的玩家
	if (distance < Npc[m_nIndex].m_VisionRadius)
	{
		// 分别处理走和跑
		if (Player[CLIENT_PLAYER_INDEX].m_RunStatus ||
			Npc[m_nIndex].m_CurrentVisionRadius > distance * 4)
		{
			nResult = nPlayer;
		}
	}
	return nResult;
}
#endif

INT KNpcAI::GetNpcNumber(INT nRelation)
{
	INT nRangeX = Npc[m_nIndex].m_VisionRadius;
	INT	nRangeY = nRangeX;
	INT	nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	INT	nRegion = Npc[m_nIndex].m_RegionIndex;
	INT	nMapX = Npc[m_nIndex].m_MapX;
	INT	nMapY = Npc[m_nIndex].m_MapY;
	INT	nRet = 0;
	INT	nRMx, nRMy, nSearchRegion;

	nRangeX = nRangeX / SubWorld[nSubWorld].m_nCellWidth;
	nRangeY = nRangeY / SubWorld[nSubWorld].m_nCellHeight;

	// 检查视野范围内的格子里的NPC
	for (INT i = -nRangeX; i < nRangeX; i++)
	{
		for (INT j = -nRangeY; j < nRangeY; j++)
		{
			// 去掉边角几个格子，保证视野是椭圆形
			if ((i * i + j * j) > nRangeX * nRangeX)
				continue;

			// 确定目标格子实际的REGION和坐标确定
			nRMx = nMapX + i;
			nRMy = nMapY + j;
			nSearchRegion = nRegion;
			if (nRMx < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
				nRMx += SubWorld[nSubWorld].m_nRegionWidth;
			}
			else if (nRMx >= SubWorld[nSubWorld].m_nRegionWidth)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
				nRMx -= SubWorld[nSubWorld].m_nRegionWidth;
			}
			if (nSearchRegion == -1)
				continue;
			if (nRMy < 0)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
				nRMy += SubWorld[nSubWorld].m_nRegionHeight;
			}
			else if (nRMy >= SubWorld[nSubWorld].m_nRegionHeight)
			{
				nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
				nRMy -= SubWorld[nSubWorld].m_nRegionHeight;
			}
			if (nSearchRegion == -1)
				continue;
			// 从REGION的NPC列表中查找满足条件的NPC			
			INT nNpcIdx = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nIndex, nRelation);
			if (nNpcIdx > 0)
				nRet++;
		}
	}
	return nRet;
}

void KNpcAI::KeepAttackRange(INT nEnemy, INT nRange)
{
	INT nX1, nY1, nX2, nY2, nDir, nWantX, nWantY;

	Npc[m_nIndex].GetMpsPos(&nX1, &nY1);
	Npc[nEnemy].GetMpsPos(&nX2, &nY2);
	nDir = g_GetDirIndex(nX1, nY1, nX2, nY2);

	nWantX = nX2 - ((nRange * g_DirCos(nDir, 64)) >> 10);
	nWantY = nY2 - ((nRange * g_DirSin(nDir, 64)) >> 10);

	Npc[m_nIndex].SendCommand(do_walk, nWantX, nWantY);
}

void KNpcAI::FollowAttack(INT i)
{
	if (CheckNpc(i))
		return;

	if (Npc[i].m_RegionIndex < 0)
		return;

	INT distance = NpcSet.GetDistance(m_nIndex, i);
#define	MINI_ATTACK_RANGE	32

	if (distance <= MINI_ATTACK_RANGE)
	{
		KeepAttackRange(i, MINI_ATTACK_RANGE);
		return;
	}
	// Attack Enemy
	if (distance <= Npc[m_nIndex].m_CurrentAttackRadius && InEyeshot(i))
	{
		ISkill* pISkill = g_SkillManager.GetSkill(Npc[m_nIndex].m_ActiveSkillID, 1);
		if (!pISkill)
			return;
		if (pISkill->IsAura())
			return;
		if (pISkill->GetSkillStyle() == SKILL_SS_Missles && (pISkill->IsTargetAlly() || pISkill->IsTargetSelf()))
		{
			if (m_IgnoreCornerLag)
				m_IgnoreCornerLag++;

			if (m_IgnoreCornerLag >= 5)
			{
				INT nDesX, nDesY;
				Npc[m_nIndex].GetMpsPos(&nDesX, &nDesY);
				Npc[m_nIndex].SendCommand(do_walk, nDesX + g_Random(100), nDesY + g_Random(100));

				//if (m_IgnoreCornerLag == 15)
				m_IgnoreCornerLag = 1;

				return;
			}

			INT nX;
			INT nY;
			Npc[m_nIndex].GetMpsPos(&nX, &nY);
			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, nX, nY);
			return;
		}
		Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, i);
		return;
	}

	// Move to Enemy
	INT x, y;
	Npc[i].GetMpsPos(&x, &y);

	Npc[m_nIndex].SendCommand(do_walk, x, y);
}

BOOL KNpcAI::InEyeshot(INT nIdx)
{
	INT distance = NpcSet.GetDistance(nIdx, m_nIndex);
	return (Npc[m_nIndex].m_VisionRadius > distance);
}

void KNpcAI::CommonAction()
{
	// 如果是对话类的NPC，就原地不动
	if (Npc[m_nIndex].m_Kind == kind_dialoger)
	{
		if (Npc[m_nIndex].m_Doing != do_stand)
			Npc[m_nIndex].SendCommand(do_stand);
		return;
	}
	INT	nOffX, nOffY;
	if (g_RandPercent(80))
	{
		nOffX = 0;
		nOffY = 0;
	}
	else
	{

		nOffX = g_Random(Npc[m_nIndex].m_CurrentActiveRadius / 2);
		nOffY = g_Random(Npc[m_nIndex].m_CurrentActiveRadius / 2);
		if (nOffX & 1)
		{
			nOffX = -nOffX;
		}
		if (nOffY & 1)
		{
			nOffY = -nOffY;
		}
	}
	Npc[m_nIndex].SendCommand(do_walk, Npc[m_nIndex].m_OriginX + nOffX, Npc[m_nIndex].m_OriginY + nOffY);
}

BOOL KNpcAI::KeepActiveRange()
{
	INT x, y;

	Npc[m_nIndex].GetMpsPos(&x, &y);
	INT	nRange = g_GetDistance(Npc[m_nIndex].m_OriginX, Npc[m_nIndex].m_OriginY, x, y);

	// 发现超出活动范围，把当前活动范围缩小，避免在活动范围边缘来回晃。
	if (Npc[m_nIndex].m_ActiveRadius < nRange)
	{
		Npc[m_nIndex].m_CurrentActiveRadius = Npc[m_nIndex].m_ActiveRadius / 2;
	}

	// 发现超出当前活动范围，往回走
	if (Npc[m_nIndex].m_CurrentActiveRadius < nRange)
	{
		Npc[m_nIndex].SendCommand(do_walk, Npc[m_nIndex].m_OriginX, Npc[m_nIndex].m_OriginY);
		return TRUE;
	}
	else	// 在当前活动范围内，恢复当前活动范围大小。
	{
		Npc[m_nIndex].m_CurrentActiveRadius = Npc[m_nIndex].m_ActiveRadius;
		return FALSE;
	}
}

#ifndef _SERVER
// 15/16 AiMode NPC的逃逸动作
INT KNpcAI::DoShowFlee(INT nIdx)
{
	INT nResult = false;
	INT nRetCode = false;

	INT x1, y1, x2, y2;
	INT nDistance = Npc[m_nIndex].m_AiParam[6];

	Npc[m_nIndex].GetMpsPos(&x1, &y1);
	//Npc[nIdx].GetMpsPos(&x2, &y2);
	Npc[m_nIndex].m_Dir = Npc[nIdx].m_Dir;
	nRetCode = GetNpcMoveOffset(Npc[m_nIndex].m_Dir, nDistance, &x2, &y2);
	if (!nRetCode)
		goto Exit0;
	Npc[m_nIndex].m_AiParam[4] = (INT)nDistance / Npc[m_nIndex].m_WalkSpeed;
	Npc[m_nIndex].m_AiParam[5] = 0;
	Npc[m_nIndex].SendCommand(do_walk, x1 + x2, y1 + y2);

	nResult = true;
Exit0:
	return nResult;
}

#endif

// 逃离Npc[nIdx]
void KNpcAI::Flee(INT nIdx)
{
	INT x1, y1, x2, y2;

	Npc[m_nIndex].GetMpsPos(&x1, &y1);
	Npc[nIdx].GetMpsPos(&x2, &y2);

	x1 = x1 * 2 - x2;
	y1 = y1 * 2 - y2;

	Npc[m_nIndex].SendCommand(do_walk, x1, y1);
}
//------------------------------------------------------------------------------
//	功能：普通主动类1
//	m_AiParam[0] 无敌人时候的巡逻概率
//	m_AiParam[1、2、3、4] 四种技能的使用概率，分别对应SkillList里的技能1 2 3 4
//	m_AiParam[5、6] 看见敌人但比较远时，待机、巡逻的概率
//------------------------------------------------------------------------------
void	KNpcAI::ProcessAIType01()
{
	INT* pAIParam = Npc[m_nIndex].m_AiParam;
	// 是否已超过活动半径
	if (KeepActiveRange())
		return;

	INT nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// 如果原本没有锁定敌人或者这个敌人跑太远，重新锁定敌人
	if (nEnemyIdx <= 0 || Npc[nEnemyIdx].m_dwID <= 0 || !InEyeshot(nEnemyIdx))
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	// 周围没有敌人，一定概率待机/巡逻
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:巡逻概率
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// 巡逻
			CommonAction();
		}
		return;
	}

	// 如果敌人在所有技能攻击范围之外，一定概率选择待机/巡逻/向敌人靠近
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		INT		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[5])	// 待机
			return;
		if (nRand < pAIParam[5] + pAIParam[6])	// 巡逻
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// 向敌人靠近
		return;
	}

	// 敌人在最大技能攻击范围之内，选择一种技能攻击
	INT		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[1])
	{
		if (!Npc[m_nIndex].SetActiveSkill(1))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2] + pAIParam[3])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2] + pAIParam[3] + pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// 待机
	{
		return;
	}

	FollowAttack(nEnemyIdx);
}


//------------------------------------------------------------------------------
//	功能：普通主动类2
//	m_AiParam[0] 无敌人时候的巡逻概率
//	m_AiParam[1] 剩余生命低于这个百分比的时候执行相应处理
//	m_AiParam[2] 在m_AiParam[1]的情况出现的时候是否执行相应处理的概率
//	m_AiParam[3] 在m_AiParam[1]的情况出现并决定要执行相应处理，使用回复技能的概率 对应SkillList里面的技能 1
//	m_AiParam[4、5、6] 三种攻击技能的使用概率，分别对应SkillList里的技能 2 3 4
//	m_AiParam[7、8] 看见敌人但比较远时，待机、巡逻的概率
//------------------------------------------------------------------------------
void	KNpcAI::ProcessAIType02()
{
	INT* pAIParam = Npc[m_nIndex].m_AiParam;
	// 是否已超过活动半径
	if (KeepActiveRange())
		return;

	INT nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// 如果原本没有锁定敌人或者这个敌人跑太远，重新锁定敌人
	if (nEnemyIdx <= 0 || Npc[nEnemyIdx].m_dwID <= 0 || !InEyeshot(nEnemyIdx))
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	// 周围没有敌人，一定概率待机/巡逻
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:巡逻概率
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// 巡逻
			CommonAction();
		}
		return;
	}

	// 检测剩余生命是否符合条件，生命太少一定概率使用补血技能或逃跑
	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[1])
	{
		if (g_RandPercent(pAIParam[2]))	// 是否使用补血技能或逃跑
		{
			if (Npc[m_nIndex].m_AiAddLifeTime < pAIParam[9] && g_RandPercent(pAIParam[3]))	// 使用补血技能
			{
				Npc[m_nIndex].SetActiveSkill(1);
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
				Npc[m_nIndex].m_AiAddLifeTime++;
				return;
			}
			else	// 逃跑
			{
				Flee(nEnemyIdx);
				return;
			}
		}
	}

	// 如果敌人在所有技能攻击范围之外，一定概率选择待机/巡逻/向敌人靠近
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		INT		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[7])	// 待机
			return;
		if (nRand < pAIParam[7] + pAIParam[8])	// 巡逻
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// 向敌人靠近
		return;
	}

	// 敌人在最大技能攻击范围之内，选择一种技能攻击
	INT		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5] + pAIParam[6])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// 待机
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

//------------------------------------------------------------------------------
//	功能：普通主动类3
//	m_AiParam[0] 无敌人时候的巡逻概率
//	m_AiParam[1] 剩余生命低于这个百分比的时候执行相应处理
//	m_AiParam[2] 在m_AiParam[1]的情况出现的时候是否执行相应处理的概率
//	m_AiParam[3] 在m_AiParam[1]的情况出现并决定要执行相应处理，使用攻击技能的概率 对应SkillList里面的技能 1
//	m_AiParam[4、5、6] 三种攻击技能的使用概率，分别对应SkillList里的技能 2 3 4
//	m_AiParam[7、8] 看见敌人但比较远时，待机、巡逻的概率
//------------------------------------------------------------------------------
void	KNpcAI::ProcessAIType03()
{
	INT* pAIParam = Npc[m_nIndex].m_AiParam;
	// 是否已超过活动半径
	if (KeepActiveRange())
		return;

	INT nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// 如果原本没有锁定敌人或者这个敌人跑太远，重新锁定敌人
	if (nEnemyIdx <= 0 || Npc[nEnemyIdx].m_dwID <= 0 || !InEyeshot(nEnemyIdx))
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	// 周围没有敌人，一定概率待机/巡逻
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:巡逻概率
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// 巡逻
			CommonAction();
		}
		return;
	}

	// 检测剩余生命是否符合条件，生命太少一定概率使用攻击技能或逃跑
	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[1])
	{
		if (g_RandPercent(pAIParam[2]))	// 是否使用攻击技能或逃跑
		{
			if (g_RandPercent(pAIParam[3]))	// 使用攻击技能
			{
				Npc[m_nIndex].SetActiveSkill(1);
				FollowAttack(nEnemyIdx);
				return;
			}
			else	// 逃跑
			{
				Flee(nEnemyIdx);
				return;
			}
		}
	}

	// 如果敌人在所有技能攻击范围之外，一定概率选择待机/巡逻/向敌人靠近
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		INT		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[7])	// 待机
			return;
		if (nRand < pAIParam[7] + pAIParam[8])	// 巡逻
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// 向敌人靠近
		return;
	}

	// 敌人在最大技能攻击范围之内，选择一种技能攻击
	INT		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < (pAIParam[4] + pAIParam[5]))
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < (pAIParam[4] + pAIParam[5] + pAIParam[6]))
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// 待机
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

//------------------------------------------------------------------------------
//	功能：普通被动类1
//	m_AiParam[0] 无敌人时候的巡逻概率
//	m_AiParam[1、2、3、4] 四种攻击技能的使用概率，分别对应SkillList里的技能 1 2 3 4
//	m_AiParam[5、6] 看见敌人但比较远时，待机、巡逻的概率
//------------------------------------------------------------------------------
void	KNpcAI::ProcessAIType04()
{
	INT* pAIParam = Npc[m_nIndex].m_AiParam;

	INT nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// 是否受到攻击，否，一定概率选择待机/巡逻
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:巡逻概率
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// 巡逻
			CommonAction();
		}
		return;
	}

	// 是否已超过活动半径
	if (KeepActiveRange())
		return;

	// 如果敌人在所有技能攻击范围之外，一定概率选择待机/巡逻/向敌人靠近
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		INT		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[5])	// 待机
			return;
		if (nRand < pAIParam[5] + pAIParam[6])	// 巡逻
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// 向敌人靠近
		return;
	}

	// 敌人在最大技能攻击范围之内，选择一种技能攻击
	INT		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[1])
	{
		if (!Npc[m_nIndex].SetActiveSkill(1))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2] + pAIParam[3])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[1] + pAIParam[2] + pAIParam[3] + pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// 待机
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

//------------------------------------------------------------------------------
//	功能：普通被动类2
//	m_AiParam[0] 无敌人时候的巡逻概率
//	m_AiParam[1] 剩余生命低于这个百分比的时候执行相应处理
//	m_AiParam[2] 在m_AiParam[1]的情况出现的时候是否执行相应处理的概率
//	m_AiParam[3] 在m_AiParam[1]的情况出现并决定要执行相应处理，使用回复技能的概率 对应SkillList里面的技能 1
//	m_AiParam[4、5、6] 三种攻击技能的使用概率，分别对应SkillList里的技能 2 3 4
//	m_AiParam[7、8] 看见敌人但比较远时，待机、巡逻的概率
//------------------------------------------------------------------------------
void	KNpcAI::ProcessAIType05()
{
	INT* pAIParam = Npc[m_nIndex].m_AiParam;

	INT nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// 是否受到攻击，否，一定概率选择待机/巡逻
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:巡逻概率
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// 巡逻
			CommonAction();
		}
		return;
	}

	// 是否已超过活动半径
	if (KeepActiveRange())
		return;

	// 检测剩余生命是否符合条件，生命太少一定概率使用补血技能或逃跑
	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[1])
	{
		if (g_RandPercent(pAIParam[2]))	// 是否使用补血技能或逃跑
		{
			if (Npc[m_nIndex].m_AiAddLifeTime < pAIParam[9] && g_RandPercent(pAIParam[3]))	// 使用补血技能
			{
				Npc[m_nIndex].m_AiAddLifeTime++;
				Npc[m_nIndex].SetActiveSkill(1);
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
				return;
			}
			else	// 逃跑
			{
				Flee(nEnemyIdx);
				return;
			}
		}
	}

	// 如果敌人在所有技能攻击范围之外，一定概率选择待机/巡逻/向敌人靠近
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		INT		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[7])	// 待机
			return;
		if (nRand < pAIParam[7] + pAIParam[8])	// 巡逻
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// 向敌人靠近
		return;
	}

	// 敌人在最大技能攻击范围之内，选择一种技能攻击
	INT		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5] + pAIParam[6])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// 待机
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

//------------------------------------------------------------------------------
//	功能：普通被动类3
//	m_AiParam[0] 无敌人时候的巡逻概率
//	m_AiParam[1] 剩余生命低于这个百分比的时候执行相应处理
//	m_AiParam[2] 在m_AiParam[1]的情况出现的时候是否执行相应处理的概率
//	m_AiParam[3] 在m_AiParam[1]的情况出现并决定要执行相应处理，使用攻击技能的概率 对应SkillList里面的技能 1
//	m_AiParam[4、5、6] 三种攻击技能的使用概率，分别对应SkillList里的技能 2 3 4
//	m_AiParam[7、8] 看见敌人但比较远时，待机、巡逻的概率
//------------------------------------------------------------------------------
void	KNpcAI::ProcessAIType06()
{
	INT* pAIParam = Npc[m_nIndex].m_AiParam;

	INT nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	// 是否受到攻击，否，一定概率选择待机/巡逻
	if (nEnemyIdx <= 0)
	{
		// pAIParam[0]:巡逻概率
		if (pAIParam[0] > 0 && g_RandPercent(pAIParam[0]))
		{	// 巡逻
			CommonAction();
		}
		return;
	}

	// 是否已超过活动半径
	if (KeepActiveRange())
		return;

	// 检测剩余生命是否符合条件，生命太少一定概率使用攻击技能或逃跑
	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[1])
	{
		if (g_RandPercent(pAIParam[2]))	// 是否使用攻击技能或逃跑
		{
			if (g_RandPercent(pAIParam[3]))	// 使用攻击技能
			{
				Npc[m_nIndex].SetActiveSkill(1);
				FollowAttack(nEnemyIdx);	// 向敌人靠近
				return;
			}
			else	// 逃跑
			{
				Flee(nEnemyIdx);
				return;
			}
		}
	}

	// 如果敌人在所有技能攻击范围之外，一定概率选择待机/巡逻/向敌人靠近
	if (KNpcSet::GetDistanceSquare(m_nIndex, nEnemyIdx) > pAIParam[MAX_AI_PARAM - 1])
	{
		INT		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[7])	// 待机
			return;
		if (nRand < pAIParam[7] + pAIParam[8])	// 巡逻
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);	// 向敌人靠近
		return;
	}

	// 敌人在最大技能攻击范围之内，选择一种技能攻击
	INT		nRand;
	nRand = g_Random(100);
	if (nRand < pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[4] + pAIParam[5] + pAIParam[6])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}
	else	// 待机
	{
		return;
	}
	FollowAttack(nEnemyIdx);
}

/*
// 一般主动型
void KNpcAI::ProcessAIType1()
{
	INT *pAIParam = Npc[m_nIndex].m_AiParam;
	// 是否已超过活动半径
	if (KeepActiveRange())
		return;

	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Npc[m_nIndex].SetActiveSkill(1);
			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
			return;
		}
	}

	INT nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;

	if (nEnemyIdx <= 0 || Npc[nEnemyIdx].m_dwID <= 0 || !InEyeshot(nEnemyIdx) )
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}



	if (nEnemyIdx > 0)
	{
		INT		nRand;
		nRand = g_Random(100);
		if (nRand < pAIParam[2])
		{
			if (!Npc[m_nIndex].SetActiveSkill(2))
			{
				CommonAction();
				return;
			}
		}
		else if (nRand < pAIParam[2] + pAIParam[3])
		{
			if (!Npc[m_nIndex].SetActiveSkill(3))
			{
				CommonAction();
				return;
			}
		}
		else if (nRand < pAIParam[2] + pAIParam[3] + pAIParam[4])
		{
			if (!Npc[m_nIndex].SetActiveSkill(4))
			{
				CommonAction();
				return;
			}
		}

//		if (g_RandPercent(pAIParam[2]))
//		{
//			Npc[m_nIndex].SetActiveSkill(2);
//		}
//		else if (g_RandPercent(pAIParam[3]))
//		{
//			Npc[m_nIndex].SetActiveSkill(3);
//		}
//		else if (g_RandPercent(pAIParam[4]))
//		{
//			Npc[m_nIndex].SetActiveSkill(4);
//		}
		else
		{
			CommonAction();
			return;
		}
		FollowAttack(nEnemyIdx);
		return;
	}
	CommonAction();
}
*/

/*
// 一般被动型
void KNpcAI::ProcessAIType2()
{
	INT *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Npc[m_nIndex].SetActiveSkill(1);
			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
			return;
		}
	}

	INT nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;
	if (nEnemyIdx <= 0 || !InEyeshot(nEnemyIdx))
		return;

	INT		nRand;
	nRand = g_Random(100);

	if (nRand < pAIParam[2])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[2] + pAIParam[3])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[2] + pAIParam[3] + pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(4))
		{
			CommonAction();
			return;
		}
	}

//	if (g_RandPercent(pAIParam[2]))
//	{
//		Npc[m_nIndex].SetActiveSkill(2);
//	}
//	else if (g_RandPercent(pAIParam[3]))
//	{
//		Npc[m_nIndex].SetActiveSkill(3);
//	}
//	else if (g_RandPercent(pAIParam[4]))
//	{
//		Npc[m_nIndex].SetActiveSkill(4);
//	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(nEnemyIdx);

	return;
}
*/

/*
// 一般逃跑型
void KNpcAI::ProcessAIType3()
{
	INT* pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	INT	nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;

	if (nEnemyIdx <= 0 || !InEyeshot(nEnemyIdx))
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	if (nEnemyIdx <= 0)
	{
		CommonAction();
		return;
	}

	if (Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Flee(nEnemyIdx);
			return;
		}
	}

	INT		nRand;
	nRand = g_Random(100);

	if (nRand < pAIParam[2])
	{
		if (!Npc[m_nIndex].SetActiveSkill(1))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[2] + pAIParam[3])
	{
		if (!Npc[m_nIndex].SetActiveSkill(2))
		{
			CommonAction();
			return;
		}
	}
	else if (nRand < pAIParam[2] + pAIParam[3] + pAIParam[4])
	{
		if (!Npc[m_nIndex].SetActiveSkill(3))
		{
			CommonAction();
			return;
		}
	}

//	if (g_RandPercent(pAIParam[2]))
///	{
//		Npc[m_nIndex].SetActiveSkill(1);
//	}
//	else if (g_RandPercent(pAIParam[3]))
//	{
//		Npc[m_nIndex].SetActiveSkill(2);
//	}
//	else if (g_RandPercent(pAIParam[4]))
//	{
//		Npc[m_nIndex].SetActiveSkill(3);
//	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(nEnemyIdx);
	return;
}
*/

/*
// 逃跑加强型
void KNpcAI::ProcessAIType4()
{
	INT*	pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	INT	nEnemyIdx = Npc[m_nIndex].m_nPeopleIdx;

	if (nEnemyIdx <= 0 || !InEyeshot(nEnemyIdx))
	{
		nEnemyIdx = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	}

	if (nEnemyIdx <= 0)
	{
		CommonAction();
		return;
	}

	INT nLifePercent = Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax;
	if (nLifePercent < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Flee(nEnemyIdx);
			return;
		}
	}
	if (nLifePercent < pAIParam[2])
	{
		if (g_RandPercent(pAIParam[3]))
		{
			Npc[m_nIndex].SetActiveSkill(1);
			Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, m_nIndex);
			return;
		}
	}

	if (g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (g_RandPercent(pAIParam[5]))
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(nEnemyIdx);
	return;
}
*/

/*
//	人多就跑型
void KNpcAI::ProcessAIType5()
{
	INT *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	INT i = Npc[m_nIndex].m_nPeopleIdx;

	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	INT nEnemyNumber = GetNpcNumber(relation_enemy);
	if (nEnemyNumber > pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Flee(i);
			return;
		}
	}

	if (g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nEnemyNumber <= pAIParam[3] && g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else
	{
		CommonAction();
		return;
	}

	FollowAttack(i);
	return;
}
*/

/*
//	成群结队型
void KNpcAI::ProcessAIType6()
{
	INT *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	INT i = Npc[m_nIndex].m_nPeopleIdx;
	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	INT nAllyNumber = GetNpcNumber(relation_none);
	if (nAllyNumber <= pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			Flee(i);
			return;
		}
	}

	if (g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nAllyNumber > pAIParam[3] && g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else
	{
		CommonAction();
		return;
	}

	FollowAttack(i);
	return;
}
*/

/*
// 挨打聚堆型
void KNpcAI::ProcessAIType7()
{
	INT *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	INT i = Npc[m_nIndex].m_nPeopleIdx;
	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	INT j = GetNearestNpc(relation_ally);

	if (j && Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax < pAIParam[0])
	{
		if (g_RandPercent(pAIParam[1]))
		{
			INT x, y;
			Npc[j].GetMpsPos(&x, &y);
			Npc[m_nIndex].SendCommand(do_walk, x, y);
			return;
		}
	}

	if (g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (g_RandPercent(pAIParam[3]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(i);
	return;
}
*/

/*
//	主动送死型
void KNpcAI::ProcessAIType8()
{
	INT *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	INT i = Npc[m_nIndex].m_nPeopleIdx;

	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	if (g_RandPercent(pAIParam[0]))
	{
		INT x, y;

		Npc[i].GetMpsPos(&x, &y);
		Npc[m_nIndex].SendCommand(do_walk, x, y);
	}
	else if (g_RandPercent(pAIParam[1]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (g_RandPercent(pAIParam[3]))
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(i);
	return;
}
*/

/*
//	越战越勇型
void KNpcAI::ProcessAIType9()
{
	INT *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	INT i = Npc[m_nIndex].m_nPeopleIdx;

	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	INT nLifePercent = Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax;

	if (g_RandPercent(pAIParam[0]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nLifePercent < pAIParam[1] && g_RandPercent(pAIParam[2]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (nLifePercent < pAIParam[3] && g_RandPercent(pAIParam[4]))
	{
		Npc[m_nIndex].SetActiveSkill(3);
	}
	else
	{
		CommonAction();
		return;
	}
	FollowAttack(i);
	return;
}
*/

/*
//	逃跑不掉型
void KNpcAI::ProcessAIType10()
{
	INT *pAIParam = Npc[m_nIndex].m_AiParam;

	if (KeepActiveRange())
		return;

	INT i = Npc[m_nIndex].m_nPeopleIdx;

	if (!i || !InEyeshot(i))
	{
		i = GetNearestNpc(relation_enemy);
		Npc[m_nIndex].m_nPeopleIdx = i;
	}

	if (!i)
	{
		CommonAction();
		return;
	}

	INT nLifePercent = Npc[m_nIndex].m_CurrentLife * 100 / Npc[m_nIndex].m_CurrentLifeMax;

	if (nLifePercent < pAIParam[0] && g_RandPercent(pAIParam[1]))
	{
		Npc[m_nIndex].SetActiveSkill(1);
	}
	else if (nLifePercent < pAIParam[2] && g_RandPercent(pAIParam[3]))
	{
		Npc[m_nIndex].SetActiveSkill(2);
	}
	else if (nLifePercent < pAIParam[4] && g_RandPercent(pAIParam[5]))
	{
		Flee(i);
		return;
	}
	else
	{
		CommonAction();
		return;
	}

	FollowAttack(i);
	return;
}
*/
