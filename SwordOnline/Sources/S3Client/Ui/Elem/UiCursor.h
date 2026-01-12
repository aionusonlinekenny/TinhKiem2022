/*****************************************************************************************
//	鼠标指针窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-17
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "UiImage.h"

#define KUI_USE_HARDWARE_MOUSE  1
//#undef  KUI_USE_HARDWARE_MOUSE

#define	MAX_CURSOR_IMAGE	16

class KUiCursor
{
public:
	void	Show(INT bShow);								//设置鼠标指针的显示状态
	void	SetPosition(INT h, INT v);						//设置鼠标指针位置
	void	GetPosition(INT& h, INT& v);					//获取鼠标指针位置
	void	SetImage(INT nIndex, const char* pImgFile);		//载入鼠标指针图形
	INT		SwitchImage(INT nIndex);						//切换当前鼠标指针图形
	void	RestoreCursor();	        					//重新激活鼠标
	void	Paint();										//绘制鼠标指针
	void	Cleanup();										//清空全部资源
	KUiCursor();
	~KUiCursor();
private:
	INT			m_bShow;
	INT			m_nCurImage;
	INT			m_nPositionH;
	INT			m_nPositionV;

    #ifdef KUI_USE_HARDWARE_MOUSE
	    HCURSOR     m_CursorImages[MAX_CURSOR_IMAGE];
    #else // KUI_USE_HARDWARE_MOUSE
		KUiImageRef	m_CursorImages[MAX_CURSOR_IMAGE];
		INT			m_nHotspotH;
		INT			m_nHotspotV;
    #endif
};
