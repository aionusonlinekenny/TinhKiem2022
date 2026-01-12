#ifndef KMISSION_H
#define KMISSION_H

#ifdef _SERVER
#include "KCore.h"
#include "KEngine.h"
#include "KLinkArrayTemplate.h"
#include "KTaskFuns.h"
#include "KPlayerDef.h"

#include "KMissionArray.h"
#include "KPlayerChat.h"
#include "KSortScript.h"

//Mission
//每个Mission有自已的独立的全局变量
//为每一个Mission记录玩家列表的统一的机制
//包括最大人数，等等都要有
//加个Chanel，有利于消息发送
//有功能可以编列所有的参加者id
extern INT g_PlayerTimerCallBackFun(void* pOwner, char* szScriptFileName, DWORD dwTimerTaskId);
extern INT g_MissionTimerCallBackFun(void* pOwner, char* szScriptFileName, DWORD dwTimerTaskId);
#define MAX_TIMER_PERMISSION 4

typedef struct
{
	ULONG			m_ulPlayerIndex;
	ULONG			m_ulPlayerID;
	unsigned char	m_ucPlayerGroup;
	ULONG			m_ulJoinTime;
	char			szName[32];
	INT				m_nParam[MAX_MISSION_PARAM];
}TMissionPlayerInfo;

typedef struct
{
	ULONG m_ulNpcIndex;
	ULONG m_ulNpcID;
	ULONG m_ucNpcGroup;
	ULONG m_ulJoinTime;
}TMissionNpcInfo;
//管理玩家集合的模板类

template<class T, ULONG ulSize>
class _KMissionPlayerArray :public KLinkArrayTemplate<T, ulSize>
{
public:
	ULONG FindSame(T* pT)
	{
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if (pT->m_ulPlayerID == m_Data[nIdx].m_ulPlayerID)
			{
				m_Data[nIdx].m_ulPlayerIndex = pT->m_ulPlayerIndex;
				return nIdx;
			}
		}
		return 0;
	};

	INT GetParam(ULONG ulIndex, INT nParam) const
	{
		if (ulIndex >= ulSize)
			return 0;
		return m_Data[ulIndex].m_nParam[nParam];
	};

	BOOL SetParam(ULONG ulIndex, INT nParam, INT nValue);
};

template<class T, ULONG ulSize>
class _KMissionNpcArray :public KLinkArrayTemplate<T, ulSize>
{
public:
	ULONG FindSame(T* pT)
	{
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;


			if (pT->m_ulNpcID == m_Data[nIdx].m_ulNpcID)
			{
				m_Data[nIdx].m_ulNpcIndex = pT->m_ulNpcIndex;
				return nIdx;
			}
		}
		return 0;
	};
};

typedef _KMissionPlayerArray<TMissionPlayerInfo, MAX_PLAYER> KMissionPlayerArray;
typedef _KMissionNpcArray<TMissionNpcInfo, MAX_NPC> KMissionNpcArray;

//管理Mission时间触发器集合的模板类
typedef KTimerFunArray <KTimerTaskFun, MAX_TIMER_PERMISSION, g_MissionTimerCallBackFun> KMissionTimerArray;

class KMission
{
	char		m_MissionValue[MAX_MISSIONARRAY_VALUE_COUNT][16];
	ULONG		m_ulMissionId;
	bool		m_bMissionLadder;
	char		m_szMissionName[64];
	INT			m_nLadderParam;
	INT			m_nGlbLadderParam[MAX_GLBMISSION_PARAM];
	void* m_pOwner;
public:
	KMissionTimerArray	m_cTimerTaskSet;
	KMission()
	{
		m_pOwner = NULL;
		for (INT i = 1; i < MAX_TIMER_PERMISSION; i++)
		{
			KTimerTaskFun* pTimer = (KTimerTaskFun*)m_cTimerTaskSet.GetData(i);
			if (pTimer)
				pTimer->SetOwner(this);
		}
		memset(m_MissionValue, 0, sizeof(m_MissionValue));
		m_ulMissionId = 0;
		m_bMissionLadder = false;
		m_szMissionName[0] = 0;
		m_nLadderParam = 0;
		memset(m_nGlbLadderParam, 0, sizeof(m_nGlbLadderParam));
	};
	BOOL	Activate();

	void SetOwner(void* pOwner)
	{
		m_pOwner = pOwner;
	};

	void* GetOwner()
	{
		return m_pOwner;
	};

	void	Init()
	{
		memset(m_MissionValue, 0, sizeof(m_MissionValue));
		m_ulMissionId = 0;
		m_bMissionLadder = false;
		memset(m_szMissionName, 0, sizeof(m_szMissionName));
		m_nLadderParam = 0;
		memset(m_nGlbLadderParam, 0, sizeof(m_nGlbLadderParam));
	};

