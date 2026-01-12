#ifndef KMISSIONARRAY_H
#define KMISSIONARRAY_H

#include "KTaskFuns.h"
#include "KMission.h"

#define MAX_MISSIONARRAY_VALUE_COUNT 10
//管理Mission的集合

template <class T, size_t ulSize>
class KMissionArray : public KLinkArrayTemplate<T, ulSize>
{
	char	m_MissionValue[MAX_MISSIONARRAY_VALUE_COUNT][16];
public:
	BOOL	Activate()
	{
		ULONG ulActiveCount = GetUsedCount();
		if (ulActiveCount == 0)
			return FALSE;

		INT nIdx = 0;
		for (INT i = 0; i < ulActiveCount; i++)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			_ASSERT(nIdx);
			if (!nIdx)
			{
				break;
			}
			m_Data[nIdx].Activate();
		}
		return TRUE;
	};

	void SetMission(ULONG ulValueId, const char* szValue)
	{
		if (ulValueId >= MAX_MISSIONARRAY_VALUE_COUNT)
			return;
		strcpy(m_MissionValue[ulValueId], szValue);
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
	ULONG FindSame(T* pMission)
	{
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if (pMission->GetMissionId() == m_Data[nIdx].GetMissionId())
				return nIdx;
		}
		return 0;
	};
	BOOL RemovePlayer(ULONG ulPlayerIndex, ULONG ulPlayerID)
	{
		ULONG ulActiveCount = GetUsedCount();
		if (ulActiveCount == 0)
			return FALSE;

		INT nIdx = 0;
		for (INT i = 0; i < ulActiveCount; i++)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			_ASSERT(nIdx);
			if (!nIdx)
			{
				break;
			}
			m_Data[nIdx].RemovePlayer(ulPlayerIndex, ulPlayerID);
		}
		return TRUE;
	}
};


//管理定时触发器的集合
template <class T, size_t ulSize, PF_TimerCallBackFun CallFun>
class KTimerFunArray :public KLinkArrayTemplate < T, ulSize>
{
public:
	char m_szScriptFile[MAX_PATH];
	BOOL	Activate()
	{
		ULONG ulActiveCount = GetUsedCount();
		if (ulActiveCount == 0)
			return FALSE;

		INT nIdx = 0;
		for (INT i = 0; i < ulActiveCount; i++)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			//	_ASSERT(nIdx);
			if (!nIdx)
			{
				break;
			}
			m_Data[nIdx].Activate(CallFun);
		}
		return TRUE;
	};

	ULONG FindSame(T* pTask)
	{
		INT nIdx = 0;
		while (1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if (pTask->GetTaskId() == m_Data[nIdx].GetTaskId())
				return nIdx;
		}
		return 0;
	};

};
#endif
