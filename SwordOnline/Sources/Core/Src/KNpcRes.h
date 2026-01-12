
#pragma once

#ifndef _SERVER

#include "KWavSound.h"
#include "KList.h"
#include "KNpcResNode.h"
#include "KNpcResList.h"
#include "KSprControl.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"

class KCacheNode;

class KStateSpr
{

public:
	INT				m_nID;						// 在表格文件中的位置（从 1 开始，0为空）
	INT				m_nType;					// 类型：头顶、脚底、身上
	INT				m_nPlayType;				// 播放类型
	INT				m_nBackStart;				// 身上类型 背后开始帧
	INT				m_nBackEnd;					// 身上类型 背后结束帧
	KSprControl		m_SprContrul;				// spr 控制
public:
	KStateSpr();
	void			Release();
};

#define		MAX_BLUR_FRAME		7
#define		START_BLUR_ALPHA	128
#define		BLUR_ALPHA_CHANGE	8

class KNpcBlur
{
public:
	INT				m_nActive;							// 当前残影处理是否处于激活状态
	INT				m_nBlurFrame;
	INT				m_nCurNo;							// 当前帧指针
	DWORD			m_dwTimer;							// 时间计数器
	DWORD			m_dwInterval;						// 多少帧取一次残影
	INT				m_nMapXpos[MAX_BLUR_FRAME];			// 对应的地图坐标 x
	INT				m_nMapYpos[MAX_BLUR_FRAME];			// 对应的地图坐标 y
	INT				m_nMapZpos[MAX_BLUR_FRAME];			// 对应的地图坐标 z
	UINT			m_SceneIDNpcIdx[MAX_BLUR_FRAME];
	UINT			m_SceneID[MAX_BLUR_FRAME];			// 
	KRUImage		m_Blur[MAX_BLUR_FRAME][MAX_PART];	// 残影绘制列表
public:
	KNpcBlur();
	~KNpcBlur();
	BOOL			Init();
	void			Remove();
	void			SetNextNo();
	void			SetMapPos(INT x, INT y, INT z, INT nNpcIdx);
	void			ChangeAlpha();
	void			ClearCurNo();
	void			SetFile(INT nNo, char* lpszFileName, INT nSprID, INT nFrameNo, INT nXpos, INT nYpos, INT nZpos, INT nBlurAlpha = START_BLUR_ALPHA);
	void			Draw(INT nIdx);
	BOOL			NowGetBlur();
};

