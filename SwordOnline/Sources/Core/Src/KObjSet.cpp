//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KObjSet.cpp
// Date:	2002.01.06
// Code:	±ß³ÇÀË×Ó
// Desc:	Obj Class
//---------------------------------------------------------------------------
#include "KCore.h"
#include "KSubWorld.h"
#include "KObj.h"
#ifndef _SERVER
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "scene/KScenePlaceC.h"
#endif
#include "KObjSet.h"
#include "CoreUseNameDef.h"
#include "KItem.h"
#include "Scene/SceneDataDef.h"
#include "Text.h"

#define		OBJ_WORLD_ID_START		1000

enum
{
	ObjDataField_Name = 1,
	ObjDataField_DataID,
	ObjDataField_Kind,
	ObjDataField_ScriptName,
	ObjDataField_ImageName,
	ObjDataField_SoundName,
	ObjDataField_LifeTime,
	ObjDataField_Layer,
	ObjDataField_Height,

	ObjDataFiled_SkillKind,
	ObjDataFiled_SkillCamp,
	ObjDataFiled_SkillRange,
	ObjDataField_SkillCastTime,
	ObjDataField_SkillID,
	ObjDataField_SkillLevel,

	ObjDataField_LightRadius,
	ObjDataField_LightRed,
	ObjDataField_LightGreen,
	ObjDataField_LightBlue,
	ObjDataField_LightAlpha,
	ObjDataField_LightReflectType,

	ObjDataField_ImageTotalFrame,
	ObjDataField_ImageCurFrame,
	ObjDataField_ImageTotalDir,
	ObjDataField_ImageCurDir,
	ObjDataField_ImageInterval,
	ObjDataField_ImageCgXpos,
	ObjDataField_ImageCgYpos,

	ObjDataField_Bar0,
	ObjDataField_Bar1,
	ObjDataField_Bar2,
	ObjDataField_Bar3,
	ObjDataField_Bar4,
	ObjDataField_Bar5,
	ObjDataField_Bar6,
	ObjDataField_Bar7,
	ObjDataField_Bar8,
	ObjDataField_Bar9,
	ObjDataField_Bar10,
	ObjDataField_Bar11,
	ObjDataField_Bar12,
	ObjDataField_Bar13,
	ObjDataField_Bar14,

	ObjDataField_ImageDropName,
	ObjDataField_ImageDropTotalFrame,
	ObjDataField_ImageDropCurFrame,
	ObjDataField_ImageDropTotalDir,
	ObjDataField_ImageDropCurDir,
	ObjDataField_ImageDropInterval,
	ObjDataField_ImageDropCgXpos,
	ObjDataField_ImageDropCgYpos,

	ObjDataField_DrawFlag,

	ObjDataField_Num,
};

KObjSet	ObjSet;

char	g_szObjKind[Obj_Kind_Num][32] = 
{
	"MapObj",
	"Body",
	"Box",
	"Item",
	"Money",
	"LoopSound",
	"RandSound",
	"Light",
	"Door",
	"Trap",
	"Prop",
	"Obstacle"
};

DWORD	g_dwObjKindNum[Obj_Kind_Num] = 
{
	0x4f70614d,
	0x79646f42,
	0x00786f42,
	0x6d657449,
	0x656e6f4d,
	0x706f6f4c,
	0x646e6152,
	0x6867694c,
	0x726f6f44,
	0x70617254,
	0x706f7250,
	0x7473624f,
};

KObjSet::KObjSet()
{
	m_nObjID = OBJ_WORLD_ID_START;
}

KObjSet::~KObjSet()
{
	m_nObjID = 0;
}

//---------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯£¬ÔØÈëµØÍ¼objÊý¾ÝÄ£°å
//---------------------------------------------------------------------------
BOOL	KObjSet::Init()
{
	BOOL	bRet = TRUE;
	INT		i;

	m_FreeIdx.Init(MAX_OBJECT);
	m_UseIdx.Init(MAX_OBJECT);

	// ¿ªÊ¼Ê±ËùÓÐµÄÊý×éÔªËØ¶¼Îª¿Õ
	for (i = MAX_OBJECT - 1; i > 0; i--)
	{
		m_FreeIdx.Insert(i);
	}

//	g_SetFilePath("");
	if ( !m_cTabFile.Load(OBJ_DATA_FILE_NAME) )
		bRet = FALSE;

//	g_SetFilePath("");
	if ( !m_cMoneyFile.Load(MONEY_OBJ_FILE_NAME))
		bRet = FALSE;

#ifndef _SERVER
	INT			nA, nR, nG, nB, nColorCount;
	char		szTemp[32];
	KIniFile	cColorIni;

//	g_SetFilePath("");
	if (!cColorIni.Load(OBJ_NAME_COLOR_FILE))
	{
		for (i = 0; i < MAX_OBJ_NAME_COLOR; i++)
			m_dwNameColor[i] = 0x00ffffff;
	}
	else
	{
		cColorIni.GetInteger("List", "Count", 0, &nColorCount);
		if (nColorCount > MAX_OBJ_NAME_COLOR)
			nColorCount = MAX_OBJ_NAME_COLOR;
		for (i = 0; i < nColorCount; i++)
		{
			sprintf(szTemp, "%d", i);
			cColorIni.GetInteger(szTemp, "A", 0, &nA);
			cColorIni.GetInteger(szTemp, "R", 0, &nR);
			cColorIni.GetInteger(szTemp, "G", 0, &nG);
			cColorIni.GetInteger(szTemp, "B", 0, &nB);
			m_dwNameColor[i] = (nA << 24) | (nR << 16) | (nG << 8) | nB;
		}
		for (i = nColorCount; i < MAX_OBJ_NAME_COLOR; i++)
			m_dwNameColor[i] = 0x00ffffff;
	}
#endif
	
	return bRet;
}

