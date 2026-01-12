#ifndef KPlayerH
#define	KPlayerH
//#include <objbase.h>
#include "KWorldMsg.h"
#include "KMessage.h"
#include "KInventory.h"
#include "KPlayerFaction.h"
#include "KPlayerChat.h"
#include "KPlayerTask.h"
#include "KPlayerMenuState.h"
#include "KPlayerTrade.h"
#include "KPlayerTeam.h"
#include "KPlayerPK.h"
#include "KPlayerAI.h"
#include "KPlayerTong.h"
#include "KPlayerChatRoom.h"
#include "KItemList.h"
#include "KNpc.h"
#include "KSkills.h"


#include "KPlayerDef.h"

#define		PLAYER_LIFE_REPLENISH			0
#define		PLAYER_MANA_REPLENISH			0
#define		PLAYER_STAMINA_GAIN				1
#define		PLAYER_STAMINA_LOSS				4

#define		STRENGTH_SET_DAMAGE_VALUE		5
#define		DEXTERITY_SET_DAMAGE_VALUE		5
#define		ENGERGY_SET_DAMAGE_VALUE		5
#define		UseEngergyDamage				2

#define		MAX_AVENGE_NUM					4

enum	UIInfo //½Å±¾Í¨ÖªÏÔÊ¾µÄ½çÃæÀàÐÍ
{
	UI_SELECTDIALOG,
	UI_SELDIALOG,
	UI_TALKDIALOG,
	UI_NOTEINFO,
	UI_MSGINFO,//×ÔÓÒÏò×óÃ°³öÀ´µÄÐÅÏ¢
	UI_NEWSINFO,//ÐÂÎÅ
	UI_NEWSINFO1,
	UI_PLAYMUSIC,
	UI_OPENTONGUI,
};

// ÖØÉúµãÎ»ÖÃÐÅÏ¢
typedef struct PLAYER_REVIVAL_POS_DATA
{
	INT				m_nSubWorldID;		// ÖØÉúµãµØÍ¼
	INT				m_ReviveID;			// ÖØÉúµãË÷Òý
	INT				m_nMpsX;			// ÖØÉúµãµØÍ¼Î»ÖÃ x
	INT				m_nMpsY;			// ÖØÉúµãµØÍ¼Î»ÖÃ y
} PLAYER_REVIVAL_POS;

typedef struct
{
	INT				m_nSubWorldId;		// ´«ËÍÃÅÊÀ½çID
	INT				m_nTime;			// ´«ËÍÃÅ±£³ÖÊ±¼ä
	INT				m_nMpsX;
	INT				m_nMpsY;
} PLAYER_TOWNPORTAL_POS;

typedef struct
{
	DWORD			m_dwMapID;
	INT				m_nX;
	INT				m_nY;
} PLAYER_EXCHANGE_POS;

// find way by kinnox;
struct KPLAYER_AUTOPATH
{
	BOOL	bAutoPathing;			//	The player is in the automatic pathfinding phase
	INT		nDesX;
	INT		nDesY;
	INT		nCurDesX;
	INT		nCurDesY;
	KPLAYER_AUTOPATH()
	{
		bAutoPathing = FALSE;
		nDesX = 0;
		nDesY = 0;
		nCurDesX = nCurDesY = 0;
	};
};
//

class KIniFile;


