//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItem.h
// Date:	2002.08
// Code:	LiXueWu, DongBo
// Desc:	Header File
//---------------------------------------------------------------------------

#ifndef	KItemH
#define	KItemH

#include	"KBasPropTbl.h"
#include	"KMagicAttrib.h"
#include	"GameDataDef.h"
#ifndef _SERVER
#include	 "../../Represent/iRepresent/KRepresentUnit.h"
#endif
#include	"KTabFile.h"
#include	"KSG_StringProcess.h"

class KPlayer;
class KIniFile;

#define		IN
#define		OUT

#define		KItemNormalAttrib KMagicAttrib

typedef struct
{
	BYTE	nKind; //TamLTM Kham nam xanh
	BOOL	bTemp;
	INT		nItemNature;
	INT		nTradePrice;
	INT		nItemGenre;				// µÀ¾ßÖÖÀà (ÎäÆ÷? Ò©Æ·? ¿óÊ¯?)
	INT		nDetailType;			// ÔÚ¸ÃÖÖÀàÖÐµÄ¾ßÌåÀà±ð
	INT		nParticularType;		// ÏêÏ¸Àà±ð
	INT		nObjIdx;				// µØÍ¼ÉÏ°Ú·ÅÊ±¶ÔÓ¦µÄÎï¼þÊý¾Ý±àºÅ
	INT		nWidth;					// µÀ¾ßÀ¸ÖÐËùÕ¼¿í¶È
	INT		nHeight;				// µÀ¾ßÀ¸ÖÐËùÕ¼¸ß¶È
	INT		nPrice;					// ¼Û¸ñ
	INT		nNewPrice;
	BOOL	bNewArrival;
	INT		nLevel;					// µÈ¼¶
	INT		nSeries;				// ÎåÐÐÊôÐÔ
	char	szItemName[80];			// Ãû³Æ
#ifndef _SERVER
	char	szImageName[80];		// ½çÃæÖÐµÄ¶¯»­ÎÄ¼þÃû
	char	szIntro[SZBUFLEN_1];			// ËµÃ÷ÎÄ×Ö
#endif
	char	szScript[128];		// ½çÃæÖÐµÄ¶¯»­ÎÄ¼þÃû
	INT		nExpirePoint;
	BOOL	bShortKey;
	INT		nStackNum;
	INT		nMaxStack;
	INT		nExpireTime;
	KLockItem		LockItem;
	BOOL	bLockSell;
	BOOL	bLockTrade;
	BOOL	bLockDrop;
	INT		nParam;
	INT		nMantle;
	INT		nRow;
	INT		nGroup;					// Ì××°
	INT		nSetID;					// Ì××°Id
	INT		nNeedToActive1;				// Ì××°À©Õ¹
	INT		nNeedToActive2;				// Ì××°À©Õ¹
	PlayerItem	BackLocal;
	INT		nFortune;
	INT		nUpgradeLvl;
	INT		nPhysicVal;
	INT		nMagicVal;
	INT		nLine; // TamLTM Kham nam xanh
	UINT uFlash;
} KItemCommonAttrib;

typedef struct
{
	UINT	uRandomSeed;
	INT		nGeneratorLevel[MAX_ITEM_MAGICLEVEL];
	INT		nVersion;
	INT		nLuck;
} KItemGeneratorParam;

class KNpc;

class KItem
{
public:
	KItem();
	~KItem();

	// ÒÔÏÂÊÇºËÐÄ³ÉÔ±±äÁ¿
private:

	KItemNormalAttrib	m_aryBaseAttrib[7];		// µÀ¾ßµÄ»ù´¡ÊôÐÔ
	KItemNormalAttrib	m_aryRequireAttrib[6];	// µÀ¾ßµÄÐèÇóÊôÐÔ


