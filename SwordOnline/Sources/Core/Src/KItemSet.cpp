#include "KCore.h"
#include "KItem.h"
#include "KItemGenerator.h"
#include "KItemSet.h"
#include <mutex>
//#include "MyAssert.h"

KItemSet	ItemSet;
/*!*****************************************************************************
// Function		: KItemSet::KItemSet
// Purpose		:
// Return		:
// Comments		:
// Author		: Spe
*****************************************************************************/
KItemSet::KItemSet()
{
	m_dwIDCreator = 100;
	m_sRepairParam.nGoldScale = 0;
	m_sRepairParam.nMagicScale = 0;
	m_sRepairParam.nPlatinaScale = 0;
	m_sRepairParam.nPriceScale = 0;
	m_sRepairParam.nWarningBaseline = 0;

	ZeroMemory(m_nItemAbradeRate, sizeof(m_nItemAbradeRate));
	ZeroMemory(&m_sRepairParam, sizeof(REPAIR_ITEM_PARAM));
#ifdef _SERVER
	m_psItemInfo = NULL;
	m_psBackItemInfo = NULL;
	ZeroMemory(m_sLoseEquipItem, sizeof(m_sLoseEquipItem));
	ZeroMemory(m_sLoseItemFromEquipmentRoom, sizeof(m_sLoseItemFromEquipmentRoom));
#endif
}

KItemSet::~KItemSet()
{
#ifdef _SERVER
	if (m_psItemInfo)
		delete[] m_psItemInfo;
	m_psItemInfo = NULL;
	if (m_psBackItemInfo)
		delete[] m_psBackItemInfo;
	m_psBackItemInfo = NULL;
#endif
}

/*!*****************************************************************************
// Function		: KItemSet::Init
// Purpose		:
// Return		: void
// Comments		:
// Author		: Spe
*****************************************************************************/
void KItemSet::Init()
{
	m_FreeIdx.Init(MAX_ITEM);
	m_UseIdx.Init(MAX_ITEM);

	for (INT i = MAX_ITEM - 1; i > 0; i--)
	{
		m_FreeIdx.Insert(i);
	}
#ifdef _SERVER
	if (m_psItemInfo)
		delete[] m_psItemInfo;
	m_psItemInfo = NULL;
	m_psItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memset(this->m_psItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
	if (m_psBackItemInfo)
		delete[] m_psBackItemInfo;
	m_psBackItemInfo = NULL;
	m_psBackItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memset(this->m_psBackItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
#endif
	KIniFile	IniFile;
	IniFile.Load(ITEM_ABRADE_FILE);
	//	Î¬ÐÞ¼Û¸ñ
	IniFile.GetInteger("Repair", "ItemPriceScale", 100, &m_sRepairParam.nPriceScale);
	IniFile.GetInteger("Repair", "MagicPriceScale", 10, &m_sRepairParam.nMagicScale);
	IniFile.GetInteger("Repair", "GoldPriceScale", 10, &m_sRepairParam.nGoldScale);
	IniFile.GetInteger("Repair", "PlatinaPriceScale", 10, &m_sRepairParam.nPlatinaScale);
	IniFile.GetInteger("Repair", "WarningBaseline", 10, &m_sRepairParam.nWarningBaseline);
	//	¹¥»÷Ä¥Ëð
	IniFile.GetInteger("Attack", "Weapon", 256, &m_nItemAbradeRate[enumAbradeAttack][itempart_weapon]);
	IniFile.GetInteger("Attack", "Head", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_head]);
	IniFile.GetInteger("Attack", "Body", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_body]);
	IniFile.GetInteger("Attack", "Belt", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_belt]);
	IniFile.GetInteger("Attack", "Foot", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_foot]);
	IniFile.GetInteger("Attack", "Cuff", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_cuff]);
	IniFile.GetInteger("Attack", "Amulet", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_amulet]);
	IniFile.GetInteger("Attack", "Ring1", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_ring1]);
	IniFile.GetInteger("Attack", "Ring2", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_ring2]);
	IniFile.GetInteger("Attack", "Pendant", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_pendant]);
	IniFile.GetInteger("Attack", "Horse", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_horse]);
	IniFile.GetInteger("Attack", "Mask", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_mask]);
	// ·ÀÓùÄ¥Ëð
	IniFile.GetInteger("Defend", "Weapon", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_weapon]);
	IniFile.GetInteger("Defend", "Head", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_head]);
	IniFile.GetInteger("Defend", "Body", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_body]);
	IniFile.GetInteger("Defend", "Belt", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_belt]);
	IniFile.GetInteger("Defend", "Foot", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_foot]);
	IniFile.GetInteger("Defend", "Cuff", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_cuff]);
	IniFile.GetInteger("Defend", "Amulet", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_amulet]);
	IniFile.GetInteger("Defend", "Ring1", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_ring1]);
	IniFile.GetInteger("Defend", "Ring2", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_ring2]);
	IniFile.GetInteger("Defend", "Pendant", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_pendant]);
	IniFile.GetInteger("Defend", "Horse", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_horse]);
	IniFile.GetInteger("Attack", "Mask", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_mask]);
	// ÒÆ¶¯Ä¥Ëð
	IniFile.GetInteger("Move", "Weapon", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_weapon]);
	IniFile.GetInteger("Move", "Head", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_head]);
	IniFile.GetInteger("Move", "Body", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_body]);
	IniFile.GetInteger("Move", "Belt", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_belt]);
	IniFile.GetInteger("Move", "Foot", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_foot]);
	IniFile.GetInteger("Move", "Cuff", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_cuff]);
	IniFile.GetInteger("Move", "Amulet", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_amulet]);
	IniFile.GetInteger("Move", "Ring1", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_ring1]);
	IniFile.GetInteger("Move", "Ring2", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_ring2]);
	IniFile.GetInteger("Move", "Pendant", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_pendant]);
	IniFile.GetInteger("Move", "Horse", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_horse]);
	IniFile.GetInteger("Attack", "Mask", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_mask]);
}

