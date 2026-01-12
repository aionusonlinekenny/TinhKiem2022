/*****************************************************************************************
//	界面--消息中心面板
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-13
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndPureTextBtn.h"
INT VerticalSplitTwoWindow(KWndWindow* pLeft, KWndWindow* pRight, INT nAt);

struct KUiMsgParam;

#define MAX_CHANNELRESOURCE 9

enum CHANNELRESOURCE
{
	CH_NEARBY,
	CH_TEAM,
	CH_WORLD,
	CH_FACTION,
	CH_SYSTEM,
	CH_CITY,
	CH_TONG,
	CH_MSGR,
	CH_CHATROOM,
};

enum CHANNELTABBUTTON
{
	CHANNELTAB_PUBLIC = 0,
	CHANNELTAB_PRIVATE,
	CHANNELTAB_CHATROOM,
	CHANNELTAB_TONG,
	CHANNELTAB_FACTION,
	CHANNELTAB_SYSTEM,
	CHANNELTAB_NUM,
};

struct KChannelResourceInfo
{
	char cTitle[32];
	char cShortTitle[MAX_CHANNELRESOURCE][32];
	KRColor uTextColor;
	KRColor uTextBorderColor;
	WORD nMenuPicIndex;
	WORD nMenuPicHeight;
	char cMenuText[32];
	WORD nMenuDeactivatePicIndex;
	WORD nMenuDeactivatePicHeight;
	char cMenuDeactivateText[32];
	KRColor uMenuBkColor;
	WORD nTextPicIndex;
	char cFormatName[32];	//用于频道订阅
	INT nNeverClose;
	char szSoundFileName[80];
	UINT uSendMsgInterval;	//用于发送消息的时间间隔(毫秒)
	UINT uSendMsgNum;	//用于缓存待发送消息最大数目,最大10句
};

struct KChannelActivateInfo
{
	DWORD nChannelID;
	BYTE cost;
	bool bSubscribe;
	INT ResourceIndex;	//-1表示无资源
	UINT uLastSendMsgTime;	//上次发消息的时间
	UINT uLeftSendMsgNum;	//已经缓存的代发消息
	UINT uBufferOffset;	//可用空间的起始位置
	char Buffer[1024 * 10];
};

struct KMSN_ChannelInfo
{
	KRColor uTextColorSelf;
	KRColor uTextBorderColorSelf;
	WORD nSelfTextPicIndex;
	KRColor uTextBKColorSelf;

	KRColor uTextFriendColor;
	KRColor uTextBorderFriendColor;
	WORD nFriendMenuPicIndex;
	WORD nFriendMenuPicHeight;
	KRColor uFriendMenuBkColor;
	WORD nFriendTextPicIndex;
	char szFriendSoundFileName[80];

	KRColor uTextColorUnknown;
	KRColor uTextBorderColorUnknown;
	WORD nStrangerMenuPicIndex;
	WORD nStrangerMenuPicHeight;
	KRColor uStrangerMenuBkColor;
	WORD nStrangerTextPicIndex;
};

struct KPlayer_Chat_Tab
{
	char	szChatTabName[32];
	INT		nId;
};

class KSysMsgCentrePad : public KWndWindow
{
public:
	KWndButton				m_OpenSysButton;
	KWndButton				m_UpButton;
	KWndButton				m_DownButton;

	KWndMessageListBox		m_SysRoom;
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	void ScrollBottom();
};

class KUiMsgCentrePad : public KWndWindow
{
public:
	//----界面面板统一的接口函数----
	static KUiMsgCentrePad* OpenWindow();					//打开窗口，返回唯一的一个类对象实例
	static void				CloseWindow(bool bDestroy);		//关闭窗口，同时可以选则是否删除对象实例
	static KUiMsgCentrePad* GetIfVisible();
	static void				Clear();
	static void				Expand();
	static void				LoadScheme(const char* pScheme);//载入界面方案
	static void				DefaultScheme(const char* pScheme);//重新初始化界面
	static void				HideAllMessage();//隐藏所有输出
	static void				ShowAllMessage();//显示所有输出

	static void				SystemMessageArrival(const char* pMsgBuff, uint16_t nMsgLength);
	//用于新聊天协议到达的处理
	static INT				NewChannelMessageArrival(DWORD nChannelID, char* szSendName, const char* pMsgBuff, uint16_t nMsgLength, const char* pItem = 0, BYTE btSomeFlag = 0U);
	static void				NewMSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, uint16_t nMsgLength, const char* pItem = 0);
	static void				OpenChannel(char* channelName, DWORD nChannelID, BYTE cost);	//同时订阅
	//用于外挂的聊天
	static void				ShowSomeoneMessage(char* szSourceName, const char* pMsgBuff, uint16_t nMsgLength);
	enum  SelfChannel
	{
		ch_Team = 0,
		ch_Faction,
		ch_Tong,
		ch_Msgr,
		ch_Cr,
		ch_Screen,
		ch_GM
	};
	static void				CloseSelfChannel(SelfChannel type);	//同时退订

	static void				QueryAllChannel();
	static void				ReplaceChannelName(char* szDest, size_t nDestSize, char* szSrc);
	static bool				GetChannelMenuinfo(INT nChannelIndex, WORD* pnPicIndex = NULL, WORD* pPicHei = NULL, KRColor* puTextColor = NULL, KRColor* puBkColor = NULL, char* pszMenuText = NULL, int16_t* pnCheckPicIndex = NULL);
	static bool				GetPeopleMenuinfo(char* szDest, WORD* pnPicIndex = NULL, WORD* pPicHei = NULL, KRColor* puTextColor = NULL, KRColor* puBkColor = NULL);
	static bool				ReplaceSpecialField(char* szDest, char* szSrc);
	static DWORD			GetChannelID(INT nChannelIndex);
	static char* GetChannelTitle(INT nChannelIndex);
	static DWORD			GetChannelCount();
	static INT				GetChannelIndex(char* channelName);
	static INT				GetChannelIndex(DWORD dwID);
	static INT				ReleaseActivateChannelAll();	//返回原来的总个数
	static bool				GetChannelSubscribe(INT nChannelIndex);
	static bool				IsChannelType(INT nChannelIndex, SelfChannel type);
	static INT				CheckChannel(INT nChannelIndex, bool b);
	static KUiMsgCentrePad* GetSelf() { return m_pSelf; }
	static BOOL GetSelfPosition(INT* pnX, INT* pnY);
	static BOOL GetSelfSize(INT* pnW, INT* pnH);
	static void SetFontSize(INT nFontSize);
	static INT GetFontSize();
	static INT SetChannelTextColor(char* cTitle, DWORD uTextColor, DWORD uTextBorderColor);
	static char* GetChannelTextColor(INT nIndex, DWORD& uTextColor, DWORD& uTextBorderColor);
	static void SetMSNTextColor(INT nType, DWORD uTextColor, DWORD uTextBorderColor);	//nType 0 is me, 1 is friend, 2 is stranger
	static void GetMSNTextColor(INT nType, DWORD& uTextColor, DWORD& uTextBorderColor);
	static void	SetReply(const char* Buff, INT nLen);
	static void ClearReply();
	static BYTE	GetChannelCost(DWORD dwID);
	static INT	PushChannelData(DWORD dwID, const char* Buffer, INT nLen);
	static INT	GetChannelData(DWORD& dwID, BYTE& cost, char*& Buffer, INT& nLen);
	static INT	PopChannelData(DWORD dwID);

	void	Breathe();								//活动函数
private:
	KUiMsgCentrePad();
	~KUiMsgCentrePad()
	{
		ReleaseActivateChannelAll();
	}
	void	Initialize();							//初始化
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	INT		ChanageHeight(INT nOffsetY, bool bAtTop);
	void	LoadScheme(KIniFile* pIni);
	INT		PtInWindow(INT x, INT y);				//判断一个点是否落在窗口内,传入的是绝对坐标
	void	SetAutoDelMsgInterval(UINT uInterval = 0);	//设置自动删除消息的时间间隔
	//参数默认值为0，表示不按照时间间隔自动删除，时间单位毫秒
	void	DragWndSize(INT nMoveOffset);
	void	DragWndPos(INT nMoveOffset);
	void	PaintWindow();							//绘制窗口

	INT		FindActivateChannelResourceIndex(char* cTitle);
	INT		FindActivateChannelIndex(char* cTitle);
	INT		FindActivateChannelIndex(DWORD nChannelID);
	BYTE	FindActivateChannelCost(DWORD nChannelID);
	INT		FindChannelResourceIndex(char* cTitle);
	INT		FindActivateChannelIndexByKey(char* cKey);
	INT		IsNeverCloseActivateChannel(INT nChannelIndex);
	void	ChannelMessageArrival(INT nChannelIndex, char* szSendName, const char* pMsgBuff, uint16_t nMsgLength, KWndMessageListBox* pM, bool bName, const char* pItem = 0, BYTE btSomeFlag = 0);
	void	MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, uint16_t nMsgLength, KWndMessageListBox* pM, const char* pItem = 0);
	void	ShowMSNMessage(char* szName, const char* pMsgBuff, uint16_t nMsgLength, KWndMessageListBox* pM, KRColor uColor, KRColor uBKColor, KRColor uBorderColor, WORD nPic, const char* pItem = 0);
	void	SendQueryChannelID(INT nChannelResourceIndex);
	void	SendChannelSubscribe(INT nChannelIndex, bool b);
	INT		AddActivateChannel(const KChannelActivateInfo& Item);	//返回增加Item的Index
	INT		ReleaseActivateChannel(INT nChannelIndex);	//返回剩余的总个数
	void	CloseActivateChannel(INT nChannelIndex);	//同时退订

	void	PopupChannelMenu(INT x, INT y);

	INT		FilterTextColor(char* pMsgBuff, uint16_t nMsgLength, const KRColor& uColor);

	INT		GetMessageSendName(KWndWindow* pWnd, INT nIndex, char* szCurText);
	INT		GetMessageSendName(KWndWindow* pWnd, INT x, INT y, char* szCurText);
	void	ShowChatTab(KIniFile* pSetting);
public:
	KWndImage			m_SizeBtn;
private:
	static KUiMsgCentrePad* m_pSelf;

	KWndImage			m_MoveImg;
	KWndButton          m_BgShadowBtn;

	KWndButton		m_TabAllBtn;
	KWndButton		m_TabSecrectBtn;
	KWndButton		m_TabTeamBtn;
	KWndButton		m_TabFactionBtn;
	KWndButton		m_TabTongBtn;
	KWndButton		m_TabOtherBtn;

	KWndPureTextBtn		m_TabButton[CHANNELTAB_NUM];
	KPlayer_Chat_Tab	m_ChatTab[CHANNELTAB_NUM];
	KSysMsgCentrePad	m_Sys;

	INT					m_nMinTopPos, m_nMaxBottomPos;
	bool				m_bSizingWnd;
	bool                m_bShowShadow;

	int16_t				m_nSizeBtnDragOffsetY;

	bool				m_bSizeUp;
	bool				m_bExpandWnd;
	RECT				m_nLastWndSize;

#define SECOND_AUTODELMSG 20000
	UINT	m_uAutoDelMsgInterval;	//自动删除消息的间隔时间，单位毫秒
	UINT	m_uLastDelMsgTime;		//上次删除消息时的时间


	KScrollMessageListBox m_MessageChannel[CHANNELTAB_NUM];
	INT	m_nCurChannelSel;

	KChannelResourceInfo m_ChannelsResource[MAX_CHANNELRESOURCE];
	INT m_nChannelsResource;
	char m_DefaultChannelSendName[32];
	INT m_nDefaultChannelResource;
	KChannelActivateInfo* m_pActivateChannel;
	INT m_nActivateChannels;
	KMSN_ChannelInfo m_MSNInfo;
	int16_t m_nCheckOnPicIndex;
	int16_t m_nCheckOffPicIndex;

	KRColor m_NameColor;
	KRColor m_BorderNameColor;

	static INT ms_DefaultHeight, ms_DefaultWidth;

	char m_szReply[128];
};
