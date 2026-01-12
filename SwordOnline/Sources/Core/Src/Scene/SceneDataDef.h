// ***************************************************************************************
// ³¡¾°Ä£¿éµÄÒ»Ð©Êý¾Ý¶¨Òå
// Copyright : Kingsoft 2002
// Author    : wooy(wu yue)
// CreateTime: 2002-11-11
// ---------------------------------------------------------------------------------------
// ***************************************************************************************

#ifndef SCENEDATADEF_H_HEADER_INCLUDED_C225572F
#define SCENEDATADEF_H_HEADER_INCLUDED_C225572F

#define	REGION_COMBIN_FILE_NAME_SERVER "Region_S.dat"		//³¡¾°µØÍ¼ÇøÓòÊý¾ÝµÄ·þÎñÆ÷¶Ë°æºÏ²¢ÎÄ¼þµÄÎÄ¼þÃû
#define	REGION_COMBIN_FILE_NAME_CLIENT "Region_C.dat"		//³¡¾°µØÍ¼ÇøÓòÊý¾ÝµÄ¿Í»§¶Ë°æºÏ²¢ÎÄ¼þµÄÎÄ¼þÃû

//³¡¾°µØÍ¼Êý¾ÝÎÄ¼þ£¬Ã¿¸öÇøÓò×éÖ¯³ÉÈô¸É¸ö´æ´¢²»Í¬Êý¾ÝµÄÎÄ¼þ£¬ÕâÐ©ÎÄ¼þÓÖÒÔÇøÓòÎªµ¥Î»£¬·Ö±ðºÏ²¢³ÉÒ»¸ö¼¯ºÏÎÄ¼þ
//¼¯ºÏÎÄ¼þµÄ¸ñÊ½ÈçÏÂ£º
//1.ÏÈ´æ´¢Ò»¸öËÄ×Ö½ÚµÄunsigned intÊý¾Ý£¬±íÊ¾´ËºÏ³ÉÎÄ¼þÖÐ¹²°üº¬¼¸¸ö×ÓÎÄ¼þ
//2.´æ´¢×ÓÎÄ¼þ´óÐ¡£¬ÒÔ¼°¿ªÊ¼¿ªÊ¼Æ«ÒÆÎ»ÖÃµÄÐÅÏ¢½á¹¹µÄÁÐ±í¡£ÓÐ¼¸¸ö×ÓÎÄ¼þ¾ÍÁ¬Ðø´æÓÐ¼¸¸öKCombinFileSection
//					¸÷×ÓÎÄ¼þµÄÆ«ÒÆÎ»ÖÃÊÇ´ÓÐÅÏ¢½á¹¹µÄÁÐ±íÖ®ºóµÄÎ»ÖÃ¿ªÊ¼ËãÆð.
struct KCombinFileSection
{
	UINT	uOffset;	//µ¥Î»£º×Ö½Ú
	UINT	uLength;	//µ¥Î»£º×Ö½Ú
};
//3.×ÓÎÄ¼þµÄ´æ´¢´ÎÐò£¬Ã¿¸ö×ÓÎÄ¼þ¶ÔÓ¦ÓÚÊ²Ã´£¬ÓÉSCENE_FILE_INDEXÀïÃ¶¾Ù¸ø³ö½âÊÍ¡£Ã¿¸öÃ¶¾ÙÖµ±íÊ¾×ÓÎÄ¼þÔÚºÏ²¢ÎÄ¼þÖÐµÄ´ÎÐòË÷Òý¡£
enum SCENE_FILE_INDEX
{
	REGION_OBSTACLE_FILE_INDEX	= 0,//"OBSTACLE.DAT"ÕÏ°­ÎÄ¼þ
	REGION_TRAP_FILE_INDEX,			//"Trap.dat"
	REGION_NPC_FILE_INDEX,			//"Npc_S.dat" or "Npc_C.dat"
	REGION_OBJ_FILE_INDEX,			//"Obj_S.dat" or "Obj_C.dat"
	REGION_GROUND_LAYER_FILE_INDEX,	//"Ground.dat"
	REGION_BUILDIN_OBJ_FILE_INDEX,	//"BuildinObj.Dat"

	REGION_ELEM_FILE_COUNT
};
//4.´æ´¢×ÓÎÄ¼þµÄÊý¾Ý¡£


