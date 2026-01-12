#include "KCore.h"
#include "KNpc.h"
#include "KNpcSet.h"
#include "KMissleSet.h"
#include "KObjSet.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
#include "KMissle.h"
#include "KObj.h"
#include "KWorldMsg.h"
#include "KSubWorld.h"
#include "KItem.h" //added later
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/Interface/iServer.h"
#endif
#include "Scene/ObstacleDef.h"
#include "Scene/SceneDataDef.h"
#include "KRegion.h"
#include "MyAssert.H"
#include <vector>
#include <iostream>

#ifndef _SERVER
#include "scene/ksceneplacec.h"
#endif


KRegion::KRegion()
{
	m_nRegionX = 16;
	m_nRegionY = 32;
	m_nIndex = -1;
	m_RegionID = -1;
	m_nActive = 0;
	m_nNpcSyncCounter = 0;
	m_nObjSyncCounter = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	ZeroMemory(m_nConnectRegion, 8 * sizeof(INT));
	//m_pNpcRef = NULL;
	//m_pObjRef = NULL;
	//m_pObstacleRef = NULL;
	//m_pMslRef = NULL;
	memset(m_nConRegionID, -1, sizeof(m_nConRegionID));
	memset(m_pConRegion, 0, sizeof(m_pConRegion));
#ifdef _SERVER
	//memset(m_dwTrap, 0, sizeof(m_dwTrap));
	//memset(m_Obstacle, 0, sizeof(m_Obstacle));
	//m_cObstacle.Clear();
	m_nNearbyPlayerCount = 0;
	m_nNearbyActiveNpcCount = 0;
	m_eActType = emACT_TYPE_NONE;
#endif
}

KRegion::~KRegion()
{

	m_cNpcRef.Clear();
	m_cObjRef.Clear();

	/*if (m_pMslRef)
		delete[] m_pMslRef;
	if (m_pNpcRef)
		delete[] m_pNpcRef;
	if (m_pObjRef)
		delete[] m_pObjRef;
	if (m_pObstacleRef)
		delete[] m_pObstacleRef;*/
}

BOOL KRegion::Init(INT nWidth, INT nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_cNpcRef.Clear();
	m_cObjRef.Clear();

	/*if (!m_pNpcRef)
		m_pNpcRef = new BYTE[nWidth * nHeight];
	if (!m_pNpcRef)
		return FALSE;
	ZeroMemory(m_pNpcRef, nWidth * nHeight);

	if (!m_pObjRef)
		m_pObjRef = new BYTE[nWidth * nHeight];
	if (!m_pObjRef)
		return FALSE;
	ZeroMemory(m_pObjRef, nWidth * nHeight);

	if (!m_pObstacleRef)
		m_pObstacleRef = new BYTE[nWidth * nHeight];
	if (!m_pObstacleRef)
		return FALSE;
	ZeroMemory(m_pObstacleRef, nWidth * nHeight);

	if (!m_pMslRef)
		m_pMslRef = new BYTE[nWidth * nHeight];
	if (!m_pMslRef)
		return FALSE;
	ZeroMemory(m_pMslRef, nWidth * nHeight);*/

	return TRUE;
}

BOOL KRegion::Load(INT nX, INT nY)
{
#ifndef _SERVER
	Close();
#endif

	m_RegionID = MAKELONG(nX, nY);

	m_nRegionX = nX * 512;
	m_nRegionY = nY * 1024;

	// ÏÂ·½
	m_nConRegionID[0] = MAKELONG(nX, nY + 1);
	// ×óÏÂ·½
	m_nConRegionID[1] = MAKELONG(nX - 1, nY + 1);
	// ×ó·½
	m_nConRegionID[2] = MAKELONG(nX - 1, nY);
	// ×óÉÏ·½
	m_nConRegionID[3] = MAKELONG(nX - 1, nY - 1);
	// ÉÏ·½
	m_nConRegionID[4] = MAKELONG(nX, nY - 1);
	// ÓÒÉÏ·½
	m_nConRegionID[5] = MAKELONG(nX + 1, nY - 1);
	// ÓÒ·½
	m_nConRegionID[6] = MAKELONG(nX + 1, nY);
	// ÓÒÏÂ·½
	m_nConRegionID[7] = MAKELONG(nX + 1, nY + 1);

	return TRUE;
}

