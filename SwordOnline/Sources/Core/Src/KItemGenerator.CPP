//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemGenerator.CPP
// Date:	2002.08.26
// Code:	DongBo
// Desc:    CPP file. ±¾ÎÄ¼þÊµÏÖµÄÀàÓÃÓÚÉú³ÉµÀ¾ß
//---------------------------------------------------------------------------
#include "KEngine.h"
#include "KCore.h"
#include "MyAssert.H"
#include "KItem.h"
#include "KSubWorldSet.h"
#include "KItemGenerator.h"

KItemGenerator	ItemGen;			//	×°±¸Éú³ÉÆ÷

KItemGenerator::KItemGenerator()
{
	ZeroMemory(m_EquipNumOfEntries, sizeof(m_EquipNumOfEntries));
	ZeroMemory(m_GMA_nCount, sizeof(m_GMA_nCount));
	ZeroMemory(m_GMA_naryLevel, sizeof(m_GMA_naryLevel));
	ZeroMemory(m_GMA_naryCandidateMA, sizeof(m_GMA_naryCandidateMA));
	ZeroMemory(m_GMA_naryDropRate, sizeof(m_GMA_naryDropRate));
	ZeroMemory(m_GMA_naryMA, sizeof(m_GMA_naryMA));
	m_MedNumOfEntries = 0;
}

KItemGenerator::~KItemGenerator()
{
}

/******************************************************************************
	¹¦ÄÜ£º	Êý¾Ý³õÊ¼»¯. ´Ótab fileÖÐ¶ÁÈ¡Êý¾Ý
******************************************************************************/
BOOL KItemGenerator::Init()
{
	if (!m_BPTLib.Init())		// ´Ëµ÷ÓÃ´ÓÈô¸ÉµÄtab fileÖÐÔØÈëËùÓÐµÄ³õÊ¼ÊôÐÔ
		return FALSE;

	for (INT i = equip_meleeweapon; i < equip_detailnum - equip_meleeweapon; i++)
	{
		switch (i)
		{
		case equip_meleeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMeleeWeaponRecordNumber();
			break;
		case equip_rangeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRangeWeaponRecordNumber();
			break;
		case equip_armor:
			m_EquipNumOfEntries[i] = m_BPTLib.GetArmorRecordNumber();
			break;
		case equip_helm:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHelmRecordNumber();
			break;
		case equip_boots:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBootRecordNumber();
			break;
		case equip_belt:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBeltRecordNumber();
			break;
		case equip_cuff:
			m_EquipNumOfEntries[i] = m_BPTLib.GetCuffRecordNumber();
			break;
		case equip_amulet:
			m_EquipNumOfEntries[i] = m_BPTLib.GetAmuletRecordNumber();
			break;
		case equip_ring:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRingRecordNumber();
			break;
		case equip_pendant:
			m_EquipNumOfEntries[i] = m_BPTLib.GetPendantRecordNumber();
			break;
		case equip_horse:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHorseRecordNumber();
			break;
		case equip_mask:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMaskRecordNumber();
			break;
		case equip_mantle:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMantleRecordNumber();
			break;
		case equip_signet:
			m_EquipNumOfEntries[i] = m_BPTLib.GetSignetRecordNumber();
			break;
		case equip_shipin:
			m_EquipNumOfEntries[i] = m_BPTLib.GetShipinRecordNumber();
			break;
		case equip_hoods:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHoodsRecordNumber();
			break;
		case equip_cloak:
			m_EquipNumOfEntries[i] = m_BPTLib.GetCloakRecordNumber();
			break;
		}
	}
	m_MedNumOfEntries = m_BPTLib.GetMedicineRecordNumber();
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉÒ©Æ·
	Èë¿Ú:	nDetailType: Ò©Æ·ÀàÐÍ
			nLevel: µÈ¼¶
			nVersion: °æ±¾ºÅ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::Gen_Medicine(IN INT nDetailType,
	IN INT nLevel,
	IN INT nVersion,
	IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;

#ifdef _SERVER	// ·þÎñÆ÷°æ±¾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = 0;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	INT nGameVersion = pItem->m_GeneratorParam.nVersion;
#endif

	const INT i = nDetailType * 5 + nLevel - 1;	// ±¾¹«Ê½ÓÉ²ß»®¶¨Òå
	// µÈ¼¶Êý¾Ý´Ó1¿ªÊ¼£¬ËùÒÔÒª¼õ»ØÀ´
	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if (NULL == pMed)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pMed;
	return TRUE;
}
/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉËæ»ú×°±¸
	Èë¿Ú:	nDetailType: ¾ßÌåÀà±ð, Èç½ü³ÌÎäÆ÷¡¢°µÆ÷¡¢......
			nParticularType: ÏêÏ¸Àà±ð
			nSeriesReq: ÎåÐÐÊôÐÔ
			nLevel: µÈ¼¶
			pnaryMALevel: Ä§·¨ÊôÐÔµÈ¼¶Êý×é[6]
			nLucky: ÔËÆøÖµ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::Gen_Equipment(IN INT nItemNature, IN INT nDetailType, IN INT nParticularType,
	IN INT nSeriesReq, IN INT nLevel,
	IN const INT* pnaryMALevel,
	IN INT nLucky,
	IN INT nVersion,
	IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	switch (nItemNature)
	{
	case NATURE_GOLD:
	{
		this->Gen_GoldEquipment(nDetailType, pnaryMALevel, nLucky, pItem);
		pItem->SetRow(nDetailType);
		//pItem->SetLevel(nLevel);
		return TRUE;
	}
	break;
	case NATURE_PLATINA:
	{
		this->Gen_PlatinaEquipment(nDetailType, pnaryMALevel, nLucky, pItem, nLevel);
		pItem->SetRow(nDetailType);
		pItem->SetLevel(nLevel);
		return TRUE;
	}
	break;
	default:
	{
		BOOL bEC = FALSE;
#ifdef _SERVER	// ·þÎñÆ÷°æ±¾
		pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
		if (pnaryMALevel)
			memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(INT) * MAX_ITEM_MAGICLEVEL);
		else
			ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(INT) * MAX_ITEM_MAGICLEVEL);
		pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
#else
		g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
		if (pnaryMALevel)
			memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(INT) * MAX_ITEM_MAGICLEVEL);
#endif
		pItem->m_GeneratorParam.nLuck = nLucky;
		INT nGameVersion = pItem->m_GeneratorParam.nVersion;

		// ¸ù¾ÝÈë¿Ú²ÎÊý, È·¶¨×°±¸µÄ»ù±¾Êý¾Ý
		INT i = 0;
		if (nDetailType == equip_mask)
			i = nParticularType;
		else
			i = nParticularType * 10 + nLevel - 1;// ±¾¹«Ê½ÓÉ²ß»®¶¨Òå
		// Ïê¼ûSPE 2002Äê8ÔÂ31ÈÕ7:40
		// email: »ù´¡µÀ¾ßÊýÖµ±í
		// µÈ¼¶Êý¾Ý´Ó1¿ªÊ¼£¬ËùÒÔÒª¼õ»ØÀ´
		const KBASICPROP_EQUIPMENT* pEqu = NULL;
		switch (nDetailType)
		{
		case equip_meleeweapon:
			pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
			break;
		case equip_rangeweapon:
			pEqu = m_BPTLib.GetRangeWeaponRecord(i);
			break;
		case equip_armor:
			pEqu = m_BPTLib.GetArmorRecord(i);
			break;
		case equip_helm:
			pEqu = m_BPTLib.GetHelmRecord(i);
			break;
		case equip_boots:
			pEqu = m_BPTLib.GetBootRecord(i);
			break;
		case equip_belt:
			pEqu = m_BPTLib.GetBeltRecord(i);
			break;
		case equip_amulet:
			pEqu = m_BPTLib.GetAmuletRecord(i);
			break;
		case equip_ring:
			pEqu = m_BPTLib.GetRingRecord(i);
			break;
		case equip_cuff:
			pEqu = m_BPTLib.GetCuffRecord(i);
			break;
		case equip_pendant:
			pEqu = m_BPTLib.GetPendantRecord(i);
			break;
		case equip_horse:
			pEqu = m_BPTLib.GetHorseRecord(i);
			break;
		case equip_mask:
			pEqu = m_BPTLib.GetMaskRecord(i);
			break;
		case equip_mantle:
			pEqu = m_BPTLib.GetMantleRecord(i);
			break;
		case equip_signet:
			pEqu = m_BPTLib.GetSignetRecord(i);
			break;
		case equip_shipin:
			pEqu = m_BPTLib.GetShipinRecord(i);
			break;
		case equip_hoods:
			pEqu = m_BPTLib.GetHoodsRecord(i);
			break;
		case equip_cloak:
			pEqu = m_BPTLib.GetCloakRecord(i);
			break;
		default:
			break;
		}
		if (NULL == pEqu)
		{
			_ASSERT(FALSE);
			return bEC;
		}
		// ÔËÐÐÖÁ´ËµÄÖ±½ÓÔ­Òò: Ö»ÓÐnÖÖ×°±¸, ¶øÉÏÃæ i µÄÖµÔÚ[0,n-1]Ö®Íâ
		// ¼ì²é3µã: nParticularType ÓÐÎó?
		//			nLevel ÓÐÎó?
		//			Ô­Ê¼µÄtab fileÓÐÎóµ¼ÖÂ m_BPTLib.m_BPTEquipment Ëù
		//			¹ÜÀíµÄÊý¾ÝÓÐÎÊÌâ?
		pItem->SetAttrib_CBR(pEqu);
		if (nSeriesReq < series_metal)
			pItem->SetSeries(::GetRandomNumber(series_metal, series_earth));
		else
			pItem->SetSeries(nSeriesReq);

		if (NULL == pnaryMALevel)
			return TRUE;

		KItemNormalAttrib	sMA[MAX_ITEM_MAGICATTRIB];
		bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
		if (bEC)
			pItem->SetAttrib_MA(sMA);

		//pItem->SetNature(nItemNature);
		return bEC;
	}
	break;
	}
}


