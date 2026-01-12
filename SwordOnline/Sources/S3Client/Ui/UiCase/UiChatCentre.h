/*****************************************************************************************
//	界面--聊天主界面
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-3-10
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndList.h"
#include "../Elem/PopupMenu.h"
#include "../Elem/WndPureTextBtn.h"

#pragma warning( disable : 4786 4800 4355 4146 4273 4503 )

#include <map>
#include <string>
#include <list>

using namespace std;

#define	MAX_UNIT_NUM	10
#define	MAX_GROUP_NUM	10		//最多允许的好友分组的数目

#define DEFAULT_GROUPNAME	"Nh鉳 "

enum UICHATCENTRE_UPDATE
{
	UICC_U_ALL,
	UICC_U_GROUP,
	UICC_U_FRIEND,
};

typedef std::list<std::string>	STRINGLIST;

struct FriendNotify
{
	virtual INT	CreateUnit(const char* Unit) = 0;
	virtual INT	DeleteUnit(const char* Unit) = 0;
	virtual INT	CreateUnitGroup(const char* Unit, const char* Name) = 0;
	virtual INT	RenameUnitGroup(const char* Unit, const char* Name, const char* NewName, const STRINGLIST& friends) = 0;
	virtual INT	DeleteUnitGroup(const char* Unit, const char* Name, const STRINGLIST& friends) = 0;
	virtual INT	MoveUnitGroup(const char* Unit, const char* Name, const char* Name2, const STRINGLIST& friends) = 0;
	virtual INT	CreateUnitMember(const char* Unit, const char* Name, const char* Group) = 0;
	virtual INT	DeleteUnitMember(const char* Unit, const char* Name) = 0;
	virtual INT	MoveUnitMember(const char* Unit, const char* Name, const char* Group) = 0;
	virtual INT	SetUnitMemberStatus(const char* Unit, const char* Name, const char* status) = 0;
};

struct AddinNotify : public FriendNotify
{
	virtual INT	CreateUnit(const char* Unit);
	virtual INT	DeleteUnit(const char* Unit);
	virtual INT	CreateUnitGroup(const char* Unit, const char* Name);
	virtual INT	RenameUnitGroup(const char* Unit, const char* Name, const char* NewName, const STRINGLIST& friends);
	virtual INT	MoveUnitGroup(const char* Unit, const char* Name, const char* Name2, const STRINGLIST& friends);
	virtual INT	DeleteUnitGroup(const char* Unit, const char* Name, const STRINGLIST& friends);
	virtual INT	CreateUnitMember(const char* Unit, const char* Name, const char* Group);
	virtual INT	DeleteUnitMember(const char* Unit, const char* Name);
	virtual INT	MoveUnitMember(const char* Unit, const char* Name, const char* Group);
	virtual INT	SetUnitMemberStatus(const char* Unit, const char* Name, const char* status);
};

struct ServerNotify : public AddinNotify
{
	virtual INT	CreateUnit(const char* Unit);
	virtual INT	RenameUnitGroup(const char* Unit, const char* Name, const char* NewName, const STRINGLIST& friends);
	virtual INT	MoveUnitGroup(const char* Unit, const char* Name, const char* Name2, const STRINGLIST& friends);
	virtual INT	DeleteUnitGroup(const char* Unit, const char* Name, const STRINGLIST& friends);
	virtual INT	DeleteUnitMember(const char* Unit, const char* Name);
	virtual INT	MoveUnitMember(const char* Unit, const char* Name, const char* Group);

	void	SendSyncData(const char* Unit);
	void	SendNotifyGroupFriend(const char* Unit, const std::string& group, const STRINGLIST& friends);
	void	SendNotifyDeleteFriend(const char* Unit, const char* Name);
	INT m_nRef;
};

struct KUiPlayerItem;

class KUiChatCentre : public KWndShowAnimate
{
public:
	//----界面面板统一的接口函数----
	static KUiChatCentre* OpenWindow(bool bShow);		//打开窗口，返回唯一的一个类对象实例
	static KUiChatCentre* GetIfVisible();		//如果窗口正被显示，则返回实例指针
	static void				CloseWindow(bool bDestroy);	//关闭窗口，同时可以选则是否删除对象实例
	static void				LoadScheme(const char* pScheme);//载入界面方案	

	static void				CreateSeverUnit();
	static BOOL				LoadPrivateSetting(KIniFile* pFile);
	static INT				SavePrivateSetting(KIniFile* pFile);

	static void				UpdateData(UICHATCENTRE_UPDATE eFlag, UINT uParam, INT nGroupIndex);
	static bool				IsMyFriend(char* Name);

	static bool				AddFriendInfo(INT nUnitIndex, char* Name, char* group);
	static bool				FriendStatus(INT nUnitIndex, char* Name, BYTE state);
	static void				DeleteFriend(INT nUnitIndex, char* Name, bool bNotify);
	static void				NewGroup(INT nUnitIndex, const char* szGroup);
	static void				ReplaceGroupName(INT nUnitIndex, const char* sz, const char* szGroup);
	static void				MoveFriendToGroup(INT nUnitIndex, const char* Name, const char* group);
	static void				MoveGroupToGroup(INT nUnitIndex, const char* group, const char* group2, bool bDelgroup);

	static INT				FindUnitIndex(const char* unit);
	static INT				FindUnitIndexByRoleNameAtServerUnit(char* roleName);
	static INT				AddAddinUnit(const char* unit, FriendNotify* pNotify);
	static INT				RemoveAddinUnit(const char* unit);
	static INT				FindAddinUnitIndex(const char* unit);

private:

	KUiChatCentre();
	~KUiChatCentre() {}
	void	Initialize();							//初始化
	void	LoadScheme(class KIniFile* pIni);
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数

	void	CancelMenu();
	void	ClearInterface();
	void	ClearFriendList();
	void	ClearUnitData(INT nUnit);

	void	OnClickButton(KWndWindow* pBtn);
	void	OnSelGroupMenu(INT nGroupIndex, INT nCmd);
	void	OnSelFriendMenu(INT nFriendIndex, INT nCmd);
	void	OnMoveFriendToGroup(INT nFriendIndex, INT nNewGroupIndex);
	bool	OnClickGroupBtn(KWndWindow* pBtn, bool bLeftClick);
	void	OnScrollList(bool bUp);
	void	OnSelUnit(INT nUnitIndex);

	void	PopupupGroupMenu(INT nGroupIndex);
	void	PopupFriendMenu(INT nFriendIndex);
	void	PopupSelGroupMenu(INT nIndexInMenu);
	void	PopupSelUnitMenu();

	void	SendFriendMsg(INT nFriendIndex, bool bDBClick);

	void	UpdateAll(INT nGroupIndex);
	void	UpdateGroup(INT nGroupIndex);
	void	UpdateFriend(KUiPlayerItem* pFriend, INT nGroupIndex);
	void	UpdateInterface();

	void	PaintWindow();							//绘制窗口

	void	DragWndSize(INT nMoveOffset);
	void	AdjustSize(INT nNumLine);
	INT		PtInWindow(INT x, INT y);				//判断一个点是否在窗口范围内,传入的是绝对坐标

private:
	typedef std::map<std::string, std::string> FRIENDINFOLIST;
	typedef std::map<std::string, BYTE> FRIENDONLINELIST;
	struct GROUPINFO
	{
		std::string strGroupName;
		BYTE nGroupMemberNum;
	};

	struct FriendUnit
	{
		std::string m_UnitName;
		FRIENDINFOLIST m_FriendMap;
		FRIENDONLINELIST m_FriendOnlineMap;
		GROUPINFO	m_GroupInfo[MAX_GROUP_NUM];
		INT m_nCurExpandGroup;
		INT	FindGroupIndex(const std::string& str);
		FriendNotify* m_pNotify;
	};

	INT m_nCurentFriendUnit;
	FriendUnit m_AllFriendUnits[MAX_UNIT_NUM];
	INT m_nFriendUnitCount;

	static ServerNotify ms_SNotify;

	static KUiChatCentre* m_pSelf;
private:
	KWndImage			m_TailImg;
	KWndButton			m_FindBtn;
	KWndButton			m_CloseBtn;
	KWndButton			m_Invisible;
	KWndButton			m_ScrollUpBtn, m_ScrollDownBtn;
	KWndPureTextBtn			m_UnitBtnFriend;
	KWndPureTextBtn			m_UnitBtnEnemy;
	KWndPureTextBtn			m_UnitBtnBlackList;
	KWndPureTextBtn			m_UnitBtnOther;

	KWndLabeledButton	m_GroupBtn[MAX_GROUP_NUM];

	KWndList			m_FriendList;

	bool				m_bValidGroup[MAX_GROUP_NUM];
	INT					m_nNumValidGroup;
	bool				m_bPopupMenu;
	INT					m_nNumFriendInList;
	KUiPlayerItem* m_pFriendList;

	UINT		m_uFriendOffLineColor;
	INT					m_nMaxVisibleLine, m_nMinVisibleLine;
	INT					m_nLineHeight;

	INT					m_nFirstGroupBtnLeft, m_nFirstGroupBtnTop;
	INT					m_nFirstVisibleLine;
	INT					m_nNumVisibleLine;
	INT					m_nTailImgDragOffsetY;

	//好友列表背景图
	KRUImage			m_ListBgImage;
	INT					m_ListBgImgLeft;
	INT					m_ListBgImgTop;
	INT					m_ListBgImgWidth;
	INT					m_ListBgImgHeight;

#define	GROUP_MENU_ITEM_COUNT	3
#define	FRIEND_MENU_ITEM_COUNT	3
	//弹出菜单的内容
	char	m_GroupMenuData[MENU_DATA_SIZE(GROUP_MENU_ITEM_COUNT)];
	char	m_FriendMenuData[MENU_DATA_SIZE(FRIEND_MENU_ITEM_COUNT)];
	struct KPopupMenuData* m_pSelGrupMenu;
};