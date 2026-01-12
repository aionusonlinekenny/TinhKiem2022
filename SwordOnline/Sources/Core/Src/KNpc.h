#ifndef KNpcH
#define KNpcH
#include <vector> // find way by kinnox;
//---------------------------------------------------------------------------
class ISkill;
#include "KCore.h"
#include "KSkillList.h"
#include "KMagicAttrib.h"
#include "GameDataDef.h"
#include "KNpcFindPath.h"
#include "KNpcDeathCalcExp.h"
#include "KIndexNode.h"
#include "KNpcGold.h"
#include <map>

class KSkill;
#ifndef _SERVER
#include "KNpcRes.h"
#include "../KJXPathFinder.h" // find way by kinnox;
#endif
//---------------------------------------------------------------------------
#define MAX_NPCSTYLE	3000

#define		MAX_AI_PARAM				11
#define		MAX_NPC_USE_SKILL			4

#define		BLOOD_COUNT					15
//	ÅÜ¶¯ËÙ¶ÈÓëÐÐ×ßËÙ¶ÈµÄ±¶Êý
//#define	WALK_RUN_TIMES	3

#define		STATE_FREEZE	0x0001
#define		STATE_POISON	0x0002
#define		STATE_STUN		0x0004
#define		STATE_HIDE		0x0008
#define		STATE_FROZEN	0x0010
#define		STATE_WALKRUN	0x0020

#define		MAX_FREEZE_TIME_PERCENT_REDUCE		277
#define		MAX_POISON_TIME_PERCENT_REDUCE		277
#define		MAX_STUN_TIME_PERCENT_REDUCE		277
#define		MAX_STATE_PERCENT			300

#define		MAX_FREEZE_TIME		15*18
#define		MAX_STUN_TIME		3*18
#define		MAX_POISON_TIME		60*18
#define		MAX_RANDMOVE_TIME		5*18
#define		MAX_FROZEN_TIME		5*18

enum NPCATTRIB
{
	attrib_mana_v,
	attrib_stamina_v,
	attrib_life_v,
	attrib_mana_p,
	attrib_stamina_p,
	attrib_life_p,
};

enum NPCCMD
{
	do_none,		// Ê²Ã´Ò²²»¸É
	do_stand,		// Õ¾Á¢
	do_walk,		// ÐÐ×ß
	do_run,			// ÅÜ¶¯
	do_jump,		// ÌøÔ¾
	do_skill,		// ·¢¼¼ÄÜµÄÃüÁî
	do_magic,		// Ê©·¨
	do_attack,		// ¹¥»÷
	do_sit,			// ´ò×ø
	do_hurt,		// ÊÜÉË
	do_death,		// ËÀÍö
	do_defense,		// ¸ñµ²
	do_idle,		// ´­Æø
	do_specialskill,// ¼¼ÄÜ¿ØÖÆ¶¯×÷
	do_special1,	// ÌØÊâ1
	do_special2,	// ÌØÊâ2
	do_special3,	// ÌØÊâ3
	do_blurmove,	// ÌØÊâ4
	do_runattack,
	do_manyattack,
	do_jumpattack,
	do_revive,
	do_goattack,
};

enum CLIENTACTION
{
	cdo_fightstand,
	cdo_stand,
	cdo_stand1,
	cdo_fightwalk,
	cdo_walk,
	cdo_fightrun,
	cdo_run,
	cdo_hurt,
	cdo_death,
	cdo_attack,
	cdo_attack1,
	cdo_magic,
	cdo_sit,
	cdo_jump,
	cdo_none,
	cdo_count,
};

enum DAMAGE_TYPE
{
	damage_physics = 0,		// ÎïÀíÉËº¦
	damage_fire,			// »ðÑæÉËº¦
	damage_cold,			// ±ù¶³ÉËº¦
	damage_light,			// ÉÁµçÉËº¦
	damage_poison,			// ¶¾ËØÉËº¦
	damage_magic,			// ÎÞÊôÐÔÉËº¦
	damage_num,				// ÉËº¦ÀàÐÍÊýÄ¿
};

// DoDeath Ê±µÄ²ÎÊý£¬¶ÔÓ¦²»Í¬µÄËÀÍö³Í·£
enum	enumDEATH_MODE
{
	enumDEATH_MODE_NPC_KILL = 0,		// ±»npcÉ±ËÀ
	enumDEATH_MODE_PLAYER_NO_PUNISH,	// ÇÐ´èÄ£Ê½±»Íæ¼ÒÉ±ËÀ
	enumDEATH_MODE_PLAYER_SPAR_NO_PUNISH,	// ÇÐ´èÄ£Ê½±»Íæ¼ÒÉ±ËÀ
	enumDEATH_MODE_PLAYER_PUNISH,		// ±»Íæ¼ÒPKÖÂËÀ£¬¸ù¾ÝPKÖµ½øÐÐ³Í·£
	enumDEATH_MODE_PKBATTLE_PUNISH,		// ÀàÊ½ÓÚ¹úÕ½Ê±µÄ³Í·£´¦Àí
	enumDEATH_MODE_TOURNAMENTS_PUNISH,
	enumDEATH_MODE_NUM,
};

typedef struct
{
	NPCCMD		CmdKind;		// ÃüÁîC
	INT			Param_X;		// ²ÎÊýX
	INT			Param_Y;		// ²ÎÊýY
	INT			Param_Z;		// ²ÎÊýY
} NPC_COMMAND;

typedef struct
{
	INT		nTotalFrame;
	INT		nCurrentFrame;
} DOING_FRAME;

struct KState
{
	INT	nMagicAttrib;
	INT	nValue[3];
	INT	nTime;
};

//TamLTM Lag pos npc
struct	KSyncPos
{
	DWORD	m_dwRegionID;
	INT		m_nMapX;
	INT		m_nMapY;
	INT		m_nOffX;
	INT		m_nOffY;
	INT		m_nDoing;
};
//end code

#ifdef _SERVER
struct KNpcAutoFindPath
{
	INT nCurXpos;
	INT nCurYpos;
	BYTE nCurNum;
	BYTE nIsHaveDo;
	INT nCurPathPos; // 当前路线的编号 用于读取txt
	INT nCurType;
	INT nCurDisten;
};

// Explosion rate member variable Explosion rate structure
struct KItemDropRate
{
	struct KItemParam
	{
		INT nGenre;
		INT nDetailType;
		INT nParticulType;
		INT nRate;			// Explosion rate
		INT nQuality;		// Is it gold?
		BYTE nMinItemLevel; // The minimum level of the item
		BYTE nMaxItemLevel; // The maximum level of the item
		BYTE nHour;			// Limited time
		BYTE nIsBang;		// Whether to bind
		BYTE nIsNoBian;		// Whether it is identification
		INT nStackNum;		// The number of overlays
		BYTE nIsNoRandGold; // Is it not random gold?
		// INT nDropType; //Type
	}; // Item part