BOOL KItemGenerator::Gen_GoldEquipment(IN INT nIndex, IN const INT* pnaryMALevel, IN INT nLucky, OUT KItem* pItem)
{
	INT i = 0;
	INT j = 0;
	INT nCount = 0;

	INT nLuckMin = (INT)LOWORD(nLucky);
	INT nLuckMax = (INT)HIWORD(nLucky);

	if (nLuckMin > MAX_ITEM_LUCK)
		nLuckMin = MAX_ITEM_LUCK;

	if (nLuckMin > nLuckMax)
		nLuckMin = nLuckMax;

	if (nLuckMax > MAX_ITEM_LUCK)
		nLuckMax = MAX_ITEM_LUCK;

#ifdef _SERVER	// ·þÎñÆ÷°æ±¾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
#endif
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(INT) * MAX_ITEM_MAGICLEVEL);
	else
	{
		for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			pItem->m_GeneratorParam.nGeneratorLevel[i] = nLuckMin == nLuckMax ? nLuckMax : GetRandomNumber(nLuckMin, nLuckMax);
			pItem->m_GeneratorParam.nGeneratorLevel[i + MAX_ITEM_MAGICATTRIB] = 0;
		}
	}
	pItem->m_GeneratorParam.nLuck = nLucky;

	nCount = m_BPTLib.GetGoldEquipNumber();
	const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldEquipRecord(nIndex);
	if (nIndex >= nCount)
		return FALSE;
	if (pTemp == NULL)
		return FALSE;
	pItem->SetAttrib_CBR((KBASICPROP_EQUIPMENT_GOLD*)pTemp);

	KItemNormalAttrib	sMA[MAX_ITEM_MAGICATTRIB];

	if (pItem->m_GeneratorParam.nGeneratorLevel[MAX_ITEM_MAGICATTRIB])
	{
		for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[i] <= 0)
				break;
			sMA[i].nAttribType = pItem->m_GeneratorParam.nGeneratorLevel[i];
			sMA[i].nValue[0] = LOWORD(pItem->m_GeneratorParam.nGeneratorLevel[i + MAX_ITEM_MAGICATTRIB]);
			sMA[i].nValue[1] = -1;
			sMA[i].nValue[2] = HIWORD(pItem->m_GeneratorParam.nGeneratorLevel[i + MAX_ITEM_MAGICATTRIB]);
		}
	}
	else
	{
		KTabFile MagicTab;
		MagicTab.Load(GOLD_EQUIP_MAGIC_FILE);
		for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			const INT* pSrc;
			KItemNormalAttrib* pDst;
			pSrc = &(pTemp->m_aryMagicAttribs[i]);
			pDst = &(sMA[i]);

			INT nType, nLow, nHigh;
			MagicTab.GetInteger(*pSrc + 1, 5, 0, &nType);
			pDst->nAttribType = nType;
			MagicTab.GetInteger(*pSrc + 1, 6, 0, &nLow);
			MagicTab.GetInteger(*pSrc + 1, 7, 0, &nHigh);

			pDst->nMax = nHigh;
			pDst->nMin = nLow;

			pDst->nValue[0] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
			MagicTab.GetInteger(*pSrc + 1, 8, 0, &nLow);
			MagicTab.GetInteger(*pSrc + 1, 9, 0, &nHigh);
			pDst->nValue[1] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
			MagicTab.GetInteger(*pSrc + 1, 10, 0, &nLow);
			MagicTab.GetInteger(*pSrc + 1, 11, 0, &nHigh);
			pDst->nValue[2] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
			if (sMA[i].nAttribType <= 0)
			{
				sMA[i].nValue[0] = 0;
				sMA[i].nValue[1] = 0;
				sMA[i].nValue[2] = 0;
			}
		}
		MagicTab.Clear();
	}
	for (NULL; i < MAX_ITEM_MAGICATTRIB; i++)
	{
		// Çå¿ÕÊ£ÏÂµÄÏî
		sMA[i].nAttribType = 0;
		sMA[i].nValue[0] = 0;
		sMA[i].nValue[1] = 0;
		sMA[i].nValue[2] = 0;
	}
	pItem->SetAttrib_MA(sMA);

	return TRUE;
}