#ifdef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾region µÄ objectÊý¾Ý£¨°üÀ¨npc¡¢trap¡¢boxµÈ£©
//	×¢Òâ£ºÊ¹ÓÃ´Ëº¯ÊýÖ®Ç°±ØÐë±£Ö¤µ±Ç°Â·¾¶ÊÇ±¾µØÍ¼Â·¾¶
//----------------------------------------------------------------------
INT KRegion::LoadObject(INT nSubWorld, INT nX, INT nY, KMapList* pMapListInfo)
{
	KPakFile cData;
	char szFilePath[MAX_SENTENCE_LENGTH] = { 0 };
	char szFile[MAX_SENTENCE_LENGTH] = { 0 };
	INT nNpcNum;
	// INT         nNpcWnum=0;
	INT nNpcRnum = 0;
	char szCurPath[80] = { 0 };
	// g_GetRootPath(szFile); //根目录
	g_GetFilePath(szCurPath);

	sprintf(szFilePath, "\\%sv_%03d", szCurPath, nY);
	// g_SetFilePath(szFilePath);
	sprintf(szFile, "%s\\%03d_%s", szFilePath, nX, REGION_COMBIN_FILE_NAME_SERVER);
	// printf("地图路径A：%s\n",szFile);
	m_cObstacle.Clear();

	if (cData.Open(szFile))
	{
		//	printf("测试00000：%s \n",szFile);
		DWORD dwHeadSize;
		DWORD dwMaxElemFile = 0;
		KCombinFileSection sElemFile[REGION_ELEM_FILE_COUNT];

		if (cData.Size() < sizeof(DWORD) + sizeof(KCombinFileSection) * REGION_ELEM_FILE_COUNT)
		{
			// ZeroMemory(m_Obstacle, sizeof(m_Obstacle));
			goto gotoCLOSE;
		}
		cData.Read(&dwMaxElemFile, sizeof(DWORD));
		if (dwMaxElemFile > REGION_ELEM_FILE_COUNT)
		{
			cData.Read(sElemFile, sizeof(sElemFile));
			cData.Seek(sizeof(KCombinFileSection) * (dwMaxElemFile - REGION_ELEM_FILE_COUNT), FILE_CURRENT);
		}
		else
		{
			cData.Read(sElemFile, sizeof(sElemFile));
		}
		dwHeadSize = sizeof(DWORD) + sizeof(KCombinFileSection) * dwMaxElemFile;

		// 载入障碍数据
		cData.Seek(dwHeadSize + sElemFile[REGION_OBSTACLE_FILE_INDEX].uOffset, FILE_BEGIN);
		LoadServerObstacle(&cData, sElemFile[REGION_OBSTACLE_FILE_INDEX].uLength);

		// 载入trap信息
		cData.Seek(dwHeadSize + sElemFile[REGION_TRAP_FILE_INDEX].uOffset, FILE_BEGIN);
		//LoadServerTrap(&cData, sElemFile[REGION_TRAP_FILE_INDEX].uLength);

		// 载入npc数据
		cData.Seek(dwHeadSize + sElemFile[REGION_NPC_FILE_INDEX].uOffset, FILE_BEGIN);
		//nNpcRnum = LoadServerNpc(nSubWorld, &cData, sElemFile[REGION_NPC_FILE_INDEX].uLength, pMapListInfo);

		// 载入obj数据
		cData.Seek(dwHeadSize + sElemFile[REGION_OBJ_FILE_INDEX].uOffset, FILE_BEGIN);
		LoadServerObj(nSubWorld, &cData, sElemFile[REGION_OBJ_FILE_INDEX].uLength);

	gotoCLOSE:
		cData.Close();
	}
	else
	{
		KPakFile cObstacle;

		//		g_SetFilePath(szFilePath);
		sprintf(szFile, "%s\\%03d_%s", szFilePath, nX, REGION_OBSTACLE_FILE);
		// printf("地图路径B：%s\n",szFile);
		if (cObstacle.Open(szFile))
		{
			// printf("测试7 \n");
			INT anObstacle[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];

			if (cObstacle.Size() != sizeof(anObstacle))
				// ZeroMemory(anObstacle, sizeof(anObstacle));
				m_cObstacle.Clear();
			else
			{
				cObstacle.Read((LPVOID)anObstacle, sizeof(anObstacle));

				for (INT i = 0; i < REGION_GRID_WIDTH; ++i)
				{
					for (INT j = 0; j < REGION_GRID_HEIGHT; ++j)
					{
						if (anObstacle[i][j])
							m_cObstacle.SetData(i, j, anObstacle[i][j]);
					}
				}
			}

			cObstacle.Close();
		}
		else
		{
			// printf("测试8 \n");
			// ZeroMemory(m_Obstacle, sizeof(m_Obstacle));
			m_cObstacle.Clear();
		}

		KPakFile cTrapData, cNpcData;
		// 载入trap信息
		KTrapFileHead sTrapFileHead;
		KSPTrap sTrapCell;
		INT i, j;

		//		g_SetFilePath(szFilePath);
		sprintf(szFile, "%s\\%03d_%s", szFilePath, nX, REGION_TRAP_FILE);
		//		memset(this->m_dwTrap, 0, sizeof(m_dwTrap));
		m_cTrap.Clear();
		// printf("Map path C: %s\n",szFile);
		if (!cTrapData.Open(szFile))
			goto TRAP_CLOSE;
		if (cTrapData.Size() < sizeof(KTrapFileHead))
			goto TRAP_CLOSE;
		cTrapData.Read(&sTrapFileHead, sizeof(KTrapFileHead));

		if (sTrapFileHead.uNumTrap * sizeof(KSPTrap) + sizeof(KTrapFileHead) != cTrapData.Size())
			goto TRAP_CLOSE;

		for (i = 0; i < sTrapFileHead.uNumTrap; ++i)
		{
			cTrapData.Read(&sTrapCell, sizeof(KSPTrap));

			if (sTrapCell.cY >= REGION_GRID_HEIGHT || sTrapCell.cX + sTrapCell.cNumCell - 1 >= REGION_GRID_WIDTH)
				continue;
			for (j = 0; j < sTrapCell.cNumCell; ++j)
			{
				// m_dwTrap[sTrapCell.cX + j][sTrapCell.cY] = sTrapCell.uTrapId;
				m_cTrap.SetData(sTrapCell.cX + j, sTrapCell.cY, sTrapCell.uTrapId);
			}
		}

	TRAP_CLOSE:
		cTrapData.Close();

		// 载入npc数据
		KNpcFileHead sNpcFileHead;
		KSPNpc sNpcCell;

		//		g_SetFilePath(szFilePath);
		sprintf(szFile, "%s\\%03d_%s", szFilePath, nX, REGION_NPC_FILE_SERVER);
		// printf("Map path D: %s\n",szFile);
		if (!cNpcData.Open(szFile))
			goto NPC_CLOSE;

		if (cNpcData.Size() < sizeof(KNpcFileHead))
			goto NPC_CLOSE;

		cNpcData.Read(&sNpcFileHead, sizeof(KNpcFileHead));

		nNpcRnum = sNpcFileHead.uNumNpc;

		for (i = 0; i < sNpcFileHead.uNumNpc; ++i)
		{
			cNpcData.Read(&sNpcCell, sizeof(KSPNpc) - sizeof(sNpcCell.szScript) - sizeof(sNpcCell.nNDropFile) - sizeof(sNpcCell.nGDropFile));
			if (sNpcCell.nScriptNameLen < sizeof(sNpcCell.szScript))
			{
				cNpcData.Read(sNpcCell.szScript, sNpcCell.nScriptNameLen);
				sNpcCell.szScript[sNpcCell.nScriptNameLen] = 0;
			}
			else
			{
				sNpcCell.szScript[0] = 0;
			}

			if (!strstr(sNpcCell.szScript, ".lua"))
			{ // 如果是乱码的话 不是脚本 就清空
				ZeroMemory(sNpcCell.szScript, sizeof(sNpcCell.szScript));
			}

			if (sNpcCell.nLevel <= 0)
				sNpcCell.nLevel = 1;

			if (pMapListInfo->nNpcSeriesAuto)
			{
				INT nRandVal = g_Random(100);

				sNpcCell.cSeries = 0;
				INT mIsGive = -1;
				if (pMapListInfo->nNpcSeriesEarth > 0)
				{
					if (g_Random(pMapListInfo->nNpcSeriesEarth) > nRandVal)
					{
						sNpcCell.cSeries = 4;
						mIsGive = 4;
					}
				}

				if (pMapListInfo->nNpcSeriesFire > 0)
				{
					if (g_Random(pMapListInfo->nNpcSeriesFire) > nRandVal)
					{
						sNpcCell.cSeries = 3;
						mIsGive = 3;
					}
				}

				if (pMapListInfo->nNpcSeriesMetal > 0)
				{
					if (g_Random(pMapListInfo->nNpcSeriesMetal) > nRandVal)
					{
						sNpcCell.cSeries = 0;
						mIsGive = 0;
					}
				}

				if (pMapListInfo->nNpcSeriesWater > 0)
				{
					if (g_Random(pMapListInfo->nNpcSeriesWater) > nRandVal)
					{
						sNpcCell.cSeries = 2;
						mIsGive = 2;
					}
				}

				if (pMapListInfo->nNpcSeriesWood > 0)
				{
					if (g_Random(pMapListInfo->nNpcSeriesWood) > nRandVal)
					{
						sNpcCell.cSeries = 1;
						mIsGive = 1;
					}
				}

				if (mIsGive == -1)
					sNpcCell.cSeries = GetRandomNumber(0, 4);
			}
			else
				sNpcCell.cSeries = GetRandomNumber(0, 4);

			if (!strstr(sNpcCell.szScript, ".lua"))
			{ // 如果是乱码的话 不是脚本 就清空
				ZeroMemory(sNpcCell.szScript, sizeof(sNpcCell.szScript));
			}

			//------------------------------------------
			if (sNpcCell.shKind != 3 && (!sNpcCell.szScript[0]))
			{ // 对话NPC? 并且 脚本为空
				ZeroMemory(sNpcCell.szScript, sizeof(sNpcCell.szScript));
				sprintf(sNpcCell.szScript, "%s", pMapListInfo->nNewWorldScript); // 脚本赋值
			}

			if (sNpcCell.shKind < 0)
				sNpcCell.shKind = 3;

			if (sNpcCell.shKind == 3)
			{ // 是怪物 就加载爆率
				sprintf(sNpcCell.nNDropFile, "%s", pMapListInfo->nNormalDropRate);
				sprintf(sNpcCell.nGDropFile, "%s", pMapListInfo->nGoldenDropRate);
			}
			else
			{
				ZeroMemory(sNpcCell.nNDropFile, sizeof(sNpcCell.nNDropFile));
				ZeroMemory(sNpcCell.nGDropFile, sizeof(sNpcCell.nGDropFile));
			}

			// NpcSet.AddMapNpc(nSubWorld, &sNpcCell);
		}
	NPC_CLOSE:
		cNpcData.Close();

		// 载入obj数据
		ObjSet.ServerLoadRegionObj(szFilePath, nX, nY, nSubWorld);
	}

	return nNpcRnum;
}
#endif

#ifndef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë¿Í»§¶ËµØÍ¼ÉÏ±¾region µÄ objectÊý¾Ý£¨°üÀ¨npc¡¢boxµÈ£©
//	Èç¹û bLoadNpcFlag == TRUE ÐèÒªÔØÈë clientonly npc else ²»ÔØÈë
//----------------------------------------------------------------------
BOOL KRegion::LoadObject(INT nSubWorld, INT nX, INT nY, char* lpszPath)
{
#ifdef TOOLVERSION
	return TRUE;
#endif
	char	szPath[FILE_NAME_LENGTH], szFile[FILE_NAME_LENGTH];

	if (!lpszPath || !lpszPath[0] || strlen(lpszPath) >= FILE_NAME_LENGTH)
		return FALSE;
	sprintf(szPath, "\\%s\\v_%03d", lpszPath, nY);
	//	g_SetFilePath(szPath);

		// ÔØÈënpcÊý×éÖÐÎ»ÓÚ±¾µØµÄ client npc
	NpcSet.InsertNpcToRegion(this->m_nIndex);

	KPakFile	cData;
	sprintf(szFile, "%s\\%03d_%s", szPath, nX, REGION_COMBIN_FILE_NAME_CLIENT);

	if (cData.Open(szFile))
	{
		DWORD	dwHeadSize;
		DWORD	dwMaxElemFile = 0;
		KCombinFileSection	sElemFile[REGION_ELEM_FILE_COUNT];

		if (cData.Size() < sizeof(DWORD) + sizeof(KCombinFileSection) * REGION_ELEM_FILE_COUNT)
			goto gotoCLOSE;
		cData.Read(&dwMaxElemFile, sizeof(DWORD));
		if (dwMaxElemFile > REGION_ELEM_FILE_COUNT)
		{
			cData.Read(sElemFile, sizeof(sElemFile));
			cData.Seek(sizeof(KCombinFileSection) * (dwMaxElemFile - REGION_ELEM_FILE_COUNT), FILE_CURRENT);
		}
		else
		{
			cData.Read(sElemFile, sizeof(sElemFile));
		}
		dwHeadSize = sizeof(DWORD) + sizeof(KCombinFileSection) * dwMaxElemFile;

		// ÔØÈënpcÊý¾Ý
		cData.Seek(dwHeadSize + sElemFile[REGION_NPC_FILE_INDEX].uOffset, FILE_BEGIN);
		LoadClientNpc(&cData, sElemFile[REGION_NPC_FILE_INDEX].uLength);

		// ÔØÈëobjÊý¾Ý
		cData.Seek(dwHeadSize + sElemFile[REGION_OBJ_FILE_INDEX].uOffset, FILE_BEGIN);
		LoadClientObj(&cData, sElemFile[REGION_OBJ_FILE_INDEX].uLength);

	gotoCLOSE:
		cData.Close();
	}
	else
	{
		// ÔØÈë Client npc Êý¾Ý
		KPakFile		cNpcData;
		KNpcFileHead	sNpcFileHead;
		KSPNpc			sNpcCell;
		DWORD			i;
		KClientNpcID	sTempID;
		INT			nNpcNo;

		// ÔØÈëµØÍ¼ÎÄ¼þÀïµÄ client npc
//		g_SetFilePath(szPath);
		sprintf(szFile, "%s\\%03d_%s", szPath, nX, REGION_NPC_FILE_CLIENT);
		if (!cNpcData.Open(szFile))
			goto NPC_CLOSE;
		if (cNpcData.Size() < sizeof(KNpcFileHead))
			goto NPC_CLOSE;
		cNpcData.Read(&sNpcFileHead, sizeof(KNpcFileHead));
		for (i = 0; i < sNpcFileHead.uNumNpc; i++)
		{
			cNpcData.Read(&sNpcCell, sizeof(KSPNpc) - sizeof(sNpcCell.szScript));
			_ASSERT(sNpcCell.nScriptNameLen < sizeof(sNpcCell.szScript));
			if (sNpcCell.nScriptNameLen > 0)
			{
				cNpcData.Read(sNpcCell.szScript, sNpcCell.nScriptNameLen);
				sNpcCell.szScript[sNpcCell.nScriptNameLen] = 0;
			}
			else
			{
				sNpcCell.szScript[0] = 0;
			}
			sTempID.m_dwRegionID = MAKELONG(nX, nY);
			sTempID.m_nNo = i;
			nNpcNo = NpcSet.SearchClientID(sTempID);
			if (nNpcNo == 0)
			{
				INT nIdx = NpcSet.AddClientNpc(sNpcCell.nTemplateID, nX, nY, sNpcCell.nPositionX, sNpcCell.nPositionY, i);
				if (nIdx > 0)
				{
					Npc[nIdx].m_Kind = sNpcCell.shKind;
					Npc[nIdx].SendCommand(do_stand);
					Npc[nIdx].m_Dir = Npc[nIdx].GetNormalNpcStandDir(sNpcCell.nCurFrame);
				}
			}
		}

	NPC_CLOSE:
		cNpcData.Close();

		// ÔØÈëClientObjectÊý¾Ý
		ObjSet.ClientLoadRegionObj(szPath, nX, nY, nSubWorld, this->m_nIndex);
	}

	return TRUE;
}
#endif

