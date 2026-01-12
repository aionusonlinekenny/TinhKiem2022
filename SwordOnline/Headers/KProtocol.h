#ifndef KPROTOCOL_H
#define KPROTOCOL_H

#ifndef __linux
#ifdef _STANDALONE
#include "GameDataDef.h"
#else
#include "../Sources/Core/src/GameDataDef.h"
#endif
#else
#include "GameDataDef.h"
#include <string.h>
#endif

#include "KProtocolDef.h"
#include "KRelayProtocol.h"

#pragma pack(push, enter_protocol)
#pragma	pack(1)

#define	PROTOCOL_MSG_TYPE	BYTE
#define PROTOCOL_MSG_SIZE	(sizeof(PROTOCOL_MSG_TYPE))
#define	MAX_PROTOCOL_NUM	210//207

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		TeamServerID;
	BYTE	HelmType;
	BYTE	ArmorType;
	BYTE	WeaponType;
	BYTE	HorseType;
	BYTE	RankID;
	KExpandRank		ExpandRank;
	DWORD	TongNameID;
	char	TongName[16];
	int32_t		TongNationalEmblem;
	char	TongAgname[16];
	BYTE	m_Figure;
	BYTE	m_btSomeFlag;
	BYTE	Translife;
	BYTE	Viprank;
	int32_t		Repute;
	int32_t		FuYuan;
	BYTE	PKFlag;
	BYTE	PKValue;
	BYTE	MantleLevel;
	int32_t		MaskType;
	BYTE	MantleType;
	int32_t		HelmPlayer;
	PLAYERTRADE	Trade;
	char	MateName[16];
	int32_t 	RankInWorld;
	BYTE	PacePer;
	int32_t		FactionNumber;
	BYTE	ImagePlayer;
} PLAYER_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		TeamServerID;
	BYTE	HelmType;
	BYTE	ArmorType;
	BYTE	WeaponType;
	BYTE	HorseType;
	BYTE	RankID;
	KExpandRank		ExpandRank;
	DWORD	TongNameID;
	char	TongName[16];
	int32_t		TongNationalEmblem;
	char	TongAgname[16];
	BYTE	m_Figure;
	BYTE	m_btSomeFlag;
	BYTE	Translife;
	BYTE	Viprank;
	int32_t		Repute;
	int32_t		FuYuan;
	BYTE	PKFlag;
	BYTE	PKValue;
	BYTE	MantleLevel;
	int32_t		MaskType;
	BYTE	MantleType;
	PLAYERTRADE	Trade;
	char	MateName[16];
	int32_t 	RankInWorld;
	BYTE	PacePer;
	BYTE	ImagePlayer;
	int32_t		FactionNumber;
} PLAYER_NORMAL_SYNC;


typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	Camp;				// ÕóÓª
	BYTE	CurrentCamp;		// µ±Ç°ÕóÓª
	BYTE	m_bySeries;			// ÎåÐÐÏµ
	DWORD		CurrentLife;
	DWORD		CurrentLifeMax;
	DWORD		CurrentMana;
	DWORD		CurrentManaMax;
	BYTE	m_btMenuState;		// ×é¶Ó¡¢½»Ò×µÈ×´Ì¬
	BYTE	m_Doing;			// ÐÐÎª
	BYTE	m_btKind;			// npcÀàÐÍ
	BYTE	Special;			// ¼ÓÇ¿µÄNPC£¨ºóÃæµÄbit±íÊ¾¼ÓÇ¿ÀàÐÍ£¬Ãû×ÖÊÇ·ñ½ðÉ«Ö®ÀàµÄÓÉ¼ÓÇ¿µÄÊýÄ¿ÔÚ¿Í»§¶ËÈ·¶¨£©
	int32_t		MissionGroup;
	int32_t		Dir;
	DWORD	MapX;				// Î»ÖÃÐÅÏ¢
	DWORD	MapY;				// Î»ÖÃÐÅÏ¢
	DWORD	ID;					// NpcµÄÎ¨Ò»ID
	int32_t		NpcSettingIdx;		// ¿Í»§¶ËÓÃÓÚ¼ÓÔØÍæ¼Ò×ÊÔ´¼°»ù´¡ÊýÖµÉè¶¨
	char	m_szName[64];		// Ãû×Ö
	WORD	byIgnoreBarrier;
	WORD	wDx;						// Ä¿µÄµØXÆ«ÒÆ
	WORD	wDy;						// Ä¿µÄµØYÆ«ÒÆ
} NPC_SYNC;

//TamLTM fix pos NPC
typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	DWORD	MapX;
	DWORD	MapY;
	BYTE	Doing;
} NPC_POS_SYNC;
//end code

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		MapX;
	int32_t		MapY;
	BYTE	Camp;
	int32_t		CurrentLife;
	int32_t		CurrentLifeMax;
	BYTE	Doing;
	BYTE	State;
	BYTE	WalkSpeed;
	BYTE	RunSpeed;
	WORD	AttackSpeed;
	WORD	CastSpeed;
	BYTE	StateInfo[MAX_SKILL_STATE];
} NPC_NORMAL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		MapX;
	int32_t		MapY;
	int32_t		OffX;
	int32_t		OffY;
} NPC_PLAYER_TYPE_NORMAL_SYNC;

typedef struct
{
	BYTE ProtocolType;
	UINT m_dwNpcID;
	WORD m_wGoldFlag;
} NPC_GOLD_CHANGE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
} NPC_SIT_SYNC, NPC_DEATH_SYNC, NPC_REQUEST_COMMAND, NPC_REQUEST_FAIL, NPC_MASK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BOOL	Rv;
} NPC_REMOVE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		nMpsX;
	int32_t		nMpsY;
} NPC_WALK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Type;
} NPC_REVIVE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		nMpsX;
	int32_t		nMpsY;
} NPC_JUMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		nMpsX;
	int32_t		nMpsY;
} NPC_RUN_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		nFrames;
	int32_t		nX;
	int32_t		nY;
} NPC_HURT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Camp;
} NPC_CHGCURCAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	BYTE	Camp;
} NPC_CHGCAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int32_t		nSkillID;
	int32_t		nSkillLevel;
	int32_t		nMpsX;
	int32_t		nMpsY;
} NPC_SKILL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	char	szMsg[MAX_SENTENCE_LENGTH];
	int32_t		nMsgLen;
} NPC_CHAT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	int32_t		nSkillID;
	int32_t		nMpsX;
	int32_t		nMpsY;
	INT			nPosX;
	INT			nPosY;
} NPC_SKILL_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int32_t		nMpsX;
	int32_t		nMpsY;
} NPC_WALK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
} NPC_REVIVE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int32_t		nMpsX;
	int32_t		nMpsY;
} NPC_RUN_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	BuySellInfo m_BuySellInfo;
} SALE_BOX_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BOOL	bEquipEx;
	int32_t		nBoxIndex;
} SOME_BOX_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	int32_t		nNpcId;
} PLAYER_DIALOG_NPC_COMMAND; //Ö÷½ÇÓënNpcId¶Ô»°µÄÇëÇó

typedef struct
{
	BYTE ProtocolType;		// agreement type
	UINT m_dwID;			// friend id
} CHAT_FRIEND_OFFLINE_SYNC; // Notify the client that a friend is offline

typedef struct
{
	BYTE ProtocolType;	   // 协议类型
	UINT m_dwID;		   // 删除id
} CHAT_DELETE_FRIEND_SYNC; // 被某个聊天好友删除

typedef struct
{
	BYTE ProtocolType; // 协议类型
	WORD m_wLength;
	UINT m_dwID;			   // 好友 id
	int m_nPlayerIdx;		   // 好友 player index
	char m_szName[32];		   // 好友名字
} CHAT_LOGIN_FRIEND_NAME_SYNC; // 玩家登录时发送玩家聊天好友数据（带名字）

typedef struct
{
	BYTE ProtocolType;			 // 协议类型
	UINT m_dwID;				 // 好友 id
	int m_nPlayerIdx;			 // 好友 player index
} CHAT_LOGIN_FRIEND_NONAME_SYNC; // 玩家登录时发送玩家聊天好友数据（不带名字）

typedef struct
{
	BYTE ProtocolType;		 // 协议类型
	UINT m_dwID;			 // 好友 id
	int m_nPlayerIdx;		 // 好友 player index
	char m_szName[32];		 // 好友名字
} CHAT_ONE_FRIEND_DATA_SYNC; // 发送玩家某一个聊天好友数据（带名字）

typedef struct
{
	BYTE ProtocolType;	// 协议类型
	UINT m_dwID;		// 新添加好友的 id
	int m_nIdx;			// 新添加好友在 player 数组中的位置
	char m_szName[32];	// 新添加好友的名字
} CHAT_ADD_FRIEND_SYNC; // 通知客户端成功添加一个聊天好友

typedef struct PLAYER_SEND_CHAT_DATA_SYNC
{
	BYTE ProtocolType; // 协议名称
	WORD m_wLength;
	BYTE m_btCurChannel;												   // 当前聊天状态
	BYTE m_btNameLen;													   // 名字长度
	BYTE m_btChatPrefixLen;												   // 控制字符长度
	WORD m_wSentenceLen;												   // 聊天语句长度
	UINT m_dwSourceID;													   //
	char m_szSentence[32 + CHAT_MSG_PREFIX_MAX_LEN + MAX_SENTENCE_LENGTH]; // 聊天语句内容
	PLAYER_SEND_CHAT_DATA_SYNC() { memset(m_szSentence, 0, sizeof(m_szSentence)); };
} PLAYER_SEND_CHAT_SYNC; // 客户端聊天内容发送给服务器

typedef struct
{
	BYTE ProtocolType;	   // 协议类型
	UINT m_dwID;		   // 好友 id
	int m_nPlayerIdx;	   // 好友 player index
} CHAT_FRIEND_ONLINE_SYNC; // 通知客户端有好友上线

typedef struct PLAYER_SEND_CHAT_DATA_COMMAND
{
	BYTE ProtocolType; // 协议名称
	WORD m_wLength;
	BYTE m_btCurChannel;											  // 当前聊天频道
	BYTE m_btType;													  // MSG_G_CHAT 或 MSG_G_CMD 或……
	BYTE m_btChatPrefixLen;											  // 格式控制字符长度
	WORD m_wSentenceLen;											  // 聊天语句长度
	UINT m_dwTargetID;												  // 聊天对象 id
	int m_nTargetIdx;												  // 聊天对象在服务器端的 idx
	char m_szSentence[MAX_SENTENCE_LENGTH + CHAT_MSG_PREFIX_MAX_LEN]; // 聊天语句内容
	PLAYER_SEND_CHAT_DATA_COMMAND() { memset(m_szSentence, 0, sizeof(m_szSentence)); };
} PLAYER_SEND_CHAT_COMMAND; // 客户端聊天内容发送给服务器