#define REGION_GROUND_LAYER_FILE	"Ground.dat"			//µØ±í²ãÐÅÏ¢¶þ½øÖÆ´æ´¢ÎÄ¼þ
#define	REGION_BUILDIN_OBJ_FILE		"BuildinObj.Dat"		//³¡¾°µØÍ¼ÄÚ½¨¶ÔÏó¼¯ºÏÐÅÏ¢ÎÄ¼þ
#define	REGION_OBSTACLE_FILE		"OBSTACLE.DAT"			//ÕÏ°­ÎÄ¼þ
#define REGION_NPC_FILE_SERVER		"Npc_S.dat"
#define REGION_NPC_FILE_CLIENT		"Npc_C.dat"
#define REGION_OBJ_FILE_SERVER		"Obj_S.dat"
#define REGION_OBJ_FILE_CLIENT		"Obj_C.dat"
#define REGION_TRAP_FILE			"Trap.dat"				//ÊÂ¼þµãÎÄ¼þ

#define	MAX_RESOURCE_FILE_NAME_LEN	128
#define NOT_ABOVE_HEAD_OBJ			0xFFFF	//²¢·Ç¸ß¿Õ¶ÔÏó


enum IPOT_RENDER_LAYER
{
	IPOT_RL_COVER_GROUND	= 0x01,		//Æ½ÆÌÔÚµØÃæÉÏ£¬ÔÚÈ«²¿ÎïÌåÖ®ÏÂ
	IPOT_RL_OBJECT			= 0x02,		//µ±Ç°ÎïÌå²ã
	IPOT_RL_INFRONTOF_ALL	= 0x04,		//ÔÚÒ»ÇÐµÄÉÏ²ã
	IPOT_RL_LIGHT_PROP		= 0x08,		//´Ë¶ÔÏó´øÓÐ¹âÔ´ÊôÐÔ
};

//## ÈýÎ¬×ø±ê
struct KTriDimensionCoord
{
	INT x, y, z;
};

//¡ö¡ö¡öGround.DatµØ±í²ãÐÅÏ¢¶þ½øÖÆ´æ´¢ÎÄ¼þ¡ö¡ö¡ö
//°´´ÎÐò´æ´¢ÈçÏÂÈý²¿·ÖÄÚÈÝ£º
//1.ÎÄ¼þÍ·½á¹¹
struct KGroundFileHead
{
	UINT uNumGrunode;	//µØ±í¸ñÍ¼ÐÎµÄÊýÄ¿
	UINT uNumObject;	//½ôÌùµØÃæµÄÀàËÆÂ·ÃæÖ®ÀàµÄ¶ÔÏóµÄÊýÄ¿
	UINT uObjectDataOffset;
};
//2.´æ´¢KGroundFileHead::nNumGrunode¸öÒÔKSPRCrunode½á¹¹±íÊöµÄµØ±í¸ñÍ¼ÐÎ
struct KSPRCrunode//## µØ±í¸ñÍ¼ÐÎÐÅÏ¢¡£
{
	struct KSPRCrunodeParam
	{
		uint16_t h, v;		//´ËÍ¼ÐÎËùÔÚµÄ£¨ÇøÓòÄÚ£©µØ±í¸ñ×ø±ê
		uint16_t nFrame;		//ÒýÓÃÍ¼ÐÎµÄµÚ¼¸Ö¡
		uint16_t nFileNameLen;//Í¼ÐÎÎÄ¼þÃû³¤¶È£¨°üÀ¨½áÊø·û£©£¬È¡Öµ²»¿É´óÓÚMAX_RESOURCE_FILE_NAME_LEN
	}	Param;

