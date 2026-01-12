// DBBackup.h: interface for the CDBBackup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DBBACKUP_H)
#define DBBACKUP_H

#include "DBTable.h"
#include "S3DBInterface.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULTSTATNUM 10
#define LEVELSTATNUM 1000
#define MONEYSTATNUM 10
#define KILLERSTATNUM 10
#define SECTLEVELSTATNUM 10
#define SECTMONEYSTATNUM 10

class CDBBackup
{
public:
	struct TRoleList
	{
		char Name[32];
		//char Account[32];
		int32_t Level;
		int32_t	Translife;
		int32_t Money;
		int32_t Sect;			//ÃÅÅÉ
		int32_t FightExp;
		int32_t KillNum;
		int32_t Repute;
		int32_t	FuYuan;
		int32_t	AccumStat1;
		int32_t	AccumStat2;
		int32_t	HonorStat;
		int32_t	TimeStat;
		BYTE TongLv;
		int32_t	TongMn;
		DWORD	TongEff;
		BYTE Sort;
	};
	/*struct TStatData
	{
		TRoleList LevelStat[LEVELSTATNUM];			//¼¶±ð×î¶àÅÅÃûÁÐ±í
		TRoleList MoneyStat[MONEYSTATNUM];			//½ðÇ®×î¶àÅÅÃûÁÐ±í
		TRoleList KillerStat[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList ReputeStat[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList FuYuanStat[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList AccumStat1[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList AccumStat2[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList HonorStat[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList TimeStat[DEFAULTSTATNUM];
		TRoleList TongLvStat[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList TongMnStat[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList TongEffStat[DEFAULTSTATNUM];			//É±ÈË×î¶àÅÅÃûÁÐ±í
		TRoleList MoneyStatBySect[MAX_FACTION + 1][SECTMONEYSTATNUM];	//¸÷ÃÅÅÉ½ðÇ®×î¶àÅÅÃûÁÐ±í[ÃÅÅÉ][Íæ¼ÒÊý]
		TRoleList LevelStatBySect[MAX_FACTION + 1][SECTLEVELSTATNUM];	//¸÷ÃÅÅÉ¼¶±ð×î¶àÅÅÃûÁÐ±í[ÃÅÅÉ][Íæ¼ÒÊý]
		int32_t SectPlayerNum[MAX_FACTION + 1];				//¸÷¸öÃÅÅÉµÄÍæ¼ÒÊý
		int32_t SectMoneyMost[MAX_FACTION + 1];				//²Æ¸»ÅÅÃûÇ°MONEYSTATNUMÍæ¼ÒÖÐ¸÷ÃÅÅÉËùÕ¼±ÈÀýÊý
		int32_t SectLevelMost[MAX_FACTION + 1];				//¼¶±ðÅÅÃûÇ°LEVELSTATNUMÍæ¼ÒÖÐ¸÷ÃÅÅÉËùÕ¼±ÈÀýÊý

	};*/

	struct TStatData
	{
		TRoleList* LevelStat;                 // Sử dụng con trỏ thay vì mảng
		TRoleList* MoneyStat;
		TRoleList* KillerStat;
		TRoleList* ReputeStat;
		TRoleList* FuYuanStat;
		TRoleList* AccumStat1;
		TRoleList* AccumStat2;
		TRoleList* HonorStat;
		TRoleList* TimeStat;
		TRoleList* TongLvStat;
		TRoleList* TongMnStat;
		TRoleList* TongEffStat;
		TRoleList** MoneyStatBySect;
		TRoleList** LevelStatBySect;
		int32_t* SectPlayerNum;
		int32_t* SectMoneyMost;
		int32_t* SectLevelMost;

		TStatData()
		{
			LevelStat = new TRoleList[LEVELSTATNUM];
			MoneyStat = new TRoleList[MONEYSTATNUM];
			KillerStat = new TRoleList[DEFAULTSTATNUM];
			ReputeStat = new TRoleList[DEFAULTSTATNUM];
			FuYuanStat = new TRoleList[DEFAULTSTATNUM];
			AccumStat1 = new TRoleList[DEFAULTSTATNUM];
			AccumStat2 = new TRoleList[DEFAULTSTATNUM];
			HonorStat = new TRoleList[DEFAULTSTATNUM];
			TimeStat = new TRoleList[DEFAULTSTATNUM];
			TongLvStat = new TRoleList[DEFAULTSTATNUM];
			TongMnStat = new TRoleList[DEFAULTSTATNUM];
			TongEffStat = new TRoleList[DEFAULTSTATNUM];

			MoneyStatBySect = new TRoleList * [MAX_FACTION + 1];
			LevelStatBySect = new TRoleList * [MAX_FACTION + 1];
			for (int i = 0; i <= MAX_FACTION; ++i)
			{
				MoneyStatBySect[i] = new TRoleList[SECTMONEYSTATNUM];
				LevelStatBySect[i] = new TRoleList[SECTLEVELSTATNUM];
			}

			SectPlayerNum = new int32_t[MAX_FACTION + 1];
			SectMoneyMost = new int32_t[MAX_FACTION + 1];
			SectLevelMost = new int32_t[MAX_FACTION + 1];
		}

