#include "KCore.h"

#include "KMissle.h"
#include "KMissleSet.h"
#include "KSubWorld.h"

CORE_API KMissleSet MissleSet;

void KMissleSet::Init()
{

	m_FreeIdx.Init(MAX_MISSLE);
	m_UseIdx.Init(MAX_MISSLE);

	// 开始时所有的数组元素都为空
	for (INT i = MAX_MISSLE - 1; i > 0; i--)
	{
		m_FreeIdx.Insert(i);
		Missle[i].m_Node.m_nIndex = i;
	}
}

INT KMissleSet::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

INT KMissleSet::Add(INT nSubWorldId, INT nRegionID, INT nMapX, INT nMapY, INT nOffsetX, INT nOffsetY)
{

	if (nRegionID < 0) return -1;
	INT nFreeIndex = FindFree();
	if (nFreeIndex <= 0) return -1;

	Missle[nFreeIndex].m_nMissleId = nFreeIndex;
	Missle[nFreeIndex].m_nRegionId = nRegionID;
	Missle[nFreeIndex].m_nCurrentMapX = nMapX;
	Missle[nFreeIndex].m_nCurrentMapY = nMapY;
	Missle[nFreeIndex].m_nXOffset = nOffsetX;
	Missle[nFreeIndex].m_nYOffset = nOffsetY;
	Missle[nFreeIndex].m_nSubWorldId = nSubWorldId;
	Missle[nFreeIndex].m_nLastDoCollisionIdx = 0;

	m_FreeIdx.Remove(nFreeIndex);
	m_UseIdx.Insert(nFreeIndex);

	return nFreeIndex;
}

/*!*****************************************************************************
// Function		: KMissleSet::Add
// Purpose		:
// Return		: INT
// Argumant		: INT nSubWorldId
// Argumant		: INT nPX
// Argumant		: INT nPY
// Comments		:
// Author		: RomanDou
*****************************************************************************/
INT KMissleSet::Add(INT nSubWorldId, INT nPX, INT nPY)
{
	if (nSubWorldId < 0) return -1;

	INT nFreeIndex = FindFree();
	if (nFreeIndex <= 0)
	{
		printf("MissleSet Have Full!!!, It Maybe A Error!");
		return -1;
	}

	SubWorld[nSubWorldId].Mps2Map(nPX, nPY, &Missle[nFreeIndex].m_nRegionId, &Missle[nFreeIndex].m_nCurrentMapX, &Missle[nFreeIndex].m_nCurrentMapY, &Missle[nFreeIndex].m_nXOffset, &Missle[nFreeIndex].m_nYOffset);

	if (Missle[nFreeIndex].m_nRegionId < 0) return -1;

	Missle[nFreeIndex].m_nMissleId = nFreeIndex;
	Missle[nFreeIndex].m_nCurrentMapZ = Missle[nFreeIndex].m_nHeight;
	Missle[nFreeIndex].m_nSubWorldId = nSubWorldId;
	Missle[nFreeIndex].m_bRemoving = FALSE;
	Missle[nFreeIndex].m_nLastDoCollisionIdx = 0;
	SubWorld[nSubWorldId].m_Region[Missle[nFreeIndex].m_nRegionId].AddMissle(nFreeIndex);//m_WorldMessage.Send(GWM_MISSLE_ADD, Missle[nFreeIndex].m_nRegionId, nFreeIndex );
	//SubWorld[nSubWorldId].m_Region[Missle[nFreeIndex].m_nRegionId].AddRef(Missle[nFreeIndex].m_nCurrentMapX, Missle[nFreeIndex].m_nCurrentMapY, obj_missle);
	m_FreeIdx.Remove(nFreeIndex);
	m_UseIdx.Insert(nFreeIndex);

	return nFreeIndex;
}

INT	KMissleSet::CreateMissile(INT nSkillId, INT nMissleId, INT nLauncher, INT nTargetId, INT nSubWorldId, INT nPX, INT nPY, INT nDir)
{
	INT nMissleIndex = -1;

	if (nMissleIndex = Add(nSubWorldId, nPX, nPY) <= 0)
		return -1;
	return nMissleIndex;
}

/*!*****************************************************************************
// Function		: KMissleSet::Activate
// Purpose		:
// Return		: INT
// Comments		:
// Author		: RomanDou
*****************************************************************************/
INT KMissleSet::Activate()
{
	for (INT i = 0; i < MAX_MISSLE; i++)
	{
		Missle[i].Activate();
	}
	return 1;
}

void KMissleSet::Remove(INT nIndex)
{
	if (nIndex <= 0) return;

	if (Missle[nIndex].m_nMissleId < 0) return;
	Missle[nIndex].Release();

	m_FreeIdx.Insert(nIndex);
	m_UseIdx.Remove(nIndex);
}

void KMissleSet::Draw()
{
	for (INT i = 0; i < MAX_MISSLE; i++)
	{
#ifndef _SERVER
		Missle[i].Paint();
#endif
	}

}

INT KMissleSet::GetCount()
{
	INT nCount = 0;
	for (INT i = 0; i < MAX_MISSLE; i++)
	{
		if (Missle[i].m_nMissleId > 0)
			nCount++;

	}
	return nCount;
}

void KMissleSet::ClearMissles()
{
	INT nUsedIndex = m_UseIdx.GetNext(0);

	while (nUsedIndex != 0)
	{

		Remove(nUsedIndex);
		nUsedIndex = m_UseIdx.GetNext(0);
	}
}
