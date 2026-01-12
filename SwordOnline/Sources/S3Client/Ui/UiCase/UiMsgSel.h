#ifndef __UiMsgSel_H__
#define __UiMsgSel_H__

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndText.h"

struct KUiQuestionAndAnswer;
struct KUiImage;

class KUiMsgSel : protected KWndShowAnimate
{
public:
	//----界面面板统一的接口函数----
	static KUiMsgSel*	OpenWindow(KUiQuestionAndAnswer* pQAA, KUiImage *pPic = NULL);		//打开窗口，返回唯一的一个类对象实例
	static KUiMsgSel*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//载入界面方案
	static void			CloseWindow(bool bDestroy);		//关闭窗口
	static void			OnClickMsg(INT nMsg);			//响应点击消息
private:
	KUiMsgSel();
	~KUiMsgSel();
	void	Show(KUiQuestionAndAnswer* pQAA);
	INT		Initialize(KUiImage *pPic);								//初始化
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);
	void	ChangeCurSel(bool bNext);
	virtual void	Breathe();


private:
	static KUiMsgSel*	m_pSelf;

	KScrollMessageListBox	m_MsgScrollList;	//备选文字和滚动条
	KWndScrollBar			m_MessageScroll;	//备选文字和滚动条
	KWndMessageListBox		m_MessageList;	//说明文字
	KWndImage				m_Image;

	//为自动滚动加的变量
	UINT	m_uLeftTime;
	bool m_bAutoUp;
	bool m_bAutoDown;
	UINT	m_uLastScrollTime;
	KUiImage		m_pPic;
	char m_szPic[MAX_ANSWERNUM][MAX_PATH];
};


#endif // __UiMsgSel_H__