#ifdef _SERVER
INT		KObjSet::AddMoneyObj(KMapPos MapPos, INT nMoneyNum)
{
	if (nMoneyNum <= 0 || nMoneyNum >= 999999999)
		return -1;

	INT nDataID = GetMoneyDataId(nMoneyNum);
	KObjItemInfo	sInfo;
	sInfo.m_nItemID = 0;
	sInfo.m_nItemWidth = 0;
	sInfo.m_nItemHeight = 0;
	sInfo.m_nMoneyNum = nMoneyNum;
	sInfo.m_szName[0] = 0;
	sInfo.m_nColorID = 0;
	sInfo.m_nGenre = -1;
	sInfo.m_nDetailType = 0;
	sInfo.m_nMovieFlag = 1;
	sInfo.m_nSoundFlag = 1;
	sInfo.m_bOverLook = FALSE;
	return Add(nDataID, MapPos, sInfo);
}
#endif

#ifdef _SERVER
INT		KObjSet::GetMoneyDataId(INT nMoney)
{
	INT nMoneyLevel = 0;
	INT	nRet = 0;
	for (INT i = 0; i < m_cMoneyFile.GetHeight() - 1; i++)
	{
		m_cMoneyFile.GetInteger(i + 2, 1, 0, &nMoneyLevel);
		if (nMoney < nMoneyLevel)
		{
			m_cMoneyFile.GetInteger(i + 2, 2, 0, &nRet);
			break;
		}
	}
	return nRet;
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÌí¼ÓÒ»¸öobj£¬·µ»ØÔÚobjÊý×éÖÐµÄÎ»ÖÃ±àºÅ£¨Èç¹û <= 0 £¬Ê§°Ü£©
//---------------------------------------------------------------------------
INT		KObjSet::Add(INT nDataID, KMapPos MapPos, KObjItemInfo sItemInfo)
{
	INT nAddNo;

	nAddNo = AddData(nDataID, MapPos, sItemInfo.m_nMoneyNum, sItemInfo.m_nItemID, sItemInfo.m_nItemWidth, sItemInfo.m_nItemHeight);
	if (nAddNo < 0)
		return -1;

	Object[nAddNo].SetWorldID(GetID());
	Object[nAddNo].m_bOverLook = sItemInfo.m_bOverLook;
	if(Object[nAddNo].m_nKind == Obj_Kind_Item)
	{
		Object[nAddNo].m_nColorID = sItemInfo.m_nColorID;
		Object[nAddNo].m_nGenre = sItemInfo.m_nGenre;
		Object[nAddNo].m_nDetailType = sItemInfo.m_nDetailType;
	}
	else
	{
		sItemInfo.m_nColorID = 0;
		sItemInfo.m_nGenre = 0;
		sItemInfo.m_nDetailType = 0;
	}
	if (sItemInfo.m_szName[0])
	{
		char szName[OBJ_NAME_LENGTH];
		const char* pszName = TGetLimitLenString(sItemInfo.m_szName, -1, szName, OBJ_NAME_LENGTH-8);

		if (Object[nAddNo].m_nKind == Obj_Kind_Item && Item[sItemInfo.m_nItemID].IsStack())
			sprintf(szName, "%s x%d", pszName, Item[sItemInfo.m_nItemID].GetStackNum());	
		else
			strcpy(szName, pszName);

		strcpy(Object[nAddNo].m_szName, szName);
	}

	OBJ_ADD_SYNC	cObjAdd;
	INT				i, nTempX, nTempY;
	cObjAdd.ProtocolType = (BYTE)s2c_objadd;
	cObjAdd.m_nID = Object[nAddNo].m_nID;
	cObjAdd.m_nDataID = Object[nAddNo].m_nDataID;
	cObjAdd.m_btDir = Object[nAddNo].m_nDir;
	cObjAdd.m_wCurFrame = Object[nAddNo].m_cImage.m_nCurFrame;
	cObjAdd.m_btState = Object[nAddNo].m_nState;
	SubWorld[MapPos.nSubWorld].Map2Mps(MapPos.nRegion, MapPos.nMapX, MapPos.nMapY, MapPos.nOffX, MapPos.nOffY, &nTempX, &nTempY);
	cObjAdd.m_nXpos = nTempX;
	cObjAdd.m_nYpos = nTempY;
	cObjAdd.m_nMoneyNum = sItemInfo.m_nMoneyNum;
	cObjAdd.m_nItemID = sItemInfo.m_nItemID;
	cObjAdd.m_btItemWidth = sItemInfo.m_nItemWidth;
	cObjAdd.m_btItemHeight = sItemInfo.m_nItemHeight;
	cObjAdd.m_btColorID = sItemInfo.m_nColorID;
	cObjAdd.m_nGenre = sItemInfo.m_nGenre;
	cObjAdd.m_nDetailType = sItemInfo.m_nDetailType;
	cObjAdd.m_btFlag = 0;
	if (sItemInfo.m_nSoundFlag)
		cObjAdd.m_btFlag |= 0x01;
	if (sItemInfo.m_nMovieFlag)
		cObjAdd.m_btFlag |= 0x02;
	if (sItemInfo.m_bOverLook)
		cObjAdd.m_btFlag |= 0x04;
	strcpy(cObjAdd.m_szName, Object[nAddNo].m_szName);
	cObjAdd.m_wLength = sizeof(OBJ_ADD_SYNC) - 1 - sizeof(cObjAdd.m_szName) + strlen(cObjAdd.m_szName);

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
	SubWorld[MapPos.nSubWorld].m_Region[MapPos.nRegion].BroadCast((BYTE*)&cObjAdd, cObjAdd.m_wLength + 1, nMaxCount, Object[nAddNo].m_nMapX, Object[nAddNo].m_nMapY);
	INT nConRegion;
	for (i = 0; i < 8; i++)
	{
		nConRegion = SubWorld[MapPos.nSubWorld].m_Region[MapPos.nRegion].m_nConnectRegion[i];
		if (nConRegion == -1)
			continue;
		SubWorld[MapPos.nSubWorld].m_Region[nConRegion].BroadCast((BYTE*)&cObjAdd, cObjAdd.m_wLength + 1, nMaxCount, Object[nAddNo].m_nMapX - POff[i].x, Object[nAddNo].m_nMapY - POff[i].y);
	}

	SubWorld[MapPos.nSubWorld].m_Region[MapPos.nRegion].AddObj(nAddNo);// m_WorldMessage.Send(GWM_OBJ_ADD, MapPos.nRegion, nAddNo);

	return nAddNo;
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º´ÓobjÊý¾ÝÎÄ¼þÖÐÔØÈëÏàÓ¦Êý¾Ý
//---------------------------------------------------------------------------
INT		KObjSet::AddData(INT nDataID, KMapPos MapPos, INT nMoneyNum, INT nItemID, INT nItemWidth, INT nItemHeight)
{
	if (nDataID <= 0 || nDataID >= m_cTabFile.GetHeight())
		return -1;
	if (MapPos.nSubWorld < 0 || MapPos.nSubWorld >= MAX_SUBWORLD)
		return -1;
	if (MapPos.nRegion < 0)
		return -1;

	INT		nFreeNo, i, nTemp;
	char	szBuffer[80];

	nFreeNo = FindFree();
	if (nFreeNo <= 0)
		return -1;

	Object[nFreeNo].Release();
	Object[nFreeNo].m_nDataID = nDataID;
	m_cTabFile.GetString(nDataID + 1, ObjDataField_Kind, g_szObjKind[0], szBuffer, sizeof(szBuffer));
	for (i = 0; i < Obj_Kind_Num; i++)
	{
		if (*(DWORD*)(&szBuffer) == g_dwObjKindNum[i])
			break;
	}
	if (i >= Obj_Kind_Num)
		i = 0;
	Object[nFreeNo].m_nKind = i;
	// ´Ó ObjData ÎÄ¼þÖÐ¶ÁÈ¡Êý¾Ý
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LifeTime, 0, &Object[nFreeNo].m_nLifeTime);
	m_cTabFile.GetString(nDataID + 1, ObjDataField_Name, "", Object[nFreeNo].m_szName, sizeof(Object[nFreeNo].m_szName));
	m_cTabFile.GetString(nDataID + 1, ObjDataField_ScriptName, "", szBuffer, sizeof(szBuffer));
	Object[nFreeNo].SetScriptFile(szBuffer);

	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageTotalFrame, 1, &nTemp);
	Object[nFreeNo].m_cImage.SetTotalFrame(nTemp);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageTotalDir, 1, &nTemp);
	Object[nFreeNo].m_cImage.SetTotalDir(nTemp);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCurFrame, 0, &nTemp);
	Object[nFreeNo].m_cImage.SetCurFrame(nTemp);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCurDir, 0, &nTemp);
	Object[nFreeNo].m_cImage.SetCurDir(nTemp);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageInterval, 0, (INT*)(&Object[nFreeNo].m_cImage.m_dwInterval));
	
	Object[nFreeNo].m_nDir = (Object[nFreeNo].m_cImage.m_nCurDir * 64 / Object[nFreeNo].m_cImage.m_nTotalDir) % 64;

	for (i = 0; i < OBJ_BAR_SIZE; i++)
	{
		m_cTabFile.GetInteger(nDataID + 1, ObjDataField_Bar0 + i, 0, &nTemp);
		Object[nFreeNo].m_btBar[i] = (BYTE)nTemp;
	}

	Object[nFreeNo].m_nIndex = nFreeNo;
	Object[nFreeNo].m_nSubWorldID = MapPos.nSubWorld;
	Object[nFreeNo].m_nRegionIdx = MapPos.nRegion;
	Object[nFreeNo].m_nMapX = MapPos.nMapX;
	Object[nFreeNo].m_nMapY = MapPos.nMapY;
	Object[nFreeNo].m_nOffX = MapPos.nOffX;
	Object[nFreeNo].m_nOffY = MapPos.nOffY;
	Object[nFreeNo].m_nMoneyNum = nMoneyNum;
	Object[nFreeNo].m_nBelong = -1;
	Object[nFreeNo].m_nBelongTime = 0;

	SetObjItem(nFreeNo, nItemID, nItemWidth, nItemHeight);

	// if (ÓÐÁËÎï¼þÕÏ°­²ã) Éè¶¨Îï¼þÕÏ°­ (not end)

	
	m_UseIdx.Insert(nFreeNo);
	m_FreeIdx.Remove(nFreeNo);

	return nFreeNo;
}
#endif

