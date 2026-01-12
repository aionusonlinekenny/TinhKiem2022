//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KBasPropTbl.h
// Date:	2002.08.14
// Code:	DongBo
// Desc:    header file. 本文件定义的类用于从tab file中读出道具的初始属性,
//			并生成对应的属性表
//---------------------------------------------------------------------------

#ifndef	KBasPropTblH
#define	KBasPropTblH

#define		SZBUFLEN_0	80		// 典型的字符串缓冲区长度
#define		SZBUFLEN_1	1024		// 典型的字符串缓冲区长度

#define		MAX_MAGIC_PREFIX	20
#define		MAX_MAGIC_SUFFIX	20
// 以下结构用于描述矿石的基本属性. 相关属性由配置文件(tab file)提供
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nItemGenre;				// 道具种类
	INT			m_nDetailType;				// 具体类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	INT			m_nObjIdx;					// 对应物件索引
	INT			m_nWidth;					// 道具栏中所占宽度
	INT			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	char		m_szScript[SZBUFLEN_1];		// 说明文字
	INT			m_nPrice;					// 价格
	BOOL		m_bShortKey;
	INT			m_nMaxStack;
} KBASICPROP_EVENTITEM;

// 以下结构用于描述药品属性的特性：数值与时间
typedef struct
{
	INT			nAttrib;
	INT			nValue;
	INT			nTime;
} KMEDATTRIB;

// 以下结构用于描述药品的基本属性. 相关属性由配置文件(tab file)提供
// 适用于以下药品: 生命补充类,内力补充类,体力补充类,毒药类,解毒类,
//					解燃烧类,解冰冻类
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nItemGenre;				// 道具种类
	INT			m_nDetailType;				// 具体类别
	INT			m_nParticularType;			// 详细类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	INT			m_nObjIdx;					// 对应物件索引
	INT			m_nWidth;					// 道具栏中所占宽度
	INT			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	INT			m_nSeries;					// 五行属性
	INT			m_nPrice;					// 价格
	INT			m_nLevel;					// 等级
	INT			m_nMaxStack;
	KMEDATTRIB	m_aryAttrib[2];				// 药品的属性
} KBASICPROP_MEDICINE;

// 以下结构用于描述一对最大,最小值
typedef struct
{
	INT			nMin;
	INT			nMax;
} KMINMAXPAIR;

// 以下结构用于给出装备的核心参数: 基础属性
typedef struct
{
	INT			nType;						// 属性类型
	KMINMAXPAIR	sRange;						// 取值范围
} KEQCP_BASIC;	// Equipment_CorePara_Basic

// 以下结构用于给出装备的核心参数: 需求属性
typedef struct
{
	INT			nType;						// 属性类型
	INT			nPara;						// 数值
} KEQCP_REQ;	// Equipment_CorePara_Requirment

// 以下结构用于给出魔法的核心参数
typedef struct
{
	INT			nPropKind;					// 修改的属性类型（对同一个数值加百分比和加点数被认为是两个属性）
	KMINMAXPAIR	aryRange[3];				// 修改属性所需的几个参数
} KMACP;	// MagicAttrib_CorePara

// 以下结构用于描述配置文件中给出的魔法属性. 相关属性由配置文件(tab file)提供
// Add by Freeway Chen in 2003.5.30
#define			MATF_CBDR		    11      // 物品类型 type(现在的值为 equip_detailnum)
#define         MATF_PREFIXPOSFIX   2       // 前缀后缀
#define         MATF_SERIES         5       // 五行
#define         MATF_LEVEL          10      // 最多有10个级别

typedef struct
{
	INT			m_nPos;						// 前缀还是后缀
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nClass;					// 五行要求
	INT			m_nLevel;					// 等级要求
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	KMACP		m_MagicAttrib;				// 核心参数
	INT			m_DropRate[MATF_CBDR];		// 出现概率
	//add by Freeway Chen in 2003.5.30
	INT         m_nUseFlag;                 // 该魔法是否被使用过
} KMAGICATTRIB_TABFILE;