	// ÒÔÏÂÊÇ¸¨ÖúÐÔµÄ³ÉÔ±±äÁ¿
public:
	KItemGeneratorParam	m_GeneratorParam;		// µÀ¾ßµÄÉú³É²ÎÊý
	KItemCommonAttrib	m_CommonAttrib;			// ¸÷ÖÖµÀ¾ßµÄ¹²Í¬ÊôÐÔ
	KItemNormalAttrib	m_aryMagicAttrib[MAX_ITEM_MAGICATTRIB];	// µÀ¾ßµÄÄ§·¨ÊôÐÔ
private:
	INT		m_nIndex;							// ×ÔÉíÔÚµÀ¾ßÊý×éÖÐµÄË÷Òý
	DWORD	m_dwOwner;
	DWORD	m_dwID;								// ¶ÀÁ¢µÄID£¬ÓÃÓÚ¿Í»§¶ËÓë·þÎñÆ÷¶ËµÄ½»Á÷
	INT		m_nCurrentDur;						// µ±Ç°ÄÍ¾Ã¶È

#ifndef _SERVER
	KRUImage	m_Image;
#endif
	// ÒÔÏÂÊÇ¶ÔÍâ½Ó¿Ú
public:
	void	ApplyMagicAttribToNPC(IN KNpc*, IN INT = 0, IN INT = 0) const;
	void	RemoveMagicAttribFromNPC(IN KNpc*, IN INT = 0, IN INT = 0) const;
	void	ApplyHiddenMagicAttribToNPC(IN KNpc*, IN INT) const;
	void	RemoveHiddenMagicAttribFromNPC(IN KNpc*, IN INT) const;
	BYTE	GetKind() const { return m_CommonAttrib.nKind; }; // TamLTM Kham nam xanh
	INT		GetLine() const { return m_CommonAttrib.nLine; }; // TamLTM Kham nam xanh
	KItemGeneratorParam* GetItemParam() { return &m_GeneratorParam; };
	void	SetTemp(BOOL bFlag) { m_CommonAttrib.bTemp = bFlag; };
	BOOL	IsTemp() const { return m_CommonAttrib.bTemp; };
	void	SetOwner(DWORD dwID) { m_dwOwner = dwID; };
	DWORD	GetOwner() const { return m_dwOwner; };
	void	SetTradePrice(INT nPrice) { m_CommonAttrib.nTradePrice = nPrice; };
	INT		GetTradePrice() const { return m_CommonAttrib.nTradePrice; };
	void	SetID(DWORD dwID) { m_dwID = dwID; };
	DWORD	GetID() const { return m_dwID; };
	INT		GetDetailType() const { return m_CommonAttrib.nDetailType; };
	INT		GetNature() const { return m_CommonAttrib.nItemNature; };
	INT		GetGenre() const { return m_CommonAttrib.nItemGenre; };
	INT		GetSeries() const { return m_CommonAttrib.nSeries; };
	INT		GetParticular() { return m_CommonAttrib.nParticularType; };
	INT		GetParticularMelee();
	INT		GetQuality();

	INT		GetVersion() { return m_GeneratorParam.nVersion; };
	void	SetVersion(INT nVersion) { m_GeneratorParam.nVersion = nVersion; };
	INT		GetLevel() { return m_CommonAttrib.nLevel; };
	void	SetSeries(INT nSeries) { m_CommonAttrib.nSeries = nSeries; };
	void	SetWidth(INT nWidth) { m_CommonAttrib.nWidth = nWidth; };
	void	SetHeight(INT nHeight) { m_CommonAttrib.nHeight = nHeight; };
	INT		GetWidth() const { return m_CommonAttrib.nWidth; };
	INT		GetHeight() const { return m_CommonAttrib.nHeight; };
	INT		GetOrgPrice() const { return m_CommonAttrib.nPrice; };
	INT		GetCurPrice() const
	{
		if (m_CommonAttrib.nNewPrice > 0)
			return m_CommonAttrib.nNewPrice;
		else
			return m_CommonAttrib.nPrice;
	};
	INT		GetSalePrice();
	void	SetExpirePoint(INT nPoint) { m_CommonAttrib.nExpirePoint = nPoint; };
	INT		GetExpirePoint() const { return m_CommonAttrib.nExpirePoint; };
	char* GetName() const {
		return (char*)m_CommonAttrib.szItemName;
	};
	char* GetScript() const { return (char*)m_CommonAttrib.szScript; };
	INT		GetObjIdx() { return m_CommonAttrib.nObjIdx; };
	void* GetRequirement(IN INT);
	INT		GetMaxDurability();
	INT		GetTotalMagicLevel();
	BOOL	IsReduce();
	INT		GetRepairPrice();
	void	Remove();
	BOOL	SetBaseAttrib(IN const KItemNormalAttrib*);
	BOOL	SetRequireAttrib(IN const KItemNormalAttrib*);
	BOOL	SetMagicAttrib(IN const KItemNormalAttrib*);
	void	SetDurability(IN const INT nDur);
	INT		GetDurability() { return m_nCurrentDur; };
	KItemGeneratorParam* GetGeneratorParam() { return &m_GeneratorParam; }
	INT		Abrade(IN const INT nAbradeP, IN const INT nRange);
	BOOL	CanBeRepaired();

