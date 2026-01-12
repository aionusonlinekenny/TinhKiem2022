/*****************************************************************************************
*****************************************************************************************/
#pragma once

struct KPopupMenuItem
{
	char			szData[64];
	UINT	uDataLen;
	UINT	uBgColor;
	uint16_t	uID;
};

#define MENU_DEFAULT_POS				-607
#define	MENU_ITEM_DEFAULT_WIDTH			0
#define	MENU_ITEM_DEFAULT_HEIGHT		0
#define	MENU_ITEM_DEFAULT_RIGHT_WIDTH	-1
#define MENU_ITEM_DEFAULT_INDENT		255
#define MENU_ITEM_DEFAULT_TITLEUPSPACE	1

enum POPUPMENU_FLAG
{
	PM_F_HAVE_HEAD_TAIL_IMG		= 0x0001,
	PM_F_ZOOM_SELECTED_ITEM		= 0x0002,
	PM_F_TAB_SPLIT_ITEM_TEXT	= 0x0004,
	PM_F_HAVE_ITEM_SEPARATOR	= 0x0008,
	PM_F_AUTO_DEL_WHEN_HIDE		= 0x0010,
	PM_F_CANCEL_BY_CALLER		= 0x0020,
};

struct KPopupMenuData
{
	int16_t			nX;
	int16_t			nY;
	int16_t			nXX;
	int16_t			nYY;

	int16_t			nItemWidth;
	int16_t			nItemRightWidth;
	int16_t			nItemHeight;
	int16_t			nNumItem;
	int16_t			nSelectedItem;
	uint16_t	usMenuFlag;

	unsigned char	byFontSize;
	unsigned char	byItemTitleUpSpace;
	int16_t			nItemTitleIndent;

//	bool			bZoomSelectedItem;
//	bool			bHaveHeadTailImg;
//	bool			bHaveSeparator;
//	bool			bAutoDelete;
//	bool			bManualCancel;

	UINT	uBorderLineColor;
	UINT	uSelItemBgColor;
	UINT	uTextColor;
	UINT	uTextBorderColor;
	UINT	uSelTextColor;
	UINT	uSelTextBorderColor;

	KPopupMenuItem	Items[1];
};

#define	MENU_DATA_SIZE(nNumItem)	(sizeof(KPopupMenuData) + (nNumItem - 1) * sizeof(KPopupMenuItem))

class KWndWindow;
class KPopupMenu
{
public:
	static void	Popup(KPopupMenuData* pMenu, KWndWindow* pCaller, UINT uParam);
	static void Cancel();
	static void	PaintMenu();
	static void	LoadTheme(const char* pScheme);
	static INT	HandleInput(UINT uMsg, UINT uParam, INT nParam);//´°¿Úº¯Êý
	static void OnWndDelete(KWndWindow* pWnd);
	static void	InitMenuData(KPopupMenuData* pMenu, INT nNumItem);
	static KPopupMenuData*	GetMenuData() {	return m_pMenu;}
private:
	static bool SelectItem(INT x, INT y, bool bApply, bool bRight = false);
	static bool SelectItem(INT nIndex, bool bApply);
private:
	static KPopupMenuData*  m_pMenu;
	static KWndWindow*		m_pCaller;
	static UINT		m_uCallerParam;
	
	static int16_t			m_nImgWidth;
	static int16_t			m_nImgHeight;
	static int16_t			m_nIndent;
    static int16_t			m_nFrame;
	static INT				m_nMenuHeight;
	static char				m_szImage[128];

	static UINT	m_uDefBorderLineColor;
	static UINT	m_uDefItemBgColor;
	static UINT	m_uDefSelItemBgColor;
	static UINT	m_uDefTextColor;
	static UINT	m_uDefTextBorderColor;
	static UINT	m_uDefSelTextColor;
	static UINT	m_uDefSelTextBorderColor;
};