#ifdef _SERVER
INT		KObjSet::SearchObjAt(KMapPos MapPos)
{
	INT i;

	i = m_UseIdx.GetNext(0);
	while(i)
	{
		if(Object[i].m_nSubWorldID == MapPos.nSubWorld && 
			Object[i].m_nRegionIdx == MapPos.nRegion && 
			Object[i].m_nMapX == MapPos.nMapX && 
			Object[i].m_nMapY == MapPos.nMapY && 
			Object[i].m_nOffX == MapPos.nOffX && 
			Object[i].m_nOffY == MapPos.nOffY)
			return i;
		i = m_UseIdx.GetNext(i);
	}
	return 0;
}
#endif


#ifdef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨objËù´øµÄÎï¼þÐÅÏ¢£¬°üÀ¨Îï¼þid£¬Îï¼þÔÚ×°±¸À¸ÖÐµÄ³¤¡¢¿í
//---------------------------------------------------------------------------
BOOL	KObjSet::SetObjItem(INT nObjIndex, INT nItemID, INT nItemWidth, INT nItemHeight)
{
	if (nObjIndex <= 0 || nObjIndex >= MAX_OBJECT)
		return FALSE;

	Object[nObjIndex].m_nItemDataID = nItemID;
	Object[nObjIndex].m_nItemWidth = nItemWidth;
	Object[nObjIndex].m_nItemHeight = nItemHeight;

	return TRUE;
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º·þÎñÆ÷¸øÄ³¸ö¿Í»§¶Ë·¢ËÍÄ³¸öobjµÄÊý¾ÝÐÅÏ¢
//---------------------------------------------------------------------------
BOOL	KObjSet::SyncAdd(INT nID, INT nClient)
{
	INT		nFindIndex;
	nFindIndex = FindID(nID);
	if (nFindIndex <= 0)
		return FALSE;

	return Object[nFindIndex].SyncAdd(nClient);
}
#endif

#ifndef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º¿Í»§¶ËÌí¼ÓÒ»¸öobj
//---------------------------------------------------------------------------
INT		KObjSet::ClientAdd(INT nID, INT nDataID, INT nState, INT nDir, INT nCurFrame, INT nXpos, INT nYpos, KObjItemInfo sInfo, DWORD nObjDwidx)
{
	if (nDataID <= 0 || nDataID >= m_cTabFile.GetHeight())
		return -1;

	INT		nAddIndex;
	INT		nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[0].Mps2Map(nXpos, nYpos, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion < 0)
		return -1;
	nAddIndex = AddData(nDataID, 0, nRegion, nMapX, nMapY, nOffX, nOffY);
	if (nAddIndex < 0)
		return -1;
	Object[nAddIndex].SetWorldID(nID);
	if (nDir >= 0)
	{
		Object[nAddIndex].SetDir(nDir);
	}
	else
	{
		if (nCurFrame > 0)
			Object[nAddIndex].m_cImage.SetCurFrame(nCurFrame);
	}
	if (nState >= 0)
	{
		Object[nAddIndex].SetState(nState, sInfo.m_nSoundFlag);
	}
	if (Object[nAddIndex].m_nKind == Obj_Kind_Item && sInfo.m_nMovieFlag)
	{
		Object[nAddIndex].m_nDropState = 1;	// ÉèÎªµô³ö¶¯»­
		if (nCurFrame > 0)
			Object[nAddIndex].m_cImageDrop.SetCurFrame(nCurFrame);
	}
	else
	{
		Object[nAddIndex].m_nDropState = 0;
	}
	Object[nAddIndex].m_nMoneyNum = sInfo.m_nMoneyNum;
	Object[nAddIndex].m_nItemDataID = sInfo.m_nItemID;
	Object[nAddIndex].m_nItemWidth = sInfo.m_nItemWidth;
	Object[nAddIndex].m_nItemHeight = sInfo.m_nItemHeight;
	Object[nAddIndex].m_nColorID = sInfo.m_nColorID;
	Object[nAddIndex].m_nGenre = sInfo.m_nGenre;
	Object[nAddIndex].m_nDetailType = sInfo.m_nDetailType;
	Object[nAddIndex].m_dwNameColor = this->GetNameColor(sInfo.m_nColorID);
	Object[nAddIndex].m_bOverLook = sInfo.m_bOverLook;

	Object[nAddIndex].m_AttackerDwid = nObjDwidx;
	Object[nAddIndex].m_IsHaveAttack = sInfo.m_cHaveAttack;

	if (Object[nAddIndex].m_IsHaveAttack)
		Object[nAddIndex].m_AttackerTime = 100800;
	else
		Object[nAddIndex].m_AttackerTime = 0;

	if (sInfo.m_szName[0])
		strcpy(Object[nAddIndex].m_szName, sInfo.m_szName);
#ifdef _DEBUG
	g_DebugLog("Obj:%x, %d, %d", SubWorld[0].m_Region[nRegion].m_RegionID, nMapX, nMapY);
#endif
	SubWorld[0].m_Region[nRegion].AddObj(nAddIndex);
	return nAddIndex;
}
#endif

#ifndef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º¿Í»§¶ËÔØÈëÒ»¸öRegionµÄËùÓÐObj
//---------------------------------------------------------------------------
BOOL	KObjSet::ClientLoadRegionObj(char *lpszMapPath, INT nRegionX, INT nRegionY, INT nSubWorld, INT nRegion)
{
	if (!lpszMapPath || !lpszMapPath[0])
		return FALSE;

	char	szFile[80];

//	sprintf(szPath, "%s\\v_%03d", lpszMapPath, nRegionY);
//	g_SetFilePath(szPath);

	KPakFile			cDataFile;
	KObjFileHead		sHead;

	sprintf(szFile, "\\%s\\v_%03d\\%03d_%s", lpszMapPath, nRegionY, nRegionX, REGION_OBJ_FILE_CLIENT);
	if (cDataFile.Open(szFile) != TRUE)
		return FALSE;
	if (cDataFile.Size() < sizeof(KObjFileHead))
	{
		cDataFile.Close();
		return FALSE;
	}
	cDataFile.Read(&sHead, sizeof(KObjFileHead));

	KSPObj	sData;
	INT		nKind;
	KObjItemInfo	sInfo;

	for (INT i = 0; (DWORD)i < sHead.uNumObj; i++)
	{
		memset(sData.szScript, 0, sizeof(sData.szScript));
		cDataFile.Read(&sData, sizeof(KSPObj) - sizeof(sData.szScript));
		cDataFile.Read(sData.szScript, sData.nScriptNameLen);
		nKind = GetDataIDKind(sData.nTemplateID);
		if (CheckClientKind(nKind) != 1)
			continue;

		sInfo.m_nItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_nColorID = 0;
		sInfo.m_nGenre = 0;
		sInfo.m_nDetailType = 0;
		sInfo.m_szName[0] = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		sInfo.m_bOverLook = 0;
		sInfo.m_cHaveAttack = 0;

		ClientAdd(0, sData.nTemplateID, sData.nState, sData.nDir, 0, sData.Pos.x, sData.Pos.y, sInfo);
	}
	cDataFile.Close();

	return TRUE;
}
#endif

#ifndef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º¿Í»§¶ËÔØÈëÒ»¸öRegionµÄËùÓÐObj
//---------------------------------------------------------------------------
BOOL	KObjSet::ClientLoadRegionObj(KPakFile *pFile, DWORD dwDataSize)
{
	if (!pFile || dwDataSize < sizeof(sizeof(KObjFileHead)))
		return FALSE;

	KObjFileHead	sHead;
	KSPObj			sData;
	INT				nKind;
	KObjItemInfo	sInfo;

	pFile->Read(&sHead, sizeof(sHead));
	for (INT i = 0; (DWORD)i < sHead.uNumObj; i++)
	{
		pFile->Read(&sData, sizeof(KSPObj) - sizeof(sData.szScript));
		if (sData.nScriptNameLen < sizeof(sData.szScript))
		{
			pFile->Read(sData.szScript, sData.nScriptNameLen);
			sData.szScript[sData.nScriptNameLen] = 0;
		}
		else
		{
			sData.szScript[0] = 0;
			pFile->Seek(sData.nScriptNameLen, FILE_CURRENT);
		}
		nKind = GetDataIDKind(sData.nTemplateID);
		if (CheckClientKind(nKind) != 1)
			continue;

		sInfo.m_nItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_nColorID = 0;
		sInfo.m_nGenre = 0;
		sInfo.m_nDetailType = 0;
		sInfo.m_szName[0] = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		sInfo.m_bOverLook = 0;
		sInfo.m_cHaveAttack = 0;

		ClientAdd(0, sData.nTemplateID, sData.nState, sData.nDir, 0, sData.Pos.x, sData.Pos.y, sInfo);
	}

	return TRUE;
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º·þÎñÆ÷¶ËÔØÈëÒ»¸öRegionµÄËùÓÐObj
//---------------------------------------------------------------------------
BOOL	KObjSet::ServerLoadRegionObj(char *lpszMapPath, INT nRegionX, INT nRegionY, INT nSubWorld)
{
	// Cancel
	return FALSE;

	if (!lpszMapPath || !lpszMapPath[0])
		return FALSE;

	char szPath[MAX_SENTENCE_LENGTH], szFile[MAX_SENTENCE_LENGTH];

	//	sprintf(szPath, "%s", lpszMapPath);
	//	g_SetFilePath(szPath);

	KPakFile cDataFile;
	KObjFileHead sHead;

	sprintf(szFile, "%s\\%03d_%s", lpszMapPath, nRegionX, REGION_OBJ_FILE_SERVER);

	// printf("-----地图路径:%s ---\n",szFile);

	if (cDataFile.Open(szFile) != TRUE)
		return FALSE;
	if (cDataFile.Size() < sizeof(KObjFileHead))
	{
		cDataFile.Close();
		return FALSE;
	}
	cDataFile.Read(&sHead, sizeof(KObjFileHead));

	KSPObj sData;
	INT nKind, nNo;
	for (INT i = 0; i < sHead.uNumObj; ++i)
	{
		memset(sData.szScript, 0, sizeof(sData.szScript));
		cDataFile.Read(&sData, sizeof(KSPObj) - sizeof(sData.szScript));
		cDataFile.Read(sData.szScript, sData.nScriptNameLen);
		nKind = GetDataIDKind(sData.nTemplateID);
		if (CheckClientKind(nKind) != 0)
			continue;

		KMapPos sPos;
		KObjItemInfo sInfo;

		sPos.nSubWorld = nSubWorld;
		SubWorld[nSubWorld].Mps2Map(sData.Pos.x, sData.Pos.y, &sPos.nRegion, &sPos.nMapX, &sPos.nMapY, &sPos.nOffX, &sPos.nOffY);
		sPos.nRegion = sPos.nRegion;

		sInfo.m_nItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_szName[0] = 0;
		sInfo.m_nColorID = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		//sInfo.m_sHaveAttack = 0; // 是否设置为攻击无效了
		// sInfo.m_AttackerDwid=0;   //上次攻击着的DWID

		nNo = Add(sData.nTemplateID, sPos, sInfo);
		if (nNo == -1)
			continue;
		if (sData.nDir >= 0)
			Object[nNo].SetDir(sData.nDir);
		if (sData.nState >= 0)
			Object[nNo].SetState(sData.nState);
		Object[nNo].SetScriptFile(sData.szScript);
		if (nIsOutScript)
		{ // 读出所有NPC脚本
			char nTrapInfo[256] = { 0 };
			if (sData.szScript[0])
			{
				if (nMapIndex <= 0)
				{ // 全部
					sprintf(nTrapInfo, "图:%d,名:%s,标:%d/%d,obj:%s", SubWorld[nSubWorld].m_SubWorldID, Object[nNo].m_szName, sData.Pos.x / 8 / 32, sData.Pos.y / 16 / 32, sData.szScript);
					//g_LogFile.gTraceLogFile(nTrapInfo, sizeof(nTrapInfo));
				}
				else
				{
					if (SubWorld[nSubWorld].m_SubWorldID == nMapIndex)
					{ // 某个地图
						sprintf(nTrapInfo, "图:%d,名:%s,标:%d/%d,obj:%s", SubWorld[nSubWorld].m_SubWorldID, Object[nNo].m_szName, sData.Pos.x / 8 / 32, sData.Pos.y / 16 / 32, sData.szScript);
						//g_LogFile.gTraceLogFile(nTrapInfo, sizeof(nTrapInfo));
					}
				}
			}
		}
	}

	cDataFile.Close();

	return TRUE;
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£º·þÎñÆ÷¶ËÔØÈëÒ»¸öRegionµÄËùÓÐObj
//---------------------------------------------------------------------------
BOOL	KObjSet::ServerLoadRegionObj(INT nSubWorld, KPakFile *pFile, DWORD dwDataSize)
{
	if (!pFile || dwDataSize < sizeof(KObjFileHead))
		return FALSE;

	KObjFileHead	sHead;
	KSPObj			sData;
	INT				nKind, nNo;
	KMapPos			sPos;
	KObjItemInfo	sInfo;

	pFile->Read(&sHead, sizeof(KObjFileHead));
	for (INT i = 0; i < sHead.uNumObj; i++)
	{
		pFile->Read(&sData, sizeof(KSPObj) - sizeof(sData.szScript));
		if (sData.nScriptNameLen < sizeof(sData.szScript))
		{
			pFile->Read(sData.szScript, sData.nScriptNameLen);
			sData.szScript[sData.nScriptNameLen] = 0;
		}
		else
		{
			sData.szScript[0] = 0;
			pFile->Seek(sData.nScriptNameLen, FILE_CURRENT);
		}
		nKind = GetDataIDKind(sData.nTemplateID);
		//if (CheckClientKind(nKind) != 0)
		if (CheckClientKind(nKind) != 1)
			continue;
		sPos.nSubWorld = nSubWorld;
		SubWorld[nSubWorld].Mps2Map(sData.Pos.x, sData.Pos.y, &sPos.nRegion, &sPos.nMapX, &sPos.nMapY, &sPos.nOffX, &sPos.nOffY);
		sPos.nRegion = sPos.nRegion;

		sInfo.m_nItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_szName[0] = 0;
		sInfo.m_nColorID = 0;
		sInfo.m_nGenre = 0;
		sInfo.m_nDetailType = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		sInfo.m_bOverLook = 0;
		nNo = Add(sData.nTemplateID, sPos, sInfo);
		if (nNo == -1)
			continue;
		if (sData.nDir >= 0)
			Object[nNo].SetImageDir(sData.nDir);
		if (sData.nState >= 0)
			Object[nNo].SetState(sData.nState);
			
		Object[nNo].SetScriptFile(sData.szScript);
	}

	return TRUE;
}
#endif

#ifndef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍ¨¹ýobjµÄdataidÌí¼ÓÒ»¸öobj
//---------------------------------------------------------------------------
INT		KObjSet::AddData(INT nDataID, INT nSubWorld, INT nRegion, INT nMapX, INT nMapY, INT nOffX, INT nOffY)
{
	if (nDataID <= 0 || nDataID >= m_cTabFile.GetHeight())
		return -1;
	if (nSubWorld < 0 || nSubWorld >= MAX_SUBWORLD)
		return -1;
	if (nRegion < 0)
		return -1;

	INT		nFreeNo, i, nTemp;
	char	szBuffer[80];

	nFreeNo = FindFree();
	if (nFreeNo <= 0)
		return -1;

	Object[nFreeNo].Release();
	Object[nFreeNo].m_nDataID = nDataID;
	m_cTabFile.GetString(nDataID + 1, ObjDataField_Kind, g_szObjKind[0], szBuffer, sizeof(szBuffer));
	for (i = 0; i < Obj_Kind_Num; i++)
	{
		if (*(DWORD*)(&szBuffer) == g_dwObjKindNum[i])
			break;
	}
	if (i >= Obj_Kind_Num)
		i = 0;
	Object[nFreeNo].m_nKind = i;
	// ´Ó ObjData ÎÄ¼þÖÐ¶ÁÈ¡Êý¾Ý
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_Layer, 1, &Object[nFreeNo].m_nLayer);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_Height, 0, &Object[nFreeNo].m_nHeight);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LifeTime, 0, &Object[nFreeNo].m_nLifeTime);
	m_cTabFile.GetString(nDataID + 1, ObjDataField_Name, "", Object[nFreeNo].m_szName, sizeof(Object[nFreeNo].m_szName));
	m_cTabFile.GetString(nDataID + 1, ObjDataField_ScriptName, "", szBuffer, sizeof(szBuffer));
	Object[nFreeNo].SetScriptFile(szBuffer);
	m_cTabFile.GetString(nDataID + 1, ObjDataField_SoundName, "", Object[nFreeNo].m_szSoundName, sizeof(Object[nFreeNo].m_szSoundName));

	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightRadius, 0, &Object[nFreeNo].m_sObjLight.m_nRadius);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightRed, 255, &Object[nFreeNo].m_sObjLight.m_nRed);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightGreen, 255, &Object[nFreeNo].m_sObjLight.m_nGreen);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightBlue, 255, &Object[nFreeNo].m_sObjLight.m_nBlue);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightAlpha, 255, &Object[nFreeNo].m_sObjLight.m_nAlpha);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightReflectType, 0, &Object[nFreeNo].m_sObjLight.m_nReflectType);

	INT nTotalFrame, nTotalDir, nCurFrame, nCurDir;
	m_cTabFile.GetString(nDataID + 1, ObjDataField_ImageName, "", Object[nFreeNo].m_szImageName, sizeof(Object[nFreeNo].m_szImageName));
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageTotalFrame, 1, &nTotalFrame);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageTotalDir, 1, &nTotalDir);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCurFrame, 0, &nCurFrame);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCurDir, 0, &nCurDir);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageInterval, 0, (INT*)(&Object[nFreeNo].m_cImage.m_dwInterval));
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCgXpos, 0, &Object[nFreeNo].m_cImage.m_nCgXpos);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCgYpos, 0, &Object[nFreeNo].m_cImage.m_nCgYpos);
	Object[nFreeNo].m_cImage.SetFileName(Object[nFreeNo].m_szImageName);
	Object[nFreeNo].m_cImage.SetTotalFrame(nTotalFrame);
	Object[nFreeNo].m_cImage.SetTotalDir(nTotalDir);
	Object[nFreeNo].m_cImage.SetCurFrame(nCurFrame);
	Object[nFreeNo].m_cImage.SetCurDir(nCurDir);
	Object[nFreeNo].SetDir(Object[nFreeNo].m_cImage.m_nCurDir * 64 / Object[nFreeNo].m_cImage.m_nTotalDir);

	INT nDropTotalFrame, nDropTotalDir, nDropCurFrame, nDropCurDir;
	m_cTabFile.GetString(
		nDataID + 1,
		ObjDataField_ImageDropName,
		Object[nFreeNo].m_szImageName,
		Object[nFreeNo].m_szImageDropName,
		sizeof(Object[nFreeNo].m_szImageDropName));
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropTotalFrame,
		nTotalFrame,
		&nDropTotalFrame);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropTotalDir,
		nTotalDir,
		&nDropTotalDir);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropCurFrame,
		nCurFrame,
		&nDropCurFrame);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropCurDir,
		nCurDir,
		&nDropCurDir);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropInterval,
		Object[nFreeNo].m_cImage.m_dwInterval,
		(INT*)(&Object[nFreeNo].m_cImageDrop.m_dwInterval));
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropCgXpos,
		Object[nFreeNo].m_cImage.m_nCgXpos,
		&Object[nFreeNo].m_cImageDrop.m_nCgXpos);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropCgYpos,
		Object[nFreeNo].m_cImage.m_nCgYpos,
		&Object[nFreeNo].m_cImageDrop.m_nCgYpos);
	Object[nFreeNo].m_cImageDrop.SetFileName(Object[nFreeNo].m_szImageDropName);
	Object[nFreeNo].m_cImageDrop.SetTotalFrame(nDropTotalFrame);
	Object[nFreeNo].m_cImageDrop.SetTotalDir(nDropTotalDir);
	Object[nFreeNo].m_cImageDrop.SetCurFrame(nDropCurFrame);
	Object[nFreeNo].m_cImageDrop.SetCurDir(nDropCurDir);

	for (i = 0; i < OBJ_BAR_SIZE; i++)
	{
		m_cTabFile.GetInteger(nDataID + 1, ObjDataField_Bar0 + i, 0, &nTemp);
		Object[nFreeNo].m_btBar[i] = (BYTE)nTemp;
	}

	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_DrawFlag, 0, &Object[nFreeNo].m_bDrawFlag);

	Object[nFreeNo].m_nIndex = nFreeNo;
	Object[nFreeNo].m_nSubWorldID = nSubWorld;
	Object[nFreeNo].m_nRegionIdx = nRegion;
	Object[nFreeNo].m_nMapX = nMapX;
	Object[nFreeNo].m_nMapY = nMapY;
	Object[nFreeNo].m_nOffX = nOffX;
	Object[nFreeNo].m_nOffY = nOffY;

	Object[nFreeNo].m_nBelongRegion = SubWorld[0].m_Region[nRegion].m_RegionID;

	// if (ÓÐÁËÎï¼þÕÏ°­²ã) Éè¶¨Îï¼þÕÏ°­

	m_UseIdx.Insert(nFreeNo);
	m_FreeIdx.Remove(nFreeNo);
	return nFreeNo;
}
#endif

