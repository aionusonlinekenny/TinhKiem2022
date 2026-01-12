#pragma once
#include "../Elem/WndShowAnimate.h"

class KUiDaTau : protected KWndShowAnimate
{
public:
	static KUiDaTau* OpenWindow(INT uParam);
	static void			 CloseWindow();
private:

	void	Initialize();
	void	LoadScheme(const char* pScheme);
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);
	void	ShowWindow(INT nNum);
private:
	static KUiDaTau* m_pSelf;
	void	UpdateNotice();
private:
	KWndButton	m_ExpBtn, m_RandomBtn, m_MoneyBtn, m_PointBtn, m_LuckyBtn, m_ItemBtn;
	char			m_szLoginBg[32];
	KWndText256		m_Notice;
};