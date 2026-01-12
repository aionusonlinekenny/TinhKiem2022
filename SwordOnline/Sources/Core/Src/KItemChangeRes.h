//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemChangeRes.h
// Date:	2002.12
// Code:	Spe
// Desc:	Header File
//---------------------------------------------------------------------------

#ifndef	KItemChangeResH
#define	KItemChangeResH

#include "KTabFile.h"

class KItemChangeRes
{
private:
	KTabFile	m_MeleeWeapon;
	KTabFile	m_RangeWeapon;
	KTabFile	m_Armor;
	KTabFile	m_Helm;
	KTabFile	m_Horse;
	KTabFile	m_Gold;
	KTabFile	m_Platina;
	KTabFile	m_Hoods;
	KTabFile	m_Cloak;
public:
	BOOL		Init();
	INT			GetWeaponRes(INT nDetail, INT nParti, INT nLevel, UINT uFlash);
	INT			GetArmorRes(INT nParti, INT nLevel);
	INT			GetHelmRes(INT nParti, INT nLevel);
	INT			GetHorseRes(INT nParti, INT nLevel);
	INT			GetGoldItemRes(INT nDetail, INT nRow, UINT uFlash = 0);
	INT			GetPlatinaItemRes(INT nDetail, INT nRow, UINT uFlash = 0);
	INT			GetHoodsRes(INT nParti);
	INT			GetCloakRes(INT nParti);
};

extern KItemChangeRes	g_ItemChangeRes;
#endif
