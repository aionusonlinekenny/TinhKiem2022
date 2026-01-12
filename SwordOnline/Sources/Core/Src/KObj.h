//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KObj.h
// Date:	2002.01.06
// Code:	边城浪子
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KObj_H
#define KObj_H

#ifdef	_SERVER
#define	MAX_OBJECT	20000
#else
#define	MAX_OBJECT	256
#endif

#include "KPolygon.h"

#ifndef _SERVER
#include "KSprite.h"
#include "KWavSound.h"
#include "KSprControl.h"
#endif

#include "KIniFile.h"
#include "GameDataDef.h"
#ifndef	_SERVER
#include "../../Represent/iRepresent/KRepresentUnit.h"
#endif
#define		OBJ_BAR_WIDTH		3								// 物件障碍 长
#define		OBJ_BAR_HEIGHT		5								// 物件障碍 宽
#define		OBJ_BAR_SIZE		(OBJ_BAR_WIDTH*OBJ_BAR_HEIGHT)	// 物件障碍 长x宽

#define		OBJ_BOX_STATE_CLOSE		0		// 宝箱状态 关
#define		OBJ_BOX_STATE_OPEN		1		// 宝箱状态 开
#define		OBJ_DOOR_STATE_CLOSE	0		// 门状态 关
#define		OBJ_DOOR_STATE_OPEN		1		// 门状态 开
#define		OBJ_PROP_STATE_DISPLAY	0		// 道具状态 关 不显示
#define		OBJ_PROP_STATE_HIDE		1		// 道具状态 开 显示

#define		OBJ_BELONG_TIME		600

enum	Obj_Bar_Type			// 物件地面格子障碍类型
{
	Obj_Bar_Empty = 0,			// 空，无障碍
	Obj_Bar_Full,				// 完整障碍
	Obj_Bar_LT,					// 左上区域为障碍
	Obj_Bar_RT,					// 右上区域为障碍
	Obj_Bar_LB,					// 左下区域为障碍
	Obj_Bar_RB,					// 右下区域为障碍
	Obj_Bar_Type_Num,			// 障碍类型数
};

typedef struct SOBJ_LIGHT_DATA
{
	INT			m_nRadius;			// 物件的光亮度
	INT			m_nRed;				// 红偏色
	INT			m_nGreen;			// 绿偏色
	INT			m_nBlue;			// 蓝偏色
	INT			m_nAlpha;			// alpha 值
	INT			m_nReflectType;		// 受光方式（不受光、重心、水平、左斜、右斜）
	SOBJ_LIGHT_DATA() { Release(); };
	void	Release() { m_nRadius = 0; m_nRed = 0; m_nGreen = 0; m_nBlue = 0; m_nAlpha = 0; m_nReflectType = 0; };
} OBJ_LIGHT;		// 物件光线相关

//---------------------------------------------------------------------------

#ifdef _SERVER
class ServerImage
{
public:
	INT			m_nTotalFrame;						// 总帧数
	INT			m_nCurFrame;						// 当前帧
	INT			m_nTotalDir;						// 总方向数
	INT			m_nCurDir;							// 当前方向
	DWORD		m_dwTimer;							// 时间计数器
	DWORD		m_dwInterval;						// 帧间隔（动画速度）

protected:
	INT			m_nDirFrames;						// 每方向帧数

public:
	ServerImage();									// 构造函数
	void		Release();							// 清空
	void		SetTotalFrame(INT nTotalFrame);		// 设定总帧数
	void		SetCurFrame(INT nCurFrame);			// 设定当前帧
	void		SetTotalDir(INT nTotalDir);			// 设定总方向数
	BOOL		SetCurDir(INT nDir);				// 设定当前方向
	void		SetInterVal(DWORD dwInterval);		// 设定帧间隔
	INT			GetOneDirFrames();					// 获得单方向帧数