typedef struct
{
	BYTE ProtocolType; // 协议类型
	WORD m_wLength;	   // 长度
	BYTE m_btError;	   // 错误类型	0 对方关闭了此频道，1 找不到对方
	char m_szName[32]; // 对方名字
} CHAT_SCREENSINGLE_ERROR_SYNC;

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	int32_t		m_nExp;				// µ±Ç°¾­Ñé
} PLAYER_EXP_SYNC;				// Íæ¼ÒÍ¬²½¾­Ñé

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
} PLAYER_APPLY_CREATE_TEAM;		// ¿Í»§¶ËÍæ¼Ò´´½¨¶ÓÎé£¬Ïò·þÎñÆ÷·¢ÇëÇó

struct PLAYER_SEND_CREATE_TEAM_SUCCESS
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	DWORD	nTeamServerID;		// ¶ÓÎéÔÚ·þÎñÆ÷ÉÏµÄÎ¨Ò»±êÊ¶
	PLAYER_SEND_CREATE_TEAM_SUCCESS() {
		nTeamServerID = -1;
		ProtocolType = NULL;
	}
};	// ·þÎñÆ÷Í¨ÖªÍæ¼Ò¶ÓÎé´´½¨³É¹¦

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btErrorID;		// ¶ÓÎé´´½¨²»³É¹¦Ô­Òò£º0 Í¬Ãû 1 Íæ¼Ò±¾ÉíÒÑ¾­ÊôÓÚÄ³Ò»Ö§¶ÓÎé 3 µ±Ç°´¦ÓÚ²»ÄÜ×é¶Ó×´Ì¬
} PLAYER_SEND_CREATE_TEAM_FALSE;// ·þÎñÆ÷Í¨Öª¿Í»§¶Ë¶ÓÎé´´½¨²»³É¹¦

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	DWORD	m_dwTarNpcID;		// ²éÑ¯Ä¿±ê npc id
} PLAYER_APPLY_TEAM_INFO;		// ¿Í»§¶ËÏò·þÎñÆ÷ÉêÇë²éÑ¯Ä³¸önpcµÄ×é¶ÓÇé¿ö

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
} PLAYER_APPLY_TEAM_INFO_FALSE;	// ·þÎñÆ÷¸æÖª¿Í»§¶ËÉêÇë²éÑ¯Ä³¸önpcµÄ×é¶ÓÇé¿öÊ§°Ü

typedef struct PLAYER_SEND_TEAM_INFO_DATA
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	int32_t		m_nCaptain;			// ¶Ó³¤ npc id
	int32_t		m_nMember[MAX_TEAM_MEMBER];	// ËùÓÐ¶ÓÔ± npc id
	DWORD	nTeamServerID;		// ¶ÓÎéÔÚ·þÎñÆ÷ÉÏµÄÎ¨Ò»±êÊ¶
	PLAYER_SEND_TEAM_INFO_DATA() {
		nTeamServerID = -1;
		ProtocolType = NULL;
		m_nCaptain = 0;
		memset(m_nMember, 0, sizeof(m_nMember));
	};
} PLAYER_SEND_TEAM_INFO;		// ·þÎñÆ÷Ïò¿Í»§¶Ë·¢ËÍÄ³¸ö¶ÓÎéµÄÐÅÏ¢Êý¾Ý

typedef struct PLAYER_SEND_SELF_TEAM_INFO_DATA
{
	BYTE	ProtocolType;							// Ð­ÒéÃû³Æ
	BYTE	m_btState;								// ¶ÓÎé×´Ì¬
	DWORD	m_dwNpcID[MAX_TEAM_MEMBER + 1];			// Ã¿Ãû³ÉÔ±µÄnpc id £¨¶Ó³¤·ÅÔÚµÚÒ»Î»£©
	char	m_szNpcName[MAX_TEAM_MEMBER + 1][32];	// Ã¿Ãû³ÉÔ±µÄÃû×Ö£¨¶Ó³¤·ÅÔÚµÚÒ»Î»£©
	DWORD	nTeamServerID;							// ¶ÓÎéÔÚ·þÎñÆ÷ÉÏµÄÎ¨Ò»±êÊ¶
	DWORD	m_nLeadExp;							// Íæ¼ÒµÄÍ³ÂÊÁ¦¾­Ñé
	BYTE	m_btLevel[MAX_TEAM_MEMBER + 1];			// Ã¿Ãû³ÉÔ±µÄµÈ¼¶£¨¶Ó³¤·ÅÔÚµÚÒ»Î»£©
	PLAYER_SEND_SELF_TEAM_INFO_DATA() {
		memset(m_szNpcName, 0, 32 * (MAX_TEAM_MEMBER + 1));
		nTeamServerID = -1;
		ProtocolType = NULL;
		m_btState = NULL;
		m_nLeadExp = 0;
		memset(m_dwNpcID, 0, sizeof(m_dwNpcID));
		memset(m_btLevel, 0, sizeof(m_btLevel));
	};
} PLAYER_SEND_SELF_TEAM_INFO;						// ·þÎñÆ÷Ïò¿Í»§¶Ë·¢ËÍ¿Í»§¶Ë×ÔÉí¶ÓÎéµÄÐÅÏ¢Êý¾Ý

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btState;
	BYTE	m_btFlag;		// ´ò¿ª»ò¹Ø±Õ
} PLAYER_TEAM_CHANGE_STATE;		// ¶ÓÎé¶Ó³¤Ïò·þÎñÆ÷ÉêÇë¿ª·Å¡¢¹Ø±Õ¶ÓÎéÊÇ·ñÔÊÐí½ÓÊÕ³ÉÔ±×´Ì¬

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	DWORD	m_dwTarNpcID;		// Ä¿±ê¶ÓÎé¶Ó³¤npc id »òÕß ÉêÇëÈË npc id
} PLAYER_APPLY_ADD_TEAM;		// Íæ¼ÒÏò·þÎñÆ÷ÉêÇë¼ÓÈëÄ³¸ö¶ÓÎé»òÕß·þÎñÆ÷ÏòÄ³¸ö¶Ó³¤×ª·¢Ä³¸öÍæ¼ÒµÄ¼ÓÈëÉêÇë

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	DWORD	m_dwNpcID;			// ±»½ÓÊÜÈë¶ÓÎéµÄnpc id
} PLAYER_ACCEPT_TEAM_MEMBER;	// Íæ¼ÒÍ¨Öª·þÎñÆ÷½ÓÊÜÄ³¸öÍæ¼ÒÈë¶ÓÎé

typedef struct PLAYER_TEAM_ADD_MEMBER_DATA
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btLevel;			// ¼ÓÈëÕßµÈ¼¶
	DWORD	m_dwNpcID;			// ¼ÓÈëÕßnpc id
	char	m_szName[32];		// ¼ÓÈëÕßÐÕÃû
	PLAYER_TEAM_ADD_MEMBER_DATA() {
		memset(m_szName, 0, 32);
		ProtocolType = NULL;
		m_btLevel = NULL;
		m_dwNpcID = NULL;
	};
} PLAYER_TEAM_ADD_MEMBER;		// ·þÎñÆ÷Í¨Öª¶ÓÎéÖÐµÄ¸÷¸öÍæ¼ÒÓÐÐÂ³ÉÔ±¼ÓÈë

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BOOL	bMySelf;
} PLAYER_APPLY_LEAVE_TEAM;		// ¿Í»§¶ËÍæ¼ÒÉêÇëÀë¶Ó

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	DWORD	m_dwNpcID;			// Àë¶Ónpc id
} PLAYER_LEAVE_TEAM;			// ·þÎñÆ÷Í¨Öª¸÷¶ÓÔ±Ä³ÈËÀë¶Ó

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	DWORD	m_dwNpcID;			// Àë¶Ónpc id
} PLAYER_TEAM_KICK_MEMBER;		// ¶Ó³¤Ìß³ýÄ³¸ö¶ÓÔ±

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	DWORD	m_dwNpcID;			// Ä¿±ênpc id
	BOOL	m_bMySelf;
} PLAYER_APPLY_TEAM_CHANGE_CAPTAIN;// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇë°Ñ×Ô¼ºµÄ¶Ó³¤Éí·Ý½»¸ø±ðµÄ¶ÓÔ±

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	DWORD	m_dwCaptainID;		// ÐÂ¶Ó³¤npc id
	DWORD	m_dwMemberID;		// ÐÂ¶ÓÔ±npc id
	BOOL	m_bMySelf;
} PLAYER_TEAM_CHANGE_CAPTAIN;	// ·þÎñÆ÷Í¨Öª¸÷¶ÓÔ±¸ü»»¶Ó³¤

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
} PLAYER_APPLY_TEAM_DISMISS;	// Ïò·þÎñÆ÷ÉêÇë½âÉ¢¶ÓÎé

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btPKFlag;			// pk ¿ª¹Ø
} PLAYER_SET_PK;				// Ïò·þÎñÆ÷ÉêÇë´ò¿ª¡¢¹Ø±ÕPK

typedef struct
{
	BYTE	ProtocolType;			// Ð­ÒéÃû³Æ
	BYTE	m_btCamp;				// ÐÂÕóÓª
	BYTE	m_btCurFaction;			// µ±Ç°ÃÅÅÉ
	BYTE	m_btFirstFaction;		// Ê×´Î¼ÓÈëÃÅÅÉ
	int32_t		m_nAddTimes;			// ¼ÓÈëÃÅÅÉ´ÎÊý
} PLAYER_FACTION_DATA;				// ·þÎñÆ÷·¢¸ø¿Í»§¶ËÃÅÅÉÐÅÏ¢

typedef struct
{
	BYTE	ProtocolType;			// Ð­ÒéÃû³Æ
} PLAYER_LEAVE_FACTION;				// ·þÎñÆ÷Í¨ÖªÍæ¼ÒÀë¿ªÃÅÅÉ

typedef struct
{
	BYTE	ProtocolType;			// Ð­ÒéÃû³Æ
	int32_t		m_nMissionGroup;				// ÐÂÕóÓª
} PLAYER_MISSION_DATA;				// ·þÎñÆ÷·¢¸ø¿Í»§¶ËÃÅÅÉÐÅÏ¢

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btState;
	int32_t		m_nID;
	int32_t		m_nDataID;
	int32_t		m_nXpos;
	int32_t		m_nYpos;
	int32_t		m_nMoneyNum;
	int32_t		m_nItemID;
	BYTE	m_btDir;
	BYTE	m_btItemWidth;
	WORD	m_wCurFrame;
	BYTE	m_btItemHeight;
	BYTE	m_btColorID;
	int32_t		m_nGenre;
	int32_t		m_nDetailType;
	BYTE	m_btFlag;
	char	m_szName[OBJ_NAME_LENGTH];
} OBJ_ADD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btState;
	int32_t		m_nID;
} OBJ_SYNC_STATE;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btDir;
	int32_t		m_nID;
} OBJ_SYNC_DIR;