		~TStatData()
		{
			delete[] LevelStat;
			delete[] MoneyStat;
			delete[] KillerStat;
			delete[] ReputeStat;
			delete[] FuYuanStat;
			delete[] AccumStat1;
			delete[] AccumStat2;
			delete[] HonorStat;
			delete[] TimeStat;
			delete[] TongLvStat;
			delete[] TongMnStat;
			delete[] TongEffStat;

			for (int i = 0; i <= MAX_FACTION; ++i)
			{
				delete[] MoneyStatBySect[i];
				delete[] LevelStatBySect[i];
			}
			delete[] MoneyStatBySect;
			delete[] LevelStatBySect;

			delete[] SectPlayerNum;
			delete[] SectMoneyMost;
			delete[] SectLevelMost;
		}

		void Reset()
		{
			// Đặt lại các mảng bên trong về giá trị ban đầu
			memset(LevelStat, 0, LEVELSTATNUM * sizeof(TRoleList));
			memset(MoneyStat, 0, MONEYSTATNUM * sizeof(TRoleList));
			memset(KillerStat, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(ReputeStat, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(FuYuanStat, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(AccumStat1, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(AccumStat2, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(HonorStat, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(TimeStat, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(TongLvStat, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(TongMnStat, 0, DEFAULTSTATNUM * sizeof(TRoleList));
			memset(TongEffStat, 0, DEFAULTSTATNUM * sizeof(TRoleList));

			for (int i = 0; i <= MAX_FACTION; ++i)
			{
				memset(MoneyStatBySect[i], 0, SECTMONEYSTATNUM * sizeof(TRoleList));
				memset(LevelStatBySect[i], 0, SECTLEVELSTATNUM * sizeof(TRoleList));
			}

			memset(SectPlayerNum, 0, (MAX_FACTION + 1) * sizeof(int32_t));
			memset(SectMoneyMost, 0, (MAX_FACTION + 1) * sizeof(int32_t));
			memset(SectLevelMost, 0, (MAX_FACTION + 1) * sizeof(int32_t));
		}
	};
	static void GetLastBackupPath(char* buf, size_t bufSize);
	static bool WasLastBackupOk();
private:
	enum StatType { stMoney, stLevel, stKiller, stRepute, stFuYuan, stAccumStat1, stAccumStat2, stHonorStat, stTimeStat, stTongLv, stTongMn, stTongEff };//ÐèÒªÅÐ¶ÏµÄÀà±ð£¨stMoney, stLevel£©

	static TRoleList* GetMin(			// Find the element with the least money/level (or other) in the list
		TRoleList* const aRoleList,	//ÁÐ±í
		const int32_t n,				//ÁÐ±í´óÐ¡
		const StatType aType,
		const char* TongName = 0);		//ÐèÒªÅÐ¶ÏµÄÀà±ð£¨stMoney, stLevel£©

	static TRoleList* GetMax(			// Find the element with the least money/level (or other) in the list
		TRoleList* const aRoleList,	//ÁÐ±í
		const int32_t n,				//ÁÐ±í´óÐ¡
		const StatType aType,
		const char* TongName = 0);		//ÐèÒªÅÐ¶ÏµÄÀà±ð£¨stMoney, stLevel£©

	static void ListSort(			// Sort the list in a specific way
		TRoleList* const aRoleList,
		const int32_t n,
		const StatType aType,
		const bool bSortZA = false);
	static int32_t GetIndexByName(char* aName, TRoleList* aList, int32_t aListSize);
	static void RoleDataCopy(TRoleList* Desc, TRoleData* Source, bool bTong = false);//°ÑRoleDataÓÐÓÃµÄ½á¹¹¸´ÖÆµ½RoleList½á¹¹ÖÐ
private:
	HANDLE m_hThread;
	HANDLE m_hManualThread;
	static void Backup();			//±¸·ÝºÍÐ´ÈëÍ³¼ÆÊý¾Ý
	static void MakeSendStatData();	//Éú³É·¢ËÍ¸ø¿Í»§¶ËµÄÍ³¼ÆÊý¾Ý½á¹¹
	static DWORD WINAPI TimerThreadFunc(LPVOID lpParam);	//±¸·Ý¼ÆÊ±Ïß³Ì
	static DWORD WINAPI ManualThreadFunc(LPVOID lpParam);	//ÊÖ¹¤±¸·ÝÏß³Ì
	static bool LoadStatData(const char* filePath, CDBBackup::TStatData& statData);
	static bool SaveStatData(const char* filePath, const CDBBackup::TStatData& statData);
public:
	CDBBackup(char* aPath, char* aName, ZDBTable* aRunTable);
	virtual ~CDBBackup() {};
	bool Open(int32_t aTime, DWORD bTime);		//Æô¶¯±¸·Ý¼ÆÊ±
	bool OpenEveryMinutes(DWORD minutes);      // New
	bool Suspend();						//¹ÒÆð
	bool Resume();						//¼ÌÐøÖ´ÐÐÏß³Ì
	bool Close();						//¹Ø±Õ
	TGAME_STAT_DATA GetSendStatData();	//È¡µÃ·¢ËÍ¸ø¿Í»§¶ËµÄÍ³¼ÆÊý¾Ý½á¹¹
	bool IsWorking();					//Ïß³ÌÊÇ·ñÔÚ¹¤×÷
	bool IsBackuping();				//±¸·ÝÊÇ·ñÔÚ¹¤×÷
	bool ManualBackup();				//ÊÖ¹¤±¸·Ý
	static void SaveStatInfo();			//°ÑÓÎÏ·ÊÀ½çµÈ¼¶ÅÅÃûÐ´µ½Ö¸¶¨Íæ¼Ò½ÇÉ«ÖÐ
};

#endif // !defined(DBBACKUP_H)
