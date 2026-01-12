/*****************************************************************************************
//	浮动提示窗口
//	Copyright : Kingsoft 2002-2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-23
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "WndMessage.h"
#include "UiImage.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../core/src/GameDataDef.h"

class KMouseOver
{
public:
	KMouseOver();
	INT		IsMoseHoverWndObj(void* pWnd, INT nObj);
	void    SetMouseHoverInfo(void* pWnd, INT nObj, INT x, INT y,
		                        bool bHeadTailImg, bool bFollowCursor, bool LAlign = false);
	void    SetMouseHoverImage(bool bAdd);
    void    SetMouseHoverTitle(const char *pcszTitleText, INT nTitleTextLen, UINT uColor);
	void    SetMouseHoverProp(const char *pcszPropText, INT nPropTextLen, UINT uColor);
	void    SetMouseHoverDesc(const char *pcszDescText, INT nDescTextLen, UINT uColor);
	void	CancelMouseHoverInfo();
	void	PaintMouseHoverInfo();
	void	LoadScheme(const char* pScheme);		//载入界面方案
	void	UpdateCursorPos(INT nX, INT nY);		//鼠标的位置更新了
	void	OnWndClosed(void* pWnd);
private:
	void	Update(INT nX, INT nY);
private:
	INT		m_nImgWidth;	        //图形水平长度
	INT		m_nImgHeight;	        //图形垂直高度
	INT		m_nLeft;		        //窗口左上角横坐标
	INT		m_nTop;			        //窗口左上角纵坐标
	INT		m_nWndWidth;	        //动态的窗口水平宽
	INT		m_nWndHeight;	        //动态的窗口垂直宽
	INT		m_nWndWidthReduce;	        //动态的窗口水平宽
	INT		m_nWndHeightReduce;	        //动态的窗口垂直宽
	INT		m_nWidthReduce;
	INT		m_nHeightReduce;
	INT		m_nMaxWidthReduce;
	INT		m_nMaxHeightReduce;
	INT		m_nIndent;		        //文字缩进（与边框的水平相隔距离）
	INT		m_nFontSize;	        //文字字体(大小)

	INT     m_nApplyX;              //外部提交的浮动窗口的X坐标
	INT     m_nApplyY;              //外部提交的浮动窗口的Y坐标
	INT     m_nTitleLineNum;        //标题(名字)部分的行数
	INT     m_nPropLineNum;         //属性部分的行数
	INT     m_nDescLineNum;         //描述部分的行数
	INT     m_nMaxLineLen;          //三部分一起算最长的行的长度

	UINT m_uTitleBgColor;   //标题(名字)背景颜色
	UINT m_uTitleColor;     //标题(名字)文字颜色
	UINT m_uPropBgColor;    //属性背景颜色
	UINT m_uPropColor;      //属性文字颜色
	UINT m_uDescBgColor;    //描述部分背景颜色
	UINT m_uDescColor;      //描述部分文字颜色
	/////////////////////////
	UINT m_uBoderShadowColor;      //属性文字颜色
	UINT m_uRectBordetShadowColor;    //描述部分背景颜色
	UINT m_uRectBordetColor;      //描述部分文字颜色
	/////////////////////////
	KRUImagePart m_Image;	        //边框图形

	void*	m_pMouseHoverWnd;		//鼠标停留的窗口
	INT		m_nObj;
	char    m_ObjTitle[GOD_MAX_OBJ_TITLE_LEN];
	char    m_ObjProp[GOD_MAX_OBJ_PROP_LEN];
	char	m_ObjDesc[GOD_MAX_OBJ_DESC_LEN];
	INT		m_nTitleLen;
	INT		m_nPropLen;
	INT		m_nDescLen;
	bool	m_bHeadTailImg;			//是否显示提头与结尾边框图
	bool	m_bTempImg;			//是否显示提头与结尾边框图
	bool	m_bFollowCursor;		//是否跟踪鼠标指针的位置，一起移动，（同时没有横宽最短限制）
	bool	m_LAlign;
	BOOL	m_bShow;
public:
	KUiImageRef	 m_HoverImage;
};

extern KMouseOver	g_MouseOver;

void SetHoverObjDescColor(UINT uColor);
void SetMouseHoverObjectDesc(void* pWnd, INT nObj, UINT uGenre,
			UINT uId, INT nContainer, INT x, INT y, bool LAlign = false);
//绘制被拖动游戏对象的函数
INT DrawDraggingGameObjFunc(INT x, INT y, const KUiDraggedObject& Obj, INT nDropQueryResult);