#ifdef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾ region µÄÕÏ°­Êý¾Ý
//----------------------------------------------------------------------
BOOL	KRegion::LoadServerObstacle(KPakFile* pFile, DWORD dwDataSize)
{
	INT anObstacle[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];

	m_cObstacle.Clear();

	if (!pFile || dwDataSize != sizeof(anObstacle))
	{
		// memset(anObstacle, 0, sizeof(anObstacle));
		return FALSE;
	}

	pFile->Read((LPVOID)anObstacle, sizeof(anObstacle));

	for (INT i = 0; i < REGION_GRID_WIDTH; ++i)
	{
		for (INT j = 0; j < REGION_GRID_HEIGHT; ++j)
		{
			if (anObstacle[i][j])
				m_cObstacle.SetData(i, j, anObstacle[i][j]);
		}
	}

	return TRUE;
}
#endif

#ifdef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾ region µÄ trap Êý¾Ý
//----------------------------------------------------------------------
BOOL KRegion::LoadServerTrap(KPakFile* pFile, DWORD dwDataSize)
{
	// memset(m_dwTrap, 0, sizeof(m_dwTrap));    //清空区域的trap数组
	m_cTrap.Clear();

	if (!pFile || dwDataSize < sizeof(KTrapFileHead))
		return FALSE;

	KTrapFileHead sTrapFileHead;
	KSPTrap sTrapCell;
	INT i, j;

	pFile->Read(&sTrapFileHead, sizeof(KTrapFileHead));
	if (sTrapFileHead.uNumTrap * sizeof(KSPTrap) + sizeof(KTrapFileHead) != dwDataSize)
		return FALSE;

	for (i = 0; i < sTrapFileHead.uNumTrap; ++i)
	{
		pFile->Read(&sTrapCell, sizeof(KSPTrap)); // 读入结构信息

		if (sTrapCell.cY >= REGION_GRID_HEIGHT || sTrapCell.cX + sTrapCell.cNumCell - 1 >= REGION_GRID_WIDTH)
			continue;

		for (j = 0; j < sTrapCell.cNumCell; ++j)
		{
			// m_dwTrap[sTrapCell.cX + j][sTrapCell.cY] = sTrapCell.uTrapId;
			m_cTrap.SetData(sTrapCell.cX + j, sTrapCell.cY, sTrapCell.uTrapId);
		}
	}

	return TRUE;
}
#endif

#ifdef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾ region µÄ npc Êý¾Ý
//----------------------------------------------------------------------
INT	KRegion::LoadServerNpc(INT nSubWorld, KPakFile* pFile, DWORD dwDataSize, KMapList* pMapListInfo)
{
	if (!pFile || dwDataSize < sizeof(KNpcFileHead))
		return FALSE;

	KNpcFileHead sNpcFileHead;
	KSPNpc sNpcCell;
	INT nNpcWnum = 0, nNpcRnum = 0;

	pFile->Read(&sNpcFileHead, sizeof(KNpcFileHead)); // 读入结构信息

	/*    nNpcWnum=pMapListInfo->nNpcNum;  //nNpcRnum=sNpcFileHead.uNumNpc;

	if (nNpcWnum<=0 && sNpcFileHead.uNumNpc<=0)
		nNpcRnum=0;
	else  if (nNpcWnum>0 && sNpcFileHead.uNumNpc>0)
		nNpcRnum=1;
	else
	{
		nNpcRnum=0;
	}*/

	for (INT i = 0; i < sNpcFileHead.uNumNpc; i++) // nNpcRnum
	{
		pFile->Read(&sNpcCell, sizeof(KSPNpc) - sizeof(sNpcCell.szScript) - sizeof(sNpcCell.nNDropFile) - sizeof(sNpcCell.nGDropFile)); // 赋值给KSPNpc结构中的szScript变量

		//--------------------------------------------------------
		if (pMapListInfo->nMinLevel > 0 && pMapListInfo->nMaxLevel > 0 && pMapListInfo->nMaxLevel >= pMapListInfo->nMinLevel)
			sNpcCell.nLevel = GetRandomNumber(pMapListInfo->nMinLevel, pMapListInfo->nMaxLevel);

		if (sNpcCell.nLevel <= 0)
			sNpcCell.nLevel = 1;

		if (pMapListInfo->nNpcSeriesAuto)
		{
			INT nRandVal = g_Random(100);

			sNpcCell.cSeries = 0;
			INT mIsGive = -1;
			if (pMapListInfo->nNpcSeriesEarth > 0)
			{
				if (g_Random(pMapListInfo->nNpcSeriesEarth) > nRandVal)
				{
					sNpcCell.cSeries = 4;
					mIsGive = 4;
				}
			}

			if (pMapListInfo->nNpcSeriesFire > 0)
			{
				if (g_Random(pMapListInfo->nNpcSeriesFire) > nRandVal)
				{
					sNpcCell.cSeries = 3;
					mIsGive = 3;
				}
			}

			if (pMapListInfo->nNpcSeriesMetal > 0)
			{
				if (g_Random(pMapListInfo->nNpcSeriesMetal) > nRandVal)
				{
					sNpcCell.cSeries = 0;
					mIsGive = 0;
				}
			}

			if (pMapListInfo->nNpcSeriesWater > 0)
			{
				if (g_Random(pMapListInfo->nNpcSeriesWater) > nRandVal)
				{
					sNpcCell.cSeries = 2;
					mIsGive = 2;
				}
			}

			if (pMapListInfo->nNpcSeriesWood > 0)
			{
				if (g_Random(pMapListInfo->nNpcSeriesWood) > nRandVal)
				{
					sNpcCell.cSeries = 1;
					mIsGive = 1;
				}
			}

			if (mIsGive == -1)
				sNpcCell.cSeries = GetRandomNumber(0, 4);
		}
		else
			sNpcCell.cSeries = GetRandomNumber(0, 4);

		//--------------------------------------------------------

		if (sNpcCell.nScriptNameLen < sizeof(sNpcCell.szScript))
		{
			pFile->Read(sNpcCell.szScript, sNpcCell.nScriptNameLen);
			sNpcCell.szScript[sNpcCell.nScriptNameLen] = 0; // 加个结束符号 防止乱码
		}
		else
		{
			sNpcCell.szScript[0] = 0;
			pFile->Seek(sNpcCell.nScriptNameLen, FILE_CURRENT);
		}

		if (nIsOutScript)
		{ // 读出所有NPC脚本
			char nTrapInfo[256] = { 0 };
			if (sNpcCell.szScript[0])
			{
				if (nMapIndex <= 0)
				{ // 全部
					sprintf(nTrapInfo, "图:%d,名:%s,标:%d/%d,原:%s", pMapListInfo->nSubWorldID, sNpcCell.szName, sNpcCell.nPositionX / 8 / 32, sNpcCell.nPositionY / 16 / 32, sNpcCell.szScript);
					//g_LogFile.gTraceLogFile(nTrapInfo, sizeof(nTrapInfo));
				}
				else
				{
					if (pMapListInfo->nSubWorldID == nMapIndex)
					{ // 某个地图
						sprintf(nTrapInfo, "图:%d,名:%s,标:%d/%d,原:%s", pMapListInfo->nSubWorldID, sNpcCell.szName, sNpcCell.nPositionX / 8 / 32, sNpcCell.nPositionY / 16 / 32, sNpcCell.szScript);
						//g_LogFile.gTraceLogFile(nTrapInfo, sizeof(nTrapInfo));
					}
				}
			}
		}

		if (!strstr(sNpcCell.szScript, ".lua"))
		{ // 如果是乱码的话 不是脚本 就清空
			ZeroMemory(sNpcCell.szScript, sizeof(sNpcCell.szScript));
		}

		//------------------------------------------
		if (sNpcCell.shKind != 3 && (!sNpcCell.szScript[0]))
		{ // 对话NPC? 并且 脚本为空
			ZeroMemory(sNpcCell.szScript, sizeof(sNpcCell.szScript));
			sprintf(sNpcCell.szScript, "%s", pMapListInfo->nNewWorldScript); // 脚本赋值
		}

		if (sNpcCell.shKind < 0)
			sNpcCell.shKind = 3;

		if (sNpcCell.shKind == 3)
		{ // 是怪物 就加载爆率
			sprintf(sNpcCell.nNDropFile, "%s", pMapListInfo->nNormalDropRate);
			sprintf(sNpcCell.nGDropFile, "%s", pMapListInfo->nGoldenDropRate);
		}
		else
		{
			ZeroMemory(sNpcCell.nNDropFile, sizeof(sNpcCell.nNDropFile));
			ZeroMemory(sNpcCell.nGDropFile, sizeof(sNpcCell.nGDropFile));
		}

		if (strstr(pMapListInfo->nMapType, "City")) // 是城市地图
		{
			NpcSet.Add(nSubWorld, &sNpcCell, pMapListInfo);
		}
		else if (strstr(pMapListInfo->nMapType, "Cave")) // 山洞地图
		{
			NpcSet.Add(nSubWorld, &sNpcCell, pMapListInfo);
		}
		else if (strstr(pMapListInfo->nMapType, "Tong")) // 帮派地图
		{
		}
		else if (strstr(pMapListInfo->nMapType, "Country")) // 城镇（新手村）地图
		{
			NpcSet.Add(nSubWorld, &sNpcCell, pMapListInfo); // 刷对话Npc
		}
		else if (strstr(pMapListInfo->nMapType, "Capital")) // 首都地图
		{
			NpcSet.Add(nSubWorld, &sNpcCell, pMapListInfo);
		}
		else if (strstr(pMapListInfo->nMapType, "Field")) // 任务地图
		{
			NpcSet.Add(nSubWorld, &sNpcCell, pMapListInfo); // 刷对话Npc
		}
		else if (strstr(pMapListInfo->nMapType, "Battle")) // 战场地图
		{
		}
		else if (strstr(pMapListInfo->nMapType, "NoNpc")) // 没有NPC
		{
		}
		else
		{
		}
	}
	return sNpcFileHead.uNumNpc;
}
#endif

