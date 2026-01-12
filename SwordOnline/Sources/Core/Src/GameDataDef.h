/*****************************************************************************************
//	Íâ½ç·ÃÎÊCoreÓÃµ½Êý¾Ý½á¹¹µÈµÄ¶¨Òå
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-12
------------------------------------------------------------------------------------------
	Ò»Ð©¶¨Òå¿ÉÄÜ´¦ÓÚÔÚÓÎÏ·ÊÀ½ç¸÷Ä£¿éµÄÍ·ÎÄ¼þÖÐ£¬ÇëÔÚ´Ë´¦°üº¬ÄÇ¸öÍ·ÎÄ¼þ£¬²¢ÇëÄÇÑùµÄÍ·ÎÄ¼þ
²»Òª°üº¬Ò»Ð©ÓëÓÎÏ·ÊÀ½ç¶ÔÍâÎÞ¹ØµÄÄÚÈÝ¡£
	¿ª·¢¹ý³ÌÖÐÓÎÏ·ÊÀ½çµÄÍâ²¿¿Í»§ÔÚÎ´»ñµÃÓÎÏ·ÊÀ½ç½Ó¿ÚÍêÕû¶¨ÒåµÄÇé¿öÏÂ£¬»áÏÖÏÈÖ±½ÓÔÚ´ËÎÄ¼þ
¶¨ÒåËüÐèÒªµÄÊý¾Ý¶¨Òå£¬ÓÎÏ·ÊÀ½ç¸÷Ä£¿é¿É¸ù¾Ý×ÔÉíÐèÒªÓëÉè¼Æ°Ñ¶¨Òå×÷ÐÞ¸Ä»òÒÆ¶¯µ½Ä£¿éÄÚµÄ¶ÔÍâ
Í·ÎÄ¼þ£¬²¢ÔÚ´Ë½¨Á¢³äÒªµÄ°üº¬¡£
*****************************************************************************************/
#ifndef GAMEDATADEF_H
#define GAMEDATADEF_H

#include "CoreObjGenreDef.h"
#include "CoreUseNameDef.h"
#include <cstdint>

#define		ITEM_VERSION						1

#define		_CHAT_SCRIPT_OPEN

#ifdef	_SERVER
#define	MAX_NPC			48000
#define	MAX_PLAYER		1200
#define	MAX_ITEM		160000
#else
#define	MAX_NPC			256
#define	MAX_PLAYER		2
#define	MAX_ITEM		512
#endif

#define		MAX_TEAM_MEMBER						7
#define		MAX_QUESTION_LENGTH					2056
#define		MAX_SENTENCE_LENGTH					256
#define		MAX_MESSAGE_LENGTH					2056
#define		MAX_ANSWERNUM						20

#define 	NORMAL_FONTSIZE						12
#define		FILE_NAME_LENGTH					80
#define		OBJ_NAME_LENGTH						40
#define		PLAYER_PICKUP_CLIENT_DISTANCE		63
#define		defMAX_EXEC_OBJ_SCRIPT_DISTANCE		200
#define		defMAX_PLAYER_SEND_MOVE_FRAME		5
#define		PLAYER_PICKUP_SERVER_DISTANCE		40000

#define		MAX_PERCENT							100
#define		MAX_INT								0x7fffffff
#define		MONEY_FLOOR							10000

#define		ROLE_NO								2
#define		PLAYER_MALE_NPCTEMPLATEID			-1
#define		PLAYER_FEMALE_NPCTEMPLATEID			-2

#define		PLAYER_SHARE_EXP_DISTANCE			768

#define		MAX_DEATH_PUNISH_PK_VALUE			10

#define		RESOLUTION_WIDTH				800
#define		RESOLUTION_HEIGHT				600

#define		POISON_DAMAGE_TIME				60
#define		POISON_DAMAGE_INTERVAL			10
#define		COLD_DAMAGE_TIME				30	//60
#define		MAX_DEATLY_STRIKE_ENHANCEP		200
#define		MIN_FATALLY_STRIKE_ENHANCEP		30
#define		MAX_FATALLY_STRIKE_ENHANCEP		50

#define		MAX_LEVEL						200
#define		MAX_SERIES						5
#define		MAX_LEAD_LEVEL					100

#define		MAX_TONG_COUNT					50
#define		MAX_TONG_LEVEL					10
#define		MAX_TONG_NATIONALEMBLEM			6

#define		MAX_COMPOUND_ITEM				3

#define		OBJ_NAME_LENGHT					80 // TamLTM Kham nam xanh
#define		MAX_PART_BUILD					9 //TamLTM Kham nam xanh // Max so part item kham nam TamLTM

#define REGION_PIXEL_WIDTH	512
#define REGION_PIXEL_HEIGHT	1024
#define REGION_CELL_SIZE_X	32
#define REGION_CELL_SIZE_Y	32
#define REGION_CELL_WIDTH	(REGION_PIXEL_WIDTH/REGION_CELL_SIZE_X)
#define REGION_CELL_HEIGHT	(REGION_PIXEL_HEIGHT/REGION_CELL_SIZE_Y)

#define		GAME_FPS					18
#define		GAME_UPDATE_TIME			10
#define		GAME_SYNC_LOSS				100

#define		MAX_TASK					10000	//255
#define		MAX_TEMP_TASK				10000

#define		MAX_SCIRPTACTION_BUFFERNUM	2056

#define 	TRADEFOLKGAME_NEED_WIDTH	6
#define 	TRADEFOLKGAME_NEED_HEIGHT	5

#define 	MIN_ITEM_LEVEL				0
#define 	MAX_ITEM_LEVEL				10
#define 	MAX_ITEM_GENERATORLEVEL		10
#define 	MAX_ITEM_LUCK				10
#define 	MAX_ITEM_MAGICATTRIB		8
#define 	MAX_ITEM_MAGICLEVEL			MAX_ITEM_MAGICATTRIB * 2
#define 	MAX_ITEM_NORMAL_MAGICATTRIB	6
#define		MAX_ITEM_S3DBINTERFACE1		15
#define		MAX_ITEM_S3DBINTERFACE2		20

#define		FACTIONS_PRR_SERIES				2
#define		HUASHANID						10
#define		MAX_FACTION						FACTIONS_PRR_SERIES * series_num + 3

#define		MIN_PASSWORD_LENGTH		0
#define		MAX_PASSWORD_LENGTH		16

#define		MAX_NPC_RECORDER_STATE 36 // Maximum number of latest status synchronized to NPC; 20

enum LOCK_STATE
{
	LOCK_STATE_CHARACTER = -3,
	LOCK_STATE_FOREVER = -2,
	LOCK_STATE_LOCK,
	LOCK_STATE_NORMAL,
	LOCK_STATE_UNLOCK,
};


#define		MAX_STATTASK					10
#define		MAX_TRANSLIFE_VALUE				11
#define		MAX_VIPRANK_VALUE				20

enum TASKVALUE_STATTASK
{
	TASKVALUE_STATTASK_BEGIN = 210,
	TASKVALUE_STATTASK_REPUTE = 210,
	TASKVALUE_STATTASK_FUYUAN,
	TASKVALUE_STATTASK_ACCUM,
	TASKVALUE_STATTASK_ACCUMSTAT1,
	TASKVALUE_STATTASK_ACCUMSTAT2,
	TASKVALUE_STATTASK_HONOR,
	TASKVALUE_STATTASK_HONORSTAT,
	TASKVALUE_STATTASK_RESPECT,
	TASKVALUE_STATTASK_TIMESTAT,
	TASKVALUE_STATTASK_VIPRANK,
	TASKVALUE_STATTASK_NUM
};

enum MAP_TYPE
{
	MAPID_UNKNOWN,
	MAPID_CITY,
	MAPID_CAPITAL,
	MAPID_CAVE,
	MAPID_BATTLEFIELD,
	MAPID_FIELD,
	MAPID_OTHERS,
	MAPID_COUNTRY,
	MAPID_TONG,
	MAPID_NUM,
};

enum ITEM_PART
{
	itempart_head = 0,
	itempart_body,
	itempart_belt,
	itempart_weapon,
	itempart_foot,
	itempart_cuff,
	itempart_amulet,
	itempart_ring1,
	itempart_ring2,
	itempart_pendant,
	itempart_horse,
	itempart_mask,
	itempart_mantle,
	itempart_signet,
	itempart_shipin,
	itempart_hoods,
	itempart_cloak,
	itempart_num,
};

//TamLTM kham nam code
enum BUILD_PART
{
	itempart_build1 = 0,
	itempart_build2,
	itempart_build3,
	itempart_build4,
	itempart_build5,
	itempart_build6,
	itempart_build7,
	itempart_build8,
	itempart_build9,
	buildpart_num,
};
//End code

enum COMPOUND_ACTION
{
	CPA_FUSION,
	CPA_CRYOLITE,
	CPA_PROPMINE,
	CPA_NUM,
};

enum ENCHASE_ACTION
{
	ECA_DISTILL,
	ECA_FORGE,
	ECA_ENCHASE,
	ECA_ATLAS,
};