BOOL KItemGenerator::Gen_PlatinaEquipment(IN INT nIndex, IN const INT* pnaryMALevel, IN INT nLucky, OUT KItem* pItem, IN INT nLevel)
{
	INT i = 0;
	INT j = 0;
	INT nCount = 0;

	INT nLuckMin = (INT)LOWORD(nLucky);
	INT nLuckMax = (INT)HIWORD(nLucky);

	if (nLuckMin > MAX_ITEM_LUCK)
		nLuckMin = MAX_ITEM_LUCK;

	if (nLuckMin > nLuckMax)
		nLuckMin = nLuckMax;

	if (nLuckMax > MAX_ITEM_LUCK)
		nLuckMax = MAX_ITEM_LUCK;

#ifdef _SERVER	// ·þÎñÆ÷°æ±¾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
#endif
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(INT) * MAX_ITEM_MAGICLEVEL);
	else
	{
		for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			pItem->m_GeneratorParam.nGeneratorLevel[i] = nLuckMin == nLuckMax ? nLuckMax : GetRandomNumber(nLuckMin, nLuckMax);
			pItem->m_GeneratorParam.nGeneratorLevel[i + MAX_ITEM_MAGICATTRIB] = 0;
		}
	}
	pItem->m_GeneratorParam.nLuck = nLucky;
	//pItem->SetLevel(nLevel);

	nCount = m_BPTLib.GetPlatinaEquipNumber();
	const KBASICPROP_EQUIPMENT_PLATINA* pTemp = m_BPTLib.GetPlatinaEquipRecord(nIndex);
	if (nIndex >= nCount)
		return FALSE;
	if (pTemp == NULL)
		return FALSE;
	pItem->SetAttrib_CBR((KBASICPROP_EQUIPMENT_PLATINA*)pTemp);

	KItemNormalAttrib	sMA[MAX_ITEM_MAGICATTRIB];

	if (pItem->m_GeneratorParam.nGeneratorLevel[MAX_ITEM_MAGICATTRIB])
	{
		for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[i] <= 0)
				break;
			sMA[i].nAttribType = pItem->m_GeneratorParam.nGeneratorLevel[i];
			sMA[i].nValue[0] = LOWORD(pItem->m_GeneratorParam.nGeneratorLevel[i + MAX_ITEM_MAGICATTRIB]);
			sMA[i].nValue[1] = -1;
			sMA[i].nValue[2] = HIWORD(pItem->m_GeneratorParam.nGeneratorLevel[i + MAX_ITEM_MAGICATTRIB]);
		}
	}
	else
	{
		KTabFile MagicTab;
		MagicTab.Load(GOLD_EQUIP_MAGIC_FILE);
		for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			if (i < MAX_ITEM_NORMAL_MAGICATTRIB)
			{
				const INT* pSrc;
				const INT* pSrc1;
				KItemNormalAttrib* pDst;
				pSrc = &(pTemp->m_aryMagicAttribs_0[i]);
				pSrc1 = &(pTemp->m_aryMagicAttribs_10[i]);
				pDst = &(sMA[i]);

				INT nType, nLow, nHigh;
				INT nType1, nLow1, nHigh1;
				MagicTab.GetInteger(*pSrc + 1, 5, 0, &nType);
				pDst->nAttribType = nType;
				MagicTab.GetInteger(*pSrc + 1, 6, 0, &nLow);
				MagicTab.GetInteger(*pSrc + 1, 7, 0, &nHigh);
				MagicTab.GetInteger(*pSrc1 + 1, 6, 0, &nLow1);
				MagicTab.GetInteger(*pSrc1 + 1, 7, 0, &nHigh1);

				if (i < 6 && nLevel > 0 && nLevel <= 10)
				{
					nLow += ((nLow1 - nLow) / 10) * nLevel;
					nHigh += ((nHigh1 - nHigh) / 10) * nLevel;
				}

				pDst->nMax = nHigh;
				pDst->nMin = nLow;

				pDst->nValue[0] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
				MagicTab.GetInteger(*pSrc + 1, 8, 0, &nLow);
				MagicTab.GetInteger(*pSrc + 1, 9, 0, &nHigh);
				pDst->nValue[1] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
				MagicTab.GetInteger(*pSrc + 1, 10, 0, &nLow);
				MagicTab.GetInteger(*pSrc + 1, 11, 0, &nHigh);
				pDst->nValue[2] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
				if (sMA[i].nAttribType <= 0)
				{
					sMA[i].nValue[0] = 0;
					sMA[i].nValue[1] = 0;
					sMA[i].nValue[2] = 0;
				}
			}
			else
			{
				const INT* pSrc;
				const INT* pSrc1;
				KItemNormalAttrib* pDst;
				pSrc = &(pTemp->m_aryMagicAttribs_0[i]);
				pSrc1 = &(pTemp->m_aryMagicAttribs_10[i]);
				pDst = &(sMA[i]);

				INT nType, nLow, nHigh;
				INT nType1, nLow1, nHigh1;
				MagicTab.GetInteger(*pSrc + 2, 5, 0, &nType);
				pDst->nAttribType = nType;
				MagicTab.GetInteger(*pSrc + 2, 6, 0, &nLow);
				MagicTab.GetInteger(*pSrc + 2, 7, 0, &nHigh);
				MagicTab.GetInteger(*pSrc1 + 2, 6, 0, &nLow1);
				MagicTab.GetInteger(*pSrc1 + 2, 7, 0, &nHigh1);

				if (i < 6 && nLevel > 0 && nLevel <= 10)
				{
					nLow += ((nLow1 - nLow) / 10) * nLevel;
					nHigh += ((nHigh1 - nHigh) / 10) * nLevel;
				}

				pDst->nMax = nHigh;
				pDst->nMin = nLow;

				pDst->nValue[0] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
				MagicTab.GetInteger(*pSrc + 2, 8, 0, &nLow);
				MagicTab.GetInteger(*pSrc + 2, 9, 0, &nHigh);
				pDst->nValue[1] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
				MagicTab.GetInteger(*pSrc + 2, 10, 0, &nLow);
				MagicTab.GetInteger(*pSrc + 2, 11, 0, &nHigh);
				pDst->nValue[2] = nLow + ((nHigh - nLow) * pItem->m_GeneratorParam.nGeneratorLevel[i] / MAX_ITEM_LUCK);
				if (sMA[i].nAttribType <= 0)
				{
					sMA[i].nValue[0] = 0;
					sMA[i].nValue[1] = 0;
					sMA[i].nValue[2] = 0;
				}
			}
		}
		MagicTab.Clear();
	}
	for (NULL; i < MAX_ITEM_MAGICATTRIB; i++)
	{
		// Çå¿ÕÊ£ÏÂµÄÏî
		sMA[i].nAttribType = 0;
		sMA[i].nValue[0] = 0;
		sMA[i].nValue[1] = 0;
		sMA[i].nValue[2] = 0;
	}
	pItem->SetAttrib_MA(sMA);

	return TRUE;
}
/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉÄ§·¨ÊôÐÔ
	Èë¿Ú:	nType: ×°±¸ÀàÐÍ
			pnaryMALevel: Êý×éÖ¸Õë, ¸ø³öÄ§·¨µÄµÈ¼¶ÒªÇó, ¹²6¸ö
			nSeriesReq: ÎåÐÐÊôÐÔ
			nLucky: ÔËÆøÖµ
	³ö¿Ú:	ÔÚ pnaryMA ËùÖ¸Êý×éÖÐ·µ»ØÄ§·¨µÄÊôÐÔºËÐÄÊý¾Ý, ¹²6¸ö
