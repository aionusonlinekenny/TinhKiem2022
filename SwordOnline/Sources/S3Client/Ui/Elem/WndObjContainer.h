/*****************************************************************************************
//	界面窗口体系结构--容纳游戏对象的窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-25
*****************************************************************************************/
#pragma once

#include "Windows.h"
#include "WndWindow.h"
#include "../Elem/WndMessage.h"
#include <mutex>
#include <cstdint>

#define	OBJCONT_S_ENABLE_CLICK_EMPTY	0x00008000
#define	OBJCONT_S_ACCEPT_FREE			0x00004000
#define	OBJCONT_S_HAVEOBJBGCOLOR		0x00002000	//放有物品时是否有背景色
#define	OBJCONT_S_TRACE_PUT_POS			0x00001000
#define	OBJCONT_S_DISABLE_PICKPUT		0x00000800	//不允许拿起东西
#define	OBJCONT_F_MOUSE_HOVER			0x00000400
#define	OBJCONT_F_DISABLE_MOUSE_HOVER	0x00000200	//不允许拿起东西

//============================
//	单个对象的容器窗口
//============================
class KWndObjectBox : public KWndWindow
{
public:
	KWndObjectBox();
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);	//初始化
	void	LoadScheme(const char* pScheme);			///载入界面方案
	void	Celar();									//清除对象物品
	void	SetObjectGenre(UINT uGenre);		//设置可以容纳的对象的类型
	INT		GetObject(KUiDraggedObject& Obj) const;		//获取容纳的对象信息
	void	HoldObject(UINT uGenre, UINT uId, INT nDataW, INT nDataH);//设置容纳的对象
	void	Clone(KWndObjectBox* pCopy);
	void	SetContainerId(INT nId);
	void	EnablePickPut(bool bEnable);
	void	EnableMouseHover(bool bEnable);

protected:
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	INT		DropObject(bool bTestOnly);				//放置物品
	void	PaintWindow();							//窗体绘制
	UINT		m_uAcceptableGenre;			//可接纳的对象类型
	KUiDraggedObject	m_Object;
	INT					m_nContainerId;

	// Khai b醥 mutex
	std::mutex mtx;
};

//============================
//	容纳多个对象的容器窗口
//============================
class KWndObjectMatrix : public KWndWindow
{
public:
	KWndObjectMatrix();
	virtual ~KWndObjectMatrix();
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);	//初始化
	void			Clear();									//清除全部的对象物品
	INT				AddObject(KUiDraggedObject* pObject, INT nCount);	//增加对象物品
	INT				RemoveObject(KUiDraggedObject* pOjbect);			//减少一个对象物品
	INT				GetObject(KUiDraggedObject& Obj, INT x, INT y) const;//获取容纳的某个对象信息
	//	INT				GetObjects(KUiDraggedObject* pObjects, INT nCount) const;//获取容纳的对象信息
	void			EnableTracePutPos(bool bEnable);
	void			SetContainerId(INT nId);
	void			EnablePickPut(bool bEnable);
	void			EnableMouseHover(bool bEnable);
	INT				GetObjectNum();
	void			GetObjectById(KUiDraggedObject& Obj, INT id);
	KUiDraggedObject* GetObjectByIndex(INT nIndex);

protected:
	void			Clone(KWndObjectMatrix* pCopy);
private:
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);	//窗口函数
	void	PaintWindow();										//窗体绘制
	INT		GetObjectAt(INT x, INT y);							//获得某个位置上的物品的索引
	INT		PickUpObjectAt(INT x, INT y);						//捡起某个位置上的对象
	INT		DropObject(INT x, INT y, bool bTestOnly);			//放置物品
	INT		TryDropObjAtPos(const RECT& dor, KUiDraggedObject*& pOverlaped);//尝试放置物品
	void	DropObject(INT x, INT y, KUiDraggedObject* pToPickUpObj);		//放下物品

protected:
	INT				m_nNumUnitHori;		//横向格数
	INT				m_nNUmUnitVert;		//纵向格数
	INT				m_nUnitWidth;		//横向格宽
	INT				m_nUnitBorder;		//格子的边框的宽高度
	INT				m_nUnitHeight;		//纵向格宽
	INT				m_nNumObjects;		//容纳的对象的数目9
	KUiDraggedObject* m_pObjects;		//容纳的对象列表
	INT				m_nMouseOverObj;

	INT				m_nPutPosX;
	INT				m_nPutPosY;
	INT				m_nPutWidth;
	INT				m_nPutHeight;
	INT				m_nContainerId;

	// Khai b醥 mutex
	std::mutex mtx;
};

void WndObjContainerInit(KIniFile* pIni);