	INT nCount;
	INT nMagicRate;
	INT nMaxRandRate;
	INT nMoneyRate;
	INT nMoneyNum;
	INT nMoneyScale;
	BYTE nMinItemLevelScale; // Minimum level probability
	BYTE nMaxItemLevelScale;
	BYTE nMinItemLevel;
	BYTE nMaxItemLevel;
	INT nTypeNum;	 // The number of drops
	BYTE nIsBianShi; // Whether it is possible to explode identifiable equipment
	KItemParam* pItemParam;
};

#endif

class KStateNode : public KNode
{
public:

	INT				m_SkillID;					// ¼¼ÄÜID
	INT				m_Level;					// ¼¼ÄÜµÈ¼¶
	INT				m_LeftTime;					// Ê£ÓàÊ±¼ä
	BOOL			m_bOverLook;
	BOOL			m_bTempStateGraphics;
	KMagicAttrib	m_State[MAX_SKILL_STATE];	// ÐÞ¸ÄÊôÐÔÁÐ±í
	INT				m_StateGraphics;			// ×´Ì¬¶¯»­Ë÷Òý
};

#ifndef _SERVER
/*--- ÓÃÓÚ±êÃ÷¿Í»§¶ËnpcÊÇÄÄ¸öregionµÄµÚ¼¸¸önpc £¬
Èç¹ûÕâÊÇÒ»¸ö·þÎñÆ÷¿ØÖÆµÄnpc £¬ID ÖµÎª 0 £¬No ÖµÎª -1  ---*/
struct	KClientNpcID
{
	DWORD	m_dwRegionID;
	INT		m_nNo;
};
#endif

class KNpc
{
	friend class KNpcSet;
public:
	DWORD					m_dwID;					// NpcµÄID
	UINT					m_IsRevive;	 // Whether to regenerate
	BYTE					m_IsRe;		 // Synchronize the number of rebirth frames
	INT						__isDoNone;
	INT					m_Index;				// NpcµÄË÷Òý
	KIndexNode				m_Node;					// Npc's Node
	INT					m_Level;				// NpcµÄµÈ¼¶
	DWORD					m_Kind;					// NpcµÄÀàÐÍ
	INT					m_Series;				// NpcµÄÏµ
	BYTE					m_btSpecial;			// NpcµÄ¼ÓÇ¿ÀàÐÍ£¨»Æ½ð¹ÖÎï£©
	BOOL					m_bNpcRemoveDeath;
	INT					m_nNpcTimeout;
	INT					m_nNpcParam[MAX_NPCPARAM];
	BOOL					m_bNpcFollowFindPath;
	DWORD					m_uFindPathTime;
	DWORD					m_uFindPathMaxTime;
	DWORD					m_uLastFindPathTime;
	INT					m_Height;				// NpcµÄ¸ß¶È(ÌøÔ¾µÄÊ±ºò·ÇÁã)
	BYTE					m_RankID;
	INT						m_NpcTitle;	   // NPC new title (archive)
	INT						m_CurNpcTitle; // Current title (temporary title is not archived)
	BYTE					m_btRankFFId; // SPR title
	BYTE					m_AutoplayId; // On-hook SPR title title
	KExpandRank				m_ExpandRank;
	KExpandRank				m_CurExpandRank;
	INT					m_nStature;				//Tall 
	BYTE					m_byTranslife;
	BYTE					m_byViprank;
	BYTE					m_byMantleLevel;
	BYTE					m_ImagePlayer;
	INT					m_nFactionNumber;
	BYTE					m_btStateInfo[MAX_SKILL_STATE];	// Npcµ±Ç°×îÐÂµÄ¼¸¸ö×´Ì¬ 
	//ÓÃÓÚNpcÄ£°å¿âÖÐ£¬µ±FALSE±íÊ¾¸ÃNpcÊý¾Ýµ±Ç°ÊÇÎÞÐ§µÄ £¬ÊýÖµÎ´¾­¹ý½Å±¾¼ÆËã£¬ÐèÒªÉú³É.
	//TRUE±íÊ¾ÓÐÐ§Êý¾Ý
	BOOL					m_bHaveLoadedFromTemplate;// 

	KState					m_PoisonState;			// ÖÐ¶¾×´Ì¬
	KState					m_FreezeState;			// ±ù¶³×´Ì¬
	KState					m_BurnState;			// È¼ÉÕ×´Ì¬
	KState					m_StunState;			// Ñ£ÔÎ×´Ì¬
	KState					m_FrozenAction;			// È¼ÉÕ×´Ì¬
	KState					m_LifeState;			// ²¹Ñª×´Ì¬
	KState					m_ManaState;			// ²¹MANA×´Ì¬
	KState					m_LoseMana;			// ×í¾Æ×´Ì¬
	KState					m_HideState;
	KState					m_SilentState;
	KState					m_RandMove;			// »ìÂÒ×´Ì¬
	KState					m_WalkRun;

	KState					m_PhysicsArmor;
	KState					m_ColdArmor;
	KState					m_LightArmor;
	KState					m_PoisonArmor;
	KState					m_FireArmor;
	KState					m_ManaShield;

	KList					m_StateSkillList;		// ¸½¼Ó×´Ì¬¼¼ÄÜÁÐ±í£¨Í¬¼¼ÄÜ²»µþ¼Ó£©
	INT					m_Camp;					// NpcµÄÕóÓª
	INT					m_CurrentCamp;			// NpcµÄµ±Ç°ÕóÓª
	NPCCMD					m_Doing;				// NpcµÄÐÐÎª
	CLIENTACTION			m_ClientDoing;			// NpcµÄ¿Í»§¶ËÐÐÎª
#ifndef _SERVER
	char szNpcTypeName[32];
#endif
	DOING_FRAME				m_Frames;				// NpcµÄÐÐÎªÖ¡Êý
	KSkillList				m_SkillList;			// NpcµÄ¼¼ÄÜÁÐ±í
	INT					m_SubWorldIndex;		// NpcËùÔÚµÄSubWorld ID
	INT					m_RegionIndex;			// NpcËùÔÚµÄRegion ID
	INT					m_ActiveSkillID;		// Npc¼¤»îµÄ¼¼ÄÜID
	INT						m_ActiveSkListIndex;

	INT					m_ActiveAuraID;			// Npc¼¤»îµÄ¹â»·¼¼ÄÜID