enum COMPOUND_PART
{
	compound_box1 = 0,
	compound_box2,
	compound_box3,
	compound_num,
};

//TamLTM kham nam xanh
enum ITEMKIND
{
	normal_item = 0,
	purple_item,
	gold_item,
	green_item,
	kind_number,
};
//End code

enum ITEMGENRE
{
	item_equip = 0,
	item_medicine,
	item_event,
	item_materials,
	item_task,
	item_townportal,
	item_magicscript,
	item_mine,	//TamLTM Kham nam xanh
	item_number,
};

enum EQUIPNATURE
{
	NATURE_NORMAL = 0,
	NATURE_VIOLET,
	NATURE_GOLD,
	NATURE_PLATINA,
	NATURE_NUM,
};

enum EQUIPLEVEL
{
	equip_normal = 0,
	equip_magic,
	equip_damage,
	equip_violet,
	equip_gold,
	equip_platina,
	equip_number,
};

enum EQUIPDETAILTYPE
{
	equip_meleeweapon = 0,
	equip_rangeweapon,
	equip_armor,
	equip_ring,
	equip_amulet,
	equip_boots,
	equip_belt,
	equip_helm,
	equip_cuff,
	equip_pendant,
	equip_horse,
	equip_mask,
	equip_mantle,
	equip_signet,
	equip_shipin,
	equip_hoods,
	equip_cloak,
	equip_detailnum,
};

enum MEDICINEDETAILTYPE
{
	medicine_blood = 0,
	medicine_mana,
	medicine_both,
	medicine_stamina,
	medicine_antipoison,
	medicine_detailnum,
};

//TamLTM code kham nam xanh
enum SPECIALITEM_TASK
{
	SP_TUTHUYTINH = 239,
	SP_LUCTHUYTINH = 240,
	SP_LAMTHUYTINH = 238,
};
//End code

enum TOWNPORTALDETAILTYPE
{
	townportal_normal,
	townportal_infinite,
	townportal_special,
};

typedef struct
{
	BOOL	bIsSkill;
	INT		nIdx;
	INT		nPlace;
	INT		nX;
	INT		nY;

	void	Release() {
		bIsSkill = FALSE;
		nIdx = 0;
		nPlace = 0;
		nX = 0;
		nY = 0;
	};
} PlayerItem;

enum INVENTORY_ROOM
{
	room_equipment = 0,
	room_repository,
	room_repository1,
	room_repository2,
	room_repository3,
	room_repository4,
	room_repository5,
	room_equipmentex,
	room_trade,
	room_tradeback,
	room_trade1,
	room_immediacy,
	room_give,
	room_builditem,		// TamLTM Kham nam
	room_compound,
	room_num,
};

enum ITEM_POSITION
{
	pos_hand = 1,
	pos_equip,
	pos_equiproom,
	pos_equiproomex,
	pos_repositoryroom,
	pos_repositoryroom1,
	pos_repositoryroom2,
	pos_repositoryroom3,
	pos_repositoryroom4,
	pos_repositoryroom5,
	pos_traderoom,
	pos_trade1,
	pos_immediacy,
	pos_give,
	pos_compound,
	pos_builditem, //TamLTM Kham nam xanh
	pos_compoundroom,
	pos_num,
};

enum EXPAND_BOX
{
	REPOSITORY_ONE,
	REPOSITORY_TWO,
	REPOSITORY_THREE,
	REPOSITORY_FOUR,
	REPOSITORY_FIVE,
	REPOSITORY_NUM,
};

enum GAME_STR_NOTE
{
	STR_NONE = 0,
	STR_ACTION_SCRIPT, // 脚本
	STR_NPCTALK_MSG,
	STR_NPCTALK_NAME,
	STR_WARTONG_NAME,	// 占领帮会
	STR_WARMASTER_NAME, // 占领帮主
	STR_GUISHU_NAME,
	STR_CHEGNHAO_NAME,
	STR_SHOP_NAME,		   // 摆台的名字
	STR_PICTATH_CLIENT,	   // 头像路径
	STR_SCRIPTTATH_CLIENT, // 客户端脚本路径
	STR_MAP_CURTIME,	   // 聊天的内容
	STR_MAP_NEXTTIME,
	STR_MAP_NAME,
	STR_MAP_TGONGNAME,
	STR_MAP_TSHOUNAME,
	STR_MAP_MASTER,
	STR_MAP_ZTONGNAME,
	STR_ROLE_CHANGENAME, // 记录改名的数据 不存档
	STR_CHECK_NAME_RET,	 // 记录查询结果
	STR_ITEM_DESCINFO,	 // 物品的说明文字
	STR_ITEM_WONNAME,	 // 归属名字
	STR_ITEM_NAME,
	STR_MAP_TEMP_MASTER, // 临时的城主
	// m_WarTongNamea  m_WarMaster	 m_GuishuName PicPath	  m_ScriptPicPath  m_SubWorldName
};

#define		MAX_HAND_ITEM				1
#define		EQUIPMENT_ROOM_WIDTH		6
#define		EQUIPMENT_ROOM_HEIGHT		10
#define		MAX_EQUIPMENT_ITEM			(EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT)
#define		REPOSITORY_ROOM_WIDTH		6
#define		REPOSITORY_ROOM_HEIGHT		10
#define		MAX_REPOSITORY_ITEM			((REPOSITORY_ROOM_WIDTH * REPOSITORY_ROOM_HEIGHT) * (1 + REPOSITORY_NUM))
#define		TRADE_ROOM_WIDTH			10
#define		TRADE_ROOM_HEIGHT			4
#define		MAX_TRADE_ITEM				(TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT)
#define		MAX_TRADE1_ITEM				MAX_TRADE_ITEM
#define		GIVE_ROOM_WIDTH				6
#define		GIVE_ROOM_HEIGHT			4
#define		MAX_GIVE_ITEM				(GIVE_ROOM_WIDTH * GIVE_ROOM_HEIGHT)
#define		MOSAICENCRUSTED_ROOM_WIDTH	2
#define		MOSAICENCRUSTED_ROOM_HEIGHT	4
#define		MAX_MOSAICENCRUSTED_ITEM	(MOSAICENCRUSTED_ROOM_WIDTH * MOSAICENCRUSTED_ROOM_HEIGHT)
#define		IMMEDIACY_ROOM_WIDTH		9
#define		IMMEDIACY_ROOM_HEIGHT		1
#define		MAX_IMMEDIACY_ITEM			(IMMEDIACY_ROOM_WIDTH * IMMEDIACY_ROOM_HEIGHT)
#define		MAX_PLAYER_ITEM_RESERVED	32
#define		MAX_PLAYER_ITEM				(MAX_EQUIPMENT_ITEM + MAX_REPOSITORY_ITEM + MAX_TRADE_ITEM + MAX_TRADE1_ITEM + MAX_IMMEDIACY_ITEM + itempart_num + MAX_HAND_ITEM + MAX_PLAYER_ITEM_RESERVED)

#define		REMOTE_REVIVE_TYPE		0
#define		LOCAL_REVIVE_TYPE		1

#define		MAX_MELEE_WEAPON		8
#define		HAND_PARTICULAR			6
#define		MAX_RANGE_WEAPON		3
#define		MAX_WEAPON				MAX_MELEE_WEAPON + MAX_RANGE_WEAPON

#define		MAX_NPC_TYPE			300

#define		MAX_NPC_DIR				64
#define		MAX_NPCSKILL			80
#define		MAX_NPCPARAM			4
#define		MAX_NPC_HEIGHT			128

#define		MAX_SKILL_STATE			18

#define		MAX_RESIST				95
#define		MAX_BOSS_RESIST			195
#define		RESIST_PLUS_SCALE		10

#define		MIN_HIT_PERCENT			5
#define		MAX_HIT_PERCENT			95

#define		LEVEL_EXPLOSIVE			1
#define		LIFE_EXPLOSIVE			25

#define		PLAYER_MOVE_DO_NOT_MANAGE_DISTANCE	5

#define	NORMAL_NPC_PART_NO		5

#ifndef _SERVER
#define		C_REGION_X(x)	(LOWORD(SubWorld[0].m_Region[ (x) ].m_RegionID))
#define		C_REGION_Y(y)	(HIWORD(SubWorld[0].m_Region[ (y) ].m_RegionID))
#endif


enum PLAYER_INSTANT_STATE
{
	enumINSTANT_STATE_LEVELUP = 0,
	enumINSTANT_STATE_REVIVE,
	enumINSTANT_STATE_CREATE_TEAM,
	enumINSTANT_STATE_LOGIN,
	enumINSTANT_STATE_NUM,
};

enum OBJ_ATTRIBYTE_TYPE
{
	series_metal,			//	½ðÏµ
	series_wood,			//	Ä¾Ïµ
	series_water,			//	Ë®Ïµ
	series_fire,			//	»ðÏµ
	series_earth,			//	ÍÁÏµ
	series_nil,
	series_num = series_nil,
	series_minus,
};

enum OBJ_GENDER
{
	OBJ_G_MALE = 0,	//ÐÛÐÔ£¬ÄÐµÄ
	OBJ_G_FEMALE,		//´ÆµÄ£¬Å®µÄ
};