#ifdef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾ region µÄ obj Êý¾Ý
//----------------------------------------------------------------------
BOOL	KRegion::LoadServerObj(INT nSubWorld, KPakFile* pFile, DWORD dwDataSize)
{
	return ObjSet.ServerLoadRegionObj(nSubWorld, pFile, dwDataSize);
}
#endif

#ifndef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë¿Í»§¶ËµØÍ¼ÉÏ±¾ region µÄ clientonlynpc Êý¾Ý
//----------------------------------------------------------------------
BOOL	KRegion::LoadClientNpc(KPakFile* pFile, DWORD dwDataSize)
{
	if (!pFile || dwDataSize < sizeof(KNpcFileHead))
		return FALSE;

	KNpcFileHead	sNpcFileHead;
	KSPNpc			sNpcCell;
	DWORD			i;
	KClientNpcID	sTempID;
	INT				nNpcNo;

	pFile->Read(&sNpcFileHead, sizeof(KNpcFileHead));
	for (i = 0; i < sNpcFileHead.uNumNpc; i++)
	{
		pFile->Read(&sNpcCell, sizeof(KSPNpc) - sizeof(sNpcCell.szScript));
		if (sNpcCell.nScriptNameLen < sizeof(sNpcCell.szScript))
		{
			pFile->Read(sNpcCell.szScript, sNpcCell.nScriptNameLen);
			sNpcCell.szScript[sNpcCell.nScriptNameLen] = 0;
		}
		else
		{
			sNpcCell.szScript[0] = 0;
			pFile->Seek(sNpcCell.nScriptNameLen, FILE_CURRENT);
		}
		sTempID.m_dwRegionID = m_RegionID;
		sTempID.m_nNo = i;
		nNpcNo = NpcSet.SearchClientID(sTempID);
		if (nNpcNo == 0)
		{
			INT nIdx = NpcSet.AddClientNpc(sNpcCell.nTemplateID, LOWORD(m_RegionID), HIWORD(m_RegionID), sNpcCell.nPositionX, sNpcCell.nPositionY, i);
			if (nIdx > 0)
			{
				Npc[nIdx].m_Kind = sNpcCell.shKind;
				Npc[nIdx].SendCommand(do_stand);
				Npc[nIdx].m_Dir = Npc[nIdx].GetNormalNpcStandDir(sNpcCell.nCurFrame);
			}
		}
	}

	return TRUE;
}
#endif

#ifndef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë¿Í»§¶ËµØÍ¼ÉÏ±¾ region µÄ clientonlyobj Êý¾Ý
//----------------------------------------------------------------------
BOOL	KRegion::LoadClientObj(KPakFile* pFile, DWORD dwDataSize)
{
	return ObjSet.ClientLoadRegionObj(pFile, dwDataSize);
}
#endif

#ifndef _SERVER
//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈëÕÏ°­Êý¾Ý¸øÐ¡µØÍ¼
//----------------------------------------------------------------------
void	KRegion::LoadLittleMapData(INT nX, INT nY, char* lpszPath, BYTE* lpbtObstacle)
{
	if (!lpbtObstacle)
		return;

	char	szPath[FILE_NAME_LENGTH], szFile[FILE_NAME_LENGTH];
	INT		i, j;

	if (!lpszPath || !lpszPath[0] || strlen(lpszPath) >= FILE_NAME_LENGTH)
		return;
	sprintf(szPath, "\\%s\\v_%03d", lpszPath, nY);

	KPakFile	cData;
	LONG		nTempTable[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];	// Map obstacle information table
	ZeroStruct(nTempTable);

	//	g_SetFilePath(szPath);
	sprintf(szFile, "%s\\%03d_%s", szPath, nX, REGION_COMBIN_FILE_NAME_CLIENT);

	if (cData.Open(szFile))
	{
		DWORD	dwHeadSize;
		DWORD	dwMaxElemFile = 0;
		KCombinFileSection	sElemFile[REGION_ELEM_FILE_COUNT];

		if (cData.Size() < sizeof(DWORD) + sizeof(KCombinFileSection) * REGION_ELEM_FILE_COUNT)
		{
			ZeroMemory(nTempTable, sizeof(nTempTable));
		}
		else
		{
			cData.Read(&dwMaxElemFile, sizeof(DWORD));
			if (dwMaxElemFile > REGION_ELEM_FILE_COUNT)
			{
				cData.Read(sElemFile, sizeof(sElemFile));
				cData.Seek(sizeof(KCombinFileSection) * (dwMaxElemFile - REGION_ELEM_FILE_COUNT), FILE_CURRENT);
			}
			else
			{
				cData.Read(sElemFile, sizeof(sElemFile));
			}

			if (sElemFile[REGION_OBSTACLE_FILE_INDEX].uLength == sizeof(nTempTable))
			{
				dwHeadSize = sizeof(DWORD) + sizeof(KCombinFileSection) * dwMaxElemFile;
				cData.Seek(dwHeadSize + sElemFile[REGION_OBSTACLE_FILE_INDEX].uOffset, FILE_BEGIN);
				cData.Read((LPVOID)nTempTable, sizeof(nTempTable));

				for (i = 0; i < REGION_GRID_HEIGHT; ++i)
				{
					for (j = 0; j < REGION_GRID_WIDTH; ++j)
					{
						if ((BYTE)nTempTable[j][i])
							lpbtObstacle[i * REGION_GRID_WIDTH + j] = (BYTE)nTempTable[j][i];
						else
							lpbtObstacle[i * REGION_GRID_WIDTH + j] = 0;
					}
				}
			}
			else
			{
				ZeroMemory(nTempTable, sizeof(nTempTable));
			}
		}

		cData.Close();
	}
	else
	{
		sprintf(szFile, "%03d_%s", nX, REGION_OBSTACLE_FILE);
		if (cData.Open(szFile))
			cData.Read((LPVOID)nTempTable, sizeof(nTempTable));
		else
			ZeroMemory(nTempTable, sizeof(nTempTable));
		cData.Close();
	}

	/*for (i = 0; i < REGION_GRID_HEIGHT; i++)
	{
		for (j = 0; j < REGION_GRID_WIDTH; j++)
		{
			lpbtObstacle[i * REGION_GRID_WIDTH + j] = (BYTE)nTempTable[j][i];
		}
	}*/
	int count = 0; // find way by kinnox;
	for (i = 0; i < REGION_GRID_HEIGHT; i++)
	{
		for (j = 0; j < REGION_GRID_WIDTH; j++)
		{
			lpbtObstacle[i * REGION_GRID_WIDTH + j] = (BYTE)nTempTable[j][i];
			/*g_DebugLog("obs region obstacle %d", i * REGION_GRID_WIDTH + j, nTempTable[j][i]);*/
			// find way by kinnox;
			if ((BYTE)nTempTable[j][i] == 0 || (2 <= (BYTE)nTempTable[j][i] && (BYTE)nTempTable[j][i] <= 5)) {
				count++;
			}
			//
		}
	}
	//g_DebugLog("count Map %d", count);// find way by kinnox;
}
#endif