	// NpcµÄÊµ¼ÊÊý¾Ý£¨ÒÑ¾­¾­¹ý×°±¸¼°¼¼ÄÜµÄÔËËãÁË£©
	INT						m_CurrentExperience;	// Npc±»É±ºóËÍ³öµÄ¾­Ñé
	INT						m_CurrentLife;			// NpcµÄµ±Ç°ÉúÃü
	INT						m_CurrentLifeMax;		// NpcµÄµ±Ç°ÉúÃü×î´óÖµ
	INT						m_CurrentLifeReplenish;	// NpcµÄµ±Ç°ÉúÃü»Ø¸´ËÙ¶È
	INT						m_CurrentLifeReplenishPercent;
	INT						m_CurrentMana;			// NpcµÄµ±Ç°ÄÚÁ¦
	INT						m_CurrentManaMax;		// NpcµÄµ±Ç°×î´óÄÚÁ¦
	INT						m_CurrentManaReplenish;	// NpcµÄµ±Ç°ÄÚÁ¦»Ø¸´ËÙ¶È
	INT						m_CurrentStamina;		// NpcµÄµ±Ç°ÌåÁ¦
	INT						m_CurrentStaminaMax;	// NpcµÄµ±Ç°×î´óÌåÁ¦
	INT						m_CurrentStaminaGain;	// NpcµÄµ±Ç°ÌåÁ¦»Ø¸´ËÙ¶È
	INT						m_CurrentStaminaLoss;	// NpcµÄµ±Ç°ÌåÁ¦ÏÂ½µËÙ¶È
	KMagicAttrib			m_PhysicsDamage;		// NpcµÄµ±Ç°ÉËº¦(ÓÉÁ¦Á¿Ãô½ÝÓëÎäÆ÷ÉËº¦¾ö¶¨£¬²»¿¼ÂÇÖ±½Ó¼ÓÉËº¦µÄÄ§·¨ÊôÐÔ)
	KMagicAttrib			m_PhysicsMagic;
	KMagicAttrib			m_CurrentFireDamage;	// NpcµÄµ±Ç°»ðÉËº¦
	KMagicAttrib			m_CurrentColdDamage;	// NpcµÄµ±Ç°±ùÉËº¦
	KMagicAttrib			m_CurrentLightDamage;	// NpcµÄµ±Ç°µçÉËº¦
	KMagicAttrib			m_CurrentPoisonDamage;	// NpcµÄµ±Ç°¶¾ÉËº¦
	KMagicAttrib			m_CurrentFireMagic;
	KMagicAttrib			m_CurrentColdMagic;
	KMagicAttrib			m_CurrentLightMagic;
	KMagicAttrib			m_CurrentPoisonMagic;

	INT						m_CurrentAttackRating;	// NpcµÄµ±Ç°ÃüÖÐÂÊ
	INT						m_CurrentDefend;		// NpcµÄµ±Ç°·ÀÓù

	INT						m_CurrentFireResist;	// NpcµÄµ±Ç°»ð¿¹ÐÔ
	INT						m_CurrentColdResist;	// NpcµÄµ±Ç°±ù¿¹ÐÔ
	INT						m_CurrentPoisonResist;	// NpcµÄµ±Ç°¶¾¿¹ÐÔ
	INT						m_CurrentLightResist;	// NpcµÄµ±Ç°µç¿¹ÐÔ
	INT						m_CurrentPhysicsResist;	// NpcµÄµ±Ç°ÎïÀí¿¹ÐÔ
	INT						m_CurrentFireResistMax;		// NpcµÄµ±Ç°×î´ó»ð¿¹ÐÔ
	INT						m_CurrentColdResistMax;		// NpcµÄµ±Ç°×î´ó±ù¿¹ÐÔ
	INT						m_CurrentPoisonResistMax;	// NpcµÄµ±Ç°×î´ó¶¾¿¹ÐÔ
	INT						m_CurrentLightResistMax;	// NpcµÄµ±Ç°×î´óµç¿¹ÐÔ
	INT						m_CurrentPhysicsResistMax;	// NpcµÄµ±Ç°×î´óÎïÀí¿¹ÐÔ

	INT						m_CurrentWalkSpeed;		// NpcµÄµ±Ç°×ß¶¯ËÙ¶È
	INT						m_CurrentRunSpeed;		// NpcµÄµ±Ç°ÅÜ¶¯ËÙ¶È
	INT						m_CurrentJumpSpeed;		// NpcµÄµ±Ç°ÌøÔ¾ËÙ¶È
	INT						m_CurrentJumpFrame;		// NpcµÄµ±Ç°ÌøÔ¾Ö¡Êý
	INT						m_CurrentAttackSpeed;	// NpcµÄµ±Ç°¹¥»÷ËÙ¶È
	INT						m_CurrentCastSpeed;		// NpcµÄµ±Ç°Ê©·¨ËÙ¶È
	INT						m_CurrentVisionRadius;	// NpcµÄµ±Ç°ÊÓÒ°·¶Î§
	INT						m_CurrentAttackRadius;	// NpcµÄµ±Ç°¹¥»÷·¶Î§
	INT						m_CurrentActiveRadius;	// NpcµÄµ±Ç°»î¶¯·¶Î§
	INT						m_CurrentHitRecover;	// NpcµÄµ±Ç°ÊÜ»÷»Ø¸´ËÙ¶È
	INT						m_CurrentHitNpcRecover; // Increase the amount of damage caused to the opponent
	INT						m_CurrentTreasure;		// Number of equipment dropped by NPC
	INT						m_CurrentHitRank;		// probability of injury

	// ÒÔÏÂÊý¾ÝÖ»ÐèÒªµ±Ç°Öµ£¬»ù´¡Öµ¾ùÎª0
	INT						m_CurrentMeleeDmgRetPercent;	// Npc½ü³ÌÉËº¦·µ»ØµÄ°Ù·Ö±È
	INT						m_CurrentMeleeDmgRet;			// Npc½ü³Ì±»»÷Ê±·µ»ØµÄÉËº¦µãÊý
	INT						m_CurrentRangeDmgRetPercent;	// NpcÔ¶³ÌÉËº¦·µ»ØµÄ°Ù·Ö±È
	INT						m_CurrentRangeDmgRet;			// NpcÔ¶³Ì±»»÷Ê±·µ»ØµÄÉËº¦µãÊý
	INT						m_CurrentReturnResPercent;
	BOOL					m_CurrentSlowMissle;			// NpcÊÇ·ñÔÚÂýËÙ×Óµ¯×´Ì¬ÏÂ

	INT						m_CurrentDamageReduce;			// ÎïÀíÉËº¦¼õÉÙ
	INT						m_CurrentElementDamageReduce;	// ÔªËØÉËº¦¼õÉÙ

