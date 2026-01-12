/*****************************************************************************************
//	界面窗口体系结构--值控制换帧的显示图形的窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-10
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndImage.h"

class KWndValueImage : public KWndImage
{
public:
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);//初始化
	void			SetValue(INT nCurrentValue, INT nFullValue);
	KWndValueImage();
	void			Clone(KWndValueImage* pCopy);
private:
	INT		m_nFrameCount;
	INT		m_nCurrentValue;
	INT		m_nFullValue;
};