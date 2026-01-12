#ifndef	KSubWorldSetH
#define	KSubWorldSetH

#include "KTimer.h"
#ifndef _SERVER
#include "KMapMusic.h"
#else
#include "KSubWorld.h"

#endif

struct MAPLIST_INFO
{
	char szName[128];
	INT nKind;
	char szNewWorldScript[128];
};

class CORE_API KSubWorldSet
{
public:
	INT		m_nLoopRate;		// 循环帧数
	INT		m_MapListCount;
	MAPLIST_INFO* m_sMapListInfo;

#ifndef _SERVER
	KMapMusic	m_cMusic;
#endif
private:
	KTimer	m_Timer;			// 计时器
	INT		m_nGameVersion;
#ifndef _SERVER
	DWORD	m_dwPing;
#endif
public:
	KSubWorldSet();
	~KSubWorldSet();
	BOOL		LoadFile();
	INT		SearchWorld(DWORD dwID);
	void		MainLoop();
	void		MessageLoop();
	BOOL		SendMessage(INT nSubWorldID, DWORD	dwMsgType, INT nParam1 = 0, INT nParam2 = 0, INT nParam3 = 0);
	INT		GetGameTime() { return m_nLoopRate; };
	INT		GetGameVersion() { return m_nGameVersion; }
	void		Close();
	void		Offline();
	BOOL		Load(LPSTR szFileName);
	// Server上才有的几个方法
#ifdef _SERVER
	void	GetRevivalPosFromId(DWORD dwSubWorldId, INT nRevivalId, POINT* pPos);
#endif
	// Client上才有的几个方法
#ifndef _SERVER
	void	SetPing(DWORD dwTimer) { m_dwPing = dwTimer; }
	DWORD	GetPing() { return m_dwPing; }
	void	Paint();
#endif
};
extern CORE_API KSubWorldSet g_SubWorldSet;
#endif