	INT						m_CurrentDamage2Mana;			// ÉËº¦×ªÄÚÁ¦°Ù·Ö±È
	INT						m_CurrentLifeStolen;			// ÍµÉúÃü°Ù·Ö±È
	INT						m_CurrentManaStolen;			// ÍµÄÚÁ¦°Ù·Ö±È
	INT						m_CurrentStaminaStolen;			// ÍµÌåÁ¦°Ù·Ö±È
	INT						m_CurrentDeadlyStrikeEnhanceP;			// ÖÂÃüÒ»»÷°Ù·Ö±È
	INT						m_CurrentFatallyStrikeEnhanceP;			// ÖÂÃüÒ»»÷°Ù·Ö±È
	INT						m_CurrentFatallyStrikeResP;
	INT						m_CurrentPiercePercent;			// ´©Í¸¹¥»÷°Ù·Ö±È
	INT						m_CurrentFreezeTimeReducePercent;	// ±ù¶³Ê±¼ä¼õÉÙ°Ù·Ö±È
	INT						m_CurrentPoisonTimeReducePercent;	// ÖÐ¶¾Ê±¼ä¼õÉÙ°Ù·Ö±È
	INT						m_CurrentStunTimeReducePercent;		// Ñ£ÔÎÊ±¼ä¼õÉÙ°Ù·Ö±È
	INT			 			m_CurrentReturnSkillPercent;
	INT						m_CurrentIgnoreSkillPercent;
	INT 					m_CurrentPoisonDamageReturn;
	INT 					m_CurrentPoisonDamageReturnPercent;
	KMagicAutoSkill 		m_ReplySkill[MAX_AUTOSKILL];
	KMagicAutoSkill 		m_RescueSkill[MAX_AUTOSKILL];
	KMagicAutoSkill 		m_AttackSkill[MAX_AUTOSKILL];
	KMagicAutoSkill 		m_CastSkill[MAX_AUTOSKILL];
	KMagicAutoSkill 		m_DeathSkill[MAX_AUTOSKILL];
	INT						m_CurrentIgnoreNegativeStateP;

	INT						m_CurrentFireEnhance;			// »ð¼ÓÇ¿
	INT						m_CurrentColdEnhance;			// ±ù¼ÓÇ¿
	INT						m_CurrentPoisonEnhance;			// ¶¾¼ÓÇ¿f
	INT						m_CurrentLightEnhance;			// µç¼ÓÇ¿
	INT						m_CurrentAddPhysicsDamage;		// Ö±½ÓµÄÎïÀíÉËº¦¼ÓÇ¿µãÊý
	INT						m_CurrentAddPhysicsDamageP;		 // 当前被动外普百分比
	INT						m_CurrentAddPhysicsMagic;		// Sat thuong vat ly noi cong
	INT						m_CurrentAddEngergyDamage;		// noi cong tang damage
	INT						m_CurrentMeleeEnhance[MAX_MELEE_WEAPON * 2];	// ½ü³ÌÎïÀí¼ÓÇ¿
	INT						m_CurrentRangeEnhance;			// Ô¶³ÌÎïÀí¼ÓÇ¿
	INT						m_CurrentHandEnhance;			// ¿ÕÊÖÎïÀí¼ÓÇ¿
	INT			 			m_CurrentManaShield;
	INT			 			m_CurrentStaticMagicShieldP;
	INT						m_CurrentLucky;
	INT						m_CurrentExpEnhance;
	INT						m_CurrentSerisesEnhance;				 // The damage of the five elements is enhanced

	INT						m_CurrentSkillEnhancePercent;
	INT						m_CurrentFiveElementsEnhance;
	INT						m_CurrentFiveElementsResist;
	INT						m_CurrentManaToSkillEnhanceP;
	// Ö»ÐèÒªµ±Ç°ÖµµÄÊý¾Ý½áÊø
	INT						m_Dir;							// NpcµÄ·½Ïò
	INT						m_RedLum;						// NpcµÄÁÁ¶È
	INT						m_GreenLum;
	INT						m_BlueLum;
	INT						m_MapX, m_MapY, m_MapZ;			// NpcµÄµØÍ¼×ø±ê
	INT						m_OffX, m_OffY;					// NpcÔÚ¸ñ×ÓÖÐµÄÆ«ÒÆ×ø±ê£¨·Å´óÁË1024±¶£©
	INT						m_DesX, m_DesY;					// NpcµÄÄ¿±ê×ø±ê
	INT						m_SkillParam1, m_SkillParam2;
	INT						m_OriginX, m_OriginY;			// NpcµÄÔ­Ê¼×ø±ê
	INT						m_NextAITime;
	BYTE					m_AIMAXTime;//NpcAI
	// NpcµÄ×°±¸£¨¾ö¶¨¿Í»§¶ËµÄ»»×°±¸£©	
	INT					m_HelmType;					// NpcµÄÍ·¿øÀàÐÍ
	INT					m_ArmorType;				// NpcµÄ¿ø¼×ÀàÐÍ
	INT					m_WeaponType;				// NpcµÄÎäÆ÷ÀàÐÍ
	INT					m_HorseType;				// NpcµÄÆïÂíÀàÐÍ
	BOOL					m_bRideHorse;				// NpcÊÇ·ñÆïÂí
	DWORD					m_dwNextSwitchHorseTime;
	INT					m_MaskType;					// Npc Ãæ¾ß¹¦ÄÜ
	BOOL					m_bMaskFeature;				// NpcÊÇ·ñÆïÂí
	INT					m_MantleType;
	BYTE					m_nPKFlag;
	INT					m_nMissionGroup;
#ifndef _SERVER
	PLAYERTRADE				m_PTrade;					// NpcÊÇ·ñÔÚ×°ÚÌ
	INT					m_MarkMask;
	DWORD					m_dwTongNameID;
	char					m_szTongName[defTONG_NAME_LENGTH_32];
	char					m_szTongAgname[defTONG_NAME_LENGTH_32];
	INT					m_nTongNationalEmblem;
	INT					m_nFigure;
	INT					m_nTeamServerID;
#endif
	char					Name[32];				// NpcµÄÃû³Æ
	char					_Name[32];			// Alternate name for Npc
	char					Owner[32];
	INT					m_nOwnerIndex;					// Player Owner Index
	INT					m_nFollowIndex;
	char					MateName[16];				// NpcµÄÃû³Æ
	INT					m_nSex;					// NpcµÄÐÔ±ð0ÎªÄÐ£¬1ÎªÅ®
	INT					m_NpcSettingIdx;		// NpcµÄÉè¶¨ÎÄ¼þË÷Òý
	INT					m_CorpseSettingIdx;		// NpcµÄÊ¬Ìå¶¨ÒåË÷Òý
	typedef std::map<INT, std::string> _StrNoteInfo;
	_StrNoteInfo nstrNoteInfo;

	void ClearstrInfo(INT i)
	{
		if (nstrNoteInfo.count(i) > 0)
		{
			nstrNoteInfo.erase(i);
		}
	}

