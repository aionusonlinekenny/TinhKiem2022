#ifndef	KRegionH
#define	KRegionH

//-----------------------------------------------------------------------
#include "KEngine.h"
#include "KWorldMsg.h"
#include <vector>
#include <map>
//-----------------------------------------------------------------------
#ifdef _SERVER
#define	MAX_BROADCAST_COUNT		100
#else
#define	MAX_REGION		9
#endif

#define	REGION_GRID_WIDTH	16
#define	REGION_GRID_HEIGHT	32

#define REGION_WIDTH 16
#define REGION_HEIGHT 32

enum MOVE_OBJ_KIND
{
	obj_npc,
	obj_object,
	obj_obstacle,
	obj_missle,
};
#ifndef TOOLVERSION
class KRegion
#else
class CORE_API KRegion
#endif
{
	friend class	KSubWorld;
public:
	typedef			std::vector<INT> VEC_LIST;
	INT				m_nSubMapidx; // Which map does it belong to?
	INT			m_nIndex;							// µØÍ¼Ë÷Òý
	INT			m_RegionID;
	KList			m_NpcList;							// ÈËÎïÁÐ±í
	VEC_LIST		m_ObjList;		  // 物件列表
	//KList			m_ObjList;							// Îï¼þÁÐ±í
	KList			m_MissleList;						// ×Óµ¯ÁÐ±í
	KList			m_PlayerList;						// Íæ¼ÒÁÐ±í
	INT			m_nConnectRegion[8];				// ÏàÁÚµÄµØÍ¼Ë÷Òý
	INT			m_nConRegionID[8];					// ÏàÁÚµÄµØÍ¼ID
	KRegion*		m_pConRegion[8];					// Adjacent Region
	INT			m_nRegionX;							// ÔÚÊÀ½çÖÐµÄÎ»ÖÃX£¨ÏóËØµã£©
	INT			m_nRegionY;							// ÔÚÊÀ½çÖÐµÄÎ»ÖÃY£¨ÏóËØµã£©
	INT			m_nWidth;
	INT			m_nHeight;
private:

	enum _KEACT_TYPE
	{
		emACT_TYPE_NONE = 0, // 一切活动终止
		emACT_TYPE_WAIT,	 // 等待终止，可能尚有Missile、Obj未消失
		emACT_TYPE_ALL,		 // 全部活动，周围有玩家
	};

	class _KGridData
	{
	public:
		_KGridData()
		{
			ZeroStruct(m_adwDataFlag);
		}

		BOOL HasData(INT nX, INT nY) CONST
		{
			return (m_adwDataFlag[nX] & (1 << nY)) != 0;
		}
		INT GetData(INT nX, INT nY) CONST
		{
			INT nResult = 0;
			// KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
			// KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
			if (nX < 0 || nX >= REGION_WIDTH)
				goto Exit0;
			if (nY < 0 || nY >= REGION_HEIGHT)
				goto Exit0;

			if (m_adwDataFlag[nX] & (1 << nY)) //
			{
				nResult = m_mapData.find(MAKELONG(nX, nY))->second;
			}
		Exit0:
			return nResult;
		}
		BOOL SetData(INT nX, INT nY, INT nData)
		{
			DWORD dwId = MAKELONG(nX, nY);
			// KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
			// KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
			if (nX < 0 || nX >= REGION_WIDTH)
				goto Exit0;
			if (nY < 0 || nY >= REGION_HEIGHT)
				goto Exit0;

			if (nData)
			{
				m_adwDataFlag[nX] |= 1 << nY;
				m_mapData[dwId] = nData;
			}
			else
			{
				m_adwDataFlag[nX] &= ~(1 << nY);
				m_mapData.erase(dwId);
			}
		Exit0:
			return TRUE;
		}
		BOOL IncData(INT nX, INT nY)
		{
			DWORD dwId = MAKELONG(nX, nY);
			// KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
			// KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
			if (nX < 0 || nX >= REGION_WIDTH)
				goto Exit0;
			if (nY < 0 || nY >= REGION_HEIGHT)
				goto Exit0;

			if (m_adwDataFlag[nX] & (1 << nY)) // 已经有的，+1
			{
				m_mapData[dwId]++;
			}
			else // 新的，直接置1
			{
				m_adwDataFlag[nX] |= 1 << nY;
				m_mapData[dwId] = 1;
			}
		Exit0:
			return TRUE;
		}
		BOOL DecData(INT nX, INT nY)
		{
			BOOL bResult = FALSE;
			DWORD dwId = MAKELONG(nX, nY);
			// KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
			// KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
			if (nX < 0 || nX >= REGION_WIDTH)
				goto Exit0;
			if (nY < 0 || nY >= REGION_HEIGHT)
				goto Exit0;

			// KGLOG_PROCESS_ERROR(m_adwDataFlag[nX] & (1 << nY));
			if (m_adwDataFlag[nX] & (1 << nY))
			{
				if (--m_mapData[dwId] == 0) // 减为0了
				{
					m_mapData.erase(dwId);
					m_adwDataFlag[nX] &= ~(1 << nY);
				}
			}
		Exit0:
			return bResult;
		}

		BOOL Clear()
		{
			ZeroStruct(m_adwDataFlag);
			m_mapData.clear();
			return TRUE;
		}

	private:
		DWORD m_adwDataFlag[REGION_WIDTH]; // 用位来标记某点是否有数据（刚好REGION_HEIGHT == 32，用一个DWORD）
		std::map<DWORD, INT> m_mapData;
	};
#ifdef _SERVER
	//LONG		m_Obstacle[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];	// µØÍ¼ÕÏ°­ÐÅÏ¢±í
	//DWORD		m_dwTrap[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];	// µØÍ¼trapÐÅÏ¢±í
	INT				m_nNearbyPlayerCount;	 // Number of nearby players
	INT				m_nNearbyActiveNpcCount; // Number of nearby active NPCs
	_KEACT_TYPE		m_eActType;		 // activation type
	_KGridData		m_cObstacle;		 // Obstacle information on each grid
	_KGridData		m_cTrap;			 // Trap information on each grid
#endif
	INT			m_nNpcSyncCounter;					// Í¬²½¼ÆÊýÆ÷
	INT			m_nObjSyncCounter;
	INT			m_nActive;							// ÊÇ·ñ¼¤»î£¨ÊÇ·ñÓÐÍæ¼ÒÔÚ¸½½ü£©
	//BYTE* m_pNpcRef;							// Ã¿¸ö¸ñ×ÓÉÏµÄNPCÊýÄ¿
	//BYTE* m_pObjRef;							// Ã¿¸ö¸ñ×ÓÉÏµÄOBJÊýÄ¿
	//BYTE* m_pObstacleRef;							// Ã¿¸ö¸ñ×ÓÉÏµÄOBJÊýÄ¿
	//BYTE* m_pMslRef;							// Ã¿¸ö¸ñ×ÓÉÏµÄMISSLEÊýÄ¿
	_KGridData		m_cNpcRef; // The number of NPCs on each grid
	_KGridData		m_cObjRef; // The number of OBJs on each grid
public:
	KRegion();
	~KRegion();
	BOOL			Init(INT nWidth, INT nHeight);
	BOOL			Load(INT nX, INT nY);
#ifdef _SERVER
	// ÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾region µÄ objectÊý¾Ý£¨°üÀ¨npc¡¢trap¡¢boxµÈ£©
	INT				LoadObject(INT nSubWorld, INT nX, INT nY, KMapList* pMapListInfo);
	// ÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾ region µÄÕÏ°­Êý¾Ý
	BOOL			LoadServerObstacle(KPakFile* pFile, DWORD dwDataSize);
	// ÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾ region µÄ trap Êý¾Ý
	BOOL			LoadServerTrap(KPakFile* pFile, DWORD dwDataSize);
	// ÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾ region µÄ npc Êý¾Ý
	INT				LoadServerNpc(INT nSubWorld, KPakFile* pFile, DWORD dwDataSize, KMapList* pMapListInfo);
	// ÔØÈë·þÎñÆ÷¶ËµØÍ¼ÉÏ±¾ region µÄ obj Êý¾Ý
	BOOL			LoadServerObj(INT nSubWorld, KPakFile* pFile, DWORD dwDataSize);

	BOOL			AddNearByNpc(INT nAdd, BOOL bConRegion = TRUE); // Increase the count of active NPCs in the area (since the command needs to dynamically change the active state of the NPC, it cannot be a private member)
#endif

#ifndef _SERVER
	// Load the object data of this region on the client map (including npc, box, etc.)
	// If bLoadNpcFlag == TRUE needs to load clientonly npc else does not load
	BOOL			LoadObject(INT nSubWorld, INT nX, INT nY, char* lpszPath);
	// Load the clientonlynpc data of this region on the client map
	BOOL			LoadClientNpc(KPakFile* pFile, DWORD dwDataSize);
	// Load the clientonlyobj data of this region on the client map
	BOOL			LoadClientObj(KPakFile* pFile, DWORD dwDataSize);
	// Load obstacle data to the minimap
	static void		LoadLittleMapData(INT nX, INT nY, char* lpszPath, BYTE* lpbtObstacle);
#endif
	void			Close();
	void			Activate();
	void			NewRegActivate();
	void			StateActivate();
	void			ActivateObj(INT mSubWorldID);
	BYTE			GetBarrier(INT MapX, INT MapY, INT nDx, INT nDy);	//	µØÍ¼¸ß¶È
	BYTE			GetNewBarrier(INT nMapX, INT nMapY, INT nDx, INT nDy, BOOL nIsCheckNpc = FALSE);
	// °´ ÏñËØµã×ø±ê * 1024 µÄ¾«¶ÈÅÐ¶ÏÄ³¸öÎ»ÖÃÊÇ·ñÕÏ°­
	// ²ÎÊý£ºnGridX nGirdY £º±¾region¸ñ×Ó×ø±ê
	// ²ÎÊý£ºnOffX nOffY £º¸ñ×ÓÄÚµÄÆ«ÒÆÁ¿(ÏñËØµã * 1024 ¾«¶È)
	// ²ÎÊý£ºbCheckNpc £ºÊÇ·ñÅÐ¶ÏnpcÐÎ³ÉµÄÕÏ°­
	// ·µ»ØÖµ£ºÕÏ°­ÀàÐÍ(if ÀàÐÍ == Obstacle_NULL ÎÞÕÏ°­)
	BYTE			GetBarrierMin(INT nGridX, INT nGridY, INT nOffX, INT nOffY, BOOL bCheckNpc);
	BYTE			GetBarrierNewMin(INT nGridX, INT nGridY, INT nOffX, INT nOffY, BOOL bCheckNpc);

	BOOL			SetTrap(INT MapX, INT MapY, INT nCellNum = 1, DWORD uTrapScriptId = 0);
	DWORD			GetTrap(INT MapX, INT MapY);						//	µÃµ½Trap±àºÅ

	BOOL IsActiveAll(VOID)
	{
#ifdef _SERVER
		return m_eActType == emACT_TYPE_ALL;
#else
		return TRUE;
#endif
	}

	inline BOOL		IsActive()
	{
#ifdef _SERVER
		return m_nActive;
#else
		return TRUE;
#endif
	};

	BOOL HasNpc(INT nMapX, INT nMapY) CONST { return m_cNpcRef.HasData(nMapX, nMapY); }
	INT GetNpcRef(INT nMapX, INT nMapY, INT nType = 1) CONST { return m_cNpcRef.HasData(nMapX, nMapY); }
	BOOL AddNpcRef(INT nMapX, INT nMapY, INT nType = 1) { return m_cNpcRef.IncData(nMapX, nMapY); }
	BOOL DecNpcRef(INT nMapX, INT nMapY, INT nType = 1) { return m_cNpcRef.DecData(nMapX, nMapY); }

	BOOL HasObj(INT nMapX, INT nMapY) CONST { return m_cObjRef.HasData(nMapX, nMapY); }
	INT GetObjRef(INT nMapX, INT nMapY, INT nType = 1) CONST { return m_cObjRef.GetData(nMapX, nMapY); }
	BOOL AddObjRef(INT nMapX, INT nMapY, INT nType = 1) { return m_cObjRef.IncData(nMapX, nMapY); }
	BOOL DecObjRef(INT nMapX, INT nMapY, INT nType = 1) { return m_cObjRef.DecData(nMapX, nMapY); }

	/*INT			GetRef(INT nMapX, INT nMapY, MOVE_OBJ_KIND nType);
	BOOL			AddRef(INT nMapX, INT nMapY, MOVE_OBJ_KIND nType);
	BOOL			DecRef(INT nMapX, INT nMapY, MOVE_OBJ_KIND nType);*/
	INT			FindNpc(INT nMapX, INT nMapY, INT nNpcIdx, INT nRelation);
	INT			FindNpcNear(INT nX1, INT nY1);
	INT			FindObject(INT nMapX, INT nMapY, bool bAutoFind = false);
	INT			FindObjectNear(INT nX1, INT nY1);
	INT			FindObject(INT nObjID);
	VEC_LIST::iterator GetObjNode(INT nIdx);
	INT			SearchNpc(DWORD dwNpcID);		// Ñ°ÕÒ±¾ÇøÓòÄÚÊÇ·ñÓÐÄ³¸öÖ¸¶¨ id µÄ npc (zroc add)
	INT			SearchNpcSettingIdx(INT  nNpcSettingIdx);
	INT			SearchNpcName(const char* szName);
#ifdef _SERVER
	void			SendSyncData(INT nClient);
	void			BroadCast(const void* pBuffer, DWORD dwSize, INT& nMaxCount, INT nX, INT nY);
	INT			FindPlayer(DWORD dwId);
	BOOL			AddObstacle(INT nX, INT nY, INT nObstacleKind); // 往某个格子上添加一个障碍
	BOOL			ClearObstacle(INT nX, INT nY);					 // 清除某个格子上的障碍
	BOOL			AddNearbyPlayer(INT nAdd, BOOL bConRegion = TRUE); // Increase the count of active players in the area
	BOOL			UpdateActiveType();								// 更新激活类型（根据附近玩家、Obj、Missile）
#endif

#ifndef _SERVER
	void		Paint();
#endif
	void		AddNpc(INT nIdx);
	void		RemoveNpc(INT nIdx);
	void		AddMissle(INT nIdx);
	void		RemoveMissle(INT nIdx);
	void		AddObj(INT nIdx);
	void		RemoveObj(INT nIdx);
	BOOL		AddPlayer(INT nIdx);
	BOOL		RemovePlayer(INT nIdx);

};