	//## Í¼ÐÎÎÄ¼þÃû
	char szImgName[1];	//´Ë½á¹¹Îª±ä³¤£¬Êµ¼Ê³¤¶ÈÎª´Ë½á¹¹ÖÐµÄnFileNameLenÊýÖµ
};
//3.´æ´¢KGroundFileHead::nNumObject¸öÒÔKSPRCoverGroundObj½á¹¹±íÊöµÄ½ôÌùµØÃæ¶ÔÏó
//ÒªÇó°´ÕÕ»æÖÆµÄÏÈºóË³ÐòÅÅÁÐºÃ
#pragma pack(push, 2)
struct KSPRCoverGroundObj//## ±íÊö½ôÌùµØÃæµÄÀàËÆÂ·ÃæÖ®ÀàµÄ¶ÔÏó½á¹¹¡£
{
	//## ¶ÔÏó¶¨Î»µãÔÚ³¡¾°µØÍ¼µÄ×ø±ê¡£µ¥Î»£¨³¡¾°µã×ø±ê£©
	INT nPositionX;			//ºáÏò×ø±ê
	INT nPositionY;			//×ÝÏò×ø±ê
	
	char szImage[MAX_RESOURCE_FILE_NAME_LEN];	//ÒýÓÃµÄÍ¼ÐÎµÄÎÄ¼þÃû¡£
	
	uint16_t	nWidth;	//Í¼ÐÎºá¿í
	uint16_t	nHeight;//Í¼ÐÎ×Ý¿í

	//## ÒýÓÃÍ¼ÐÎµÄµÚ¼¸Ö¡
	uint16_t nFrame;

	//±£Áô£¬Îª¶¨Öµ0
	union
	{
		unsigned char	bReserved;
		unsigned char	bRelateRegion;
	};

	//## »æÖÆ´ÎÐò£¨Ö÷£©
	unsigned char	bOrder;

	//## »æÖÆ´ÎÐò£¨´Î£©
	int16_t	nLayer;

};
#pragma pack(pop)
//¡ö¡ö¡öGround.DatµØ±í²ãÐÅÏ¢¶þ½øÖÆ´æ´¢ÎÄ¼þ--´æ´¢ÄÚÈÝ½áÊø---¡ö¡ö¡ö


//¡ö¡ö¡öBuildinObj.Dat³¡¾°ÄÚ½¨¶ÔÏó¶þ½øÖÆ´æ´¢ÎÄ¼þ¡ö¡ö¡ö
//°´´ÎÐò´æ´¢ÈçÏÂÈý²¿·ÖÄÚÈÝ£º
//1.ÎÄ¼þÍ·½á¹¹
struct KBuildinObjFileHead
{
	//## ÇøÓòÖÐÄÚ½¨¶ÔÏóµÄÊýÄ¿
	UINT nNumBios;		//nNumBios = nNumBiosTree + nNumBiosLine + nNumBiosPoint + nNumBiosAbove
	//## ÒÔÊ÷·½Ê½ÅÅÐòµÄÄÚ½¨¶ÔÏóµÄÊýÄ¿
	uint16_t nNumBiosTree;
	//## ÒÔµ×±ß·½Ê½ÅÅÐòµÄÄÚ½¨¶ÔÏóµÄÊýÄ¿
	uint16_t nNumBiosLine;
	//## ÒÔµã·½Ê½ÅÅÐòµÄÄÚ½¨¶ÔÏóµÄÊýÄ¿
	uint16_t nNumBiosPoint;
	//## ¸ß¿ÕÄÚ½¨¶ÔÏóµÄÊýÄ¿
	uint16_t nNumBiosAbove;

	//## ¸ß¿Õ¶ÔÏó×î´óµÄ»æÍ¼´ÎÐò±àºÅ¡£
	uint16_t nMaxAboveHeadObjOrder;
	uint16_t nNumBulidinLight;
};
//2.KBuildinObjFileHead::nNumBios¸öÒÔKBuildinObj½á¹¹±íÊöµÄ³¡¾°ÄÚ½¨¶ÔÏó
//´æ´¢µÄË³ÐòÎª:´æ´¢ÒÔµã·½Ê½ÅÅÐòµÄÈ«²¿¶ÔÏó;
//ÔÙ´æ´¢ÒÔµ×±ß·½Ê½ÅÅÐòµÄÈ«²¿¶ÔÏó;
//ÔÙ´æ´¢°´°´Ê÷·½Ê½ÅÅÐòµÄÈ«²¿¶ÔÏó;
//ÔÙ´æ´¢È«²¿µÄ¸ß¿Õ¶ÔÏó
struct KBuildinObj//## ±íÊö³¡¾°µØÍ¼ÉÏµÄÄÚ½¨¶ÔÏóµÄ½á¹¹
{
	//## Í¼ÐÎµÄÊôÐÔ
	UINT Props;//ÆäÖµÎªÃ¶¾ÙSPBIO_PROPSÈ¡ÖµµÄ×éºÏ