	char* GetstrInfo(INT i, char* str)
	{
		// char nMsg[256]={0};
		/*_StrNoteInfo::iterator it;
		for( it = nstrNoteInfo.begin(); it != nstrNoteInfo.end(); ++it)
		{
			 if (it->first == i)
			 {
#ifdef _SERVER
				 printf("--获取(%s)脚本:%s --\n",Name,it->second.c_str());
#endif
				 sprintf(str,it->second.c_str());
				 return str;
			 }
		} */
		if (nstrNoteInfo.count(i) > 0)
		{ // 如果存在
			sprintf(str, nstrNoteInfo[i].c_str());

			/*#ifdef _SERVER
						printf("--获取(%s)(%d)脚本:%s --\n",Name,i,nstrNoteInfo[i].c_str());
			#endif */

			return str;
		}
		return NULL;
	}
	void SetstrInfo(INT i, char* str)
	{
		if (str)
			nstrNoteInfo[i] = str;
		else
			nstrNoteInfo[i] = "系统";
		/*#ifdef _SERVER
						 printf("--设置(%s)(%d)脚本:%s --\n",Name,i,nstrNoteInfo[i].c_str());
		#endif */
	}
	char					ActionScript[80];		// NpcµÄÐÐÎª½Å±¾
	DWORD					m_ActionScriptID;		// NpcµÄÐÐÎª½Å±¾ID£¨Ê¹ÓÃÊ±ÓÃÕâ¸öÀ´¼ìË÷£©
	DWORD					m_DropScriptID;		// NpcµÄÐÐÎª½Å±¾ID£¨Ê¹ÓÃÊ±ÓÃÕâ¸öÀ´¼ìË÷£©

	DWORD					m_TrapScriptID;			// NpcµÄµ±Ç°Trap½Å±¾ID;

	INT					m_nPeopleIdx;			// ¶ÔÏóÈËÎï
	INT					m_nLastDamageIdx;		// ×îºóÒ»´ÎÉËº¦µÄÈËÎïË÷Òý
	INT					m_nLastPoisonDamageIdx;	// ×îºóÒ»´Î¶¾ÉËº¦µÄÈËÎïË÷Òý
	INT					m_nObjectIdx;			// ¶ÔÏóÎïÆ·

	// NpcµÄ»ù±¾Êý¾Ý£¨Î´¿¼ÂÇ×°±¸¡¢¼¼ÄÜµÄÓ°Ïì£©
	INT					m_Experience;			// Npc±»É±»ñµÃµÄ¾­Ñé
	INT					m_LifeMax;				// NpcµÄ×î´óÉúÃü
	INT					m_LifeReplenish;		// NpcµÄÉúÃü»Ø¸´ËÙ¶È
	INT					m_ManaMax;				// NpcµÄ×î´óÄÚÁ¦
	INT					m_ManaReplenish;		// NpcµÄÄÚÁ¦»Ø¸´ËÙ¶È
	INT					m_StaminaMax;			// NpcµÄ×î´óÌåÁ¦
	INT					m_StaminaGain;			// NpcµÄÌåÁ¦»Ø¸´ËÙ¶È
	INT					m_StaminaLoss;			// NpcµÄÌåÁ¦ÏÂ½µËÙ¶È
	INT					m_AttackRating;			// NpcµÄÃüÖÐÂÊ
	INT					m_Defend;				// NpcµÄ·ÀÓù
	INT					m_FireResist;			// NpcµÄ»ð¿¹ÐÔ
	INT					m_ColdResist;			// NpcµÄÀä¿¹ÐÔ
	INT					m_PoisonResist;			// NpcµÄ¶¾¿¹ÐÔ
	INT					m_LightResist;			// NpcµÄµç¿¹ÐÔ
	INT					m_PhysicsResist;		// NpcµÄÎïÀí¿¹ÐÔ
	INT					m_FireResistMax;		// NpcµÄ×î´ó»ð¿¹ÐÔ
	INT					m_ColdResistMax;		// NpcµÄ×î´ó±ù¿¹ÐÔ
	INT					m_PoisonResistMax;		// NpcµÄ×î´ó¶¾¿¹ÐÔ
	INT					m_LightResistMax;		// NpcµÄ×î´óµç¿¹ÐÔ
	INT					m_PhysicsResistMax;		// NpcµÄ×î´óÎïÀí¿¹ÐÔ
	INT					m_WalkSpeed;			// NpcµÄÐÐ×ßËÙ¶È
	INT					m_RunSpeed;				// NpcµÄÅÜ¶¯ËÙ¶È
	INT					m_JumpSpeed;			// NpcµÄÌøÔ¾ËÙ¶È
	INT					m_AttackSpeed;			// NpcµÄ¹¥»÷ËÙ¶È
	INT					m_CastSpeed;			// NpcµÄÊ©·¨ËÙ¶È
	INT					m_VisionRadius;			// NpcµÄÊÓÒ°·¶Î§
	INT					m_DialogRadius;			// NpcµÄ¶Ô»°·¶Î§
	INT					m_ActiveRadius;			// NpcµÄ»î¶¯·¶Î§
	INT					m_HitRecover;			// NpcµÄÊÜ»÷»Ø¸´ËÙ¶È
	BOOL					m_bClientOnly;			// 

	INT					m_nCurrentMeleeSkill;	// Npcµ±Ç°ÕýÖ´ÐÐµÄ¸ñ¶·¼¼ÄÜ
	INT					m_nCurrentMeleeTime;

	// AI²ÎÊý
	INT					m_AiMode;				// AIÄ£Ê½
	INT					m_AiParam[MAX_AI_PARAM];// ÓÃÓÚAIÄ£¿é¼ÆËãAI
	INT					m_AiAddLifeTime;

	INT					m_HeadImage;

	INT					m_FightMode;			// ¿Í»§¶Ë´¦Àí¶¯×÷ÓÃ¡£
	INT					m_OldFightMode;
	BOOL					m_bExchangeServer;

	BOOL					m_bActivateAutoMoveBarrier1; //TamLTM Them check auto move barrier
	BOOL					m_bActivateAutoMoveBarrier2; //TamLTM Them check auto move barrier
	BOOL					m_bActivateAutoMoveBarrier3; //TamLTM Them check auto move barrier
	BOOL					m_bActivateAutoMoveBarrier4; //TamLTM Them check auto move barrier

	//TamLTM Them check auto
	bool					isCheckNotBarrierPlayer;
	bool					isCheckAutoRunPlayer;

	//pos
	INT					m_nMovePosX;
	INT					m_nMovePosY;
	//enc code

	INT 				m_nRankInWorld;
	INT 				m_nRepute;
	INT 				m_nFuYuan;
	INT 				m_nPKValue;

