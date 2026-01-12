#ifndef KInventoryH
#define	KInventoryH

class CORE_API KInventory
{
	friend	class	KItemList;

private:
	INT* m_pArray;
	INT		m_nMoney;
	INT		m_nWidth;
	INT		m_nHeight;

public:
	KInventory();
	~KInventory();

	BOOL	Init(INT nWidth, INT nHeight);
	void	Release();
	void	Clear();
	void	ClearItem();
	BOOL	PickUpItem(INT nIdx, INT nX, INT nY, INT nWidth, INT nHeight);
	INT		FindItem(INT nX, INT nY);
	BOOL	FindFreeCell(INT nX, INT nY);
	BOOL	PlaceItem(INT nXpos, INT nYpos, INT nIdx, INT nWidth, INT nHeight);
	BOOL	HoldItem(INT nIdx, INT nWidth, INT nHeight);
	BOOL	FindRoom(INT nWidth, INT nHeight, POINT* pPos);
	BOOL	CheckRoom(INT nXpos, INT nYpos, INT nWidth, INT nHeight);
	BOOL	AddMoney(INT nMoney);
	INT		GetMoney() { return m_nMoney; }
	BOOL	SetMoney(INT nMoney) { if (nMoney < 0) return FALSE; m_nMoney = nMoney; return TRUE; }
	INT		GetNextItem(INT nStartIdx, INT nXpos, INT nYpos, INT* pX, INT* pY);
	INT		CalcSameDetailType(INT nItemNature, INT nItemGenre, INT nDetail, INT nLevel, INT nSeries);
	BOOL	FindEmptyPlace(INT nWidth, INT nHeight, POINT* pPos);

	// 输入物品类型和具体类型，察看Inventory里面有没有相同的物品，输出位置和编号
	BOOL	FindSameToRemove(INT nItemNature, INT nItemGenre, INT nDetail, INT nLevel, INT nSeries, INT* pnIdx);

	// 输入物品类型和具体类型，察看Inventory里面有没有相同的物品
	BOOL	CheckSameImmediacyItem(INT nIdx);
	BOOL	FindSameItem(INT nItemGenre, INT nDetail, INT nParticular, INT nLevel, INT* pnIdx, INT* pnX, INT* pnY);
	BOOL	FindSameItemToStack(INT nIdx, INT* pnIdx, INT* pnX, INT* pnY);
	INT		FindSameItemToMove(INT nIdx, INT* pnIdx, INT* pnX, INT* pnY);
};
#endif //KInventoryH
