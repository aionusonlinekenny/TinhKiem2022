//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCore.cpp
// Date:	2000.08.08
// Code:	Daphnis Wang
// Desc:	Core class
//---------------------------------------------------------------------------
#include "KCore.h"
#include "KEngine.h"
#include "KFilePath.h"
#ifndef _SERVER
#include "KNpcResList.h"
#include "KBmpFile.h"
#include "ImgRef.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "../KJXPathFinder.h"// find way by kinnox;
#endif
#include "KItemChangeRes.h"
#include "KNpcSet.h"
#include "KTabFile.h"
#include "KSkills.h"
#include "KPlayerSet.h"
#include "KPlayerTeam.h"
#include "KPlayerChatRoom.h"
#include "KMissleSet.h"
#include "KTongData.h"
#include "KFaction.h"
#include "KGameData.h"
#include "KMath.h"
#include "time.h"
#include "KPlayerTask.h"
#include "KSubWorldSet.h"
#include "KItemGenerator.h"
#include "KObjSet.h"
#include "KItemSet.h"
#include "KNpc.h"
#include "KNpcTemplate.h"
#include "CoreUseNameDef.h"
#include "GameDataDef.h"
#include "KBuySell.h"
#include "KSortScript.h"
#include "LuaFuns.h"

#pragma warning (disable: 4512)
#pragma warning (disable: 4786)

#ifdef _SERVER
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif
//#include <KPakList.h>
#endif

#ifdef _SERVER
extern INT g_WayPointPriceUnit;	//WayPoint±í¸ñÖÐ¼Û¸ñµÄµ¥Î»Á¿£¬WayPoint¼Û¸ñ = µ¥Î»Á¿ * ±í¸ñÊýÖµ
extern INT g_StationPriceUnit;	//Station±í¸ñÖÐ¼Û¸ñµÄµ¥Î»Á¿£¬Station¼Û¸ñ = µ¥Î»Á¿ * ±í¸ñÊýÖµ
extern INT g_DockPriceUnit;
//KPakList g_PakList;
#endif


#ifndef _SERVER
#define CLIENTWEAPONSKILL_TABFILE		"\\settings\\ClientWeaponSkill.txt"	
KTabFile g_ClientWeaponSkillTabFile;

#define ADJUSTCOLOR_TABFILE				"\\settings\\AdjustColor.txt"	
UINT		InitAdjustColorTab();

UINT* g_pAdjustColorTab = NULL;
UINT g_ulAdjustColorCount = 0;
extern KJXPathFinder g_JXPathFinder; // find way by kinnox;
#endif

BOOL	InitTaskSetting();

#ifndef _SERVER
#include "KMagicDesc.h"
#include "Scene/KScenePlaceC.h"
BOOL g_bUISelIntelActiveWithServer = FALSE;//µ±Ç°Ñ¡Ôñ¿òÊÇ·ñÓë·þÎñÆ÷¶Ë½»»¥
BOOL g_bUISpeakActiveWithServer = FALSE;
INT	g_bUISelLastSelCount = 0;
extern KTabFile g_StringResourseTabFile;
KTabFile g_RankTabSetting;
#endif

//ÊÇ·ñ½«½Å±¾³ö´íÐÅÏ¢Êä³öµ½ÎÄ¼þ¹©µ÷ÊÔ
#ifdef _SERVER
IServer* g_pServer;
IClient* g_pTongClient;
IClient* g_pDataClient;
extern KTabFile g_EventItemTab;
extern KTabFile g_QuestItemTab;
extern KTabFile g_TownPortalTab;
extern KTabFile g_MagicScriptTab;
#else
IClient* g_pClient;
BOOL	 g_bPingReply;
#endif
//---------------------------------------------------------------------------
#ifdef TOOLVERSION
KSpriteCache	g_SpriteCache;
#endif
KTabFile			g_OrdinSkillsSetting, g_MisslesSetting, g_PlayerTitle, g_NpcChangeName, g_TaskChangeName, g_ItemChangeName, g_SkillChangeName;
KTabFile			g_SkillLevelSetting, g_FsJinMai, g_ForbitMap, g_ObjChangeName;
KTabFile			g_NpcSetting, g_GMSetting, g_ForbitWaiGua, g_AccoutSet, g_ReBornSetting, g_PlayerVipSetting;
KTabFile			g_CompEquip, g_CompAtlas, g_GetMsg; // 记录Npc人物类型文件
KIniFile			g_TaskSetConfig, g_NpcMapDropRate, g_MagicBaoshiDesc, g_TaskLoop;
//#ifndef _SERVER
KTabFile			g_NpcKindFile; //¼ÇÂ¼NpcÈËÎïÀàÐÍÎÄ¼þ
//#endif
INT				g_nMeleeWeaponSkill[MAX_MELEEWEAPON_PARTICULARTYPE_NUM];
INT				g_nRangeWeaponSkill[MAX_RANGEWEAPON_PARTICULARTYPE_NUM];
INT				g_nHandSkill;