typedef struct
{
	BYTE	ProtocolType;
	int32_t		m_nID;
	BYTE	m_btSoundFlag;
} OBJ_SYNC_REMOVE;

typedef struct
{
	BYTE	ProtocolType;
	int32_t		m_nID;
} OBJ_CLIENT_SYNC_ADD;

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	int32_t		m_nLeadLevel;		// Í³ÂÊÁ¦¾­ÑéÖµ
	int32_t		m_nLeadExp;		// Í³ÂÊÁ¦¾­ÑéÖµ
} PLAYER_LEAD_EXP_SYNC;			// Í¬²½Í³ÂÊÁ¦¾­ÑéÖµ

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BOOL	m_bSetLevel;
	BYTE	m_btLevel;			// µ±Ç°µÈ¼¶
	int32_t		m_nExp;				// µ±Ç°¾­Ñé
	int32_t		m_nAttributePoint;	// Ê£ÓàÊôÐÔµã
	int32_t		m_nSkillPoint;		// Ê£Óà¼¼ÄÜµã
	int32_t		m_nBaseLifeMax;		// µ±Ç°×î´óÉúÃüÖµ
	int32_t		m_nBaseStaminaMax;	// µ±Ç°×î´óÌåÁ¦Öµ
	int32_t		m_nBaseManaMax;		// µ±Ç°×î´óÄÚÁ¦Öµ
} PLAYER_LEVEL_UP_SYNC;			// Íæ¼ÒÉý¼¶

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btLevel;			// µ±Ç°µÈ¼¶
	DWORD	m_dwTeammateID;		// ¶ÓÓÑ npc id
} PLAYER_TEAMMATE_LEVEL_SYNC;	// Íæ¼ÒÉý¼¶µÄÊ±ºòÍ¨Öª¶ÓÓÑ

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btAttribute;		// ÊôÐÔ(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	int32_t		m_nAddNo;			// ¼ÓµÄµãÊý
} PLAYER_ADD_BASE_ATTRIBUTE_COMMAND;	// Íæ¼ÒÌí¼Ó»ù±¾ÊôÐÔµã

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	int32_t		m_nSkillID;			// ¼¼ÄÜid
	int32_t		m_nAddPoint;		// Òª¼ÓµÄµãÊý
} PLAYER_ADD_SKILL_POINT_COMMAND;// Íæ¼ÒÉêÇëÔö¼ÓÄ³¸ö¼¼ÄÜµÄµãÊý

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btAttribute;		// ÊôÐÔ(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	int32_t		m_nBasePoint;		// »ù±¾µãÊý
	int32_t		m_nCurPoint;		// µ±Ç°µãÊý
	int32_t		m_nLeavePoint;		// Ê£ÓàÎ´·ÖÅäÊôÐÔµã
} PLAYER_ATTRIBUTE_SYNC;		// Íæ¼ÒÍ¬²½ÊôÐÔµã

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	int32_t		m_nSkillID;			// ¼¼ÄÜid
	int32_t		m_nSkillLevel;		// ¼¼ÄÜµÈ¼¶
	int32_t		m_nAddLevel;		// ×°±¸Ìí¼ÓµÄ¼¼ÄÜµã
	int32_t		m_nSkillExp;		// ¼¼ÄÜµÈ¼¶
	BOOL	m_bTempSkill;
	int32_t		m_nLeavePoint;		// Ê£ÓàÎ´·ÖÅä¼¼ÄÜµã
} PLAYER_SKILL_LEVEL_SYNC;		// Íæ¼ÒÍ¬²½¼¼ÄÜµã

//typedef struct
//{
//	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
//	int		m_nItemID;			// ÎïÆ·id
//	int		m_nSourcePos;		// À´Ô´Î»ÖÃ
//	int		m_nTargetPos;		// Ä¿µÄÎ»ÖÃ
//} PLAYER_EQUIP_ITEM_COMMAND;	// Íæ¼ÒÊó±êÓÒ¼üµã»÷Ê¹ÓÃÎïÆ·(×°±¸)

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btPlace;			// Ò©Æ·Î»ÖÃ
	BYTE	m_btX;				// Ò©Æ·Î»ÖÃ
	BYTE	m_btY;				// Ò©Æ·Î»ÖÃ
	int32_t		m_nItemID;			// ÎïÆ·id
} PLAYER_EAT_ITEM_COMMAND;		// Íæ¼ÒÊó±êÓÒ¼üµã»÷Ê¹ÓÃÎïÆ·(³ÔÒ©)

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	int32_t		m_nObjID;			// Êó±êµã»÷µÄobjµÄid
	BYTE	m_btPosType;		// Î»ÖÃÀàÐÍ
	BYTE	m_btPosX;			// ×ø±ê x
	BYTE	m_btPosY;			// ×ø±ê y
} PLAYER_PICKUP_ITEM_COMMAND;		// Íæ¼Ò»ñµÃÎïÆ·£¨Êó±êµã»÷µØÍ¼ÉÏµÄobj£©

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	BOOL			m_bIsNew;
	BOOL			m_bTemp;
	DWORD			m_dwOwner;
	int32_t				m_TradePrice;
	int32_t				m_ID;				// ÎïÆ·µÄID
	BYTE			m_Nature;
	BYTE			m_Genre;			// ÎïÆ·µÄÀàÐÍ
	int32_t				m_Detail;			// ÎïÆ·µÄÀà±ð
	int32_t				m_Particur;			// ÎïÆ·µÄÏêÏ¸Àà±ð
	BYTE			m_Series;			// ÎïÆ·µÄÎåÐÐ
	BYTE			m_Level;			// ÎïÆ·µÄµÈ¼¶
	BYTE			m_btPlace;			// ×ø±ê
	BYTE			m_btX;				// ×ø±ê
	BYTE			m_btY;				// ×ø±ê
	PlayerItem		m_BackLocal;
	int32_t				m_ItemX;
	int32_t				m_ItemY;
	int32_t				m_Luck;				// MF
	int32_t				m_MagicLevel[MAX_ITEM_MAGICLEVEL];	// Éú³É²ÎÊý
	WORD			m_Version;			// ×°±¸°æ±¾
	int32_t				m_Durability;		// ÄÍ¾Ã¶È
	UINT			m_RandomSeed;		// Ëæ»úÖÖ×Ó
	BYTE			m_bStack;
	int32_t				m_StackNum;
	int32_t				m_ExpireTime;
	KLockItem		m_LockItem;
	BOOL			m_bLockSell;
	BOOL			m_bLockTrade;
	BOOL			m_bLockDrop;
	int32_t				m_Param;
	int32_t				m_Mantle;
	int32_t				m_Width;
	int32_t				m_Height;
	int32_t				m_Fortune;
	int32_t				m_Kind;
} ITEM_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD			m_dwID;
	int32_t				m_MagicLevel[MAX_ITEM_MAGICLEVEL];
	KMagicAttrib	m_MagicAttrib[MAX_ITEM_MAGICATTRIB];
} ITEM_SYNC_MAGIC;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_ID;				// ÎïÆ·µÄID
} ITEM_REMOVE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_ID;				// ÎïÆ·µÄID
	BYTE			m_Number;
} PLAYER_SELL_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_Shop;
	BYTE			m_BuyIdx;			// ÂòµÚ¼¸¸ö¶«Î÷
	BYTE			m_Number;			// new add
} PLAYER_BUY_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_nMoney1;			// ×°±¸À¸
	int32_t				m_nMoney2;			// ÖüÎïÏä
	int32_t				m_nMoney3;			// ½»Ò×À¸
} PLAYER_MONEY_SYNC;					// ·þÎñÆ÷Í¨Öª¿Í»§¶ËÇ®µÄÊýÁ¿

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	BYTE			m_btDownPos;
	BYTE			m_btDownX;
	BYTE			m_btDownY;
	BYTE			m_btUpPos;
	BYTE			m_btUpX;
	BYTE			m_btUpY;
} PLAYER_MOVE_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	BYTE			m_btDownPos;
	BYTE			m_btDownX;
	BYTE			m_btDownY;
	BYTE			m_btUpPos;
	BYTE			m_btUpX;
	BYTE			m_btUpY;
} PLAYER_MOVE_ITEM_SYNC;


typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	BYTE			m_btSrcPos;
	BYTE			m_btSrcX;
	BYTE			m_btSrcY;
	BYTE			m_btDestPos;
	BYTE			m_btDestX;
	BYTE			m_btDestY;
} ITEM_AUTO_MOVE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
} PLAYER_THROW_AWAY_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t 			m_shLife;
	int32_t 			m_shStamina;
	int32_t 			m_shMana;
	int16_t			m_shAngry;
	BYTE			m_btTeamData;
} CURPLAYER_NORMAL_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	// npc²¿·Ö
	DWORD			m_dwID;				// NpcµÄID
	BYTE			m_btLevel;			// NpcµÄµÈ¼¶
	BYTE			m_btSex;			// ÐÔ±ð
	BYTE			m_btKind;			// NpcµÄÀàÐÍ
	BYTE			m_btSeries;			// NpcµÄÎåÐÐÏµ
	int32_t 			m_wLifeMax;			// NpcµÄ×î´óÉúÃü
	int32_t 			m_wStaminaMax;		// NpcµÄ×î´óÌåÁ¦
	int32_t 			m_wManaMax;			// NpcµÄ×î´óÄÚÁ¦
	int32_t				m_HeadImage;
	// player ²¿·Ö
	WORD			m_wAttributePoint;	// Î´·ÖÅäÊôÐÔµã
	WORD			m_wSkillPoint;		// Î´·ÖÅä¼¼ÄÜµã
	WORD			m_wStrength;		// Íæ¼ÒµÄ»ù±¾Á¦Á¿£¨¾ö¶¨»ù±¾ÉËº¦£©
	WORD			m_wDexterity;		// Íæ¼ÒµÄ»ù±¾Ãô½Ý£¨¾ö¶¨ÃüÖÐ¡¢ÌåÁ¦£©
	WORD			m_wVitality;		// Íæ¼ÒµÄ»ù±¾»îÁ¦£¨¾ö¶¨ÉúÃü¡¢ÌåÁ¦£©
	WORD			m_wEngergy;			// Íæ¼ÒµÄ»ù±¾¾«Á¦£¨¾ö¶¨ÄÚÁ¦£©
	WORD			m_wLucky;			// Íæ¼ÒµÄ»ù±¾ÐÒÔËÖµ
	int32_t				m_nExp;				// µ±Ç°¾­ÑéÖµ(µ±Ç°µÈ¼¶ÔÚnpcÉíÉÏ)
	BYTE			m_byTranslife;
	int32_t				m_nLeadExp;		// Í³ÂÊÁ¦¾­ÑéÖµ

	// ÃÅÅÉ
	BYTE			m_btCurFaction;		// µ±Ç°ÃÅÅÉ
	BYTE			m_btFirstFaction;	// µÚÒ»´Î¼ÓÈëµÄÊÇÄÄ¸öÃÅÅÉ
	int32_t				m_nFactionAddTimes;	// ¼ÓÈë¸÷ÖÖÃÅÅÉµÄ×Ü´ÎÊý

	int32_t				m_nMissionGroup;
	int32_t				m_nRoomId;
	// ÅÅÃû
	WORD			m_wWorldStat;		// ÊÀ½çÅÅÃû
	WORD			m_wSectStat;		// ÃÅÅÉÅÅÃû
	int32_t				m_nKillPeopleNumber;
	// Ç®
	int32_t				m_nMoney1;
	int32_t				m_nMoney2;
	DWORD			m_dwEquipExpandTime;
	BYTE			m_btRepositoryNum;
	DWORD			m_dwLeaveTongTime;
	BYTE			m_btImagePlayer;
} CURPLAYER_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wProtocolLong;
	BYTE	m_nOperateType;				//²Ù×÷ÀàÐÍ
	BYTE	m_bUIId, m_bOptionNum, m_bParam1, m_bParam2;// m_bParam1,Ö÷ÐÅÏ¢ÊÇÊý×Ö±êÊ¶»¹ÊÇ×Ö·û´®±êÊ¶, m_bParam2,ÊÇ·ñÊÇÓë·þÎñÆ÷½»»¥µÄÑ¡Ôñ½çÃæ
	BOOL	m_bParam;
	int32_t		m_nParam, m_nParam1;
	int32_t		m_nBufferLen;
	char	m_szKey[MAX_PATH];
	char	m_pContent[MAX_SCIRPTACTION_BUFFERNUM];				//´ø¿ØÖÆ·û
} PLAYER_SCRIPTACTION_SYNC;

