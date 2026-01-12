/*****************************************************************************************
//	界面窗口体系结构--切换显示状态时带移动控制的图形的窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-17
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndMovingImage.h"

class KWndShowAnimate : public KWndMovingImage
{
public:
	virtual INT	Init(KIniFile* pIniFile, const char* pSection);//初始化
	virtual void Show();
	virtual void Hide();
	virtual void ShowCompleted();
	static void	SetMoveSpeed(INT nMoveSpeed, INT nFullRange);
	static void SetInterval(UINT uInterval);
	KWndShowAnimate();
	void	Clone(KWndShowAnimate* pCopy);
protected:
	void	PaintWindow();
private:
	static	INT	ms_nMoveSpeed;
	static	INT	ms_nFullRange;
	INT		m_nCurrentSpeed;
	SIZE	m_AppearRange;
	SIZE	m_DisappearRange;
	static	UINT ms_uMoveTimeInterval;
	UINT	m_uMoveLastTime;		//当前帧切换出来的时间
};