enum NPCCAMP
{
	camp_begin,
	camp_justice,
	camp_evil,
	camp_balance,
	camp_free,
	camp_animal,
	camp_event,
	camp_audience,
	camp_tongwar,
	camp_num,
};

enum NPCBOSS
{
	npc_normal,
	npc_blue,				// ÐÂÊÖÕóÓª£¨¼ÓÈëÃÅÅÉÇ°µÄÍæ¼Ò£©
	npc_gold,			// ÕýÅÉÕóÓª
	npcboss_num,				// ÕóÓªÊý
};

enum ITEM_IN_ENVIRO_PROP
{
	IIEP_NORMAL = 0,	//Ò»°ã/Õý³£/¿ÉÓÃ
	IIEP_NOT_USEABLE,	//²»¿ÉÓÃ/²»¿É×°Åä
	IIEP_SPECIAL,		//ÌØ¶¨µÄ²»Í¬Çé¿ö
	IIEP_SHOP,
};

#define	GOD_MAX_OBJ_TITLE_LEN	2048	//128ÁÙÊ±¸ÄÎª1024ÎªÁË¼æÈÝ¾É´úÂë to be modified
#define	GOD_MAX_OBJ_PROP_LEN	1024
#define	GOD_MAX_OBJ_DESC_LEN	2048

//==================================
//	ÓÎÏ·¶ÔÏóµÄÃèÊö
//==================================
struct KGameObjDesc
{
	char	szTitle[GOD_MAX_OBJ_TITLE_LEN];	//±êÌâ£¬Ãû³Æ
	char	szProp[GOD_MAX_OBJ_PROP_LEN];	//ÊôÐÔ£¬Ã¿ÐÐ¿ÉÒÔtab»®·ÖÎª¿¿×óÓë¿¿ÓÒ¶ÔÆëÁ½²¿·Ö
	char	szDesc[GOD_MAX_OBJ_DESC_LEN];	//ÃèÊö
};

//==================================
//	ÎÊÌâÓë¿ÉÑ¡´ð°¸
//==================================
struct KUiAnswer
{
	char	AnswerText[MAX_SENTENCE_LENGTH];	//¿ÉÑ¡´ð°¸ÎÄ×Ö£¨¿ÉÒÔ°üº¬¿ØÖÆ·û£©
	INT		AnswerLen;			//¿ÉÑ¡´ð°¸´æ´¢³¤¶È£¨°üÀ¨¿ØÖÆ·û£¬²»°üº¬½áÊø·û£©
};

struct KUiQuestionAndAnswer
{
	INT			LeftTime;
	char		Question[MAX_QUESTION_LENGTH];	//ÎÊÌâÎÄ×Ö£¨¿ÉÒÔ°üº¬¿ØÖÆ·û£©
	INT			QuestionLen;	//ÎÊÌâÎÄ×Ö´æ´¢³¤¶È£¨°üÀ¨¿ØÖÆ·û£¬²»°üº¬½áÊø·û£©
	INT			AnswerCount;	//¿ÉÑ¡´ð°¸µÄÊýÄ¿
	KUiAnswer	Answer[1];		//ºòÑ¡´ð°¸
};

struct KUiImage
{
	char		Name[MAX_PATH];
	INT			Frame;
};

//==================================
//	¼òÂÔ±íÊ¾ÓÎÏ·¶ÔÏóµÄ½á¹¹
//==================================
struct KUiGameObject
{
	UINT uGenre;	//¶ÔÏóÀàÊô
	UINT uId;		//¶ÔÏóid
	UINT uNature;
	//	INT			 nData;		//Óë¶ÔÏóÊµÀýÏà¹ØµÄÄ³Êý¾Ý
};

//==================================
//	ÒÔ×ø±ê±íÊ¾µÄÒ»¸öÇøÓò·¶Î§
//==================================
struct KUiRegion
{
	INT		h;		//×óÉÏ½ÇÆðµãºá×ø±ê
	INT		v;		//×óÉÏ½ÇÆðµã×Ý×ø±ê
	INT		Width;	//ÇøÓòºá¿í
	INT		Height;	//ÇøÓò×Ý¿í
};

//==================================
//	¿ÉÒÔÓÎÏ·¶ÔÏóÈÝÄÉµÄµØ·½
//==================================
enum UIOBJECT_CONTAINER
{
	UOC_IN_HAND = 1,		//ÊÖÖÐÄÃ×Å
	UOC_GAMESPACE,			//ÓÎÏ·´°¿Ú
	UOC_IMMEDIA_ITEM,		//¿ì½ÝÎïÆ·
	UOC_IMMEDIA_SKILL,		//¿ì½ÝÎä¹¦0->ÓÒ¼üÎä¹¦£¬1,2...-> F1,F2...¿ì½ÝÎä¹¦
	UOC_ITEM_TAKE_WITH,		//ËæÉíÐ¯´ø
	UOC_ITEM_TAKE_WITH_EX,
	UOC_TO_BE_TRADE,		//Òª±»ÂòÂô£¬ÂòÂôÃæ°åÉÏ
	UOC_OTHER_TO_BE_TRADE,	//ÂòÂôÃæ°åÉÏ£¬±ðÈËÒªÂô¸ø×Ô¼ºµÄ£¬
	UOC_EQUIPTMENT,			//ÉíÉÏ×°±¸
	UOC_NPC_SHOP,			//npcÂòÂô³¡Ëù
	UOC_STORE_BOX,
	UOC_STORE_BOX1,//´¢ÎïÏä
	UOC_STORE_BOX2,
	UOC_STORE_BOX3,
	UOC_STORE_BOX4,
	UOC_STORE_BOX5,
	UOC_SKILL_LIST,			//ÁÐ³öÈ«²¿ÓµÓÐ¼¼ÄÜµÄ´°¿Ú£¬¼¼ÄÜ´°¿Ú
	UOC_SKILL_TREE,
	UOC_ITEM_GIVE,
	UOC_BUILD_ITEM, //TamLTM kham nam xanh
	UOC_COMPOUND,
	UOC_COMPOUND_BOX,
	//×ó¡¢ÓÒ¿ÉÓÃ¼¼ÄÜÊ÷
};


enum MONEYUNIT
{
	moneyunit_money = 0,	// Í·
	moneyunit_coin,
	moneyunit_fuyuan,		// Ñü´ø
	moneyunit_repute,	// ÎäÆ÷
	moneyunit_accum,
	moneyunit_honor,
	moneyunit_respect,
	moneyunit_extpoint,		// ÉíÌå
	moneyunit_num,
};
//==================================
// iCoreShell::GetGameDataº¯Êýµ÷ÓÃ,uDataIdÈ¡ÖµÎªGDI_TRADE_DATAÊ±£¬
// uParamµÄÐí¿ÉÈ¡ÖµÁÐ±í
// ×¢ÊÍÖÐµÄReturn:ÐÐ±íÊ¾Ïà¹ØµÄGetGameDataµ÷ÓÃµÄ·µ»ØÖµµÄº¬Òå
//==================================
enum UI_TRADE_OPER_DATA
{
	UTOD_IS_WILLING,		//ÊÇ·ñ½»Ò×ÒâÏò(½ÐÂôÖÐ)
	//Return: ·µ»Ø×Ô¼ºÊÇ·ñ´¦ÓÚ½ÐÂôÖÐµÄ²¼¶ûÖµ
	UTOD_IS_LOCKED,			//×Ô¼ºÊÇ·ñ´¦ÓÚÒÑËø¶¨×´Ì¬
	//Return: ·µ»Ø×Ô¼ºÊÇ·ñ´¦ÓÚÒÑËø¶¨×´Ì¬µÄ²¼¶ûÖµ
	UTOD_IS_TRADING,		//ÊÇ·ñ¿ÉÒÔÕýÔÚµÈ´ý½»Ò×²Ù×÷£¨½»Ò×ÊÇ·ñÒÑÈ·¶¨£©
	//Return: ·µ»ØÊÇ·ñÕýÔÚµÈ´ý½»Ò×²Ù×÷£¨½»Ò×ÊÇ·ñÒÑÈ·¶¨£©
	UTOD_IS_OTHER_LOCKED,	//¶Ô·½ÊÇ·ñÒÑ¾­´¦ÓÚËø¶¨×´Ì¬
	//Return: ·µ»Ø¶Ô·½ÊÇ·ñÒÑ¾­´¦ÓÚËø¶¨×´Ì¬µÄ²¼¶ûÖµ
	UTOD_IS_OTHER_TRADING,
};

//==================================
//	ÂòÂôÎïÆ·
//==================================
struct KUiItemBuySelInfo
{
	INT				nItemNature;
	char			szItemName[64];	//ÎïÆ·Ãû³Æ
	INT				nOldPrice;
	INT				nCurPrice;			//ÂòÂô¼ÛÇ®£¬ÕýÖµÎªÂô¼Û¸ñ£¬¸ºÖµ±íÊ¾ÂòÈëµÄ¼Û¸ñÎª(-nPrice)
	INT				nMoneyUnit;
	bool			bNewArrival;
};