typedef struct
{
	WORD	SkillId;
	BYTE	SkillLevel;
	int32_t		SkillExp;
} SKILL_SEND_ALL_SYNC_DATA;

typedef struct
{
	BYTE						ProtocolType;
	WORD						m_wProtocolLong;
	SKILL_SEND_ALL_SYNC_DATA	m_sAllSkill[MAX_NPCSKILL];
} SKILL_SEND_ALL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	WeatherID;
} SYNC_WEATHER;

typedef struct defWORLD_SYNC
{
	BYTE	ProtocolType;
	int32_t		SubWorld;
	int32_t		Region;
	BYTE	Weather;
	DWORD	Frame;
} WORLD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	int32_t		nSelectIndex;
	int32_t		nSelectUi;
}PLAYER_SELECTUI_COMMAND;

typedef struct
{
	BYTE			ProtocolType;	// c2s_viewequip
	DWORD			m_dwNpcID;
} VIEW_EQUIP_COMMAND;

typedef struct
{
	BYTE			ProtocolType;	// c2s_viewequip
	DWORD			m_dwNpcID;
} PLAYER_TRADE_VIEW_COMMAND;

//´Ë½á¹¹ÒÑ¾­±»tagDBSelPlayer½á¹¹Ìæ»»
/*typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int				m_nSelect;			//
} DB_PLAYERSELECT_COMMAND;*/

/*
 * { Add by liupeng 2003.05.10
 *
 * #pragma pack( push, 1 )
*/

/*
 * Nonstandard extension used : zero-sized array in struct/union
 */
#pragma warning(disable: 4200)

#define KSG_PASSWORD_MAX_SIZE   64

typedef struct tagKSG_PASSWORD
{
	char szPassword[KSG_PASSWORD_MAX_SIZE];    // ÏÖÔÚ²ÉÓÃMD5µÄ×Ö·û´®£¬ÓÉÓÚÊÇ32¸ö×Ö·û£¬¼ÓÉÏÄ©Î²'\0'£¬ÐèÒªÖÁÉÙ33¸ö¿Õ¼ä£¬Òò´ËÊ¹ÓÃ64
} KSG_PASSWORD;

#define _NAME_LEN	32

struct tagProtoHeader
{
	BYTE	cProtocol;
};

struct tagResult : public tagProtoHeader
{
	BYTE	cResult;
};

struct tagDBSelPlayer : public tagProtoHeader
{
	char	szRoleName[_NAME_LEN];
};

struct tagDBDelPlayer : public tagProtoHeader
{
	char	        szAccountName[_NAME_LEN];
	KSG_PASSWORD	Password;
	char	        szRoleName[_NAME_LEN];
};

//É¾³ýÓëÐÂ½¨½ÇÉ«µÄ·µ»ØÏûÏ¢´øµÄÊý¾Ý
struct tagNewDelRoleResponse : public tagDBSelPlayer
{
	bool	bSucceeded;		//ÊÇ·ñ³É¹¦
};

// 2003.05.11
struct tagDBSyncPlayerInfo : public tagProtoHeader
{
	size_t	dataLength;
	char	szData[0];
};

/*
 * 2003.06.27
 * s2c_gateway_broadcast
*/

#define	AP_WARNING_ALL_PLAYER_QUIT			1
#define	AP_NOTIFY_GAMESERVER_SAFECLOSE		2
#define	AP_NOTIFY_ALL_PLAYER				3

#define	AP_PUNISH_LOCK						1
#define	AP_PUNISH_CHAT						2
#define	AP_LOCK_ACCOUNTNAME					1
#define	AP_UNLOCK_ACCOUNTNAME				0
#define	AP_LOCK_CHAT						2
#define	AP_UNLOCK_CHAT						3
#define	MAX_GATEWAYBROADCAST_LEN	260
struct tagGatewayBroadCast : public tagProtoHeader
{
	UINT	uCmdType;
	char	szData[MAX_GATEWAYBROADCAST_LEN];
};

/*
 * 2003.05.22
 * s2c_syncgamesvr_roleinfo_cipher
*/
struct tagGuidableInfo : public tagProtoHeader
{
	GUID guid;
	WORD nExtPoint;			//¿ÉÓÃµÄ¸½ËÍµã
	WORD nChangePoint;		//±ä»¯µÄ¸½ËÍµã
	size_t	datalength;
	char	szData[0];

};

/*
 * c2s_permitplayerlogin
 */
struct tagPermitPlayerLogin : public tagProtoHeader
{
	GUID guid;

	BYTE szRoleName[_NAME_LEN];
	BYTE szAccountName[_NAME_LEN];
	char szIp[32];
	/*
	 * Succeeded : true
	 * Failed	 : false
	 */
	bool bPermit;
};

struct tagPermitPlayerExchange
{
	BYTE cProtocol;
	GUID guid;
	DWORD dwIp;
	WORD wPort;
	bool bPermit;
};
/*
 * c2s_notifyplayerlogin
 */
struct tagNotifyPlayerLogin : public tagPermitPlayerLogin
{
	UINT			nIPAddr;
	uint16_t	nPort;
};

/*
 * s2c_querymapinfo
 */
struct tagQueryMapInfo : public tagProtoHeader
{
};

/*
 * s2c_querygameserverinfo
 */
struct tagQueryGameSvrInfo : public tagProtoHeader
{
};

/*
 * s2c_notifysvrip
 */
struct tagNotifySvrIp : public tagProtoHeader
{
	WORD	pckgID;

	BYTE	cIPType;
	DWORD	dwMapID;

	DWORD	dwSvrIP;
};

/*
 * s2c_notifyplayerexchange
 */
struct tagNotifyPlayerExchange : public tagProtoHeader
{
	GUID			guid;
	UINT			nIPAddr;
	uint16_t	nPort;
};

/*
 * c2s_requestsvrip
 */

 /*
  * BYTE	cIPType
  */
#define INTRANER_IP	0
#define INTERNET_IP 1

struct tagRequestSvrIp : public tagProtoHeader
{
	WORD	pckgID;

	BYTE	cIPType;
	DWORD	dwMapID;
};

/*
 * c2c_notifyexchange
 */
struct tagSearchWay : public tagProtoHeader
{
	int32_t		lnID;
	int32_t		nIndex;
	DWORD	dwPlayerID;
};

/*
 * c2s_updatemapinfo
 */
struct tagUpdateMapID : public tagProtoHeader
{
	/*
	 * For example : Are your clear older information when it
	 *		update local informatin
	 */
	BYTE cReserve;

	BYTE cMapCount;

	int16_t szMapID[0]; // Con trỏ thay vì mảng cố định
};

/*
 * c2s_updategameserverinfo
 */
struct tagGameSvrInfo : public tagProtoHeader
{
	UINT			nIPAddr_Intraner;
	UINT			nIPAddr_Internet;

	uint16_t	nPort;
	WORD			wCapability;
};

/*
 * s2c_identitymapping
 */
struct tagIdentityMapping : public tagGameSvrInfo
{
	GUID guid;
};

/*
 * c2s_logiclogin
 * s2c_gmgateway2relaysvr
 * s2c_gmnotify
 */
struct tagLogicLogin : public tagProtoHeader
{
	GUID guid;
};

/*
 * s2c_logiclogout
 */
struct tagLogicLogout : public tagProtoHeader
{
	BYTE szRoleName[_NAME_LEN];
};

/*
 * c2s_registeraccount
 */
struct tagRegisterAccount : public tagProtoHeader
{
	BYTE szAccountName[_NAME_LEN];
};

/*
 * c2s_entergame
 */
struct tagEnterGame : public tagProtoHeader
{
	/*
	 * Succeeded : content is account name
	 * Failed	 : content is null
	 */

	BYTE szAccountName[_NAME_LEN];
};

struct tagEnterGame2 : public EXTEND_HEADER
{
	char szAccountName[_NAME_LEN];
	char szCharacterName[_NAME_LEN];
	DWORD	dwNameID;
	ULONG	lnID;
};

/*
 * c2s_leavegame
 */

 /*
  * BYTE cCmdType
  */
#define NORMAL_LEAVEGAME	0x0		// lock account
#define HOLDACC_LEAVEGAME	0x1A	// clear resource but don't to unlock account 

struct tagLeaveGame : public tagProtoHeader
{
	BYTE cCmdType;
	WORD nExtPoint;        //½«Òª¿Û³ýµÄ¸½ËÍµã
	BYTE nExtDay;
	/*
	 * Succeeded : content is account name
	 * Failed	 : content is null
	 */

	char szAccountName[_NAME_LEN];
};

struct tagLeaveGame2 : public EXTEND_HEADER
{
	BYTE cCmdType;
	char szAccountName[_NAME_LEN];
};
/*
*  c2s_registerfamily
*/
struct tagRegisterFamily : public tagProtoHeader
{
	BYTE bRegister;		//1 is Register, 0 is unRegister
	BYTE nFamily;
	BYTE RelayMethod;
};

/*
 * c2s_gmsvr2gateway_saverole
 */
struct tagGS2GWSaveRole : public tagProtoHeader
{
	size_t	datalength;
	BYTE	szData[0];
};

/*
 * #pragma pack( pop )
 *
 * } End of the struct define
 */

typedef struct
{
	char	szName[32];
	BYTE	Sex;
	BYTE	Series;
	int32_t		Faction;
	int32_t		Level;
} RoleBaseInfo/* client */, S3DBI_RoleBaseInfo /* server */;