******************************************************************************/
// Add by Freeway Chen in 2003.5.30
BOOL KItemGenerator::Gen_MagicAttrib(INT nType, const INT* pnaryMALevel, INT nSeriesReq, INT nLucky, KItemNormalAttrib* pnaryMA, INT nGameVersion)
{
	INT nResult = false;

	KBPT_ClassMAIT  SelectedMagicTable;
	// ¼ÇÂ¼Ã¿Ò»¸ö¼ÇÂ¼Ñ¡ÖÐµÄÄ§·¨ÊôÐÔ£¬Ö÷ÒªÊÇÎªÁË×îºó½«Ê¹ÓÃ±êÖ¾Çå³ý
	KMAGICATTRIB_TABFILE* pMagicAttrTable[MAX_ITEM_MAGICLEVEL] = { NULL, NULL, NULL, NULL, NULL, NULL , NULL, NULL };

	INT i = 0;

	_ASSERT(pnaryMALevel);
	_ASSERT(pnaryMA);

	if (pnaryMALevel[MAX_ITEM_MAGICATTRIB])
	{
		for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			if (pnaryMALevel[i] <= 0)
				break;
			pnaryMA[i].nAttribType = pnaryMALevel[i];
			pnaryMA[i].nValue[0] = LOWORD(pnaryMALevel[i + MAX_ITEM_MAGICATTRIB]);
			pnaryMA[i].nValue[1] = -1;
			pnaryMA[i].nValue[2] = HIWORD(pnaryMALevel[i + MAX_ITEM_MAGICATTRIB]);
		}
	}
	else
	{
		for (i = 0; i < MAX_ITEM_MAGICLEVEL; i++)
		{
			// Èç¹û·¢ÏÖ¼¶±ðÎªÁã£¬±íÊ¾²»ÐèÒª¼ÌÐø´¦ÀíÁË£¬ÍË³ö
			if (pnaryMALevel[i] == 0)
				break;
			SelectedMagicTable.Clear();

			// ¸ù¾ÝÇ°×ººó×º¡¢ÎïÆ·ÀàÐÍ¡¢ÎåÐÐÐèÇó¡¢ÐèÇó¼¶±ð½«ËùÓÐ·ûºÏÌõ¼þµÄÄ§·¨ÊôÐÔÏîÈ¡³öÀ´
			const KBPT_ClassMAIT* pCMITItem = m_BPTLib.GetCMIT(
				1 - (i & 1),        // Ç°×ººó×º
				nType,              // ÎïÆ·ÀàÐÍ
				nSeriesReq,         // ÎåÐÐÐèÇó 
				pnaryMALevel[i]     // ÐèÇó¼¶±ð
			);

			_ASSERT(pCMITItem);
			if (!pCMITItem)
				break;

			// ±éÀú·ûºÏÌõ¼þµÄÄ§·¨ÊôÐÔ£¬¸ù¾Ý
			// 1.ÊÇ·ñÒÑ¾­Ê¹ÓÃ£¬
			// 2.ÔËÆøÖµ
			// 3.ÊÇ·ñÓÐÏàÍ¬µÄ¡°ÊôÐÔµ÷ÕûÀà±ð¡±
			// È¡³öÏàÓ¦µÄÏî·ÅÔÚSelectedMagicTableÖÐ
			INT nCMITItemCount = pCMITItem->GetCount();
			INT j = 0;
			INT nDecide = GetRandomNumber(0, 100 - 1) / (1 + nLucky * 20 / 100);

			for (j = 0; j < nCMITItemCount; j++)
			{
				// ¸ù¾ÝË÷ÒýÖµ£¬È¡³öÏàÓ¦µÄÏî
				INT nMAIndex = pCMITItem->Get(j);
				KMAGICATTRIB_TABFILE* pMAItem = (KMAGICATTRIB_TABFILE*)m_BPTLib.GetMARecord(nMAIndex);
				if (!pMAItem)
				{
					_ASSERT(pMAItem);
					continue;
				}

				// Èç¹ûÒÑ¾­Ê¹ÓÃ£¬Ìø¹ýÈ¡ÏÂÒ»¸ö
				if (pMAItem->m_nUseFlag)
					continue;

				// Èç¹ûÔËÆø²»×ã£¬È¡ÏÂÒ»¸ö
				if ((pMAItem->m_DropRate[nType]) <= nDecide)
					continue;

				// ¼ì²éÊÇ·ñÒÑ¾­ÓÐÏàÍ¬µÄ¡°ÊôÐÔµ÷ÕûÀà±ð¡±
				INT k = 0;
				for (k = 0; k < i; k++)
				{
					_ASSERT(pMagicAttrTable[k]);

					if ((pMagicAttrTable[k]->m_MagicAttrib.nPropKind) == (pMAItem->m_MagicAttrib.nPropKind))
						break;
				}

				// Èç¹û·¢ÏÖ ¡°ÊôÐÔµ÷ÕûÀà±ð¡±ÏàÍ¬£¬ËµÃ÷ÒÑ¾­ÓÐÒ»¸öÏàÍ¬µÄÖµ±»Ñ¡ÖÐ£¬ÄÇÃ´È¡ÏÂÒ»¸ö
				if (k < i)
					continue;

				SelectedMagicTable.Insert(nMAIndex);
			}

			INT nSelectedCount = SelectedMagicTable.GetCount();

			if (nSelectedCount == 0)
			{
				// Èç¹ûÃ»ÓÐÂú×ãÌõ¼þµÄÄ§·¨ÊôÐÔ£¬¾ÍÍË³ö
				break;
			}

			// Èç¹û·¢ÏÖÂú×ãÌõ¼þµÄÄ§·¨ÊôÐÔ£¬¾ÍÉèÖÃ
			INT nLuckyItemIndex = GetRandomNumber(0, nSelectedCount - 1);
			KMAGICATTRIB_TABFILE* pMAItem = (KMAGICATTRIB_TABFILE*)m_BPTLib.GetMARecord(
				SelectedMagicTable.Get(nLuckyItemIndex)
			);

			// ±£´æÂú×ãÌõ¼þµÄÄ§·¨ÊôÐÔ£¬ÒÔ±ãÔÚ×îºóµÄÊ±ºò½«Ê¹ÓÃ±êÖ¾Çå³ý
			pMagicAttrTable[i] = pMAItem;

			_ASSERT(pMAItem);
			_ASSERT(!(pMAItem->m_nUseFlag));     // È·¶¨»¹Ã»ÓÐÊ¹ÓÃ

			// ÉèÖÃÊ¹ÓÃ±êÖ¾
			pMAItem->m_nUseFlag = true;

			pnaryMA[i].nAttribType = pMAItem->m_MagicAttrib.nPropKind;
			pnaryMA[i].nValue[0] = GetRandomNumber(
				pMAItem->m_MagicAttrib.aryRange[0].nMin,
				pMAItem->m_MagicAttrib.aryRange[0].nMax
			);
			pnaryMA[i].nValue[1] = GetRandomNumber(
				pMAItem->m_MagicAttrib.aryRange[1].nMin,
				pMAItem->m_MagicAttrib.aryRange[1].nMax
			);
			pnaryMA[i].nValue[2] = GetRandomNumber(
				pMAItem->m_MagicAttrib.aryRange[2].nMin,
				pMAItem->m_MagicAttrib.aryRange[2].nMax
			);

			INT Min = 1, Max = 0;

			const KBPT_ClassMAIT* pCMITItem1 = m_BPTLib.GetCMIT(1 - (i & 1), nType, nSeriesReq, 10);

			_ASSERT(pCMITItem1);
			if (!pCMITItem1)
				break;

			for (INT z = 0; z < pCMITItem1->GetCount(); z++)
			{
				KMAGICATTRIB_TABFILE* pMAItemMin = (KMAGICATTRIB_TABFILE*)m_BPTLib.GetMARecord(pCMITItem1->Get(z));

				if (pnaryMA[i].nAttribType == pMAItemMin->m_MagicAttrib.nPropKind)
				{
					if (pMAItemMin->m_MagicAttrib.aryRange[0].nMin < Min)
						Min = pMAItemMin->m_MagicAttrib.aryRange[0].nMin;

					if (pMAItemMin->m_MagicAttrib.aryRange[0].nMax > Max)
						Max = pMAItemMin->m_MagicAttrib.aryRange[0].nMax;
				}
			}

			pnaryMA[i].nMin = Min;
			pnaryMA[i].nMax = Max;

		} // for ±éÀúÐèÒªÈ¡»ØµÄÄ§·¨ÊôÐÔ±í
	}
	for (NULL; i < MAX_ITEM_MAGICATTRIB; i++)
	{
		// Çå¿ÕÊ£ÏÂµÄÏî
		pnaryMA[i].nAttribType = 0;
		pnaryMA[i].nValue[0] = 0;
		pnaryMA[i].nValue[1] = 0;
		pnaryMA[i].nValue[2] = 0;
	}

	for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
	{
		if (!pMagicAttrTable[i])
			break;

		_ASSERT(pMagicAttrTable[i]->m_nUseFlag);   // È·¶¨ÒÑ¾­Ê¹ÓÃ

		pMagicAttrTable[i]->m_nUseFlag = false;    // ÉèÖÃÎªÃ»ÓÃÊ¹ÓÃ
	}

	nResult = true;
	//Exit0:
	return nResult;
}


