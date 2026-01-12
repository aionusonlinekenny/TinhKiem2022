/*****************************************************************************************
//	界面--游戏显示玩家信息主界面
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-16
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndImage.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndText.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndShadow.h"
#include "../Elem/WndValueImage.h"
#include "../Elem/WndObjContainer.h"
#include "../Elem/WndLabeledButton.h"

#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../Core/src/GameDataDef.h"
#include "../../../Engine/src/KIniFile.h"
#include "UiMsgCentrePad.h"

struct CHAT_ITEM
{
	char szName[FILE_NAME_LENGTH];
	INT	nNameLen;
	char pItem[MAX_SENTENCE_LENGTH];

	void Clear() {
		szName[0] = 0;
		nNameLen = 0;
		pItem[0] = 0;
	};
};

#include "../Elem/ComWindow.h"

class GameWorld_DateTime : public KWndLabeledButton
{
public:
	DECLARE_COMCLASS(GameWorld_DateTime)
	GameWorld_DateTime();
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);	//初始化
	void            UpdateData();

private:
	INT             m_nSmoothPing, m_nCrowdPing, m_nBlockPing;
	bool            m_bNetStatus;
	bool			m_bRegionName;
	bool			m_bServerName;
	bool			m_bLogo;
	char            m_szRegionName[32], m_szServerName[32], m_szSmoothMsg[32], m_szCrowdMsg[32], m_szBlockMsg[32];
	UINT	m_uLastSwitchTime, m_uDefaultColor, m_uSmoothColor, m_uCrowdColor, m_uBlockColor;

private:
	void OnButtonClick();
	void UpdateNetStatus();
};

class KImmediaItem : public KWndObjectBox
{
public:
	INT m_nIndex, m_nOrder;
	DWORD m_TextColor;
	KImmediaItem() :m_nIndex(-1), m_nOrder(0), m_TextColor(0xFFFFFF00) {}
	void PaintWindow();
	INT	Init(KIniFile* pIniFile, const char* pSection);
};

struct KUiDraggedObject;
struct KPopupMenuData;


class KUiPlayerBar : protected KWndImage
{
public:
	//----界面面板统一的接口函数----
	static KUiPlayerBar* OpenWindow();					//打开窗口，返回唯一的一个类对象实例
	static KUiPlayerBar* GetIfVisible();				//如果窗口正被显示，则返回实例指针
	static void			CloseWindow(bool bDestroy);		//关闭窗口，同时可以选则是否删除对象实例
	static void			LoadScheme(const char* pScheme);//载入界面方案

	static void			InputNameMsg(char bChannel, const char* szName, bool bFocus);
	static INT			FindRecentPlayer(const char* szName);
	static INT          AddRecentPlayer(const char* szName);
	static char* GetRecentPlayerName(INT nIndex);//根据index取得某密聊频道的目标玩家名

	static const char* SelfName()
	{
		if (m_pSelf)
			return m_pSelf->m_szSelfName;
		return NULL;
	}
	static BOOL	IsSelfName(char* szName)
	{
		if (m_pSelf && szName)
		{
			return (strcmpi(m_pSelf->m_szSelfName, szName) == 0);
		}
		return FALSE;
	}
	static void	OnUseItem(INT nIndex);				//使用快捷物品
	static BOOL	IsCanSendMessage(const char* Buffer, INT nLen, char* szDestName, INT nChannelID);				//能发送吗?
	static void	OnSendChannelMessage(DWORD nChannelID, const char* Buffer, INT nLen);				//发送频道聊天到服务器
	static void	OnSendSomeoneMessage(const char* Name, const char* Buffer, INT nLen);				//发送someone聊天到服务器

	void	UpdateItem(INT nIndex, UINT uGenre, UINT uId);	//变更物品摆换
	void	UpdateSkill(INT nIndex, UINT uGenre, UINT uId);	//变更快捷技能

	static	bool		GetExp(INT& nFull, INT& nCurrLevelExp, INT& nCurrentExp);

	static BOOL			LoadPrivateSetting(KIniFile* pFile);
	static INT			SavePrivateSetting(KIniFile* pFile);

	static void         SetCurrentChannel(INT nIndex);
	static void         SwitchChannel(BOOL bUp = FALSE);  //切换到下一个频道，到底了就切换到第一个，循环
	//bUp是true就上升，否则下降
	//e...下标是0是最顶部的概念
	static INT          GetCurChannel();                  //得到PlayerBar里当前的频道
	BOOL				TextMsgFilterItem();
	static void			InputItemMsg(UINT uId);

private:
	KUiPlayerBar();
	~KUiPlayerBar();
	void	Breathe();									//活动函数
	void	Initialize();								//初始化
	void	PaintWindow();

	void	UpdateData();								//更新数据
	void	UpdateXXXNumber(INT& nMana, INT& nFullMana);							//更新常变的那些数值数据
	void	UpdateRuntimeAttribute(INT& nMoney, INT& nLevel);

	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	void	OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);//响应界面操作取起放下东西
	void	LoadScheme(KIniFile* pIni);//载入界面方案

	void	OnSend(BOOL bDirectSend);
	void	OnDirectSendChannelMessage(DWORD nChannelID, BYTE cost, const char* Buffer, INT nLen);				//发送频道聊天到服务器
	void	PopupChannelMenu(INT x, INT y);
	void	PopupPhraseMenu(INT x, INT y, bool bFirstItem);
	void	InputRecentMsg(bool bPrior);
	void	OnSwitchSize();			//切换大小

	INT 	GetChannelIndex(const char* pTitle);
	//void SetCurrentChannel(INT nIndex);
	void 	InputCurrentChannel(INT nIndex, bool bFocus);
	//返回在Channel中的索引, < 0 失败
	//INT AddRecentPlayer(const char* szName);
	void 	ReplaceSpecialName(char* szDest, size_t nDestSize, char* szSrc);

	INT 	IsHasCost(BYTE cost, INT nMoney, INT nLevel, INT nMana, INT nFullMana, INT& nUseMoney, INT& nUseLevel, INT& nUseMana);
private:
	static KUiPlayerBar* m_pSelf;
private:
	KWndButton		m_Face, m_EscDlg, m_Friend, m_Market;		//表情
	KWndImage		m_InputBack;

	GameWorld_DateTime		m_DateTime;		//时间
#define	UPB_IMMEDIA_ITEM_COUNT	9
	KImmediaItem	m_ImmediaItem[UPB_IMMEDIA_ITEM_COUNT];
	KWndObjectBox	m_ImmediaSkill[2];


	//KWndImage		m_ImmediaFour;
	KWndImage		m_ToolsMiniBar;
	KWndButton		m_Icoin;
	KWndButton		m_UnlockBtn;
	KWndButton		m_AutoPlay;
	KWndButton		m_Support;

	KWndButton	m_SendBtn;	//发送
	KWndEdit512	m_InputEdit;	//输入框
	KWndPureTextBtn	m_ChannelSwitchBtn;	//切换频道
	KWndButton	m_ChannelOpenBtn;	//频道打开

private:
	KWndButton	m_SwitchBtn;	//切换大小

	char		m_cPreMsgCounter;
	char		m_cLatestMsgIndex;
#define	MAX_RECENT_MSG_COUNT	8
	char		m_RecentMsg[MAX_RECENT_MSG_COUNT][MAX_SENTENCE_LENGTH];
	CHAT_ITEM	m_RecentItem[MAX_RECENT_MSG_COUNT];
	INT m_nCurChannel;
#define	MAX_RECENTPLAYER_COUNT	10
	char m_RecentPlayerName[MAX_RECENTPLAYER_COUNT][32];
	INT m_nRecentPlayerName;

	INT				m_bMiniMode;

	char			m_szSelfName[32];	//自己的名字

	INT				m_nExperienceFull;
	INT				m_nCurLevelExperience;
	INT				m_nExperience;

	CHAT_ITEM		m_pItem;
};