typedef struct
{
	BYTE				ProtocolType;
	RoleBaseInfo		m_RoleList[MAX_PLAYER_IN_ACCOUNT];
} ROLE_LIST_SYNC;

//ÒÆ×ÔRoleDBManager/kroledbheader.h
//ÓÃÀ´Ìæ»»ÉÏÃæµÄROLE_LIST_SYNC,ROLE_LIST_SYNC½á¹¹²»ÔÙÐèÒªÁË
struct TProcessData
{
	unsigned char	nProtoId;
	size_t			nDataLen;//TRoleNetMsgÊ±±íÊ¾¸ÃBlockµÄÊµ¼ÊÊý¾Ý³¤¶È,TProcessDataÊ±±íÊ¾StreamµÄÊµ¼ÊÊý¾Ý³¤¶È
	ULONG	ulIdentity;
	bool			bLeave;
	bool			bOffline;
	char			szName[32];
	char			pDataBuffer[1];//Êµ¼ÊµÄÊý¾Ý
};

struct TCheckNameData
{
	unsigned char	nProtoId;
	UINT			nDataLen;//TRoleNetMsgÊ±±íÊ¾¸ÃBlockµÄÊµ¼ÊÊý¾Ý³¤¶È,TProcessDataÊ±±íÊ¾StreamµÄÊµ¼ÊÊý¾Ý³¤¶È
	unsigned int	ulIdentity;
	char            nAccName[32];   //ÕËºÅÃû
	char            nRoleName[32];
};

struct TChangeNameData
{
	unsigned char	nProtoId;
	UINT			nDataLen;//TRoleNetMsgÊ±±íÊ¾¸ÃBlockµÄÊµ¼ÊÊý¾Ý³¤¶È,TProcessDataÊ±±íÊ¾StreamµÄÊµ¼ÊÊý¾Ý³¤¶È
	unsigned int	ulIdentity;
	char            nAccName[32];   //ÕËºÅÃû
	char            nSrcRoleName[32];
	char            nTarRoleName[32];
};

struct tagRoleEnterGame
{
	BYTE			ProtocolType;
	bool			bLock;
	bool			bOffline;
	char			Name[_NAME_LEN];
};

//ÐÂ½¨½ÇÉ«µÄÐÅÏ¢½á¹¹
//×¢ÊÍ£ºÐÂ½¨¾öÏûÏ¢c2s_newplayer£¬´«ËÍµÄ²ÎÊýÎªTProcessData½á¹¹ÃèÊöµÄÊý¾Ý£¬ÆäÖÐTProcessData::pDataBufferÒªÀ©Õ¹ÎªNEW_PLAYER_COMMAND
struct NEW_PLAYER_COMMAND
{
	BYTE			m_btRoleNo;			// ½ÇÉ«±àºÅ
	BYTE			m_btSeries;			// ÎåÐÐÏµ
	uint16_t	m_NativePlaceId;	//³öÉúµØID
	char			m_szName[32];		// ÐÕÃû
};

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_LogoutType;
} LOGOUT_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			szAccName[32];
} LOGIN_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	WORD			m_wLength;
	char			m_szSentence[MAX_SENTENCE_LENGTH];
} TRADE_APPLY_OPEN_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
} TRADE_APPLY_CLOSE_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	BYTE			m_btState;			// if == 0 close if == 1 open if == 2 trading
	DWORD			m_dwNpcID;			// Èç¹ûÊÇ¿ªÊ¼½»Ò×£¬¶Ô·½µÄ npc id
	BOOL			m_bFolkGame;
} TRADE_CHANGE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	WORD			m_wLength;
	DWORD			m_dwID;
	BYTE			m_btState;
	char			m_szSentence[MAX_SENTENCE_LENGTH];
} NPC_SET_MENU_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;//agreement type
	int				m_btStateInfo[MAX_NPC_RECORDER_STATE];  //BYTE
	UINT			m_ID;//GID of Npc
}	NPC_SYNC_STATEINFO;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	DWORD			m_dwID;
	BOOL			m_bFolkGame;
} TRADE_APPLY_START_COMMAND;

// ·þÎñÆ÷×ª·¢½»Ò×ÉêÇë
typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_nDestIdx;			// ÉêÇëÕßÔÚ·þÎñÆ÷¶ËµÄplayer idx
	DWORD			m_dwNpcId;			// ÉêÇëÕßµÄ npc id
	BOOL			m_bFolkGame;
} TRADE_APPLY_START_SYNC;

// ½ÓÊÜ»ò¾Ü¾ø±ðÈËµÄ½»Ò×ÉêÇë
typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	BYTE			m_bDecision;		// Í¬Òâ 1 ²»Í¬Òâ 0
	int32_t				m_nDestIdx;			// ½»Ò×¶Ô·½ÔÚ·þÎñÆ÷¶ËµÄplayer idx
	BOOL			m_bFolkGame;
} TRADE_REPLY_START_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_nMoney;
} TRADE_MOVE_MONEY_COMMAND;		// c2s_trademovemoney

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_nMoney;
} TRADE_MONEY_SYNC;				// s2c_trademoneysync

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	BYTE			m_btDecision;		// È·¶¨½»Ò× 1  ÍË³ö½»Ò× 0  È¡ÏûÈ·¶¨ 4  Ëø¶¨½»Ò× 2  È¡ÏûËø¶¨ 3
	BYTE			m_btFolkGame;
} TRADE_DECISION_COMMAND;				// ½»Ò×Ö´ÐÐ»òÈ¡Ïû c2s_tradedecision

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	BYTE			m_btDecision;		// ½»Ò×ok 1  ½»Ò×È¡Ïû 0  Ëø¶¨ 2  È¡ÏûËø¶¨ 3
} TRADE_DECISION_SYNC;					// s2c_tradedecision

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_byDir;			// È¡Ç®µÄ·½Ïò£¨0´æ£¬1È¡£©
	DWORD			m_dwMoney;			// Ç®Êý
} STORE_MONEY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	DWORD			m_dwNpcID;
} TEAM_INVITE_ADD_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	WORD			m_wLength;			// ³¤¶È
	int32_t				m_nIdx;
	char			m_szName[32];
} TEAM_INVITE_ADD_SYNC;

typedef struct
{
	BYTE			ProtocolType;		//
	int32_t				m_nAuraSkill;
} SKILL_CHANGEAURASKILL_COMMAND;		//¸ü»»¹â»·¼¼ÄÜ

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btResult;
	int32_t				m_nIndex;
} TEAM_REPLY_INVITE_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BOOL			m_bSelfAskFolkGame;
	BOOL			m_bDestReplyFolkGame;
	BYTE			m_btSelfLock;
	BYTE			m_btDestLock;
	BYTE			m_btSelfOk;
	BYTE			m_btDestOk;
} TRADE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	DWORD			m_dwSkillID;		// ¼¼ÄÜ
	int32_t				m_nLevel;
	int32_t				m_nTime;			// Ê±¼ä
	BOOL			m_bOverLook;
	KMagicAttrib	m_MagicAttrib[MAX_SKILL_STATE];
} STATE_EFFECT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BOOL	bNegative;
} IGNORE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwTime;
} PING_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwReplyServerTime;
	DWORD			m_dwClientTime;
} PING_CLIENTREPLY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btSitFlag;
} NPC_SIT_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
} NPC_HORSE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	m_dwID;
	BOOL	m_bRideHorse;
}	NPC_HORSE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int32_t				m_dwRegionID;
	int32_t				m_nObjID;
} OBJ_MOUSE_CLICK_SYNC;

typedef struct tagSHOW_MSG_SYNC
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	WORD			m_wMsgID;
	LPVOID			m_lpBuf;
	//char            m_Mmsgs[125];
	tagSHOW_MSG_SYNC() {
		m_lpBuf = NULL;
		ProtocolType = NULL;
		m_wLength = 0;
		m_wMsgID = 0;
	};
	~tagSHOW_MSG_SYNC() { Release(); }
	void	Release() {
		if (m_lpBuf)
		{
			m_lpBuf = malloc(sizeof(m_lpBuf));
			free(m_lpBuf);
		}
		m_lpBuf = NULL;
	}
} SHOW_MSG_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btFlag;
} PK_APPLY_NORMAL_FLAG_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btFlag;
} PK_NORMAL_FLAG_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwNpcID;
	BOOL			m_bRefuse;
	BOOL			m_bSpar;
} PK_APPLY_ENMITY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	BYTE			m_btState;
	DWORD			m_dwNpcID;
	BOOL			m_bAim;
	BOOL			m_bSpar;
	char			m_szName[32];
} PK_ENMITY_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	BYTE			m_btState;
	DWORD			m_dwNpcID;
	char			m_szName[32];
} PK_EXERCISE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int32_t				m_nPKValue;
} PK_VALUE_SYNC;

typedef struct
{
	int32_t		m_nID;
	BYTE	m_btNature;
	BYTE	m_btGenre;			// ÎïÆ·µÄÀàÐÍ
	int32_t		m_btDetail;			// ÎïÆ·µÄÀà±ð
	int32_t		m_btParticur;		// ÎïÆ·µÄÏêÏ¸Àà±ð
	BYTE	m_btSeries;			// ÎïÆ·µÄÎåÐÐ
	BYTE	m_btLevel;			// ÎïÆ·µÄµÈ¼¶
	BYTE	m_btLuck;			// MF
	int32_t		m_btMagicLevel[MAX_ITEM_MAGICLEVEL];	// Éú³É²ÎÊý
	WORD	m_wVersion;			// ×°±¸°æ±¾
	DWORD	m_dwRandomSeed;		// Ëæ»úÖÖ×Ó
} SViewItemInfo;

typedef struct
{
	BYTE			ProtocolType;
	DWORD			m_dwNpcID;
	BYTE			m_Avatar;
	SViewItemInfo	m_sInfo[itempart_num];
} VIEW_EQUIP_SYNC;				// s2c_viewequip

typedef struct//¸Ã½á¹¹ÊÇËùÍ³¼ÆµÄÍæ¼ÒµÄ»ù±¾Êý¾Ý
{
	char	Name[20];
	int32_t		nValue;
	BYTE	bySort;
}TRoleList;

