//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemGenerator.h
// Date:	2002.08.26
// Code:	DongBo
// Desc:    header file. 本文件定义的类用于生成道具
//---------------------------------------------------------------------------

#ifndef	KItemGeneratorH
#define	KItemGeneratorH

#include "KBasPropTbl.h"
#include "KItem.h"

#define		IN
#define		OUT

#define	NUMOFCMA	150		// 经验值. 供每种装备使用的魔法总数不超过此数

//class KItem;

class KItemGenerator
{
public:
	KItemGenerator();
	~KItemGenerator();

	// 以下是核心成员变量
protected:
	KLibOfBPT	m_BPTLib;

	// 以下是辅助成员变量
	INT			m_GMA_naryMA[2][NUMOFCMA];	// [0][x]: 前缀; [1][x]: 后缀
	INT			m_GMA_nCount[2];
	INT			m_GMA_naryLevel[2][NUMOFCMA];
	INT			m_GMA_naryDropRate[2][NUMOFCMA];
	INT			m_GMA_naryCandidateMA[NUMOFCMA];
	INT			m_EquipNumOfEntries[equip_detailnum];
	INT			m_MedNumOfEntries;
	// 以下是对外接口
public:
	BOOL Init();
	BOOL Gen_Quest(IN INT, IN OUT KItem*);
	BOOL Gen_TownPortal(IN INT, IN OUT KItem*);
	BOOL Gen_MagicScript(IN INT, IN OUT KItem*, IN INT, IN INT, IN INT);
	BOOL Gen_Event(IN INT, IN OUT KItem*);
	BOOL Gen_Medicine(IN INT, IN INT, IN INT, IN OUT KItem*);
	BOOL Gen_Equipment(IN INT, IN INT, IN INT, IN INT, IN INT, IN const INT*, IN INT,
		IN INT, IN OUT KItem*);
	BOOL Gen_ExistEquipment(IN INT, IN INT, IN INT, IN INT, IN INT, IN const INT*, IN INT,
		IN INT, IN OUT KItem*);
	BOOL Gen_GoldEquipment(IN INT, IN const INT*, IN INT, OUT KItem*);
	BOOL Gen_PlatinaEquipment(IN INT, IN const INT*, IN INT, OUT KItem*, IN INT);
	BOOL GetEquipmentCommonAttrib(IN INT, IN INT, IN INT, IN INT, IN OUT KItem*);
	BOOL GetMedicineCommonAttrib(IN INT, IN INT, IN OUT KItem*);

	// 以下是辅助函数
private:
	BOOL Gen_MagicAttrib(INT, const INT*, INT, INT, KItemNormalAttrib*, INT nVersion);
	const KMAGICATTRIB_TABFILE* GetMARecord(INT) const;
	BOOL GMA_GetAvaliableMA(INT);
	BOOL GMA_GetLevelAndDropRate(INT);
	INT  GMA_GetCandidateMA(INT, INT, INT);
	void GMA_ChooseMA(INT nPos, INT nLevel, INT nLucky, KItemNormalAttrib* pINA);
};

extern KItemGenerator	ItemGen;			//	装备生成器
#endif	// #ifndef	KItemGeneratorH
