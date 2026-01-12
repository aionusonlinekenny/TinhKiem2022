#ifndef	KWorldH
#define	KWorldH

#ifdef _SERVER
#define	MAX_SUBWORLD	200
#else
#define	MAX_SUBWORLD	1
#endif
#define	VOID_REGION		-2
//-------------------------------------------------------------
#include "KEngine.h"
#include "KRegion.h"
#include "KWeatherMgr.h"
#include <map>

#ifdef _SERVER
#include "KMission.h"
#include "KMissionArray.h"
#define	MAX_MISSIONARRAY 2
typedef KMissionArray <KMission, MAX_MISSIONARRAY> KSubWorldMissionArray;
#define MAX_GLOBAL_MISSIONCOUNT 10
typedef KMissionArray <KMission, MAX_GLOBAL_MISSIONCOUNT> KGlobalMissionArray;
extern KGlobalMissionArray g_GlobalMissionArray;
#endif

//-------------------------------------------------------------


#ifndef TOOLVERSION
class KSubWorld
#else

class CORE_API KSubWorld
#endif
{
public:
	INT			m_nIndex;
	INT			m_SubWorldID;
#ifdef _SERVER
	KSubWorldMissionArray m_MissionArray;
	char		m_szNewWorldScript[128];
	char		m_szNewWorldParam[128];
	KLinkArray	m_cActRegionIdx;
#endif
	KRegion* m_Region;
#ifndef _SERVER
	INT			m_ClientRegionIdx[MAX_REGION];
	char		m_szMapPath[FILE_NAME_LENGTH];
	//KLittleMap	m_cLittleMap;
#endif
	INT			m_nWorldRegionWidth;			//	SubWorldÀï¿í¼¸¸öRegion
	INT			m_nWorldRegionHeight;			//	SubWorldÀï¸ß¼¸¸öRegion
	INT			m_nTotalRegion;					//	SubWorldÀïRegion¸öÊý
	INT			m_nRegionWidth;					//	RegionµÄ¸ñ×Ó¿í¶È
	INT			m_nRegionHeight;				//	RegionµÄ¸ñ×Ó¸ß¶È
	INT			m_nCellWidth;					//	CellµÄÏñËØ¿í¶È
	INT			m_nCellHeight;					//	CellµÄÏñËØ¸ß¶È
	INT			m_nRegionBeginX;
	INT			m_nRegionBeginY;
	INT				m_nRegionEndX;                  //  地图的结束位置X Rigth
	INT				m_nRegionEndY;                  //  地图的结束位置Y Buttom
	INT			m_nWeather;						//	ÌìÆø±ä»¯
	DWORD			m_dwCurrentTime;				//	µ±Ç°Ö¡
	KWorldMsg		m_WorldMessage;					//	ÏûÏ¢
	KList			m_NoneRegionNpcList;			//	²»ÔÚµØÍ¼ÉÏµÄNPC

	typedef std::map<INT, std::string> _WorldMapInfo;                 //SkillId,Echance
	_WorldMapInfo nWorldMapInfo;

#ifdef _SERVER
	KWeatherMgr* m_pWeatherMgr;
#endif
private:
public:
	KSubWorld();
	~KSubWorld();
	void			Activate();
	void			GetFreeObjPos(POINT& pos);
	void			GetFreePos(POINT& pos);
	void			GetFreeNewObjPos(POINT& pos);

	BOOL			CanPutObj(POINT pos, INT nModle = 0, BOOL nIsCheckNpc = FALSE);
	void			ObjChangeRegion(INT nSrcRegionIdx, INT nDesRegionIdx, INT nObjIdx);
	void			MissleChangeRegion(INT nSrcRegionIdx, INT nDesRegionIdx, INT nObjIdx);
	void			AddPlayer(INT nRegion, INT nIdx);
	void			RemovePlayer(INT nRegion, INT nIdx);
	void			Close();
	INT				GetDistance(INT nRx1, INT nRy1, INT nRx2, INT nRy2);						// ÏñËØ¼¶×ø±ê
	void			Map2Mps(INT nR, INT nX, INT nY, INT nDx, INT nDy, INT* nRx, INT* nRy);		// ¸ñ×Ó×ø±ê×ªÏñËØ×ø±ê
	void			NewMap2Mps(INT nR, INT nX, INT nY, INT nDx, INT nDy, INT* nRx, INT* nRy);//Convert grid coordinates to pixel coordinates
	static void		Map2Mps(INT nRx, INT nRy, INT nX, INT nY, INT nDx, INT nDy, INT* pnX, INT* pnY);		// ¸ñ×Ó×ø±ê×ªÏñËØ×ø±ê
	void			Mps2Map(INT Rx, INT Ry, INT* nR, INT* nX, INT* nY, INT* nDx, INT* nDy);	// ÏñËØ×ø±ê×ª¸ñ×Ó×ø±ê
	void			GetMps(INT* nX, INT* nY, INT nSpeed, INT nDir, INT nMaxDir = 64);			// È¡µÃÄ³·½ÏòÄ³ËÙ¶ÈÏÂÒ»µãµÄ×ø±ê
	BYTE			TestBarrier(INT nMpsX, INT nMpsY);
	BYTE			TestBarrier(INT nRegion, INT nMapX, INT nMapY, INT nDx, INT nDy, INT nChangeX, INT nChangeY);	// ¼ì²âÏÂÒ»µãÊÇ·ñÎªÕÏ°­
	BYTE			TestBarrierMin(INT nRegion, INT nMapX, INT nMapY, INT nDx, INT nDy, INT nChangeX, INT nChangeY);	// ¼ì²âÏÂÒ»µãÊÇ·ñÎªÕÏ°­
	BYTE			GetBarrier(INT nMpsX, INT nMpsY);											// È¡µÃÄ³µãµÄÕÏ°­ÐÅÏ¢
	DWORD			GetTrap(INT nMpsX, INT nMpsY);
	BOOL			SetTrap(INT nMpsX, INT nMpsY, INT nCellNum = 1, DWORD uTrapScriptId = 0);
	void			MessageLoop();
	INT				FindRegion(INT RegionID);													// ÕÒµ½Ä³IDµÄRegionµÄË÷Òý
	INT				FindFreeRegion(INT nX = 0, INT nY = 0);
	void			RemoveNpc(INT nIdx);
	
#ifdef _SERVER
	void			AddActiveRegion(INT nRegionIdx) { m_cActRegionIdx.Insert(nRegionIdx + 1); }
	void			RemoveActiveRegion(INT nRegionIdx) { m_cActRegionIdx.Remove(nRegionIdx + 1); }
	INT				RevivalAllNpc();//½«µØÍ¼ÉÏËùÓÐµÄNpc°üÀ¨ÒÑËÀÍöµÄNpcÈ«²¿»Ö¸´³ÉÔ­Ê¼×´Ì¬
	void			BroadCast(const char* pBuffer, size_t uSize);
	void			BroadCastRegion(const void* pBuffer, size_t uSize, INT& nMaxCount, INT nRegionIndex, INT nOX, INT nOY);
	INT				LoadMap(INT nIdx);
	void			LoadObject(char* szPath, char* szFile);
	void			NpcChangeRegion(INT nSrcRegionIdx, INT nDesRegionIdx, INT nNpcIdx);
	void			PlayerChangeRegion(INT nSrcRegionIdx, INT nDesRegionIdx, INT nObjIdx);
	BOOL			SendSyncData(INT nIdx, INT nClient);
	INT				GetRegionIndex(INT nRegionID);
	INT				FindNpcFromName(const char* szName);
	INT 			ExecuteScript(DWORD scriptId, char* functionName, INT nParam1, INT nParam2);
	INT 			ExecuteScript(char* scriptFileName, char* functionName, INT nParam, INT worldIndex);
#endif
#ifndef _SERVER
	BOOL			LoadMap(INT nIdx, INT nRegion);
	void			NpcChangeRegion(INT nSrcRegionIdx, INT nDesRegionIdx, INT nNpcIdx);
	void			Paint();
	void			Mps2Screen(INT* Rx, INT* Ry);
	void			Screen2Mps(INT* Rx, INT* Ry);
#endif
private:
	void			LoadTrap();
	void			ProcessMsg(KWorldMsgNode* pMsg);
#ifndef _SERVER
	void			LoadCell();
#endif
};

#ifndef TOOLVERSION
extern KSubWorld	SubWorld[MAX_SUBWORLD];
#else 
extern CORE_API KSubWorld	SubWorld[MAX_SUBWORLD];
#endif
#endif