// ÓÎÏ·Í³¼Æ½á¹¹
typedef struct
{
	TRoleList MoneyStat[10];			//½ðÇ®×î¶àÅÅÃûÁÐ±í£¨Ê®¸öÍæ¼Ò£¬×î¶à¿É´ïµ½100¸ö£©
	TRoleList LevelStat[10];			//¼¶±ð×î¶àÅÅÃûÁÐ±í£¨Ê®¸öÍæ¼Ò£¬×î¶à¿É´ïµ½100¸ö£©
	TRoleList KillerStat[10];			//É±ÈË×î¶àÅÅÃûÁÐ±í
	TRoleList ReputeStat[10];
	TRoleList FuYuanStat[10];
	TRoleList AccumStat1[10];
	TRoleList AccumStat2[10];
	TRoleList HonorStat[10];
	TRoleList TimeStat[10];
	TRoleList TongLvStat[10];
	TRoleList TongMnStat[10];
	TRoleList TongEffStat[10];
	//[ÃÅÅÉºÅ][Íæ¼ÒÊý]£¬ÆäÖÐ[0]ÊÇÃ»ÓÐ¼ÓÈëÃÅÅÉµÄÍæ¼Ò
	TRoleList MoneyStatBySect[MAX_FACTION + 1][10];	//¸÷ÃÅÅÉ½ðÇ®×î¶àÅÅÃûÁÐ±í
	TRoleList LevelStatBySect[MAX_FACTION + 1][10];	//¸÷ÃÅÅÉ¼¶±ð×î¶àÅÅÃûÁÐ±í

	//[ÃÅÅÉºÅ]£¬ÆäÖÐ[0]ÊÇÃ»ÓÐ¼ÓÈëÃÅÅÉµÄÍæ¼Ò
	int32_t SectPlayerNum[MAX_FACTION + 1];				//¸÷¸öÃÅÅÉµÄÍæ¼ÒÊý
	int32_t SectMoneyMost[MAX_FACTION + 1];				//²Æ¸»ÅÅÃûÇ°Ò»°ÙÍæ¼ÒÖÐ¸÷ÃÅÅÉËùÕ¼±ÈÀýÊý
	int32_t SectLevelMost[MAX_FACTION + 1];				//¼¶±ðÅÅÃûÇ°Ò»°ÙÍæ¼ÒÖÐ¸÷ÃÅÅÉËùÕ¼±ÈÀýÊý
}  TGAME_STAT_DATA;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	bSleep;
	DWORD	NpcID;
} NPC_SLEEP_SYNC;

//////////////
//ÅÅÃûÏà¹Ø
typedef struct
{
	BYTE		ProtocolType;
	DWORD		dwLadderID;
	TRoleList	StatData[10];
} LADDER_DATA;

typedef struct
{
	BYTE		ProtocolType;
	WORD		wSize;
	int			nCount;
	DWORD		dwLadderID[0];
} LADDER_LIST;

typedef struct
{
	BYTE		ProtocolType;
	DWORD		dwLadderID;
} LADDER_QUERY;


///////////////////
//chat Ïà¹Ø

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	packageID;
	char	someone[_NAME_LEN];
	BYTE	sentlen;
	char	item[MAX_SENTENCE_LENGTH];
} CHAT_SOMEONECHAT_CMD, CHAT_SOMEONECHAT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	packageID;
	BYTE	filter;
	DWORD	channelid;
	BYTE	cost;	//0: ÎÞÏÞÖÆ£¬1: 10Ôª/¾ä£¬2: <10Lv ? ²»ÄÜËµ : MaxMana/2/¾ä, 3: MaxMana/10/¾ä,4: <20Lv ? ²»ÄÜËµ : MaxMana*4/5/¾ä
	BYTE	sentlen;
	char	item[MAX_SENTENCE_LENGTH];
	BYTE	someflag;
} CHAT_CHANNELCHAT_CMD;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	packageID;
	char	someone[_NAME_LEN];
	DWORD	channelid;
	BYTE	sentlen;
	char	item[MAX_SENTENCE_LENGTH];
	BYTE	someflag;
} CHAT_CHANNELCHAT_SYNC;

enum { codeSucc, codeFail, codeStore };
typedef struct
{
	BYTE	ProtocolType;
	DWORD	packageID;
	BYTE	code;
	char	item[MAX_SENTENCE_LENGTH];
	BYTE	someflag;
} CHAT_FEEDBACK;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	WORD	wChatLength;
} CHAT_EVERYONE;


typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	WORD	wChatLength;
	BYTE	byHasIdentify;
	WORD	wPlayerCount;
} CHAT_GROUPMAN;


typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	nameid;
	ULONG lnID;
	WORD	wChatLength;
} CHAT_SPECMAN;


enum { tgtcls_team, tgtcls_fac, tgtcls_tong, tgtcls_msgr, tgtcls_cr, tgtcls_scrn, tgtcls_bc };
typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	DWORD	nFromIP;
	DWORD	nFromRelayID;
	DWORD	channelid;
	BYTE	TargetCls;
	DWORD	TargetID;
	WORD	routeDateLength;
} CHAT_RELEGATE;


///////////////////////////////////////
// tong Ïà¹Ø

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} S2C_TONG_HEAD;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} STONG_PROTOCOL_HEAD;