KTabFile 			g_MaskChangeRes;
KIniFile 			g_GameSetting;
KIniFile 			g_MapTraffic;

#ifdef _SERVER
INT				g_ExpRate = 1;
INT				g_MoneyRate = 1;
INT				g_SkillExpRate = 1;
INT				g_Skill120ExpRate = 1;
KLuaScript* g_pStartScript = NULL;

KSG_LogFile g_Log;

INT nIsOutScript;
INT nMapIndex;
#endif

#ifndef	_SERVER
KSoundCache		g_SoundCache;
KMusic* g_pMusic = NULL;
#endif

KLuaScript* g_pNpcLevelScript = NULL;
KLuaScript g_WorldScript;

void g_InitProtocol();

//---------------------------------------------------------------------------
CORE_API void g_InitCore()
{
	srand((UINT)time(NULL));

	time_t ltime;
	time(&ltime);

#ifdef _SERVER
	printf("Starting Core.... %s", ctime(&ltime));
	printf("MaxPlayer : %d\n", MAX_PLAYER);
	printf("MaxItem   : %d\n", MAX_ITEM);
	printf("MaxNpc    : %d\n", MAX_NPC);
	printf("MaxLever  : %d\n", MAX_LEVEL);

	g_Log.Init("\\gameserver_log\\TradeLog.txt");
#endif

	g_InitProtocol();
	//g_FindDebugWindow("#32770", "DebugWin");

#ifdef _DEBUG
	{
		g_FindDebugWindow("#32770", "DebugWin");
		srand((UINT)time(NULL));
#ifdef DEBUGOPT_SCRIPT_MSG_FILEOUT
		struct tm* newtime;
		time_t long_time;
		time(&long_time);   	 /* Get time as long integer. */
		newtime = localtime(&long_time); /* Convert to local time. */
		char szFileName[MAX_PATH];
		char szFileName1[MAX_PATH];
		char szPathName[MAX_PATH];
		g_CreatePath("\\DebugData");
		g_CreatePath("\\DebugData\\Script");

		GetCurrentDirectory(MAX_PATH, szPathName);

		sprintf(szFileName, "%s\\DebugData\\Script\\ScriptOut_%d%d%d%d%d.txt", szPathName, newtime->tm_mon, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
		sprintf(szFileName1, "%s\\DebugData\\Script\\ScriptErr_%d%d%d%d%d.txt", szPathName, newtime->tm_mon, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
		lua_setdebugout(szFileName, szFileName1);
#endif
	}

#endif

#ifndef __linux
	g_RandomSeed(GetTickCount());
#else
	g_RandomSeed(clock());
#endif
	srand((UINT)time(NULL));
	time(&ltime);

#ifdef _SERVER
	//INT nPakNum = g_PakList.Open("\\package.ini");//Load the server's PAK file
	//printf("Loading Pak list successfully, loading %d PAK files\n", nPakNum);
#endif

	if (g_GameSetting.Load(GAME_SETTING_FILE))
	{ // nCurSerIdx
		//g_GameSetting.GetInteger("SkillsIcon", "ExpSkillIdx", 0, &nExpSkillIdx);
		//g_GameSetting.GetInteger("SkillsIcon", "LuckySkillIdx", 0, &nLuckySkillIdx);
		//g_GameSetting.GetInteger("SkillsIcon", "KangkillIdx", 0, &nKangkillIdx);
		//g_GameSetting.GetInteger("SkillsIcon", "YaoPinSkillIdx", 0, &nYaoPinSkillIdx);

		//g_GameSetting.GetInteger("Gameconfig", "GameVersion", 1, &nGameVersion); // 1为中文版本 2 其他版本 需要替换字符串的
#ifdef _SERVER
		//nIsOrdered = 0;
		//nSeparateWorldCount = 10;
		// g_GameSetting.GetInteger("Gameconfig","CoreIsOrdered",0,&nIsOrdered);
		// g_GameSetting.GetInteger("Gameconfig","SeparateWorldCount",10,&nSeparateWorldCount);
		//g_GameSetting.GetInteger("Gameconfig", "MaxSubWorldCount", 1, &nMaxSubWorldCount);
		// INT nThreads=0;
		// g_GameSetting.GetInteger("Gameconfig","CoreThreads",0,&nThreads);
		// if (nMaxSubWorldCount>=nSeparateWorldCount)
		//g_GameSetting.GetInteger("SYSTEM", "ServerIdx", 1, &nCurSerIdx);
		//g_GameSetting.GetInteger("Gameconfig", "CoreSleepTime", 500, &nSleepTime);
		//g_GameSetting.GetInteger("Gameconfig", "MaxPlayerCount", 1, &nMaxPlayerCount);
		//g_GameSetting.GetInteger("Gameconfig", "MaxNpcCount", 1, &nMaxNpcCount);
		//g_GameSetting.GetInteger("Gameconfig", "MaxItemCount", 1, &nMaxItemCount);
		//g_GameSetting.GetInteger("Gameconfig", "MaxObjCount", 1, &nMaxObjCount);
		//g_GameSetting.GetInteger("Gameconfig", "MaxMissleCount", 1, &nMaxMissleCount);
		//g_GameSetting.GetInteger("Item", "MaxSellCount", 0, &nMaxSellCount);
		g_GameSetting.GetInteger2("Gameconfig", "OutScriptWithMap", &nIsOutScript, &nMapIndex);
		//printf("-------SeparateWorldCount:%d,%d,%d,%d,%d -------\n", nSeparateWorldCount, nExpSkillIdx, nLuckySkillIdx, nKangkillIdx, nYaoPinSkillIdx);
		if (!g_ObjChangeName.Load("\\settings\\obj\\ObjData.txt"))
			printf("--加载配置文件失败:%s\n", "\\settings\\obj\\ObjData.txt");
		if (!g_NpcChangeName.Load("\\settings\\npcs.txt"))
			printf("--加载配置文件失败:%s\n", "\\settings\\npcs.txt");
		if (!g_TaskChangeName.Load("\\settings\\item\\questkey.txt"))
			printf("--加载配置文件失败:%s\n", "\\settings\\item\\questkey.txt");
		if (!g_ItemChangeName.Load("\\settings\\item\\mine.txt"))
			printf("--加载配置文件失败:%s\n", "\\settings\\item\\mine.txt");
		if (!g_SkillChangeName.Load("\\settings\\skills.txt"))
			printf("--加载配置文件失败:%s\n", "\\settings\\skills.txt");
#endif
	}
	else
	{
		printf("--加载配置文件失败:%s\n", GAME_SETTING_FILE);
		return;
	}

#ifdef TOOLVERSION	
	g_SpriteCache.Init(256);
#endif

#ifndef _SERVER
	g_bPingReply = TRUE;
	g_SoundCache.Init(256);
	g_SubWorldSet.m_cMusic.Init();
#endif

#ifndef TOOLVERSION
	g_InitSeries();
	if (!g_InitMath())
		g_DebugLog("[Math]Init Math Error!");

	ItemSet.Init();
	ItemGen.Init();

	printf("Item Load OK !\n");

#ifndef _SERVER
	InitAdjustColorTab();//¼ÓÔØÆ«É«±í......
	g_MagicDesc.Init();
#endif
	g_ItemChangeRes.Init();
#endif

	g_PlayerTitle.Load("\\Settings\\playertitle.txt");

	g_NpcMapDropRate.Load(MAP_DROPRATE_FILE);

	NpcSet.Init();
	printf("NPC Load OK !\n");

	ObjSet.Init();
	printf("Obj Load OK !\n");

	MissleSet.Init();
	printf("Missle Load OK !\n");

	g_IniScriptEngine("\\script");
	printf("Engine Script Load OK !\n");

	//	g_SetFilePath(SETTING_PATH);
	g_OrdinSkillsSetting.Load(SKILL_SETTING_FILE);
	g_MisslesSetting.Load(MISSLES_SETTING_FILE);
	g_NpcSetting.Load(NPC_SETTING_FILE);
	printf("Skills Settings Load OK !\n");

	InitGameSetting();
	InitSkillSetting();
	InitMissleSetting();
	InitNpcSetting();

	InitTaskSetting();
	printf("Init Settings Load OK !\n");

	// Õâ¸öÉæ¼°µ½Óë¼¼ÄÜÏà¹ØµÄ¶«Î÷£¬ËùÒÔ±ØÐë·ÅÔÚ¼¼ÄÜ³õÊ¼»¯Ö®ºó

	if (!PlayerSet.Init())
		g_DebugLog("Init PlayerSet Error!!!!\n");

	printf("Player Load OK !\n");

	if (!TongData.Init())
		g_DebugLog("Init TongData Error!!!!\n");

#ifdef _SERVER
	if (!GameData.Init())
		printf("Init GameData Error!!!!\n");

	memset(g_TaskGlobalValue, 0, sizeof(g_TaskGlobalValue));
	g_TeamSet.Init();
	g_ChatRoomSet.Init();

	//	g_SetFilePath("\\");
	g_SubWorldSet.Load("\\maps\\WorldSet.ini");
	g_WorldScript.Init();
	g_WorldScript.RegisterFunctions(WorldScriptFuns, g_GetWorldScriptFunNum());

#endif
	g_SubWorldSet.LoadFile();
#ifndef _SERVER
	g_ScenePlace.Initialize();
#endif
	time(&ltime);

	if (!g_Faction.Init())
		g_DebugLog("Init Faction Fail\n");

	printf("Faction Load OK !\n");

	time(&ltime);

	memset(g_nMeleeWeaponSkill, 0, sizeof(g_nMeleeWeaponSkill));
	memset(g_nRangeWeaponSkill, 0, sizeof(g_nRangeWeaponSkill));
	KTabFile Weapon_PhysicsSkillIdFile;
	if (Weapon_PhysicsSkillIdFile.Load(WEAPON_PHYSICSSKILLFILE))
	{
		INT nHeight = Weapon_PhysicsSkillIdFile.GetHeight() - 1;
		INT nDetailCol = Weapon_PhysicsSkillIdFile.FindColumn(WEAPON_DETAILTYPE);
		INT	nParticularCol = Weapon_PhysicsSkillIdFile.FindColumn(WEAPON_PARTICULARTYPE);
		INT nPhysicsSkillCol = Weapon_PhysicsSkillIdFile.FindColumn(WEAPON_SKILLID);

		for (INT i = 0; i < nHeight; i++)
		{
			INT nDetail = 0;
			INT nParticular = 0;
			INT nPhysicsSkill = 0;
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2, nDetailCol, -1, &nDetail);
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2, nParticularCol, -1, &nParticular);
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2, nPhysicsSkillCol, -1, &nPhysicsSkill);

			//½ü³ÌÎäÆ÷
			if (nDetail == 0)
			{
				if (nParticular >= 0 && nParticular < MAX_MELEEWEAPON_PARTICULARTYPE_NUM && nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nMeleeWeaponSkill[nParticular] = nPhysicsSkill;
			}
			else if (nDetail == 1)
			{
				if (nParticular >= 0 && nParticular < MAX_RANGEWEAPON_PARTICULARTYPE_NUM && nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nRangeWeaponSkill[nParticular] = nPhysicsSkill;
			}
			else if (nDetail == -1) //¿ÕÊÖ
			{
				if (nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nHandSkill = nPhysicsSkill;
			}
		}
	}
#ifndef TOOLVERSION
	if (!BuySell.Init())
		g_DebugLog("BuySell Init Failed!\n");
#endif
}


BOOL	InitTaskSetting()
{
#ifdef _SERVER
	if (!g_EventItemTab.Load(EVENTITEM_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD EventItem Setting File %s", EVENTITEM_TABFILE);
	}
	if (!g_QuestItemTab.Load(QUESTITEM_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD QuestItem Setting File %s", QUESTITEM_TABFILE);
	}
	if (!g_TownPortalTab.Load(TOWNPORTAL_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD TownPortal Setting File %s", TOWNPORTAL_TABFILE);
	}
	if (!g_MagicScriptTab.Load(MAGICSCRIPT_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD MagicScript Setting File %s", MAGICSCRIPT_TABFILE);
	}
	if (!g_WayPointTabFile.Load(WORLD_WAYPOINT_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD WayPoint Setting File %s", WORLD_WAYPOINT_TABFILE);
	}
	if (!g_StationTabFile.Load(WORLD_STATION_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD WayPoint Setting File %s", WORLD_STATION_TABFILE);
	}

	if (!g_StationPriceTabFile.Load(WORLD_STATIONPRICE_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD STATION PRICE TabFile %s", WORLD_STATIONPRICE_TABFILE);
	}

	if (!g_WayPointPriceTabFile.Load(WORLD_WAYPOINTPRICE_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD WAYPOINT PRICE TabFile %s", WORLD_WAYPOINTPRICE_TABFILE);
	}

	if (!g_DockTabFile.Load(WORLD_DOCK_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD DOCK TabFile %s", WORLD_DOCK_TABFILE);
	}

	if (!g_DockPriceTabFile.Load(WORLD_DOCKPRICE_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD DOCK PRICE TabFile %s", WORLD_DOCKPRICE_TABFILE);
	}

	INT nWidth1 = g_StationPriceTabFile.GetWidth();
	INT nHeight1 = g_StationPriceTabFile.GetHeight();
	INT nWidth2 = g_WayPointPriceTabFile.GetWidth();
	INT nHeight2 = g_WayPointPriceTabFile.GetHeight();
	INT nWidth3 = g_DockPriceTabFile.GetWidth();
	INT nHeight3 = g_DockPriceTabFile.GetHeight();


	KASSERT(nHeight1 * nWidth1);
	KASSERT(nHeight2 * nWidth2);
	KASSERT(nHeight3 * nWidth3);

	if (nWidth1 * nHeight1)
	{
		g_StationPriceTabFile.GetInteger(1, 1, 1, &g_StationPriceUnit);
		g_pStationPriceTab = new INT[nWidth1 * nHeight1];
		memset(g_pStationPriceTab, -1, nWidth1 * nHeight1 * sizeof(INT));
	}

	if (nWidth2 * nHeight2)
	{
		g_WayPointPriceTabFile.GetInteger(1, 1, 1, &g_WayPointPriceUnit);
		g_pWayPointPriceTab = new INT[nWidth2 * nHeight2];
		memset(g_pWayPointPriceTab, -1, nWidth2 * nHeight2 * sizeof(INT));
	}

	if (nWidth3 * nHeight3)
	{
		g_DockPriceTabFile.GetInteger(1, 1, 1, &g_DockPriceUnit);
		g_pDockPriceTab = new INT[nWidth3 * nHeight3];
		memset(g_pDockPriceTab, -1, nWidth3 * nHeight3 * sizeof(INT));
	}

#endif
#ifndef _SERVER
	if (!g_StringResourseTabFile.Load(STRINGRESOURSE_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD %s", STRINGRESOURSE_TABFILE);
	}

	if (!g_RankTabSetting.Load(PLAYER_RANK_SETTING_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD %s", PLAYER_RANK_SETTING_TABFILE);
	}
#endif

	return TRUE;
}

BOOL	InitNpcSetting()
{
	memset(g_pNpcTemplate, 0, sizeof(void*) * MAX_NPCSTYLE);
	if (!g_NpcKindFile.Load(NPC_RES_KIND_FILE_NAME))
	{
		g_DebugLog("Can not load %s", NPC_RES_KIND_FILE_NAME);
	}

	// Load the script file for Npc level setting, which will be used when loading Npc in the future.
#ifdef _SERVER

	g_pNpcLevelScript = (KLuaScript*)g_GetScript(NPC_LEVELSCRIPT_FILENAME);

	if (!g_pNpcLevelScript)
	{
		printf("-%s Npc等级设定脚本无法加载-\n", NPC_LEVELSCRIPT_FILENAME);
	}

#else
	g_pNpcLevelScript = new KLuaScript;
	g_pNpcLevelScript->Init();
	if (!g_pNpcLevelScript->Load(NPC_LEVELSCRIPT_FILENAME))
	{
		// printf("[error]致命错误,无法正确读取%s\n", NPC_LEVELSCRIPT_FILENAME);
		delete g_pNpcLevelScript;
		g_pNpcLevelScript = NULL;
	}
#endif

#ifndef _SERVER
	g_NpcResList.Init();
#endif

	return TRUE;
}

BOOL	InitMissleSetting()
{
	INT nMissleNum = g_MisslesSetting.GetHeight() - 1;

	for (INT i = 0; i < nMissleNum; i++)
	{
		INT nMissleId = 0;
		g_MisslesSetting.GetInteger(i + 2, "MissleId", -1, &nMissleId);

		if (nMissleId > 0)
		{
			g_MisslesLib[nMissleId].GetInfoFromTabFile(i + 2);
			g_MisslesLib[nMissleId].m_nMissleId = nMissleId;
		}
	}
	//printf("Init MisslesLib %d\n", sizeof(g_MisslesLib));
	return TRUE;
}

extern INT g_LoadSkillInfo();
BOOL	InitSkillSetting()
{

	if (!g_SkillManager.Init())
	{
		_ASSERT(0);
	}

#ifndef _SERVER
	if (!g_ClientWeaponSkillTabFile.Load(CLIENTWEAPONSKILL_TABFILE))
	{
		g_DebugLog("Can Not Load %s", CLIENTWEAPONSKILL_TABFILE);
	}
#endif

	return TRUE;
}
#ifdef _SERVER
BOOL	LoadNpcSettingFromBinFile(LPSTR BinFile = NPC_TEMPLATE_BINFILE)
{
	return FALSE;
}

BOOL	SaveAsBinFileFromNpcSetting(LPSTR BinFile = NPC_TEMPLATE_BINFILE)
{
	return FALSE;
}
#endif

//---------------------------------------------------------------------------

void g_ReleaseCore()
{
#ifdef _SERVER
	GameData.Save();

	if (g_pServer)
	{
		g_pServer->Release();
		g_pServer = NULL;
	}

	if (g_pTongClient)
	{
		g_pTongClient->Release();
		g_pTongClient = NULL;
	}

	if (g_pDataClient)
	{
		g_pDataClient->Release();
		g_pDataClient = NULL;
	}

	if (g_pStationPriceTab)
	{
		delete[] g_pStationPriceTab;
		g_pStationPriceTab = NULL;
	}

	if (g_pWayPointPriceTab)
	{
		delete[] g_pWayPointPriceTab;
		g_pWayPointPriceTab = NULL;
	}

	if (g_pDockPriceTab)
	{
		delete[] g_pDockPriceTab;
		g_pDockPriceTab = NULL;
	}
#else
	g_SubWorldSet.Close();
	g_ScenePlace.ClosePlace();

	if (g_pAdjustColorTab)
	{
		delete[]g_pAdjustColorTab;
		g_pAdjustColorTab = NULL;
		g_ulAdjustColorCount = 0;
	}
#endif

	g_UnInitMath();
}

#ifdef _SERVER
void g_SetServer(LPVOID pServer)
{
	g_pServer = reinterpret_cast<IServer*>(pServer);
}

void g_SetTongClient(LPVOID pTong)
{
	g_pTongClient = reinterpret_cast<IClient*>(pTong);
}

void g_SetDataServer(LPVOID pServer)
{
	g_pDataClient = reinterpret_cast<IClient*>(pServer);

}
#endif

#ifndef _SERVER
void g_SetClient(LPVOID pClient)
{
	g_pClient = reinterpret_cast<IClient*>(pClient);
}

UINT	InitAdjustColorTab()
{
	g_pAdjustColorTab = NULL;
	g_ulAdjustColorCount = 0;
	KTabFile TabFile;
	if (!TabFile.Load(ADJUSTCOLOR_TABFILE))
	{
		_ASSERT(0);
		g_DebugLog("Can't load %s", ADJUSTCOLOR_TABFILE);
		return 0;
	}

	INT nHeight = TabFile.GetHeight() - 1;

	if (nHeight <= 0)
		return 0;

	g_pAdjustColorTab = (UINT*)new ULONG[nHeight];
	g_ulAdjustColorCount = nHeight;
	for (INT i = 0; i < nHeight; i++)
	{
		BYTE bAlpha = 0;
		BYTE bRed = 0;
		BYTE bGreen = 0;
		BYTE bBlue = 0;
		INT nAlpha;
		INT nRed;
		INT nGreen;
		INT nBlue;
		TabFile.GetInteger(i + 2, "ALPHA", 0x000000ff, &nAlpha);
		nAlpha &= 0xff;
		TabFile.GetInteger(i + 2, "RED", 0, &nRed);
		nRed &= 0xff;
		TabFile.GetInteger(i + 2, "GREEN", 0, &nGreen);
		nGreen &= 0xff;
		TabFile.GetInteger(i + 2, "BLUE", 0, &nBlue);
		nBlue &= 0xff;
		ULONG ulAdjustColor = nAlpha << 24 | nRed << 16 | nGreen << 8 | nBlue;
		g_pAdjustColorTab[i] = ulAdjustColor;
	}
	return g_ulAdjustColorCount;
}
#endif


BOOL InitGameSetting()
{
	if (!g_MapTraffic.Load(MINIMAP_SETTING_FILE_INI))
	{
		g_DebugLog("Can not load %s", MINIMAP_SETTING_FILE_INI);
	}
	if (!g_MaskChangeRes.Load(CHANGERES_MASK_FILE))
	{
		g_DebugLog("Can not load %s", CHANGERES_MASK_FILE);
	}
	if (g_GameSetting.Load(GAME_SETTING_FILE_INI))
	{
#ifdef _SERVER
		g_GameSetting.GetInteger("ServerConfig", "ExpRate", 1, &g_ExpRate);
		g_GameSetting.GetInteger("ServerConfig", "MoneyRate", 1, &g_MoneyRate);
		g_GameSetting.GetInteger("ServerConfig", "SkillRate", 0, &g_SkillExpRate);
		g_GameSetting.GetInteger("ServerConfig", "Skill120Rate", 0, &g_Skill120ExpRate);
#endif
	}
	else
	{
		g_DebugLog("Can not load %s", GAME_SETTING_FILE_INI);
	}
	return TRUE;
}

INT PositionToRoom(INT P)
{
	switch (P)
	{
	case pos_hand:
	case pos_equip:
	case pos_equiproom:
		return room_equipment;
	case pos_equiproomex:
		return room_equipmentex;
	case pos_repositoryroom:
		return room_repository;
	case pos_repositoryroom1:
		return room_repository1;
	case pos_repositoryroom2:
		return room_repository2;
	case pos_repositoryroom3:
		return room_repository3;
	case pos_repositoryroom4:
		return room_repository4;
	case pos_repositoryroom5:
		return room_repository5;
	case pos_traderoom:
		return room_equipment;
	case pos_trade1:
		return room_equipment;
	case pos_immediacy:
		return room_immediacy;
	case pos_give:
		return room_give;
	case pos_compound:
		return room_equipment;
		//TamLTM kham nam
	case pos_builditem:
		return room_builditem;
		//end
	case pos_compoundroom:
		return room_compound;
	default:
		return room_equipment;
	}
	return room_equipment;
}