	BOOL SetMissionId(ULONG ulMissionId)
	{
		m_ulMissionId = ulMissionId;
		return TRUE;
	}

	void SetMissionLadder(const char* szName, INT nLadderParam, INT* nGlbParam)
	{
		m_bMissionLadder = true;
		memcpy(m_szMissionName, szName, strlen(szName));
		m_nLadderParam = nLadderParam;
		for (INT i = 0; i < MAX_GLBMISSION_PARAM; i++)
			m_nGlbLadderParam[i] = nGlbParam[i];
	}

	ULONG GetMissionId()
	{
		return m_ulMissionId;
	}

	const char* GetMissionName()
	{
		return m_szMissionName;
	}

	INT GetMissionLadderParam()
	{
		return m_nLadderParam;
	}

	TMissionLadderInfo* GetMin(TMissionLadderInfo* const aMSLDList, const INT n);

	void ListSort(TMissionLadderInfo* const aMSLDList, const INT n);

	void DataCopy(TMissionLadderInfo* Desc, TMissionPlayerInfo* Source);
	ULONG Msg2Group(const char* strMsg, unsigned char ucGroup, INT nChannelID)
	{
		if (!strMsg || !strMsg[0])
			return 0;
		INT nCount = 0;
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_MissionPlayer.m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if ((m_MissionPlayer.m_Data[nIdx].m_ucPlayerGroup == ucGroup) &&
				(m_MissionPlayer.m_Data[nIdx].m_nParam[MISSION_PARAM_AVAILABLE] == MISSION_AVAILABLE_VALUE))
			{
				KPlayerChat::SendSystemInfo(1, m_MissionPlayer.m_Data[nIdx].m_ulPlayerIndex, MESSAGE_BROADCAST_ANNOUCE_HEAD, (char*)strMsg, strlen(strMsg), nChannelID);
				nCount++;
			}
		}
		return nCount;
	}

	ULONG Msg2All(const char* strMsg, INT nChannelID)
	{
		if (!strMsg || !strMsg[0])
			return 0;
		INT nCount = 0;
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_MissionPlayer.m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if (m_MissionPlayer.m_Data[nIdx].m_nParam[MISSION_PARAM_AVAILABLE] == MISSION_AVAILABLE_VALUE)
			{
				KPlayerChat::SendSystemInfo(1, m_MissionPlayer.m_Data[nIdx].m_ulPlayerIndex, MESSAGE_BROADCAST_ANNOUCE_HEAD, (char*)strMsg, strlen(strMsg), nChannelID);
				nCount++;
			}
		}
		return nCount;
	}

	ULONG GetNextPlayer(ULONG ulIdx, ULONG& ulPlayerIndex)
	{
		ulPlayerIndex = 0;
		while (1)
		{
			ulIdx = m_MissionPlayer.m_UseIdx.GetNext(ulIdx);
			if (ulIdx == 0)
				return 0;

			if (m_MissionPlayer.m_Data[ulIdx].m_nParam[MISSION_PARAM_AVAILABLE] == MISSION_AVAILABLE_VALUE)
			{
				ulPlayerIndex = m_MissionPlayer.m_Data[ulIdx].m_ulPlayerIndex;
				return ulIdx;
			}
		}
		return 0;
	}

	ULONG GetNextNpc(ULONG ulIdx, ULONG& ulNpcIndex)
	{
		ulNpcIndex = 0;
		while (1)
		{
			ulIdx = m_MissionNpc.m_UseIdx.GetNext(ulIdx);
			if (ulIdx == 0)
				return 0;

			ulNpcIndex = m_MissionNpc.m_Data[ulIdx].m_ulNpcIndex;
			return ulIdx;
		}
		return 0;
	}

	ULONG GetPlayerCount() const
	{
		return m_MissionPlayer.GetUsedCount();
	};

	ULONG GetNpcCount() const
	{
		return m_MissionNpc.GetUsedCount();
	};

	void SetMission(ULONG ulValueId, const char* szValue)
	{
		if (ulValueId >= MAX_MISSIONARRAY_VALUE_COUNT)
			return;
		strcpy(m_MissionValue[ulValueId], szValue);
	};

	ULONG GetGroupPlayerCount(unsigned char ucGroup) const
	{
		ULONG ulGroupPlayerCount = 0;
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_MissionPlayer.m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if (ucGroup >= 0)
			{
				if ((m_MissionPlayer.m_Data[nIdx].m_ucPlayerGroup == ucGroup) && (m_MissionPlayer.m_Data[nIdx].m_nParam[MISSION_PARAM_AVAILABLE] == MISSION_AVAILABLE_VALUE))
					ulGroupPlayerCount++;
			}
		}
		return ulGroupPlayerCount;
	};

	ULONG GetGroupNpcCount(unsigned char ucGroup) const
	{
		ULONG ulGroupNpcCount = 0;
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_MissionNpc.m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if (m_MissionNpc.m_Data[nIdx].m_ucNpcGroup == ucGroup)
				ulGroupNpcCount++;
		}
		return ulGroupNpcCount;
	};

	INT GetMissionValue(ULONG ulValueId) const
	{
		if (ulValueId >= MAX_MISSIONARRAY_VALUE_COUNT)
			return 0;

		INT nResult = 0;
		if (m_MissionValue[ulValueId][0])
			nResult = atoi(m_MissionValue[ulValueId]);
		return nResult;
	};

	const char* GetMissionString(ULONG ulValueId) const
	{
		if (ulValueId >= MAX_MISSIONARRAY_VALUE_COUNT)
			return "";
		return m_MissionValue[ulValueId];
	};
	BOOL	ExecuteScript(char* ScriptFileName, char* szFunName, INT nParam);
	BOOL	ExecuteScript(DWORD dwScriptId, char* szFunName, INT nParam);
	BOOL StartTimer(ULONG ucTimerId, ULONG ulTimerInterval)
	{
		KTimerTaskFun Timer;
		Timer.SetTimer(ulTimerInterval, ucTimerId);
		Timer.SetOwner(this);
		m_cTimerTaskSet.Add(&Timer);
		return TRUE;
	}

	BOOL StopTimer(unsigned char ucTimerId)
	{
		KTimerTaskFun Timer;
		Timer.SetTimer(1, ucTimerId);
		ULONG ulIndex = 0;
		KTimerTaskFun* pTimer = m_cTimerTaskSet.GetData(&Timer);
		if (pTimer)
			pTimer->CloseTimer();

	}

	DWORD GetTimerRestTimer(unsigned char ucTimerId)
	{
		KTimerTaskFun Timer;
		Timer.SetTimer(1, ucTimerId);
		ULONG ulIndex = 0;
		KTimerTaskFun* pTimer = m_cTimerTaskSet.GetData(&Timer);
		if (pTimer)
			return pTimer->GetRestTime();
		return 0;
	}

	BOOL StopMission();

	KMissionPlayerArray m_MissionPlayer;
	KMissionNpcArray m_MissionNpc;
	TMissionLadderInfo m_MissionLadder[MISSION_STATNUM];

	ULONG			AddPlayer(ULONG ulPlayerIndex, ULONG ulPlayerID, unsigned char ucPlayerGroup, INT ulJoinTime = 0);
	ULONG			AddNpc(ULONG ulNpcIndex, ULONG ulNpcID, unsigned char ucNpcGroup, INT ulJoinTime = 0);
	BOOL			RemovePlayer(ULONG ulPlayerIndex, ULONG ulPlayerID);
	BOOL			RemoveNpc(ULONG ulNpcIndex, ULONG ulNpcID = 0);
	void 			SetPlayerParam(ULONG ulIndex, INT nParam, INT nValue);
	ULONG GetMissionPlayer_DataIndex(ULONG ulPlayerIndex, ULONG ulPlayerID)//
	{
		if (ulPlayerIndex >= MAX_PLAYER)
			return 0;
		TMissionPlayerInfo Info;
		Info.m_ulPlayerIndex = ulPlayerIndex;
		Info.m_ulPlayerID = ulPlayerID;
		return m_MissionPlayer.FindSame(&Info);
	}

	ULONG GetMissionPlayer_PlayerIndex(ULONG ulDataIndex)
	{
		if (ulDataIndex > m_MissionPlayer.GetTotalCount())
			return 0;
		return m_MissionPlayer.m_Data[ulDataIndex].m_ulPlayerIndex;
	}

	ULONG GetMissionPlayer_GroupId(ULONG ulDataIndex)
	{
		if (ulDataIndex > m_MissionPlayer.GetTotalCount())
			return 0;
		return m_MissionPlayer.m_Data[ulDataIndex].m_ucPlayerGroup;
	}

	ULONG GetMissionNpc_DataIndex(ULONG ulNpcIndex, ULONG ulNpcID)//
	{
		if (ulNpcIndex >= MAX_NPC)
			return 0;
		TMissionNpcInfo Info;
		Info.m_ulNpcIndex = ulNpcIndex;
		Info.m_ulNpcID = ulNpcID;
		return m_MissionNpc.FindSame(&Info);
	}

	ULONG GetMissionNpc_NpcIndex(ULONG ulDataIndex)
	{
		if (ulDataIndex > m_MissionNpc.GetTotalCount())
			return 0;
		return m_MissionNpc.m_Data[ulDataIndex].m_ulNpcIndex;
	}

	ULONG GetMissionNpc_GroupId(ULONG ulDataIndex)
	{
		if (ulDataIndex > m_MissionNpc.GetTotalCount())
			return 0;
		return m_MissionNpc.m_Data[ulDataIndex].m_ucNpcGroup;
	}
};
#endif
#endif