//==================================
//	±íÊ¾Ä³¸öÓÎÏ·¶ÔÏóÔÚ×ø±êÇøÓò·¶Î§µÄÐÅÏ¢½á¹¹
//==================================
struct KUiObjAtRegion
{
	KUiGameObject	Obj;
	KUiRegion		Region;
};

struct KUiObjAtContRegion : public KUiObjAtRegion
{
	union
	{
		UIOBJECT_CONTAINER	eContainer;
		INT					nContainer;
	};
};

struct KUiMsgParam
{
	unsigned char	eGenre;	//È¡Öµ·¶Î§ÎªÃ¶¾ÙÀàÐÍMSG_GENRE_LIST,¼ûMsgGenreDef.hÎÄ¼þ
	unsigned char	cChatPrefixLen;
	uint16_t	nMsgLength;
	char			szName[32];
#define	CHAT_MSG_PREFIX_MAX_LEN	16
	unsigned char	cChatPrefix[CHAT_MSG_PREFIX_MAX_LEN];
};

struct KUiInformationParam
{
	char	sInformation[MAX_MESSAGE_LENGTH];	//ÏûÏ¢ÎÄ×ÖÄÚÈÝ
	int16_t	nInforLen;			//ÏûÏ¢ÎÄ×ÖÄÚÈÝµÄ´æ´¢³¤¶È
	BOOL	bNeedConfirmNotify;
};

enum PLAYER_ACTION_LIST
{
	PA_NONE = 0,	//ÎÞ¶¯×÷
	PA_RUN = 0x01,	//ÅÜ
	PA_SIT = 0x02,	//´ò×ø
	PA_RIDE = 0x04,	//Æï£¨Âí£©
};
//==================================
//	ÏµÍ³ÏûÏ¢·ÖÀà
//==================================
enum SYS_MESSAGE_TYPE
{
	SMT_NORMAL = 0,	//²»²Î¼Ó·ÖÀàµÄÏûÏ¢
	SMT_SYSTEM,		//ÏµÍ³£¬Á¬½ÓÏà¹Ø
	SMT_PLAYER,		//Íæ¼ÒÏà¹Ø
	SMT_TEAM,		//×é¶ÓÏà¹Ø
	SMT_FRIEND,		//ÁÄÌìºÃÓÑÏà¹Ø
	SMT_MISSION,	//ÈÎÎñÏà¹Ø
	SMT_CLIQUE,		//°ïÅÉÏà¹Ø
	SMT_TRADE,		//°ïÅÉÏà¹Ø
	SMT_WAR,		//°ïÅÉÏà¹Ø
};

//==================================
//	ÏµÍ³ÏûÏ¢ÏìÓ¦·½Ê½
//==================================
enum SYS_MESSAGE_CONFIRM_TYPE
{
	SMCT_NONE,				//ÔÚ¶Ô»°ÏûÏ¢´°¿ÚÖ±½ÓÂÓ¹ý£¬²»ÐèÒªÏìÓ¦¡£
	SMCT_CLICK,				//µã»÷Í¼±êºóÁ¢¼´É¾³ý¡£
	SMCT_MSG_BOX,			//µã»÷Í¼±êºóµ¯³öÏûÏ¢¿ò¡£
	SMCT_UI_RENASCENCE,		//Ñ¡ÔñÖØÉú
	SMCT_UI_ATTRIBUTE,		//´ò¿ªÊôÐÔÒ³Ãæ
	SMCT_UI_SKILLS,			//´ò¿ª¼¼ÄÜÒ³Ãæ
	SMCT_UI_ATTRIBUTE_SKILLS,//´ò¿ªÊôÐÔÒ³Ãæ¼¼ÄÜÒ³Ãæ
	SMCT_UI_TEAM_INVITE,	//´ðÓ¦»ò¾Ü¾ø¼ÓÈë¶ÓÎéµÄÑûÇë,
	//						pParamBuf Ö¸ÏòÒ»¸öKUiPlayerItem½á¹¹µÄÊý¾Ý£¬±íÊ¾ÑûÇéÈË(¶Ó³¤)
	SMCT_UI_TEAM_APPLY,		//´ðÓ¦»ò¾Ü¾ø¼ÓÈë¶ÓÎéµÄÉêÇë,
	//						pParamBuf Ö¸ÏòÒ»¸öKUiPlayerItem½á¹¹µÄÊý¾Ý£¬±íÊ¾ÉêÇëÈË
	SMCT_UI_TEAM,			//´ò¿ª¶ÓÎé¹ÜÀíÃæ°å
	SMCT_UI_INTERVIEW,		//´ò¿ªÁÄÌì¶Ô»°½çÃæ,
	//						pParamBuf Ö¸ÏòÒ»¸öKUiPlayerItem½á¹¹µÄÊý¾Ý£¬±íÊ¾·¢À´ÏûÏ¢µÄºÃÓÑ
	SMCT_UI_FRIEND_INVITE,	//Åú×¼»ò¾Ü¾ø±ðÈË¼Ó×Ô¼ºÎªºÃÓÑ
	//						pParamBuf Ö¸ÏòÒ»¸öKUiPlayerItem½á¹¹µÄÊý¾Ý£¬±íÊ¾·¢³öºÃÓÑÑûÇëµÄÈË
	SMCT_UI_TRADE,			//´ðÓ¦»ò¾Ü¾ø½»Ò×µÄÇëÇó,
	SMCT_UI_TRADE_FOLKGAME,
	SMCT_UI_SPAR,
	//						pParamBuf Ö¸ÏòÒ»¸öKUiPlayerItem½á¹¹µÄÊý¾Ý£¬±íÊ¾·¢³ö½»Ò×ÑûÇëµÄÈË
	SMCT_DISCONNECT,		//¶ÏÏß
	SMCT_UI_TONG_JOIN,//´ðÓ¦»ò¾Ü¾ø¼ÓÈë°ï»áµÄÉêÇë
	SMCT_UI_TONG_JOIN_APPLY,//´ðÓ¦»ò¾Ü¾ø¼ÓÈë°ï»áµÄÉêÇë
};

//==================================
//	ÏµÍ³ÏûÏ¢
//==================================
struct KSystemMessage
{
	char			szMessage[256];	//ÏûÏ¢ÎÄ±¾
	UINT	uReservedForUi;	//½çÃæÊ¹ÓÃµÄÊý¾ÝÓò,coreÀïÌî0¼´¿É
	unsigned char	eType;			//ÏûÏ¢·ÖÀàÈ¡ÖµÀ´×ÔÃ¶¾ÙÀàÐÍ SYS_MESSAGE_TYPE
	unsigned char	byConfirmType;	//ÏìÓ¦ÀàÐÍ
	unsigned char	byPriority;		//ÓÅÏÈ¼¶,ÊýÖµÔ½´ó£¬±íÊ¾ÓÅÏÈ¼¶Ô½¸ß
	unsigned char	byParamSize;	//°éËæGDCNI_SYSTEM_MESSAGEÏûÏ¢µÄpParamBufËùÖ¸²ÎÊý»º³åÇø¿Õ¼äµÄ´óÐ¡¡£
};

//==================================
//	ÁÄÌìÆµµÀµÄÃèÊö
//==================================
struct KUiChatChannel
{
	INT			 nChannelNo;
	UINT uChannelId;
	union
	{
		INT		 nChannelIndex;
		INT		 nIsSubscibed;	//ÊÇ·ñ±»¶©ÔÄ
	};
	char		 cTitle[32];
};

//==================================
//	ÁÄÌìºÃÓÑµÄÒ»¸ö·Ö×éµÄÐÅÏ¢
//==================================
struct KUiChatGroupInfo
{
	char	szTitle[32];	//·Ö×éµÄÃû³Æ
	INT		nNumFriend;		//×éÄÚºÃÓÑµÄÊýÄ¿
};

//==================================
//	ºÃÓÑ·¢À´µÄÁÄÌì»°Óï
//==================================
struct KUiChatMessage
{
	UINT uColor;
	int16_t	nContentLen;
	char	szContent[256];
};

//==================================
//	Ö÷½ÇµÄÒ»Ð©²»Ò×±äµÄÊý¾Ý
//==================================
struct KUiPlayerBaseInfo
{
	char	Name[32];	//Ãû×Ö
	char	szTongName[32];
	char	szMateName[32];
	char	Title[128];	//³ÆºÅ
	INT		nCurFaction;// µ±Ç°¼ÓÈëÃÅÅÉ id £¬Èç¹ûÎª -1 £¬µ±Ç°Ã»ÓÐÔÚÃÅÅÉÖÐ
	INT		nMissionGroup;// µ±Ç°¼ÓÈëÃÅÅÉ id £¬Èç¹ûÎª -1 £¬µ±Ç°Ã»ÓÐÔÚÃÅÅÉÖÐ
	INT		nRoomId;
	INT		nRankInWorld;//½­ºþÅÅÃûÖµ,ÖµÎª0±íÊ¾Î´ÉÏÅÅÃû°å
	INT		nRankInSect;
	INT		nPKValue;
	INT		nRepute;
	INT		nFuYuan;
	INT		nTranslife;

