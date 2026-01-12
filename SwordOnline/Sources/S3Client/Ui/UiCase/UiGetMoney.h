// -------------------------------------------------------------------------
//	文件名		：	UiGetMoney.h
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-1-7
//	功能描述	：	取钱界面
// -------------------------------------------------------------------------
#ifndef __UiGetMoney_H__
#define __UiGetMoney_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"


class KUiGetMoney : protected KWndImage
{
public:
	//----界面面板统一的接口函数----
	static KUiGetMoney*	OpenWindow(INT nMoney, INT nMaxMoney, KWndWindow* pRequester,
							 UINT uParam, KWndWindow* pMoneyWnd);	//打开窗口，返回唯一的一个类对象实例
	static KUiGetMoney*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//载入界面方案
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiGetMoney();
	~KUiGetMoney() {}
	INT		Initialize();								//初始化
	void	Show(KWndWindow* pMoneyWnd);
	void	Hide();
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);
	void	OnCancel();
	void	OnOk();
	void	OnCheckInput();
private:
	static KUiGetMoney*	m_pSelf;
	KWndEdit32			m_Money;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;
	INT					m_nMaxMoney;
	KWndWindow*			m_pRequester;
	UINT		m_uRequesterParam;
};


#endif // __UiGetMoney_H__