// Following Code is implement by DongBo
//BOOL KItemGenerator::Gen_MagicAttrib(INT nType, const INT* pnaryMALevel, INT nSeriesReq, INT nLucky,
//									 KItemNormalAttrib* pnaryMA)
//{
//	
//    // È·¶¨ÊÊÓÃÓÚ¸Ã×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔ¼°ÆäÊýÁ¿
//	if (FALSE == GMA_GetAvaliableMA(nType))
//		{ _ASSERT(FALSE); return FALSE; }
//
//	// È·¶¨ÉÏÊö¸÷Ä§·¨ÊôÐÔµÄµÈ¼¶,³öÏÖ¸ÅÂÊ
//	if (FALSE == GMA_GetLevelAndDropRate(nType))
//		{ _ASSERT(FALSE); return FALSE; }
//
//	// ´ÓÉÏÊöÄ§·¨ÊôÐÔÖÐÑ¡³ö·ûºÏÌõ¼þµÄ. ²ß»®ÒªÇóÑ¡³ö6¸ö
//	for (INT i = 0; i < 6; i++)				// ÆæÊý: Ç°×º, Å¼Êý: ºó×º
//	{
//		GMA_ChooseMA((i&1), pnaryMALevel[i], nLucky, &(pnaryMA[i]));
//		if (pnaryMA[i].nAttribType == 0)
//			break;
//	}
//	return TRUE;
//}