// 以下结构用于描述魔法属性. 相关属性由配置文件(tab file)提供
/*
typedef struct
{
	INT			m_nPos;						// 前缀还是后缀
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nClass;					// 五行要求
	INT			m_nLevel;					// 等级要求
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	KMACP		m_MagicAttrib;				// 核心参数
	INT			m_DropRate;					// 出现概率
} KMAGICATTRIB;
*/
// 以下结构用于描述装备的初始属性. 相关数据由配置文件(tab file)提供
typedef struct
{
	char			m_szName[SZBUFLEN_0];		// 名称
	INT				m_nItemGenre;				// 道具种类 (武器? 药品? 矿石?)
	INT				m_nDetailType;				// 具体类别
	INT				m_nParticularType;			// 详细类别
	char			m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	INT				m_nObjIdx;					// 对应物件索引
	INT				m_nWidth;					// 道具栏中所占宽度
	INT				m_nHeight;					// 道具栏中所占高度
	char			m_szIntro[SZBUFLEN_1];		// 说明文字
	INT				m_nSeries;					// 五行属性
	INT				m_nPrice;					// 价格
	INT				m_nLevel;					// 等级
	KEQCP_BASIC		m_aryPropBasic[7];			// 基础属性
	KEQCP_REQ		m_aryPropReq[6];			// 需求属性
} KBASICPROP_EQUIPMENT;


// 以下结构用于描述唯一装备的初始属性. 相关数据由配置文件(tab file)提供
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nItemGenre;				// 道具种类 (武器? 药品? 矿石?)
	INT			m_nDetailType;				// 具体类别
	INT			m_nParticularType;			// 详细类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	INT			m_nObjIdx;					// 对应物件索引
	INT			m_nWidth;					// 物品栏宽度
	INT			m_nHeight;					// 物品栏高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	INT			m_nSeries;					// 五行属性
	INT			m_nPrice;					// 价格
	INT			m_nLevel;					// 等级	
	KEQCP_BASIC	m_aryPropBasic[7];			// 基础属性
	KEQCP_REQ	m_aryPropReq[6];			// 需求属性
	INT			m_aryMagicAttribs_0[MAX_ITEM_MAGICATTRIB];		// 魔法属性
	INT			m_aryMagicAttribs_10[MAX_ITEM_MAGICATTRIB];		// 魔法属性
	INT			m_nGroup;						// 所在套装
	INT			m_nSetID;					// 所属序号
	INT			m_nNeedToActive1;					// 扩展套装
	INT			m_nNeedToActive2;					// 扩展套装
	INT			ExpandMagic1;					// 扩展套装
	INT			ExpandMagic2;					// 扩展套装
} KBASICPROP_EQUIPMENT_PLATINA;
// 以下结构用于描述黄金装备的初始属性. 相关数据由配置文件(tab file)提供
// flying 根据策划需求修改自KBASICPROP_EQUIPMENT_PLATINA类型
// Fix By Minh Kiem
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nItemGenre;				// 道具种类 (武器? 药品? 矿石?)
	INT			m_nDetailType;				// 具体类别
	INT			m_nParticularType;			// 详细类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	INT			m_nObjIdx;					// 对应物件索引
	INT			m_nWidth;					// 物品栏宽度
	INT			m_nHeight;					// 物品栏高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	INT			m_nSeries;					// 五行属性
	INT			m_nPrice;					// 价格
	INT			m_nLevel;					// 等级	
	KEQCP_BASIC	m_aryPropBasic[7];			// 基础属性
	KEQCP_REQ	m_aryPropReq[6];			// 需求属性
	INT			m_aryMagicAttribs[MAX_ITEM_MAGICATTRIB];		// 魔法属性
	INT			m_nGroup;						// 所在套装
	INT			m_nSetID;					// 所属序号
	INT			m_nNeedToActive1;					// 扩展套装
	INT			m_nNeedToActive2;					// 扩展套装
} KBASICPROP_EQUIPMENT_GOLD;

typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nItemGenre;				// 道具种类
	INT			m_nDetailType;				// 具体类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	INT			m_nObjIdx;					// 对应物件索引
	INT			m_nWidth;					// 道具栏中所占宽度
	INT			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	INT			m_nPrice;
	BOOL		m_bShortKey;
	INT			m_nMaxStack;					// 是否可叠放
	BOOL		m_bLockDrop;					// 是否可叠放
	BOOL		m_bLockTrade;					// 是否可叠放
	BOOL		m_bLockSell;					// 是否可叠放
} KBASICPROP_QUEST;

typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nItemGenre;				// 道具种类
	INT			m_nDetailType;				// 具体类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	INT			m_nObjIdx;					// 对应物件索引
	INT			m_nWidth;					// 道具栏中所占宽度
	INT			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	char		m_szScript[SZBUFLEN_1];		// 说明文字
	INT			m_nPrice;
	BOOL		m_bShortKey;
	INT			m_nMaxStack;					// 是否可叠放
} KBASICPROP_TOWNPORTAL;

typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	INT			m_nItemGenre;				// 道具种类
	INT			m_nDetailType;				// 具体类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	INT			m_nObjIdx;					// 对应物件索引
	INT			m_nWidth;					// 道具栏中所占宽度
	INT			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	char		m_szScript[SZBUFLEN_1];		// 说明文字s
	INT			m_nPrice;					// 价格
	BOOL		m_bShortKey;
	INT			m_nMaxStack;
} KBASICPROP_MAGICSCRIPT;
//=============================================================================

class KBasicPropertyTable			// 缩写: BPT,用于派生类
{
public:
	KBasicPropertyTable();
	~KBasicPropertyTable();

	// 以下是核心成员变量
protected:
	void* m_pBuf;						// 指向属性表缓冲区的指针
	// 属性表是一个结构数组,
	// 其具体类型由派生类决定
	INT			m_nNumOfEntries;			// 属性表含有多少项数据

	// 以下是辅助性的成员变量
	INT         m_nSizeOfEntry;				// 每项数据的大小(即结构的大小)
	char		m_szTabFile[MAX_PATH];		// tabfile的文件名

	// 以下是对外接口
public:
	virtual BOOL Load();					// 从tabfile中读出初始属性值, 填入属性表
	INT NumOfEntries() const { return m_nNumOfEntries; }

	// 以下是辅助函数
protected:
	BOOL GetMemory();
	void ReleaseMemory();
	void SetCount(INT);
	virtual BOOL LoadRecord(INT i, KTabFile* pTF) = 0;
};

class KBPT_Event : public KBasicPropertyTable
{
public:
	KBPT_Event();
	~KBPT_Event();

public:
	const KBASICPROP_EVENTITEM* GetRecord(IN INT) const;
	const KBASICPROP_EVENTITEM* FindRecord(IN INT) const;
protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
};

class KBPT_TownPortal : public KBasicPropertyTable
{
public:
	KBPT_TownPortal();
	~KBPT_TownPortal();

	// 以下是对外接口
public:
	const KBASICPROP_TOWNPORTAL* GetRecord(IN INT) const;

protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
};

// =====>药材<=====
// =====>药品<=====
class KBPT_Medicine : public KBasicPropertyTable
{
public:
	KBPT_Medicine();
	~KBPT_Medicine();

	// 以下是对外接口
public:
	const KBASICPROP_MEDICINE* GetRecord(IN INT) const;
	const KBASICPROP_MEDICINE* FindRecord(IN INT, IN INT) const;

	// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
};

// =====>任务物品<=====
class KBPT_Quest : public KBasicPropertyTable
{
public:
	KBPT_Quest();
	~KBPT_Quest();

	// 以下是对外接口
public:
	const KBASICPROP_QUEST* GetRecord(IN INT) const;
	const KBASICPROP_QUEST* FindRecord(IN INT) const;

protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
};

class KBPT_MagicScript : public KBasicPropertyTable
{
public:
	KBPT_MagicScript();
	~KBPT_MagicScript();

	// 以下是对外接口
public:
	const KBASICPROP_MAGICSCRIPT* GetRecord(IN INT) const;
	const KBASICPROP_MAGICSCRIPT* FindRecord(IN INT) const;

protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
};

class KBPT_Equipment : public KBasicPropertyTable
{
public:
	KBPT_Equipment();
	~KBPT_Equipment();

	// 以下是对外接口
public:
	const KBASICPROP_EQUIPMENT* GetRecord(IN INT) const;
	const KBASICPROP_EQUIPMENT* FindRecord(IN INT, IN INT, IN INT) const;
	void Init(IN INT);
	// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
};

class KBPT_Equipment_Platina : public KBasicPropertyTable
{
public:
	KBPT_Equipment_Platina();
	virtual ~KBPT_Equipment_Platina();