	INT					m_nAllSkill;
	BOOL					m_nIsOver;
	BYTE					IsExeGoldScript; // Whether to execute the global death script
	BYTE					IsCreatBoss;
	INT						IsJinYan;	 // Activity experience
	INT						IsLuKey;	 // Activity Lucky
	INT						m_GoldLucky; // Global luck
	INT						IsJinQian;	 // activity money
	INT						m_IsVip; // 会员
	INT						m_IsXingYunXing;
	KNpcGold				m_cGold;

#ifdef _SERVER
	INT					m_AiSkillRadiusLoadFlag;// Õ½¶·npc¼¼ÄÜ·¶Î§ÊÇ·ñÒÑ¾­³õÊ¼»¯ Ö»ÐèÒªÔÚ¹¹ÔìµÄÊ±ºò³õÊ¼»¯Ò»´Î
	KNpcDeathCalcExp		m_cDeathCalcExp;		// Õ½¶·npcËÀÍöºóËÍ³ö¾­Ñé¸ø²»Í¬µÄplayer
	KItemDropRate* m_pDropRate;		  // Explosion rate
	typedef std::map<INT, INT> _IntDropInfo;
	_IntDropInfo intDropInfo;
#endif
	INT						m_nCurPKPunishState;	// PKËÀÍöÊ±µÄ³Í·£ÐÔÖÊ£¬ÓÃÓÚ¹úÕ½
	BOOL					m_bReviveNow;
#ifndef	_SERVER
	KNpcRes					m_DataRes;				// NpcµÄ¿Í»§¶Ë×ÊÔ´£¨Í¼Ïó¡¢ÉùÒô£©
	INT						m_SyncSignal;			// Í¬²½ÐÅºÅ
	KClientNpcID			m_sClientNpcID;			// ÓÃÓÚ±êÃ÷¿Í»§¶ËnpcÊÇÄÄ¸öregionµÄµÚ¼¸¸önpc
	DWORD					m_dwRegionID;			// ±¾npcËùÔÚregionµÄid
	char					m_szChatBuffer[MAX_SENTENCE_LENGTH];
	INT						m_nChatContentLen;
	INT						m_nChatNumLine;
	INT						m_nChatFontWidth;
	UINT					m_nCurChatTime;
	INT						m_nSleepFlag;
	INT						m_nHurtHeight;
	INT						m_nHurtDesX;
	INT						m_nHurtDesY;
	BOOL					m_bIsPosEdition;
	BYTE					m_nPacePercent;
	BOOL					m_bTongFlag;			// ÊÇ·ñÓÐÕÐÈËÍ¼±ê
	KSyncPos				m_sSyncPos; //TamLTM lag pos Npc
	std::vector<FindPathNode> m_PathFind;// find way by kinnox;
	FindPathNode		m_AutoMoveTemp;	// find way by kinnox;
#endif
private:
	INT						m_LoopFrames;			// Ñ­»·Ö¡Êý
	INT						m_nPlayerIdx;
	INT						m_DeathFrame;			// ËÀÍöÖ¡Êý
	INT						m_StandFrame;
	INT						m_HurtFrame;
	INT						m_AttackFrame;
	INT						m_CastFrame;
	INT						m_WalkFrame;
	INT						m_RunFrame;
	INT						m_StandFrame1;
	INT						m_ReviveFrame;			// ÖØÉúÖ¡Êý
	INT						m_SitFrame;
	INT						m_JumpFrame;
	INT						m_JumpFirstSpeed;
	NPC_COMMAND				m_Command;				// ÃüÁî½á¹¹
	BOOL					m_ProcessAI;			// ´¦ÀíAI±êÖ¾
	BOOL					m_ProcessState;			// ´¦Àí×´Ì¬±êÖ¾
	INT						m_XFactor;
	INT						m_YFactor;
	INT						m_JumpStep;
	INT						m_JumpDir;
	INT						m_SpecialSkillStep;		// ÌØÊâ¼¼ÄÜ²½Öè
	NPC_COMMAND				m_SpecialSkillCommand;	// ÌØÊâ¼¼ÄÜÐÐÎªÃüÁî
	KNpcFindPath			m_PathFinder;
	BOOL					m_bActivateFlag;

#ifndef	_SERVER
	INT						m_ResDir;
	// ÓÃÓÚÆÕÍ¨Õ½¶·npcÃ°Ñª´¦Àí
	BYTE					m_btCurBlood;
	INT						m_nBlood[BLOOD_COUNT];
	char					m_szBlood[BLOOD_COUNT][16];
#endif

private:
	BOOL				WaitForFrame();
	BOOL				IsReachFrame(INT nPercent);
	void				DoStand();
	void				OnStand();
	void				DoRevive();
	void				OnRevive();
	void				DoWait();
	void				OnWait();
	void				DoWalk();
	void				OnWalk();
	void				DoRun();
	void				OnRun();
	void				DoSkill(INT nX, INT nY);
	INT				DoOrdinSkill(KSkill* pSkill, INT nX, INT nY);
	void				OnSkill();
	void				DoJump();
	BOOL				OnJump();
	void				DoSit();
	void				OnSit();
	void				DoHurt(INT nHurtFrames = 0, INT nX = 0, INT nY = 0);
	void				OnHurt();

	// mode == 0 npc µ¼ÖÂ == 1 player µ¼ÖÂ£¬²»µô¶«Î÷ == 2 player µ¼ÖÂ£¬µô¶«Î÷
	// Óë DeathPunish µÄ²ÎÊý¶ÔÓ¦ ¾ßÌå²ÎÔÄ enumDEATH_MODE
	void				DoDeath(INT nMode = 0);

	void				OnDeath();
	void				DoDefense();
	void				OnDefense();
	void				DoIdle();
	void				OnIdle();

	//	ÓÐ¹Ø¸ñ¶·¼¼ÄÜµÄ------------------------------------------

	BOOL				DoBlurMove();
	void				OnBlurMove();

	BOOL				DoManyAttack();
	void				OnManyAttack();
	BOOL				DoBlurAttack();

	BOOL				DoJumpAttack();
	BOOL				OnJumpAttack();

	BOOL				DoRunAttack();
	void				OnRunAttack();
	BOOL				CastMeleeSkill(KSkill* pSkill);