//--------------------------------------------------------------------------
//	Find Npc
//--------------------------------------------------------------------------
inline INT KRegion::FindNpc(INT nMapX, INT nMapY, INT nNpcIdx, INT nRelation)
{
	// Kiểm tra đối tượng có hợp lệ không
	if (this == nullptr)
		return 0;

	// Kiểm tra giới hạn của mảng m_pNpcRef
	if (nMapX < 0 || nMapX >= m_nWidth || nMapY < 0 || nMapY >= m_nHeight)
		return 0;

	// Kiểm tra giá trị của m_pNpcRef tại vị trí (nMapX, nMapY)
	/*if (m_pNpcRef[nMapY * m_nWidth + nMapX] == 0)
		return 0;*/

	if (GetNpcRef(nMapX, nMapY) <= 0)
		return 0;

	// Duyệt qua danh sách NpcList
	KIndexNode* pNode = static_cast<KIndexNode*>(m_NpcList.GetHead());

	while (pNode != nullptr)
	{
		// Kiểm tra giới hạn của mảng Npc
		if (pNode->m_nIndex >= 0 && pNode->m_nIndex < MAX_NPC)
		{
			// Kiểm tra vị trí của Npc
			if (Npc[pNode->m_nIndex].m_MapX == nMapX && Npc[pNode->m_nIndex].m_MapY == nMapY)
			{
				// Kiểm tra mối quan hệ
				if (NpcSet.GetRelation(nNpcIdx, pNode->m_nIndex) & nRelation)
				{
					return pNode->m_nIndex;
				}
			}
		}

		pNode = static_cast<KIndexNode*>(pNode->GetNext());
	}

	return 0;
}
#endif