	char	StatusDesc[16];		//×´Ì¬ÃèÊö
	UINT nCurTong;// µ±Ç°¼ÓÈë°ïÅÉname id £¬Èç¹ûÎª 0 £¬µ±Ç°Ã»ÓÐÔÚ°ïÅÉÖÐ
};

//==================================
//	Ö÷½ÇµÄÒ»Ð©Ò×±äµÄÊý¾Ý
//==================================
struct KUiPlayerRuntimeInfo
{
	LONG		nLifeFull;			//ÉúÃüÂúÖµ
	LONG		nLife;				//ÉúÃü
	INT		nManaFull;			//ÄÚÁ¦ÂúÖµ
	INT		nMana;				//ÄÚÁ¦
	INT		nStaminaFull;		//ÌåÁ¦ÂúÖµ
	INT		nStamina;			//ÌåÁ¦
	INT		nExperienceFull;	//¾­ÑéÂúÖµ
	INT		nExperience;		//µ±Ç°¾­ÑéÖµ
	INT		nCurLevelExperience;//µ±Ç°¼¶±ðÉý¼¶ÐèÒªµÄ¾­ÑéÖµ

	unsigned char	byAction;	//ÕýÔÚ½øÐÐµÄÐÐÎª¶¯×÷£¬ÎªÃ¶¾ÙPLAYER_ACTION_LISTÈ¡ÖµµÄ×éºÏ
};

//==================================
//	Ö÷½ÇµÄÒ»Ð©ÊôÐÔÊý¾ÝË÷Òý
//==================================
enum UI_PLAYER_ATTRIBUTE
{
	UIPA_STRENGTH = 0,			//Á¦Á¿
	UIPA_DEXTERITY,				//Ãô½Ý
	UIPA_VITALITY,				//»îÁ¦
	UIPA_ENERGY,				//¾«Á¦
};

//==================================
//	Ö÷½ÇµÄÒ»Ð©Ò×±äµÄÊôÐÔÊý¾Ý
//==================================
struct KUiPlayerAttribute
{
	INT		nMoney;				//ÒøÁ½
	INT		nLevel;				//µÈ¼¶
	char	StatusDesc[16];		//×´Ì¬ÃèÊö

	LONG		nBARemainPoint;		//»ù±¾ÊôÐÔÊ£ÓàµãÊý
	INT		nStrength;			//Á¦Á¿
	INT		nDexterity;			//Ãô½Ý
	INT		nVitality;			//»îÁ¦
	INT		nEnergy;			//¾«Á¦

	INT		nKillMAX;			//×î´óÉ±ÉËÁ¦
	INT		nKillMIN;			//×îÐ¡É±ÉËÁ¦
	INT		nRightKillMax;		//ÓÒ¼ü×î´óÉ±ÉËÁ¦
	INT		nRightKillMin;		//ÓÒ¼ü×îÐ¡É±ÉËÁ¦

	INT		nLeftAttack;			//¹¥»÷Á¦
	INT		nRightAttack;			//¹¥»÷Á¦
	INT		nDefence;			//·ÀÓùÁ¦
	INT		nMoveSpeed;			//ÒÆ¶¯ËÙ¶È
	INT		nAttackSpeed;		//¹¥»÷ËÙ¶È
	INT		nCastSpeed;

	INT		nPhyDef;			//ÎïÀí·ÀÓù
	INT		nPhyDefPlus;			//ÎïÀí·ÀÓù
	INT		nCoolDef;			//±ù¶³·ÀÓù
	INT		nCoolDefPlus;			//±ù¶³·ÀÓù
	INT		nLightDef;			//ÉÁµç·ÀÓù
	INT		nLightDefPlus;			//ÉÁµç·ÀÓù
	INT		nFireDef;			//»ðÑæ·ÀÓù
	INT		nFireDefPlus;			//»ðÑæ·ÀÓù
	INT		nPoisonDef;			//¶¾ËØ·ÀÓù
	INT		nPoisonDefPlus;			//¶¾ËØ·ÀÓù

	INT		nPKValue;
	INT		nRepute;
	INT		nFuYuan;
	INT		nTranslife;
	INT		nRankInWorld;
	char	Title[128];	//³ÆºÅ
};

//==================================
//	Ö÷½ÇµÄÁ¢¼´Ê¹ÓÃÎïÆ·ÓëÎä¹¦
//==================================
struct KUiPlayerImmedItemSkill
{
	KUiGameObject	ImmediaItem[IMMEDIACY_ROOM_WIDTH];
	KUiGameObject	IMmediaSkill[2];
};

//==================================
//	Ö÷½Ç×°±¸°²»»µÄÎ»ÖÃ
//==================================
enum UI_EQUIPMENT_POSITION
{
	UIEP_HEAD = 0,
	UIEP_HAND,
	UIEP_NECK,
	UIEP_FINESSE,
	UIEP_BODY,
	UIEP_WAIST,
	UIEP_FINGER1,
	UIEP_FINGER2,
	UIEP_WAIST_DECOR,
	UIEP_FOOT,
	UIEP_HORSE,
	UIEP_MASK,
	UIEP_MANTLE,
	UIEP_SIGNET,
	UIEP_SHIPIN,
	UIEP_HOODS,
	UIEP_CLOAK,
};

//TamLTM Code kham nam xanh
enum UI_BUILDITEM_POSITION
{
	UIEP_BUILDITEM1 = 0,
	UIEP_BUILDITEM2 = 1,
	UIEP_BUILDITEM3 = 2,
	UIEP_BUILDITEM4 = 3,
	UIEP_BUILDITEM5 = 4,
	UIEP_BUILDITEM6 = 5,
	UIEP_BUILDITEM7 = 6,
	UIEP_BUILDITEM8 = 7,
	UIEP_BUILDITEM9 = 8,
};
// End code

enum MOSAICENCRUSTED_POSITION
{
	MOSAICENCRUSTED_UIEP_BOX_1 = 0,
	MOSAICENCRUSTED_UIEP_BOX_2,
	MOSAICENCRUSTED_UIEP_BOX_3,
	MOSAICENCRUSTED_UIEP_BOX_4,
	MOSAICENCRUSTED_UIEP_BOX_5,
	MOSAICENCRUSTED_UIEP_BOX_6,
	MOSAICENCRUSTED_UIEP_BOX_7,
	MOSAICENCRUSTED_UIEP_BOX_8,
	MOSAICENCRUSTED_UIEP_BOX_9,
};

enum PLAYER_SECLECT_UI
{
	SELECT_TALKUI = 0,
	SELECT_SELECTUI,
	SELECT_NUM,
};

//==================================
//	µ¥Ïî¼¼ÄÜÊý¾Ý
//==================================
struct KUiSkillData : public KUiGameObject
{
	union
	{
		INT		nLevel;
		INT		nData;
	};
};

//==================================
//	Ò»¸ö¶ÓÎéÖÐ×î¶à°üº¬³ÉÔ±µÄÊýÄ¿
//==================================
#define	PLAYER_TEAM_MAX_MEMBER	8

//==================================
//	Í³Ë§ÄÜÁ¦Ïà¹ØµÄÊý¾Ý
//==================================
struct KUiPlayerLeaderShip
{
	INT		nLeaderShipLevel;			//Í³Ë§Á¦µÈ¼¶
	INT		nLeaderShipExperience;		//Í³Ë§Á¦¾­ÑéÖµ
	INT		nLeaderShipExperienceFull;	//Éýµ½ÏÂ¼¶ÐèÒªµÄ¾­ÑéÖµ
};

//==================================
//	Ò»¸öÍæ¼Ò½ÇÉ«Ïî
//==================================
struct KUiPlayerItem
{
	char			Name[32];	//Íæ¼Ò½ÇÉ«ÐÕÃû
	UINT	uId;		//Íæ¼Ò½ÇÉ«id
	INT				nIndex;		//Íæ¼Ò½ÇÉ«Ë÷Òý
	INT				nData;		//´ËÍæ¼ÒÏà¹ØµÄÒ»ÏîÊýÖµ£¬º¬ÒåÓë¾ßÌåµÄÓ¦ÓÃÎ»ÖÃÓÐ¹Ø
	INT				nTeamID;
};

//==================================
//	×é¶ÓÐÅÏ¢µÄÃèÊö
//==================================
struct KUiTeamItem
{
	KUiPlayerItem	Leader;
};

//==================================
//	¶ÓÎéÐÅÏ¢
//==================================
struct KUiPlayerTeam
{
	bool			bTeamLeader;			//Íæ¼Ò×Ô¼ºÊÇ·ñ¶Ó³¤
	char			cNumMember;				//¶ÓÔ±ÊýÄ¿
	char			cNumTojoin;				//Óû¼ÓÈëµÄÈËÔ±µÄÊýÄ¿
	bool			bOpened;				//¶ÓÎéÊÇ·ñÔÊÐíÆäËûÈË¼ÓÈë
	INT				nModePick;
	bool			bPKFollowCaptain;				//¶ÓÎéÊÇ·ñÔÊÐíÆäËûÈË¼ÓÈë
	INT				nTeamServerID;			//¶ÓÎéÔÚ·þÎñÆ÷ÉÏµÄid£¬ÓÃÓÚ±êÊ¶¸Ã¶ÓÎé£¬-1 Îª¿Õ
	INT				nCaptainPower;
};

