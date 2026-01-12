/*****************************************************************************************
//	界面--选择出生地窗口
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-6-7
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndList.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"
#include "../../Login/LoginDef.h"
#include "../../Login/Login.h"

struct KNativePlace
{
	char			szName[32];
	char			szImage[128];
	uint16_t	nId;
	uint16_t	nDescLen;
	char			sDesc[MAX_MESSAGE_LENGTH];
};

class KUiSelNativePlace : protected KWndShowAnimate
{
public:
	static KUiSelNativePlace* OpenWindow(INT nPlaceId = -1);//打开窗口，返回唯一的一个类对象实例
	static void				 CloseWindow(bool bDestroy);	//关闭窗口
private:
	KUiSelNativePlace();
	~KUiSelNativePlace();
	void	Initialize();						//初始化
	void	LoadScheme(const char* pScheme);	//载入界面方案
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);	//窗口函数
	void	OnOk();
	void	OnCancel();
private:
	void	LoadList(INT nPlaceId);
	void	UpdateData();
	static KUiSelNativePlace* m_pSelf;
private:
	KNativePlace* m_pPlaceList;
	KWndList		m_List;
	KWndImage		m_PlaceImg;
	KWndButton		m_Ok;
	KWndButton		m_Cancel;
	KWndText256		m_PlaceDesc;
	char			m_szLoginBg[32];
	INT				m_nLastSelPlace;
};