#ifdef TOOLVERSION
class CORE_API KPlayer
#else
class KPlayer
#endif
{
private:

#ifndef _SERVER
	INT				m_nLeftSkillID;
	INT				m_nLeftSkillLevel;
	INT				m_nRightSkillID;
	INT				m_nRightSkillLevel;
	BOOL			m_MouseDown[2];



#endif

#ifdef _SERVER
	PLAYER_REVIVAL_POS		m_sLoginRevivalPos;	// µÇÈëÖØÉúµãÎ»ÖÃ£¨»á´æÅÌ£©
	PLAYER_REVIVAL_POS		m_sDeathRevivalPos;	// ËÀÍöÖØÉúµã£¨Ä¬ÈÏÎªµÇÈëÖØÉúµã£¬²»´æÅÌ£©
	PLAYER_TOWNPORTAL_POS	m_sPortalPos;		// ´«ËÍÃÅÎ»ÖÃ
	BOOL					m_bUseReviveIdWhenLogin;
#endif
	INT						m_nPhysicsSkillID;		//µ±Ç°Íæ¼ÒµÄÎïÀí¹¥»÷¼¼ÄÜ
	INT						m_nPeapleIdx;
	INT						m_nObjectIdx;
	INT						m_nPickObjectIdx;
	INT						m_nPlayerIndex;				// ±¾ÊµÀýÔÚ Player Êý×éÖÐµÄÎ»ÖÃ
	KCacheNode* m_pLastScriptCacheNode;

public:

#ifdef _SERVER
	char					m_szLastName[32];
	DWORD					m_dwTaskExcuteScriptId;
	char					m_szTaskExcuteFun[32];
	char					m_szLastInput[32];
	BYTE					m_byLixian;
	PLAYER_EXCHANGE_POS		m_sExchangePos;
	KTimerTaskFun			m_TimerTask;
	BOOL					m_bIsQuiting;
	UINT					m_uMustSave;
	DWORD					m_ulLastSaveTime;
	DWORD					m_dwLoginTime;
	//DWORD			m_uLastPingTime;
	char					AccountName[32];
	void* m_pStatusLoadPlayerInfo;	//¼ÓÔØÍæ¼ÒÐÅÏ¢Ê±ÓÃ
	BYTE* m_pCurStatusOffset;			//¶þ½øÖÆÊ±£¬¼ÇÂ¼¶Áµ½Ö¸ÕëÎ»ÖÃÁË
	BOOL					m_bFinishLoading;			//Íê³É¼ÓÔØ
	BYTE					m_SaveBuffer[128 * 1024];	//±£´æ»º³å
	INT						m_nLastNetOperationTime;	//×îºóÒ»´ÎÍøÂç²Ù×÷Ê±¼ä
	BOOL					m_bSleepMode;
	KList					m_PlayerWayPointList;		//
	KList					m_PlayerStationList;
	INT						m_nViewEquipTime;			// ×îºóÒ»´Î²ì¿´ËûÈË×°±¸µÄÊ±¼ä
	INT						m_nPrePayMoney;
	char					m_szIpAddress[32];

	enum
	{
		FF_CHAT = 0x01,
	};
	INT				m_nForbiddenTm;			// ½ûÖ¹±êÖ¾
	BOOL				m_bForbidEnmity;
	BOOL				m_bForbidTrade;
	BOOL				m_bForbidUseTownP;
	BOOL				m_bForbidName;
	BOOL				m_bForbidCamp;
	BOOL				m_bForbidParty;
	BOOL				m_PlayerDBLoad;
	BOOL				m_bPlayerOffline;
	BOOL				m_bLogoutGateway;
#endif

#ifndef _SERVER
	INT				m_RunStatus;				// ÊÇÅÜ»¹ÊÇ×ß
	INT				m_nNextLevelLeadExp;		// Í³ÂÊÁ¦ÏÂÒ»¼¶¾­ÑéÖµ
	INT				m_nSendMoveFrames;	// ÓÃÓÚ¿ØÖÆ¿Í»§¶ËÏò·þÎñÆ÷·¢ËÍÒÆ¶¯(×ß»òÅÜ)Ð­ÒéµÄÆµÂÊ£¬Ê¹Ö®²»ÄÜ·¢ËÍ´óÁ¿µÄÒÆ¶¯Ð­Òé£¬¼õÐ¡´ø¿íÑ¹Á¦
	DWORD				m_dwRightMouse;
	INT				m_nLastNpcIndex;
	TMissionLadderSelfInfo m_MissionData;
	TMissionLadderInfo m_MissionRank[MISSION_STATNUM];
	BOOL				m_bDebugMode;
#endif

	KIndexNode			m_Node;
	GUID				m_Guid;
	BOOL				m_bExchangeServer;
	DWORD				m_dwID;						// Íæ¼ÒµÄ32Î»ID
	INT				m_nIndex;					// Íæ¼ÒµÄNpc±àºÅ
	INT				m_nNetConnectIdx;			// µÚ¼¸¸öÍøÂçÁ¬½Ó
	KItemList			m_ItemList;					// Íæ¼ÒµÄ×°±¸ÁÐ±í
	BuySellInfo			m_BuyInfo;					// ½øÐÐµÄ½»Ò×ÁÐ±í
	KPlayerMenuState	m_cMenuState;			// ÊÇ·ñ´¦ÓÚ½»Ò×»ò¶ÓÎé¿ª·Å×´Ì¬
	KTrade				m_cTrade;					// ½»Ò×Ä£¿é
#ifdef _SERVER
	PLAYERTRADE			m_PTrade;			// ÊÇ·ñ´¦ÓÚ½»Ò×»ò¶ÓÎé¿ª·Å×´Ì¬
	BYTE				m_nExtDay;
#endif
	INT				m_nAttributePoint;			// Î´·ÖÅäÊôÐÔµã
	INT				m_nSkillPoint;				// Î´·ÖÅä¼¼ÄÜµã

	INT				m_nStrength;				// Íæ¼ÒµÄ»ù±¾Á¦Á¿£¨¾ö¶¨»ù±¾ÉËº¦£©
	INT				m_nDexterity;				// Íæ¼ÒµÄ»ù±¾Ãô½Ý£¨¾ö¶¨ÃüÖÐ¡¢ÌåÁ¦£©
	INT				m_nVitality;				// Íæ¼ÒµÄ»ù±¾»îÁ¦£¨¾ö¶¨ÉúÃü¡¢ÌåÁ¦£©
	INT				m_nEngergy;					// Íæ¼ÒµÄ»ù±¾¾«Á¦£¨¾ö¶¨ÄÚÁ¦£©
	INT				m_nLucky;					// Íæ¼ÒµÄ»ù±¾ÔËÆø£¨Ó°ÏìµÃµ½×°±¸µÄºÃ»µ£©

	INT				m_nCurStrength;				// Íæ¼ÒµÄµ±Ç°Á¦Á¿£¨¾ö¶¨»ù±¾ÉËº¦£©
	INT				m_nCurDexterity;			// Íæ¼ÒµÄµ±Ç°Ãô½Ý£¨¾ö¶¨ÃüÖÐ¡¢ÌåÁ¦£©
	INT				m_nCurVitality;				// Íæ¼ÒµÄµ±Ç°»îÁ¦£¨¾ö¶¨ÉúÃü¡¢ÌåÁ¦£©
	INT				m_nCurEngergy;				// Íæ¼ÒµÄµ±Ç°¾«Á¦£¨¾ö¶¨ÄÚÁ¦£©

	INT				m_nExp;						// µ±Ç°¾­ÑéÖµ(µ±Ç°µÈ¼¶ÔÚnpcÉíÉÏ)
	INT				m_nNextLevelExp;			// ÏÂÒ»¼¶¾­ÑéÖµ

	INT				m_nLeadExp;				// Í³ÂÊÁ¦¾­ÑéÖµ
	INT				m_nLeadLevel;				// Í³ÂÊÁ¦µÈ¼¶
	char				Name[32];
	INT				m_ImagePlayer;

	KPlayerTeam			m_cTeam;					// Íæ¼ÒµÄ×é¶ÓÐÅÏ¢
	KPlayerFaction		m_cFaction;					// Íæ¼ÒµÄÃÅÅÉÐÅÏ¢

	KPlayerChat			m_cChat;

	KPlayerTask			m_cTask;					// Íæ¼ÒÈÎÎñÏµÍ³(±äÁ¿)

	KPlayerPK			m_cPK;						// PK¹ØÏµ´¦Àí

	KPlayerAI			m_cAI;

	KPlayerTong			m_cTong;					// ×Ô¼ºµÄ°ï»áÐÅÏ¢

	KPlayerChatRoom		m_cRoom;					// ×Ô¼ºµÄ°ï»áÐÅÏ¢

	DWORD				m_dwDeathScriptId;			// 
	DWORD				m_dwDamageScriptId;			// 

	char				m_szTaskAnswerFun[MAX_ANSWERNUM][32];
	INT				m_nAvailableAnswerNum;//µ±Ç°Ñ¡Ôñ½çÃæÏÂ£¬×î´ó»Ø´ðÊý¡£
	bool				m_bWaitingPlayerFeedBack;	//µ±Ç°ÊÇ·ñÕýµÈ´ýÍæ¼ÒÔÚ¿Í»§¶ËµÄ·´À¡¡£¸Ã×´Ì¬ÏÂ£¬µ±Ç°½Å±¾²»ÖÃ¿Õ.ÀàÊ½¶Ô»°Ñ¡ÔñÇé¿ö
	//	DWORD			m_dwOutOfDateFeedBackTime;	//µ±Ç°µÈ´ýÍæ¼ÒÓë·þÎñÆ÷½Å±¾½»»¥µÄ×î´óÏÞ¶¨Ê±¿Ì£¬µ½ÁËÕâ¸öÓÎÏ·Ê±¼äÈç¹ûÍæ¼ÒÈÔÎ´·´À¡ÔòÊÓ×÷·Ï¡£
	BYTE				m_btTryExecuteScriptTimes;	//
	//char				m_CurScriptName[128];
	INT				m_nWorldStat;
	INT				m_nSectStat;
	INT				m_nKillPeopleNumber;

	BYTE				m_nLockPKState;

	DWORD				m_TalkUiScriptId;
	DWORD				m_SelUiScriptId;

	INT				m_nPaceBarTime;
	INT				m_nPaceBarTimeMax;
public:
	KPlayer();
	~KPlayer();

	void			ChatFriendOnLine(DWORD dwID, INT nFriendIdx);// 获得通知某好友上线了
	void			SetLockMove(LockMoveItem* LockMove);
	LockMoveItem*	GetLockMove() { return &m_LockMove; };
#ifdef _SERVER
	void			SetLevel(INT nLevel);
	INT				IsOffLine() { return m_bPlayerOffline; };
	INT				IsLogoutGateway() { return m_bLogoutGateway; };
	void			LogoutGateway() { m_bLogoutGateway = TRUE; };
	BYTE			GetExtDay() { return m_nExtDay; };
	void			SetExtDay(BYTE nExtDay) { m_nExtDay = nExtDay; };
#endif
	void			SetLockState(BOOL bLock);
	BOOL			GetLockState();
	void			SetEquipExpandTime(INT dwTime);
	void			SetExpandBoxNum(INT nNum);

	void			SetExtPoint(INT nPoint, INT nChangePoint);
	void			AddExtPoint(INT nPoint, INT nChangePoint);
	INT				GetExtPoint();

	INT				GetCoin();
	INT				GetExtPointChanged();

	void			SetPlayerIndex(INT nNo);					// Éè¶¨ m_nPlayerIndex
	void			GetAboutPos(KMapPos* pMapPos);			// »ñµÃÍæ¼Ò¸½½üÒ»¸ö¿ÕÎ»ÖÃ
	INT				GetPlayerIndex();							// »ñµÃ±¾ÊµÀýÔÚ Player Êý×éÖÐµÄÎ»ÖÃ
	DWORD			GetPlayerID() { return m_dwID; };
	void			GetFaction(char* lpszName);	// »ñµÃµ±Ç°ÃÅÅÉÃû³Æ not end
	void			GetFactionName(char* lpszName);	// »ñµÃµ±Ç°ÃÅÅÉÃû³Æ not end
	INT				GetFactionCamp();
	INT				GetFactionNo();
	BOOL			ExecuteScript(char* ScriptFileName, char* szFunName, INT nParam = 0, const char* nType = SCRIPT_PLAYERID);
	BOOL			ExecuteScript(char* ScriptFileName, char* szFunName, char* szParams, const char* nType = SCRIPT_PLAYERID);
	BOOL			ExecuteScript(DWORD dwScriptId, char* szFunName, char* szParams, const char* nType = SCRIPT_PLAYERID);
	BOOL			ExecuteScript(DWORD dwScriptId, char* szFunName, INT nParam, const char* nType = SCRIPT_PLAYERID);
	BOOL			ExecuteScriptA(char* ScriptFileName, char* szFunName, INT nParam = 0, char* szCanshu = NULL, BOOL nIsGive = TRUE); //NPCå¯¹è¯æ¡†è°ƒç”¨
	BOOL			ExecuteScriptB(DWORD dwScriptId, char* szFunName, INT nParam = 0, char* szCanshu = NULL, BOOL nIsGive = TRUE);

	BOOL			ExecuteScript2Param(char* ScriptFileName, char* szFunName, INT nResultCount, INT nParam1 = 0, INT nParam2 = 0);
	BOOL			ExecuteScript2Param(DWORD dwScriptId, char* cFuncName, INT nResultCount, INT nParam1, INT nParam2);
	;
	BOOL			ExecuteScript3Param(char* ScriptFileName, char* szFunName, INT nResultCount, INT nParam1 = 0, INT nParam2 = 0, INT nParam3 = 0);
	BOOL 			ExecuteScript3Param(DWORD dwScriptId, char* cFuncName, INT nResultCount, INT nParam1, INT nParam2, INT nParam3);
	BOOL			ExecuteScriptC(char* ScriptFileName, char* szFunName, char* szParams = NULL, char* snParams = NULL, INT inParam = 0, INT njb = 0, INT njxb = 0, INT inval = 0, INT invala = 0, BOOL nIsGive = TRUE);
	BOOL			ExecuteScriptD(DWORD dwScriptId, char* szFunName, char* szParams = NULL, char* snParams = NULL, INT inParam = 0, INT njb = 0, INT njxb = 0, INT inval = 0, INT invala = 0, BOOL nIsGive = FALSE);

	// TamLTM kham
	BOOL			ExecuteScript2(char* ScriptFileName, char* szFunName, INT nParam1 = 0, INT nParam2 = 0);
	BOOL			ExecuteScript2(char* ScriptFileName, char* szFunName, char* szParams1, char* szParams2);
	BOOL			ExecuteScript2(DWORD dwScriptId, char* szFunName, char* szParams1, char* szParams2);
	BOOL			ExecuteScript2(DWORD dwScriptId, char* szFunName, INT nParam1, INT nParam2);
	//end code

	BOOL			DoScript(char* ScriptCommand);				//Ö´ÐÐÄ³¸ö½Å±¾Ö¸Áî
	void			SendTitle();

	void			ChangeCurStrength(INT nData);			// ¸Ä±äµ±Ç°Á¦Á¿(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
	void			ChangeCurDexterity(INT nData);			// ¸Ä±äµ±Ç°Ãô½Ý(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
	void			ChangeCurVitality(INT nData);			// ¸Ä±äµ±Ç°»îÁ¦(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
	void			ChangeCurEngergy(INT nData);			// ¸Ä±äµ±Ç°¾«Á¦(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
	BOOL			ExecuteScript(char* ScriptFileName);
	void			Release();
	void			Active();								// Íæ¼ÒÃ¿´ÎÓÎÏ·Ñ­»·¶¼ÐèÒª´¦ÀíµÄ¶«Î÷
	void			ProcessMsg(KWorldMsgNode* lpMsg);		// ´¦ÀíÊÀ½çÏûÏ¢£¬×ªÎªNPCÃüÁî

	LPSTR			GetPlayerName() { return Name; };

	BOOL			NewPlayerGetBaseAttribute(INT Series);	// ÐÂÍæ¼ÒµÇÂ½Ê±¸ù¾ÝÎåÐÐÊôÐÔ²úÉú Á¦Á¿ Ãô½Ý »îÁ¦ ¾«Á¦ ËÄÏîÊýÖµ
	void			AddBaseLucky(INT nData);				// Ôö¼Ó»ù±¾ÔËÆø
#ifdef _SERVER
	void			AddExp(INT nExp, INT nTarLevel);		// Ôö¼Ó¾­Ñé(Ô­Ê¼Êý¾Ý£¬»¹Î´¾­¹ý´¦Àí)
	void			AddSelfExp(INT nExp, INT nTarLevel);	// Ôö¼Ó¾­Ñé(²»ÐèÒªÔÙ¾­¹ý¶ÓÎé·ÖÅäµÄ´¦Àí£¬µ«ÐèÒª¿¼ÂÇ±»¿³ËÀnpcµÄµÈ¼¶)
	void			AddSkillExp(INT nExp);
	void			DirectAddExp(INT nExp);					// Ö±½ÓÔö¼Ó¾­ÑéÖµ£¬²»¿¼ÂÇÆäËûÒòËØ
	void			LevelUp();								// ÉýÒ»¼¶
#endif
	void			AddLeadExp(INT nExp);					// Ôö¼ÓÍ³ÂÊÁ¦¾­Ñé
	void			SetLeadLevel(INT nLevel);					// Ôö¼ÓÍ³ÂÊÁ¦¾­Ñé
	void			UpdataCurData();
	void			ReCalcEquip();							// ÖØÐÂ¼ÆËãÉíÉÏµÄ×°±¸
	void			ReCalcSkill();
	void			ReCalcState();
	void			ChangePlayerCamp(INT nCamp);			// ¸Ä±äÍæ¼ÒÕóÓª
	void			Revive(INT nType);

	BOOL			CheckTrading(bool bOverLookTrade = false);
	void			SetFirstDamage();
	void			SetFirstMagic();
	void			SetBaseAttackRating();
	void			SetBaseDefence();
	void			SetBaseResistData();
	void			SetBaseSpeedAndRadius();
	INT				GetPhysicsSkillId() { return m_nPhysicsSkillID; };
	void			SetPhysicsSkillId(INT nPhysicsSkillID)
	{
		if (nPhysicsSkillID <= 0) return;
		ISkill* pISkill = g_SkillManager.GetSkill(nPhysicsSkillID, 1);
		if (!pISkill)
			return;

		if (nPhysicsSkillID > 0 && pISkill->IsPhysical())
			m_nPhysicsSkillID = nPhysicsSkillID;
	};

	INT				m_nExtPoint;
	INT				m_nChangeExtPoint;
	BOOL			m_bLockState;
	INT				m_dwEquipExpandTime;
	INT				m_btRepositoryNum;
	LockMoveItem	m_LockMove;
#ifndef _SERVER
	INT				GetTargetNpc() { return m_nPeapleIdx; };
	INT				GetTargetObj() { return m_nObjectIdx; };
	void			SetTargetNpc(INT n) { m_nPeapleIdx = n; };
	void			SetTargetObj(INT n) { m_nObjectIdx = n; };
	void			FindSelectNpc(INT x, INT y, INT nRelation);
	void			FindSelectObject(INT x, INT y);
	void			Walk(INT nDir, INT nSpeed);
	void			TurnLeft();
	void			TurnRight();
	void			TurnBack();
	BOOL			ConformIdx(INT nIdx);
	void			GetEchoDamage(INT* nMin, INT* nMax, INT nType);// »ñÈ¡½çÃæÐèÒªÏÔÊ¾µÄÉËº¦Öµ
	void			ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);// ´¦Àí¼üÅÌÊó±êÏûÏ¢
	void			AutoRun(INT x, INT y);
	void			RecvSyncData();								// ½ÓÊÕÍ¬²½Êý¾Ý

	void			ApplyTeamInfo(DWORD dwNpcID);				// Ïò·þÎñÆ÷ÉêÇë²éÑ¯Ä³¸önpcËùÔÚ¶ÓÎéµÄÐÅÏ¢
	void			ApplySelfTeamInfo();						// Ïò·þÎñÆ÷ÉêÇë²éÑ¯Íæ¼Ò×ÔÉíµÄ¶ÓÎéÇé¿ö
	BOOL			ApplyCreateTeam();//char *lpszTeamName);		// Íæ¼ÒÏò·þÎñÆ÷ÉêÇë´´½¨¶ÓÎé
	BOOL			ApplyTeamOpenClose(BOOL bFlag);				// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇë¿ª·Å¡¢¹Ø±Õ¶ÓÎéÊÇ·ñÔÊÐí¼ÓÈë³ÉÔ±×´Ì¬
	BOOL			ApplyTeamPKFollow(BOOL bFlag);
	BOOL			ApplyTeamModePick(INT nSel);
	void			ApplyAddTeam(INT nNpcIndex);				// Íæ¼ÒÏò·þÎñÆ÷ÉêÇë¼ÓÈëÄ³¸ö¶ÓÎé
	void			AcceptTeamMember(DWORD dwNpcID);			// Íæ¼ÒÍ¨Öª·þÎñÆ÷½ÓÊÜÄ³¸önpcÎª¶ÓÎé³ÉÔ±
	void			TeamDropApplyOne(DWORD dwNpcID);			// ¶Ó³¤É¾³ý¼ÓÈë¶ÓÎéÉêÇëÁÐ±íÖÐµÄÄ³¸önpc
	void			LeaveTeam();								// Í¨Öª·þÎñÆ÷±¾Íæ¼ÒÀë¿ª¶ÓÎé
	void			TeamKickMember(DWORD dwNpcID);				// ¶Ó³¤Í¨Öª·þÎñÆ÷Ìß³ýÄ³¸ö¶ÓÔ±
	void			ApplyTeamChangeCaptain(DWORD dwNpcID);		// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇë°Ñ×Ô¼ºµÄ¶Ó³¤Éí·Ý½»¸ø±ðµÄ¶ÓÔ±
	void			ApplyTeamDismiss();							// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇë½âÉ¢¶ÓÎé
	void			ApplySetPK(BOOL bPK);						// Íæ¼ÒÏò·þÎñÆ÷ÉêÇë´ò¿ª¡¢¹Ø±Õpk¿ª¹Ø
	void			SendChat(KUiMsgParam* pMsg, char* lpszSentence);// ¿Í»§¶Ë·¢ËÍÁÄÌìÓï¾ä¸ø·þÎñÆ÷
	void			ApplyAddBaseAttribute(INT nAttribute, INT nNo);// ¶Ó³¤Ïò·þÎñÆ÷ÉêÇëÔö¼ÓËÄÏîÊôÐÔÖÐÄ³Ò»ÏîµÄµãÊý(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	BOOL			ApplyAddSkillLevel(INT nSkillID, INT nAddPoint);// Ïò·þÎñÆ÷ÉêÇëÄ³¸ö¼¼ÄÜÉý¼¶
	BOOL			ApplyUseItem(INT nItemID, ItemPos SrcPos);	// Ïò·þÎñÆ÷ÉêÇëÊ¹ÓÃÄ³¸öÎïÆ·£¨Êó±êÓÒ¼üµã»÷¸ÃÎïÆ·£©
	BOOL			CanUseItem(INT nIdx);
	void			PickUpObj(INT nObjIndex);					// ¿Í»§¶ËÊó±êµã»÷obj¼ìÆðÄ³¸öÎïÆ·£¬Ïò·þÎñÆ÷·¢ÏûÏ¢
	void			ObjMouseClick(INT nObjIndex);				// ¿Í»§¶ËÊó±êµã»÷obj£¬Ïò·þÎñÆ÷·¢ÏûÏ¢
	void			MoveItem(ItemPos DownPos, ItemPos UpPos);	// DownPos ²»ÄÜÊÇÊÖ£¬UpPos ±ØÐëÊÇÊÖ
	INT				ThrowAwayItem();
	void			TradeApplyOpen(char* lpszSentence, INT nLength);
	void			TradeApplyClose();
	void			TradeApplyStart(INT nNpcIdx, bool bFolkGame);
	BOOL			TradeMoveMoney(INT nMoney);					// ½»Ò×Ê±ÊäÈë×Ô¼ºµÄÇ®	
	void			TradeDecision(INT nDecision, INT nId = -1);				// if nDecision == 0 ÍÆ³ö½»Ò×  if nDecision == 1 È·¶¨½»Ò×  if nDecision == 2 È¡Ïû½»Ò×È·¶¨
	void			TradeApplyLock();				// ½»Ò×Ëø¶¨»òÈ¡ÏûËø¶¨

	void			SetChatCurChannel(INT nChannelNo);			// Éè¶¨µ±Ç°ÁÄÌìÆµµÀ
	void			TeamInviteAdd(DWORD dwNpcID);				// ÑûÇë¼ÓÈë¶ÓÎé

	void			SetLeftSkill(INT nSkillID);
	void			SetRightSkill(INT nSkillID);
	void			UpdateWeaponSkill();
	INT				GetLeftSkill() { return m_nLeftSkillID; };
	INT				GetRightSkill() { return m_nRightSkillID; };
	void			SetDefaultImmedSkill();
	void			s2cApplyAddTeam(BYTE* pProtocol);			// ÊÕµ½·þÎñÆ÷Í¨ÖªÓÐÈËÉêÇë¼ÓÈë¶ÓÎé
	void			s2cTradeChangeState(BYTE* pMsg);
	void			s2cTradeMoneySync(BYTE* pMsg);
	void			s2cTradeDecision(BYTE* pMsg);				// ÊÕµ½·þÎñÆ÷Í¨Öª½»Ò×Íê³É»òÈ¡Ïû
	void			SyncCurPlayer(BYTE* pMsg);
	void			s2cLevelUp(BYTE* pMsg);
	void			s2cGetCurAttribute(BYTE* pMsg);
	void			s2cSetExp(INT nExp);
	void			s2cSyncMoney(BYTE* pMsg);
	void			s2cTradeApplyStart(BYTE* pMsg);
	void			s2cJoinTongReply(BYTE* pMsg);
	void			s2cSparApplyStart(BYTE* pMsg);
	void			CheckObject(INT nIdx);
	void			PickObjectNear();
