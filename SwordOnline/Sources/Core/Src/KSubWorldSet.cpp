#include "KCore.h"
#include "KObjSet.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KNpc.h"
#include "KIniFile.h"
#include "KSubWorldSet.h"
#include "KMissleSet.h"
#include "LuaFuns.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
KSubWorldSet g_SubWorldSet;

KSubWorldSet::KSubWorldSet()
{
	m_nLoopRate = 0;
	m_MapListCount = 0;
	m_sMapListInfo = NULL;
	m_nGameVersion = ITEM_VERSION;
#ifndef _SERVER
	m_dwPing = 0;
#endif
}

KSubWorldSet::~KSubWorldSet()
{
	if (m_sMapListInfo)
	{
		delete[] m_sMapListInfo;
		m_sMapListInfo = NULL;
	}
}

INT KSubWorldSet::SearchWorld(DWORD dwID)
{
	for (INT i = 0; i < MAX_SUBWORLD; i++)
	{
		if ((DWORD)SubWorld[i].m_SubWorldID == dwID)
			return i;
	}
	return -1;
}

BOOL KSubWorldSet::Load(LPSTR szFileName)
{
	KIniFile IniFile;
	char szKeyName[32];
	INT nWorldID, nWorldCount = 0, nSucess = 0, nFail = 0;
	INT nNpcmun = 0, nZnNpcmun = 0, nZnNpcmunA = 0;
	IniFile.Load(szFileName);
	IniFile.GetInteger("Init", "Count", 1, &nWorldCount);
	if (nWorldCount >= MAX_SUBWORLD)
	{
		printf("----load map:Count:%d,MaxCount:%d-----\n", nWorldCount, MAX_SUBWORLD);
		IniFile.Clear();
		return FALSE;
	}

	printf("Loading File Map: %s\n", szFileName);

	for (INT i = 0; i < nWorldCount; ++i)
	{
		sprintf((char*)szKeyName, "World%03d", i);
		IniFile.GetInteger("World", szKeyName, 5000, &nWorldID);
#ifdef _SERVER
		SubWorld[i].m_nIndex = i;
		nNpcmun = SubWorld[i].LoadMap(nWorldID);

		if (nNpcmun >= 0)
		{
			nSucess = nSucess + 1;
			printf("Loaded (%d) Map OK ID: %d,NpcNum:%d\n", i, nWorldID, nNpcmun);
			nZnNpcmun += nNpcmun;
		}
		else
		{
			nFail = nFail + 1;
			printf("Load (%d) Fail Map ID: %d,NpcNum:%d\n", i, nWorldID, nNpcmun);
		}
#endif

	}
	printf("Map Load Sucessed: %d Map,NpcCount:%d\n", nSucess, nZnNpcmun);
	printf("Map Load Failed: %d Map \n", nFail);
	IniFile.Clear();
	return TRUE;
}

BOOL KSubWorldSet::LoadFile()
{
	char		szKeyMap[MAPID_NUM][16] = { "", "City", "Capital", "Cave", "BattleField", "Field", "Others", "Country", "Tong" };

	KIniFile Ini;
	if (Ini.Load(MAPLIST_SETTING_FILE))
	{
		char szKeyName[32];
		char szMapType[12];

		Ini.GetInteger("List", "Count", 0, &m_MapListCount);
		if (m_MapListCount <= 0)
			return FALSE;

		m_sMapListInfo = (MAPLIST_INFO*)new MAPLIST_INFO[m_MapListCount + 1];

		for (INT i = 1; i <= m_MapListCount; i++)
		{
			sprintf(szKeyName, "%d_name", i);
			Ini.GetString("List", szKeyName, "", m_sMapListInfo[i].szName, sizeof(m_sMapListInfo[i].szName));
			m_sMapListInfo[i].nKind = MAPID_UNKNOWN;
			sprintf(szKeyName, "%d_MapType", i);
			Ini.GetString("List", szKeyName, "", szMapType, sizeof(szMapType));
			for (INT j = 0; j < MAPID_NUM; j++)
			{
				if (strcmp(szMapType, szKeyMap[j]) == 0)
					m_sMapListInfo[i].nKind = j;
			}
			sprintf(szKeyName, "%d_NewWorldScript", i);
			Ini.GetString("List", szKeyName, "", m_sMapListInfo[i].szNewWorldScript, sizeof(m_sMapListInfo[i].szNewWorldScript));
		}
	}
	else
		return FALSE;

	return TRUE;
}

INT nActiveRegionCount;

void KSubWorldSet::MainLoop()
{
	m_nLoopRate++;

#ifndef _SERVER
	this->m_cMusic.Play(SubWorld[0].m_SubWorldID, SubWorld[0].m_dwCurrentTime, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode);

#endif
	nActiveRegionCount = 0;
	for (INT i = 0; i < MAX_SUBWORLD; i++)
	{
		SubWorld[i].Activate();
#ifndef _SERVER
		NpcSet.CheckBalance();
#endif
	}

	//	if ((m_nLoopRate % 100) == 0)
	//		printf("Region:%d:%d\n", m_nLoopRate, nActiveRegionCount);
#ifdef _SERVER
	PlayerSet.AutoSave();
#endif
}

void KSubWorldSet::MessageLoop()
{

	for (INT i = 0; i < MAX_SUBWORLD; i++)
	{
		SubWorld[i].MessageLoop();
	}
}

BOOL KSubWorldSet::SendMessage(INT nSubWorldID, DWORD dwMsgType, INT nParam1, INT nParam2, INT nParam3)
{
	KWorldMsgNode* pNode = NULL;

	pNode = new KWorldMsgNode;
	if (!pNode)
		return FALSE;

	pNode->m_dwMsgType = dwMsgType;
	pNode->m_nParam[0] = nParam1;
	pNode->m_nParam[1] = nParam2;
	pNode->m_nParam[2] = nParam3;
	if (pNode->m_dwMsgType == 4001) g_DebugLog("Send !!!!");
	return SubWorld[nSubWorldID].m_WorldMessage.Send(pNode);
}

void KSubWorldSet::Close()
{
	for (INT i = 0; i < MAX_SUBWORLD; i++)
	{
		SubWorld[i].Close();
	}
	NpcSet.RemoveAll();
#ifndef _SERVER
	Player[CLIENT_PLAYER_INDEX].m_ItemList.RemoveAll();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.Release();
	g_Team[0].Release();
	m_cMusic.Stop();
#endif
}
void KSubWorldSet::Offline()
{
	for (INT i = 0; i < MAX_SUBWORLD; i++)
	{
		SubWorld[i].Close();
	}
#ifndef _SERVER
	Player[CLIENT_PLAYER_INDEX].m_ItemList.RemoveAll();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.Release();
	g_Team[0].Release();
	m_cMusic.Stop();
#endif
}

#ifndef _SERVER
void KSubWorldSet::Paint()
{
	SubWorld[0].Paint();
}
#endif

#ifdef _SERVER
void KSubWorldSet::GetRevivalPosFromId(DWORD dwSubWorldId, INT nRevivalId, POINT* pPos)
{
	if (!pPos)
		return;

	KIniFile IniFile;
	char	szKeyName[32];
	char	szSection[32];

	g_SetFilePath(SETTING_PATH);
	IniFile.Load("RevivePos.ini");
	sprintf(szSection, "%d", dwSubWorldId);
	sprintf(szKeyName, "%d", nRevivalId);

	INT nX = 51200;
	INT nY = 102400;
	IniFile.GetInteger2(szSection, szKeyName, &nX, &nY);

	pPos->x = nX;
	pPos->y = nY;
}
#endif