	//====Í¼ÐÎÏà¹Ø====
	//ËÄ¸öÍ¼ÐÎµã°´ÄæÊ±Õë·½Ïò°ÚÁÐ
	KTriDimensionCoord ImgPos1;	//Í¼ÐÎµÚÒ»¸öµã¶ÔÓ¦×ø±êµã
	KTriDimensionCoord ImgPos2;	
	KTriDimensionCoord ImgPos3;
	KTriDimensionCoord ImgPos4; //Í¼ÐÎµÚËÄ¸öµã¶ÔÓ¦×ø±êµã

	int16_t	nImgWidth;		//Í¼ÐÎÌùÍ¼µÄ¿í(µ¥Î»£ºÏñËØµã)
	int16_t	nImgHeight;		//Í¼ÐÎÌùÍ¼µÄ¸ß(µ¥Î»£ºÏñËØµã)
    
	char  szImage[MAX_RESOURCE_FILE_NAME_LEN];	//Í¼ÐÎ×ÊÔ´µÄÎÄ¼þÃû
	UINT   uFlipTime;	//µ±Ç°Ö¡ÇÐ»»³öÀ´µÄÊ±¼ä
    uint16_t nFrame;		//»æÖÆµÄÍ¼ÐÎÖ¡Ë÷Òý
	uint16_t nImgNumFrames;//´Ë¶ÔÏó°üº¬Í¼ÐÎÖ¡µÄÊýÄ¿
	uint16_t nAniSpeed;	//¶¯»­µÄ²¥·ÅËÙ¶È£¬ÖµÁã±íÊ¾Îªµ¥Ö¡Í¼

	//====¸ß¿Õ¶ÔÏóµÄ»æÍ¼Ë³Ðò====
	uint16_t nOrder;

	//====±ê¼Ç¶ÔÏóÎ»ÖÃ====
	KTriDimensionCoord oPos1;
	KTriDimensionCoord oPos2;

	float fAngleXY;
	float fNodicalY;
};
//¡ö¡ö¡öBuildinObj.Dat³¡¾°ÄÚ½¨¶ÔÏó¶þ½øÖÆ´æ´¢ÎÄ¼þ--´æ´¢ÄÚÈÝ½áÊø---¡ö¡ö¡ö