void KRegion::Activate()
{
	KIndexNode* pNode = NULL;
	KIndexNode* pTmpNode = NULL;
	INT nCounter = 0;

	if (IsActiveAll()) // Npc is active only when there is a player nearby.
	{
		pNode = (KIndexNode*)m_NpcList.GetHead();
		while (pNode)
		{
			pTmpNode = (KIndexNode*)pNode->GetNext();
			INT nNpcIdx = pNode->m_nIndex;
#ifdef _SERVER
			if ((nCounter << 1) == m_nNpcSyncCounter)
				Npc[nNpcIdx].NormalSync();//Send sync signal
			nCounter++;
#endif
			Npc[nNpcIdx].Activate();
			pNode = pTmpNode;
		}
		m_nNpcSyncCounter++;
		if (m_nNpcSyncCounter > m_NpcList.GetNodeCount() * 2)
			m_nNpcSyncCounter = 0;
	}

	nCounter = 0;
	INT nObjIdx = 0;
	// Due to the requirement of timing deletion, obj must be Active and cannot be interrupted for other reasons.
	VEC_LIST::const_iterator it = m_ObjList.begin();
	for (; it != m_ObjList.end(); ++it)
	{
		nObjIdx = *it;

		Object[nObjIdx].Activate();
#ifdef _SERVER
		if ((nCounter >> 1) == m_nObjSyncCounter)
			Object[nObjIdx].SyncState();
		nCounter++;
#endif // #ifdef _SERVER
	}

	m_nObjSyncCounter++;
	if (m_nObjSyncCounter > (INT)m_ObjList.size() * 2)
		m_nObjSyncCounter = 0;

	pNode = (KIndexNode*)m_MissleList.GetHead();
	while (pNode)
	{
		pTmpNode = (KIndexNode*)pNode->GetNext();
		Missle[pNode->m_nIndex].Activate();
		pNode = pTmpNode;
	}
#ifdef _SERVER
	pNode = (KIndexNode*)m_PlayerList.GetHead();
	while (pNode)
	{
		pTmpNode = (KIndexNode*)pNode->GetNext();
		Player[pNode->m_nIndex].Active();
		pNode = pTmpNode;
	}
#endif

#ifndef _SERVER
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex == m_nIndex) // 是Player所在的Region
		Player[CLIENT_PLAYER_INDEX].Active();
#endif
}

void KRegion::AddNpc(INT nIdx)
{
	if (nIdx > 0 && nIdx < MAX_NPC)
	{
		//_ASSERT(Npc[nIdx].m_Node.m_Ref == 0);
		if (Npc[nIdx].m_Node.m_Ref == 0)
		{
			m_NpcList.AddTail(&Npc[nIdx].m_Node);
			Npc[nIdx].m_Node.AddRef();
		}
	}
}

void KRegion::RemoveNpc(INT nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_NPC)
		return;

	if (Npc[nIdx].m_Node.m_Ref <= 0)
		return;

	_ASSERT(Npc[nIdx].m_Node.m_Ref > 0);

	if (Npc[nIdx].m_Node.m_Ref > 0)
	{
		Npc[nIdx].m_Node.Remove();
		Npc[nIdx].m_Node.Release();

#ifndef _SERVER
		//g_DebugLog("KRegion::RemoveNpc Npc[nIdx].RemoveRes()");
		Npc[nIdx].RemoveRes();
#endif
	}
}

void KRegion::AddMissle(INT nId)
{
	if (nId > 0 && nId < MAX_MISSLE)
	{
		_ASSERT(Missle[nId].m_Node.m_Ref == 0);
		if (Missle[nId].m_Node.m_Ref == 0)
		{
			m_MissleList.AddTail(&Missle[nId].m_Node);
			Missle[nId].m_Node.AddRef();
		}
	}
}

void KRegion::RemoveMissle(INT nId)
{
	if (nId > 0 && nId < MAX_MISSLE)
	{
		//		_ASSERT(Missle[nId].m_Node.m_Ref > 0);
		if (Missle[nId].m_Node.m_Ref > 0)
		{
			Missle[nId].m_Node.Remove();
			Missle[nId].m_Node.Release();
		}
	}
}

void KRegion::AddObj(INT nIdx)
{
	m_ObjList.push_back(nIdx);

#ifdef _SERVER
	UpdateActiveType();
#endif

	if (Object[nIdx].m_nMapX >= 0 && Object[nIdx].m_nMapY >= 0)
	{
		AddObjRef(Object[nIdx].m_nMapX, Object[nIdx].m_nMapY);
#ifdef _SERVER
		if (Object[nIdx].m_nObstacleKind > Obstacle_NULL)
			AddObstacle(Object[nIdx].m_nMapX, Object[nIdx].m_nMapY, Object[nIdx].m_nObstacleKind);
#else
		if (Object[nIdx].m_nObstacleKind > Obstacle_NULL)
		{
			INT nMpsX, nMpsY;
			SubWorld[0].Map2Mps(Object[nIdx].m_nRegionIdx, Object[nIdx].m_nMapX, Object[nIdx].m_nMapY, Object[nIdx].m_nOffX, Object[nIdx].m_nOffY, &nMpsX, &nMpsY);
			g_ScenePlace.AddObstacle(nMpsX, nMpsY, Object[nIdx].m_nObstacleKind);
		}
#endif
	}
}

void KRegion::RemoveObj(INT nIdx)
{
	VEC_LIST::iterator it = m_ObjList.begin();
	for (; it != m_ObjList.end(); ++it)
	{
		if (*it == nIdx)
		{
			m_ObjList.erase(it);
			break;
		}
	}

#ifdef _SERVER
	UpdateActiveType();
#endif

	if (Object[nIdx].m_nMapX >= 0 && Object[nIdx].m_nMapY >= 0)
	{
		DecObjRef(Object[nIdx].m_nMapX, Object[nIdx].m_nMapY);
#ifdef _SERVER
		if (Object[nIdx].m_nObstacleKind > Obstacle_NULL)
			ClearObstacle(Object[nIdx].m_nMapX, Object[nIdx].m_nMapY);
#else
		if (Object[nIdx].m_nObstacleKind > Obstacle_NULL)
		{
			INT nMpsX, nMpsY;
			SubWorld[0].Map2Mps(Object[nIdx].m_nRegionIdx, Object[nIdx].m_nMapX, Object[nIdx].m_nMapY, Object[nIdx].m_nOffX, Object[nIdx].m_nOffY, &nMpsX, &nMpsY);
			g_ScenePlace.ClearObstacle(nMpsX, nMpsY);
		}
#endif
	}
}

DWORD KRegion::GetTrap(INT nMapX, INT nMapY)
{
#ifdef _SERVER

	if (nMapX < 0 || nMapY < 0 || nMapX >= REGION_GRID_WIDTH || nMapY >= REGION_GRID_HEIGHT)
	{
		return 0;
	}
	return m_cTrap.GetData(nMapX, nMapY); // m_dwTrap[nMapX][nMapY];

	/*DWORD	GetTrap(INT nMapX, INT nMapY) CONST { return m_cTrap.GetData(nMapX, nMapY); };*/

#else
	return 0;
#endif
}

//TamLTM va cham voi NPC
BYTE KRegion::GetBarrier(INT nMapX, INT nMapY, INT nDx, INT nDy)
{

#ifdef _SERVER	
	LONG lType, lInfo;
	LONG lRet = 0;

	lInfo = m_cObstacle.GetData(nMapX, nMapY); //m_Obstacle[nMapX][nMapY];
	lRet = lInfo & 0x0000000f;
	lType = (lInfo >> 4) & 0x0000000f;

	switch (lType)
	{
	case Obstacle_LT:
		if (nDx + nDy > 32)
			lRet = Obstacle_NULL;
		//TamLTM Debug
		g_DebugLog("KRegion GetBarrier 1");
		break;
	case Obstacle_RT:
		if (nDx < nDy)
			lRet = Obstacle_NULL;
		//TamLTM Debug
		g_DebugLog("KRegion GetBarrier 2");
		break;
	case Obstacle_LB:
		if (nDx > nDy)
			lRet = Obstacle_NULL;
		//TamLTM Debug
		g_DebugLog("KRegion GetBarrier 3");
		break;
	case Obstacle_RB:
		if (nDx + nDy < 32)
			lRet = Obstacle_NULL;
		//TamLTM Debug
		g_DebugLog("KRegion GetBarrier 4");
		break;
	default:
		break;
	}
	if (lRet != Obstacle_NULL)
		return lRet;
	//if (m_pNpcRef)
	{
		/*if (m_pNpcRef[nMapY * m_nWidth + nMapX] > 0)*/
		if (GetNpcRef(nMapX, nMapY))
			return Obstacle_JumpFly;
	}
	//if (m_pObstacleRef)
	//{
	//	if (m_pObstacleRef[nMapY * m_nWidth + nMapX] > 0)
	//		return Obstacle_JumpFly;
	//}
	return Obstacle_NULL;
#else
	//if (m_pNpcRef)
	{
		/*if (m_pNpcRef[nMapY * m_nWidth + nMapX] > 0)*/
		if (GetNpcRef(nMapX, nMapY))
			return Obstacle_JumpFly;
	}
	//if (m_pObstacleRef)
	//{
	//	if (m_pObstacleRef[nMapY * m_nWidth + nMapX] > 0)
	//		return Obstacle_JumpFly;
	//}
	return Obstacle_NULL;
#endif
}