typedef struct
{
	int32_t		m_nPlayerIdx;
	int32_t		m_nCamp;
	char	m_szTongName[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_APPLY_CREATE;

typedef struct
{
	int32_t		m_nPlayerIdx;
	DWORD	m_dwNpcID;
} STONG_SERVER_TO_CORE_APPLY_ADD;

typedef struct
{
	int32_t		m_nSelfIdx;
	int32_t		m_nTargetIdx;
	DWORD	m_dwNameID;
} STONG_SERVER_TO_CORE_CHECK_ADD_CONDITION;

typedef struct
{
	int32_t		m_nCamp;
	int32_t		m_nPlayerIdx;
	DWORD	m_dwPlayerNameID;
	char	m_szTongName[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_CREATE_SUCCESS;

typedef struct
{
	int32_t		m_nSelfIdx;
	int32_t		m_nTargetIdx;
	DWORD	m_dwNameID;
} STONG_SERVER_TO_CORE_REFUSE_ADD;

typedef struct
{
	int32_t		m_nSelfIdx;
	int32_t		m_nInfoID;
	DWORD		m_nParam1;
	int32_t		m_nParam2;
	int32_t		m_nParam3;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_GET_INFO;

typedef struct
{
	int32_t		m_nPlayerIdx;
	DWORD	m_dwPlayerNameID;
	BYTE	m_btCamp;
	DWORD	m_dwMoney;
	BYTE	m_btLevel;
	DWORD	m_dwTotalEff;
	BOOL	m_bRecruit;
	int32_t		m_nMemberNum;
	int32_t		m_nTongParam;
	int32_t 	m_nTongJiyuParam;
	char	m_szTongName[defTONG_NAME_LENGTH_32];
	char	m_szMasterName[defTONG_NAME_LENGTH_32];
	char	m_szAgname[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_ADD_SUCCESS;

typedef struct
{
	int32_t		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szName[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_BE_INSTATED;

typedef struct
{
	int32_t		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_BE_KICKED;

typedef struct
{
	int32_t		m_nPlayerIdx;
	char	m_szAgname[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_BE_CHANGED_AGNAME;

typedef struct
{
	DWORD	m_dwTongNameID;
	int32_t		m_nMoney;
	int32_t		m_nCamp;
} STONG_SERVER_TO_CORE_BE_CHANGED_CAMP;

typedef struct
{
	DWORD	m_dwTongNameID;
	BYTE 	m_btLevel;
} STONG_SERVER_TO_CORE_BE_CHANGED_LEVEL;

typedef struct
{
	DWORD	m_dwTongNameID;
	int32_t  	m_nMoney;
} STONG_SERVER_TO_CORE_BE_CHANGED_MONEY;

typedef struct
{
	DWORD	m_dwTongNameID;
	int32_t  	m_nEff;
} STONG_SERVER_TO_CORE_BE_CHANGED_TONG_EFF;

typedef struct
{
	DWORD	m_dwTongNameID;
	BOOL 	m_bRecruit;
} STONG_SERVER_TO_CORE_BE_CHANGED_RECRUIT;

typedef struct
{
	DWORD	m_dwTongNameID;
	int32_t 	m_nTongParam;
} STONG_SERVER_TO_CORE_BE_CHANGED_TONGPARAM;

typedef struct
{
	DWORD	m_dwTongNameID;
	int32_t  	m_nMoney;
	int32_t 	m_nTongJiyuParam;
	char	m_szTongJiyuNotify[defTONG_NOTIFY_LENGTH];
} STONG_SERVER_TO_CORE_BE_CHANGED_JIYU;

typedef struct
{
	int32_t		m_nPlayerIdx;
	int32_t 	m_nTotalEff;
	DWORD	m_dwTotalEff;
	DWORD	m_dwTongNameID;
} STONG_SERVER_TO_CORE_BE_CHANGED_EFF;

typedef struct
{
	int32_t		m_nPlayerIdx;
	BOOL	m_bSuccessFlag;
	char	m_szName[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_LEAVE;

typedef struct
{
	int32_t		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	DWORD	m_dwTongNameID;
	char	m_szName[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER;

typedef struct
{
	int32_t		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	DWORD	m_dwTongNameID;
	char	m_szName[defTONG_NAME_LENGTH_32];
	char	m_szAgname[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_CHECK_GET_AGNAME_POWER;

typedef struct
{
	int32_t		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	DWORD	m_dwTongNameID;
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szName[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_CHANGE_AS;

typedef struct
{
	DWORD	m_dwTongNameID;
	char	m_szName[defTONG_NAME_LENGTH_32];
} STONG_SERVER_TO_CORE_CHANGE_MASTER;

typedef struct
{
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	DWORD	m_nMoney;
	BYTE	nType;
	int32_t		m_nPlayerIdx;
	int32_t		m_nSelect;
	int32_t 	m_nNumMember;
} STONG_SERVER_TO_CORE_MONEY;

typedef struct
{
	DWORD	m_dwTongNameID;
	int32_t 	m_nValue;
	int32_t 	m_nValueSync;
	BYTE	nType;
	int32_t		m_nPlayerIdx;
	int32_t		m_nSelect;
} STONG_SERVER_TO_CORE_EFF;

typedef struct
{
	DWORD	m_dwParam;
	int32_t		m_nFlag;
	int32_t		m_nCamp;
	BYTE	m_btLevel;
	int32_t		m_nFigure;
	DWORD	m_dwMemberNum;
	BYTE	m_btManagerNum;
	BYTE	m_btDirectorNum;
	int32_t		m_nPos;
	char	m_szTongName[defTONG_NAME_LENGTH_32];
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szMaster[defTONG_NAME_LENGTH_32];
	char	m_szName[defTONG_NAME_LENGTH_32];
	int32_t		m_nJoinTm;
	DWORD	m_nMoney;
	DWORD	m_dwTotalEff;
	int32_t 	m_nSaveEff;
	BOOL 	m_bRecruit;
	int32_t		m_nTongParam;
	int32_t		m_nTongJiyuParam;
	char 	m_szTongJiyuNotify[defTONG_NOTIFY_LENGTH];
} STONG_SERVER_TO_CORE_LOGIN;

// Íæ¼ÒÉêÇë½¨Á¢°ï»á ÓÃÀ©Õ¹Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
	BYTE 	m_btSex;
	BYTE 	m_btLevel;
	char	m_szName[defTONG_NAME_LENGTH_32 + 1];
} TONG_APPLY_CREATE_COMMAND;

// Íæ¼ÒÉêÇë¼ÓÈë°ï»á ÓÃÀ©Õ¹Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwNpcID;
} TONG_APPLY_ADD_COMMAND;

// Íæ¼ÒÉêÇë¼ÓÈë°ï»á ÓÃÀ©Õ¹Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btCurFigure;
	BYTE	m_btCurPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_APPLY_INSTATE_COMMAND;

// °ï»á½¨Á¢Ê§°Ü À©Õ¹Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btFailId;
} TONG_CREATE_FAIL_SYNC;

// ×ª·¢¼ÓÈë°ï»áÉêÇë À©Õ¹Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int32_t		m_nPlayerIdx;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_APPLY_ADD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	char	m_cTongName[defTONG_NAME_LENGTH_32];
} JOIN_TONG_SYNC;

// Í¨ÖªÍæ¼Ò½¨Á¢°ï»á³É¹¦ ÓÃÆÕÍ¨Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btCamp;
	char	m_szName[defTONG_NAME_LENGTH_32 + 1];
} TONG_CREATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	DWORD			m_dwID;
} TONG_JOIN_REPLY;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	char			m_cTongName[defTONG_NAME_LENGTH_32];
} S2C_JOIN_TONG;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_Index;
	DWORD			m_dwID;
} S2C_TONG_JOIN_REPLY;

// Í¨ÖªÍæ¼Ò¼ÓÈë°ï»á ÓÃÀ©Õ¹Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
	char	m_szTongName[defTONG_NAME_LENGTH_32];
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szMaster[defTONG_NAME_LENGTH_32];
} TONG_ADD_SYNC;

// Íæ¼ÒÉêÇë½âÉ¢°ï»á ÓÃÀ©Õ¹Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} TONG_APPLY_DISMISS_COMMAND;

// Íæ¼Ò°ï»á±»½âÉ¢ ÓÃÆÕÍ¨Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
} TONG_DISMISS_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int32_t		m_nPlayerIdx;
	DWORD	m_dwNameID;
	BYTE	m_btFlag;			// ÊÇ·ñ½ÓÊÜ TRUE ½ÓÊÜ FALSE ²»½ÓÊÜ
} TONG_ACCEPT_MEMBER_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btInfoID;
	DWORD		m_nParam1;
	int32_t		m_nParam2;
	int32_t		m_nParam3;
	char	m_szBuf[64];
} TONG_APPLY_INFO_COMMAND;

typedef struct
{
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szName[defTONG_NAME_LENGTH_32];
	int32_t 	m_nMasterJoinTm;
	int32_t 	m_nDirectorJoinTm[defTONG_MAX_DIRECTOR];
	int32_t 	m_nManagerJoinTm[defTONG_MAX_MANAGER];
	BOOL	m_bOnline;
	int32_t		m_nMasterEff;
	int32_t		m_nDirectorEff[defTONG_MAX_DIRECTOR];
	int32_t 	m_nManagerEff[defTONG_MAX_MANAGER];
} TONG_ONE_LEADER_INFO;

typedef struct
{
	char	m_szName[defTONG_NAME_LENGTH_32];
	int32_t 	m_nMemberJoinTm;
	BOOL	m_bOnline;
	BYTE	m_btSex;
	int32_t 	m_nMemberEff;
} TONG_ONE_MEMBER_INFO;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwNpcID;
	DWORD	m_dwMoney;
	int32_t		m_nCredit;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	DWORD	m_dwTotalEff;
	int32_t 	m_nSaveEff;
	BOOL	m_bRecruit;
	int32_t 	m_nTongParam;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	int32_t		m_nTongJiyuParam;
	char 	m_szTongJiyuNotify[defTONG_NOTIFY_LENGTH];
	char	m_szTongName[defTONG_NAME_LENGTH_32];
	TONG_ONE_LEADER_INFO	m_sMember[1 + defTONG_MAX_DIRECTOR];
} TONG_HEAD_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwMoney;
	int32_t		m_nCredit;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	char	m_szTongName[defTONG_NAME_LENGTH_32];
	TONG_ONE_LEADER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
} TONG_MANAGER_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwMoney;
	int32_t		m_nCredit;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szTongName[defTONG_NAME_LENGTH_32];
	char	m_szMaleAgname[defTONG_NAME_LENGTH_32];
	char	m_szFemaleAgname[defTONG_NAME_LENGTH_32];
	TONG_ONE_MEMBER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
} TONG_MEMBER_INFO_SYNC;

// Íæ¼Ò×ÔÉíÔÚ°ï»áÖÐµÄÐÅÏ¢ ÓÃÀ©Õ¹Ð­Òé
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btJoinFlag;
	BYTE	m_btFigure;
	BYTE	m_btCamp;
	BYTE	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	DWORD	m_dwMemberNum;
	char	m_szTongName[defTONG_NAME_LENGTH_32];
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szMasterName[defTONG_NAME_LENGTH_32];
	int32_t		m_nTongJiyuParam;
	char 	m_szTongJiyuNotify[defTONG_NOTIFY_LENGTH];
	DWORD	m_dwMoney;
	DWORD	m_dwTotalEff;
	BOOL	m_bRecruit;
	int32_t		m_nTongParam;
} TONG_SELF_INFO_SYNC;


typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btOldFigure;
	BYTE	m_btOldPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_INSTATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_APPLY_KICK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_KICK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_APPLY_LEAVE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_APPLY_CHANGE_MASTER_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[defTONG_NAME_LENGTH_32];
	char	m_szAgname[defTONG_NAME_LENGTH_32];
} TONG_APPLY_CHANGE_AGNAME_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	int32_t		m_btSex;
	char	m_szAgname[defTONG_NAME_LENGTH_32];
} TONG_APPLY_CHANGE_SEX_AGNAME_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btCamp;
	int32_t 	m_nMoney;
} TONG_APPLY_CHANGE_CAMP_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int32_t 	m_nExtPoint;
} APPLY_GET_EXTPOINT_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int32_t 	m_nParam;
} APPLY_GET_PARAM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFailID;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_CHANGE_MASTER_FAIL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFailID;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_CHANGE_AGNAME_FAIL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_btFailID;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_CHANGE_CAMP_FAIL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
} TONG_CHANGE_CAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btLevel;
} TONG_CHANGE_LEVEL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_bRecruit;
} TONG_CHANGE_RECRUIT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int32_t 	m_nTongParam;
} TONG_CHANGE_TONGPARAM_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	BYTE	m_bRecruit;
} TONG_CHANGE_RECRUIT_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	int32_t		m_nTongParam;
} TONG_CHANGE_TONGPARAM_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int32_t		m_nTongJiyuParam;
	char 	m_szTongJiyuNotify[defTONG_NOTIFY_LENGTH];
} TONG_CHANGE_JIYU_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int32_t 	m_nSaveEff;
	int32_t 	m_dwTotalEff;
} TONG_CHANGE_EFF_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	char	m_szName[defTONG_NAME_LENGTH_32];
	int32_t		m_nMoney;
	int32_t		m_nTongJiyuParam;
	char 	m_szTongJiyuNotify[defTONG_NOTIFY_LENGTH];
} TONG_APPLY_CHANGE_INFO_COMMAND;

// tong Ïà¹Ø end

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwItemID;
	int32_t		nChange;
} ITEM_DURABILITY_CHANGE;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	int		Rank;
}	TITLE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	ID;
	KExpandRank		ExpandRank;
}	EXPANDTITLE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwItemID;
	BYTE	m_Number;
} ITEM_REPAIR;

typedef struct
{
	BYTE	ProtocolType;
	WORD	nTaskId;
	char	szTaskValue[16];
}S2C_SYNCTASKVALUE;

typedef struct tagS2C_PLAYER_SYNC
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	WORD			m_wMsgID;
	LPVOID			m_lpBuf;
	tagS2C_PLAYER_SYNC() {
		m_lpBuf = NULL;
		ProtocolType = NULL;
		m_wLength = NULL;
		m_wMsgID = NULL;
	};
	~tagS2C_PLAYER_SYNC() { Release(); }
	void	Release() {
		if (m_lpBuf)
		{
			m_lpBuf = malloc(sizeof(m_lpBuf));
			free(m_lpBuf);
		}
		m_lpBuf = NULL;
	}
} S2C_PLAYER_SYNC;

typedef struct tagPLAYER_COMMAND
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	WORD			m_wMsgID;
	LPVOID			m_lpBuf;
	WORD			nParam;
	tagPLAYER_COMMAND() {
		m_lpBuf = NULL;
		ProtocolType = NULL;
		m_wLength = NULL;
		m_wMsgID = NULL;
		nParam = NULL;
	};
	~tagPLAYER_COMMAND() { Release(); }
	void	Release() {
		if (m_lpBuf)
		{
			m_lpBuf = malloc(sizeof(m_lpBuf));
			free(m_lpBuf);
		}
		m_lpBuf = NULL;
	}
} PLAYER_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	char	szBuf[32];
} C2S_BUF_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	int32_t				nExtPoint;
	int32_t				nChangeExtPoint;
} S2C_EXTPOINT;

typedef struct
{
	BYTE	ProtocolType;
	char	m_szName[FILE_NAME_LENGTH];
	char	m_szInitString[MAX_MESSAGE_LENGTH];
}PLAYER_GIVE;

typedef struct
{
	BYTE	ProtocolType;
	DWORD	dwItemID;
	int32_t		nNum;
	BOOL	bIsBreakAll;
}PLAYER_BREAK_COMMAND;

typedef struct
{
	int32_t		m_nID;				// ÎïÆ·µÄID
	BYTE	m_btNature;
	BYTE	m_btGenre;			// ÎïÆ·µÄÀàÐÍ
	int32_t		m_btDetail;			// ÎïÆ·µÄÀà±ð
	int32_t		m_btParticur;		// ÎïÆ·µÄÏêÏ¸Àà±ð
	BYTE	m_btSeries;			// ÎïÆ·µÄÎåÐÐ
	BYTE	m_btLevel;			// ÎïÆ·µÄµÈ¼¶
	BYTE	m_btLuck;			// MF
	int32_t		m_btMagicLevel[MAX_ITEM_MAGICLEVEL];	// Éú³É²ÎÊý
	WORD	m_wVersion;			// ×°±¸°æ±¾
	DWORD	m_dwRandomSeed;		// Ëæ»úÖÖ×Ó
	int32_t		m_nIdx;
	UINT	m_uPrice;
	int32_t		m_bX;
	int32_t		m_bY;
	uint16_t 	m_nStackNum;
	int32_t		m_ExpireTime;
	int32_t		m_nParam;
	int32_t		m_nFortune;
} SViewSellItemInfo;

