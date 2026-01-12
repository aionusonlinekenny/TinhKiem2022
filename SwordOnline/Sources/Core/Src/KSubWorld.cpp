#include "KCore.h"
#include <math.h>
#include "KRegion.h"
#include "KMath.h"
#include "KNpc.h"
#include "KPlayer.h"
#include "KNpcSet.h"
#include "KObjSet.h"
#include "KPlayerSet.h"
#include "KMissleSet.h"
#ifndef _STANDALONE
#include "crtdbg.h"
#endif
#include "Scene/ObstacleDef.h"
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/Interface/iServer.h"
#endif
#ifndef _SERVER
#include "scene/KScenePlaceC.h"
#endif
#include "KSubWorld.h"

#ifndef TOOLVERSION
KSubWorld	SubWorld[MAX_SUBWORLD];
#else 
CORE_API KSubWorld	SubWorld[MAX_SUBWORLD];
#endif

#define		defLOGIC_CELL_WIDTH			32
#define		defLOGIC_CELL_HEIGHT		32

#ifdef _SERVER
KGlobalMissionArray g_GlobalMissionArray;
#endif

KSubWorld::KSubWorld()
{
	m_Region = NULL;
	m_nRegionWidth = 512 / 32;
	m_nRegionHeight = 1024 / 32;
	m_nCellWidth = defLOGIC_CELL_WIDTH;
	m_nCellHeight = defLOGIC_CELL_HEIGHT;
	m_dwCurrentTime = 0;
	m_SubWorldID = -1;
	m_nWeather = WEATHERID_NOTHING;
	m_nIndex = -1;
	m_nRegionBeginX = 0;
	m_nRegionBeginY = 0;

#ifndef _SERVER
	m_nWorldRegionWidth = 3;
	m_nWorldRegionHeight = 3;
	memset(m_ClientRegionIdx, 0, sizeof(this->m_ClientRegionIdx));
	memset(this->m_szMapPath, 0, sizeof(this->m_szMapPath));
#endif
#ifdef _SERVER
	m_nWorldRegionWidth = 0;
	m_nWorldRegionHeight = 0;
	m_pWeatherMgr = NULL;
	m_szNewWorldScript[0] = 0;
	//MissionArryÖÐ0ËäÈ»ÎÞÐ§£¬µ«»¹ÊÇ¼ÓÉÏÈ¥£¬ÈÝ´í.TotalCount Îª¿Õ¼ä¼õ1£¬ËùÒÔ²»»áÔ½½ç
	for (INT i = 0; i <= m_MissionArray.GetTotalCount(); i++)
	{
		m_MissionArray.m_Data[i].SetOwner(this);
	}
#endif
	m_nTotalRegion = m_nWorldRegionWidth * m_nWorldRegionHeight;

}

KSubWorld::~KSubWorld()
{
	if (m_Region)
	{
		delete[] m_Region;
		m_Region = NULL;
	}

#ifdef _SERVER
	if (m_pWeatherMgr)
	{
		delete m_pWeatherMgr;
		m_pWeatherMgr = NULL;
	}
#endif
}

INT KSubWorld::FindRegion(INT nRegion)
{
	for (INT i = 0; i < m_nTotalRegion; i++)
	{
		if (m_Region[i].m_RegionID == nRegion)
			return i;
	}
	return -1;
}

INT KSubWorld::FindFreeRegion(INT nX, INT nY)
{
	if (nX == 0 && nY == 0)
	{
		for (INT i = 0; i < m_nTotalRegion; i++)
		{
			if (m_Region[i].m_RegionID == -1)
				return i;
		}
	}
	else
	{
		for (INT i = 0; i < m_nTotalRegion; i++)
		{
			if (m_Region[i].m_RegionID == -1)
				return i;
			INT nRegoinX = LOWORD(m_Region[i].m_RegionID);
			INT nRegoinY = HIWORD(m_Region[i].m_RegionID);

			if ((nX - nRegoinX) * (nX - nRegoinX) + (nY - nRegoinY) * (nY - nRegoinY) > 2)	// ²»ÔÚ¸½½ü
				return i;
		}
	}
	return -1;
}

extern INT nActiveRegionCount;

void KSubWorld::Activate()
{
	if (m_SubWorldID == -1)
		return;
	m_dwCurrentTime++;

#ifndef _SERVER
	g_ScenePlace.SetCurrentTime(m_dwCurrentTime);
	NpcSet.ClearActivateFlagOfAllNpc();
#endif

	for (INT i = 0; i < m_nTotalRegion; i++)
	{
		if (m_Region[i].IsActive())
		{
			//			g_DebugLog("[Region]%d Activating", i);
			m_Region[i].Activate();
			nActiveRegionCount++;
		}
	}

#ifdef _SERVER
	KIndexNode* pNode = (KIndexNode*)m_NoneRegionNpcList.GetHead();
	while (pNode)
	{
		Npc[pNode->m_nIndex].Activate();
		pNode = (KIndexNode*)pNode->GetNext();
	}

	if (m_pWeatherMgr)
	{
		INT nWeather = m_pWeatherMgr->Activate();
		if (m_nWeather != nWeather)
		{
			m_nWeather = nWeather;
			SYNC_WEATHER weatherMsg;
			weatherMsg.ProtocolType = s2c_changeweather;
			weatherMsg.WeatherID = (BYTE)m_nWeather;
			BroadCast((const char*)&weatherMsg, sizeof(SYNC_WEATHER));
		}
	}

	m_MissionArray.Activate();
#endif
}

INT KSubWorld::GetDistance(INT nRx1, INT nRy1, INT nRx2, INT nRy2)
{
	return (INT)sqrt(double((nRx1 - nRx2)) * (nRx1 - nRx2) + (nRy1 - nRy2) * (nRy1 - nRy2));
}

void KSubWorld::Map2Mps(INT nR, INT nX, INT nY, INT nDx, INT nDy, INT* nRx, INT* nRy)
{
	/*
	#ifdef TOOLVERION
		*nRx = nX;
		*nRy = nY;
		return;
	#endif
	*/
#ifndef _SERVER
	//	_ASSERT(nR >= 0 && nR < 9);
#endif
	//_ASSERT(nR >= 0);

	if (nR < 0 || nR >= m_nTotalRegion)
	{
		*nRx = 0;
		*nRy = 0;
		return;
	}

	INT x, y;

	x = m_Region[nR].m_nRegionX;
	y = m_Region[nR].m_nRegionY;

	x += nX * m_nCellWidth;
	y += nY * m_nCellHeight;

	x += (nDx >> 10);
	y += (nDy >> 10);

	*nRx = x;
	*nRy = y;
}

void KSubWorld::NewMap2Mps(INT nR, INT nX, INT nY, INT nDx, INT nDy, INT* nRx, INT* nRy)
{
	/*
	#ifdef TOOLVERION
		*nRx = nX;
		*nRy = nY;
		return;
	#endif

	#ifndef _SERVER
	//	_ASSERT(nR >= 0 && nR < 9);
	#endif
		//_ASSERT(nR >= 0);
	*/
	if (nR < 0 || nR >= m_nTotalRegion)
	{
		*nRx = 0;
		*nRy = 0;
		return;
	}

	INT x, y;

	x = m_Region[nR].m_nRegionX;
	y = m_Region[nR].m_nRegionY;

	x += nX * m_nCellWidth;
	y += nY * m_nCellHeight;

	x += (nDx >> 10);
	y += (nDy >> 10);

	*nRx = x;
	*nRy = y;
}

void KSubWorld::Map2Mps(INT nRx, INT nRy, INT nX, INT nY, INT nDx, INT nDy, INT* pnX, INT* pnY)
{
	*pnX = (nRx * REGION_GRID_WIDTH + nX) * defLOGIC_CELL_WIDTH + (nDx >> 10);
	*pnY = (nRy * REGION_GRID_HEIGHT + nY) * defLOGIC_CELL_HEIGHT + (nDy >> 10);
}

void KSubWorld::Mps2Map(INT Rx, INT Ry, INT* nR, INT* nX, INT* nY, INT* nDx, INT* nDy)
{
	if (m_nCellWidth == 0 || m_nCellHeight == 0 || m_nRegionWidth == 0 || m_nRegionHeight == 0)
		return;

	INT x = Rx / (m_nRegionWidth * m_nCellWidth);	// 取商	  16*32 ==512
	INT y = Ry / (m_nRegionHeight * m_nCellHeight); // 取商	  32*32==1024

	/*if (Rx%(m_nRegionWidth * m_nCellWidth)>=(m_nRegionWidth * m_nCellWidth)/2)
		x++;

	if (Ry%(m_nRegionHeight * m_nCellHeight)>=(m_nRegionHeight * m_nCellHeight)/2)
		y++; */

	*nX = 0;
	*nY = 0;
	*nDx = 0;
	*nDy = 0;
#ifdef _SERVER
	// 非法的坐标
	if (x >= m_nWorldRegionWidth + m_nRegionBeginX || y >= m_nWorldRegionHeight + m_nRegionBeginY || x < m_nRegionBeginX || y < m_nRegionBeginY)
	{
		*nR = -1; // 区域无效
		return;
	}
#endif
	// 非法的坐标
#ifdef _SERVER
	* nR = GetRegionIndex(MAKELONG(x, y)); // 区域坐标 取出所在的区域编号
#else
	INT nRegionID = MAKELONG(x, y);
	*nR = FindRegion(nRegionID);
	if (*nR == -1)
		return;
#endif
	if (*nR >= m_nTotalRegion)
	{
		*nR = -1;
		return;
	}
	//////加个障碍物判断/////////////////

	/////////////////////////////////////
	x = Rx - m_Region[*nR].m_nRegionX;
	y = Ry - m_Region[*nR].m_nRegionY;

	*nX = x / m_nCellWidth;	 // 取商
	*nY = y / m_nCellHeight; // 取商

	*nDx = (x - *nX * m_nCellWidth) << 10; // 1024
	*nDy = (y - *nY * m_nCellHeight) << 10;
}

