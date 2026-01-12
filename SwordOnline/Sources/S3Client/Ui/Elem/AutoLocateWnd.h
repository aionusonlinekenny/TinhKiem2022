/*****************************************************************************************
//	窗口自动定位
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-2-17
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include <cstdint>

void ALW_GetWndPosition(INT& nX, INT& nY, INT nWidth, INT nHeight, bool bUseInputParamPos = false, bool bIgnoreCursorPos = false);