typedef struct
{
	BYTE			ProtocolType;
	SViewSellItemInfo	m_sInfo;
} PLAYER_TRADE_ITEM_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	LockMoveItem	m_LockMove;
}PLAYER_LOCKMOVE;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	DWORD			dwID;
	int32_t				nParam;
}PLAYER_TRADE;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	DWORD			dwID;
	BOOL			bOpen;
}PLAYER_TRADE_ACTION_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_nIdx;			// ÂòµÚ¼¸¸ö¶«Î÷
	DWORD			m_dwID;
	BYTE			m_Place;			// ·ÅÔÚÉíÉÏÄÄ¸öµØ·½
	BYTE			m_X;				// ×ø±êX
	BYTE			m_Y;				// ×ø±êY
} PLAYER_TRADE_BUY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	DWORD			m_dwID;				// ÎïÆ·µÄID
	int32_t				m_nPrice;
}PLAYER_TRADE_SET_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	char			Name[32];
}PLAYER_TRADE_START_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
}PLAYER_TRADE_START_FAIL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	char	m_szName[defTONG_NAME_LENGTH_32];
	char	m_szAgname[defTONG_NAME_LENGTH_32];
} TONG_CHANGE_AGNAME_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	int32_t		m_nEff;
	int32_t 	m_nSelect;
	char	m_szName[defTONG_NAME_LENGTH_32];
	BYTE	m_btFigure;
} TONG_APPLY_SAVE_EFF_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	DWORD	m_dwTongNameID;
	DWORD	m_dwMoney;
	char	m_szName[defTONG_NAME_LENGTH_32];
} TONG_APPLY_SAVE_COMMAND;

typedef struct
{
	DWORD	m_dwTongNameID;
	char	m_szAgname[defTONG_NAME_LENGTH_32];
	char	m_szName[defTONG_NAME_LENGTH_32];
	int32_t		m_nPlayerIdx;
} STONG_SERVER_TO_CORE_AGNAME;

typedef struct
{
	BYTE	ProtocolType;
	char	MissionName[64];
	TMissionLadderSelfInfo SelfData;
	TMissionLadderInfo MissionRank[MISSION_STATNUM];
} PLAYER_MISSION_RANKDATA;

typedef struct
{
	BYTE	ProtocolType;
	int32_t			m_nSaleType;
	BuySellInfo	m_BuySellInfo;
} S2C_SUPERSHOP;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	DWORD			m_dwID;
} SPAR_APPLY_START_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_nDestIdx;			// ÉêÇëÕßÔÚ·þÎñÆ÷¶ËµÄplayer idx
	DWORD			m_dwNpcId;			// ÉêÇëÕßµÄ npc id
} SPAR_APPLY_START_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Ð­ÒéÀàÐÍ
	int32_t				m_nDecision;
	BOOL			m_bRoomList;
} PLAYER_CHATROOM_DECISION_COMMAND;

typedef struct
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btErrorID;		// ¶ÓÎé´´½¨²»³É¹¦Ô­Òò£º0 Í¬Ãû 1 Íæ¼Ò±¾ÉíÒÑ¾­ÊôÓÚÄ³Ò»Ö§¶ÓÎé 3 µ±Ç°´¦ÓÚ²»ÄÜ×é¶Ó×´Ì¬
} PLAYER_SEND_CREATE_CHATROOM_FALSE;// ·þÎñÆ÷Í¨Öª¿Í»§¶Ë¶ÓÎé´´½¨²»³É¹¦

struct PLAYER_SEND_CHATROOM_STATE
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	BYTE	m_btMsgId;
	int32_t		nId;
	char	lpszName[16];
	PLAYER_SEND_CHATROOM_STATE() {
		lpszName[0] = 0;
		ProtocolType = NULL;
		m_btMsgId = NULL;
		nId = 0;
	}
};	// ·þÎñÆ÷Í¨ÖªÍæ¼Ò¶ÓÎé´´½¨³É¹¦

struct PLAYER_REQUEST_CHATROOM_LIST
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	char	lpszRoomHost[16];
	char	lpszRoomName[10];
	BYTE	byRoomMemNum;
	int32_t		nRoomLeftTime;
	int32_t		nRoomParam;
	PLAYER_REQUEST_CHATROOM_LIST() {
		nRoomParam = 0;
		ProtocolType = NULL;
		lpszRoomHost[0] = 0;
		lpszRoomName[0] = 0;
		byRoomMemNum = NULL;
		nRoomLeftTime = 0;
	}
};	// ·þÎñÆ÷Í¨ÖªÍæ¼Ò¶ÓÎé´´½¨³É¹¦

struct PLAYER_REQUEST_LIST
{
	BYTE	ProtocolType;		// Ð­ÒéÃû³Æ
	char	lpszName[_NAME_LEN];
	int32_t		nMemIndex;
	int32_t		nMemParam;
	BOOL	bIsRoomHost;
	PLAYER_REQUEST_LIST() {
		nMemParam = 0;
		lpszName[0] = 0;
		nMemIndex = 0;
		bIsRoomHost = FALSE;
		ProtocolType = NULL;
	}
};	// ·þÎñÆ÷Í¨ÖªÍæ¼Ò¶ÓÎé´´½¨³É¹¦

typedef struct
{
	BYTE	ProtocolType;
	int32_t		int_ID;
} CP_DATAU;

//TamLTM da tau
typedef struct
{
	BYTE			ProtocolType;
	int32_t				sScript;
} PLAYER_REQUEST_LOAD_DATAU;

// da tau nhiem vu
typedef struct
{
	BYTE	ProtocolType;
	int32_t		nIdQuestIndex;
} FINISH_QUEST_SYNC;
//end code

// TamLTM kham nam
typedef struct
{
	BYTE	ProtocolType;
	char	szString[80];
} GET_STRING;

typedef struct
{
	BYTE			ProtocolType;
	int32_t				nType;
	char			szFunc[32];
} PLAYER_UI_CMD_SCRIPT; // protocol load script

typedef struct
{
	BYTE			ProtocolType;
	DWORD			dwID;
	int32_t				nX;
	int32_t				nY;
} RECOVERY_BOX_CMD;

typedef struct
{
	BYTE ProtocolType;
	BYTE nValue;
} S2C_OTHER_BOX; // XANH

typedef struct
{
	BYTE	ProtocolType;
	BYTE	nType;
	int32_t		nNum[2];
	char	szStr[64];
	char	szFunc[32];
} C2S_PLAYER_INPUT_INFO;
//end code

//TamLTM Open progress bar
typedef struct
{
	BYTE			ProtocolType;
	int32_t				sScript;
} PLAYER_REQUEST_LOAD_PROGRESS_BAR;

typedef struct
{
	BYTE	ProtocolType;
	int32_t		nIdQuestIndex;
} OPEN_PROGRESS_BAR_SYNC;

//end code

//TamLTM Uy thac offline
typedef struct
{
	BYTE			ProtocolType;
} PLAYER_REQUEST_OFFLINE;
//end code

//Compound
typedef struct
{
	BYTE	ProtocolType;
	int32_t		nAction;
} C2S_COMPOUND_ITEM;
//end code


//rut tien
typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_byDir;			// È¡Ç®µÄ·½Ïò£¨0´æ£¬1È¡£©
	DWORD			m_dwMoney;			// Ç®Êý
} WITHDRAWA_MONEY_COMMAND; // rut tien;
//end code

// ÔÚµ÷ÓÃÕâÖ§º¯ÊýÖ®Ç°±ØÐëÅÐ¶ÏÊÇ·ñ´¦ÓÚ½»Ò××´Ì¬£¬Èç¹ûÕýÔÚ½»Ò×£¬²»ÄÜµ÷ÓÃÕâÖ§º¯Êý
void SendClientCmdSell(int32_t nID, int32_t nNumber);
void SendClientCmdDirectSell(int32_t nId, int32_t nNumber);
// ÔÚµ÷ÓÃÕâÖ§º¯ÊýÖ®Ç°±ØÐëÅÐ¶ÏÊÇ·ñ´¦ÓÚ½»Ò××´Ì¬£¬Èç¹ûÕýÔÚ½»Ò×£¬²»ÄÜµ÷ÓÃÕâÖ§º¯Êý
void SendClientCmdBuy(int32_t nShop, int32_t nBuyIdx, BYTE nNumber);
// ÔÚµ÷ÓÃÕâÖ§º¯ÊýÖ®Ç°±ØÐëÅÐ¶ÏÊÇ·ñ´¦ÓÚ½»Ò××´Ì¬£¬Èç¹ûÕýÔÚ½»Ò×£¬²»ÄÜµ÷ÓÃÕâÖ§º¯Êý
void SendClientCmdRun(int32_t nX, int32_t nY);
// ÔÚµ÷ÓÃÕâÖ§º¯ÊýÖ®Ç°±ØÐëÅÐ¶ÏÊÇ·ñ´¦ÓÚ½»Ò××´Ì¬£¬Èç¹ûÕýÔÚ½»Ò×£¬²»ÄÜµ÷ÓÃÕâÖ§º¯Êý
void SendClientCmdWalk(int32_t nX, int32_t nY);
// ÔÚµ÷ÓÃÕâÖ§º¯ÊýÖ®Ç°±ØÐëÅÐ¶ÏÊÇ·ñ´¦ÓÚ½»Ò××´Ì¬£¬Èç¹ûÕýÔÚ½»Ò×£¬²»ÄÜµ÷ÓÃÕâÖ§º¯Êý
void SendClientCmdSkill(int32_t nSkillID, int32_t nX, int32_t nY, int32_t nNpxPosX, int32_t nNpxPosY);
void SendClientCmdSit(bool bFlag);
void SendClientCmdMoveItem(void* pDownPos, void* pUpPos);
void SendClientCmdQueryLadder(DWORD	dwLadderID);
void SendClientCmdRequestNpc(int32_t nID);
void SendClientCmdStoreMoney(int32_t nDir, int32_t nMoney);
void SendClientCmdRevive();
void SendObjMouseClick(int32_t nObjID, DWORD dwRegionID);
void SendClientCmdRepair(DWORD dwID);
void SendClientCmdRide();
void SendClientCmdBreak(DWORD dwID, int32_t nNum, BOOL bIsBreakAll);
void SendClientCPSetImageCmd(int32_t ID);
void SendClientDaTauCmd(int32_t szScript); //TamLTM da tau
//TamLTM kham nam xanh
void SendUiCmdScript(int32_t nName, char* szFunc);
void SendClientRecoveryBox(DWORD dwID, int32_t nX, int32_t nY);
void SendClientCmdInputBox(BYTE nType, int32_t* nNum, char* szStr, char* szFunc);
//end code
void SendClientOpenProgressBarCmd(int32_t szScript); //TamLTM open progress bar
void SendClientOffline(); //TamLTM Uy thac offline
void SendCompoundAction(int32_t nAction); //Compound
void SendEnchaseAction(int32_t nAction); //Enchase
void SendClientCmdWithDrawaMoney(int32_t nDir, int32_t nMoney);

extern	int32_t	g_nProtocolSize[MAX_PROTOCOL_NUM];
#pragma pack(pop, enter_protocol)
#endif