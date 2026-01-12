#ifndef	KBuySellH
#define	KBuySellH

#define	BUY_SELL_SCALE		4

class KItem;

typedef struct
{
	INT		nItemNature;
	INT		nGenre;
	INT		nDetailType;
	INT		nParticularType;
	INT		nSeriesReq;
	INT		nLevel;
	INT		nNewPrice;
	INT		nParam;
	INT		nExpirePoint;
	INT		nLock;
	INT		nNewArrival;
} ItemGenerate;

class KInventory;

class KBuySell
{
public:
	KBuySell();
	~KBuySell();
	INT				Init();
	INT				GetWidth() { return m_Width; }
	INT				GetHeight() { return m_Height; }
	KItem* GetItem(INT nIndex);
	INT				GetItemIndex(INT nShop, INT nIndex);
	BOOL			BuySellCheck(INT nBuy, INT nBuyIdx);
private:
	INT** m_SellItem;
	KItem* m_Item;
	INT				m_Width;
	INT				m_Height;
	INT				m_ItemNum;
public:
#ifdef _SERVER
	BOOL			CanBuy(INT nPlayerIdx, INT nBuy, INT nBuyIdx, INT nBuyNumber);
	void			OpenSale(INT nPlayerIdx, INT nShop, INT nShopMoneyUnit);
	void			OpenSale(INT nPlayerIdx, INT nSaleType, INT nMoneyUnit, INT nShopNum, INT* nShopId);
	BOOL			Buy(INT nPlayerIdx, INT nBuy, INT nBuyIdx, BYTE nBuyNumber);	// 买第几个买卖列表中的第几项道具
	BOOL			Sell(INT nPlayerIdx, INT nBuy, INT nIdx, INT nBuyNumber);
#endif
#ifndef _SERVER
	KInventory* m_pShopRoom;
	KInventory* m_pSShopRoom;
	void			OpenSale(BuySellInfo* pInfo);
	void			OpenSale(INT nSaleType, BuySellInfo* pInfo);
	void			PaintItem(INT nIdx, INT nX, INT nY, BOOL bStack = FALSE);
#endif
};

extern KBuySell	BuySell;
#endif
