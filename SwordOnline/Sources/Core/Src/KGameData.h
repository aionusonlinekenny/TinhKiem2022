#ifndef KGameDataH
#define	KGameDataH

#define 	MAX_DATA_INT				100

enum GAMEDATA
{
	GameDataInt,
	GameDataGr,
	GameDataNum,
};

class KGameData
{
public:
#ifdef	_SERVER
	char 			m_szSave[MAX_DATA_INT][16];
	KDataGroup		m_sDataGroup[MAX_PLAYER];
	INT 			m_nNumGroup;

public:
	KGameData();
	BOOL	Init();

	INT 	GetDataInt(INT nData);
	const char* GetDataStr(INT nData);
	void 	SetData(INT nNo, char* szValue);
	INT		AddDataGr(KDataGroup* pInfo);
	void	SetDataGr(INT nGroup, KDataGroup* pInfo);
	INT		FindFreeDataGr();
	INT		FindDataId(DWORD dwId);
	INT 	GetDataGr_dwName(INT nNo);
	INT 	GetDataGr_nValue(INT nNo, INT nIndex);
	char* GetStrDataGr_Name1(INT nNo);
	char* GetStrDataGr_Name2(INT nNo);
	BOOL	InitGameData();
	BOOL	Save();
#endif
};

extern KGameData GameData;
#endif //KGameDataH