BYTE KSubWorld::TestBarrier(INT nMpsX, INT nMpsY)
{
	if (m_nCellWidth == 0 || m_nCellHeight == 0 || m_nRegionWidth == 0 || m_nRegionHeight == 0)
		return 0xff;

	INT x = nMpsX / (m_nRegionWidth * m_nCellWidth);
	INT y = nMpsY / (m_nRegionHeight * m_nCellHeight);
#ifdef _SERVER
	// 非法的坐标
	if (x >= m_nWorldRegionWidth + m_nRegionBeginX || y >= m_nWorldRegionHeight + m_nRegionBeginY || x < m_nRegionBeginX || y < m_nRegionBeginY)
		return 0xff;
	INT nRegion = GetRegionIndex(MAKELONG(x, y));
#else
	INT nRegion = FindRegion(MAKELONG(x, y));
	if (nRegion == -1)
		return 0xff;
#endif
	if (nRegion >= m_nTotalRegion)
		return 0xff;

	x = nMpsX - m_Region[nRegion].m_nRegionX;
	y = nMpsY - m_Region[nRegion].m_nRegionY;

	INT nCellX = x / m_nCellWidth;
	INT nCellY = y / m_nCellHeight;

	INT nOffX = x - nCellX * m_nCellWidth;
	INT nOffY = y - nCellY * m_nCellHeight;

#ifndef _SERVER
	BYTE bRet = (BYTE)g_ScenePlace.GetObstacleInfo(nMpsX, nMpsY); // 获取障碍的信息
	if (bRet != Obstacle_NULL)
		return bRet;

	return m_Region[nRegion].GetNewBarrier(nCellX, nCellY, nOffX, nOffY, TRUE);
#endif

#ifdef _SERVER
	return m_Region[nRegion].GetBarrier(nCellX, nCellY, nOffX, nOffY);
#endif
}

//Move NPC
BYTE KSubWorld::TestBarrier(INT nRegion, INT nMapX, INT nMapY, INT nDx, INT nDy, INT nChangeX, INT nChangeY)
{
	INT nOldMapX = nMapX;
	INT nOldMapY = nMapY;
	INT nOldRegion = nRegion;

	nDx += nChangeX;
	nDy += nChangeY;

	if (nDx < 0)
	{
		nDx += (m_nCellWidth << 10);
		nMapX--;
	}
	else if (nDx >= (m_nCellWidth << 10))
	{
		nDx -= (m_nCellWidth << 10);
		nMapX++;
	}

	if (nDy < 0)
	{
		nDy += (m_nCellHeight << 10);
		nMapY--;
	}
	else if (nDy >= (m_nCellHeight << 10))
	{
		nDy -= (m_nCellHeight << 10);
		nMapY++;
	}

	if (nMapX < 0)
	{
		if (m_Region[nRegion].m_nConnectRegion[DIR_LEFT] == -1)
			return 0xff;

		nRegion = m_Region[nRegion].m_nConnectRegion[DIR_LEFT];
		nMapX += m_nRegionWidth;
	}
	else if (nMapX >= m_nRegionWidth)
	{
		if (m_Region[nRegion].m_nConnectRegion[DIR_RIGHT] == -1)
			return 0xff;
		nRegion = m_Region[nRegion].m_nConnectRegion[DIR_RIGHT];
		nMapX -= m_nRegionWidth;
	}

	if (nMapY < 0)
	{
		if (m_Region[nRegion].m_nConnectRegion[DIR_UP] == -1)
			return 0xff;
		nRegion = m_Region[nRegion].m_nConnectRegion[DIR_UP];
		;
		nMapY += m_nRegionHeight;
	}
	else if (nMapY >= m_nRegionHeight)
	{
		if (m_Region[nRegion].m_nConnectRegion[DIR_DOWN] == -1)
			return 0xff;
		nRegion = m_Region[nRegion].m_nConnectRegion[DIR_DOWN];
		nMapY -= m_nRegionHeight;
	}

	INT nXf, nYf;
	nXf = (nDx >> 10);
	nYf = (nDy >> 10);

#ifdef TOOLVERSION
	return Obstacle_NULL;
#endif
	// 需要检查的
#ifndef _SERVER
	INT nMpsX, nMpsY;
	NewMap2Mps(nRegion, nMapX, nMapY, nDx, nDy, &nMpsX, &nMpsY);
	BYTE bRet = (BYTE)g_ScenePlace.GetObstacleInfo(nMpsX, nMpsY);
	if (bRet != Obstacle_NULL)
		return bRet;
	return m_Region[nRegion].GetBarrier(nMapX, nMapY, nXf, nYf);
#else
	return m_Region[nRegion].GetBarrier(nMapX, nMapY, nXf, nYf);
#endif
}

BYTE KSubWorld::TestBarrierMin(INT nRegion, INT nMapX, INT nMapY, INT nDx, INT nDy, INT nChangeX, INT nChangeY)
{
	INT nOldMapX = nMapX;
	INT nOldMapY = nMapY;
	INT nOldRegion = nRegion;

	nDx += nChangeX;
	nDy += nChangeY;

	if (nDx < 0)
	{
		nDx += (m_nCellWidth << 10);
		nMapX--;
	}
	else if (nDx >= (m_nCellWidth << 10))
	{
		nDx -= (m_nCellWidth << 10);
		nMapX++;
	}

	if (nDy < 0)
	{
		nDy += (m_nCellHeight << 10);
		nMapY--;
	}
	else if (nDy >= (m_nCellHeight << 10))
	{
		nDy -= (m_nCellHeight << 10);
		nMapY++;
	}

	if (nMapX < 0)
	{
		if (m_Region[nRegion].m_nConnectRegion[DIR_LEFT] == -1)
			return 0xff;

		nRegion = m_Region[nRegion].m_nConnectRegion[DIR_LEFT];
		nMapX += m_nRegionWidth;
	}
	else if (nMapX >= m_nRegionWidth)
	{
		if (m_Region[nRegion].m_nConnectRegion[DIR_RIGHT] == -1)
			return 0xff;
		nRegion = m_Region[nRegion].m_nConnectRegion[DIR_RIGHT];
		nMapX -= m_nRegionWidth;
	}

	if (nMapY < 0)
	{
		if (m_Region[nRegion].m_nConnectRegion[DIR_UP] == -1)
			return 0xff;
		nRegion = m_Region[nRegion].m_nConnectRegion[DIR_UP];
		nMapY += m_nRegionHeight;
	}
	else if (nMapY >= m_nRegionHeight)
	{
		if (m_Region[nRegion].m_nConnectRegion[DIR_DOWN] == -1)
			return 0xff;
		nRegion = m_Region[nRegion].m_nConnectRegion[DIR_DOWN];
		nMapY -= m_nRegionHeight;
	}

#ifndef _SERVER

	INT nMpsX, nMpsY;
	NewMap2Mps(nRegion, nMapX, nMapY, nDx, nDy, &nMpsX, &nMpsY);
	BYTE bRet = (BYTE)g_ScenePlace.GetObstacleInfoMin(nMpsX, nMpsY, nDx & 0x000003ff, nDy & 0x000003ff);
	if (bRet != Obstacle_NULL)
		return bRet;

	if (nMapX == nOldMapX && nMapY == nOldMapY && nRegion == nOldRegion)
		return Obstacle_NULL;

	return m_Region[nRegion].GetBarrierMin(nMapX, nMapY, nDx, nDy, TRUE); // 精确查找

#else

	if (nMapX == nOldMapX && nMapY == nOldMapY && nRegion == nOldRegion)
	{
		return m_Region[nRegion].GetBarrierMin(nMapX, nMapY, nDx, nDy, FALSE);
	}

	return m_Region[nRegion].GetBarrierMin(nMapX, nMapY, nDx, nDy, TRUE);
#endif
}

DWORD KSubWorld::GetTrap(INT nMpsX, INT nMpsY)
{
	INT nRegion, nMapX, nMapY, nOffX, nOffY;
	Mps2Map(nMpsX, nMpsY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion == -1)
		return 0;
	return m_Region[nRegion].GetTrap(nMapX, nMapY);
}

