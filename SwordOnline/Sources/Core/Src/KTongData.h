#ifndef KTongDataH
#define	KTongDataH

struct KTongMap
{
	char szName[80];
	char szPath[MAX_PATH];
};

class KTongLevelAdd
{
private:
	INT		m_nExpVal[MAX_TONG_LEVEL];				// 升级经验
	INT		m_nEffVal[MAX_TONG_LEVEL];			// 每个系升级加生命点
	INT		m_nMemberNum[MAX_TONG_LEVEL];			// 每个系升级加生命点
public:
	KTongLevelAdd();
	BOOL		Init();
	INT			GetExp(INT nLevel);	
	INT			GetEff(INT nLevel);			// 传入等级获得可带队员数
	INT			GetMemNum(INT nLevel);			// 传入经验获得可带队员数
};

class KOneTong
{
private:
	DWORD		m_dwNameID;
	KTongMap	m_sMap;

public:
	KOneTong();
	BOOL		Init();
	BOOL		AddTong();
};


class KTongData
{
private:
#ifdef	_SERVER
	INT						m_nNumTong;
public:
	KOneTong				m_sTong[MAX_TONG_COUNT];
#endif

public:
	KTongParam				m_sTongParam;
	KTongLevelAdd			m_sTongLevel;

	KTongData();
	BOOL	Init();
};

extern KTongData TongData;
#endif //KTongDataH