	// 以下是对外接口
public:
	const KBASICPROP_EQUIPMENT_PLATINA* GetRecord(IN INT) const;
	const KBASICPROP_EQUIPMENT_PLATINA* FindRecord(IN INT, IN INT, IN INT) const;
	INT GetRecordCount() const { return KBasicPropertyTable::NumOfEntries(); };
	void Init();
	// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
};

// 黄金装备
class KBPT_Equipment_Gold : public KBasicPropertyTable
{
public:
	KBPT_Equipment_Gold();
	virtual ~KBPT_Equipment_Gold();

	// 以下是对外接口
public:
	const KBASICPROP_EQUIPMENT_GOLD* GetRecord(IN INT) const;
	const KBASICPROP_EQUIPMENT_GOLD* FindRecord(IN INT, IN INT, IN INT) const;
	INT GetRecordCount() const { return KBasicPropertyTable::NumOfEntries(); };
	void Init();
	// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
};

class KBPT_MagicAttrib_TF : public KBasicPropertyTable
{
public:
	KBPT_MagicAttrib_TF();
	~KBPT_MagicAttrib_TF();

	// 以下是辅助成员变量
protected:
	INT m_naryMACount[2][MATF_CBDR];	// 每种装备可适用的魔法数目,分前后缀进行统计
	// 共有MATF_CBDR种装备可以具备魔法
// 以下是对外接口
public:
	void GetMACount(INT*) const;
	const KMAGICATTRIB_TABFILE* GetRecord(IN INT) const;

	// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(INT i, KTabFile* pTF);
	void Init();
};
/*
class KBPT_MagicAttrib : public KBasicPropertyTable
{
public:
	KBPT_MagicAttrib();
	~KBPT_MagicAttrib();

// 以下是辅助函数
protected:
};
*/

//============================================================================

// Add by Freeway Chen in 2003.5.30
class KBPT_ClassMAIT    // Magic Item Index Table
{
public:
	KBPT_ClassMAIT();
	~KBPT_ClassMAIT();

	// 以下是核心成员变量
protected:
	INT* m_pnTable;				// 缓冲区指针, 所存数据为
	// KBPT_MagicAttrib_TF::m_pBuf数组的下标
	INT		m_nSize;				// 缓冲区内含多少项数据(并非字节数)

	// 以下是辅助成员变量
	INT		m_nNumOfValidData;		// 缓冲区中有效数据的个数
	// 初始化工作完成后m_nNumOfValidData < m_nSize
// 以下是对外接口
public:
	BOOL Clear();
	BOOL Insert(INT nItemIndex);
	INT  Get(INT i) const;
	INT  GetCount() const { return m_nNumOfValidData; }
};

//============================================================================

class KBPT_ClassifiedMAT
{
public:
	KBPT_ClassifiedMAT();
	~KBPT_ClassifiedMAT();

	// 以下是核心成员变量
protected:
	INT* m_pnTable;				// 缓冲区指针, 所存数据为
	// KBPT_MagicAttrib_TF::m_pBuf数组的下标
	INT		m_nSize;				// 缓冲区内含多少项数据(并非字节数)

	// 以下是辅助成员变量
	INT		m_nNumOfValidData;		// 缓冲区中有效数据的个数
	// 初始化工作完成后m_nNumOfValidData==m_nSize
// 以下是对外接口
public:
	BOOL GetMemory(INT);
	BOOL Set(INT);
	INT Get(INT) const;
	BOOL GetAll(INT*, INT*) const;

	// 以下是辅助函数
protected:
	void ReleaseMemory();
};

class KLibOfBPT
{
public:
	KLibOfBPT();
	~KLibOfBPT();

	// 以下是核心成员变量
protected:
	KBPT_Medicine			m_BPTMedicine;
	KBPT_MagicScript		m_BPTMagicScript;
	KBPT_Event				m_BPTEvent;
	KBPT_Quest				m_BPTQuest;
	KBPT_TownPortal			m_BPTTownPortal;
	KBPT_Equipment			m_BPTHorse;
	KBPT_Equipment			m_BPTMeleeWeapon;
	KBPT_Equipment			m_BPTRangeWeapon;
	KBPT_Equipment			m_BPTArmor;
	KBPT_Equipment			m_BPTHelm;
	KBPT_Equipment			m_BPTBoot;
	KBPT_Equipment			m_BPTBelt;
	KBPT_Equipment			m_BPTAmulet;
	KBPT_Equipment			m_BPTRing;
	KBPT_Equipment			m_BPTCuff;
	KBPT_Equipment			m_BPTPendant;
	KBPT_Equipment			m_BPTMask;
	KBPT_Equipment			m_BPTMantle;
	KBPT_Equipment			m_BPTSignet;
	KBPT_Equipment			m_BPTShipin;
	KBPT_Equipment			m_BPTHoods;
	KBPT_Equipment			m_BPTCloak;