//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÕÒÃ»ÓÐÊ¹ÓÃµÄ¿ÕÏÐobj
//---------------------------------------------------------------------------
INT		KObjSet::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÅÐ¶ÏÄ³¸öÀàÐÍµÄobjÊÇ²»ÊÇclientonlyÀàÐÍ
//---------------------------------------------------------------------------
INT 	KObjSet::CheckClientKind(INT nKind)
{
	switch (nKind)
	{
	case Obj_Kind_MapObj:
	case Obj_Kind_Body:
	case Obj_Kind_LoopSound:
	case Obj_Kind_RandSound:
	case Obj_Kind_Light:
		return 1;
	case Obj_Kind_Door:
	case Obj_Kind_Trap:
	case Obj_Kind_Prop:
	case Obj_Kind_Box:
	case Obj_Kind_Item:
	case Obj_Kind_Money:
		return 0;
	default:
		return -1;
	}
	return -1;
}
//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÕÒÄ³¸öidµÄobj
//---------------------------------------------------------------------------
INT		KObjSet::FindID(INT nID)
{
	if (nID < OBJ_WORLD_ID_START)
		return 0;
	/*
	for (INT i = 1; i < MAX_OBJECT; i++)
	{
		if ( Object[i].m_nIndex > 0 && Object[i].m_nID == nID)
			return i;
	}*/
	INT i = m_UseIdx.GetNext(0);
	while (i != 0)
	{
		if (Object[i].m_nIndex > 0 && Object[i].m_nID == nID)
			return i;
		i = m_UseIdx.GetNext(i);
	}
	return 0;
}