/******************************************************************************
	¹¦ÄÜ:	È·¶¨ÊÊÓÃÓÚÖ¸¶¨×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔ¼°ÆäÊýÁ¿
	Èë¿Ú:	nType: Ö¸¶¨×°±¸
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã. m_GMA_naryMA[0]¸ø³öÊÊÓÃÓÚ¸Ã×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔÇ°×º
							m_GMA_nCount[0]¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄ×ÜÊý
							m_GMA_naryMA[1]¸ø³öÊÊÓÃÓÚ¸Ã×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔºó×º
							m_GMA_nCount[1]¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄ×ÜÊý
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::GMA_GetAvaliableMA(INT nType)
{
	//TODO: ¿ÉÄÜÐèÒª¶ÔnType½øÐÐ×ª»», ´Ó×°±¸ÀàÐÍÖµ±ä³É m_BPTLib.m_CMAT Êý×éµÄË÷Òý
	for (INT i = 0; i < 2; i++)		// 0: Ç°×º, 1: ºó×º
	{
		const KBPT_ClassifiedMAT* pCMAT;
		pCMAT = m_BPTLib.GetCMAT(i, nType);
		if (pCMAT == NULL)
		{
			_ASSERT(FALSE); return FALSE;
		}
		m_GMA_nCount[i] = NUMOFCMA;
		pCMAT->GetAll((INT*)m_GMA_naryMA[i], &(m_GMA_nCount[i]));
	}
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ:	È·¶¨¸÷¸ø¶¨Ä§·¨ÊôÐÔµÄµÈ¼¶ÒªÇóºÍ³öÏÖ¸ÅÂÊ
	Èë¿Ú:	nType: Ö¸¶¨×°±¸
			m_GMA_naryMAÊý×é: ¸ø³öÈ«²¿Ä§·¨ÊôÐÔµÄË÷ÒýÖµ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã. m_GMA_naryLevelÊý×é¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄµÈ¼¶ÒªÇó
							m_GMA_naryDropRate¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄ³öÏÖ¸ÅÂÊ
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::GMA_GetLevelAndDropRate(INT nType)
{
	//TODO: ¿ÉÄÜÐèÒª¶ÔnType½øÐÐ×ª»», ´Ó×°±¸ÀàÐÍÖµ±ä³É m_BPTLib.m_CMAT Êý×éµÄË÷Òý

	for (INT i = 0; i < 2; i++)		// 0: Ç°×º, 1: ºó×º
	{
		for (INT n = 0; n < m_GMA_nCount[i]; n++)
		{
			const KMAGICATTRIB_TABFILE* pMATF;
			pMATF = GetMARecord(m_GMA_naryMA[i][n]);
			if (pMATF == NULL)
			{
				_ASSERT(FALSE); return FALSE;
			}
			m_GMA_naryLevel[i][n] = pMATF->m_nLevel;
			m_GMA_naryDropRate[i][n] = pMATF->m_DropRate[nType];		// ¿ÉÓÅ»¯
		}
	}
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ:	´ÓÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔÖÐÑ¡³ö·ûºÏÌõ¼þµÄÄ§·¨
	Èë¿Ú:	nPos: 0: Ñ¡Ç°×º, 1: Ñ¡ºó×º
			nLevel: ¶ÔÄ§·¨ÊôÐÔµÄµÈ¼¶ÒªÇó
			nLucky: ÔËÆøÖµ
			m_GMA_naryMAÊý×é: ¸ø³öÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔµÄË÷ÒýÖµ
	³ö¿Ú:	*pINA ¸ø³öËùÑ¡Ä§·¨µÄºËÐÄ²ÎÊý
******************************************************************************/
void KItemGenerator::GMA_ChooseMA(INT nPos, INT nLevel, INT nLucky, KItemNormalAttrib* pINA)
{
	_ASSERT(pINA != NULL);

	// ¸³³õÖµ
	pINA->nAttribType = 0;
	pINA->nValue[0] = 0;
	pINA->nValue[1] = 0;
	pINA->nValue[2] = 0;

	// É¸Ñ¡³öÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔ
	INT nDropRate = ::GetRandomNumber(0, 100) - nLucky;
	INT nCount = GMA_GetCandidateMA(nPos, nLevel, nDropRate);
	if (nCount > 0)
	{
		INT nFinal = ::GetRandomNumber(0, nCount);
		INT nMAi = m_GMA_naryCandidateMA[nFinal];
		INT nMA = m_GMA_naryMA[nPos][nMAi];
		if (nMA == -1)	// Ëæ»ú³öÖØ¸´µÄÊôÐÔÁË
		{
			pINA->nAttribType = 0;
			return;
		}
		m_GMA_naryMA[nPos][nMAi] = -1;		// ÖÃÎª-1,±íÊ¾¸ÃMAÒÑÓÃ
		const KMAGICATTRIB_TABFILE* pMATF = GetMARecord(nMA);
		if (NULL != pMATF)
		{
			const KMACP* pMACP = &(pMATF->m_MagicAttrib);
			pINA->nAttribType = pMACP->nPropKind;
			pINA->nValue[0] = ::GetRandomNumber(pMACP->aryRange[0].nMin, pMACP->aryRange[0].nMax);
			pINA->nValue[1] = ::GetRandomNumber(pMACP->aryRange[1].nMin, pMACP->aryRange[1].nMax);
			pINA->nValue[2] = ::GetRandomNumber(pMACP->aryRange[2].nMin, pMACP->aryRange[2].nMax);
		}
	}
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÄ§·¨ÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	i: Ö¸¶¨¼ÇÂ¼
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KMAGICATTRIB_TABFILE* KItemGenerator::GetMARecord(INT i) const
{
	return m_BPTLib.GetMARecord(i);
}

/******************************************************************************
	¹¦ÄÜ:	´ÓÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔÖÐÑ¡³ö·ûºÏÌõ¼þµÄÄ§·¨
	Èë¿Ú:	nPos: 0: Ñ¡Ç°×º, 1: Ñ¡ºó×º
			nLevel: ¶ÔÄ§·¨ÊôÐÔµÄµÈ¼¶ÒªÇó
			nDropRate: Ñ¡³ö³öÏÖ¸ÅÂÊ´óÓÚ´ËÖµµÄÄ§·¨ÊôÐÔ
			m_GMA_naryMAÊý×é: ¸ø³öÈ«²¿¿ÉÓÃµÄÄ§·¨ÊôÐÔµÄË÷ÒýÖµ
			m_GMA_naryLevelÊý×é: ¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄµÈ¼¶ÒªÇó
			m_GMA_naryDropRateÊý×é: ¸ø³öÉÏÊöÄ§·¨ÊôÐÔµÄ³öÏÖ¸ÅÂÊ
	³ö¿Ú:	m_GMA_naryCandidateMAÊý×é¸ø³ö·ûºÏÌõ¼þµÄÄ§·¨
	ËµÃ÷:	Èô n = m_GMA_naryMA[nPos][m_GMA_naryCandidateMA[i]]
			Ôò GetMARecord(n) ·µ»ØÖ¸ÏòÄ§·¨ÊôÐÔ½á¹¹µÄÖ¸Õë
******************************************************************************/
INT KItemGenerator::GMA_GetCandidateMA(INT nPos, INT nLevel, INT nDropRate)
{
	INT nCount = 0;
	for (INT i = 0; i < m_GMA_nCount[nPos]; i++)
	{
		if (m_GMA_naryMA[nPos][i] != -1 &&		// -1±íÊ¾¸ÃMAÒÑÓÃ¹ý
			m_GMA_naryLevel[nPos][i] == nLevel &&
			m_GMA_naryDropRate[nPos][i] >= nDropRate)
		{
			m_GMA_naryCandidateMA[nCount++] = i;
		}
	}
	return nCount;
}

BOOL KItemGenerator::GetMedicineCommonAttrib(IN INT nDetailType, IN INT nLevel, IN OUT KItem* pItem)
{
	if (NULL == pItem)
	{
		KASSERT(FALSE);
		return FALSE;
	}

	const INT i = nDetailType * 5 + nLevel - 1;

	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if (NULL == pMed)
	{
		_ASSERT(FALSE);
		return FALSE;
	}
	*pItem = *pMed;
	return TRUE;
}
/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉÖ¸¶¨»Æ½ð×°±¸×°±¸
	Èë¿Ú:	nCondition: Ç°ÌáÌõ¼þ£¬(Emf+Pmf)*LevelµÄÔËËã½á¹û
			pItem: ×°±¸ÏîÄ¿Ö¸Õë¡£
	³ö¿Ú:	³É¹¦Ê±·µ»ØÕæ, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»Ø¼Ù
******************************************************************************/
BOOL KItemGenerator::GetEquipmentCommonAttrib(IN INT nDetailType, IN INT nParticularType, IN INT nLevel, IN INT nSeries, IN OUT KItem* pItem)
{
	if (NULL == pItem)
	{
		KASSERT(FALSE);
		return FALSE;
	}

	INT i;
	if (nDetailType > equip_horse)
		i = nParticularType;
	else
		i = nParticularType * 10 + nLevel - 1;// ±¾¹«Ê½ÓÉ²ß»®¶¨Òå
	// Ïê¼ûSPE 2002Äê8ÔÂ31ÈÕ7:40
	// email: »ù´¡µÀ¾ßÊýÖµ±í
	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch (nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_mantle:
		pEqu = m_BPTLib.GetMantleRecord(i);
		break;
	case equip_signet:
		pEqu = m_BPTLib.GetSignetRecord(i);
		break;
	case equip_shipin:
		pEqu = m_BPTLib.GetShipinRecord(i);
		break;
	case equip_hoods:
		pEqu = m_BPTLib.GetHoodsRecord(i);
		break;
	case equip_cloak:
		pEqu = m_BPTLib.GetCloakRecord(i);
		break;
	default:
		break;
	}
	if (NULL == pEqu)
	{
		_ASSERT(FALSE); return FALSE;
	}
	// ÔËÐÐÖÁ´ËµÄÖ±½ÓÔ­Òò: Ö»ÓÐnÖÖ×°±¸, ¶øÉÏÃæ i µÄÖµÔÚ[0,n-1]Ö®Íâ
	// ¼ì²é3µã: nParticularType ÓÐÎó?
	//			nLevel ÓÐÎó?
	//			Ô­Ê¼µÄtab fileÓÐÎóµ¼ÖÂ m_BPTLib.m_BPTEquipment Ëù
	//			¹ÜÀíµÄÊý¾ÝÓÐÎÊÌâ?
	pItem->SetAttrib_CBR(pEqu);
	if (nSeries < series_metal)
		pItem->SetSeries(::GetRandomNumber(series_metal, series_earth));
	else
		pItem->SetSeries(nSeries);
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ£º	Éú³ÉËæ»ú×°±¸
	Èë¿Ú:	nDetailType: ¾ßÌåÀà±ð, Èç½ü³ÌÎäÆ÷¡¢°µÆ÷¡¢......
			nParticularType: ÏêÏ¸Àà±ð
			nSeriesReq: ÎåÐÐÊôÐÔ
			nLevel: µÈ¼¶
			pnaryMALevel: Ä§·¨ÊôÐÔµÈ¼¶Êý×é[6]
			nLucky: ÔËÆøÖµ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, Ïà¹ØÊý¾ÝÔÚ pItem ËùÖ¸¶ÔÏóÖÐ¸ø³ö
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItemGenerator::Gen_ExistEquipment(IN INT nItemNature,
	IN INT nDetailType,
	IN INT nParticularType,
	IN INT nSeriesReq,
	IN INT nLevel,
	IN const INT* pnaryMALevel,
	IN INT nLucky,
	IN INT nVersion,
	IN OUT KItem* pItem
)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	pItem->m_CommonAttrib.nItemNature = nItemNature;
	switch (nItemNature)
	{
	case NATURE_GOLD:
	{
		this->Gen_GoldEquipment(nDetailType, pnaryMALevel, nLucky, pItem);
		pItem->SetRow(nDetailType);
		return TRUE;
	}
	break;
	case NATURE_PLATINA:
	{
		this->Gen_PlatinaEquipment(nDetailType, pnaryMALevel, nLucky, pItem, nLevel);
		pItem->SetRow(nDetailType);
		pItem->SetLevel(nLevel);
		return TRUE;
	}
	break;
	default:
	{
		BOOL bEC = FALSE;

		g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
		if (pnaryMALevel)
			memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(INT) * MAX_ITEM_MAGICLEVEL);
		INT nGameVersion = pItem->m_GeneratorParam.nVersion;

		// ¸ù¾ÝÈë¿Ú²ÎÊý, È·¶¨×°±¸µÄ»ù±¾Êý¾Ý
		INT i = 0;
		if (nDetailType == equip_mask)
			i = nParticularType;
		else
			i = nParticularType * 10 + nLevel - 1;// ±¾¹«Ê½ÓÉ²ß»®¶¨Òå
		// Ïê¼ûSPE 2002Äê8ÔÂ31ÈÕ7:40
		// email: »ù´¡µÀ¾ßÊýÖµ±í
		// µÈ¼¶Êý¾Ý´Ó1¿ªÊ¼£¬ËùÒÔÒª¼õ»ØÀ´
		const KBASICPROP_EQUIPMENT* pEqu = NULL;
		switch (nDetailType)
		{
		case equip_meleeweapon:
			pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
			break;
		case equip_rangeweapon:
			pEqu = m_BPTLib.GetRangeWeaponRecord(i);
			break;
		case equip_armor:
			pEqu = m_BPTLib.GetArmorRecord(i);
			break;
		case equip_helm:
			pEqu = m_BPTLib.GetHelmRecord(i);
			break;
		case equip_boots:
			pEqu = m_BPTLib.GetBootRecord(i);
			break;
		case equip_belt:
			pEqu = m_BPTLib.GetBeltRecord(i);
			break;
		case equip_amulet:
			pEqu = m_BPTLib.GetAmuletRecord(i);
			break;
		case equip_ring:
			pEqu = m_BPTLib.GetRingRecord(i);
			break;
		case equip_cuff:
			pEqu = m_BPTLib.GetCuffRecord(i);
			break;
		case equip_pendant:
			pEqu = m_BPTLib.GetPendantRecord(i);
			break;
		case equip_horse:
			pEqu = m_BPTLib.GetHorseRecord(i);
			break;
		case equip_mask:
			pEqu = m_BPTLib.GetMaskRecord(i);
			break;
		case equip_mantle:
			pEqu = m_BPTLib.GetMantleRecord(i);
			break;
		case equip_signet:
			pEqu = m_BPTLib.GetSignetRecord(i);
			break;
		case equip_shipin:
			pEqu = m_BPTLib.GetShipinRecord(i);
			break;
		case equip_hoods:
			pEqu = m_BPTLib.GetHoodsRecord(i);
			break;
		case equip_cloak:
			pEqu = m_BPTLib.GetCloakRecord(i);
			break;
		default:
			break;
		}
		if (NULL == pEqu)
		{
			_ASSERT(FALSE); return bEC;
		}
		// ÔËÐÐÖÁ´ËµÄÖ±½ÓÔ­Òò: Ö»ÓÐnÖÖ×°±¸, ¶øÉÏÃæ i µÄÖµÔÚ[0,n-1]Ö®Íâ
		// ¼ì²é3µã: nParticularType ÓÐÎó?
		//			nLevel ÓÐÎó?
		//			Ô­Ê¼µÄtab fileÓÐÎóµ¼ÖÂ m_BPTLib.m_BPTEquipment Ëù
		//			¹ÜÀíµÄÊý¾ÝÓÐÎÊÌâ?
		pItem->SetAttrib_CBR(pEqu);
		if (nSeriesReq < series_metal)
			pItem->SetSeries(::GetRandomNumber(series_metal, series_earth));
		else
			pItem->SetSeries(nSeriesReq);

		if (NULL == pnaryMALevel)
			return TRUE;
		KItemNormalAttrib	sMA[MAX_ITEM_MAGICATTRIB];	// µÀ¾ßµÄÄ§·¨ÊôÐÔ
		bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
		if (bEC)
			pItem->SetAttrib_MA(sMA);

		pItem->SetNature(nItemNature);
		return bEC;
	}
	break;
	}
}


