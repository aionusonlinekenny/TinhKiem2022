#ifndef __SKILLDEF_H__
#define __SKILLDEF_H__

#define MAX_SKILL							2500
#define	MAX_LIVESKILL						10
#define MAX_SKILLLEVEL						64
#define MAX_TRAIN_SKILLEXPLEVEL				20
#define MaxMissleDir						64
#define MAXSKILLLEVELSETTINGNUM				20  //供填写技能升级时最大的相关数据种类
#define MAX_MISSLESTYLE						650
#define MISSLE_MIN_COLLISION_ZHEIGHT		0	  //子弹落地碰撞的高度。
#define MISSLE_MAX_COLLISION_ZHEIGHT		20   //子弹高于该高度时,不计算碰撞	
#define MAX_ADDSKILLDAMAGE					9
#define MAX_APPENDSKILL						5
#define MAX_AUTOSKILL						5

//---------------------------------------------------------------------------
// MoveKind 运动类型
//---------------------------------------------------------------------------
enum eMissleMoveKind
{
	MISSLE_MMK_Stand,							//	原地
	MISSLE_MMK_Line,							//	直线飞行
	MISSLE_MMK_Random,							//	随机飞行（暗黑二女巫的Charged Bolt）
	MISSLE_MMK_Circle,							//	环行飞行（围绕在身边，暗黑二刺客的集气）
	MISSLE_MMK_Helix,							//	阿基米德螺旋线（暗黑二游侠的Bless Hammer）
	MISSLE_MMK_Follow,							//	跟踪目标飞行
	MISSLE_MMK_Motion,							//	玩家动作类
	MISSLE_MMK_Parabola,						//	抛物线
	MISSLE_MMK_SingleLine,						//	必中的单一直线飞行魔法
	MISSLE_MMK_RollBack = 100,					//  子单来回飞行
	MISSLE_MMK_Toss,						//	左右震荡
};

//---------------------------------------------------------------------------
// FollowKind 跟随类型	(主要是针对原地、环行与螺旋线飞行有意义)
//---------------------------------------------------------------------------
enum eMissleFollowKind
{
	MISSLE_MFK_None,							//	不跟随任何物件
	MISSLE_MFK_NPC,								//	跟随NPC或玩家
	MISSLE_MFK_Missle,							//	跟随子弹
};

#define	MAX_MISSLE_STATUS 4
enum eMissleStatus
{
	MS_DoWait,
	MS_DoFly,
	MS_DoVanish,
	MS_DoCollision,
};


enum eSkillLRInfo
{
	BothSkill,          //左右键皆可
	LeftOnlySkill,		//左键
	RightOnlySkill,		//右键
	NoneSkill,			//都不可
};

//--------------------------------------------------------Skill.h

//技能发送者的类型
enum eGameActorType
{
	Actor_Npc,
	Actor_Obj,
	Actor_Missle,
	Actor_Sound,
	Actor_None,
};
enum eSkillLauncherType
{
	SKILL_SLT_Npc = 0,
	SKILL_SLT_Obj,
	SKILL_SLT_Missle,
};


#ifndef _SERVER

struct	TOrginSkill
{
	INT		nNpcIndex;				//	Npc的index
	DWORD	nSkillId;				//	发送的skillid
};

#endif


enum eSkillParamType
{
	SKILL_SPT_TargetIndex = -1,
	SKILL_SPT_Direction = -2,
};

// Type of skill
enum eSKillStyle
{
	SKILL_SS_Missles = 0,		 // Bullets 0 This skill is used to send bullets
	SKILL_SS_Melee,				 // External power system 1 long-range bullet attack skills?
	SKILL_SS_InitiativeNpcState, // Active type 2 This skill is used to change the active state of the current NPC.
	SKILL_SS_PassivityNpcState,	 // Passive type 3 This skill is used to change the passive state of NPC
	SKILL_SS_CreateNpc,			 // Generate npc class 4 This skill is used to generate a new npc
	SKILL_SS_BuildPoison,		 // Poison Refining Category 5 This skill is used for refining poison.
	SKILL_SS_AddPoison,			 // Poisoning type 6 This skill is used to add poison to weapons
	SKILL_SS_GetObjDirectly,	 // Fetching Category 7 This skill is used to fetch objects from the air.
	SKILL_SS_StrideObstacle,	 // Crossover Category 8 This skill is used to cross obstacles
	SKILL_SS_BodyToObject,		 // Transformation type 9 This skill is used to turn corpses into treasure chests
	SKILL_SS_Mining,			 // Mining Class 10 This skill is used to mine randomly generated ores.
	SKILL_SS_RepairWeapon,		 // Repair class 11 This skill is used to repair equipment
	SKILL_SS_Capture,			 // Capture class 12 This skill is used to capture animal NPCs
	SKILL_SS_Thief,				 // Theft category 13
	// SKILL_SS_NewMissles, //New bullets 14
};


//The direction starting format of multiple bullets fired at the same time
enum eMisslesForm
{
	SKILL_MF_Wall = 0, // 0 wall shape Multiple bullets are arranged vertically, similar to a fire wall shape
	SKILL_MF_Line,	   // 1 linear multiple bullets are arranged parallel to the direction of the player
	SKILL_MF_Spread,   // 2 Scattered shape Multiple bullets are diverged at a certain angle
	SKILL_MF_Circle,   // 3 circle multiple bullets in a circle
	SKILL_MF_Random,   // 4 Random multiple bullets are randomly discharged
	SKILL_MF_Zone,	   // Area 5 Place multiple bullets within a certain range
	SKILL_MF_AtTarget, // 6 fixed points based on multiple bullets
	SKILL_MF_AtFirer,  // 7 itself Multiple bullets stop at the player's current position
	SKILL_MF_COUNT,
};

enum eMeleeForm
{
	Melee_AttackWithBlur = SKILL_MF_COUNT,
	Melee_Jump,
	Melee_JumpAndAttack,
	Melee_RunAndAttack,
	Melee_ManyAttack,
	Melee_MoveWithBlur,
};


enum eSKillCostType
{
	SKILL_SCT_MANA = 1,
	SKILL_SCT_LIFE = 2,
	SKILL_SCT_STAMINA = 8,
	SKILL_SCT_MONEY = 16,
};

enum eMisslesGenerateStyle
{
	SKILL_MGS_NULL = 0,
	SKILL_MGS_SAMETIME,    //同时
	SKILL_MGS_ORDER,	 //按顺序
	SKILL_MGS_RANDONORDER,
	SKILL_MGS_RANDONSAME,
	SKILL_MGS_CENTEREXTENDLINE,  //由中间向两周扩散
};

typedef struct
{
	INT dx;
	INT dy;
}TCollisionOffset;

typedef struct
{
	INT nRegion;
	INT nMapX;
	INT nMapY;
}
TMisslePos;

typedef struct
{
	TCollisionOffset m_Offset[4];
}
TCollisionMatrix;
extern TCollisionMatrix g_CollisionMatrix[64];


typedef struct
{
	INT nLauncher;
	DWORD dwLauncherID;
	eSkillLauncherType eLauncherType; //发送者，一般为Npc


	INT nParent;
	eSkillLauncherType eParentType;	  //母	 
	DWORD dwParentID;

	INT nParam1;
	INT nParam2;
	INT nWaitTime;
	INT nTargetId;
	DWORD dwTargetNpcID;
}
TOrdinSkillParam, * LPOrdinSkillParam;

#endif //__SKILLDEF_H__