BYTE KSubWorld::GetBarrier(INT nMpsX, INT nMpsY)
{
#ifdef _SERVER
	INT nRegion, nMapX, nMapY, nOffX, nOffY;
	Mps2Map(nMpsX, nMpsY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion == -1)
		return 0xff;
	return m_Region[nRegion].GetBarrier(nMapX, nMapY, (nOffX >> 10), (nOffY) >> 10);
#else
	return (BYTE)g_ScenePlace.GetObstacleInfo(nMpsX, nMpsY);
#endif
}

#ifdef _SERVER
INT KSubWorld::LoadMap(INT nId)
{
	KMapList MapList;

	memset(&MapList, 0, sizeof(MapList));

	char		szPathName[FILE_NAME_LENGTH];
	char		szFileName[FILE_NAME_LENGTH];
	char		szKeyName[32] = { 0 };
	char		szNewWorldScript[128] = { 0 };
	INT			nZNpcid = 0;

	char strMapKeyName[64] = { 0 };

	MapList.nSubWorldID = nId;
	m_SubWorldID = nId;

	sprintf(strMapKeyName, "%d_NpcSeriesAuto", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nNpcSeriesAuto);

	sprintf(strMapKeyName, "%d_NpcSeriesWood", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nNpcSeriesWood);

	sprintf(strMapKeyName, "%d_NpcSeriesMetal", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nNpcSeriesMetal);

	sprintf(strMapKeyName, "%d_NpcSeriesWater", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nNpcSeriesWater);

	sprintf(strMapKeyName, "%d_NpcSeriesFire", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nNpcSeriesFire);

	sprintf(strMapKeyName, "%d_NpcSeriesEarth", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nNpcSeriesEarth); // 五行

	sprintf(strMapKeyName, "%d_AutoGoldenNpc", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nAutoGoldenNpc); // 蓝怪出现的几率

	sprintf(strMapKeyName, "%d_GoldenType", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nGoldenType); // 黄金种类

	sprintf(strMapKeyName, "%d_NpcDefense", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nNpcDefense); // 防御

	sprintf(strMapKeyName, "%d_NpcMaxLife", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nNpcMaxLife); // 生命

	INT nTempInt = 0;

	sprintf(strMapKeyName, "%d_MapType", nId);
	g_NpcMapDropRate.GetString("List", strMapKeyName, "NoNpc", MapList.nMapType, sizeof(MapList.nMapType));

	sprintf(strMapKeyName, "%d_name", nId);
	g_NpcMapDropRate.GetString("List", strMapKeyName, "无名图", MapList.nMapName, sizeof(MapList.nMapName));
	// sprintf(m_SubWorldName,MapList.nMapName);   //地图名称
	nWorldMapInfo[FILE_NAME_LENGTH] = MapList.nMapName;

	sprintf(strMapKeyName, "%d_NpcAutoLevelMin", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nMinLevel);
	sprintf(strMapKeyName, "%d_NpcAutoLevelMax", nId);
	g_NpcMapDropRate.GetInteger("List", strMapKeyName, 0, &MapList.nMaxLevel);
	sprintf(strMapKeyName, "%d_GoldenDropRate", nId);
	g_NpcMapDropRate.GetString("List", strMapKeyName, "", MapList.nGoldenDropRate, sizeof(MapList.nGoldenDropRate)); // 蓝怪爆率
	sprintf(strMapKeyName, "%d_NormalDropRate", nId);
	g_NpcMapDropRate.GetString("List", strMapKeyName, "", MapList.nNormalDropRate, sizeof(MapList.nNormalDropRate)); // 爆率
	sprintf(strMapKeyName, "%d_NewWorldScript", nId);
	g_NpcMapDropRate.GetString("List", strMapKeyName, "", MapList.nNewWorldScript, sizeof(MapList.nNewWorldScript)); // 死亡触发脚本
	sprintf(strMapKeyName, "%d_NewWorldParam", nId);
	g_NpcMapDropRate.GetString("List", strMapKeyName, "", MapList.nNewWorldParam, sizeof(MapList.nNewWorldParam)); // 地图限制

	if (!MapList.nGoldenDropRate[0])
		sprintf(MapList.nGoldenDropRate, "\\settings\\droprate\\npcdroprate.ini");
	if (!MapList.nNormalDropRate[0])
		sprintf(MapList.nNormalDropRate, "\\settings\\droprate\\npcdroprate.ini");

	if (!strstr(MapList.nNewWorldScript, ".lua"))
	{
		g_NpcMapDropRate.GetString("List", "Default_NewWorldScript", "", MapList.nNewWorldScript, sizeof(MapList.nNewWorldScript));
	}

	KIniFile	IniFile;
	//IniFile.Load(MAPLIST_SETTING_FILE);

	sprintf(szKeyName, "%d", nId);
	g_NpcMapDropRate.GetString("List", szKeyName, "", szPathName, sizeof(szPathName));

	//IniFile.GetString("List", szKeyName, "", szPathName, sizeof(szPathName));

	g_StrCpyLen(szNewWorldScript, MapList.nNewWorldScript, sizeof(szNewWorldScript));
	g_StrCpyLen(m_szNewWorldParam, MapList.nNewWorldParam, sizeof(szNewWorldScript));
	//g_NpcMapDropRate.GetString("List", szNewWorldScript, "", m_szNewWorldScript, sizeof(m_szNewWorldScript));*/

	g_SetFilePath("\\maps");
	sprintf(szFileName, "%s.wor", szPathName);

	if (!IniFile.Load(szFileName))
	{
		printf("-------Load Map:(%d) Path Failure(%s)-------\n", nId, szFileName);
		m_SubWorldID = -1;
		return -1;

		//return FALSE;
	}

	/*INT nIsInDoor;
	IniFile.GetInteger("MAIN", "IsInDoor", 0, &nIsInDoor);
	if (!nIsInDoor)
	{
		m_pWeatherMgr = new KWeatherMgr;
		m_pWeatherMgr->InitFromIni(IniFile);
	}*/

	m_nRegionWidth = 512 / 32;
	m_nRegionHeight = 1024 / 32;

	RECT	sRect;
	IniFile.GetRect("MAIN", "rect", &sRect);
	m_nRegionBeginX = sRect.left;
	m_nRegionBeginY = sRect.top;
	m_nRegionEndX = sRect.right;
	m_nRegionEndY = sRect.bottom;
	m_nWorldRegionWidth = sRect.right - sRect.left + 1;			 // 宽度
	m_nWorldRegionHeight = sRect.bottom - sRect.top + 1;		 // 高度
	m_nTotalRegion = m_nWorldRegionWidth * m_nWorldRegionHeight; // 有多少个区域
	m_cActRegionIdx.Init(m_nTotalRegion + 1);

	//printf("-------Map (%d) total area (Z:%d,W:%d,H:%d)-------\n", nId, m_nTotalRegion, m_nWorldRegionWidth, m_nWorldRegionHeight);

	m_nCellWidth = defLOGIC_CELL_WIDTH;
	m_nCellHeight = defLOGIC_CELL_HEIGHT;

	IniFile.Clear();
	//printf("-------Map(%d):left:%u,top:%u,right:%u,bottom:%u;WW:%u,WH:%u-------\n", nId, m_nRegionBeginX, m_nRegionBeginY, m_nRegionEndX, m_nRegionEndY, m_nWorldRegionWidth, m_nWorldRegionHeight);

	if (m_nTotalRegion <= 0 || m_nWorldRegionWidth <= 0 || m_nWorldRegionHeight <= 0)
	{
		printf("-------Map (%d) Data error (W:%d,H:%d)-------\n", nId, m_nWorldRegionWidth, m_nWorldRegionHeight);
		m_SubWorldID = -1;
		return -2;
	}

	if (m_Region)
	{
		// DEL_CODEPOINT(m_Region);
		Close();
		delete[] m_Region;
		m_Region = NULL;
	}

	m_Region = new KRegion[m_nTotalRegion];	// Create a new memory with actual area size

	if (m_Region == NULL)
	{
		printf("-------Map (%d) failed to allocate memory (%d)-------\n", nId, m_nTotalRegion);
		m_SubWorldID = -1;
		return -2;
	}

	char	szPath[MAX_PATH];
	sprintf(szPath, "\\maps\\%s", szPathName);
	INT		nX, nY, nIdx;

	// Load map obstacles and connect each Region
	for (nY = 0; nY < m_nWorldRegionHeight; ++nY)
	{
		for (nX = 0; nX < m_nWorldRegionWidth; ++nX)
		{
			char szRegionPath[128] = { 0 };

			g_SetFilePath(szPath);
			nIdx = nY * m_nWorldRegionWidth + nX; // How many widths are there in this height?
			if (nIdx < m_nTotalRegion && m_Region[nIdx].Load(nX + m_nRegionBeginX, nY + m_nRegionBeginY))
			{ // 分配内存
				m_Region[nIdx].Init(m_nRegionWidth, m_nRegionHeight);
				m_Region[nIdx].m_nIndex = nIdx;
				m_Region[nIdx].m_nSubMapidx = m_nIndex;
			}

			for (INT i = 0; i < 8; ++i)
			{
				short nTmpX, nTmpY;
				nTmpX = (short)LOWORD(m_Region[nIdx].m_nConRegionID[i]) - m_nRegionBeginX;
				nTmpY = (short)HIWORD(m_Region[nIdx].m_nConRegionID[i]) - m_nRegionBeginY;
				if (nTmpX < 0 || nTmpY < 0 || nTmpX >= m_nWorldRegionWidth || nTmpY >= m_nWorldRegionHeight)
				{
					m_Region[nIdx].m_nConnectRegion[i] = -1;
					m_Region[nIdx].m_pConRegion[i] = NULL;
					continue;
				}
				INT nConIdx = nTmpY * m_nWorldRegionWidth + nTmpX;
				m_Region[nIdx].m_nConnectRegion[i] = nConIdx;
				m_Region[nIdx].m_nSubMapidx = m_nIndex;
				m_Region[nIdx].m_pConRegion[i] = &m_Region[nConIdx];
			}
		}
	}

	sprintf(szPath, "\\maps\\%s", szPathName);
	// printf("Map path: %s\n",szPath);
	for (nY = 0; nY < m_nWorldRegionHeight; ++nY)
	{
		for (nX = 0; nX < m_nWorldRegionWidth; ++nX)
		{
			// printf("Test 4: %s \n",szPath);
			INT m_NpcNum = 0;
			g_SetFilePath(szPath);

			try
			{
				if (nY * m_nWorldRegionWidth + nX < m_nTotalRegion)
				{
					m_NpcNum = m_Region[nY * m_nWorldRegionWidth + nX].LoadObject(m_nIndex, nX + m_nRegionBeginX, nY + m_nRegionBeginY, &MapList);
					nZNpcid += m_NpcNum;
				}
			}
			catch (...)
			{
				printf("Error loading map (%d): %s\n", nId, szPath);
			}
		}
	}
	return nZNpcid;
}
#endif

