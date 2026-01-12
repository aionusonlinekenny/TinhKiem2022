#pragma once
#include <cstdint>

typedef	int32_t(*fnTextProcessEncodePlugin)();
typedef	int32_t(*fnTextProcessDrawPlugin)();

struct KOutputTextParam
{
	int32_t	nX;
	int32_t	nY;
	int32_t	nZ;
	int16_t 	nSkipLine;
	int16_t 	nNumLine;
	uint32_t Color;
	uint32_t BorderColor;
	uint16_t nVertAlign;	//0:居中/1:上对齐/2:下对齐
	int32_t bPicPackInSingleLine;

	//加一个构造函数,主要是nVertAlign的初始化[wxb 2003-7-10]
	KOutputTextParam()
	{
		nX = nY = nZ = 0;
		nSkipLine = nNumLine = 0;
		Color = 0;
		nVertAlign = 1;	//缺省为居中
		bPicPackInSingleLine = false;
		BorderColor = 0;
	}
};