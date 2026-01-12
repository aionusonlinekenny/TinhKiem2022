/*****************************************************************************************
//	界面窗口体系结构--带移动控制的显示图形的窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-10
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndImage.h"

class KWndMovingImage : public KWndImage
{
public:
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);//初始化
	void			SetMoveValue(INT nCurrentValue, INT nFullValue);
	KWndMovingImage();
protected:
	void	Clone(KWndMovingImage* pCopy);
protected:	
	POINT	m_oFixPos;
	SIZE	m_MoveRange;
	INT		m_nCurrentValue;
	INT		m_nFullValue;
};