#ifndef _SERVER
BOOL KSubWorld::LoadMap(INT nId, INT nRegion)
{
	static INT	nXOff[8] = { 0, -1, -1, -1, 0,  1, 1, 1 };
	static INT	nYOff[8] = { 1, 1,  0,  -1, -1, -1, 0, 1 };
	KIniFile	IniFile;
	KIniFile	Ini;

	if (!m_Region)
	{
		m_Region = new KRegion[MAX_REGION];
	}

	if (nId != m_SubWorldID)
	{
		SubWorld[0].Close();
		g_ScenePlace.ClosePlace();
		// NpcSet.RemoveAll(Player[CLIENT_PLAYER_INDEX].m_nIndex); -- later finish it. spe

		char	szKeyName[32];
		char	szPathName[FILE_NAME_LENGTH];

		IniFile.Load(MAPLIST_SETTING_FILE);
		sprintf(szKeyName, "%d", nId);
		IniFile.GetString("List", szKeyName, "", szPathName, sizeof(szPathName));

		sprintf(m_szMapPath, "\\maps\\%s", szPathName);

		g_ScenePlace.OpenPlace(nId);
		sprintf(szPathName, "\\maps\\%d.ini", nId);
		Ini.Load(szPathName);
		g_ScenePlace.LoadIni(&Ini);

		m_SubWorldID = nId;
		g_ScenePlace.LoadSymbol(nId);
		//g_ScenePlace.DirectFindPos(0, 0, FALSE, FALSE);	//»æ»­

		m_nRegionWidth = KScenePlaceRegionC::RWPP_AREGION_WIDTH / 32;
		m_nRegionHeight = KScenePlaceRegionC::RWPP_AREGION_HEIGHT / 32;
		m_nCellWidth = 32;
		m_nCellHeight = 32;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex = -1;
	}

	INT nX = LOWORD(nRegion);
	INT nY = HIWORD(nRegion);

	INT nIdx = FindRegion(nRegion);

	if (nIdx < 0)
	{
		nIdx = m_ClientRegionIdx[0];

		if (m_Region[nIdx].Load(nX, nY))
		{
			m_Region[nIdx].m_nIndex = nIdx;
			m_Region[nIdx].Init(m_nRegionWidth, m_nRegionHeight);
			m_Region[nIdx].LoadObject(0, nX, nY, m_szMapPath);
		}
	}

	g_ScenePlace.SetFocusPosition(m_Region[nIdx].m_nRegionX, m_Region[nIdx].m_nRegionY, 0);
	m_ClientRegionIdx[0] = nIdx;
	for (INT i = 0; i < 8; i++)
	{
		INT nConIdx;
		nConIdx = FindRegion(m_Region[nIdx].m_nConRegionID[i]);

		if (nConIdx < 0)
		{
			nConIdx = FindFreeRegion(nX, nY);
			_ASSERT(nConIdx >= 0);

			if (m_Region[nConIdx].Load(nX + nXOff[i], nY + nYOff[i]))
			{
				m_Region[nConIdx].m_nIndex = nConIdx;
				m_Region[nConIdx].Init(m_nRegionWidth, m_nRegionHeight);
				m_Region[nConIdx].LoadObject(0, nX + nXOff[i], nY + nYOff[i], m_szMapPath);
			}
			else
			{
				m_Region[nConIdx].m_nIndex = -1;
				m_Region[nConIdx].m_RegionID = -1;
				nConIdx = -1;
			}
		}
		m_ClientRegionIdx[i + 1] = nConIdx;
		m_Region[nIdx].m_nConnectRegion[i] = nConIdx;
	}
	if (m_Region[nIdx].m_nConnectRegion[0] >= 0)
	{
		m_Region[m_Region[nIdx].m_nConnectRegion[0]].m_nConnectRegion[0] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[0]].m_nConnectRegion[1] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[0]].m_nConnectRegion[7] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[0]].m_nConnectRegion[4] = nIdx;
		m_Region[m_Region[nIdx].m_nConnectRegion[0]].m_nConnectRegion[2] = m_Region[nIdx].m_nConnectRegion[1];
		m_Region[m_Region[nIdx].m_nConnectRegion[0]].m_nConnectRegion[3] = m_Region[nIdx].m_nConnectRegion[2];
		m_Region[m_Region[nIdx].m_nConnectRegion[0]].m_nConnectRegion[5] = m_Region[nIdx].m_nConnectRegion[6];
		m_Region[m_Region[nIdx].m_nConnectRegion[0]].m_nConnectRegion[6] = m_Region[nIdx].m_nConnectRegion[7];
	}

	if (m_Region[nIdx].m_nConnectRegion[1] >= 0)
	{
		m_Region[m_Region[nIdx].m_nConnectRegion[1]].m_nConnectRegion[0] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[1]].m_nConnectRegion[1] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[1]].m_nConnectRegion[7] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[1]].m_nConnectRegion[4] = m_Region[nIdx].m_nConnectRegion[2];
		m_Region[m_Region[nIdx].m_nConnectRegion[1]].m_nConnectRegion[2] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[1]].m_nConnectRegion[3] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[1]].m_nConnectRegion[5] = nIdx;
		m_Region[m_Region[nIdx].m_nConnectRegion[1]].m_nConnectRegion[6] = m_Region[nIdx].m_nConnectRegion[0];
	}

	if (m_Region[nIdx].m_nConnectRegion[2] >= 0)
	{
		m_Region[m_Region[nIdx].m_nConnectRegion[2]].m_nConnectRegion[0] = m_Region[nIdx].m_nConnectRegion[1];
		m_Region[m_Region[nIdx].m_nConnectRegion[2]].m_nConnectRegion[1] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[2]].m_nConnectRegion[7] = m_Region[nIdx].m_nConnectRegion[0];
		m_Region[m_Region[nIdx].m_nConnectRegion[2]].m_nConnectRegion[4] = m_Region[nIdx].m_nConnectRegion[3];
		m_Region[m_Region[nIdx].m_nConnectRegion[2]].m_nConnectRegion[2] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[2]].m_nConnectRegion[3] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[2]].m_nConnectRegion[5] = m_Region[nIdx].m_nConnectRegion[4];
		m_Region[m_Region[nIdx].m_nConnectRegion[2]].m_nConnectRegion[6] = nIdx;
	}

	if (m_Region[nIdx].m_nConnectRegion[3] >= 0)
	{
		m_Region[m_Region[nIdx].m_nConnectRegion[3]].m_nConnectRegion[0] = m_Region[nIdx].m_nConnectRegion[2];
		m_Region[m_Region[nIdx].m_nConnectRegion[3]].m_nConnectRegion[1] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[3]].m_nConnectRegion[2] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[3]].m_nConnectRegion[3] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[3]].m_nConnectRegion[4] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[3]].m_nConnectRegion[5] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[3]].m_nConnectRegion[6] = m_Region[nIdx].m_nConnectRegion[4];
		m_Region[m_Region[nIdx].m_nConnectRegion[3]].m_nConnectRegion[7] = nIdx;
	}

	if (m_Region[nIdx].m_nConnectRegion[4] >= 0)
	{
		m_Region[m_Region[nIdx].m_nConnectRegion[4]].m_nConnectRegion[0] = nIdx;
		m_Region[m_Region[nIdx].m_nConnectRegion[4]].m_nConnectRegion[1] = m_Region[nIdx].m_nConnectRegion[2];
		m_Region[m_Region[nIdx].m_nConnectRegion[4]].m_nConnectRegion[2] = m_Region[nIdx].m_nConnectRegion[3];
		m_Region[m_Region[nIdx].m_nConnectRegion[4]].m_nConnectRegion[3] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[4]].m_nConnectRegion[4] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[4]].m_nConnectRegion[5] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[4]].m_nConnectRegion[6] = m_Region[nIdx].m_nConnectRegion[5];
		m_Region[m_Region[nIdx].m_nConnectRegion[4]].m_nConnectRegion[7] = m_Region[nIdx].m_nConnectRegion[6];
	}

	if (m_Region[nIdx].m_nConnectRegion[5] >= 0)
	{
		m_Region[m_Region[nIdx].m_nConnectRegion[5]].m_nConnectRegion[0] = m_Region[nIdx].m_nConnectRegion[6];
		m_Region[m_Region[nIdx].m_nConnectRegion[5]].m_nConnectRegion[1] = nIdx;
		m_Region[m_Region[nIdx].m_nConnectRegion[5]].m_nConnectRegion[2] = m_Region[nIdx].m_nConnectRegion[4];
		m_Region[m_Region[nIdx].m_nConnectRegion[5]].m_nConnectRegion[3] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[5]].m_nConnectRegion[4] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[5]].m_nConnectRegion[5] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[5]].m_nConnectRegion[6] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[5]].m_nConnectRegion[7] = -1;
	}

	if (m_Region[nIdx].m_nConnectRegion[6] >= 0)
	{
		m_Region[m_Region[nIdx].m_nConnectRegion[6]].m_nConnectRegion[0] = m_Region[nIdx].m_nConnectRegion[7];
		m_Region[m_Region[nIdx].m_nConnectRegion[6]].m_nConnectRegion[1] = m_Region[nIdx].m_nConnectRegion[0];
		m_Region[m_Region[nIdx].m_nConnectRegion[6]].m_nConnectRegion[2] = nIdx;
		m_Region[m_Region[nIdx].m_nConnectRegion[6]].m_nConnectRegion[3] = m_Region[nIdx].m_nConnectRegion[4];
		m_Region[m_Region[nIdx].m_nConnectRegion[6]].m_nConnectRegion[4] = m_Region[nIdx].m_nConnectRegion[5];
		m_Region[m_Region[nIdx].m_nConnectRegion[6]].m_nConnectRegion[5] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[6]].m_nConnectRegion[6] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[6]].m_nConnectRegion[7] = -1;
	}

	if (m_Region[nIdx].m_nConnectRegion[7] >= 0)
	{
		m_Region[m_Region[nIdx].m_nConnectRegion[7]].m_nConnectRegion[0] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[7]].m_nConnectRegion[1] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[7]].m_nConnectRegion[2] = m_Region[nIdx].m_nConnectRegion[0];
		m_Region[m_Region[nIdx].m_nConnectRegion[7]].m_nConnectRegion[3] = nIdx;
		m_Region[m_Region[nIdx].m_nConnectRegion[7]].m_nConnectRegion[4] = m_Region[nIdx].m_nConnectRegion[6];
		m_Region[m_Region[nIdx].m_nConnectRegion[7]].m_nConnectRegion[5] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[7]].m_nConnectRegion[6] = -1;
		m_Region[m_Region[nIdx].m_nConnectRegion[7]].m_nConnectRegion[7] = -1;
	}
	return TRUE;
}
#endif

