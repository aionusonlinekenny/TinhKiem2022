/*****************************************************************************************
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
------------------------------------------------------------------------------------------
	提供对图形资源引用的维护操作。
*****************************************************************************************/
#pragma once
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include <cstdint>

//======================================
//		图形对象的引用结构
//======================================
struct KUiImageRef : public KRUImage
{
	INT			nNumFrames;		//图形对象的图形帧数目
	INT			nInterval;		//帧间隔
	INT			nFlipTime;		//当前帧切换出来的时间
};

#define	IDF_LEFT_TO_RIGHT	0
#define IDF_RIGHT_TO_LEFT	1
#define IDF_TOP_TO_BOTTOM	2
#define IDF_BOTTOM_TO_TOP	3


//======================================
//		图形对象的引用结构
//======================================
struct KUiImagePartRef : public KRUImagePart
{
	INT			Width;			//图横宽
	INT			Height;			//图纵宽
	INT			nDivideFashion;	//分割方式,取值为IDF_*之一
};

void	IR_InitUiImageRef(KUiImageRef& Img);
void	IR_InitUiImagePartRef(KUiImagePartRef& Img);
void	IR_UpdateTime();										//更新图形换帧计算用时钟
INT		IR_NextFrame(KUiImageRef& Img, bool bReverse = false);							//换帧计算
void	IR_GetReferenceSpot(KUiImageRef& Img, INT& h, INT& v);	//获得图像参考点（一般所说为重心）
void	IR_UpdateImagePart(KUiImagePartRef& Img, INT nPartValue, INT nFullValue);	//设置绘制图的局部
INT		IR_IsTimePassed(UINT uInterval, UINT& uLastTimer);			//判断时间是否已经到了
UINT IR_GetRemainTime(UINT uInterval, UINT uLastTimer);		//获取剩余时间，如果时间已经到了/过了，返回值都为0
UINT IR_GetCurrentTime();													//获取当前的时间