/*!*****************************************************************************
// Function		: KItemSet::SearchID
// Purpose		:
// Return		: INT
// Argumant		: DWORD dwID
// Comments		:
// Author		: Spe
*****************************************************************************/
INT KItemSet::SearchID(DWORD dwID)
{
	INT nIdx = 0;

	while (1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;
		if (Item[nIdx].GetID() == dwID)
			break;
	}
	return nIdx;

}

INT KItemSet::Add(KItem* pItem)
{
	KASSERT(NULL != pItem);

	INT i = FindFree();

	if (!i)
		return 0;

	Item[i] = *pItem;
#ifdef _SERVER
	SetID(i);
#endif
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);
	return i;
}

/*!*****************************************************************************
// Function		: KItemSet::Add
// Purpose		:
// Return		: INT Êý×é±àºÅ
// Argumant		: INT µÀ¾ßÀàÐÍ£¨×°±¸£¿Ò©Æ·£¿¿óÊ¯£¿¡­¡­£©
// Argumant		: INT Ä§·¨µÈ¼¶£¨Èç¶ÔÓ¦ÓÚ×°±¸£¬¾ÍÊÇÒ»°ã×°±¸£¬À¶É«×°±¸£¬ÁÁ½ðµÈ¡­¡­£©
// Argumant		: INT ÎåÐÐÊôÐÔ
// Argumant		: INT µÈ¼¶
// Argumant		: INT ÐÒÔËÖµ
// Comments		:
// Author		: Spe
*****************************************************************************/
INT KItemSet::Add(IN INT nItemNature, INT nItemGenre, INT nSeries,
	INT nLevel, INT nLuck, INT nDetailType/*=-1*/,
	INT nParticularType/*=-1*/, INT* pnMagicLevel, INT nVersion/*=0*/, UINT nRandomSeed, INT nStackNum)
{
	INT i = FindFree();

	if (i == 0)
		return 0;

	KItem* pItem = &Item[i];
	pItem->m_GeneratorParam.nVersion = nVersion;
	pItem->m_GeneratorParam.uRandomSeed = nRandomSeed;

	if (nStackNum > 0 && nStackNum <= pItem->m_CommonAttrib.nMaxStack)
		pItem->m_CommonAttrib.nStackNum = nStackNum;

	// Đặt NATURE từ giá trị nItemNature
	pItem->SetNature(nItemNature);
	pItem->SetLock(0);

	switch (nItemGenre)
	{
	case item_equip:			// ×°±¸Àà
		ItemGen.Gen_Equipment(nItemNature, nDetailType, nParticularType, nSeries, nLevel, pnMagicLevel, nLuck, nVersion, pItem);
		break;
	case item_medicine:			// Ò©Æ·Àà
		ItemGen.Gen_Medicine(nDetailType, nLevel, nVersion, pItem);
		break;
	case item_event:				// ¿óÊ¯Àà
		ItemGen.Gen_Event(nDetailType, pItem);
		break;
	case item_materials:				// ¿óÊ¯Àà
		break;
	case item_task:				// Ä¬ÈÏÊÇÈÎÎñÀà
		ItemGen.Gen_Quest(nDetailType, pItem);
		break;
	case item_magicscript:
		ItemGen.Gen_MagicScript(nDetailType, pItem, nLevel, nSeries, nLuck);
		break;
	case item_townportal:
		ItemGen.Gen_TownPortal(nDetailType, pItem);
		break;
	default:
		break;
	}

#ifdef _SERVER
	SetID(i);
#endif
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);
	return i;
}

// end
/*!*****************************************************************************
// Function		: KItemSet::FindFree
// Purpose		:
// Return		: INT
// Comments		:
// Author		: Spe
*****************************************************************************/
INT KItemSet::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

void KItemSet::Remove(IN INT nIdx)
{
	Item[nIdx].Remove();

	m_UseIdx.Remove(nIdx);
	m_FreeIdx.Insert(nIdx);
}

void KItemSet::SetID(IN INT nIdx) {
	std::lock_guard<std::mutex> lock(mutex); // Lock mutex để đảm bảo chỉ một luồng có thể thực hiện đặt ID tại một thời điểm
	Item[nIdx].SetID(m_dwIDCreator);
	m_dwIDCreator++;
}

#ifdef _SERVER
//---------------------------------------------------------------------------
//	¹¦ÄÜ£ºcopy m_psItemInfo to m_psBackItemInfo
//---------------------------------------------------------------------------
void	KItemSet::BackItemInfo()
{
	_ASSERT(this->m_psItemInfo);
	_ASSERT(this->m_psBackItemInfo);
	if (!m_psItemInfo)
		return;
	if (!m_psBackItemInfo)
		m_psBackItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
	memcpy(m_psBackItemInfo, this->m_psItemInfo, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
}
#endif

INT KItemSet::GetAbradeRange(IN INT nType, IN INT nPart)
{
	if (nType < 0 || nType >= enumAbradeNum)
		return 0;
	if (nPart < 0 || nPart >= itempart_num)
		return 0;

	return m_nItemAbradeRate[nType][nPart];
}