void KSubWorld::LoadTrap()
{
}

void KSubWorld::MessageLoop()
{
	KWorldMsgNode	Msg;
	while (m_WorldMessage.Get(&Msg))
	{
		ProcessMsg(&Msg);
	}
}

void KSubWorld::ProcessMsg(KWorldMsgNode* pMsg)
{
	switch (pMsg->m_dwMsgType)
	{
	case GWM_NPC_DEL:
		if (pMsg->m_nParam[0] >= 0 && pMsg->m_nParam[0] < MAX_NPC)
#ifndef _SERVER
		{
			if (Npc[pMsg->m_nParam[0]].m_RegionIndex >= 0)
			{
				INT nIdx = pMsg->m_nParam[0];
				INT nSubWorld = Npc[nIdx].m_SubWorldIndex;
				INT nRegion = Npc[nIdx].m_RegionIndex;
				SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(nIdx);
				SubWorld[nSubWorld].m_Region[nRegion].DecNpcRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
			}

			NpcSet.Remove(pMsg->m_nParam[0]);
		}
#else
		{
			INT nIdx = pMsg->m_nParam[0];
			if (nIdx >= 0 && nIdx < MAX_NPC)
			{
				INT nSubWorld = Npc[nIdx].m_SubWorldIndex;
				INT nRegion = Npc[nIdx].m_RegionIndex;

				SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(nIdx);
				SubWorld[nSubWorld].m_Region[nRegion].DecNpcRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
				NpcSet.Remove(nIdx);
			}
		}
#endif
		break;
	case GWM_NPC_CHANGE_REGION:
		NpcChangeRegion(pMsg->m_nParam[0], pMsg->m_nParam[1], pMsg->m_nParam[2]);
		break;
	case GWM_OBJ_DEL:
		if (pMsg->m_nParam[0] >= 0 && pMsg->m_nParam[0] < MAX_OBJECT)
#ifndef _SERVER
		{
			if (Object[pMsg->m_nParam[0]].m_nRegionIdx >= 0)
				SubWorld[Object[pMsg->m_nParam[0]].m_nSubWorldID].m_Region[Object[pMsg->m_nParam[0]].m_nRegionIdx].RemoveObj(pMsg->m_nParam[0]);

			ObjSet.Remove(pMsg->m_nParam[0]);
		}
#else
		{
			SubWorld[Object[pMsg->m_nParam[0]].m_nSubWorldID].m_Region[Object[pMsg->m_nParam[0]].m_nRegionIdx].RemoveObj(pMsg->m_nParam[0]);
			ObjSet.Remove(pMsg->m_nParam[0]);
		}
#endif
		break;
	case GWM_OBJ_CHANGE_REGION:
		ObjChangeRegion(pMsg->m_nParam[0], pMsg->m_nParam[1], pMsg->m_nParam[2]);
		break;
	case GWM_MISSLE_CHANGE_REGION:
		MissleChangeRegion(pMsg->m_nParam[0], pMsg->m_nParam[1], pMsg->m_nParam[2]);
		break;
	case GWM_MISSLE_DEL:
		if (pMsg->m_nParam[0] >= 0 && pMsg->m_nParam[0] < MAX_MISSLE)
#ifndef _SERVER
			if (Missle[pMsg->m_nParam[0]].m_nRegionId >= 0)
			{
				SubWorld[Missle[pMsg->m_nParam[0]].m_nSubWorldId].m_Region[Missle[pMsg->m_nParam[0]].m_nRegionId].RemoveMissle(pMsg->m_nParam[0]);
				//		g_DebugLog("[Missle]Missle%dDEL", pMsg->m_nParam[0]);

				MissleSet.Remove(pMsg->m_nParam[0]);
			}
#else
			if (pMsg->m_nParam[0] >= 0 && pMsg->m_nParam[0] < MAX_MISSLE)
			{
				SubWorld[Missle[pMsg->m_nParam[0]].m_nSubWorldId].m_Region[Missle[pMsg->m_nParam[0]].m_nRegionId].RemoveMissle(pMsg->m_nParam[0]);
				MissleSet.Remove(pMsg->m_nParam[0]);
			}
#endif
		break;
	case GWM_PLAYER_CHANGE_REGION:
#ifdef _SERVER
		PlayerChangeRegion(pMsg->m_nParam[0], pMsg->m_nParam[1], pMsg->m_nParam[2]);
#endif
		break;
	default:
		break;
	}
}

#ifdef _SERVER
void KSubWorld::NpcChangeRegion(INT nSrcRnidx, INT nDesRnIdx, INT nIdx)
{
	if (nIdx < 0 || nIdx >= MAX_NPC)
		return;

	Npc[nIdx].SetActiveFlag(TRUE);
	if (nSrcRnidx == -1)
	{
		_ASSERT(nDesRnIdx >= 0);
		if (nDesRnIdx >= 0)
		{
			m_Region[nDesRnIdx].AddNpc(nIdx);
			Npc[nIdx].m_RegionIndex = nDesRnIdx;
		}
		return;
	}
	else if (nSrcRnidx == VOID_REGION)
	{
		Npc[nIdx].m_Node.Remove();
		Npc[nIdx].m_Node.Release();
	}
	else if (nSrcRnidx >= 0)
	{
		SubWorld[Npc[nIdx].m_SubWorldIndex].m_Region[nSrcRnidx].RemoveNpc(nIdx);
		if (nDesRnIdx != VOID_REGION)	// ²»ÊÇ¼ÓÈëµ½ËÀÍöÖØÉúÁ´±í
			Npc[nIdx].m_RegionIndex = -1;
	}

	if (nDesRnIdx >= 0)
	{
		m_Region[nDesRnIdx].AddNpc(nIdx);
		Npc[nIdx].m_RegionIndex = nDesRnIdx;
	}
	else if (nDesRnIdx == VOID_REGION)
	{
		m_NoneRegionNpcList.AddTail(&Npc[nIdx].m_Node);
		Npc[nIdx].m_Node.AddRef();
	}
}
#endif