//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÕÒÄ³¸öÃû×ÖµÄobj
//---------------------------------------------------------------------------
INT		KObjSet::FindName(char *lpszObjName)
{
	if (!lpszObjName || !lpszObjName[0])
		return 0;

	//for (INT i = 1; i < MAX_OBJECT; i++)
	INT i = m_UseIdx.GetNext(0);
	while(i != 0)
	{
		if (Object[i].m_nIndex > 0 && strcmp(Object[i].m_szName, lpszObjName) == 0)
			return i;
		i = m_UseIdx.GetNext(i);
	}
	return 0;
}

//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºÖ»ÄÜÔÚObj µÄ SetWorldID µÄÊ±ºòµ÷ÓÃ£¬ÆäËûÊ±ºò¶¼Ö±½ÓÊ¹ÓÃm_nObjID
//---------------------------------------------------------------------------
INT		KObjSet::GetID()
{
	m_nObjID++;
	return (m_nObjID - 1);
}

void	KObjSet::Remove(INT nIdx)
{
	Object[nIdx].Release();
	m_FreeIdx.Insert(nIdx);
	m_UseIdx.Remove(nIdx);
}

#ifndef _SERVER
void	KObjSet::RemoveIfClientOnly(INT nIdx)
{
	if (CheckClientKind(Object[nIdx].GetKind()) != 1)
		return;
	Object[nIdx].Release();
	m_FreeIdx.Insert(nIdx);
	m_UseIdx.Remove(nIdx);
}
#endif