	void	SetExpireTime(INT nSec) { m_CommonAttrib.nExpireTime = nSec; };
	INT		GetExpireTime() {
		if (m_CommonAttrib.nExpireTime > KSG_GetCurSec())
			return m_CommonAttrib.nExpireTime;
		else
			return 0;
	};

	void	SetLevel(INT i) { m_CommonAttrib.nLevel = i; };
	void	SetDetailType(INT nState) { m_CommonAttrib.nDetailType = nState; };
	void	SetNature(INT nState) { m_CommonAttrib.nItemNature = nState; };
	void	SetGenre(INT nState) { m_CommonAttrib.nItemGenre = nState; };
	void	SetParticular(INT nState) { m_CommonAttrib.nParticularType = nState; };

	void	SetBackLocal(PlayerItem* pLocal) { m_CommonAttrib.BackLocal = *pLocal; };
	void	SetBackLocal(INT nPlace, INT nX, INT nY)
	{
		m_CommonAttrib.BackLocal.nPlace = nPlace;
		m_CommonAttrib.BackLocal.nX = nX;
		m_CommonAttrib.BackLocal.nY = nY;
	};
	PlayerItem* GetBackLocal() { return &m_CommonAttrib.BackLocal; };

	void	LockItem(INT nLock) {
		if (nLock > LOCK_STATE_NORMAL)
		{
			m_CommonAttrib.LockItem.nState = LOCK_STATE_UNLOCK;
			m_CommonAttrib.LockItem.dwLockTime = KSG_GetCurSec() + nLock;
		}
		else
		{
			m_CommonAttrib.LockItem.nState = nLock;
			m_CommonAttrib.LockItem.dwLockTime = 0;
		}
	};
	void	SetLock(KLockItem* sLockItem) {
		if (sLockItem)
			m_CommonAttrib.LockItem = *sLockItem;
		else
		{
			m_CommonAttrib.LockItem.nState = LOCK_STATE_NORMAL;
			m_CommonAttrib.LockItem.dwLockTime = 0;
		}
	};
	KLockItem* GetLock() { return &m_CommonAttrib.LockItem; };

	BOOL	LockPick() {
		if (m_CommonAttrib.nItemNature || m_CommonAttrib.bLockTrade || m_CommonAttrib.LockItem.IsLock())
			return FALSE;
		return TRUE;
	};

	void	SetLockSell(BOOL bFlag) { m_CommonAttrib.bLockSell = bFlag; };
	BOOL	GetLockSell() {
		/*if (m_CommonAttrib.nItemGenre == item_task)
			return TRUE;*/
		return m_CommonAttrib.bLockSell;
	};
	void	SetLockTrade(BOOL bFlag) { m_CommonAttrib.bLockTrade = bFlag; };
	BOOL	GetLockTrade() {
		return m_CommonAttrib.bLockTrade;
	};
	void	SetLockDrop(BOOL bFlag) { m_CommonAttrib.bLockDrop = bFlag; };
	BOOL	GetLockDrop() {
		/*if (m_CommonAttrib.nItemGenre == item_task)
			return TRUE;*/
		return m_CommonAttrib.bLockDrop;
	};
	BOOL	CanShortKey();
	INT		GetShortKey() const { return m_CommonAttrib.bShortKey; };

	void	SetRow(INT n) { m_CommonAttrib.nRow = n; };
	INT		GetRow() const { return m_CommonAttrib.nRow; };
	INT		GetGroup() const { return m_CommonAttrib.nGroup; };
	INT		GetSetID() const { return m_CommonAttrib.nSetID; };
	INT		GetNeedToActive1() const { return m_CommonAttrib.nNeedToActive1; };
	INT		GetNeedToActive2() const { return m_CommonAttrib.nNeedToActive2; };