//==================================
//	Ä§·¨ÊôÐÔ
//==================================
#ifndef MAGICATTRIB
#define MAGICATTRIB
struct KMagicAttrib
{
	INT			nAttribType;					//ÊôÐÔÀàÐÍ
	INT			nValue[3];						//ÊôÐÔ²ÎÊý
	int16_t			nMin;
	int16_t			nMax;
	KMagicAttrib() { nValue[0] = nValue[1] = nValue[2] = nAttribType = nMin = nMax = 0; };
};

struct KMagicAutoSkill
{
	INT				nSkillId;
	INT				nSkillLevel;
	INT				nRate;						//ÊôÐÔ²ÎÊý
	DWORD			dwNextCastTime;
	INT				nWaitCastTime;
	KMagicAutoSkill() { nSkillId = nSkillLevel = nRate = dwNextCastTime = nWaitCastTime = 0; };
};
#else
struct KMagicAttrib;
struct KMagicAutoSkill;
#endif


struct KStateControl
{
	INT				nSkillId;
	INT				nLeftTime;
};

struct KMapPos
{
	INT		nSubWorld;
	INT		nRegion;
	INT		nMapX;
	INT		nMapY;
	INT		nOffX;
	INT		nOffY;
};

//==================================
//	Ñ¡ÏîÉèÖÃÏî
//==================================
enum OPTIONS_LIST
{
	OPTION_PERSPECTIVE,		//Í¸ÊÓÄ£Ê½  nParam = (INT)(bool)bEnable ÊÇ·ñ¿ªÆô
	OPTION_DYNALIGHT,		//¶¯Ì¬¹âÓ°	nParam = (INT)(bool)bEnable ÊÇ·ñ¿ªÆô
	OPTION_MUSIC_VALUE,		//ÒôÀÖÒôÁ¿	nParam = ÒôÁ¿´óÐ¡£¨È¡ÖµÎª0µ½-10000£©
	OPTION_SOUND_VALUE,		//ÒôÐ§ÒôÁ¿	nParam = ÒôÁ¿´óÐ¡£¨È¡ÖµÎª0µ½-10000£©
	OPTION_BRIGHTNESS,		//ÁÁ¶Èµ÷½Ú	nParam = ÁÁ¶È´óÐ¡£¨È¡ÖµÎª0µ½-100£©
	OPTION_WEATHER,			//ÌìÆøÐ§¹û¿ª¹Ø nParam = (INT)(bool)bEnable ÊÇ·ñ¿ªÆô
};

//==================================
//	Ëù´¦µÄµØÓòÊ±¼ä»·¾³ÐÅÏ¢
//==================================

struct KUiSceneTimeInfo
{
	char	szSceneName[80];		//³¡¾°Ãû
	INT		nSceneId;				//³¡¾°id
	INT		nScenePos0;				//³¡¾°µ±Ç°×ø±ê£¨¶«£©
	INT		nScenePos1;				//³¡¾°µ±Ç°×ø±ê£¨ÄÏ£©
	INT		nGameSpaceTime;			//ÒÔ·ÖÖÓÎªµ¥Î»
};


struct KUiSceneTimeInfoOften : public KUiSceneTimeInfo
{
};

struct KMapTraffic
{
	char	Content[80];
	INT		Kind;
	POINT	Point;
	POINT	TPoint;
	INT		MapIndex;
	void	Release() {
		memset(Content, 0, sizeof(Content));
		Kind = -1;
		Point.x = Point.y = TPoint.x = TPoint.y = MapIndex = 0;
	};
};
//==================================
//	¹âÔ´ÐÅÏ¢
//==================================
//ÕûÊý±íÊ¾µÄÈýÎ¬µã×ø±ê
struct KPosition3
{
	INT nX;
	INT nY;
	INT nZ;
};

struct KLightInfo
{
	KPosition3 oPosition;			// ¹âÔ´Î»ÖÃ
	DWORD dwColor;					// ¹âÔ´ÑÕÉ«¼°ÁÁ¶È
	LONG  nRadius;					// ×÷ÓÃ°ë¾¶
};


//Ð¡µØÍ¼µÄÏÔÊ¾ÄÚÈÝÏî
enum SCENE_PLACE_MAP_ELEM
{
	SCENE_PLACE_MAP_ELEM_NONE = 0x00,		//ÎÞ¶«Î÷
	SCENE_PLACE_MAP_ELEM_PIC = 0x01,		//ÏÔÊ¾ËõÂÔÍ¼
	SCENE_PLACE_MAP_ELEM_CHARACTER = 0x02,		//ÏÔÊ¾ÈËÎï
	SCENE_PLACE_MAP_ELEM_PARTNER = 0x04,		//ÏÔÊ¾Í¬¶ÓÎéÈË
};

//³¡¾°µÄµØÍ¼ÐÅÏ¢
struct KSceneMapInfo
{
	INT	nScallH;		//ÕæÊµ³¡¾°Ïà¶ÔÓÚµØÍ¼µÄºáÏò·Å´ó±ÈÀý
	INT nScallV;		//ÕæÊµ³¡¾°Ïà¶ÔÓÚµØÍ¼µÄ×ÝÏò·Å´ó±ÈÀý
	INT	nFocusMinH;
	INT nFocusMinV;
	INT nFocusMaxH;
	INT nFocusMaxV;
	INT nOrigFocusH;
	INT nOrigFocusV;
	INT nFocusOffsetH;
	INT nFocusOffsetV;
};

enum NPC_RELATION
{
	relation_none = 1,
	relation_self = 2,
	relation_ally = 4,
	relation_enemy = 8,
	relation_dialog = 16,
	relation_all = relation_none | relation_ally | relation_enemy | relation_self | relation_dialog,
	relation_num,
};

enum NPCKIND
{
	kind_normal = 0,
	kind_player,
	kind_partner,
	kind_dialoger,	//¶Ô»°Õß
	kind_bird,
	kind_mouse,
	kind_num
};

enum	// Îï¼þÀàÐÍ
{
	Obj_Kind_MapObj = 0,		// µØÍ¼Îï¼þ£¬Ö÷ÒªÓÃÓÚµØÍ¼¶¯»­
	Obj_Kind_Body,				// npc µÄÊ¬Ìå
	Obj_Kind_Box,				// ±¦Ïä
	Obj_Kind_Item,				// µôÔÚµØÉÏµÄ×°±¸
	Obj_Kind_Money,				// µôÔÚµØÉÏµÄÇ®
	Obj_Kind_LoopSound,			// Ñ­»·ÒôÐ§
	Obj_Kind_RandSound,			// Ëæ»úÒôÐ§
	Obj_Kind_Light,				// ¹âÔ´£¨3DÄ£Ê½ÖÐ·¢¹âµÄ¶«Î÷£©
	Obj_Kind_Door,				// ÃÅÀà
	Obj_Kind_Trap,				// ÏÝÚå
	Obj_Kind_Prop,				// Ð¡µÀ¾ß£¬¿ÉÖØÉú
	Obj_Kind_Obstacle,
	Obj_Kind_Num,				// Îï¼þµÄÖÖÀàÊý
};

//Ö÷½ÇÉí·ÝµØÎ»µÈÒ»Ð©¹Ø¼üÊôÐÔÏî
enum PLAYER_BRIEF_PROP
{
	PBP_LEVEL = 1,	//µÇ¼¶±ä»¯	nParam±íÊ¾µ±Ç°µÈ¼¶
	PBP_FACTION,	//ÃÅÅÉ		nParam±íÊ¾ÃÅÅÉÊôÐÔ£¬Èç¹ûnParamÎª-1±íÊ¾Ã»ÓÐÃÅÅÉ
	PBP_CLIQUE,		//°ïÅÉ		nParamÎª·Ç0Öµ±íÊ¾ÈëÁË°ïÅÉ£¬0Öµ±íÊ¾ÍÑÀëÁË°ïÅÉ
};

//ÐÂÎÅÏûÏ¢µÄÀàÐÍ¶¨Òå
enum NEWS_MESSAGE_TYPE
{
	NEWSMESSAGE_NORMAL,			//Ò»°ãÏûÏ¢£¬ÏÔÊ¾£¨Ò»´Î£©¾ÍÏûÏ¢ÏûÍöÁË
	//ÎÞÊ±¼ä²ÎÊý
	NEWSMESSAGE_COUNTING,		//µ¹¼Æ£¨Ãë£©ÊýÏûÏ¢£¬¼ÆÊýµ½0Ê±£¬¾ÍÏûÏ¢¾ÍÏûÍöÁË¡£
	//Ê±¼ä²ÎÊýÖÐµÄÊý¾Ý½á¹¹ÖÐ½öÃëÊý¾ÝÓÐÐ§£¬µ¹¼ÆÊýÒÔÃëÎªµ¥Î»¡£
	NEWSMESSAGE_TIMEEND,		//¶¨Ê±ÏûÏ¢£¬¶¨Ê±µ½Ê±£¬ÏûÏ¢¾ÍÏûÍêÁË£¬·ñÔòÃ¿°ë·ÖÖÓÏÔÊ¾Ò»´Î¡£
	//Ê±¼ä²ÎÊý±íÊ¾ÏûÍöµÄÖ¸¶¨Ê±¼ä¡£
};