//## spbio¶ÔÏóµÄÊôÐÔ¶¨Òå
enum SPBIO_PROPS_LIST
{ 
	//##Documentation
	//## Æ½ÐÐÓÚµØ±í£¬²»×÷ÊÜ¹âÇÐ·Ö¡£
	SPBIO_P_PLANETYPE_H = 0x00, 
	//##Documentation
	//## Æ½ÐÐÓÚµØ±í£¬ÇÒ×÷ÊÜ¹âÇÐ·Ö¡£
	SPBIO_P_PLANETYPE_H_D = 0x01, 
	//##Documentation
	//## ´¹Ö±ÓÚµØ±í£¬²»×÷ÊÜ¹âÇÐ·Ö¡£
	SPBIO_P_PLANETYPE_V = 0x02, 
	//##Documentation
	//## ´¹Ö±ÓÚµØ±í£¬ÇÒ×÷ÊÜ¹âÇÐ·Ö¡£
	SPBIO_P_PLANETYPE_V_D = 0x03, 
	//##Documentation
	//## Í¼ÃæÀàÐÍ¶¨ÒåÖµmask
	SPBIO_P_PLANETYPE_MASK = 0x03, 
	//##Documentation
	//## ²»ÊÜ¹â
	SPBIO_P_LIT_NONE = 0x00, 
	//##Documentation
	//## °´ÕÕÇÐ·ÖÊÜ¹â
	SPBIO_P_LIT_DIV = 0x04, 
	//##Documentation
	//## °´ÕÕÖØÐÄÊÜ¹â
	SPBIO_P_LIT_CENTRE = 0x08, 
	//##Documentation
	//## Æ½ÐÐÊÜ¹â
	SPBIO_P_LIT_PARALLEL = 0x0C, 
	//##Documentation
	//## ¹âÕÕÇÐ·Ö·½Ê½¶¨ÒåµÄmask
	SPBIO_P_LIT_MASK = 0x0C, 
	//##Documentation
	//## ²»µ²¹â
	SPBIO_P_BLOCK_LIGHT_NONE = 0x00, 
	//##Documentation
	//## °´ÕÕµ×±ßµ²¹â
	SPBIO_P_BLOCK_LIGHT_BOTTOM = 0x10, 
	//##Documentation
	//## °´ÕÕÒ»¸öÔ²µ²¹â
	SPBIO_P_BLOCK_LIGHT_CIRCLE = 0x20, 
	//##Documentation
	//## µ²¹â·½Ê½¶¨ÒåÖµmask
	SPBIO_P_BLOCK_LIGHT_MASK = 0x30, 
	//##Documentation
	//## µµ¹â°ë¾¶Îª0.2
	SPBIO_P_BLOCK_L_RADIUS_2 = 0x00, 
	//##Documentation
	//## µµ¹â°ë¾¶Îª0.3
	SPBIO_P_BLOCK_L_RADIUS_3 = 0x40, 
	//##Documentation
	//## µµ¹â°ë¾¶Îª0.4
	SPBIO_P_BLOCK_L_RADIUS_4 = 0x80, 
	//##Documentation
	//## µµ¹â°ë¾¶Îª0.5
	SPBIO_P_BLOCK_L_RADIUS_5 = 0xC0, 
	//##Documentation
	//## µµ¹â°ë¾¶ÎªÊýÖµmask
	SPBIO_P_BLOCK_L_RADIUS_MASK = 0xC0, 
	//##Documentation
	//## °´ÕÕµãÅÅÐò
	SPBIO_P_SORTMANNER_POINT = 0x0000, 
	//##Documentation
	//## °´ÕÕ±ßÅÅÐò
	SPBIO_P_SORTMANNER_LINE = 0x0100, 
	//##Documentation
	//## °´ÕÕÊ÷ÅÅÐò
	SPBIO_P_SORTMANNER_TREE = 0x0200, 
	//##Documentation
	//## ÅÅÐò·½Ê½µÄmask
	SPBIO_P_SORTMANNER_MASK = 0x0300,
	
	//##Documentation
	//## ÐèÒªÒª¼ÓÁÁÏÔÊ¾
	SPBIO_F_HIGHT_LIGHT = 0x0400,
};


//¡ö¡ö¡öTrap.dat³¡¾°ÊÂ¼þµãÐÅÏ¢¶þ½øÖÆ´æ´¢ÎÄ¼þ¡ö¡ö¡ö
//°´´ÎÐò´æ´¢ÈçÏÂÈý²¿·ÖÄÚÈÝ£º
//1.ÎÄ¼þÍ·½á¹¹
typedef struct KTrapFileHead
{
	UINT uNumTrap;		//ÊÂ¼þµãµÄÊýÄ¿
	UINT uReserved[2];	//¹Ì¶¨ÖµÎª0
};

//2.´æ´¢KTrapFileHead::nNumTrap¸öÒÔKSPTrap½á¹¹±íÊöµÄµØ±íÊÂ¼þµã
struct KSPTrap
{
	unsigned char	cX;			//XÎ»ÖÃ
	unsigned char	cY;			//YÎ»ÖÃ
	unsigned char	cNumCell;	//Á¬Ðø¸²¸ÇÁË¼¸¸öÐ¡¸ñ×Ó
	unsigned char	cReserved;	//±£Áô£¬¹Ì¶¨ÖµÎª0
	UINT	uTrapId;	//¸ù¾ÝÊÂ¼þ¹ØÁªÎÄ¼þÃû×ª»¯¶øµÃµ½µÄÊÂ¼þid±êÊ¶
};
//¡ö¡ö¡öTrap.Dat³¡¾°ÊÂ¼þµãÐÅÏ¢¶þ½øÖÆ´æ´¢ÎÄ¼þ--´æ´¢ÄÚÈÝ½áÊø---¡ö¡ö¡ö