	BOOL	IsStack() const {
		if (m_CommonAttrib.nMaxStack <= 1)
			return FALSE;
		else
			return TRUE;
	};

	INT		GetStackNum() const {
		if (IsStack())
			return m_CommonAttrib.nStackNum;
		else
			return 1;
	};
	INT		AddStackNum(INT nNum) {
		INT nResult = 0;
		if (IsStack())
		{
			if (nNum < 0)
				nNum = 0;
			else if ((m_CommonAttrib.nStackNum + nNum) > m_CommonAttrib.nMaxStack)
			{
				nResult = (m_CommonAttrib.nStackNum + nNum) - m_CommonAttrib.nMaxStack;
				nNum = m_CommonAttrib.nMaxStack - m_CommonAttrib.nStackNum;
			}
		}
		else
			nNum = 0;

		m_CommonAttrib.nStackNum += nNum;
		return nResult;
	};
	INT		SetStackNum(INT nNum) {
		INT nResult = 0;
		if (IsStack())
		{
			if (nNum < 1)
				nNum = 1;
			else if (nNum > m_CommonAttrib.nMaxStack)
			{
				nResult = nNum - m_CommonAttrib.nMaxStack;
				nNum = m_CommonAttrib.nMaxStack;
			}
		}
		else
			nNum = 1;
		m_CommonAttrib.nStackNum = nNum;
		return nResult;
	};

	INT		GetMaxStackNum() const { return m_CommonAttrib.nMaxStack; };

	void	SetParam(INT i) { m_CommonAttrib.nParam = i; };
	INT		GetParam() { return m_CommonAttrib.nParam; };

	INT		GetMantle() { return m_CommonAttrib.nMantle; };
	void	SetMantle(INT n) { m_CommonAttrib.nMantle = n; };

	UINT	GetFlash() { return m_CommonAttrib.uFlash; };
	void	SetFlash(UINT uFlash) { m_CommonAttrib.uFlash = uFlash; };

	void	SetNewPrice(INT uPrice) { m_CommonAttrib.nNewPrice = uPrice; };
	INT		GetNewPrice() { return m_CommonAttrib.nNewPrice; };
	void	SetNewArrival(BOOL bNewArrival) { m_CommonAttrib.bNewArrival = bNewArrival; };
	INT		GetNewArrival() { return m_CommonAttrib.bNewArrival; };

	void	SetFortune(INT i) { m_CommonAttrib.nFortune = i; };
	INT		GetFortune() { return m_CommonAttrib.nFortune; };
	INT		GetColorItem();
#ifndef _SERVER
	void	Paint(INT nX, INT nY, bool bResize = false, bool bPaintStack = true);
	void	GetDesc(char* pszMsg, bool bShowPrice = false, bool bPriceScale = false, INT nActiveAttrib = 0, INT nGoldActiveAttrib = 0);
	char* GetImageName() { return (char*)m_CommonAttrib.szImageName; };
#endif

	// ÒÔÏÂÊÇ¸¨Öú½Ó¿Ú
	friend class	KItemGenerator;
	friend class	KPlayer;
	friend class	KItemList;
private:
	BOOL SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT*);
	BOOL SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT_GOLD*);
	BOOL SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT_PLATINA*);
	BOOL SetAttrib_MA(IN const KItemNormalAttrib*);
	BOOL SetAttrib_MA(IN const KMACP*);
	void operator = (const KBASICPROP_EQUIPMENT&);
	void operator = (const KBASICPROP_EQUIPMENT_GOLD&);
	void operator = (const KBASICPROP_EQUIPMENT_PLATINA&);
	void operator = (const KBASICPROP_EVENTITEM&);
	void operator = (const KBASICPROP_MEDICINE&);
	void operator = (const KBASICPROP_QUEST&);
	void operator = (const KBASICPROP_TOWNPORTAL&);
	void operator = (const KBASICPROP_MAGICSCRIPT&);

private:
	BOOL SetAttrib_Base(const KEQCP_BASIC*);
	BOOL SetAttrib_Req(const KEQCP_REQ*);
};

extern KItem Item[MAX_ITEM];

#endif