struct KNewsMessage
{
	INT		nType;						//ÏûÏ¢ÀàÐÍ
	char	sMsg[MAX_MESSAGE_LENGTH];	//ÏûÏ¢ÄÚÈÝ
	INT		nMsgLen;					//ÏûÏ¢ÄÚÈÝ´æ´¢³¤¶È
};

struct KRankIndex
{
	bool			bValueAppened;	//Ã¿Ò»ÏîÊÇ·ñÓÐÃ»ÓÐ¶îÍâÊý¾Ý
	bool			bSortFlag;		//Ã¿Ò»ÏîÊÇ·ñÓÐÃ»ÓÐÉý½µ±ê¼Ç
	uint16_t	usIndexId;		//ÅÅÃûÏîIDÊýÖµ
};

#define MAX_RANK_MESSAGE_STRING_LENGTH 128

struct KRankMessage
{
	char szMsg[MAX_RANK_MESSAGE_STRING_LENGTH];	// ÎÄ×ÖÄÚÈÝ
	uint16_t		usMsgLen;				// ÎÄ×ÖÄÚÈÝµÄ³¤¶È
	int16_t			cSortFlag;				// Æì±êÖµ£¬QOO_RANK_DATAµÄÊ±ºò±íÊ¾³öÉý½µ£¬¸ºÖµ±íÊ¾½µ£¬ÕýÖµ±íÊ¾Éý£¬0Öµ±íÊ¾Î»ÖÃÎ´±ä
	INT			nValueAppend;			// ´ËÏî¸½´øµÄÖµ
};

struct KMissionRecord
{
	char			szHeader[32];
	INT				nHeaderLen;
	INT				nRecordAct;
	char			szContent[MAX_SCIRPTACTION_BUFFERNUM];	//´æ´¢ÈÎÎñÌáÊ¾ÐÅÏ¢µÄ»º³åÇø£¬£¨×Ö·û´®Îª¿ØÖÆ·ûÒÑ¾­±àÂëµÄ×Ö·û´®£©
	INT				nContentLen;	//sContentÄÚÓÐÐ§ÄÚÈÝµÄ³¤¶È(µ¥Î»£º×Ö½Ú)£¬³¤¶È×î´óÒ»¶¨²»³¬¹ý256×Ö½Ú
	UINT	uValue;			//¹ØÁªÊýÖµ
};

enum TEAM_FIGURE
{
	TEAM_CAPTAIN,				// ¶ÓÎé³ÉÔ±Éí·Ý ¶Ó³¤
	TEAM_MEMBER,				// ¶ÓÎé³ÉÔ±Éí·Ý ¶ÓÔ±
};

enum CHAT_STATUS
{
	CHAT_S_ONLINE = 0, // 在线
	CHAT_S_BUSY,	   // 忙碌
	CHAT_S_HIDE,	   // 隐身
	CHAT_S_LEAVE,	   // 离开
	CHAT_S_DISCONNECT, // 掉线
};

enum CHATROOM_FIGURE
{
	CHATROOM_NOTHING,
	CHATROOM_MEMBER,				// ¶ÓÎé³ÉÔ±Éí·Ý ¶ÓÔ±
	CHATROOM_HOST,				// ¶ÓÎé³ÉÔ±Éí·Ý ¶Ó³¤
};
//---------------------------- °ï»áÏà¹Ø ------------------------

#define		defTONG_MAX_DIRECTOR				7
#define		defTONG_MAX_MANAGER					56
#define		defTONG_ONE_PAGE_MAX_NUM			7
#define		defTONG_NOTIFY_LENGTH				256
#define		defTONG_NAME_LENGTH_32				32
#define		defTONG_NAME_LENGTH_12				12
#define		defTONG_EFF_LASTWEEK				1
#define		defTONG_EFF_THISWEEK				2
#define		defTONG_EFF_USEABLE					3

enum TONG_MEMBER_FIGURE
{
	enumTONG_FIGURE_MEMBER,				// °ïÖÚ
	enumTONG_FIGURE_MANAGER,			// ¶Ó³¤
	enumTONG_FIGURE_DIRECTOR,			// ³¤ÀÏ
	enumTONG_FIGURE_MASTER,				// °ïÖ÷
	enumTONG_FIGURE_NUM,
};

enum
{
	enumTONG_APPLY_INFO_ID_SELF,		// ÉêÇë²éÑ¯×ÔÉíÐÅÏ¢
	enumTONG_APPLY_INFO_ID_MASTER,		// ÉêÇë²éÑ¯°ïÖ÷ÐÅÏ¢
	enumTONG_APPLY_INFO_ID_DIRECTOR,	// ÉêÇë²éÑ¯³¤ÀÏÐÅÏ¢
	enumTONG_APPLY_INFO_ID_MANAGER,		// ÉêÇë²éÑ¯¶Ó³¤ÐÅÏ¢
	enumTONG_APPLY_INFO_ID_MEMBER,		// ÉêÇë²éÑ¯°ïÖÚÐÅÏ¢(Ò»Åú°ïÖÚ)
	enumTONG_APPLY_INFO_ID_ONE,			// ÉêÇë²éÑ¯Ä³°ï»á³ÉÔ±ÐÅÏ¢(Ò»¸ö°ïÖÚ)
	enumTONG_APPLY_INFO_ID_TONG_HEAD,	// ÉêÇë²éÑ¯Ä³°ï»áÐÅÏ¢£¬ÓÃÓÚÉêÇë¼ÓÈë°ï»á
	enumTONG_APPLY_INFO_ID_NUM,
};

enum
{
	enumTONG_CREATE_ERROR_ID1,	// Player[m_nPlayerIndex].m_nIndex <= 0
	enumTONG_CREATE_ERROR_ID2,	// ½»Ò×¹ý³ÌÖÐ
	enumTONG_CREATE_ERROR_ID3,	// °ï»áÃûÎÊÌâ
	enumTONG_CREATE_ERROR_ID4,	// °ï»áÕóÓªÎÊÌâ
	enumTONG_CREATE_ERROR_ID5,	// ÒÑ¾­ÊÇ°ï»á³ÉÔ±
	enumTONG_CREATE_ERROR_ID6,	// ×Ô¼ºµÄÕóÓªÎÊÌâ
	enumTONG_CREATE_ERROR_ID7,	// µÈ¼¶ÎÊÌâ	
	enumTONG_CREATE_ERROR_ID8,	// Ç®ÎÊÌâ
	enumTONG_CREATE_ERROR_ID9,	// ×é¶Ó²»ÄÜ½¨°ï»á
	enumTONG_CREATE_ERROR_ID10,	// °ï»áÄ£¿é³ö´í
	enumTONG_CREATE_ERROR_ID11,	// Ãû×Ö×Ö·û´®³ö´í
	enumTONG_CREATE_ERROR_ID12,	// Ãû×Ö×Ö·û´®¹ý³¤
	enumTONG_CREATE_ERROR_ID13,	// °ï»áÍ¬Ãû´íÎó
	enumTONG_CREATE_ERROR_ID14,	// °ï»á²úÉúÊ§°Ü
	enumTONG_CREATE_ERROR_ID15,	// °ï»á²úÉúÊ§°Ü
};

enum {
	enumWORLD_RANK_TOP_1 = 1,
	enumWORLD_RANK_TOP_2,
	enumWORLD_RANK_TOP_3,
	enumWORLD_RANK_TOP_4,
};

enum
{
	enumTONG_CHANGE_CAMP_ERROR_ID1,
	enumTONG_CHANGE_CAMP_ERROR_ID2,
	enumTONG_CHANGE_CAMP_ERROR_ID3,
	enumTONG_CHANGE_CAMP_ERROR_ID4,
	enumTONG_CHANGE_CAMP_ERROR_ID5,
	enumTONG_CHANGE_CAMP_ERROR_ID6,
	enumTONG_CHANGE_CAMP_ERROR_ID7,
	enumTONG_CHANGE_CAMP_ERROR_ID8,
};

//°ïÅÉÐÅÏ¢
struct KTongInfo
{
	char  szName[defTONG_NAME_LENGTH_32];			//°ï»áÃû×Ö
	char  szMasterName[defTONG_NAME_LENGTH_32];		//°ïÖ÷Ãû×Ö
	INT	  nTongJiyuParam;
	char  szTongJiyuNotify[defTONG_NOTIFY_LENGTH];
	INT   nFaction;				//°ï»áÕóÓª
	INT   nMoney;				//°ï»á×Ê½ð
	BYTE  btLevel;
	DWORD dwTotalEff;
	INT	  nMasterJoinTm;
	INT   nMasterSaveEff;
	BOOL  bRecruit;
	INT	  nMemberCount;         //°ïÖÚÊýÁ¿
	INT	  nManagerCount;
	INT	  nDirectorCount;
};