#ifdef TOOLVERSION
class CORE_API  KNpcRes
#else
class KNpcRes
#endif
{

	enum
	{
		SHADOW_BEGIN = 0,
		STATE_BEGIN = 1,
		PART_BEGIN = 1 + 6,
		SPEC_BEGIN = 1 + MAX_PART + 6,
		MENUSTATE_BEGIN = 1 + MAX_PART + 6 + 1,
	};
public:
	INT				m_nXpos;							// 坐标 x
	INT				m_nYpos;							// 坐标 y
	INT				m_nZpos;							// 坐标 z
	INT				m_nXposNew;							// 坐标 x
	INT				m_nYposNew;							// 坐标 y
	INT				m_nZposNew;							// 坐标 z
	INT				m_nXposOld;							// 坐标 x
	INT				m_nYposOld;							// 坐标 y
	INT				m_nZposOld;							// 坐标 z
private:
	INT				m_nDoing;							// Npc的动作
	INT				m_nAction;							// Npc的实际动作（与武器、骑马有关）
	INT				m_nNpcKind;							// 特殊 普通
	// INT				m_nXpos;							// 坐标 x
	// INT				m_nYpos;							// 坐标 y
	// INT				m_nZpos;							// 坐标 z
	UINT 			m_SceneID_NPCIdx;                   // 在场景中的ID 对应的NPCidx
	UINT			m_SceneID;							// 在场景中的ID
	INT				m_nHelmType;						// 当前头部类型
	INT				m_nArmorType;						// 当前身体类型
	INT				m_nWeaponType;						// 当前武器类型
	INT				m_nHorseType;						// 当前马匹类型
	INT				m_nMantleType;						// 当前马匹类型
	BOOL			m_bRideHorse;						// 当前是否骑马
	INT				m_nBlurState;
	char			m_szSoundName[80];					// 当前音效文件名
	KCacheNode* m_pSoundNode;						// 声效指针
	KWavSound* m_pWave;							// 声效wav指针
public:
	enum
	{
		adjustcolor_physics = 0,		// 物理伤害
		adjustcolor_poison,
		adjustcolor_freeze,			// 火焰伤害
		adjustcolor_burn,			// 冰冻伤害
		adjustcolor_confuse,			// 闪电伤害
		adjustcolor_stun,			// 毒素伤害
	};
	KSprControl		m_cNpcImage[MAX_PART];				// 所有动作的所有spr文件名
	KSprControl		m_cNpcEffectImage[MAX_PART];				// 所有动作的所有spr文件名
	KSprControl		m_cNpcShadow;						// npc阴影
	KStateSpr		m_cStateSpr[MAX_SKILL_STATE];						// 状态特效，0 1 为头顶 2 3 为脚底 4 5 为身上
	KSprControl		m_cSpecialSpr;						// 特殊的只播放一遍的随身spr文件
	UINT			m_ulAdjustColorId;

	KSprControl		m_cMenuStateSpr;
	INT				m_nMenuState;
	INT				m_nBackMenuState;
	INT				m_nSleepState;

	INT				m_nSortTable[MAX_PART];				// 排序表

	KRUImage		m_cDrawFile[MAX_NPC_IMAGE_NUM];// 绘制列表 身体部件 + 阴影 + 魔法状态 + 特殊动画 + 头顶状态
	KNpcBlur		m_cNpcBlur;							// npc 残影

	KNpcResNode* m_pcResNode;						// npc 资源
	KNpcResNode* m_pcResTemp;						// npc 资源

private:
	// 由一个图像资源文件名得到他的阴影图像文件名
	void			GetSoundName();						// 获得当前动作的音效文件名
	void			PlaySound(INT nX, INT nY);			// 播放当前动作的音效
	void			SetMenuStateSpr(INT nMenuState);					// set menu state spr
public:
	KNpcRes();
	~KNpcRes();
	BOOL			Init(char* lpszNpcName, KNpcResList* pNpcResList);	// 初始化
	void			_Init(char* lpszNpcName, KNpcResList* pNpcResList);	// 初始化
	void			Remove(INT nNpcIdx);								// 清除
	void			Draw(INT nNpcIdx, INT nDir, INT nAllFrame, INT nCurFrame, BOOL bInMenu = FALSE, BOOL bPaintBody = TRUE);		// 绘制
	void			DrawBorder();
	INT				DrawMenuState(INT nHeightOffset);
	void			GetResFile(INT nActionNo, char* lpszSprName);
	BOOL			SetHelm(INT nHelmType);								// 设定头盔类型
	BOOL			SetArmor(INT nArmorType, INT nMantleType);							// 设定盔甲类型
	BOOL			SetWeapon(INT nWeaponType);							// 设定武器类型
	BOOL			SetHorse(INT nHorseType);							// 设定马匹类型
	BOOL			SetAction(INT nDoing);								// 设定动作类型
	INT				GetHelm() { return m_nHelmType; };
	INT				GetArmor() { return m_nArmorType; };
	INT				GetWeapon() { return m_nWeaponType; };
	BOOL			SetRideHorse(BOOL bRideHorse);						// 设定是否骑马
	BOOL			IgnoreShowRes();
	void			SetPos(INT nNpcIdx, INT x, INT y, INT z = 0, BOOL bFocus = FALSE, BOOL bMenu = FALSE);// 设定 npc 位置
	void			SetState(BYTE* pNpcStateList, KNpcResList* pNpcResList);	// 设定状态特效
	void			SetSpecialSpr(char* lpszSprName);					// 设定特殊的只播放一遍的随身spr文件
	void			SetBlur(BOOL bBlur);								// 残影打开关闭
	void			CreateBlur(INT nNpcIdx, INT nRange, INT nDir);
	void			SetAdjustColorId(ULONG ulColorId) { m_ulAdjustColorId = ulColorId; };			// 设置偏色情况，如果为0表示不偏色.
	INT				GetAction() { return m_nAction; };
	void			SetMenuState(INT nState, char* lpszSentence = NULL, INT nSentenceLength = 0);	// 设定头顶状态
	INT				GetMenuState();						// 获得头顶状态
	void			SetSleepState(BOOL bFlag);			// 设定睡眠状态
	BOOL			GetSleepState();						// 获得睡眠状态
	void			StopSound();
	INT				GetSndVolume(INT nVol);
	static void		GetShadowName(char* lpszShadow, char* lpszSprName);
	INT				GetNormalNpcStandDir(INT nFrame);	// 动画帧数转换成逻辑方向(0 - 63)
	void			MakeBlurLine(INT nNpcIdx, INT x, INT y, INT z, INT nToX, INT nToY, INT nToZ);
};
#endif