#ifndef _SERVER
void KSubWorld::NpcChangeRegion(INT nSrcRnidx, INT nDesRnIdx, INT nIdx)
{
	INT		nSrc, nDest;

	if (nSrcRnidx == -1)
	{
		nDest = SubWorld[0].FindRegion(nDesRnIdx);
		if (nDest < 0)
			return;
		m_Region[nDest].AddNpc(nIdx);
		Npc[nIdx].m_dwRegionID = m_Region[nDest].m_RegionID;
		Npc[nIdx].m_RegionIndex = nDest;
		return;
	}

	nSrc = SubWorld[0].FindRegion(nSrcRnidx);
	if (nSrc >= 0)
		SubWorld[0].m_Region[nSrc].RemoveNpc(nIdx);

	nDest = SubWorld[0].FindRegion(nDesRnIdx);

	KIndexNode* pNode = &Npc[nIdx].m_Node;
	if (nDest >= 0)
	{
		m_Region[nDest].AddNpc(nIdx);

		if (Player[CLIENT_PLAYER_INDEX].m_nIndex == nIdx)
			LoadMap(m_SubWorldID, m_Region[nDest].m_RegionID);
		Npc[nIdx].m_dwRegionID = m_Region[nDest].m_RegionID;
		Npc[nIdx].m_RegionIndex = nDest;
	}
	else if (Player[CLIENT_PLAYER_INDEX].m_nIndex == nIdx)
	{
		LoadMap(m_SubWorldID, nDesRnIdx);
		nDest = SubWorld[0].FindRegion(nDesRnIdx);
		if (nDest >= 0)
		{
			m_Region[nDest].AddNpc(nIdx);
			Npc[nIdx].m_dwRegionID = m_Region[nDest].m_RegionID;
			Npc[nIdx].m_RegionIndex = nDest;
	}
}
}
#endif

void KSubWorld::ObjChangeRegion(INT nSrcRnidx, INT nDesRnIdx, INT nIdx)
{
#ifdef _SERVER
	_ASSERT(nSrcRnidx >= 0);
#endif
	if (nDesRnIdx == -1)
	{
		SubWorld[Object[nIdx].m_nSubWorldID].m_Region[nSrcRnidx].RemoveObj(nIdx);
		//		g_cObjMgr.Remove(nIdx);
		return;
	}
	if (nSrcRnidx == -1)
	{
		m_Region[nDesRnIdx].AddObj(nIdx);
		return;
	}
	KRegion::VEC_LIST::iterator it = m_Region[nSrcRnidx].GetObjNode(nIdx);
	if (it != m_Region[nSrcRnidx].m_ObjList.end())
	{
		m_Region[nSrcRnidx].m_ObjList.erase(it);
		m_Region[nDesRnIdx].m_ObjList.push_back(nIdx);
	}
}

void KSubWorld::MissleChangeRegion(INT nSrcRnidx, INT nDesRnIdx, INT nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_MISSLE)
		return;

	if (nSrcRnidx == -1)
	{
		m_Region[nDesRnIdx].AddMissle(nIdx);
		return;
	}

	SubWorld[Missle[nIdx].m_nSubWorldId].m_Region[nSrcRnidx].RemoveMissle(nIdx);

	if (nDesRnIdx == -1)
	{
		MissleSet.Remove(nIdx);
		return;
	}

	m_Region[nDesRnIdx].AddMissle(nIdx);
}

#ifdef _SERVER
void KSubWorld::PlayerChangeRegion(INT nSrcRnidx, INT nDesRnIdx, INT nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_PLAYER)
		return;

	if (nSrcRnidx < 0)
		return;

	RemovePlayer(nSrcRnidx, nIdx);

	if (nDesRnIdx == -1)
	{
		//PlayerSet.PrepareRemove(Player[nIdx].m_nNetConnectIdx);
		printf("Player change region to not exist..., kill it\n");
		g_pServer->ShutdownClient(Player[nIdx].m_nNetConnectIdx);
		return;
	}

	if (nDesRnIdx >= 0)
	{
		AddPlayer(nDesRnIdx, nIdx);
	}
	/* ÒòÎªRemovePlayerºÍAddPlayerÒÑ¾­×öÁË´ÓÁ´±íÖÐÇå³ý½Úµã£¬ÐÞ¸ÄÖÜ±ß¾Å¸öREGIONµÄÒýÓÃ¼ÆÊýµÄ²Ù×÷ÁË
		KIndexNode* pNode = &Player[nIdx].m_Node;
		if (pNode->m_Ref > 0)
		{
			pNode->Remove();
			pNode->Release();
		}
		m_Region[nSrcRnidx].m_nActive--;
		if (m_Region[nSrcRnidx].m_nActive < 0)
			m_Region[nSrcRnidx].m_nActive = 0;

		for (INT i = 0; i < 8; i++)
		{
			if (m_Region[nSrcRnidx].m_nConnectRegion[i] < 0)
				continue;
			m_Region[m_Region[nSrcRnidx].m_nConnectRegion[i]].m_nActive--;
			if (m_Region[m_Region[nSrcRnidx].m_nConnectRegion[i]].m_nActive < 0)
				m_Region[m_Region[nSrcRnidx].m_nConnectRegion[i]].m_nActive = 0;
		}

		if (pNode->m_Ref == 0)
		{
			m_Region[nDesRnIdx].m_PlayerList.AddTail(pNode);
			pNode->AddRef();
		}

		m_Region[nDesRnIdx].m_nActive++;
		for (i = 0; i < 8; i++)
		{
			if (m_Region[nDesRnIdx].m_nConnectRegion[i] < 0)
				continue;
			m_Region[m_Region[nDesRnIdx].m_nConnectRegion[i]].m_nActive++;
			if (m_Region[m_Region[nDesRnIdx].m_nConnectRegion[i]].m_nActive < 0)
				m_Region[m_Region[nDesRnIdx].m_nConnectRegion[i]].m_nActive = 0;
		}
	*/
}
#endif

void KSubWorld::GetMps(INT* nX, INT* nY, INT nSpeed, INT nDir, INT nMaxDir /* = 64 */)
{
	/*if (nDir < 0)
		nDir = MaxMissleDir +nDir;

	if (nDir >= MaxMissleDir)
		nDir -= MaxMissleDir;*/
	INT n;
	if (nDir < 0)
	{
		if (nDir >= -MaxMissleDir)
			nDir = MaxMissleDir + nDir;
		else
		{							  //-129;
			n = -nDir % MaxMissleDir; //		 -1
			nDir = MaxMissleDir - n - 1;
		}
		// nWantDir = MaxMissleDir+ nWantDir;
	}

	if (nDir >= MaxMissleDir)
	{
		n = nDir / MaxMissleDir;
		nDir -= MaxMissleDir * n;
	}

	*nX += (g_DirCos(nDir, nMaxDir) * nSpeed) >> 10;
	*nY += (g_DirSin(nDir, nMaxDir) * nSpeed) >> 10;
}
#ifdef _SERVER
BOOL KSubWorld::SendSyncData(INT nIdx, INT nClient)
{
	WORLD_SYNC	WorldSync;
	WorldSync.ProtocolType = (BYTE)s2c_syncworld;
	WorldSync.Region = m_Region[Npc[nIdx].m_RegionIndex].m_RegionID;
	WorldSync.Weather = m_nWeather;
	WorldSync.Frame = m_dwCurrentTime;
	WorldSync.SubWorld = m_SubWorldID;

	if (SUCCEEDED(g_pServer->PackDataToClient(nClient, (BYTE*)&WorldSync, sizeof(WORLD_SYNC))))
	{
		return TRUE;
	}
	else
	{
		printf("player Packing world sync data failed...\n");
		return FALSE;
	}
}
#endif

#ifndef _SERVER
void KSubWorld::LoadCell()
{
}
#endif

#ifdef TOOLVERSION
INT CORE_API g_ScreenX = 0;
INT CORE_API g_ScreenY = 0;
#endif

#ifndef _SERVER
void KSubWorld::Paint()
{
	INT nIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	INT nX, nY;

	Map2Mps(Npc[nIdx].m_RegionIndex, Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, Npc[nIdx].m_OffX, Npc[nIdx].m_OffY, &nX, &nY);

	//	m_nScreenX = nX - 400;
	//	m_nScreenY = nY - 300;

#ifdef TOOLVERSION
	g_ScreenX = nX;
	g_ScreenY = nY;
#endif

	for (INT i = 0; i < m_nTotalRegion; i++)
	{
		m_Region[i].Paint();
	}
}

void KSubWorld::Mps2Screen(INT* Rx, INT* Ry)
{
	}

void KSubWorld::Screen2Mps(INT* Rx, INT* Ry)
{
}
#endif

#ifdef _SERVER
INT KSubWorld::GetRegionIndex(INT nRegionID)
{
	INT nRet;
	int16_t nX = (int16_t)LOWORD(nRegionID) - m_nRegionBeginX;
	int16_t nY = (int16_t)HIWORD(nRegionID) - m_nRegionBeginY;

	if (nX < 0 || nY < 0 || nX >= m_nWorldRegionWidth || nY >= m_nWorldRegionHeight)
		return -1;

	nRet = nY * m_nWorldRegionWidth + nX;
	return nRet;
}
#endif

