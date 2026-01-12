#ifndef KPLAYERCHATROOM_H
#define KPLAYERCHATROOM_H

#include	"KPlayerDef.h"

#define		MAX_CHATROOM			20
#define		MAX_CHATROOM_MEMBER		50		// 最大队员数量(不包括队长)

#ifdef _SERVER
class KPlayerChatRoom	// 服务器端玩家的组队信息
{
	friend class KPlayer;
public:
	INT		m_nID;				// 如果已经组队，队伍的 id
	INT		m_nFlag;
	INT		m_nFigure;			// 如果已经组队，player 的身份：TEAM_CAPTAIN TEAM_MEMBER

private:
	INT		m_nPlayerIndex;
public:
	KPlayerChatRoom() { m_nPlayerIndex = 0; };
	void	Active();
	void	Init(INT nPlayerIdx);
	void	Release();
	BOOL	CreateChatRoom(char* szRoomName, INT nRoomPw, INT nLefttime, BOOL bIsGmRoom);
	BOOL	AddTime(const char* lpszName, INT nLefttime);
	BOOL	RequestList();
	BOOL	JoinRoom(INT nRoomIndex, INT nRoomPw);
	BOOL	LeaveRoom();
	BOOL	DeleteRoom();
	BOOL	ForceLeaveRoom();
	BOOL	Roomcommand(INT n);
};
#endif

#ifndef _SERVER
class KPlayerChatRoom	// 客户端玩家的组队信息
{
public:
	char	m_szRoomName[10];
	INT		m_nID;
	INT		m_nFlag;
	INT		m_nFigure;

public:
	KPlayerChatRoom();
	void	Release();

	void	JoinRoom(const char* szRoomName, INT nId, CHATROOM_FIGURE eFigure);								// 更新界面显示
};
#endif

// 此队伍是否为空通过判断队长 id 来决定，当为 -1 时为空（每个队伍必定有一个队长）
#ifdef _SERVER
class KChatRoom
{
private:
	INT		m_nIndex;									// 本 Team 在 g_ChatRoom 中的位置
public:
	BOOL	m_bIsGmRoom;
	char	m_szRoomHost[16];
	INT		m_nRoomHost;
	char	m_szRoomName[10];
	INT		m_nRoomLeftTime;
	INT		m_nRoomDiceTime;
	INT		m_nRoomDiceMax;
	INT		m_nRoomPw;
	INT		m_nMember[MAX_CHATROOM_MEMBER];					// 所有队员 id ，服务器端用 player index ，客户端用 npc id ，-1 为空
	INT		m_nMemParam[MAX_CHATROOM_MEMBER];
	INT		m_nMemNum;									// 已有队员数量(不包括队长)
	char	m_szBlackName[MAX_CHATROOM_MEMBER][16];

public:
	KChatRoom();											// 构造函数
	void	Release();									// 清空
	void	RemoveLeftRoom();									// 清空
	void	DeleteRoom();									// 清空
	void	SetIndex(INT nIndex);						// 设定 Team 在 g_ChatRoom 中的位置
	INT		FindFree();									// 寻找队员空位
	INT		FindFreeBlackList();									// 寻找队员空位
	BOOL	OpenGame(INT ntype, INT n);
	BOOL	AddParam(const char* lpszName, INT ntype, INT n);
	BOOL	CheckMemberName(const char* lpszName);				// 寻找具有指定npc id的队员（不包括队长）
	BOOL	CheckBlackList(const char* lpszName);
	BOOL	CreateChatRoom(INT nIdx, const char* szRoomName, INT nRoomPw, INT nLefttime, BOOL bIsGmRoom);// 创建一支队伍
	void	MsgAll(const char* szMsg);
	void	Dice(INT nIsHight);
};

extern	KChatRoom	g_ChatRoom[MAX_CHATROOM];


class KChatRoomSet
{
public:
	void	Init();										// 初始化
	INT		CreateChatRoom(INT nIdx, const char* szRoomName, INT nRoomPw, INT nLefttime, BOOL bIsGmRoom);	// 创建一支队伍
	BOOL	AddTime(const char* lpszName, INT nLefttime);
	BOOL	RequestRoomList(INT nPlayerIndex);
	BOOL	RequestList(INT nPlayerIndex, INT nRoomID);
	INT		JoinRoom(INT nPlayerIndex, INT nRoomIndex, INT nRoomPw);
	BOOL	LeaveRoom(INT nPlayerIndex, INT nRoomID);
	BOOL	DeleteRoom(INT nPlayerIndex, INT nRoomID);
	BOOL	ForceLeaveRoom(INT nPlayerIndex, INT nRoomID);
	BOOL	AddBlackList(INT nPlayerIndex, INT nRoomID, INT nListIndex);
	BOOL	RemoveBlackList(INT nPlayerIndex, INT nRoomID, INT nListIndex);
	BOOL	ChangePw(INT nPlayerIndex, INT nRoomID, INT nRoomPw);
	BOOL	OpenGame(INT nPlayerIndex, INT nRoomID, INT n);
	BOOL	Hight(INT nPlayerIndex, INT nRoomID, INT n);
	BOOL	Low(INT nPlayerIndex, INT nRoomID, INT n);
private:
	INT		FindFree();
	BOOL	CheckHost(const char* lpszHost);					// 判断队名是否可用
	INT		FindName(const char* lpszName);					// 判断队名是否可用
};

extern	KChatRoomSet	g_ChatRoomSet;

#endif

#endif