	void				DoSpecial1();
	void				OnSpecial1();
	void				DoSpecial2();
	void				OnSpecial2();
	void				DoSpecial3();
	void				OnSpecial3();
	void				Goto(INT nMpsX, INT nMpsY);
	void				RunTo(INT nMpsX, INT nMpsY);
	void				JumpTo(INT nMpsX, INT nMpsY);
	void				ServerMove(INT nSpeed);
	void				ServerJump(INT nSpeed);
	BOOL				NewPath(INT nMpsX, INT nMpsY);
	BOOL				NewJump(INT nMpsX, INT nMpsY);
	BOOL				CheckHitTarget(INT nAR, INT nDf, INT nIngore = 0);
	void				FixPos();
#ifdef _SERVER
	void				PlayerDeadCreateMoneyObj(INT nMoneyNum);	// Íæ¼ÒËÀµÄÊ±ºòµô³öÀ´µÄÇ®Éú³ÉÒ»¸öobject
	void				UpdateNpcStateInfo();		//ÖØÐÂ¸üÐÂ½ÇÉ«µÄ×´Ì¬ÐÅÏ¢
#endif

public:
	friend class KNpcAttribModify;
	friend class KThiefSkill;
	KNpc();
	void				SetActiveFlag(BOOL bFlag) { m_bActivateFlag = bFlag; };
	BOOL				CheckTrap(INT nMapX, INT nMapY);
	void				Init();
	void				Remove();
	void				Activate();
	BOOL				IsPlayer();
	void				SetFightMode(BOOL bFightMode);
	void				TurnTo(INT nIdx);
	void				SendCommand(NPCCMD cmd, INT x = 0, INT y = 0, INT z = 0);
	void				ProcCommand(INT nAI);
	ISkill* GetActiveSkill();
	BOOL				ProcessState();
	void				ProcStatus();
	void				ModifyAttrib(INT nAttacker, void* pData);
	void				SetId(DWORD	dwID) { m_dwID = dwID; };
	BOOL				IsMatch(DWORD dwID) { return dwID == m_dwID; };	// ÊÇ·ñIDÓë¸ÃIndexÆ¥Åä
	BOOL				Cost(NPCATTRIB nType, INT nCost, BOOL bIsAudit = FALSE, BOOL bNotShowMessage = FALSE);				// ÏûºÄÄÚÁ¦ÌåÁ¦µÈ,Èç¹ûOnlyCheckCanCostÎªTRUE,±íÊ¾Ö»ÊÇ¼ì²éµ±Ç°µÄÄÚÁ¦µÈÊÇ·ñ¹»ÏûºÄ£¬²¢²»Êµ¼ÊµÄ¿Û
	void				Load(INT nNpcSettingIdx, INT nLevel, INT nSubWorld = -1, INT nBoss = 0); // Load from TabFile
	void				GetMpsPos(INT* pPosX, INT* pPosY);
	BOOL				SetActiveSkill(INT nSkillIdx);
	void				SetAuraSkill(INT nSkillID);
	void				SetSkillAppendAura(INT nAppendNo, INT nSkillID);
	void				SetCamp(INT nCamp);
	void				SwitchMaskFeature();

	//TamLTM Fix move toa do xy player.
	//void				CheckMoveBarrier(BOOL Obstacle_LT /*12h*/, BOOL Obstacle_RT /*6h*/, BOOL Obstacle_LB /*9h*/, BOOL Obstacle_RB /*3h*/);
	void				CheckMoveBarrier();
	void				CheckTimerMoveBarrier();
	void				DoAutoMoveBarrier(INT arrowMove);

	BOOL				GetCheckAutoMoveBarr(BOOL isCheck);
	void				MoveToBarrierPlayer(INT nX, INT nY, INT isCheckMoveCalculator = 0);
	void				MiniMapXY(INT nX, INT nY);
	//end code

	//TamLTM show name pk
	void				ShowPKNamePlayer(char* m_nNamePK); //TamLTM Show ten va thong bao cua nhan vat dang cuu sat voi nhau
	//end code
#ifdef _SERVER
	void				SetTempCurrentCamp(INT nCamp);
#endif
	void				SetCurrentCamp(INT nCamp);
	void				ChangeCurrentCamp(INT nCamp);
	void				RestoreCurrentCamp();
	void				SetRank(INT nRank);
	void				SetExpandRank(KExpandRank* ExpandRank);
	void				SetStateSkillEffect(INT nLauncher, INT nSkillID, INT nLevel, void* pData, INT nDataNum, INT nTime = -1, BOOL bOverLook = FALSE);	// Ö÷¶¯¸¨Öú¼¼ÄÜÓë±»¶¯¼¼ÄÜ
	void				SysnCastSkillEffect(int nLauncher, int nSkillID, int nParam1, int nParam2, int nLevel);
	void				ClearStateSkillEffect(bool bSkillCast = false);
	void				IgnoreState(BOOL bNegative);
	void				ReCalcStateEffect();
	void				ClearNormalState();
	BOOL				IsNpcStateExist(INT nId);
	BOOL				IsNpcSkillExist(INT nId);
	void				SetImmediatelySkillEffect(INT nLauncher, void* pData, INT nDataNum);
	void				AppendSkillEffect(INT nSkillID, BOOL bIsPhysical, BOOL bIsMelee, void* pSrcData, void* pDesData);
	INT				ModifyMissleLifeTime(INT nLifeTime);
	INT				ModifyMissleSpeed(INT nSpeed);
	BOOL				ModifyMissleCollsion(BOOL bCollsion);
	void				RestoreNpcBaseInfo(); //Set Current_Data ;
	void				RestoreState();
	void				ClearNpcState();
	BOOL				SetPlayerIdx(INT nIdx);
	void				DialogNpc(INT nIndex);
	void				Revive();
	void				AddBaseLifeMax(INT nLife);	// Ôö¼Ó»ù±¾×î´óÉúÃüµã
	void				AddCurLifeMax(INT nLife);
	void				AddBaseStaminaMax(INT nStamina);// Ôö¼Ó»ù±¾×î´óÌåÁ¦µã
	void				AddCurStaminaMax(INT nStamina);
	void				AddBaseManaMax(INT nMana);	// Ôö¼Ó»ù±¾×î´óÄÚÁ¦µã
	void				AddCurManaMax(INT nMana);
	void				SetBaseLifeMax(INT nLifeMax); // ÉèÖÃ»ù±¾×î´óÉúÃüµã
	void				SetBaseStaminaMax(INT nStamina);
	void				SetBaseManaMax(INT nMana);
	void				CalcCurLifeReplenish();		// ¼ÆËãµ±Ç°ÉúÃü»Ø¸´ËÙ¶È
	void				CalcCurLucky();		// ¼ÆËãµ±Ç°ÉúÃü»Ø¸´ËÙ¶È
	void				SetSeries(INT nSeries);// Éè¶¨´Ë npc µÄÎåÐÐÊôÐÔ£¨ÄÚÈÝ»¹Ã»Íê³É£©
	void				GetNpcCopyFromTemplate(INT nNpcTemplateId, INT nLevel);
	void				SetPhysicsDamage(INT nMinDamage, INT nMaxDamage);	// Éè¶¨ÎïÀí¹¥»÷µÄ×î´ó×îÐ¡Öµ
	void				SetEngergyDamage(INT nEngergyDamage);
	void				SetReviveFrame(INT nReviveFrame);
	void				SetBaseAttackRating(INT nAttackRating);					// Éè¶¨¹¥»÷ÃüÖÐÂÊ
	void				SetBaseDefence(INT nDefence);							// Éè¶¨·ÀÓùÁ¦
	INT					GetCurActiveWeaponSkill();
	void				LoadDataFromTemplate(void* nNpcTemp = NULL/*INT nNpcTemplateId*/);
	void				ResetNpcTypeName(INT nMark);