void KSubWorld::Close()
{
	if (m_SubWorldID == -1)
		return;

	for (INT i = 0; i < m_nTotalRegion; i++)
	{
		m_Region[i].Close();
	}
	m_WorldMessage.Clear();
	m_nIndex = -1;
	m_SubWorldID = -1;
}

#ifdef _SERVER
void KSubWorld::LoadObject(char* szPath, char* szFileName)
{
	if (!szFileName || !szFileName[0])
		return;

	KIniFile IniFile;
	// ¼ÓÔØÊÀ½çÖÐµÄ¶¯Ì¬Îï¼þ£¨Npc¡¢Object¡¢Trap£©
	g_SetFilePath(szPath);
	INT nObjNumber = 0;
	if (IniFile.Load(szFileName))
	{
		IniFile.GetInteger("MAIN", "elementnum", 0, &nObjNumber);
	}

	INT nLength = strlen(szFileName);
	szFileName[nLength - 4] = 0;
	strcat(szPath, "\\");
	strcat(szPath, szFileName);

	char	szSection[32];
	char	szDataFile[32];

	for (INT i = 0; i < nObjNumber; i++)
	{
		float	fPos[3];
		INT		nPos[3];

		g_SetFilePath(szPath);
		sprintf(szSection, "%d", i);
		IniFile.GetFloat3(szSection, "groundoffset", fPos);
		nPos[0] = (INT)(fPos[0] * 32);
		nPos[1] = (INT)(fPos[1] * 32);
		nPos[2] = (INT)fPos[2];

		szDataFile[0] = 0;
		IniFile.GetString(szSection, "event", "", szDataFile, sizeof(szDataFile));
		strcat(szDataFile, "_data.ini");

		KIniFile IniChange;
		INT nType = 0, nLevel = 0;
		char	szName[32];

		if (!IniChange.Load(szDataFile))
			continue;

		IniChange.GetInteger("MAIN", "Type", 0, &nType);
		IniChange.GetInteger("MAIN", "Level", 4, &nLevel);
		IniChange.GetString("MAIN", "mapedit_templatesection", "", szName, sizeof(szName));

		switch (nType)
		{
		case kind_normal:		// ÆÕÍ¨Õ½¶·npc
		{
			INT nFindNo = g_NpcSetting.FindRow(szName);
			if (nFindNo == -1)
				continue;
			else
				nFindNo = nFindNo - 2;

			INT nSettingInfo = MAKELONG(nLevel, nFindNo);
			NpcSet.Add(nSettingInfo, m_nIndex, nPos[0], nPos[1]);
		}
		break;
		case kind_player:
			break;
		case kind_partner:
			break;
		case kind_dialoger:		// ÆÕÍ¨·ÇÕ½¶·npc
			break;
		case kind_bird:			// ¿Í»§¶Ëonly
			break;
		case kind_mouse:		// ¿Í»§¶Ëonly
			break;
		default:
			break;
		}
	}
}
#endif

void KSubWorld::AddPlayer(INT nRegion, INT nIdx)
{
	if (nRegion < 0 || nRegion >= m_nTotalRegion)
	{
		printf("[RegionIdx]Region:(%d) Player(%d)\n", nRegion, nIdx);
		return;
	}

	if (m_Region[nRegion].AddPlayer(nIdx))
	{
		m_Region[nRegion].m_nActive++;

		for (INT i = 0; i < 8; i++)
		{
			INT nConRegion = m_Region[nRegion].m_nConnectRegion[i];
			if (nConRegion == -1)
				continue;

			m_Region[nConRegion].m_nActive++;
		}
	}
	else
	{
		printf("[RegionAdd]Region:(%d) Player(%d)\n", nRegion, nIdx);
	}
}

void KSubWorld::RemovePlayer(INT nRegion, INT nIdx)
{
	if (nRegion < 0 || nRegion >= m_nTotalRegion)
		return;

	if (m_Region[nRegion].RemovePlayer(nIdx))
	{
		m_Region[nRegion].m_nActive--;

		if (m_Region[nRegion].m_nActive < 0)
		{
			_ASSERT(0);
			m_Region[nRegion].m_nActive = 0;
		}

		for (INT i = 0; i < 8; i++)
		{
			INT nConRegion = m_Region[nRegion].m_nConnectRegion[i];
			if (nConRegion == -1)
				continue;

			m_Region[nConRegion].m_nActive--;

			if (m_Region[nConRegion].m_nActive < 0)
			{
				_ASSERT(0);
				m_Region[nConRegion].m_nActive = 0;
			}
		}
	}
}

void KSubWorld::GetFreeObjPos(POINT& pos)
{
	POINT	posLocal = pos;
	POINT	posTemp;
	INT nLayer = 1;

	//if (CanPutObj(posLocal))
	//	return;

	INT nCheckNum = 32, nIsCheckNpcPos = 0, nModel = 0, nIsCheckNpc = 0;

	g_GameSetting.GetInteger2("Item", "ItemDropCheckNum", &nCheckNum, &nIsCheckNpcPos); // 查找的范围
	g_GameSetting.GetInteger2("Item", "ItemDropModel", &nModel, &nIsCheckNpc);

	if (nIsCheckNpcPos == 1) // 如果 NPC所在的地方能掉东西 就 返回
	{						 // 检测NPC点
		if (CanPutObj(posLocal, nModel, nIsCheckNpc))
			return;
	}

	while (1)
	{
		for (INT i = 0; i <= nLayer; ++i)
		{
			posTemp.y = posLocal.y + i * 32;
			posTemp.x = posLocal.x + (nLayer - i) * 32;
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			posTemp.y = posLocal.y + i * 32;
			posTemp.x = posLocal.x - (nLayer - i) * 32;
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			posTemp.y = posLocal.y - i * 32;
			posTemp.x = posLocal.x + (nLayer - i) * 32;
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			posTemp.y = posLocal.y - i * 32;
			posTemp.x = posLocal.x - (nLayer - i) * 32;
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
		}
		nLayer++;
		if (nLayer >= 10)
			break;
	}
	return;
}

void KSubWorld::GetFreePos(POINT& pos)
{
	POINT posLocal = pos;
	POINT posTemp;
	INT nLayer = 1;

	// if (CanPutObj(posLocal,nModel)) //如果 NPC所在的地方能掉东西 就 返回
	//	return;

	INT xPos = 1, yPos = 1, nCheckNum = 512, nIsCheckNpcPos = 0, nModel = 0, nIsCheckNpc = 0;

	if (nIsCheckNpcPos == 1) // 如果 NPC所在的地方能掉东西 就 返回
	{						 // 检测NPC点
		if (CanPutObj(posLocal, nModel, nIsCheckNpc))
			return;
	}

	while (1)
	{
		for (INT i = 0; i <= nLayer; ++i)
		{
			posTemp.y = posLocal.y;
			posTemp.x = posLocal.x + (i + 1) * xPos; // 正右边

			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			else
			{
			}

			posTemp.y = posLocal.y + (i + 1) * yPos;
			posTemp.x = posLocal.x; // 正下边
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			else
			{
			}

			posTemp.y = posLocal.y;
			posTemp.x = posLocal.x - (i + 1) * xPos; // 正左边

			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			else
			{
			}

			posTemp.y = posLocal.y - (i + 1) * yPos;
			posTemp.x = posLocal.x; // 正上边
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			else
			{
			}
			posTemp.y = posLocal.y + (i + 1) * yPos; // 像数坐标
			posTemp.x = posLocal.x + (i + 1) * xPos; // 往右下方向(相隔的距离) (nLayer - i)

			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			else
			{
			}

			posTemp.y = posLocal.y + (i + 1) * yPos;
			posTemp.x = posLocal.x - (i + 1) * xPos; // 往左下方向
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			else
			{
			}

			posTemp.y = posLocal.y - (i + 1) * yPos; // 往右上方向
			posTemp.x = posLocal.x + (i + 1) * xPos;
			if (CanPutObj(posTemp, nModel))
			{
				pos = posTemp;
				return;
			}
			else
			{
			}

			posTemp.y = posLocal.y - (i + 1) * yPos;
			posTemp.x = posLocal.x - (i + 1) * xPos; // 往左上方向
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
			else
			{
			}
		}

		nLayer++;

		if (nLayer >= nCheckNum)
			break;
	}

	return;
}

