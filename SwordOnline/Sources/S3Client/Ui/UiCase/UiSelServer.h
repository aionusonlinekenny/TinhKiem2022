/*****************************************************************************************
//	??--?????????
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-12
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndShowAnimate.h"

class KUiSelServer : protected KWndShowAnimate
{
public:
	static KUiSelServer* OpenWindow();				//????,????????????
	static void			 CloseWindow(bool bDestroy);//????
private:
	KUiSelServer();
	~KUiSelServer();
	void	Initialize();						//???
	void	LoadScheme(const char* pScheme);	//??????
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);	//????
	void	OnLogin();
	void	OnCancel();
	INT		OnKeyDown(UINT uKey);
	void	OnClickButton(KWndButton* pBtn);
	void	GetList();

private:
	static KUiSelServer* m_pSelf;
private:
	struct KLoginServer* m_pServList;

	KWndList		m_List;
	KWndScrollBar	m_ScrollBar;
	KWndButton		m_Login;
	KWndButton		m_Cancel;
	char			m_szLoginBg[32];

	bool			m_bSelRegion;
	INT				m_nRegionIndex;
};