	inline bool			IsAlive() const { return (m_Doing != do_death && m_Doing != do_revive); }
	INT					GetMapX(void) const { return m_MapX; };
	INT					GetMapY(void) const { return m_MapY; };
	INT					GetMapZ(void) const { return m_MapZ; };
	INT					GetOffX(void) const { return m_OffX; };
	INT					GetOffY(void) const { return m_OffY; };
	inline INT			GetSubWorldIndex() { return m_SubWorldIndex; }
	void				SwitchRideHorse(BOOL bRideHorse);
#ifdef	_SERVER
	INT					UpdateDBStateList(BYTE*);
	BOOL				CanSwitchRideHorse();
	void				ExecuteRevive() { DoRevive(); };
	BOOL				SendSyncData(INT nClient, BOOL bBroadCast = FALSE);						// ÏòÒ»¸ö¿Í»§¶Ë·¢ÍêÕûÍ¬²½Êý¾Ý
	void				NormalSync();									// ¹ã²¥Ð¡Í¬²½
	void				SyncPos(INT nNetConnectIdx = -1, BOOL bDoStand = FALSE); // Force synchronized coordinates
	void				BroadCastState();//Broadcast the character's status information, halo, headband and other effects
	void				BroadCastRevive(INT nType);
	INT					GetPlayerIdx();
	BOOL				CalcDamage(INT nAttacker, INT nMissleSeries, INT nMin, INT nMax, DAMAGE_TYPE nType, BOOL bIsMelee, BOOL bReturn = FALSE, INT nSeries_DamageP = 0, INT nStole_Life = 0, INT nStole_Mana = 0, INT nStole_Stamina = 0, BOOL bIsDS = FALSE, BOOL bIsFS = FALSE);
	void				ReplySkill(INT nAttacker);
	void				RescueSkill();
	void				AttackSkill(INT nUnderAttacker);
	void				CastSkill();
	void				DeathSkill();
	BOOL				ReceiveDamage(INT nLauncher, INT nMissleSeries, BOOL bIsMelee, void* pData, BOOL bUseAR, INT nDoHurtP, INT nMissRate);
	void				Cast(INT nSkillId, INT nSkillLevel, INT nTarget);
	// mode == 0 npc µ¼ÖÂ == 1 player µ¼ÖÂ£¬²»µô¶«Î÷ == 2 player µ¼ÖÂ£¬µô¶«Î÷
	// Óë DoDeath µÄ²ÎÊý¶ÔÓ¦ ¾ßÌå²ÎÔÄ enumDEATH_MODE
	void				DeathPunish(INT nMode, INT nBelongPlayer);
	BOOL				IsDead() const { return m_Doing == do_death || m_Doing == do_revive; }

	void				RestoreLiveData();								// ÖØÉúºó»Ö¸´NpcµÄ»ù±¾Êý¾Ý
	BOOL				SetPos(INT nX, INT nY, BOOL bSync2Client = TRUE);
	INT					ChangeWorld(DWORD dwSubWorldID, INT nX, INT nY);	// ÇÐ»»ÊÀ½ç
	void				TobeExchangeServer(DWORD dwMapID, INT nX, INT nY);
	void				RestoreLife() { m_CurrentLife = m_CurrentLifeMax; };
	void				RestoreMana() { m_CurrentMana = m_CurrentManaMax; };
	void				RestoreStamina() { m_CurrentStamina = m_CurrentStaminaMax; };
	void				SendDataToNearRegion(void* pBuffer, DWORD dwSize);// ÏòÖÜÎ§¾ÅÆÁ¹ã²¥
	INT					DeathCalcPKValue(INT nKiller);					// ËÀÍöÊ±ºò¼ÆËãPKÖµ
	INT					FindAroundPlayer(const char* Name);// ²éÕÒÖÜÎ§9¸öRegionÖÐÊÇ·ñÓÐÖ¸¶¨µÄ player
	void				SetSex(INT nSex) { m_nSex = nSex; };
	INT					GetSex() { return m_nSex; };
#endif

	//TamLTM fix add ham fix xy map
#ifndef _SERVER
	BOOL					SetPos(INT nX, INT nY);
	void					HurtAutoMove();
	void					AutoFixXY();
	INT						m_nCheckAutoMoveBarrier;
	INT						m_nMoveToFlagMiniMapX;
	INT						m_nMoveToFlagMiniMapY;
#endif
	//end code

#ifndef _SERVER
	void					SetSleepMode(BOOL bSleep) { m_nSleepFlag = bSleep; m_DataRes.SetSleepState(bSleep); };
	void					SetNpcState(BYTE* pNpcState);
	void					RemoveRes();
	void					ProcNetCommand(NPCCMD cmd, INT x = 0, INT y = 0, INT z = 0);
	void					Paint();
	INT					PaintInfo(INT nHeightOffset, INT nFontSize = 12, DWORD	dwBorderColor = 0);
	void					PaintTop(INT nHeightOffset, INT nnHeightOffset, INT nFontSize = 12, DWORD	dwBorderColor = 0);
	INT					PaintChat(INT nHeightOffset);
	INT					SetChatInfo(const char* Name, const char* pMsgBuff, uint16_t nMsgLength);
	INT					PaintLife(INT nHeightOffset, bool bSelect);
	INT					PaintMantle(INT nHeightOff, INT nFontSize, INT nMpsX, INT nMpsY);
	INT					PaintViprank(INT nHeightOff, INT nFontSize, INT nMpsX, INT nMpsY);
	INT					PaintTranslife(INT nHeightOff, INT nFontSize, INT nMpsX, INT nMpsY);
	INT					PaintWorldRank(INT nHeightOff, INT nFontSize, INT nMpsX, INT nMpsY, INT nWorldRank);
	void					DrawBorder();
	INT					DrawMenuState(INT n);
	void					DrawBlood();	//»æÖÆÑªÌõºÍÃû×ÖÔÚ¹Ì¶¨Î»ÖÃ
	BOOL					IsCanInput() { return m_ProcessAI; };
	void					SetMenuState(INT nState, char* lpszSentence = NULL, INT nLength = 0);	// Éè¶¨Í·¶¥×´Ì¬
	INT					GetMenuState();				// »ñµÃÍ·¶¥×´Ì¬
	DWORD					SearchAroundID(DWORD dwID);	// ²éÕÒÖÜÎ§9¸öRegionÖÐÊÇ·ñÓÐÖ¸¶¨ ID µÄ npc
	void					SetSpecialSpr(char* lpszSprName);// Éè¶¨ÌØÊâµÄÖ»²¥·ÅÒ»±éµÄËæÉísprÎÄ¼þ
	void					SetInstantSpr(INT nNo);
	INT					GetNormalNpcStandDir(INT nFrame);
	KNpcRes* GetNpcRes() { return &m_DataRes; };
	void				GetNpcResFile(INT nNpcSettingIdx, char* pszResPath);
	void				KeyToImage(char*, INT, KUiImage*);

	INT GetNpcPate();
	INT GetNpcPatePeopleInfo();

	// Ã°Ñª´¦Àí
	void				AddBlood(INT nNo);
	INT					PaintBlood(INT nHeightOffset);	//»æÖÆÃ°Ñª
#endif
};
#ifndef TOOLVERSION
extern KNpc Npc[MAX_NPC];
#else
extern CORE_API KNpc Npc[MAX_NPC];
#endif

#endif

