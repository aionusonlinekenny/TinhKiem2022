//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KObjSet.h
// Date:	2002.01.06
// Code:	边城浪子
// Desc:	Obj Class
//---------------------------------------------------------------------------
#ifndef	KObjSetH
#define	KObjSetH

#include "KObj.h"
#include "KLinkArray.h"

#define		MAX_OBJ_NAME_COLOR		16

typedef struct
{
	char	m_szCopyRight[31];
	BYTE	m_btVersion;
	DWORD	m_dwNum;
} KObjMapDataHead;

typedef struct
{
	INT		m_nTemplateID;
	INT		m_nState;
	INT		m_nDir;
	INT		m_nPos[3];
	DWORD	m_dwScriptLength;
	char	m_szScript[80];
} KObjMapData;

typedef struct
{
	INT		m_nItemID;
	INT		m_nMoneyNum;
	INT		m_nItemWidth;
	INT		m_nItemHeight;
	INT		m_nColorID;
	INT		m_nGenre;
	INT		m_nDetailType;
	INT		m_nMovieFlag;
	INT		m_nSoundFlag;
	BOOL		m_bOverLook;
	char		m_szName[FILE_NAME_LENGTH];
	DWORD		m_dwNpcId; // TamLTM Kham nam xanh
	INT			m_cHaveAttack; // Whether it is set to attack is invalid
} KObjItemInfo;

class KObjSet
{
public:
	KTabFile	m_cTabFile;
	KTabFile	m_cMoneyFile;
	INT			m_nObjID;
#ifndef _SERVER
	DWORD		m_dwNameColor[MAX_OBJ_NAME_COLOR];
#endif
private:
	KLinkArray	m_UseIdx;
	KLinkArray	m_FreeIdx;
public:
	KObjSet();
	~KObjSet();
	BOOL	Init();
	INT		GetID();
	void	Remove(INT nIdx);
#ifdef _SERVER
	// 添加一个obj，返回在obj数组中的位置编号（如果 < 0 ，失败）
	INT		Add(INT nDataID, KMapPos MapPos, KObjItemInfo sItemInfo);

	// 从obj数据文件中载入相应数据
	INT		AddData(INT nDataID, KMapPos MapPos, INT nMoneyNum = 0, INT nItemID = 0, INT nItemWidth = 0, INT nItemHeight = 0);

	INT		AddMoneyObj(KMapPos MapPos, INT nMoneyNum);

	// 服务器端载入一个Region的所有Obj
	BOOL	ServerLoadRegionObj(char* lpszMapPath, INT nRegionX, INT nRegionY, INT nSubWorld);

	// 服务器端载入一个Region的所有Obj
	BOOL	ServerLoadRegionObj(INT nSubWorld, KPakFile* pFile, DWORD dwDataSize);

	// 设定obj所带的物件信息，包括物件id，物件在装备栏中的长、宽
	BOOL	SetObjItem(INT nObjIndex, INT nItemID, INT nItemWidth, INT nItemHeight);

	BOOL	SyncAdd(INT nID, INT nClient);

	INT		SearchObjAt(KMapPos MapPos);
#endif

#ifndef _SERVER
	INT		AddData(INT nDataID, INT nSubWorld, INT nRegion, INT nMapX, INT nMapY, INT nOffX, INT nOffY);
	INT		ClientAdd(INT nID, INT nDataID, INT nState, INT nDir, INT nCurFrame, INT nXpos, INT nYpos, KObjItemInfo sInfo, DWORD nObjDwidx = 0);
	BOOL		ClientLoadRegionObj(char* lpszMapPath, INT nRegionX, INT nRegionY, INT nSubWorld, INT nRegion);
	BOOL		ClientLoadRegionObj(KPakFile* pFile, DWORD dwDataSize);
	void		RemoveIfClientOnly(INT nIdx);
	INT		SearchObjAt(INT nX, INT nY, INT nRange);
	DWORD		GetNameColor(INT nColorID);
#endif
	INT		FindID(INT nID);
	INT		FindName(char* lpszObjName);
private:
	INT		FindFree();
	INT		CheckClientKind(INT nKind);
	INT		GetDataIDKind(INT nDataID);
#ifdef _SERVER
	INT		GetMoneyDataId(INT nMoney);
#endif
};

extern	KObjSet	ObjSet;
extern	char	g_szObjKind[Obj_Kind_Num][32];

#endif