	KBPT_MagicAttrib_TF		m_BPTMagicAttrib;
	// Add By Minh Kiem
	KBPT_Equipment_Gold		m_BPTGoldEquip;
	KBPT_Equipment_Platina	m_BPTPlatinaEquip;
	// Add by Freeway Chen in 2003.5.30
	// 四维分别为前后缀、物品类型、五行、级别
	KBPT_ClassMAIT          m_CMAIT[MATF_PREFIXPOSFIX][MATF_CBDR][MATF_SERIES][MATF_LEVEL];

	KBPT_ClassifiedMAT		m_CMAT[2][MATF_CBDR];

	// 以下是对外接口
public:
	BOOL Init();

	const KMAGICATTRIB_TABFILE* GetMARecord(IN INT) const;
	const INT					GetMARecordNumber() const;

	// Add by Freeway Chen in 2003.5.30
	const KBPT_ClassMAIT* GetCMIT(IN INT nPrefixPostfix, IN INT nType, IN INT nSeries, INT nLevel) const;

	const KBPT_ClassifiedMAT* GetCMAT(IN INT, INT) const;
	// Add By Minh Kiem
	const KBASICPROP_EQUIPMENT_GOLD* GetGoldEquipRecord(IN INT nIndex) const;
	const INT							GetGoldEquipNumber() const;

	const KBASICPROP_EQUIPMENT_PLATINA* GetPlatinaEquipRecord(IN INT nIndex) const;
	const INT							GetPlatinaEquipNumber() const;

	const KBASICPROP_EQUIPMENT* GetMeleeWeaponRecord(IN INT) const;
	const INT					GetMeleeWeaponRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetRangeWeaponRecord(IN INT) const;
	const INT					GetRangeWeaponRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetArmorRecord(IN INT) const;
	const INT					GetArmorRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetHelmRecord(IN INT) const;
	const INT					GetHelmRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetBootRecord(IN INT) const;
	const INT					GetBootRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetBeltRecord(IN INT) const;
	const INT					GetBeltRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetAmuletRecord(IN INT) const;
	const INT					GetAmuletRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetRingRecord(IN INT) const;
	const INT					GetRingRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetCuffRecord(IN INT) const;
	const INT					GetCuffRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetPendantRecord(IN INT) const;
	const INT					GetPendantRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetHorseRecord(IN INT) const;
	const INT					GetHorseRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetMaskRecord(IN INT) const;
	const INT					GetMaskRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetMantleRecord(IN INT) const;
	const INT					GetMantleRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetSignetRecord(IN INT) const;
	const INT					GetSignetRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetShipinRecord(IN INT) const;
	const INT					GetShipinRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetHoodsRecord(IN INT) const;
	const INT					GetHoodsRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetCloakRecord(IN INT) const;
	const INT					GetCloakRecordNumber() const;
	const KBASICPROP_EQUIPMENT_PLATINA* FindEquipmentUnique(IN INT, IN INT, IN INT) const;
	const KBASICPROP_MEDICINE* GetMedicineRecord(IN INT) const;
	const INT					GetMedicineRecordNumber() const;
	const KBASICPROP_MEDICINE* FindMedicine(IN INT, IN INT) const;
	const KBASICPROP_QUEST* GetQuestRecord(IN INT) const;
	const INT					GetQuestRecordNumber() const;
	const KBASICPROP_TOWNPORTAL* GetTownPortalRecord(IN INT) const;
	const INT					GetTownPortalRecordNumber() const;
	const KBASICPROP_MAGICSCRIPT* GetMagicScript(IN INT) const;
	const INT					GetMagicScriptRecordNumber() const;
	const KBASICPROP_EVENTITEM* GetEvent(IN INT) const;
	const INT					GetEventRecordNumber() const;
	// 以下是辅助函数
protected:
	BOOL InitMALib();

	// Add by Freeway Chen in 2003.5.30
	BOOL InitMAIT();
};
#endif		// #ifndef KBasPropTblH