// ■■■Trap.Dat scene event point information binary storage file--end of storage content---■■■
// Used to store map attribute file parameters
struct KMapList
{
	char nMapName[64];
	char nMapType[32];
	INT nSubWorldID;
	INT nNpcSeriesAuto;
	INT nNpcSeriesMetal;
	INT nNpcSeriesWood;
	INT nNpcSeriesWater;
	INT nNpcSeriesFire;
	INT nNpcSeriesEarth;
	INT nAutoGoldenNpc;
	INT nGoldenType;
	INT nNpcDefense;
	INT nNpcMaxLife;
	char nGoldenDropRate[MAX_RESOURCE_FILE_NAME_LEN];
	char nNormalDropRate[MAX_RESOURCE_FILE_NAME_LEN];
	char nNewWorldScript[MAX_RESOURCE_FILE_NAME_LEN];
	char nNewWorldParam[MAX_RESOURCE_FILE_NAME_LEN];
	unsigned char nCSeries; // npc 五行
	INT nMinLevel;
	INT nMaxLevel;
	INT nExpSale; // 经验倍率
	// unsigned short		nScriptNameLen;	// Npc脚本文件名长度（包括结束符），取值不可大于MAX_RESOURCE_FILE_NAME_LEN
	// char			    sZScript[MAX_RESOURCE_FILE_NAME_LEN];
};

//¡ö¡ö¡öNpc_S.dat, Npc_C.datµØÍ¼ÉÏµÄnpc´æ´¢ÎÄ¼þ¡ö¡ö¡ö
//°´´ÎÐò´æ´¢ÈçÏÂÈý²¿·ÖÄÚÈÝ£º
//1.ÎÄ¼þÍ·½á¹¹
struct KNpcFileHead
{
	UINT uNumNpc;		//ÊÂ¼þµãµÄÊýÄ¿
	UINT uReserved[2];	//¹Ì¶¨ÖµÎª0
};
//2.´æ´¢KNpcFileHead::nNumNpc¸öÒÔKSPNpc½á¹¹±íÊöµÄnpc
struct KSPNpc
{
	INT				nTemplateID;	// Ä£°å±àºÅ(¶ÔÓ¦Ä£°åÎÄ¼þ"\Settings\NpcS.txt"£¬±àºÅ´Ó 0 ¿ªÊ¼)
	INT				nPositionX;
	INT				nPositionY;
	bool				bSpecialNpc;	// ÌØÊânpc
	char				cReserved[3];	// ±£Áô
	char				szName[32];		// npc Ãû×Ö
	int16_t				nLevel;			// µÈ¼¶
	int16_t				nCurFrame;		// ÔØÈëÊ±Í¼ÏñÊÇµÚ¼¸Ö¡
	int16_t				nHeadImageNo;	// Í·ÏñÍ¼ÐÎ±àºÅ
	int16_t				shKind;			// npcÀàÐÍ
	unsigned char		cCamp;			// npc ÕóÓª
	unsigned char		cSeries;		// npc ÎåÐÐ
	uint16_t			nScriptNameLen;	// Npc½Å±¾ÎÄ¼þÃû³¤¶È£¨°üÀ¨½áÊø·û£©£¬È¡Öµ²»¿É´óÓÚMAX_RESOURCE_FILE_NAME_LEN
	char				szScript[MAX_RESOURCE_FILE_NAME_LEN];	//Npc½Å±¾ÎÄ¼þÃû//´Ë½á¹¹Êµ¼Ê´æ´¢Ê±Îª±ä³¤£¬Êµ¼Ê³¤¶ÈÎª´Ë½á¹¹ÖÐµÄnScriptNameLenÊýÖµ
	char				nNDropFile[MAX_RESOURCE_FILE_NAME_LEN];
	char				nGDropFile[MAX_RESOURCE_FILE_NAME_LEN];
};
//¡ö¡ö¡öNpc_S.dat, Npc_C.datµØÍ¼ÉÏµÄnpc´æ´¢ÎÄ¼þ--´æ´¢ÄÚÈÝ½áÊø---¡ö¡ö¡ö

