/*****************************************************************************************
//	界面--聊天主界面
//	Author	:   
//	CreateTime:	2015-7-29
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndList.h"


struct OneRoomInfo
{
	char	szName[10];
	INT		nPass;
};

class KUiChatRoom : public KWndShowAnimate
{
public:
	//----界面面板统一的接口函数----
	static KUiChatRoom*	OpenWindow();		//打开窗口，返回唯一的一个类对象实例
	static KUiChatRoom*	GetIfVisible();		//如果窗口正被显示，则返回实例指针
	static void				CloseWindow(bool bDestroy);	//关闭窗口，同时可以选则是否删除对象实例
	static void				LoadScheme(const char* pScheme);//载入界面方案	
	static void				ServerRequestRoomList(KOneRoom *pRoom);
	static void				ServerRequestMemberList(KOneMem *pMem);
	static void				ServerRequestBlackList(KOneMem *pMem);
	static void				UpdateInterface();
	static void				Join(const char* lpszName, INT nFigure);
	static void				Leave();
private:

	KUiChatRoom();
	~KUiChatRoom();
	void	Initialize();							//初始化
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	void	Breathe();
private:
	static KUiChatRoom* m_pSelf;
	
	void	OnClickButton(KWndWindow* pBtn);	
	
	KWndButton	m_BtnClose;
	KWndButton	m_BtnExitRoom;
	KWndButton	m_BtnEnterRoom;
	KWndButton	m_BtnDelRoom;
	KWndButton	m_BtnOpenDice;
	KWndButton	m_BtnHightDice;
	KWndButton	m_BtnLowDice;
	KWndButton	m_BtnPassword;
	KWndText80	m_TxtCurRoomTitle;

	KScrollMessageListBox	m_RoomList;	//说明文字
	KScrollMessageListBox	m_MemberList;	//说明文字
	KScrollMessageListBox	m_BlackList;	//说明文字
	
	OneRoomInfo	Room;
	INT			m_nRoomNameLen;
	INT			m_nCreaterLen;
	INT			m_nTimeLen;
	INT			m_nMemNumLen;
	INT			m_nMemNameLen;
	INT			m_nRoomFigure;
	INT			m_nRoomDiceTime;
	INT			m_nRoomDiceRet;
	BOOL		m_bIsInRoom;
	UINT	m_uLastUpdateTime;
	char		m_szRoomName[32];
	UINT		m_uKeyPicIndex, m_uDicePicIndex;
};
