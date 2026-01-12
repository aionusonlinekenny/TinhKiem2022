#ifndef KItemSetH
#define	KItemSetH

#include "KLinkArray.h"
#include <mutex>
#include <atomic>

#define		IN
#define		OUT
class	KItem;

enum enumAbrade
{
	enumAbradeAttack = 0,
	enumAbradeDefend,
	enumAbradeMove,
	enumAbradeNum,
};
// Íæ¼ÒÖ®¼ä½»Ò×½øÐÐÊ±£¬ÓÃÓÚÅÐ¶ÏÍæ¼ÒÎïÆ·À¸ÄÜ·ñ·ÅÏÂÂò½øµÄÎïÆ·
typedef struct
{
	INT		m_nIdx;
	INT		m_nX;
	INT		m_nY;
	INT		m_nWidth;
	INT		m_nHeight;
} TRADE_ITEM_INFO;

typedef struct
{
	INT		nPriceScale;
	INT		nMagicScale;
	INT		nGoldScale;
	INT		nPlatinaScale;
	INT		nWarningBaseline;
} REPAIR_ITEM_PARAM;

class KItemSet
{
private:
	//DWORD			m_dwIDCreator;		//	IDÉú³ÉÆ÷£¬ÓÃÓÚ¿Í»§¶ËÓë·þÎñÆ÷¶ËµÄ½»Á÷
	std::atomic<INT> m_dwIDCreator{ 0 }; // Sử dụng atomic để đảm bảo thao tác tăng giảm giá trị ID là an toàn
	KLinkArray		m_FreeIdx;			//	¿ÉÓÃ±í
	KLinkArray		m_UseIdx;			//	ÒÑÓÃ±í

public:
	INT				m_nItemAbradeRate[enumAbradeNum][itempart_num];
	REPAIR_ITEM_PARAM	m_sRepairParam;
#ifdef _SERVER
	// Íæ¼ÒÖ®¼ä½»Ò×½øÐÐÊ±£¬ÓÃÓÚÅÐ¶ÏÍæ¼ÒÎïÆ·À¸ÄÜ·ñ·ÅÏÂÂò½øµÄÎïÆ·
	TRADE_ITEM_INFO* m_psItemInfo;
	// Íæ¼ÒÖ®¼ä½»Ò×½øÐÐÊ±£¬ÓÃÓÚÅÐ¶ÏÍæ¼ÒÎïÆ·À¸ÄÜ·ñ·ÅÏÂÂò½øµÄÎïÆ·
	TRADE_ITEM_INFO* m_psBackItemInfo;
	// copy m_psItemInfo to m_psBackItemInfo
	void			BackItemInfo();
	// ÓÃÓÚÍæ¼Ò±»PKËÀÍöºóµÄ³Í·£¼ÆËã£¬µôÂäËæÉíÎïÆ·
	PlayerItem		m_sLoseItemFromEquipmentRoom[EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT];
	// ÓÃÓÚÍæ¼Ò±»PKËÀÍöºóµÄ³Í·£¼ÆËã£¬µôÂä´©ÔÚÉíÉÏµÄ×°±¸
	PlayerItem		m_sLoseEquipItem[itempart_num];
#endif

public:
	KItemSet();
	~KItemSet();
	void			Init();
	INT				GetItemNumber() { return m_UseIdx.GetCount(); };
	INT				SearchID(IN DWORD dwID);
	INT				Add(KItem* pItem);
	INT				Add(IN INT nItemNature, IN INT nItemGenre, IN INT nSeries, IN INT nLevel, IN INT nLuck, IN INT nDetail = -1, IN INT nParticular = -1, IN INT* pnMagicLevel = NULL, IN INT nVersion = 0, IN UINT nRandomSeed = 0, IN INT nStackNum = 0);
	void			Remove(IN INT nIdx);
	INT				GetAbradeRange(IN INT nType, IN INT nPart);
private:
	std::mutex		mutex; // Khai báo một mutex để sử dụng cho việc đồng bộ hóa
	void			SetID(IN INT nIdx);
	INT				FindFree();
};

extern KItemSet	ItemSet;
#endif