struct KTongParam
{
	INT		m_nLevel;
	INT		m_nLeadLevel;
	INT		m_nMoney;
	INT		m_nItemCount;
	INT		m_nItemGenre;
	INT		m_nItemDetail;
	INT		m_nRepute;
	INT		m_nFuYuan;
	INT		m_nLeaveMoney;
	INT		m_nLeaveRepute;
	INT		m_nLeaveTime;
	INT		m_nMoneyChangeCamp;
	INT		m_nMoneySaveJiyu;
};

//¶Ô°ïÄÚ³ÉÔ±×öµÄ¶¯×÷£¬»ò¸Ä±ä×Ô¼ºÓë°ï»áµÄ¹ØÏµµÄ¶¯×÷
enum TONG_ACTION_TYPE
{
	TONG_ACTION_DISMISS,       //ÌßÈË
	TONG_ACTION_ASSIGN,        //ÈÎÃü
	TONG_ACTION_DEMISE,        //´«Î»
	TONG_ACTION_LEAVE,         //Àë°ï
	TONG_ACTION_RECRUIT,       //ÕÐÈË×´Ì¬
	TONG_ACTION_APPLY,         //ÉêÇë¼ÓÈë
	TONG_ACTION_CHANGE_AGNAME,
	TONG_ACTION_CHANGE_MALE_AGNAME,
	TONG_ACTION_CHANGE_FEMALE_AGNAME,
	TONG_ACTION_CHANGE_CAMP_JUSTIE,
	TONG_ACTION_CHANGE_CAMP_EVIL,
	TONG_ACTION_CHANGE_CAMP_BALANCE,
};


enum TONG_MONEY_ACTION_TYPE
{
	TONG_ACTION_SAVE,       //ÌßÈË
	TONG_ACTION_GET,        //ÈÎÃü
};

//¶Ô°ïÄÚ³ÉÔ±×öµÄ¶¯×÷£¬»ò¸Ä±ä×Ô¼ºÓë°ï»áµÄ¹ØÏµµÄ¶¯×÷Ê±µÄ²ÎÊý
struct KTongOperationParam
{
	TONG_ACTION_TYPE	eOper;				//²Ù×÷Ïî
	INT                 nData[4];
	char				szData[32];		//²Ù×÷ÏîÐèÒªµÄÃÜÂë£¬Èç¹û¶ÔÓ¦µÄ²Ù×÷Ïî²»ÐèÒªÃÜÂë£¬ÔòÖ±½ÓÌîÃÜÂëÎª¿Õ¡£
};

//°ï»á³ÉÔ±Ïî
struct KTongMemberItem : public KUiPlayerItem
{
	char szAgname[defTONG_NAME_LENGTH_32];	//ÔÚ°ïÄÚµÄÖ°Îñ/³ÆºÅ
	BOOL bOnline;
	INT  nSaveEff;
	INT	 nJoinTm;
};

//Ä³¸öÍæ¼ÒÓëXXµÄ¹ØÏµ£¬(XX¿ÉÒÔÊÇ°ï»á£¬¶ÓÎéµÈµÈ)
struct KUiPlayerRelationWithOther : KUiPlayerItem
{
	INT		nRelation;
	INT		nParam;
};

//Í¨ÓÃµÄ´øÃû³ÆÃèÊöÓÎÏ·¶ÔÏóµÄ½á¹¹
struct KUiGameObjectWithName
{
	char			szName[32];
	char			szString[32];
	INT				nData;
	INT				nParam;
	UINT 	uParam;
};

enum
{
	enumLadderBegin = 0,
	enumTopTenMasterHand,
	enumTopTenRicher,
	enumTopTenKiller,
	enumTopTenRepute,
	enumTopTenFuYuan,
	enumTopTenAccumStat1,
	enumTopTenAccumStat2,
	enumTopTenHonorStat,
	enumTopTenTimeStat,
	enumTopTenTongLv,
	enumTopTenTongMn,
	enumTopTenTongEff,
	enumFacTopTenMasterHand,
	enumFacTopTenRicher = enumFacTopTenMasterHand + (MAX_FACTION + 1),
	enumLadderEnd = enumFacTopTenRicher + (MAX_FACTION + 1),
};

enum NPCFIGHT
{
	enumFightNone = 0,
	enumFightActive,
	enumFightNum,
};

enum
{
	enumPKNormal = 0,
	enumPKWar,
	enumPKMurder,
	enumPKTongWar,
	enumPKNum,
};

enum
{
	enumPKLogNothing,
	enumPKLogAll,
	enumPKLogTongWar,
	enumPKLogMurder,
	enumPKLogWar,
	enumPKLogNormal,
	enumPKLogNum,
};

struct KOneRoom
{
	char	lpszRoomHost[32];
	char	lpszRoomName[32];
	BYTE	byRoomMemNum;
	INT		nRoomLeftTime;
	INT		nRoomParam;
};

struct KOneMem
{
	char	lpszName[32];
	INT		nMemParam;
	INT		nMemIndex;
	BOOL	bIsRoomHost;
};

struct KLockItem
{
	KLockItem() { Clear(); };
	~KLockItem() { Clear(); };
	int16_t nState;
	DWORD dwLockTime;
	void Clear() {
		nState = LOCK_STATE_NORMAL;
		dwLockTime = 0;
	};
	BOOL IsLock() {
		if (nState == LOCK_STATE_LOCK || nState == LOCK_STATE_CHARACTER || nState == LOCK_STATE_FOREVER)
			return TRUE;
		else
			return FALSE;
	};
};

#define MAX_DATAGROUP_VALUE 4
struct KDataGroup
{
	INT		nNameId;
	INT		nValue[MAX_DATAGROUP_VALUE];
	char	szName1[32];
	char	szName2[32];
	void Clear() {
		nNameId = 0;
		memset(nValue, 0, sizeof(nValue));
		memset(szName1, 0, sizeof(szName1));
		memset(szName2, 0, sizeof(szName2));
	}
};

#define MAX_MISSION_PARAM			16
#define MAX_GLBMISSION_PARAM		3
#define MISSION_STATNUM				10
#define MISSION_PARAM_AVAILABLE		0
#define MISSION_AVAILABLE_VALUE		1
struct TMissionLadderSelfInfo
{
	char		  szMissionName[64];
	unsigned char ucGroup;
	INT			  nGlbParam[MAX_GLBMISSION_PARAM];
	INT			  nParam[MAX_MISSION_PARAM];
	void Clear() {
		memset(szMissionName, 0, sizeof(szMissionName));
		memset(nGlbParam, 0, sizeof(nGlbParam));
		memset(nParam, 0, sizeof(nParam));
	};
};

struct TMissionLadderInfo
{
	unsigned char ucGroup;
	char		  Name[32];
	INT			  nParam[MAX_MISSION_PARAM];
	void Clear() {
		ucGroup = 0;
		memset(Name, 0, sizeof(Name));
		memset(nParam, 0, sizeof(nParam));
	};
};

#define	MAX_SUPERSHOP_SHOPTAB 10
typedef struct
{
	INT     m_nCurShop;
	INT     m_nShopIdx[MAX_SUPERSHOP_SHOPTAB];
	INT     m_nShopNum;
	INT     m_nMoneyUnit;
	DWORD   m_SubWorldID;
	INT     m_nMpsX;
	INT     m_nMpsY;

	void Clear() {
		m_nCurShop = 0;
		memset(m_nShopIdx, -1, sizeof(m_nShopIdx));
		m_nShopNum = 0;
		m_nMoneyUnit = moneyunit_money;
		m_SubWorldID = static_cast<DWORD>(-1);
		m_nMpsX = 0;
		m_nMpsY = 0;
	}
} BuySellInfo;


struct PLAYERTRADE
{
	char cName[32];
	BOOL nTrade;
	INT nDest;
	void Release() {
		memset(cName, 0, sizeof(cName));
		nTrade = FALSE;
		nDest = 0;
	};
};

typedef struct
{
	char    szName[64];
	DWORD   dwColor;
	INT     nStateGraphics;
	DWORD   dwLeftTime;

	void Release() {
		memset(szName, 0, sizeof(szName));
		dwColor = 0;
		dwLeftTime = 0;
		nStateGraphics = 0;
	}
} KExpandRank;

struct	LockMoveItem
{
	BOOL bLock;
	INT	nPlace;
	void	Release() {
		bLock = FALSE;
		nPlace = 0;
	};
};


#define		MAX_AUTO_SIZE				50
#define		MAX_AUTO_LIST				15
#define		MAX_TIME_OVERLOOK			100
#define		MAX_TIME_RESET_OVERLOOK		1000
#define		MAX_TIME_SPACE				1500
#define		MAX_AUTO_STATESKILL			3
#define		MAX_AUTO_FILTERL			32

enum
{
	enumRoomNotEnough1,
	enumRoomNotEnough2,
	enumRoomNotEnough4,
	enumRoomNotEnough6,
};

enum
{
	enumPickEquipAll = 0,
	enumPickEquipJewelry,
	enumPickEquipHaveMagic,
	enumPickNum,
};

#endif