// Get the type of obstacle (grid coordinates)
BYTE KRegion::GetNewBarrier(INT nMapX, INT nMapY, INT nDx, INT nDy, BOOL nIsCheckNpc)
{
#ifdef _SERVER

	if (nMapX < 0 || nMapX >= REGION_GRID_WIDTH || nMapY < 0 || nMapY >= REGION_GRID_HEIGHT)
	{
		return Obstacle_NULL;
	}

	long lType, lInfo;
	long lRet = 0;

	lInfo = m_cObstacle.GetData(nMapX, nMapY); // m_Obstacle[nMapX][nMapY];  //16 32

	lRet = lInfo & 0x0000000f;
	lType = (lInfo >> 4) & 0x0000000f;

	switch (lType)
	{
	case Obstacle_LT: // 左上
		if (nDx + nDy > 32)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_RT: // 右上
		if (nDx < nDy)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_LB: // 左下
		if (nDx > nDy)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_RB: // 右下
		if (nDx + nDy < 32)
			lRet = Obstacle_NULL;
		break;
	default:
		break;
	}

	if (lRet != Obstacle_NULL)
		return lRet;

	if (nIsCheckNpc)
	{
		// if (m_pNpcRef[nMapY * m_nWidth + nMapX] > 0)
		if (GetNpcRef(nMapX, nMapY))
			return Obstacle_JumpFly;
	}

	return Obstacle_NULL;
#else

	if (nMapX < 0 || nMapX >= REGION_GRID_WIDTH || nMapY < 0 || nMapY >= REGION_GRID_HEIGHT)
	{
		return Obstacle_NULL;
	}

	if (nIsCheckNpc)
	{
		// if (m_pNpcRef[nMapY * m_nWidth + nMapX] > 0)
		if (GetNpcRef(nMapX, nMapY))
			return Obstacle_JumpFly;
	}
	return Obstacle_NULL;
#endif
}

BYTE KRegion::GetBarrierNewMin(INT nGridX, INT nGridY, INT nOffX, INT nOffY, BOOL bCheckNpc)
{
#ifdef _SERVER
	//_ASSERT(0 <= nGridX && nGridX < REGION_GRID_WIDTH && 0 <= nGridY && nGridY < REGION_GRID_HEIGHT);
	if (nGridX < 0 || nGridX >= REGION_GRID_WIDTH || nGridY < 0 || nGridY >= REGION_GRID_HEIGHT)
	{
		return Obstacle_NULL;
	}

	long lType;
	long lRet = Obstacle_NULL;

	lRet = /*m_Obstacle[nGridX][nGridY] */ m_cObstacle.GetData(nGridX, nGridY) & 0x0000000f;
	lType = (/*m_Obstacle[nGridX][nGridY]*/ m_cObstacle.GetData(nGridX, nGridY) >> 4) & 0x0000000f;

	if (lRet == Obstacle_NULL)
	{
		if (bCheckNpc)
		{
			// if (m_pNpcRef[nGridY * m_nWidth + nGridX] > 0)
			if (GetNpcRef(nGridX, nGridY))
				return Obstacle_JumpFly;
		}

		return Obstacle_NULL;
	}

	switch (lType)
	{
	case Obstacle_LT:
		if (nOffX + nOffY > 32 * 1024)
			return Obstacle_NULL;
		break;
	case Obstacle_RT:
		if (nOffX < nOffY)
			return Obstacle_NULL;
		break;
	case Obstacle_LB:
		if (nOffX > nOffY)
			return Obstacle_NULL;
		break;
	case Obstacle_RB:
		if (nOffX + nOffY < 32 * 1024)
			return Obstacle_NULL;
		break;
	default:
		break;
	}

	return lRet;

#else
	//_ASSERT(0 <= nGridX && nGridX < REGION_GRID_WIDTH && 0 <= nGridY && nGridY < REGION_GRID_HEIGHT);
	if (nGridX < 0 || nGridX >= REGION_GRID_WIDTH || nGridY < 0 || nGridY >= REGION_GRID_HEIGHT)
	{
		return Obstacle_NULL;
	}

	if (bCheckNpc)
	{
		// if (m_pNpcRef[nGridY * m_nWidth + nGridX] > 0)
		if (GetNpcRef(nGridX, nGridY))
			return Obstacle_JumpFly;
	}
	return Obstacle_NULL;
#endif
}

//----------------------------------------------------------------------------
//	¹¦ÄÜ£º°´ ÏñËØµã×ø±ê * 1024 µÄ¾«¶ÈÅÐ¶ÏÄ³¸öÎ»ÖÃÊÇ·ñÕÏ°­
//	²ÎÊý£ºnGridX nGirdY £º±¾region¸ñ×Ó×ø±ê
//	²ÎÊý£ºnOffX nOffY £º¸ñ×ÓÄÚµÄÆ«ÒÆÁ¿(ÏñËØµã * 1024 ¾«¶È)
//	²ÎÊý£ºbCheckNpc £ºÊÇ·ñÅÐ¶ÏnpcÐÎ³ÉµÄÕÏ°­
//	·µ»ØÖµ£ºÕÏ°­ÀàÐÍ(if ÀàÐÍ == Obstacle_NULL ÎÞÕÏ°­)
// TamLTM va cham voi vat can va rao chan
//----------------------------------------------------------------------------
BYTE	KRegion::GetBarrierMin(INT nGridX, INT nGridY, INT nOffX, INT nOffY, BOOL bCheckNpc)
{
#ifdef _SERVER
	///_ASSERT(0 <= nGridX && nGridX < REGION_GRID_WIDTH && 0 <= nGridY && nGridY < REGION_GRID_HEIGHT);

	if (nGridX < 0 || nGridX >= REGION_GRID_WIDTH || nGridY < 0 || nGridY >= REGION_GRID_HEIGHT)
	{
		return Obstacle_NULL;
	}
	long lType;
	long lRet = Obstacle_NULL;

	lRet = /*m_Obstacle[nGridX][nGridY]*/ m_cObstacle.GetData(nGridX, nGridY) & 0x0000000f;
	lType = (/*m_Obstacle[nGridX][nGridY]*/ m_cObstacle.GetData(nGridX, nGridY) >> 4) & 0x0000000f;

	if (lRet == Obstacle_NULL)
	{
		if (bCheckNpc)
		{
			// if (m_pNpcRef[nGridY * m_nWidth + nGridX] > 0)
			if (GetNpcRef(nGridX, nGridY))
				return Obstacle_JumpFly; // 是否有NPC
		}

		return Obstacle_NULL;
	}

	switch (lType)
	{
	case Obstacle_LT:
		if (nOffX + nOffY > 32 * 1024)
			return Obstacle_NULL;
		break;
	case Obstacle_RT:
		if (nOffX < nOffY)
			return Obstacle_NULL;
		break;
	case Obstacle_LB:
		if (nOffX > nOffY)
			return Obstacle_NULL;
		break;
	case Obstacle_RB:
		if (nOffX + nOffY < 32 * 1024)
			return Obstacle_NULL;
		break;
	default:
		break;
	}

	return lRet;

#else
	//_ASSERT(0 <= nGridX && nGridX < REGION_GRID_WIDTH && 0 <= nGridY && nGridY < REGION_GRID_HEIGHT);
	if (nGridX < 0 || nGridX >= REGION_GRID_WIDTH || nGridY < 0 || nGridY >= REGION_GRID_HEIGHT)
	{
		return Obstacle_NULL;
	}

	if (bCheckNpc)
	{
		// if (m_pNpcRef[nGridY * m_nWidth + nGridX] > 0)
		if (GetNpcRef(nGridX, nGridY))
			return Obstacle_JumpFly;
	}
	return Obstacle_NULL;
#endif
}
/*
INT KRegion::GetRef(INT nMapX, INT nMapY, MOVE_OBJ_KIND nType)
{
	INT nRet = 0;
	if (nMapX >= m_nWidth || nMapY >= m_nHeight)
		return 0;

	switch (nType)
	{
	case obj_npc:
		nRet = (INT)m_pNpcRef[nMapY * m_nWidth + nMapX];
		break;
	case obj_object:
		nRet = (INT)m_pObjRef[nMapY * m_nWidth + nMapX];
		break;
	case obj_obstacle:
		nRet = (INT)m_pObstacleRef[nMapY * m_nWidth + nMapX];
		break;
	case obj_missle:
		nRet = (INT)m_pMslRef[nMapY * m_nWidth + nMapX];
		break;
	default:
		break;
	}
	return nRet;
}

BOOL KRegion::AddRef(INT nMapX, INT nMapY, MOVE_OBJ_KIND nType)
{
	//#ifdef _SERVER
	//	std::lock_guard<std::mutex> lock(mutex);
	//#endif
		// Kiểm tra xem con trỏ this có null không
	if (this->m_nIndex <= 0) {
		// Xử lý khi con trỏ this là null
		return FALSE;
	}

	// Kiểm tra xem con trỏ this trỏ đến một vùng nhớ hợp lệ không
	try {
		INT temp = this->m_nIndex;

		if (m_nWidth <= 0 && m_nHeight <= 0)
			return FALSE;
	}
	catch (...) {
		// Xử lý khi xảy ra lỗi khi đọc this->m_nIndex
		return FALSE;
	}

	BYTE* pBuffer = nullptr;
	INT nRef = 0;

	// Kiểm tra giá trị nMapX và nMapY
	if (nMapX < 0 || nMapX >= m_nWidth || nMapY < 0 || nMapY >= m_nHeight)
		return FALSE;

	// Xác định pBuffer dựa trên giá trị nType
	switch (nType)
	{
	case obj_npc:
		pBuffer = m_pNpcRef;
		break;
	case obj_object:
		pBuffer = m_pObjRef;
		break;
	case obj_obstacle:
		pBuffer = m_pObstacleRef;
		break;
	case obj_missle:
		pBuffer = m_pObjRef;
		break;
	default:
		return FALSE; // Giá trị nType không hợp lệ
	}

	// Kiểm tra giá trị của pBuffer
	if (!pBuffer)
		return FALSE;

	// Kiểm tra giá trị của nRef
	INT index = nMapY * m_nWidth + nMapX;
	if (index <= 0 || index >= m_nWidth * m_nHeight)
		return FALSE;

	nRef = static_cast<INT>(pBuffer[index]);

	// Kiểm tra giá trị của nRef
	if (nRef && (nRef >= 255 || nRef < 0))
		return FALSE;
	else
	{
		pBuffer[index]++;
		return TRUE;
	}
}

BOOL KRegion::DecRef(INT nMapX, INT nMapY, MOVE_OBJ_KIND nType)
{
	//#ifdef _SERVER
	//	std::lock_guard<std::mutex> lock(mutex);
	//#endif
		// Kiểm tra xem con trỏ this có null không
	if (!this) {
		// Xử lý khi con trỏ this là null
		return FALSE;
	}

	// Kiểm tra xem con trỏ this trỏ đến một vùng nhớ hợp lệ không
	try {
		INT temp = this->m_nIndex;

		if (m_nWidth <= 0 && m_nHeight <= 0)
			return FALSE;
	}
	catch (...) {
		// Xử lý khi xảy ra lỗi khi đọc this->m_nIndex
		return FALSE;
	}

	BYTE* pBuffer = NULL;
	INT nRef = 0;

	if (nMapX >= m_nWidth || nMapY >= m_nHeight)
		return FALSE;

	switch (nType)
	{
	case obj_npc:
		pBuffer = m_pNpcRef;
		break;
	case obj_object:
		pBuffer = m_pObjRef;
		break;
	case obj_obstacle:
		pBuffer = m_pObstacleRef;
		break;
	case obj_missle:
		pBuffer = m_pObjRef;
		break;
	default:
		break;
	}

	INT index = nMapY * m_nWidth + nMapX;

	if (index <= 0 || index >= m_nWidth * m_nHeight)
		return FALSE;

	if (pBuffer && pBuffer[index])
	{
		nRef = (INT)pBuffer[index];
		if (nRef <= 0 || nRef > 255)
		{
			//			_ASSERT(0);
			return FALSE;
		}
		else
		{
			pBuffer[index]--;
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
}*/