#endif
#ifdef _SERVER
	BOOL			PayExtPoint(INT nPoint);
	BOOL			PayCoin(INT nCoin);
	void			RepairItem(DWORD dwItemID);
	void			AutoLoseItem(DWORD dwItemID);
	void			PlayerBreakItem(DWORD dwItemID, INT nNum, BOOL bIsBreakAll = FALSE);
	BOOL			PrePay(INT nMoney);
	void			SetLastNetOperationTime(INT nTime);
	INT				FindAroundPlayer(DWORD dwNpcID);		// Ñ°ÕÒÍæ¼ÒÖÜÎ§µÄÄ³¸öÖ¸¶¨npc idµÄplayer index
	INT				FindAroundNpc(DWORD dwNpcID);			// Ñ°ÕÒÍæ¼ÒÖÜÎ§µÄÄ³¸öÖ¸¶¨npc idµÄnpc index
	INT				FindNearNpc(INT nNpcSettingIdx, INT nDist = 0);
	INT				FindNearNpc(const char* szName, INT nDist = 0);
	BOOL			IsExchangingServer();
	void			TobeExchangeServer(DWORD dwMapID, INT nX, INT nY);
	BOOL			IsWaitingRemove();
	BOOL			IsLoginTimeOut();
	void			WaitForRemove();
	void			LoginTimeOut();
	BOOL			UseTownPortal();
	BOOL			BackToTownPortal();
	BOOL			BackToTownPortal(INT nWorld, INT nPosX, INT nPosY);
	void			GetLoginRevivalPos(INT* lpnSubWorld, INT* lpnMpsX, INT* lpnMpsY);		// »ñÈ¡Íæ¼ÒµÇÈëÖØÉúµãÎ»ÖÃ
	void			GetDeathRevivalPos(INT* lpnSubWorld, INT* lpnMpsX, INT* lpnMpsY);		// »ñÈ¡Íæ¼ÒËÀÍöÖØÉúµãÎ»ÖÃ
	void			SetRevivalPos(INT nSubWorld, INT nRevalId);								// Éè¶¨Íæ¼ÒÖØÉúµãID
	void			GetLoginRevival(POINT* Pos) {
		Pos->x = m_sLoginRevivalPos.m_nSubWorldID;
		Pos->y = m_sLoginRevivalPos.m_ReviveID;
	};
	INT				GetLoginRevivalID() { return m_sLoginRevivalPos.m_ReviveID; };
	BOOL			Save();									// ±£´æÍæ¼ÒÊý¾Ý
	BOOL			CanSave();
	void			ProcessUser();
	BOOL			SendSyncData(INT& nStep, UINT& nParam);	// ·¢ËÍÍ¬²½Êý¾Ý
	BOOL			SendSyncData_Skill();					// ·¢ËÍÍ¬²½Êý¾Ý - ¼¼ÄÜ
	void			SendCurNormalSyncData();				// ·¢ËÍÆ½Ê±¸ø×Ô¼ºµÄÍ¬²½Êý¾Ý
	void			SetChatForbiddenTm(INT nTm)
	{
		m_nForbiddenTm = nTm;
	};
	void			ForbidEnmity(BOOL bFlag);
	void			ForbidTrade(BOOL bFlag);
	void			ForbidUseTownP(BOOL bFlag);
	void			ForbidName(BOOL bFlag);
	void			ForbidCamp(BOOL bFlag);
	void 			ForbidParty(BOOL bFlag);

	BOOL			IsForbidUseTownP() { return m_bForbidUseTownP; };

	BOOL			AddFaction(char* lpszFactionName);		// ¼ÓÈëÃÅÅÉ
	BOOL			AddFaction(INT nFactionID);				// ¼ÓÈëÃÅÅÉ
	BOOL			LeaveCurFaction();						// Àë¿ªÃÅÅÉ

	void			BuyItem(BYTE* pProtocol);
	void			SellItem(BYTE* pProtocol);
	void			DirectSellItem(BYTE* pProtocol);
	void			QuitGame(INT nQuitType);				// ÍË³öÓÎÏ·
	void			S2CSendTeamInfo(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶Ë²éÑ¯Ä³¸önpc×é¶ÓÐÅÏ¢µÄÉêÇëºó£¬ÏòÕâ¸ö¿Í»§¶Ë·¢ËÍ¶ÓÎéÐÅÏ¢
	void			SendSelfTeamInfo();						// ·þÎñÆ÷Ïò¿Í»§¶Ë·¢ËÍ¶ÓÎéÐÅÏ¢
	BOOL			CreateTeam(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÇëÇó´´½¨Ò»Ö§¶ÓÎé
	BOOL			SetTeamState(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÇëÇó¿ª·Å¡¢¹Ø±Õ±¾player¶ÓÎéÊÇ·ñÔÊÐí¶ÓÔ±¼ÓÈë×´Ì¬
	BOOL			S2CSendAddTeamInfo(BYTE* pProtocol);	// ÊÕµ½¿Í»§¶ËÇëÇó¼ÓÈëÒ»Ö§¶ÓÎé
	BOOL			AddTeamMember(BYTE* pProtocol);			// ¶Ó³¤Í¨Öª·þÎñÆ÷½ÓÊÜÄ³¸önpcÎª¶ÓÎé³ÉÔ±
	void			LeaveTeam(BYTE* pProtocol);				// ÊÕµ½¿Í»§¶Ë¶ÓÔ±Í¨ÖªÀë¿ª¶ÓÎé
	void			SendSystemMessage(const char* szHead, const char* szMessage);
	void			SendTeamMessage(INT nTeamID, const char* szMessage);
	void			TeamKickOne(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶Ë¶Ó³¤Í¨ÖªÌß³öÄ³¸ö¶ÓÔ±
	BOOL			TeamChangeCaptain(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶Ë¶Ó³¤Í¨Öª°Ñ¶Ó³¤Éí·Ý½»¸øÄ³¸ö¶ÓÔ±
	void			TeamDismiss(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶Ë¶Ó³¤ÇëÇó½âÉ¢¶ÓÎé
	void			SetPK(BYTE* pProtocol);					// ÊÕµ½¿Í»§¶ËÇëÇóÉè¶¨PK×´Ì¬
	void			ServerSendChat(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶Ë·¢À´µÄÁÄÌìÓï¾ä
	void			AddBaseAttribute(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÒªÇóÔö¼Ó»ù±¾ÊôÐÔµã(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	INT				ResetProp();
	void			ResetBaseAttribute(BYTE* pProtocol);
	void			AddSkillPoint(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÒªÇóÔö¼ÓÄ³¸ö¼¼ÄÜµÄµãÊý
	void			IncSkillLevel(INT nSkillId, INT nAddLevel);
	void			IncSkillExp(INT nSkillId, INT nAddExp);
	INT				GetSkillExp(INT nSkillId);
	BOOL			ServerPickUpItem(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÏûÏ¢Êó±êµã»÷Ä³¸öobj¼ðÆð×°±¸»ò½ðÇ®
	void			EatItem(BYTE* pProtocol);				// ÊÕµ½¿Í»§¶ËÏûÏ¢³ÔÒ©
	void			ServerMoveItem(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÏûÏ¢ÒÆ¶¯ÎïÆ·
	void			ServerThrowAwayItem(BYTE* pProtocol);	// ÊÕµ½¿Í»§¶ËÏûÏ¢¶ªÆúÎïÆ·
	void			TradeApplyOpen(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇë½øÈë´ý½»Ò××´Ì¬
	void			TradeApplyClose(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇëÈ¡Ïû´ý½»Ò××´Ì¬
	void			TradeApplyStart(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇë¿ªÊ¼½»Ò×
	void			TradeMoveMoney(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇë½»Ò×ÖÐmoneyµÄ¸Ä±ä
	void			TradeDecision(BYTE* pProtocol);			// ÊÕµ½¿Í»§¶ËÉêÇë½»Ò×È·¶¨»òÈ¡Ïû
	void			TradeFolkGameDecision(BYTE btDecision);			// ÊÕµ½¿Í»§¶ËÉêÇë½»Ò×È·¶¨»òÈ¡Ïû
	void			c2sTradeReplyStart(BYTE* pProtocol);
	void			SyncTradeState(BOOL bSelfAsk = FALSE, BOOL bDestReply = FALSE);						// ¸ø½»Ò×Ë«·½µÄ¿Í»§¶Ë·¢ËÍ½»Ò××´Ì¬ÐÅÏ¢
	void			SendEquipItemInfo(INT nTargetPlayer);	// ·¢ËÍ×Ô¼º×°±¸ÔÚÉíÉÏµÄ×°±¸ÐÅÏ¢¸ø±ðÈË¿´

	void			SparApplyStart(BYTE* pProtocol);		// ÊÕµ½¿Í»§¶ËÉêÇë¿ªÊ¼½»Ò×

	PLAYER_REVIVAL_POS* GetDeathRevivalPos()
	{
		return &m_sDeathRevivalPos;
	};
	BOOL			IsUseReviveIdWhenLogin() { return m_bUseReviveIdWhenLogin; };
	void			SetLoginType(BOOL bUseReviveId) { m_bUseReviveIdWhenLogin = bUseReviveId; };

	BOOL			CreateTong(INT nCamp, char* lpszTongName);
	void			SetNumImg(INT nNumber);

	void			TradeStart(BYTE* pProtocol);
	void			SendTradeCancel();
	BOOL			SendTradeItem(INT nIndex);	// ·¢ËÍ×Ô¼ºÉèÖÃµÄ³öÊÛÐÅÏ¢¸ø±ðÈË¿´
	void			SendTradeCount(INT nIndex);
	INT				GetTradeCount();

	void			SendMSGroup();
	void			SendMSRank(TMissionLadderSelfInfo* SelfData, TMissionLadderInfo* RankData);

	void			SetSavePw(char* szTask, BOOL bShow);
	BOOL			CheckSavePw(const char* szTask);

	void			LoadScript(INT nScript); //TamLTM da tau vng
	void			LoadScriptProgressBar(INT nScript); //TamLTM Load progress bar
	void			Offline();//TamLTM Uy Thac offline
	void			RecoveryBox(DWORD dwID, INT nX, INT nY); // TamLTM Kham nam xanh

	INT				Compound(INT);
	INT				Enchase(INT, INT, INT, INT);
#endif
private:
	//	void			CalcCurStrength();						// ¼ÆËãµ±Ç°Á¦Á¿
	//	void			CalcCurDexterity();						// ¼ÆËãµ±Ç°Ãô½Ý
	//	void			CalcCurVitality();						// ¼ÆËãµ±Ç°»îÁ¦
	//	void			CalcCurEngergy();						// ¼ÆËãµ±Ç°¾«Á¦
	void			LevelAddBaseLifeMax();					// µÈ¼¶ÉýÒ»¼¶ºóÔö¼Ó×î´óÉúÃüµã
	void			LevelAddBaseManaMax();					// µÈ¼¶ÉýÒ»¼¶ºóÔö¼Ó×î´óÄÚÁ¦µã
	void			LevelAddBaseStaminaMax();				// µÈ¼¶ÉýÒ»¼¶ºóÔö¼Ó×î´óÌåÁ¦µã

	void			SendFactionData();						// Ïò¿Í»§¶Ë·¢ËÍÃÅÅÉÊý¾Ý

	void			SyncCurrentBaseAttriibute(INT type, INT attribute, INT curAttribute);
	void			AddBaseStrength(INT nData);				// Ôö¼Ó»ù±¾Á¦Á¿
	void			AddBaseDexterity(INT nData);			// Ôö¼Ó»ù±¾Ãô½Ý
	void			AddBaseVitality(INT nData);				// Ôö¼Ó»ù±¾»îÁ¦
	void			AddBaseEngergy(INT nData);				// Ôö¼Ó»ù±¾¾«Á¦

	void			ResetBaseStrength(INT nData);			// Ôö¼Ó»ù±¾Á¦Á¿
	void			ResetBaseDexterity(INT nData);			// Ôö¼Ó»ù±¾Ãô½Ý
	void			ResetBaseVitality(INT nData);			// Ôö¼Ó»ù±¾»îÁ¦
	void			ResetBaseEngergy(INT nData);			// Ôö¼Ó»ù±¾¾«Á¦

	void			SetNpcPhysicsDamage();					// ÓÉµ±Ç°Á¦Á¿¼ÆËã¶ÔÓ¦npcµÄÎïÀíÉËº¦(PhysicsDamage)
	void			SetNpcEngergyDamage();					// ÓÉµ±Ç°Á¦Á¿¼ÆËã¶ÔÓ¦npcµÄÎïÀíÉËº¦(PhysicsDamage)
	void			SetNpcAttackRating();					// ÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄ¹¥»÷ÃüÖÐÂÊ(AttackRating)
	void			SetNpcDefence();						// ÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄ·ÀÓùÁ¦
	//	void			SetNpcWalkSpeed();						// ÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄÐÐ×ßËÙ¶È
	//	void			SetNpcRunSpeed();						// ÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄÅÜ²½ËÙ¶È
#ifndef _SERVER
	void			ProcessMouse(INT x, INT y, INT Key, MOUSE_BUTTON nButton);
	void			OnButtonUp(INT x, INT y, MOUSE_BUTTON nButton);				// ´¦ÀíÊó±ê¼üÌ§Æð
	void			OnButtonDown(INT x, INT y, INT Key, MOUSE_BUTTON nButton);		// ´¦ÀíÊó±ê¼ü°´ÏÂ
	void			OnButtonMove(INT x, INT y, INT Key, MOUSE_BUTTON nButton);		// ´¦ÀíÊó±ê¼ü°´ÏÂºóÒÆ¶¯
	void			OnMouseMove(INT x, INT y);									// ´¦ÀíÊó±êÒÆ¶¯

	//Question:Îªµ¥»ú²âÊÔ°æÊ¹ÓÃ
	friend INT			LuaInitStandAloneGame(Lua_State* L);
#endif


	// ¶¹¶¹µÄ¶«Î÷
private:
	void			S2CExecuteScript(char* ScriptName, char* szParam);

#ifdef _SERVER
	//Êý¾Ý¿âÄ£¿éº¯Êý-----------------
private:
	INT				LoadPlayerBaseInfo(BYTE* pRoleBuffer, BYTE*& pRoleBaseBuffer, UINT& nParam);
	INT				LoadPlayerItemList(BYTE* pRoleBuffer, BYTE*& pItemBuffer, UINT& nParam);
	INT				LoadPlayerFightSkillList(BYTE* pRoleBuffer, BYTE*& pSkillBuffer, UINT& nParam);
	INT				LoadPlayerStateSkillList(BYTE* pRoleBuffer, BYTE*& pSkillBuffer, UINT& nParam);
	INT					LoadPlayerFriendList(BYTE* pRoleBuffer, BYTE*& pFriendBuffer, UINT& nParam);
	INT				LoadPlayerTaskList(BYTE* pRoleBuffer, BYTE*& pTaskBuffer, UINT& nParam);
	INT				SavePlayerBaseInfo(BYTE* pRoleBuffer);
	INT				SavePlayerItemList(BYTE* pRoleBuffer);
	INT				SavePlayerFightSkillList(BYTE* pRoleBuffer);
	INT				SavePlayerStateSkillList(BYTE* pRoleBuffer);
	INT				SavePlayerFriendList(BYTE* pRoleBuffer);
	INT				SavePlayerTaskList(BYTE* pRoleBuffer);

#endif

public:
	void			SetNpcDamageAttrib();
	void			DoScriptAction(PLAYER_SCRIPTACTION_SYNC* pUIInfo); //Í¨Öª¸Ã¿Í»§¶ËÏÔÊ¾Ä³¸öUI½çÃæ
	void			ProcessPlayerSelectFromUI(BYTE* pProtocol);			// ´¦Àíµ±Íæ¼Ò´ÓÑ¡Ôñ²Ëµ¥Ñ¡ÔñÄ³ÏîÊ±µÄ²Ù×÷
#ifndef _SERVER
	void			DialogNpc(INT nIndex);
	void			OnSelectFromUI(PLAYER_SELECTUI_COMMAND* pSelectUI, UIInfo eUIInfo);//µ±Íæ¼Ò´ÓÑ¡Ôñ¿òÖÐÑ¡ÔñÄ³Ïîºó£¬½«Ïò·þÎñÆ÷·¢ËÍ			
	void			OnScriptAction(PLAYER_SCRIPTACTION_SYNC*);
#endif
#ifdef _SERVER
	void			RestoreLiveData();						//ÖØÉúºó»Ö¸´Íæ¼ÒµÄ»ù±¾Êý¾Ý
	void			SetTimer(DWORD nTime, INT nTimeTaskId);//Ê±¼äÈÎÎñ½Å±¾£¬¿ªÆô¼ÆÊ±Æ÷
	void			CloseTimer();							//¹Ø±ÕÊ±¼ä¼ÆÊ±Æ÷

	INT				AddDBPlayer(char* szPlayerName, INT sex, DWORD* pdwID);
	INT				LoadDBPlayerInfo(BYTE* pPlayerInfo, INT& nStep, UINT& nParam);
	BOOL			GetNewPlayerFromIni(KIniFile* pIniFile, BYTE* pRoleBuffer);
	INT				UpdateDBPlayerInfo(BYTE* pPlayerInfo);
	INT				DeletePlayer(char* szPlayerName = NULL);//×¢Òâ£º±¾º¯ÊýÊÇÇå³ýÍæ¼ÒÕÊºÅ£¡£¡£¡£¬²»ÄÜÂÒÓÃ
	void			LaunchPlayer();
	BOOL			Pay(INT nMoney);
	BOOL			Earn(INT nMoney);
	void			DialogNpc(BYTE* pProtocol);

	void			SetBaseStrength(INT nData);
	void			SetBaseDexterity(INT nData);
	void			SetBaseVitality(INT nData);
	void			SetBaseEngergy(INT nData);
#endif
};

#ifdef TOOLVERSION
extern CORE_API KPlayer	Player[MAX_PLAYER];
#else
extern KPlayer	Player[MAX_PLAYER];
#endif
#endif //KPlayerH