BOOL KItemGenerator::Gen_Quest(IN INT nDetailType, IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));

	const KBASICPROP_QUEST* pQuest = NULL;
	pQuest = m_BPTLib.GetQuestRecord(nDetailType);
	if (NULL == pQuest)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pQuest;

	return TRUE;
}

BOOL KItemGenerator::Gen_TownPortal(IN INT nDetailType, IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));

	const KBASICPROP_TOWNPORTAL* pPortal = NULL;
	pPortal = m_BPTLib.GetTownPortalRecord(nDetailType);
	if (NULL == pPortal)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pPortal;
	return TRUE;
}

BOOL KItemGenerator::Gen_MagicScript(IN INT nDetailType, IN OUT KItem* pItem, IN INT nLevel, IN INT nSeries, IN INT nLuck)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));

	const KBASICPROP_MAGICSCRIPT* pMagicScript = NULL;
	pMagicScript = m_BPTLib.GetMagicScript(nDetailType);
	if (NULL == pMagicScript)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pMagicScript;
	if (nSeries < series_metal)
		pItem->SetSeries(::GetRandomNumber(series_metal, series_earth));
	else
		pItem->SetSeries(nSeries);
	pItem->SetLevel(nLevel);
	pItem->m_GeneratorParam.nLuck = nLuck;
	return TRUE;
}

BOOL KItemGenerator::Gen_Event(IN INT nDetailType, IN OUT KItem* pItem)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));

	const KBASICPROP_EVENTITEM* pEvent = NULL;
	pEvent = m_BPTLib.GetEvent(nDetailType);
	if (NULL == pEvent)
	{
		_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pEvent;
	return TRUE;
}