INT		KObjSet::GetDataIDKind(INT nDataID)
{
	if (nDataID <= 0 || nDataID >= m_cTabFile.GetHeight())
		return -1;
	char	szBuffer[80];
	m_cTabFile.GetString(nDataID + 1, ObjDataField_Kind, g_szObjKind[0], szBuffer, sizeof(szBuffer));
	INT i;
	for (i = 0; i < Obj_Kind_Num; i++)
	{
		if (strcmp(szBuffer, g_szObjKind[i]) == 0)
			break;
	}
	if (i >= Obj_Kind_Num)
		return -1;
	return i;
}

#ifndef _SERVER
INT	KObjSet::SearchObjAt(INT nX, INT nY, INT nRange)
{
	INT nIdx = 0;
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

	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;

		if (Object[nIdx].m_nRegionIdx < 0)
			continue;

		if (Obj_Kind_Box != Object[nIdx].m_nKind
			&& Obj_Kind_Item != Object[nIdx].m_nKind
			&& Obj_Kind_Money != Object[nIdx].m_nKind
			&& Obj_Kind_Prop != Object[nIdx].m_nKind
			&& Obj_Kind_Door != Object[nIdx].m_nKind)
			continue;
		
		INT x, y;
		SubWorld[0].Map2Mps(Object[nIdx].m_nRegionIdx, Object[nIdx].m_nMapX, Object[nIdx].m_nMapY,
			Object[nIdx].m_nOffX, Object[nIdx].m_nOffY, &x, &y);

		if (nSrcY[0] > y)
			continue;

		if (nSrcY[0] < y - 40 - Object[nIdx].m_nHeight)
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
DWORD	KObjSet::GetNameColor(INT nColorID)
{
	if (nColorID < 0 || nColorID >= MAX_OBJ_NAME_COLOR)
		return this->m_dwNameColor[0];
	return this->m_dwNameColor[nColorID];
}
#endif