//Adjustment of distance of dropped items
void KSubWorld::GetFreeNewObjPos(POINT& pos)
{
	POINT posLocal = pos;
	POINT posTemp;
	INT nLayer = 1;

	// if (CanPutObj(posLocal,nModel)) //如果 NPC所在的地方能掉东西 就 返回
	//	return;

	INT xPos = 16, yPos = 16, nCheckNum = 32, nIsCheckNpcPos = 0, nModel = 0, nIsCheckNpc = 0;
	g_GameSetting.GetInteger2("Item", "ItemDropPos", &xPos, &yPos);						// 偏移
	g_GameSetting.GetInteger2("Item", "ItemDropCheckNum", &nCheckNum, &nIsCheckNpcPos); // 查找的范围
	g_GameSetting.GetInteger2("Item", "ItemDropModel", &nModel, &nIsCheckNpc);

	if (nIsCheckNpcPos == 1) // 如果 NPC所在的地方能掉东西 就 返回
	{						 // 检测NPC点
		if (CanPutObj(posLocal, nModel, nIsCheckNpc))
			return;
	}

	while (1)
	{
		for (INT i = 0; i <= nLayer; ++i)
		{
			posTemp.y = posLocal.y;
			posTemp.x = posLocal.x + (i + 1) * xPos; // 正右边

			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}

			posTemp.y = posLocal.y + (i + 1) * yPos;
			posTemp.x = posLocal.x; // 正下边
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}

			posTemp.y = posLocal.y;
			posTemp.x = posLocal.x - (i + 1) * xPos; // 正左边

			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}

			posTemp.y = posLocal.y - (i + 1) * yPos;
			posTemp.x = posLocal.x; // 正上边
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}

			posTemp.y = posLocal.y + (i + 1) * yPos; // 像数坐标
			posTemp.x = posLocal.x + (i + 1) * xPos; // 往右下方向(相隔的距离) (nLayer - i)

			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}

			posTemp.y = posLocal.y + (i + 1) * yPos;
			posTemp.x = posLocal.x - (i + 1) * xPos; // 往左下方向
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}

			posTemp.y = posLocal.y - (i + 1) * yPos; // 往右上方向
			posTemp.x = posLocal.x + (i + 1) * xPos;
			if (CanPutObj(posTemp, nModel))
			{
				pos = posTemp;
				return;
			}

			posTemp.y = posLocal.y - (i + 1) * yPos;
			posTemp.x = posLocal.x - (i + 1) * xPos; // 往左上方向
			if (CanPutObj(posTemp, nModel, nIsCheckNpc))
			{
				pos = posTemp;
				return;
			}
		}

		nLayer++;

		if (nLayer >= nCheckNum)
			break;
	}

	return;
}

BOOL KSubWorld::CanPutObj(POINT pos, INT nModle, BOOL nIsCheckNpc)
{
	INT nRegion, nMapX, nMapY, nOffX, nOffY, nIsShowInfo = 0, nIsShowHave = 0;
	Mps2Map(pos.x, pos.y, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	if (nModle == 0)
	{
		if (nRegion >= 0 && !m_Region[nRegion].GetNewBarrier(nMapX, nMapY, nOffX, nOffY, nIsCheckNpc) && !m_Region[nRegion].HasObj(nMapX, nMapY)) // m_Region[nRegion].GetRef(nMapX,nMapY,obj_object))
		{
#ifdef _SERVER
			// if (nIsShowHave)
			//    printf("----[找到地上空位置]原像素:X=%d,Y=%d,区域:%d,格子:X=%d,Y=%d,偏移:XOFF=%d,YOFF=%d----\n",pos.x,pos.y,nRegion,nMapX,nMapY,nOffX,nOffY);
#endif
			return TRUE;
		}
	}
	else
	{
		if (nRegion >= 0 && !m_Region[nRegion].GetBarrierNewMin(nMapX, nMapY, nOffX, nOffY, nIsCheckNpc) && !m_Region[nRegion].HasObj(nMapX, nMapY)) // m_Region[nRegion].GetRef(nMapX,nMapY,obj_object))
		{
#ifdef _SERVER
			// if (nIsShowHave)
			//  printf("----[找到地上空位置]原像素:X=%d,Y=%d,区域:%d,格子:X=%d,Y=%d,偏移:XOFF=%d,YOFF=%d----\n",pos.x,pos.y,nRegion,nMapX,nMapY,nOffX,nOffY);
#endif
			return TRUE;
		}
	}
	return FALSE;
}

#ifdef _SERVER
void KSubWorld::BroadCast(const char* pBuffer, size_t uSize)
{
	INT nIdx = PlayerSet.GetFirstPlayer();
	while (nIdx)
	{
		if (Player[nIdx].m_nNetConnectIdx >= 0
			&& Player[nIdx].m_nIndex > 0
			&& Npc[Player[nIdx].m_nIndex].m_SubWorldIndex == m_nIndex)
		{
			g_pServer->PackDataToClient(Player[nIdx].m_nNetConnectIdx, pBuffer, uSize);
		}
		nIdx = PlayerSet.GetNextPlayer();
	}
}

INT	KSubWorld::RevivalAllNpc()
{
	KIndexNode* pNode = NULL;
	size_t	ulCount = 0;
	for (INT i = 0; i < m_nTotalRegion; i++)
	{
		KRegion* pCurRegion = &m_Region[i];
		pNode = (KIndexNode*)pCurRegion->m_NpcList.GetHead();

		while (pNode)
		{
			INT nNpcIdx = pNode->m_nIndex;
			if (!Npc[nNpcIdx].IsPlayer())
			{
				if (Npc[nNpcIdx].m_Doing == do_revive)
					Npc[nNpcIdx].m_Doing = do_none;
				Npc[nNpcIdx].ExecuteRevive();
			}
			pNode = (KIndexNode*)pNode->GetNext();
		}
	}

	pNode = (KIndexNode*)m_NoneRegionNpcList.GetHead();

	while (pNode)
	{
		INT nNpcIdx = pNode->m_nIndex;
		if (!Npc[nNpcIdx].IsPlayer())
		{
			Npc[nNpcIdx].m_Frames.nTotalFrame = 1;
			Npc[nNpcIdx].m_Frames.nCurrentFrame = 0;
			ulCount++;
		}
		pNode = (KIndexNode*)pNode->GetNext();
	}

	return ulCount;
}

INT KSubWorld::FindNpcFromName(const char* szName)
{
	if (!szName || !szName[0])
		return 0;

	KIndexNode* pNode = NULL;

	INT nResult = 0;

	for (INT i = 0; i < m_nTotalRegion; i++)
	{
		KRegion* pCurRegion = &m_Region[i];
		pNode = (KIndexNode*)pCurRegion->m_NpcList.GetHead();

		while (pNode)
		{
			INT nNpcIdx = pNode->m_nIndex;
			if (!strcmp(Npc[nNpcIdx].Name, szName))
			{
				nResult = nNpcIdx;
				return nResult;
			}
			pNode = (KIndexNode*)pNode->GetNext();
		}
	}

	pNode = (KIndexNode*)m_NoneRegionNpcList.GetHead();

	while (pNode)
	{
		INT nNpcIdx = pNode->m_nIndex;
		if (!strcmp(Npc[nNpcIdx].Name, szName))
		{
			return nNpcIdx;
		}
		pNode = (KIndexNode*)pNode->GetNext();
	}

	return nResult;
}

void KSubWorld::BroadCastRegion(const void* pBuffer, size_t uSize, INT& nMaxCount, INT nRegionIndex, INT nOX, INT nOY)
{
	const POINT POff[8] =
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

	if (nRegionIndex < 0)
	{
		return;
	}

	m_Region[nRegionIndex].BroadCast(pBuffer, uSize, nMaxCount, nOX, nOY);
	for (UINT i = 0; i < 8; i++)
	{
		const INT nConnectRegion = m_Region[nRegionIndex].m_nConnectRegion[i];
		if (nConnectRegion != -1)
		{
			m_Region[nConnectRegion].BroadCast(pBuffer, uSize, nMaxCount, nOX - POff[i].x, nOY - POff[i].y);
		}
	}
}

INT KSubWorld::ExecuteScript(DWORD scriptId, char* functionName, INT nParam1, INT nParam2)
{
	try
	{
		bool bExecuteScriptMistake = true;
		KLuaScript* pScript = (KLuaScript*)g_GetScript(scriptId);
		if (pScript)
		{
			INT nTopIndex = 0;
			pScript->SafeCallBegin(&nTopIndex);

			if (pScript->CallFunction(functionName, 0, "dd", nParam1, nParam2))
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);
		}

		if (bExecuteScriptMistake)
		{
			return FALSE;
		}

		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

INT KSubWorld::ExecuteScript(char* scriptFileName, char* functionName, INT nParam, INT worldIndex)
{
	DWORD dwScriptId = g_FileName2Id(scriptFileName);
	return ExecuteScript(dwScriptId, functionName, nParam, worldIndex);
}
#endif

void KSubWorld::RemoveNpc(INT nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_NPC)
		return;

	_ASSERT(Npc[nIdx].m_Node.m_Ref > 0);

	if (Npc[nIdx].m_Node.m_Ref > 0)
	{
		Npc[nIdx].m_Node.Remove();
		Npc[nIdx].m_Node.Release();
}
#ifndef _SERVER
	Npc[nIdx].RemoveRes();
#endif
}

BOOL KSubWorld::SetTrap(INT nMpsX, INT nMpsY, INT nCellNum, DWORD uTrapScriptId)
{
	INT nRegion, nMapX, nMapY, nOffX, nOffY;
	Mps2Map(nMpsX, nMpsY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion == -1)
		return FALSE;

	return m_Region[nRegion].SetTrap(nMapX, nMapY, nCellNum, uTrapScriptId);
}