//¡ö¡ö¡öObj_S.dat, Obj_C.datµØÍ¼ÉÏµÄObj´æ´¢ÎÄ¼þ¡ö¡ö¡ö
//°´´ÎÐò´æ´¢ÈçÏÂÈý²¿·ÖÄÚÈÝ£º
//1.ÎÄ¼þÍ·½á¹¹
struct KObjFileHead
{
	UINT uNumObj;		//ÊÂ¼þµãµÄÊýÄ¿
	UINT uReserved[2];	//¹Ì¶¨ÖµÎª0
};
//2.´æ´¢KObjFileHead::nNumObj¸öÒÔKSPObj½á¹¹±íÊöµÄObj
struct KSPObj
{
	INT					nTemplateID;	//Ä£°å±àºÅ(¶ÔÓ¦Ä£°åÎÄ¼þ"\Settings\ObjData.txt"£¬±àºÅ´Ó1¿ªÊ¼)
	int16_t				nState;
	uint16_t		nBioIndex;		//µ±bSkipPaintÎªÕæÖµÊ±£¬´ËÊý¾ÝÓÐÐ§£¬·ñÔòÎª¶¨Öµ0
	KTriDimensionCoord	Pos;
	char				nDir;
	bool				bSkipPaint;		//ÊÇ·ñºöÂÔ´ËobjµÄ»æÖÆ
	uint16_t		nScriptNameLen;	//Obj½Å±¾ÎÄ¼þÃû³¤¶È£¨°üÀ¨½áÊø·û£©£¬È¡Öµ²»¿É´óÓÚMAX_RESOURCE_FILE_NAME_LEN
	char				szScript[MAX_RESOURCE_FILE_NAME_LEN];	//Obj½Å±¾ÎÄ¼þÃû//Êµ¼Ê´æ´¢Ê±Îª±ä³¤£¬Êµ¼Ê³¤¶ÈÎª´Ë½á¹¹ÖÐµÄnScriptNameLenÊýÖµ
};
//¡ö¡ö¡öObj_S.dat, Obj_C.datµØÍ¼ÉÏµÄnpc´æ´¢ÎÄ¼þ--´æ´¢ÄÚÈÝ½áÊø---¡ö¡ö¡ö

// ³¡¾°ÄÚ½¨¹âÔ´ÊôÐÔ½á¹¹
struct KBuildInLightInfo
{
	KTriDimensionCoord	oPos;			// ¹âÔ´×ø±ê
	DWORD				dwColor;		// ¹âÔ´ÑÕÉ«ºÍÁÁ¶È
	INT					nMinRange;		// ×îÐ¡°ë¾¶
	INT					nMaxRange;		// ×î´ó°ë¾¶
	INT					nCycle;			// Ñ­»·ÖÜÆÚ,ºÁÃë
};

#pragma pack(1)
// ÓÃÓÚ¹âÕÕ¼ÆËãµÄÑÕÉ«½á¹¹
struct KLColor
{
	//UINT r;
	//UINT g;
	//UINT b;
    //UINT ReserveForAlign;
	uint16_t r;
	uint16_t g;
	uint16_t b;
    uint16_t ReserveForAlign;

    // ÏÂÃæµÄ´úÂëµ¼ÖÂºÜ¶àµÄÐÔÄÜËðÊ§
//	KLColor()
//	{
//        r = 0, g = 0, b = 0;
//	}
//	
//	KLColor(INT rr, INT gg, INT bb)
//	{
//		r = rr, g = gg, b = bb;
//	}

	void SetColor(INT rr, INT gg, INT bb)
	{
		r = rr, g = gg, b = bb;
	}

	void SetColor(DWORD dwColor)
	{
		r = (uint16_t)((dwColor>>16) & 0xff);
		g = (uint16_t)((dwColor>>8) & 0xff);
		b = (uint16_t)(dwColor & 0xff);
	}
	void Scale(float f)
	{
		r = (INT)(r * f);
		g = (INT)(g * f);
		b = (INT)(b * f);
	}
	DWORD GetColor()
	{
		return 0xff000000 | (r<<16) | (g<<8) | b;
	}

	const KLColor& operator+=(KLColor& color)
	{
		r += color.r, g += color.g, b += color.b;
		return *this;
	}

	const KLColor operator+(KLColor& color)
	{
		KLColor c;
		c.r += r + color.r, c.g += g + color.g, c.b += b + color.b;
		return c;
	}
};

#pragma pack()


#endif /* SCENEDATADEF_H_HEADER_INCLUDED_C225572F */