BOOL KRegion::AddPlayer(INT nIdx)
{
	if (nIdx > 0 && nIdx < MAX_PLAYER)
	{
		_ASSERT(Player[nIdx].m_Node.m_Ref == 0);
		if (Player[nIdx].m_Node.m_Ref == 0)
		{
			m_PlayerList.AddTail(&Player[nIdx].m_Node);
			Player[nIdx].m_Node.m_Ref = 0;
			Player[nIdx].m_Node.AddRef();

#ifdef _SERVER
			AddNearbyPlayer(1); // Active count
#endif							// #ifdef _SERVER

			return TRUE;
		}
	}
	return FALSE;
}

BOOL KRegion::RemovePlayer(INT nIdx)
{
	if (nIdx > 0 && nIdx < MAX_PLAYER)
	{
		if (Player[nIdx].m_Node.m_Ref > 0)
		{
			Player[nIdx].m_Node.Remove();
			Player[nIdx].m_Node.Release();
			Player[nIdx].m_Node.m_Ref = 0;
#ifdef _SERVER
			AddNearbyPlayer(-1); // 减Active计数
#endif						 // #ifdef _SERVER
			return TRUE;
		}
	}
	return FALSE;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÑ°ÕÒ±¾ÇøÓòÄÚÊÇ·ñÓÐÄ³¸öÖ¸¶¨ id µÄ npc
//-------------------------------------------------------------------------
INT		KRegion::SearchNpc(DWORD dwNpcID)
{
	KIndexNode* pNode = NULL;

	pNode = (KIndexNode*)m_NpcList.GetHead();
	while (pNode)
	{
		if (Npc[pNode->m_nIndex].m_dwID == dwNpcID)
			return pNode->m_nIndex;
		pNode = (KIndexNode*)pNode->GetNext();
	}

	return 0;
}

INT		KRegion::SearchNpcSettingIdx(INT  nNpcSettingIdx)
{
	KIndexNode* pNode = NULL;

	pNode = (KIndexNode*)m_NpcList.GetHead();
	while (pNode)
	{
		if (Npc[pNode->m_nIndex].m_NpcSettingIdx == nNpcSettingIdx)
			return pNode->m_nIndex;
		pNode = (KIndexNode*)pNode->GetNext();
	}

	return 0;
}

INT		KRegion::SearchNpcName(const char* szName)
{
	KIndexNode* pNode = NULL;

	pNode = (KIndexNode*)m_NpcList.GetHead();
	while (pNode)
	{
		if (strcmp(Npc[pNode->m_nIndex].Name, szName) == 0)
			return pNode->m_nIndex;
		pNode = (KIndexNode*)pNode->GetNext();
	}

	return 0;
}

INT KRegion::FindNpcNear(INT nX1, INT nY1)
{
	KIndexNode* pNode = NULL;

	pNode = (KIndexNode*)m_NpcList.GetHead();

	while (pNode)
	{
		INT nX2, nY2;
		Npc[pNode->m_nIndex].GetMpsPos(&nX2, &nY2);

		if (Npc[pNode->m_nIndex].m_dwID != Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
			return pNode->m_nIndex;

		pNode = (KIndexNode*)pNode->GetNext();
	}
	return 0;
}

INT KRegion::FindObject(INT nMapX, INT nMapY, bool bAutoFind)
{
	VEC_LIST::iterator it = m_ObjList.begin();
	for (; it != m_ObjList.end(); ++it)
	{
		if (Object[*it].m_nMapX == nMapX && Object[*it].m_nMapY == nMapY)
		{
			return *it;
		}
	}
	return 0;
}

INT KRegion::FindObjectNear(INT nX1, INT nY1)
{
	VEC_LIST::iterator it = m_ObjList.begin();
	for (; it != m_ObjList.end(); ++it)
	{
		INT nX2, nY2;
		Object[Object[*it].m_nIndex].GetMpsPos(&nX2, &nY2);
		if ((nX1 - nX2) * (nX1 - nX2) + (nY1 - nY2) * (nY1 - nY2) < PLAYER_PICKUP_CLIENT_DISTANCE * PLAYER_PICKUP_CLIENT_DISTANCE)
		{
			return *it;
		}
	}

	return 0;
}

INT KRegion::FindObject(INT nObjID)
{
	VEC_LIST::iterator it = m_ObjList.begin();
	for (; it != m_ObjList.end(); ++it)
	{
		if (Object[*it].m_nID == nObjID)
		{
			return *it;
		}
	}
	return 0;
}

#ifdef	_SERVER

BOOL KRegion::AddNearbyPlayer(INT nAdd, BOOL bConRegion /* = TRUE */)
{
	m_nNearbyPlayerCount += nAdd;

	// Temporarily added, check why m_nNearbyPlayerCount is less than 0
	// The problem is very complicated. Let’s do a temporary repair first.
	if (m_nNearbyPlayerCount < 0)
	{
		// Temporarily fix this bug
		// If there is no one left, the NPC is 0?
		// m_nNearbyActiveNpcCount = 0;
		m_nNearbyPlayerCount = 0;
	}

	if (bConRegion)
	{
		for (INT i = 0; i < 8; i++)
		{
			if (m_pConRegion[i])
				m_pConRegion[i]->AddNearbyPlayer(nAdd, FALSE);
		}
	}

	return UpdateActiveType();
}

BOOL KRegion::AddNearByNpc(INT nAdd, BOOL bConRegion /* = TRUE*/)
{
	m_nNearbyActiveNpcCount += nAdd;
	// 临时加的，查为什么m_nNearbyPlayerCount会小于0
	// 问题很复杂，先做临时修复操作咯
	// if (m_nNearbyPlayerCount < 0)
	if (m_nNearbyActiveNpcCount < 0)
	{
		// 临时修复此bug
		m_nNearbyActiveNpcCount = 0;
	}

	if (bConRegion)
	{
		for (INT i = 0; i < 8; i++)
		{
			if (m_pConRegion[i])
				m_pConRegion[i]->AddNearByNpc(nAdd, FALSE);
		}
	}

	return UpdateActiveType();
}

BOOL KRegion::UpdateActiveType()
{
	BOOL bResult = FALSE;
	_KEACT_TYPE eActType = emACT_TYPE_NONE;

	if (m_nNearbyPlayerCount > 0 || m_nNearbyActiveNpcCount > 0)
		eActType = emACT_TYPE_ALL;
	else if (m_ObjList.size() > 0 || !m_MissleList.IsEmpty())
		eActType = emACT_TYPE_WAIT;
	else
		eActType = emACT_TYPE_NONE;

	//KG_PROCESS_SUCCESS(eActType == m_eActType);

	if (eActType == emACT_TYPE_ALL)
	{
		// 刚开始完全活动
		// TODO:liuchang 需要修改NpcAi
	}

	if (m_eActType == emACT_TYPE_NONE)
	{
		// 刚开始有活动
		SubWorld[m_nSubMapidx].AddActiveRegion(m_nIndex);
	}
	else if (eActType == emACT_TYPE_NONE)
	{
		// 刚开始完全无活动
		SubWorld[m_nSubMapidx].RemoveActiveRegion(m_nIndex);
	}

	m_eActType = eActType;

Exit1:
	bResult = TRUE;
	// Exit0:
	return bResult;
}

BOOL KRegion::AddObstacle(INT nX, INT nY, INT nObstacleKind)
{
	BOOL bResult = FALSE;

	if (nObstacleKind >= 0 && nObstacleKind < Obstacle_Kind_Num)
	{
		m_cObstacle.SetData(nX, nY, (Obstacle_Full << 4) + nObstacleKind);
		bResult = TRUE;
	}

Exit0:
	return bResult;
}

BOOL KRegion::ClearObstacle(INT nX, INT nY)
{
	return m_cObstacle.SetData(nX, nY, Obstacle_NULL);
}

void KRegion::SendSyncData(INT nClient)
{
	KIndexNode* pNode = NULL;

	pNode = (KIndexNode*)m_NpcList.GetHead();
	while (pNode)
	{
		Npc[pNode->m_nIndex].SendSyncData(nClient);
		pNode = (KIndexNode*)pNode->GetNext();
	}

	VEC_LIST::iterator it = m_ObjList.begin();
	for (; it != m_ObjList.end(); ++it)
	{
		Object[*it].SyncAdd(nClient);
	}
}

void KRegion::BroadCast(const void* pBuffer, DWORD dwSize, INT& nMaxCount, INT nOX, INT nOY)
{
#define	MAX_SYNC_RANGE	25
	KIndexNode* pNode = NULL;

	pNode = (KIndexNode*)m_PlayerList.GetHead();
	while (pNode && nMaxCount > 0)
	{
		_ASSERT(pNode->m_nIndex > 0 && pNode->m_nIndex < MAX_PLAYER);
		INT nPlayerIndex = pNode->m_nIndex;
		INT nNpcIndex = Player[nPlayerIndex].m_nIndex;
		INT nTX = Npc[nNpcIndex].m_MapX;
		INT nTY = Npc[nNpcIndex].m_MapY;
		INT nDX = nTX - nOX;
		INT nDY = nTY - nOY;
		if (Player[pNode->m_nIndex].m_nNetConnectIdx >= 0
			&& (nDX * nDX + nDY * nDY) <= MAX_SYNC_RANGE * MAX_SYNC_RANGE
			&& Player[pNode->m_nIndex].m_bSleepMode == FALSE)
			g_pServer->PackDataToClient(Player[pNode->m_nIndex].m_nNetConnectIdx, (BYTE*)pBuffer, dwSize);
		if (nMaxCount)
			nMaxCount--;
		pNode = (KIndexNode*)pNode->GetNext();
	}
}

//---------------------------------------------------------------------
// ²éÕÒ¸ÃRegionÖÐNpcIDÎªdwIdµÄPlayerË÷Òý
//---------------------------------------------------------------------
INT KRegion::FindPlayer(DWORD dwId)
{
	KIndexNode* pNode = NULL;
	INT	nRet = -1;

	pNode = (KIndexNode*)m_PlayerList.GetHead();
	while (pNode)
	{
		if (Npc[Player[pNode->m_nIndex].m_nIndex].m_dwID == dwId)
		{
			nRet = pNode->m_nIndex;
			break;
		}
		pNode = (KIndexNode*)pNode->GetNext();
	}
	return nRet;
}

#endif

BOOL KRegion::SetTrap(INT nMapX, INT nMapY, INT nCellNum, DWORD uTrapScriptId)
{
	//m_dwTrap[nMapX][nMapY] = dwTrapId;
#ifdef _SERVER
	if (nMapX < 0 || nMapY < 0 || nMapX >= REGION_GRID_WIDTH || nMapY >= REGION_GRID_HEIGHT)
	{
		return FALSE;
	}

	INT i;
	for (i = 0; i < nCellNum; ++i)
	{
		if (nMapX + i >= REGION_GRID_WIDTH)
			break;

		// m_dwTrap[nMapX+i][nMapY]=uTrapScriptId;
		m_cTrap.SetData(nMapX + i, nMapY, uTrapScriptId);
	}

	/*
		for (i = 0; i < sTrapFileHead.uNumTrap; ++i)  //这个区域有多少个TRAP 点
		{
			pFile->Read(&sTrapCell, sizeof(KSPTrap));
			if (sTrapCell.cY >= REGION_GRID_HEIGHT || sTrapCell.cX + sTrapCell.cNumCell - 1 >= REGION_GRID_WIDTH)
				continue;  //下一个Trap点

			for (j = 0; j < sTrapCell.cNumCell; ++j)  //TRAP的宽度 和高度
			{
				m_dwTrap[sTrapCell.cX + j][sTrapCell.cY] = sTrapCell.uTrapId;
			}
		}

	*/
	// if (i+1==nCellNum && i>=0)  //完全设置
	return TRUE;
	// else                //其他情况 失败
	//	 return FALSE;

#else
	return FALSE;
#endif
}

#ifndef _SERVER
void KRegion::Paint()
{
	KIndexNode* pNode = NULL;

	pNode = (KIndexNode*)m_NpcList.GetHead();
	while (pNode)
	{
		Npc[pNode->m_nIndex].Paint();
		pNode = (KIndexNode*)pNode->GetNext();
	}
	pNode = (KIndexNode*)m_MissleList.GetHead();
	while (pNode)
	{
		Missle[pNode->m_nIndex].Paint();
		pNode = (KIndexNode*)pNode->GetNext();
	}

	//pNode = (KIndexNode*)m_ObjList.GetHead();
	//while (pNode)
	//{
	//	//Object[pNode->m_nIndex].Paint();		//need add -spe
	//	pNode = (KIndexNode*)pNode->GetNext();
	//}
}
#endif

KRegion::VEC_LIST::iterator KRegion::GetObjNode(INT nIdx)
{
	VEC_LIST::iterator it = m_ObjList.begin();
	for (; it != m_ObjList.end(); ++it)
	{
		if (*it == nIdx)
			break;
	}
	return it;
}

void KRegion::Close()		// Clear several linked lists in the Region (the content pointed to has not been cleared)
{
	KIndexNode* pNode = NULL;
	KIndexNode* pTempNode = NULL;

	if (!m_nWidth || !m_nHeight)
		return;

	m_cNpcRef.Clear();
	m_cObjRef.Clear();

#ifdef _SERVER
	m_cObstacle.Clear();
	m_cTrap.Clear();
#endif

	/*if (m_pNpcRef)
	{
		ZeroMemory(m_pNpcRef, m_nWidth * m_nHeight);
	}
	if (m_pObjRef)
	{
		ZeroMemory(m_pObjRef, m_nWidth * m_nHeight);
	}
	if (m_pObstacleRef)
	{
		ZeroMemory(m_pObstacleRef, m_nWidth * m_nHeight);
	}
	if (m_pMslRef)
	{
		ZeroMemory(m_pMslRef, m_nWidth * m_nHeight);
	}*/
	pNode = (KIndexNode*)m_NpcList.GetHead();

	while (pNode)
	{
		pTempNode = pNode;
		pNode = (KIndexNode*)pNode->GetNext();
#ifdef _SERVER
		if (pTempNode->m_nIndex > 0 && pTempNode->m_nIndex < MAX_NPC)
		{
#else
		if (pTempNode->m_nIndex > 0)
		{
#endif
			RemoveNpc(pTempNode->m_nIndex);
			// NpcSet.Remove(pTempNode->m_nIndex,FALSE); //It was originally logged out and does not need to be synchronized to the client.
			Npc[pTempNode->m_nIndex].m_RegionIndex = -1;
		}

		/*#ifdef _SERVER
				if (pTempNode)
				{
					delete pTempNode;   //原来是注销掉的
					pTempNode=NULL;
				}
		#endif*/
	}
	KIndexNode* pMissNode = NULL;
	KIndexNode* pMissTempNode = NULL;

	pMissNode = (KIndexNode*)m_MissleList.GetHead();

	while (pMissNode)
	{
		pMissTempNode = pMissNode;
		pMissNode = (KIndexNode*)pMissNode->GetNext();
#ifdef _SERVER
		if (pMissTempNode->m_nIndex > 0 && pMissTempNode->m_nIndex < MAX_MISSLE)
		{
#else
		if (pMissTempNode->m_nIndex > 0)
		{
#endif
			MissleSet.Remove(pMissTempNode->m_nIndex);
			Missle[pMissTempNode->m_nIndex].m_nRegionId = -1;
		}

		pMissTempNode->Remove();
		pMissTempNode->Release();

		/*#ifdef _SERVER
				if (pMissTempNode)
				{
				  delete pMissTempNode;		 	    //原来是注销掉的
				  pMissTempNode=NULL;				//原来是注销掉的
				}
		#endif*/
	}

	// 同时清除 clientonly 类型的 obj ---- zroc add
	/*	VEC_LIST::iterator it = m_ObjList.begin();
		for(; it != m_ObjList.end(); ++it)
		{
	#ifndef _SERVER
			//Object[*it].Remove(FALSE);
			Object[*it].Remove(FALSE,TRUE);
	#endif
			 ObjSet.Remove(*it);
			//g_cObjMgr.Remove(*it);        // TODO:
			Object[*it].m_nRegionIdx = -1;
		}
		m_ObjList.clear();*/

		// Also clear obj of clientonly type ----zroc add
	VEC_LIST::iterator it = m_ObjList.begin();
	for (; it != m_ObjList.end(); ++it)
	{
#ifndef _SERVER
		Object[*it].Remove(FALSE);
#endif
		ObjSet.Remove(*it); // TODO:
		Object[*it].m_nRegionIdx = -1;
	}
	m_ObjList.clear();

	KIndexNode* ppNode = NULL;
	KIndexNode* ppTempNode = NULL;

	ppNode = (KIndexNode*)m_PlayerList.GetHead();
	while (ppNode)
	{
		ppTempNode = ppNode;
		ppNode = (KIndexNode*)ppNode->GetNext();

		ppTempNode->Remove();
		ppTempNode->Release();

		/*if (ppTempNode)
		{
		  delete ppTempNode;	                    //原来是注销掉的
		  ppTempNode=NULL;
		}*/
	}

	m_RegionID = -1;
	m_nIndex = -1;
	m_nActive = 0;
	memset(m_nConnectRegion, -1, sizeof(m_nConnectRegion));
	memset(m_nConRegionID, -1, sizeof(m_nConRegionID));
}