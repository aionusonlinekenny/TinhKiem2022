//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcDeathCalcExp.h
// Date:	2003.07.21
// Code:	±ß³ÇÀË×Ó
// Desc:	KNpcDeathCalcExp Class
//---------------------------------------------------------------------------

#ifndef KNPCDEATHCALCEXP_H
#define KNPCDEATHCALCEXP_H

#define		defMAX_CALC_EXP_NUM		3
#define		defMAX_CALC_EXP_TIME	1200

typedef struct
{
	INT		m_nAttackIdx;
	INT		m_nTotalDamage;
	INT		m_nTime;
} KCalcExpInfo;

class KNpcDeathCalcExp
{
	friend class KNpc;
private:
	INT				m_nNpcIdx;
	KCalcExpInfo	m_sCalcInfo[defMAX_CALC_EXP_NUM];
public:
	void			Init(INT nNpcIdx);
	void			Active();
#ifdef _SERVER
	void			AddDamage(INT nPlayerIdx, INT nDamage);
	INT				CalcExp();
#endif
	void			Clear();
};

#endif