	BOOL		GetNextFrame(BOOL bLoop = TRUE);	// 自动向后换帧
	BOOL		GetPrevFrame(BOOL bLoop = TRUE);	// 自动向前换帧
	BOOL		CheckEnd();							// 判断动画是否播放到最后
	void		SetDirStart();						// 设定当前方向的当前帧为第一帧
	void		SetDirEnd();						// 设定当前方向的当前帧为最后一帧
	BOOL		SetCurDir64(INT nDir);				// 设定当前方向(方向需从64方向转换到真正的方向)
	INT			GetCurDirFrameNo();
};
#endif
#ifdef TOOLVERSION
class CORE_API KObj
#else
class KObj
#endif
{
	friend class KObjSet;
public:
	INT			m_nID;					// 物件的世界唯一ID，客户端与服务器端一一对应，如果值为 0 ，此为客户端 obj ，服务器端没有对应的 obj
	INT			m_nDataID;				// 此物件在物件数据表中的位置(就是在模板中的编号)
	INT			m_nKind;				// 类型	对应 Obj_Kind_XXXX
	INT			m_nIndex;				// 在 Object 数组中的位置
	INT			m_nBelongRegion;		// 本物体属于哪个region，不是坐标，坐标可以变动，这个是随产生时那个region，客户端存储的是 region id

	INT			m_nSubWorldID;			// 子世界ID
	INT			m_nRegionIdx;			// 区域idx
	INT			m_nMapX;				// 地图格子坐标 x
	INT			m_nMapY;				// 地图格子坐标 y
	INT			m_nOffX;				// 地图格子中的偏移 x (注：其值为像素点值 * 1024)
	INT			m_nOffY;				// 地图格子中的偏移 y (注：其值为像素点值 * 1024)
	INT			m_nDir;					// 方向，此方向为物件的逻辑方向，按64方向计算，不是具体的图像方向
	INT				m_IsHaveAttack;	 // Whether it is set to invalid for attack
	UINT			m_AttackerDwid; // The dwid that was attacked last time
	UINT			m_AttackerTime; // Recovery time

	INT			m_nState;				// 状态
	INT			m_nLifeTime;			// 物件的消失时间（主要用于尸体类、装备类、宝箱类等）
	INT			m_nBornTime;			// 重生倒计时（box打开状态经过lifetime自动关闭，prop隐身状态经过lifetime自动显示）
	INT			m_nObjValue;

	char			m_szName[80];			// 物件名称
	//	char		m_szScriptName[80];		// 所带脚本文件名
	DWORD			m_dwScriptID;			// 所带脚本文件ID
	INT			m_nColorID;				// 物件名字显示时的颜色id，客户端通过查表找到相应颜色

	INT			m_nItemDataID;			// 用于物品，指明物品的具体数据
	INT			m_nItemWidth;			// 物品长（放置在容器中的时候）
	INT			m_nItemHeight;			// 物品宽（放置在容器中的时候）
	INT			m_nMoneyNum;			// 如果是掉在地上的钱，钱的数量
	INT 		m_nGenre;
	INT 		m_nDetailType;
	BOOL			m_bOverLook;
	INT				m_nObstacleKind; // Object obstacle type

#ifdef _SERVER
	BOOL			m_bPickExecute;
	// 如果是打怪物掉出来的装备或钱：谁打出来的(记录的是player index) if == -1 不从属于任何人，任何人都能捡
	INT			m_nBelong;
	// m_nBelong 起作用的时间(是个倒计时)
	INT			m_nBelongTime;

	ServerImage m_cImage;				// 服务器端图像控制
#endif

#ifndef _SERVER
	char		m_szImageName[80];		// 图像文件名
	char		m_szSoundName[80];		// 声音文件名
	KSprControl	m_cImage;				// 物件图像

	char		m_szImageDropName[80];	// 如果是item类型，掉出来的时候播放的动画文件名
	KSprControl	m_cImageDrop;			// 如果是item类型，掉出来的时候播放的动画的图像控制
	INT			m_nDropState;			// 如果是item类型，当前是掉出动画状态(1)还是放置在地板上状态(0)
	KRUImage	m_Image;
	UINT	m_SceneID;

	INT			m_nLayer;				// 物件位于第几层
	INT			m_nHeight;				// 物件的逻辑高度
	OBJ_LIGHT	m_sObjLight;			// 物件与亮度、发光相关的数据
	DWORD		m_dwNameColor;			// 物件名字的颜色
#endif

	KPolygon	Polygon;				// 底面多边形
	BYTE		m_btBar[OBJ_BAR_SIZE];	// 物件底面障碍信息

private:
	DWORD		m_dwTimer;				// 时间计数器(用于声音播放)

#ifndef _SERVER
	KCacheNode* m_pSoundNode;			// 声效指针
	KWavSound* m_pWave;				// 声效wav指针

	BOOL		m_bDrawFlag;			// 绘制是否特殊处理（FALSE 绘制   TRUE 不绘制）
#endif

public:
	KObj();
	void		GetMpsPos(INT* pX, INT* pY);
	void		Release();
	void		Activate();
	void		ExecScript(INT nPlayerIdx);
	void		SetIndex(INT nIndex);
	void		SetWorldID(INT nID);			// 物件的世界唯一 ID
	void		SetLifeTime(INT nLifeTime);
	void		SetObjValue(INT nValue) { m_nObjValue = nValue; };
	INT			GetObjValue() { return m_nObjValue; };
	BOOL		SetDir(INT n64Dir);
	void		SetState(INT nState, INT nPlaySoundFlag = 0);
	void		SetImageDir(INT nDir);
	void		Remove(BOOL bSoundFlag);
	void		SetScriptFile(char* lpszScriptFile);
	INT			GetKind();

#ifdef _SERVER
	void		SetObjPickExecute(BOOL bFlag) { m_bPickExecute = (BOOL)bFlag; };
	BOOL		GetObjPickExecute() { return m_bPickExecute; };
	void		SetItemDataID(INT nItemDataID);
	BOOL		SyncAdd(INT nClient);
	void		SyncState();
	void		SyncDir();
	void		SyncRemove(BOOL bSoundFlag);
	INT			GetItemDataID();
	void		SetItemBelong(INT nPlayerIdx);
	void		SetEntireBelong(INT nPlayerIdx);
#endif

#ifndef _SERVER
	void		Draw();
	void		DrawInfo(INT nFontSize = 12);
	void		DrawBorder();
#endif

	// 凸多边形转换为障碍信息
	void		PolygonChangeToBar(
		KPolygon Polygon,		// 凸多边形
		INT nGridWidth,			// 格子长
		INT nGridHeight,		// 格子宽
		INT nTableWidth,		// 表格长
		INT nTableHeight,		// 表格宽
		BYTE* lpbBarTable);		// 表格内容
	INT			GetDistanceSquare(INT nNpcIndex);
private:
	void		BoxOpen();
	void		BoxClose();
	void		DoorOpen();
	void		DoorClose();
#ifndef _SERVER
	void		PlayLoopSound();
	void		PlayRandSound();
	INT			GetSoundPan();
	INT			GetSoundVolume();
	void		PlaySound();
#endif
};

extern	KObj	Object[MAX_OBJECT];
//---------------------------------------------------------------------------
#endif