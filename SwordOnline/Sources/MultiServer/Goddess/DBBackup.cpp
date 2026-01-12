// DBBackup.cpp: implementation of the CDBBackup class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DBTABLE.H"
#include "DBBackup.h"
#include "DBDumpLoad.h"
#include <FSTREAM>
#include <direct.h>
#include "Macro.h"
#include "CRC32.h"
#include <time.h>

using namespace std;

#define IS_OUTPUT_LOG true

static char DBPath[32] = { 0 };						//Êý¾Ý¿âÄ¿Â¼
static char DBName[32] = { 0 };						//Êý¾Ý¿âÃû
static ZDBTable* RunTable = NULL;

static bool IsBackupWorking = false;				//ÊÇ·ñÕýÔÚ±¸·Ý
static bool IsThreadWorking = false;				//Ïß³ÌÊÇ·ñÔÚ¹¤×÷
static bool IsSuspended = false;					//Ïß³ÌÊÇ·ñ¹ÒÆð
static WORD BackupTime = 0;				//±¸·Ý¼ä¸ôÊ±¼ä
static DWORD BackupBeginTime = 0;				//±¸·Ý¼ä¸ôÊ±¼ä
static bool  UseIntervalMode = false;  // NEW
static DWORD IntervalMinutes = 0;      // NEW
static bool IsTimeToBackup = true;
static CDBBackup::TStatData oldGameStatData;
static CDBBackup::TStatData newGameStatData;
static TGAME_STAT_DATA SendStatData;				//È¡µÃ·¢ËÍ¸ø¿Í»§¶ËµÄÍ³¼ÆÊý¾Ý½á¹¹
static bool g_LastBackupOk = false;
static char g_LastBackupPath[MAX_PATH] = {0};

int32_t CDBBackup::GetIndexByName(char* aName, TRoleList* aList, int32_t aListSize)
{
	if (aList[0].Name[0] == '\0') return -1;
	for (int32_t i = 0; i < aListSize; ++i)
	{
		if (aList[i].Name[0] == '\0') return -1;
		if (strcmp(aName, aList[i].Name) == 0)
			return i;
	}
	return -1;
}

CDBBackup::TRoleList* CDBBackup::GetMin(TRoleList* const aRoleList, const int32_t n, const StatType aType, const char* TongName)
{//²éÕÒ³öÁÐ±íÖÐÇ®/¼¶±ð£¨»òÆäËû£©×îÉÙµÄÒ»¸öÔªËØ
	TRoleList* tmpData;
	tmpData = &aRoleList[0];
	while (true)
	{
		int32_t i = 0;
		for (i = 0; i < n; ++i)
		{
			if (aRoleList[i].Name[0] == '\0')
				return &aRoleList[i];
			int32_t aSourse = 0, aDest = 0;
			switch (aType)
			{
			case stMoney:
				aDest = tmpData->Money;
				aSourse = aRoleList[i].Money;
				break;
			case stLevel:
				aDest = tmpData->Level;
				aSourse = aRoleList[i].Level;
				break;
			case stKiller:
				aDest = tmpData->KillNum;
				aSourse = aRoleList[i].KillNum;
				break;
			case stRepute:
				aDest = tmpData->Repute;
				aSourse = aRoleList[i].Repute;
				break;
			case stAccumStat1:
				aDest = tmpData->AccumStat1;
				aSourse = aRoleList[i].AccumStat1;
				break;
			case stAccumStat2:
				aDest = tmpData->AccumStat2;
				aSourse = aRoleList[i].AccumStat2;
				break;
			case stHonorStat:
				aDest = tmpData->HonorStat;
				aSourse = aRoleList[i].HonorStat;
				break;
			case stTimeStat:
				aDest = tmpData->TimeStat;
				aSourse = aRoleList[i].TimeStat;
				break;
			case stTongLv:
				if (TongName[0] && (strcmp(TongName, aRoleList[i].Name) == 0))
					return &aRoleList[i];
				aDest = tmpData->TongLv;
				aSourse = aRoleList[i].TongLv;
				break;
			case stTongMn:
				if (TongName[0] && (strcmp(TongName, aRoleList[i].Name) == 0))
					return &aRoleList[i];
				aDest = tmpData->TongMn;
				aSourse = aRoleList[i].TongMn;
				break;
			case stTongEff:
				if (TongName[0] && (strcmp(TongName, aRoleList[i].Name) == 0))
					return &aRoleList[i];
				aDest = tmpData->TongEff;
				aSourse = aRoleList[i].TongEff;
				break;
			}
			if (aDest > aSourse)
			{
				tmpData = &aRoleList[i];
				break;
			}
		}
		if (i == n)break;
	}
	return tmpData;
}

CDBBackup::TRoleList* CDBBackup::GetMax(TRoleList* const aRoleList, const int32_t n, const StatType aType, const char* TongName)
{//²éÕÒ³öÁÐ±íÖÐÇ®/¼¶±ð£¨»òÆäËû£©×îÉÙµÄÒ»¸öÔªËØ
	TRoleList* tmpData;
	tmpData = &aRoleList[0];
	while (true)
	{
		int32_t i = 0;
		for (i = 0; i < n; ++i)
		{
			if (aRoleList[i].Name[0] == '\0')
				return &aRoleList[i];
			int32_t aSourse = 0, aDest = 0;
			switch (aType)
			{
			case stMoney:
				aDest = tmpData->Money;
				aSourse = aRoleList[i].Money;
				break;
			case stLevel:
				aDest = tmpData->Level;
				aSourse = aRoleList[i].Level;
				break;
			case stKiller:
				aDest = tmpData->KillNum;
				aSourse = aRoleList[i].KillNum;
				break;
			case stRepute:
				aDest = tmpData->Repute;
				aSourse = aRoleList[i].Repute;
				break;
			case stAccumStat1:
				aDest = tmpData->AccumStat1;
				aSourse = aRoleList[i].AccumStat1;
				break;
			case stAccumStat2:
				aDest = tmpData->AccumStat2;
				aSourse = aRoleList[i].AccumStat2;
				break;
			case stHonorStat:
				aDest = tmpData->HonorStat;
				aSourse = aRoleList[i].HonorStat;
				break;
			case stTimeStat:
				aDest = tmpData->TimeStat;
				aSourse = aRoleList[i].TimeStat;
				break;
			case stTongLv:
				if (TongName[0] && (strcmp(TongName, aRoleList[i].Name) == 0))
					return &aRoleList[i];
				aDest = tmpData->TongLv;
				aSourse = aRoleList[i].TongLv;
				break;
			case stTongMn:
				if (TongName[0] && (strcmp(TongName, aRoleList[i].Name) == 0))
					return &aRoleList[i];
				aDest = tmpData->TongMn;
				aSourse = aRoleList[i].TongMn;
				break;
			case stTongEff:
				if (TongName[0] && (strcmp(TongName, aRoleList[i].Name) == 0))
					return &aRoleList[i];
				aDest = tmpData->TongEff;
				aSourse = aRoleList[i].TongEff;
				break;
			}
			if (aDest < aSourse)
			{
				tmpData = &aRoleList[i];
				break;
			}
		}
		if (i == n)break;
	}
	return tmpData;
}

bool CDBBackup::OpenEveryMinutes(DWORD minutes)
{
    if ((DBPath[0] == '\0') || (DBName[0] == '\0'))
        return false;

    if (minutes == 0) minutes = 1;

    UseIntervalMode = true;
    IntervalMinutes = minutes;

    BackupTime      = 0;   // không dùng trong interval mode
    BackupBeginTime = 0;   // giữ 0, nếu muốn delay lần đầu có thể set từ ngoài

    DWORD dwThreadId, dwThrdParam = 1;
    m_hThread = CreateThread(NULL, 0, TimerThreadFunc, &dwThrdParam, 0, &dwThreadId);
    if (!m_hThread)
    {
        UseIntervalMode = false;
        IntervalMinutes = 0;
        return false;
    }

    char aAppPath[MAX_PATH] = { 0 };
    char* x = getcwd(aAppPath, MAX_PATH);
    strcat(aAppPath, "\\StatData.dat");

    newGameStatData.Reset();
    LoadStatData(aAppPath, newGameStatData);
    MakeSendStatData();

    return true;
}

void CDBBackup::ListSort(TRoleList* const aRoleList, const int32_t n, const StatType aType, const bool bSortZA)
{//¶ÔÁÐ±í×öÌØ¶¨µÄÅÅÐò
	int32_t i, j;
	for (i = 0; i < n; ++i)
	{
		TRoleList* tmpData = &aRoleList[i];
		for (j = i + 1; j < n; ++j)
		{
			int64_t aSourse = 0, aDest = 0;
			switch (aType)
			{
			case stMoney:
				aDest = tmpData->Money;
				aSourse = aRoleList[j].Money;
				break;
			case stLevel:
				aDest = (int64_t)(tmpData->Level + tmpData->Translife * MAX_LEVEL) * 0xffffffff + (int64_t)tmpData->FightExp;
				aSourse = (int64_t)(aRoleList[j].Level + aRoleList[j].Translife * MAX_LEVEL) * 0xffffffff + (int64_t)aRoleList[j].FightExp;
				break;
			case stKiller:
				aDest = tmpData->KillNum;
				aSourse = aRoleList[j].KillNum;
				break;
			case stRepute:
				aDest = tmpData->Repute;
				aSourse = aRoleList[j].Repute;
				break;
			case stAccumStat1:
				aDest = tmpData->AccumStat1;
				aSourse = aRoleList[j].AccumStat1;
				break;
			case stAccumStat2:
				aDest = tmpData->AccumStat2;
				aSourse = aRoleList[j].AccumStat2;
				break;
			case stHonorStat:
				aDest = tmpData->HonorStat;
				aSourse = aRoleList[j].HonorStat;
				break;
			case stTimeStat:
				aDest = tmpData->TimeStat;
				aSourse = aRoleList[j].TimeStat;
				break;
			case stTongLv:
				aDest = tmpData->TongLv;
				aSourse = aRoleList[j].TongLv;
				break;
			case stTongMn:
				aDest = tmpData->TongMn;
				aSourse = aRoleList[j].TongMn;
				break;
			case stTongEff:
				aDest = tmpData->TongEff;
				aSourse = aRoleList[j].TongEff;
				break;
			}
			if (bSortZA)
			{
				if (aSourse < aDest)
					swap(*tmpData, aRoleList[j]);
			}
			else
			{
				if (aSourse > aDest)
					swap(*tmpData, aRoleList[j]);
			}
		}
	}
}

void CDBBackup::RoleDataCopy(TRoleList* Desc, TRoleData* Source, bool bTong/*= false*/)
{//°ÑRoleDataÓÐÓÃµÄ½á¹¹¸´ÖÆµ½RoleList½á¹¹ÖÐ
	//strcpy(Desc->Account, Source->BaseInfo.caccname);
	if (bTong)
		strcpy(Desc->Name, Source->BaseInfo.itongname);
	else
		strcpy(Desc->Name, Source->BaseInfo.szName);
	Desc->Sect = Source->BaseInfo.nSect;
	Desc->Money = Source->BaseInfo.imoney + Source->BaseInfo.isavemoney;
	Desc->Level = Source->BaseInfo.ifightlevel;
	Desc->Translife = Source->BaseInfo.ifighttranslife;
	Desc->FightExp = Source->BaseInfo.ifightexp;
	Desc->KillNum = Source->BaseInfo.nKillPeopleNumber;
	Desc->Repute = Source->BaseInfo.istattask[TASKVALUE_STATTASK_REPUTE - TASKVALUE_STATTASK_BEGIN];
	Desc->FuYuan = Source->BaseInfo.istattask[TASKVALUE_STATTASK_FUYUAN - TASKVALUE_STATTASK_BEGIN];
	Desc->AccumStat1 = Source->BaseInfo.istattask[TASKVALUE_STATTASK_ACCUMSTAT1 - TASKVALUE_STATTASK_BEGIN];
	Desc->AccumStat2 = Source->BaseInfo.istattask[TASKVALUE_STATTASK_ACCUMSTAT2 - TASKVALUE_STATTASK_BEGIN];
	Desc->HonorStat = Source->BaseInfo.istattask[TASKVALUE_STATTASK_HONORSTAT - TASKVALUE_STATTASK_BEGIN];
	Desc->TimeStat = Source->BaseInfo.istattask[TASKVALUE_STATTASK_TIMESTAT - TASKVALUE_STATTASK_BEGIN];
	Desc->TongLv = Source->BaseInfo.itonglevel;
	Desc->TongMn = Source->BaseInfo.itongmemnum;
	Desc->TongEff = Source->BaseInfo.itongeff;
}

CDBBackup::CDBBackup(char* aPath, char* aName, ZDBTable* aRunTable)
{
	memset(DBPath, 0, 32);
	memset(DBName, 0, 32);
	strcpy(DBPath, aPath);
	strcpy(DBName, aName);
	BackupTime = 0;
	BackupBeginTime = 0;
	m_hThread = NULL;
	m_hManualThread = NULL;
	RunTable = aRunTable;

	//memset(&oldGameStatData, 0, sizeof(TStatData));
	//memset(&newGameStatData, 0, sizeof(TStatData));
	oldGameStatData.Reset();
	newGameStatData.Reset();
	memset(&SendStatData, 0, sizeof(TGAME_STAT_DATA));
}

bool CDBBackup::Open(int32_t aTime, DWORD bTime)
{
	aTime = aTime % 24;
	if ((DBPath[0] == '\0') || (DBName[0] == '\0'))
		return false;//³õÊ¼»¯ÓÐÎÊÌâ¾ÍÍË³ö

	BackupTime = aTime;
	BackupBeginTime = bTime;
	DWORD dwThreadId, dwThrdParam = 1;
	m_hThread = CreateThread(
		NULL,                        // no security attributes 
		0,                           // use default stack size  
		TimerThreadFunc,             // thread function 
		&dwThrdParam,                // argument to thread function 
		0,                           // use default creation flags 
		&dwThreadId);                // returns the thread identifier 
	if (!m_hThread)
	{
		BackupTime = 0;
		BackupBeginTime = 0;
		return false;//´´½¨Ïß³ÌÊ§°Ü
	}

	char aAppPath[MAX_PATH] = { 0 };
	char* x = getcwd(aAppPath, MAX_PATH);
	strcat(aAppPath, "\\StatData.dat");
	//¶ÁÈ¡¾ÉÅÅÃû
	//memset(&newGameStatData, 0, sizeof(CDBBackup::TStatData));
	newGameStatData.Reset();
	/*FILE* aStatFile = fopen(aAppPath, "rb");
	if (aStatFile)
	{
		int32_t a = fread(&newGameStatData, 1, sizeof(CDBBackup::TStatData), aStatFile);
		fclose(aStatFile);
	}*/

	LoadStatData(aAppPath, newGameStatData);
	MakeSendStatData();

	return true;
}

bool CDBBackup::Close()
{
	BackupTime = 0;
	BackupBeginTime = 0;
	UseIntervalMode = false;     // NEW
	IntervalMinutes = 0;         // NEW
	if (m_hManualThread) CloseHandle(m_hManualThread);
	return (CloseHandle(m_hThread) == TRUE);

}

bool CDBBackup::Suspend()
{
	if (!m_hThread) return false;	//Èç¹ûÏß³ÌÃ»ÓÐ³õÊ¼»¯ºÃ¾Í²»ÄÜ¹ÒÆð
	if (!IsThreadWorking) return false;	//Èç¹ûÏß³ÌÃ»ÓÐ¿ªÊ¼¾Í²»ÄÜ¹ÒÆð
	if (IsBackupWorking) return false;	//Èç¹ûÕýÔÚ±¸·Ý¾ÍÖÐÖ¹¹ÒÆð
	if (IsSuspended) return false;	//Èç¹ûÕýÔÚ±¸·Ý¾ÍÖÐÖ¹¹ÒÆð
	if (SuspendThread(m_hThread) == -1)return false;
	IsSuspended = true;
	return true;
}

bool CDBBackup::Resume()
{//¼ÌÐøÖ´ÐÐÏß³Ì
	if (!m_hThread) return false;	//Èç¹ûÏß³ÌÃ»ÓÐ³õÊ¼»¯ºÃ¾Í²»ÄÜ¹ÒÆð
	if (!IsSuspended) return false;	//Èç¹ûÕýÔÚ±¸·Ý¾ÍÖÐÖ¹¹ÒÆð
	if (!IsThreadWorking) return false;	//Èç¹ûÏß³ÌÃ»ÓÐ¿ªÊ¼¾Í²»ÄÜ¼ÌÐøÖ´ÐÐÏß³Ì

	if (ResumeThread(m_hThread) == -1)return false;
	IsSuspended = false;
	return true;
}

TGAME_STAT_DATA CDBBackup::GetSendStatData()
{//È¡µÃ·¢ËÍ¸ø¿Í»§¶ËµÄÍ³¼ÆÊý¾Ý½á¹¹
	return SendStatData;
}

bool CDBBackup::IsWorking()
{//Ïß³ÌÊÇ·ñÔÚ¹¤×÷
	return IsThreadWorking;
}

bool CDBBackup::IsBackuping()
{//±¸·ÝÊÇ·ñÔÚ¹¤×÷
	return IsBackupWorking;
}

bool CDBBackup::ManualBackup()
{//手工备份
    // Không cho chạy chồng
    if (IsBackupWorking) return false;

    DWORD dwThreadId, dwThrdParam = 1;
    m_hManualThread = CreateThread(
        NULL, 0, ManualThreadFunc, &dwThrdParam, 0, &dwThreadId);
    if (!m_hManualThread)
        return false;

    return true;
}
/*
DWORD WINAPI CDBBackup::TimerThreadFunc(LPVOID lpParam)
{//±¸·Ý¼ÆÊ±Ïß³Ì
	IsThreadWorking = true;
	while (true)
	{
		time_t rawtime;
		struct tm* timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		if (timeinfo->tm_hour == BackupTime)
		{
			if (IsTimeToBackup)
			{
				Backup();
				SaveStatInfo();
			}
			IsTimeToBackup = false;
		}
		else
		{
			IsTimeToBackup = true;
		}

		Sleep(1000 * 60 * 30);
		//Sleep(BackupTime);
	}
	IsThreadWorking = false;
	return 0;
}
*/
DWORD WINAPI CDBBackup::TimerThreadFunc(LPVOID lpParam)
{
    IsThreadWorking = true;
    while (true)
    {
        time_t rawtime;
        struct tm* timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        if (UseIntervalMode)
        {
            Backup();
            SaveStatInfo();
        }
        else
        {
            if (timeinfo->tm_hour == BackupTime)
            {
                if (IsTimeToBackup)
                {
                    Backup();
                    SaveStatInfo();
                }
                IsTimeToBackup = false;
            }
            else
            {
                IsTimeToBackup = true;
            }
        }


        if (UseIntervalMode && IntervalMinutes > 0)
            Sleep(1000 * 60 * IntervalMinutes); // phút -> ms
        else
            Sleep(1000 * 60 * 30);
    }

    IsThreadWorking = false;
    return 0;
}

DWORD WINAPI CDBBackup::ManualThreadFunc(LPVOID lpParam)
{//ÊÖ¹¤±¸·ÝÏß³Ì
	Backup();
	SaveStatInfo();
	return 0;
}

void CDBBackup::Backup()
{
	IsBackupWorking = true;

	//´ò¿ª±¸·Ý×´Ì¬logÎÄ¼þ
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	/*char aLogPath[MAX_PATH] = { 0 };
	char* x = getcwd(aLogPath, MAX_PATH);

	char aFileName[64];
	sprintf(aFileName, "\\goddess_log\\Backup_%02d-%02d-%02d_%04d-%02d-%02d.log",
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
	strcat(aLogPath, aFileName);

	fstream aLogFile(aLogPath, ios::out);*/
	char aLogPath[MAX_PATH] = { 0 };
	char* x = getcwd(aLogPath, MAX_PATH);

	// Tạo thư mục log: .\goddess_log
	char logDir[MAX_PATH] = {0};
	strcpy(logDir, aLogPath);
	strcat(logDir, "\\goddess_log");
	_mkdir(logDir); // bỏ qua nếu đã tồn tại

	char aFileName[128];
	sprintf(aFileName, "\\Backup_%02d-%02d-%02d_%04d-%02d-%02d.log",
			timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
			timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);

	strcpy(aLogPath, logDir);
	strcat(aLogPath, aFileName);

	fstream aLogFile(aLogPath, ios::out);

	SYSTEMTIME aSysTime;
	GetSystemTime(&aSysTime);
	string aBackupDir;
	char aIntStr[10];
	itoa(aSysTime.wYear, aIntStr, 10);
	aBackupDir = aIntStr;
	itoa(aSysTime.wMonth, aIntStr, 10);
	aBackupDir += aIntStr;
	itoa(aSysTime.wDay, aIntStr, 10);
	aBackupDir += aIntStr;
	aBackupDir += "_";
	itoa(aSysTime.wHour, aIntStr, 10);
	aBackupDir += aIntStr;
	itoa(aSysTime.wMinute, aIntStr, 10);
	aBackupDir += aIntStr;
	itoa(aSysTime.wSecond, aIntStr, 10);
	aBackupDir += aIntStr;
	itoa(aSysTime.wMilliseconds, aIntStr, 10);
	aBackupDir += aIntStr;
	aLogFile << "Backup Log File Open Time:" << timeinfo->tm_year + 1900 << "." <<
		timeinfo->tm_mon + 1 << "." << timeinfo->tm_mday << " " <<
		timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << endl;
	int32_t i, j;
	// Xây đường dẫn chuẩn
	char cwd[MAX_PATH] = {0};
	_getcwd(cwd, MAX_PATH);

	// 1) Root dir: .\database (DBPath)
	char rootDir[MAX_PATH] = {0};
	strcpy(rootDir, cwd);
	strcat(rootDir, "\\");
	strcat(rootDir, DBPath);
	_mkdir(rootDir);

	// 2) Thư mục backup theo timestamp: .\database\<YYYYMMDD_HHMMSSms>
	char backupDir[MAX_PATH] = {0};
	strcpy(backupDir, rootDir);
	strcat(backupDir, "\\");
	strcat(backupDir, aBackupDir.c_str());
	_mkdir(backupDir);

	// 3) File thống kê .txt: .\database\<ts>\<ts>.txt
	char txtPath[MAX_PATH] = {0};
	strcpy(txtPath, backupDir);
	strcat(txtPath, "\\");
	strcat(txtPath, aBackupDir.c_str());
	strcat(txtPath, ".txt");
	fstream aDBSOutput(txtPath, ios::out);
	aDBSOutput << "Tai khoan\tTen nhan vat\tMon phai\tTrung sinh\tDang cap\tNgan luong" << endl;
    int32_t aDBSPlayerCount = 0;                                   // ⇦ THÊM LẠI Ở ĐÂY
    int32_t aDBSSectPlayerCount[MAX_FACTION + 2] = { 0 };          // ⇦ THÊM LẠI Ở ĐÂY
    double  aDBSMoneyCount = 0;                                    // ⇦ THÊM LẠI Ở ĐÂY
    double  aDBSSectMoneyCount[MAX_FACTION + 2] = { 0 };           // ⇦ THÊM LẠI Ở ĐÂY
    int32_t aDBSLevelPlayerCount[MAX_LEVEL + 1] = { 0 };           // ⇦ THÊM LẠI Ở ĐÂY
	// 4) File dump .bak: .\database\<ts>\<ts>.bak
	char dumpPath[MAX_PATH] = {0};
	strcpy(dumpPath, backupDir);
	strcat(dumpPath, "\\");
	strcat(dumpPath, aBackupDir.c_str());
	strcat(dumpPath, ".bak");

	// Reset kết quả lần backup
	g_LastBackupOk = false;
	g_LastBackupPath[0] = '\0';

	CDBDump DBDump;
	if (!DBDump.Open(dumpPath)) {
		aLogFile << "Open dump failed: " << dumpPath << endl;
		IsBackupWorking = false;
		return;
	}

	/*
	char aSavePath[MAX_PATH] = { 0 };
	x = getcwd(aSavePath, MAX_PATH);
	strcat(aSavePath, "\\");
	strcat(aSavePath, DBPath);
	strcat(aSavePath, "\\");
#ifdef WIN32
	int32_t y = mkdir(aSavePath);
#else
	mkdir(aSavePath, 0);
#endif
	strcat(aSavePath, aBackupDir.c_str());
	strcat(aSavePath, "\\");
#ifdef WIN32
	y = mkdir(aSavePath);
#else
	mkdir(aSavePath, 0);
#endif
	strcat(aSavePath, aBackupDir.c_str());

	//Êý¾Ý¿â¼ÇÂ¼Í³¼Æ£¨Î¬»¤²é¿´ÓÃ£©==========Add by Fellow,2003.08.26
	char aDBSFullPath[MAX_PATH] = { 0 };
	strcpy(aDBSFullPath, aSavePath);
	strcat(aDBSFullPath, ".txt");
	fstream aDBSOutput(aDBSFullPath, ios::out);
	aDBSOutput << "Tai khoan\tTen nhan vat\tMon phai\tTrung sinh\tDang cap\tNgan luong" << endl;
	int32_t aDBSPlayerCount = 0;
	int32_t aDBSSectPlayerCount[MAX_FACTION + 2] = { 0 };
	double aDBSMoneyCount = 0;
	double aDBSSectMoneyCount[MAX_FACTION + 2] = { 0 };
	int32_t aDBSLevelPlayerCount[MAX_LEVEL + 1] = { 0 };
	//==================

	strcat(aSavePath, ".bak");
	CDBDump DBDump;
	DBDump.Open(aSavePath);*/

	if (!RunTable)
	{
		aLogFile << "RunTable NULL Error." << endl;
		return;
	}

	TStatData* aStatData = new TStatData;//ÊµÊ±±¸·Ýºó²úÉúµÄÓÎÏ·Í³¼ÆÊý¾Ý
	//memset(&aStatData, 0, sizeof(TStatData));
	aStatData->Reset();

	ZCursor* cursor = NULL;
	cursor = RunTable->first();
	aLogFile << "RunTable cursor Opened. Backup begin." << endl;
	while (cursor)
	{
		if (!DBDump.AddData(cursor->key, cursor->key_size, cursor->data, cursor->size))
		{
			aLogFile << "Role[" << cursor->key << "] Dump Error." << endl;
		}

		TRoleList* tmpData;
		TRoleData* pRoleData = (TRoleData*)cursor->data;

		//Êý¾Ý¿â¼ÇÂ¼Í³¼Æ£¨Î¬»¤²é¿´ÓÃ£©==========Add by Fellow,2003.08.26
		char aDBSSect[32] = { 0 };
		int32_t aDBSSectIndex = (int32_t)pRoleData->BaseInfo.nSect;
		switch (aDBSSectIndex)
		{
		case 0:strcpy(aDBSSect, "Thieu Lam phai"); break;
		case 1:strcpy(aDBSSect, "Thien Vuong Bang"); break;
		case 2:strcpy(aDBSSect, "Duong Mon"); break;
		case 3:strcpy(aDBSSect, "Ngu Doc Giao"); break;
		case 4:strcpy(aDBSSect, "Nga My phai"); break;
		case 5:strcpy(aDBSSect, "Thuy Yen Mon"); break;
		case 6:strcpy(aDBSSect, "Cai Bang"); break;
		case 7:strcpy(aDBSSect, "Thien Nhan Giao"); break;
		case 8:strcpy(aDBSSect, "Vo Dang phai"); break;
		case 9:strcpy(aDBSSect, "Con Lon phai"); break;
		case 10:strcpy(aDBSSect, "Hoa Son phai"); break;
		case 11:strcpy(aDBSSect, "Vu Hon phai"); break;
		case 12:strcpy(aDBSSect, "Tieu Dao phai"); break;
		default:
			if (pRoleData->BaseInfo.ijoincount == 0)
			{
				strcpy(aDBSSect, "So nhap"); break;
			}
			else
			{
				strcpy(aDBSSect, "Xuat su"); break;
			}
		}
		if (aDBSSectIndex == 255)
		{
			if (pRoleData->BaseInfo.ijoincount == 0)
			{
				++aDBSSectPlayerCount[11];
				aDBSSectMoneyCount[11] += pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney;
			}
			else
			{
				++aDBSSectPlayerCount[12];
				aDBSSectMoneyCount[12] += pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney;
			}
		}
		else
		{
			++aDBSSectPlayerCount[aDBSSectIndex];
			aDBSSectMoneyCount[aDBSSectIndex] += pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney;
		}
		aDBSMoneyCount += pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney;
		++aDBSPlayerCount;
		if ((pRoleData->BaseInfo.ifightlevel % MAX_LEVEL > 0) && (pRoleData->BaseInfo.ifightlevel % MAX_LEVEL <= MAX_LEVEL))
			aDBSLevelPlayerCount[pRoleData->BaseInfo.ifightlevel % MAX_LEVEL]++;
		else
		{
			aDBSOutput << "***Phat sinh van de cap bac*** ";
			aDBSLevelPlayerCount[0]++;
		}
		aDBSOutput << pRoleData->BaseInfo.caccname << "\t" <<
			pRoleData->BaseInfo.szName << "\t" <<
			aDBSSect << "\t" <<
			pRoleData->BaseInfo.ifightlevel / MAX_LEVEL << "\t" <<
			pRoleData->BaseInfo.ifightlevel % MAX_LEVEL << "\t" <<
			pRoleData->BaseInfo.isavemoney + pRoleData->BaseInfo.imoney << endl;

		//=======================================

		if (pRoleData->BaseInfo.iteam == camp_event)
		{//¹ýÂËÄ³Ð©½ÇÉ«
			if (!RunTable->next(cursor))break;
			continue;
		}

		if (pRoleData->BaseInfo.iroletm < BackupBeginTime)
		{
			if (!RunTable->next(cursor))break;
			continue;
		}
		//////////////////////////////Íæ¼ÒÍ³¼Æ////////////////////////////////////
		//¶Ô½ðÇ®ÅÅÐò
		tmpData = GetMin(aStatData->MoneyStat, MONEYSTATNUM, stMoney);
		if (tmpData->Money < pRoleData->BaseInfo.imoney + pRoleData->BaseInfo.isavemoney)
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}

		//¶Ô¼¶±ðÅÅÐò
		tmpData = GetMin(aStatData->LevelStat, LEVELSTATNUM, stLevel);
		if (tmpData->Level < pRoleData->BaseInfo.ifightlevel)
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}

		//¶ÔÉ±ÈËÊýÅÅÐò
		tmpData = GetMin(aStatData->KillerStat, DEFAULTSTATNUM, stKiller);
		if (tmpData->KillNum < pRoleData->BaseInfo.nKillPeopleNumber)
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}
		tmpData = GetMin(aStatData->ReputeStat, DEFAULTSTATNUM, stRepute);
		if (tmpData->Repute < pRoleData->BaseInfo.istattask[TASKVALUE_STATTASK_REPUTE - TASKVALUE_STATTASK_BEGIN])
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}
		tmpData = GetMin(aStatData->FuYuanStat, DEFAULTSTATNUM, stFuYuan);
		if (tmpData->Repute < pRoleData->BaseInfo.istattask[TASKVALUE_STATTASK_FUYUAN - TASKVALUE_STATTASK_BEGIN])
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}
		tmpData = GetMin(aStatData->AccumStat1, DEFAULTSTATNUM, stAccumStat1);
		if (tmpData->AccumStat1 < pRoleData->BaseInfo.istattask[TASKVALUE_STATTASK_ACCUMSTAT1 - TASKVALUE_STATTASK_BEGIN])
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}
		tmpData = GetMin(aStatData->AccumStat2, DEFAULTSTATNUM, stAccumStat2);
		if (tmpData->AccumStat2 < pRoleData->BaseInfo.istattask[TASKVALUE_STATTASK_ACCUMSTAT2 - TASKVALUE_STATTASK_BEGIN])
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}
		tmpData = GetMin(aStatData->HonorStat, DEFAULTSTATNUM, stHonorStat);
		if (tmpData->HonorStat < pRoleData->BaseInfo.istattask[TASKVALUE_STATTASK_HONORSTAT - TASKVALUE_STATTASK_BEGIN])
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}
		tmpData = GetMax(aStatData->TimeStat, DEFAULTSTATNUM, stTimeStat);
		if (tmpData->TimeStat < pRoleData->BaseInfo.istattask[TASKVALUE_STATTASK_TIMESTAT - TASKVALUE_STATTASK_BEGIN])
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}
		tmpData = GetMin(aStatData->TongLvStat, DEFAULTSTATNUM, stTongLv, pRoleData->BaseInfo.itongname);
		if (tmpData->TongLv < pRoleData->BaseInfo.itonglevel)
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData, true);
		}
		tmpData = GetMin(aStatData->TongMnStat, DEFAULTSTATNUM, stTongMn, pRoleData->BaseInfo.itongname);
		if (tmpData->TongMn < pRoleData->BaseInfo.itongmemnum)
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData, true);
		}
		tmpData = GetMin(aStatData->TongEffStat, DEFAULTSTATNUM, stTongEff, pRoleData->BaseInfo.itongname);
		if (tmpData->TongEff < pRoleData->BaseInfo.itongeff)
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData, true);
		}
		//¸÷ÃÅÅÉ¶Ô½ðÇ®ÅÅÐò
		if ((pRoleData->BaseInfo.nSect < (MAX_FACTION + 1)) && (pRoleData->BaseInfo.nSect >= 0))
		{
			tmpData = GetMin(aStatData->MoneyStatBySect[pRoleData->BaseInfo.nSect + 1], SECTMONEYSTATNUM, stMoney);
		}
		else
		{
			tmpData = GetMin(aStatData->MoneyStatBySect[0], SECTMONEYSTATNUM, stMoney);
		}
		if (tmpData->Money < pRoleData->BaseInfo.imoney + pRoleData->BaseInfo.isavemoney)
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}

		//¸÷ÃÅÅÉ¶Ô¼¶±ðÅÅÐò
		if ((pRoleData->BaseInfo.nSect < (MAX_FACTION + 1)) && (pRoleData->BaseInfo.nSect >= 0))
		{
			tmpData = GetMin(aStatData->LevelStatBySect[pRoleData->BaseInfo.nSect + 1], SECTLEVELSTATNUM, stMoney);
		}
		else
		{
			tmpData = GetMin(aStatData->LevelStatBySect[0], SECTLEVELSTATNUM, stMoney);
		}
		if (tmpData->Level < pRoleData->BaseInfo.ifightlevel)
		{//Èç¹ûµ±Ç°Êý¾Ý½Ï´ó¾Í°Ñµ±Ç°Êý¾Ý´úÌæÁÐ±íÖÐ×îÐ¡µÄ
			RoleDataCopy(tmpData, pRoleData);
		}

		//////////////////////////////ÃÅÅÉÍ³¼Æ////////////////////////////////////
		//¸÷¸öÃÅÅÉµÄÍæ¼ÒÊýÍ³¼Æ
		if ((pRoleData->BaseInfo.nSect < (MAX_FACTION + 1)) && (pRoleData->BaseInfo.nSect >= 0))
		{
			++aStatData->SectPlayerNum[pRoleData->BaseInfo.nSect + 1];
		}
		else
		{
			++aStatData->SectPlayerNum[0];
		}
		if ((pRoleData->BaseInfo.nSect < (MAX_FACTION + 1)) && (pRoleData->BaseInfo.nSect >= 0))
		{
			++aStatData->SectMoneyMost[pRoleData->BaseInfo.nSect + 1];
		}
		else
		{
			++aStatData->SectMoneyMost[0];
		}
		if ((pRoleData->BaseInfo.nSect < (MAX_FACTION + 1)) && (pRoleData->BaseInfo.nSect >= 0))
		{
			++aStatData->SectLevelMost[pRoleData->BaseInfo.nSect + 1];
		}
		else
		{
			++aStatData->SectLevelMost[0];
		}
		if (!RunTable->next(cursor))break;
	}
	DBDump.Close();		//¹Ø±Õ±¸·ÝÊý¾Ý¿â
	strcpy(g_LastBackupPath, dumpPath);
	g_LastBackupOk = true;
	aLogFile << "DB Dump Finished." << endl;
	aLogFile << "RunTable cursor closed." << endl;

	//Êý¾Ý¿â¼ÇÂ¼Í³¼Æ£¨Î¬»¤²é¿´ÓÃ£©==========Add by Fellow,2003.08.26
	aDBSOutput << "==Hoan tat ghi chep==" << endl << endl;
	aDBSOutput << "==Thong ke==" << endl;
	aDBSOutput << "Tong so nguoi choi toan server: " << aDBSPlayerCount << endl;
	for (i = 0; i < (MAX_FACTION + 2); ++i)
	{
		char aDBSSect[32] = { 0 };
		switch (i)
		{
		case 0:strcpy(aDBSSect, "Thieu Lam phai"); break;
		case 1:strcpy(aDBSSect, "Thien Vuong Bang"); break;
		case 2:strcpy(aDBSSect, "Duong Mon"); break;
		case 3:strcpy(aDBSSect, "Ngu Doc Giao"); break;
		case 4:strcpy(aDBSSect, "Nga My phai"); break;
		case 5:strcpy(aDBSSect, "Thuy Yen Mon"); break;
		case 6:strcpy(aDBSSect, "Cai Bang"); break;
		case 7:strcpy(aDBSSect, "Thien Nhan Giao"); break;
		case 8:strcpy(aDBSSect, "Vo Dang phai"); break;
		case 9:strcpy(aDBSSect, "Con Lon phai"); break;
		case 10:strcpy(aDBSSect, "Hoa Son phai"); break;
		case 11:strcpy(aDBSSect, "Vu Hon phai"); break;
		case 12:strcpy(aDBSSect, "Tieu Dao phai"); break;
		case 13:strcpy(aDBSSect, "So nhap");; break;
		case 14:strcpy(aDBSSect, "Xuat su");; break;
		}
		aDBSOutput << aDBSSect << ": " << aDBSSectPlayerCount[i] << endl;
	}
	aDBSOutput << "------------------------------------------------" << endl;
	aDBSOutput << "Tong ngan luong toan server: " << aDBSMoneyCount << endl;
	for (i = 0; i < (MAX_FACTION + 2); ++i)
	{
		char aDBSSect[32] = { 0 };
		switch (i)
		{
		case 0:strcpy(aDBSSect, "Thieu Lam phai"); break;
		case 1:strcpy(aDBSSect, "Thien Vuong Bang"); break;
		case 2:strcpy(aDBSSect, "Duong Mon"); break;
		case 3:strcpy(aDBSSect, "Ngu Doc Giao"); break;
		case 4:strcpy(aDBSSect, "Nga My phai"); break;
		case 5:strcpy(aDBSSect, "Thuy Yen Mon"); break;
		case 6:strcpy(aDBSSect, "Cai Bang"); break;
		case 7:strcpy(aDBSSect, "Thien Nhan Giao"); break;
		case 8:strcpy(aDBSSect, "Vo Dang phai"); break;
		case 9:strcpy(aDBSSect, "Con Lon phai"); break;
		case 10:strcpy(aDBSSect, "Hoa Son phai"); break;
		case 11:strcpy(aDBSSect, "Vu Hon phai"); break;
		case 12:strcpy(aDBSSect, "Tieu Dao phai"); break;
		case 13:strcpy(aDBSSect, "So nhap");; break;
		case 14:strcpy(aDBSSect, "Xuat su");; break;
		}
		aDBSOutput << aDBSSect << ": " << aDBSSectMoneyCount[i] << endl;
	}
	aDBSOutput << "------------------------------------------------" << endl;
	aDBSOutput << "Thong ke so luong nguoi choi moi cap[1-200 cap]:" << endl;
	for (i = 1; i < (MAX_LEVEL + 1); ++i)
		if (aDBSLevelPlayerCount[i] != 0)
			aDBSOutput << i << "cap: " << aDBSLevelPlayerCount[i] << endl;
	aDBSOutput << "Nhan si loi dang cap: " << aDBSLevelPlayerCount[0] << endl;

	//////////////////////////////ÅÅÐò////////////////////////////////////
	ListSort(aStatData->MoneyStat, MONEYSTATNUM, stMoney);
	ListSort(aStatData->LevelStat, LEVELSTATNUM, stLevel);
	ListSort(aStatData->KillerStat, DEFAULTSTATNUM, stKiller);
	ListSort(aStatData->ReputeStat, DEFAULTSTATNUM, stRepute);
	ListSort(aStatData->FuYuanStat, DEFAULTSTATNUM, stFuYuan);
	ListSort(aStatData->AccumStat1, DEFAULTSTATNUM, stAccumStat1);
	ListSort(aStatData->AccumStat2, DEFAULTSTATNUM, stAccumStat2);
	ListSort(aStatData->HonorStat, DEFAULTSTATNUM, stHonorStat);
	ListSort(aStatData->TimeStat, DEFAULTSTATNUM, stTimeStat, true);
	ListSort(aStatData->TongLvStat, DEFAULTSTATNUM, stTongLv);
	ListSort(aStatData->TongMnStat, DEFAULTSTATNUM, stTongMn);
	ListSort(aStatData->TongEffStat, DEFAULTSTATNUM, stTongEff);
	for (i = 0; i < (MAX_FACTION + 1); ++i)
	{//¸÷ÃÅÅÉ½ðÇ®ÅÅÐò
		ListSort(aStatData->MoneyStatBySect[i], SECTMONEYSTATNUM, stMoney);
	}
	for (i = 0; i < (MAX_FACTION + 1); ++i)
	{//¸÷ÃÅÅÉ¼¶±ðÅÅÐò
		ListSort(aStatData->LevelStatBySect[i], SECTLEVELSTATNUM, stLevel);
	}

	///////////////ÒÔÏÂÎª¶ÔÍ³¼ÆÊý¾ÝµÄ´¦Àí//////////////////////////////////////////////////////
	char aAppPath[MAX_PATH] = { 0 };
	x = getcwd(aAppPath, MAX_PATH);
	strcat(aAppPath, "\\StatData.dat");
	//¶ÁÈ¡¾ÉÅÅÃû
	//memset(&oldGameStatData, 0, sizeof(CDBBackup::TStatData));
	oldGameStatData.Reset();

	LoadStatData(aAppPath, oldGameStatData);

	//FILE* aStatFile = fopen(aAppPath, "rb");
	//if (aStatFile)
	//{
	//	int32_t a = fread(&oldGameStatData, 1, sizeof(CDBBackup::TStatData), aStatFile);
	//	fclose(aStatFile);
	//}

	newGameStatData = *aStatData;

	//ÕÒ³öÇ°Ê®ÃûµÄoldGameStatDataÖÐµÄÅÅÃû£¨»òÃû´ÎÉÏÉý»¹ÊÇÏÂ½µ£©
	int32_t aIndex;
	for (i = 0; i < DEFAULTSTATNUM; ++i)
	{
		//µÈ¼¶
		aIndex = CDBBackup::GetIndexByName(
			newGameStatData.LevelStat[i].Name, oldGameStatData.LevelStat, LEVELSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.LevelStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.LevelStat[i].Sort = 255;
			else
				newGameStatData.LevelStat[i].Sort = 0;
		}
		else
			newGameStatData.LevelStat[i].Sort = 1;

		//½ðÇ®
		aIndex = CDBBackup::GetIndexByName(newGameStatData.MoneyStat[i].Name, oldGameStatData.MoneyStat, MONEYSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.MoneyStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.MoneyStat[i].Sort = 255;
			else
				newGameStatData.MoneyStat[i].Sort = 0;
		}
		else
			newGameStatData.MoneyStat[i].Sort = 1;

		//É±ÈËÊý
		aIndex = CDBBackup::GetIndexByName(newGameStatData.KillerStat[i].Name, oldGameStatData.KillerStat, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.KillerStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.KillerStat[i].Sort = 255;
			else
				newGameStatData.KillerStat[i].Sort = 0;
		}
		else
			newGameStatData.KillerStat[i].Sort = 1;

		aIndex = CDBBackup::GetIndexByName(newGameStatData.ReputeStat[i].Name, oldGameStatData.ReputeStat, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.ReputeStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.ReputeStat[i].Sort = 255;
			else
				newGameStatData.ReputeStat[i].Sort = 0;
		}
		else
			newGameStatData.ReputeStat[i].Sort = 1;

		aIndex = CDBBackup::GetIndexByName(newGameStatData.FuYuanStat[i].Name, oldGameStatData.FuYuanStat, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.FuYuanStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.FuYuanStat[i].Sort = 255;
			else
				newGameStatData.FuYuanStat[i].Sort = 0;
		}
		else
			newGameStatData.FuYuanStat[i].Sort = 1;

		aIndex = CDBBackup::GetIndexByName(newGameStatData.AccumStat1[i].Name, oldGameStatData.AccumStat1, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.AccumStat1[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.AccumStat1[i].Sort = 255;
			else
				newGameStatData.AccumStat1[i].Sort = 0;
		}
		else
			newGameStatData.AccumStat1[i].Sort = 1;

		aIndex = CDBBackup::GetIndexByName(newGameStatData.AccumStat2[i].Name, oldGameStatData.AccumStat2, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.AccumStat2[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.AccumStat2[i].Sort = 255;
			else
				newGameStatData.AccumStat2[i].Sort = 0;
		}
		else
			newGameStatData.AccumStat2[i].Sort = 1;

		aIndex = CDBBackup::GetIndexByName(newGameStatData.HonorStat[i].Name, oldGameStatData.HonorStat, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.HonorStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.HonorStat[i].Sort = 255;
			else
				newGameStatData.HonorStat[i].Sort = 0;
		}
		else
			newGameStatData.HonorStat[i].Sort = 1;

		aIndex = CDBBackup::GetIndexByName(newGameStatData.TimeStat[i].Name, oldGameStatData.TimeStat, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.TimeStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.TimeStat[i].Sort = 255;
			else
				newGameStatData.TimeStat[i].Sort = 0;
		}
		else
			newGameStatData.TimeStat[i].Sort = 1;


		aIndex = CDBBackup::GetIndexByName(newGameStatData.TongLvStat[i].Name, oldGameStatData.TongLvStat, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.TongLvStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.TongLvStat[i].Sort = 255;
			else
				newGameStatData.TongLvStat[i].Sort = 0;
		}
		else
			newGameStatData.TongLvStat[i].Sort = 1;

		aIndex = CDBBackup::GetIndexByName(newGameStatData.TongMnStat[i].Name, oldGameStatData.TongMnStat, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.TongMnStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.TongMnStat[i].Sort = 255;
			else
				newGameStatData.TongMnStat[i].Sort = 0;
		}
		else
			newGameStatData.TongMnStat[i].Sort = 1;

		aIndex = CDBBackup::GetIndexByName(newGameStatData.TongEffStat[i].Name, oldGameStatData.TongEffStat, DEFAULTSTATNUM);
		if (aIndex != -1)
		{
			if (i < aIndex)
				newGameStatData.TongEffStat[i].Sort = 1;
			else if (i > aIndex)
				newGameStatData.TongEffStat[i].Sort = 255;
			else
				newGameStatData.TongEffStat[i].Sort = 0;
		}
		else
			newGameStatData.TongEffStat[i].Sort = 1;

		for (j = 0; j < (MAX_FACTION + 1); ++j)
		{//¸÷¸öÃÅÅÉ
			//µÈ¼¶
			aIndex = CDBBackup::GetIndexByName(newGameStatData.LevelStatBySect[j][i].Name, oldGameStatData.LevelStatBySect[j], SECTLEVELSTATNUM);
			if (aIndex != -1)
			{
				if (i < aIndex)
					newGameStatData.LevelStatBySect[j][i].Sort = 1;
				else if (i > aIndex)
					newGameStatData.LevelStatBySect[j][i].Sort = 255;
				else
					newGameStatData.LevelStatBySect[j][i].Sort = 0;
			}
			else
				newGameStatData.LevelStatBySect[j][i].Sort = 1;

			//½ðÇ®
			aIndex = CDBBackup::GetIndexByName(newGameStatData.MoneyStatBySect[j][i].Name, oldGameStatData.MoneyStatBySect[j], SECTMONEYSTATNUM);
			if (aIndex != -1)
			{
				if (i < aIndex)
					newGameStatData.MoneyStatBySect[j][i].Sort = 1;
				else if (i > aIndex)
					newGameStatData.MoneyStatBySect[j][i].Sort = 255;
				else
					newGameStatData.MoneyStatBySect[j][i].Sort = 0;
			}
			else
				newGameStatData.MoneyStatBySect[j][i].Sort = 1;
		}
	}
	////´¢´æÐÂÅÅÃû
	//aStatFile = fopen(aAppPath, "wb");
	//if (aStatFile)
	//{
	//	int32_t a = fwrite(&newGameStatData, 1, sizeof(CDBBackup::TStatData), aStatFile);

	//	fclose(aStatFile);
	//}

	SaveStatData(aAppPath, newGameStatData);

	MakeSendStatData();//°ÑÍ³¼ÆÊý¾ÝÐ´µ½·¢ËÍ¸ø¿Í»§¶ËµÄÍ³¼ÆÊý¾Ý½á¹¹ÖÐ

	if (IS_OUTPUT_LOG) {//ÊÇ·ñÊä³öÍ³¼ÆÊý¾Ý
		aLogFile << "DB Statistic Log:" << endl;
		aLogFile << "-------------Level-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.LevelStat[i].Name << "\t" <<
				SendStatData.LevelStat[i].nValue / MAX_LEVEL << "\t" <<
				SendStatData.LevelStat[i].nValue % MAX_LEVEL << "\t" <<
				(int32_t)SendStatData.LevelStat[i].bySort << endl;
		}
		aLogFile << "-------------Money-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.MoneyStat[i].Name << "\t" <<
				SendStatData.MoneyStat[i].nValue << "\t" <<
				(int32_t)SendStatData.MoneyStat[i].bySort << endl;
		}
		aLogFile << "-------------Killer-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.KillerStat[i].Name << "\t" <<
				SendStatData.KillerStat[i].nValue << "\t" <<
				(int32_t)SendStatData.KillerStat[i].bySort << endl;
		}
		aLogFile << "-------------Repute-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.ReputeStat[i].Name << "\t" <<
				SendStatData.ReputeStat[i].nValue << "\t" <<
				(int32_t)SendStatData.ReputeStat[i].bySort << endl;
		}
		aLogFile << "-------------FuYuan-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.FuYuanStat[i].Name << "\t" <<
				SendStatData.FuYuanStat[i].nValue << "\t" <<
				(int32_t)SendStatData.FuYuanStat[i].bySort << endl;
		}
		aLogFile << "-------------AccumStat1-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.AccumStat1[i].Name << "\t" <<
				SendStatData.AccumStat1[i].nValue << "\t" <<
				(int32_t)SendStatData.AccumStat1[i].bySort << endl;
		}
		aLogFile << "-------------AccumStat2-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.AccumStat2[i].Name << "\t" <<
				SendStatData.AccumStat2[i].nValue << "\t" <<
				(int32_t)SendStatData.AccumStat2[i].bySort << endl;
		}
		aLogFile << "-------------HonorStat-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.HonorStat[i].Name << "\t" <<
				SendStatData.HonorStat[i].nValue << "\t" <<
				(int32_t)SendStatData.HonorStat[i].bySort << endl;
		}
		aLogFile << "-------------TimeStat-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.TimeStat[i].Name << "\t" <<
				SendStatData.TimeStat[i].nValue << "\t" <<
				(int32_t)SendStatData.TimeStat[i].bySort << endl;
		}
		aLogFile << "-------------TongLv-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.TongLvStat[i].Name << "\t" <<
				SendStatData.TongLvStat[i].nValue << "\t" <<
				(int32_t)SendStatData.TongLvStat[i].bySort << endl;
		}
		aLogFile << "-------------TongMn-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.TongMnStat[i].Name << "\t" <<
				SendStatData.TongMnStat[i].nValue << "\t" <<
				(int32_t)SendStatData.TongMnStat[i].bySort << endl;
		}
		aLogFile << "-------------TongEff-------------" << endl;
		for (i = 0; i < DEFAULTSTATNUM; ++i)
		{
			aLogFile << SendStatData.TongEffStat[i].Name << "\t" <<
				SendStatData.TongEffStat[i].nValue << "\t" <<
				(int32_t)SendStatData.TongEffStat[i].bySort << endl;
		}
		for (i = 0; i < (MAX_FACTION + 1); ++i)
		{
			aLogFile << "--------------Sect " << i << " Level--------------" << endl;
			for (j = 0; j < SECTMONEYSTATNUM; ++j)
			{
				aLogFile << SendStatData.LevelStatBySect[i][j].Name << "\t" <<
					SendStatData.LevelStatBySect[i][j].nValue / MAX_LEVEL << "\t" <<
					SendStatData.LevelStatBySect[i][j].nValue % MAX_LEVEL << "\t" <<
					(int32_t)SendStatData.LevelStatBySect[i][j].bySort << endl;
			}
			aLogFile << "-------------Sect " << i << " Money---------------" << endl;
			for (j = 0; j < SECTMONEYSTATNUM; ++j)
			{
				aLogFile << SendStatData.MoneyStatBySect[i][j].Name << "\t" <<
					SendStatData.MoneyStatBySect[i][j].nValue << "\t" <<
					(int32_t)SendStatData.MoneyStatBySect[i][j].bySort << endl;
			}
		}
		aLogFile << "Thong ke so luong nguoi choi moi mon phai" << endl;
		for (i = 0; i < (MAX_FACTION + 1); ++i)
		{
			aLogFile << "Sect " << i << ": " << SendStatData.SectPlayerNum[i] << endl;
		}
		aLogFile << "Thong ke so luong phu ho moi mon phai(Tong: " << MONEYSTATNUM << ")" << endl;
		for (i = 0; i < (MAX_FACTION + 1); ++i)
		{
			aLogFile << "Sect " << i << ": " << SendStatData.SectMoneyMost[i] << endl;
		}
		aLogFile << "Thong ke so luong cao thu moi mon phai(Tong: " << LEVELSTATNUM << ")" << endl;
		for (i = 0; i < (MAX_FACTION + 1); ++i)
		{
			aLogFile << "Sect " << i << ": " << SendStatData.SectLevelMost[i] << endl;
		}
	}
	//================

	IsBackupWorking = false;
}

void CDBBackup::MakeSendStatData()
{//Éú³É·¢ËÍ¸ø¿Í»§¶ËµÄÍ³¼ÆÊý¾Ý½á¹¹
	int32_t i, j;
	memset(&SendStatData, 0, sizeof(TGAME_STAT_DATA));
	for (i = 0; i < 10; ++i)
	{
		strcpy(SendStatData.LevelStat[i].Name, newGameStatData.LevelStat[i].Name);
		SendStatData.LevelStat[i].nValue = newGameStatData.LevelStat[i].Level + newGameStatData.LevelStat[i].Translife * MAX_LEVEL;
		SendStatData.LevelStat[i].bySort = newGameStatData.LevelStat[i].Sort;

		strcpy(SendStatData.MoneyStat[i].Name, newGameStatData.MoneyStat[i].Name);
		SendStatData.MoneyStat[i].nValue = newGameStatData.MoneyStat[i].Money;
		SendStatData.MoneyStat[i].bySort = newGameStatData.MoneyStat[i].Sort;

		strcpy(SendStatData.KillerStat[i].Name, newGameStatData.KillerStat[i].Name);
		SendStatData.KillerStat[i].nValue = newGameStatData.KillerStat[i].KillNum;
		SendStatData.KillerStat[i].bySort = newGameStatData.KillerStat[i].Sort;

		strcpy(SendStatData.ReputeStat[i].Name, newGameStatData.ReputeStat[i].Name);
		SendStatData.ReputeStat[i].nValue = newGameStatData.ReputeStat[i].Repute;
		SendStatData.ReputeStat[i].bySort = newGameStatData.ReputeStat[i].Sort;

		strcpy(SendStatData.FuYuanStat[i].Name, newGameStatData.FuYuanStat[i].Name);
		SendStatData.FuYuanStat[i].nValue = newGameStatData.FuYuanStat[i].FuYuan;
		SendStatData.FuYuanStat[i].bySort = newGameStatData.FuYuanStat[i].Sort;

		strcpy(SendStatData.AccumStat1[i].Name, newGameStatData.AccumStat1[i].Name);
		SendStatData.AccumStat1[i].nValue = newGameStatData.AccumStat1[i].AccumStat1;
		SendStatData.AccumStat1[i].bySort = newGameStatData.AccumStat1[i].Sort;

		strcpy(SendStatData.AccumStat2[i].Name, newGameStatData.AccumStat2[i].Name);
		SendStatData.AccumStat2[i].nValue = newGameStatData.AccumStat2[i].AccumStat2;
		SendStatData.AccumStat2[i].bySort = newGameStatData.AccumStat2[i].Sort;

		strcpy(SendStatData.HonorStat[i].Name, newGameStatData.HonorStat[i].Name);
		SendStatData.HonorStat[i].nValue = newGameStatData.HonorStat[i].HonorStat;
		SendStatData.HonorStat[i].bySort = newGameStatData.HonorStat[i].Sort;

		strcpy(SendStatData.TimeStat[i].Name, newGameStatData.TimeStat[i].Name);
		SendStatData.TimeStat[i].nValue = newGameStatData.TimeStat[i].TimeStat;
		SendStatData.TimeStat[i].bySort = newGameStatData.TimeStat[i].Sort;

		strcpy(SendStatData.TongLvStat[i].Name, newGameStatData.TongLvStat[i].Name);
		SendStatData.TongLvStat[i].nValue = newGameStatData.TongLvStat[i].TongLv;
		SendStatData.TongLvStat[i].bySort = newGameStatData.TongLvStat[i].Sort;

		strcpy(SendStatData.TongMnStat[i].Name, newGameStatData.TongMnStat[i].Name);
		SendStatData.TongMnStat[i].nValue = newGameStatData.TongMnStat[i].TongMn;
		SendStatData.TongMnStat[i].bySort = newGameStatData.TongMnStat[i].Sort;

		strcpy(SendStatData.TongEffStat[i].Name, newGameStatData.TongEffStat[i].Name);
		SendStatData.TongEffStat[i].nValue = newGameStatData.TongEffStat[i].TongEff;
		SendStatData.TongEffStat[i].bySort = newGameStatData.TongEffStat[i].Sort;
		for (j = 0; j < (MAX_FACTION + 1); ++j)
		{//¸÷¸öÃÅÅÉ
			strcpy(SendStatData.LevelStatBySect[j][i].Name, newGameStatData.LevelStatBySect[j][i].Name);
			SendStatData.LevelStatBySect[j][i].nValue = newGameStatData.LevelStatBySect[j][i].Level;
			SendStatData.LevelStatBySect[j][i].bySort = newGameStatData.LevelStatBySect[j][i].Sort;

			strcpy(SendStatData.MoneyStatBySect[j][i].Name, newGameStatData.MoneyStatBySect[j][i].Name);
			SendStatData.MoneyStatBySect[j][i].nValue = newGameStatData.MoneyStatBySect[j][i].Money;
			SendStatData.MoneyStatBySect[j][i].bySort = newGameStatData.MoneyStatBySect[j][i].Sort;

			SendStatData.SectLevelMost[j] = newGameStatData.SectLevelMost[j];
			SendStatData.SectMoneyMost[j] = newGameStatData.SectMoneyMost[j];
			SendStatData.SectPlayerNum[j] = newGameStatData.SectPlayerNum[j];
		}
	}
}

void CDBBackup::SaveStatInfo()
{
	// Sử dụng phân bổ động cho aStatData
	TStatData* aStatData = new TStatData;
	if (aStatData == nullptr) {
		// Xử lý lỗi phân bổ
		return;
	}

	// Sử dụng phân bổ động cho aAppPath
	char* aAppPath = new char[MAX_PATH];
	if (aAppPath == nullptr) {
		// Xử lý lỗi phân bổ
		delete aStatData;
		return;
	}
	memset(aAppPath, 0, MAX_PATH);

	char* x = getcwd(aAppPath, MAX_PATH);
	strcat(aAppPath, "\\StatData.dat");

	LoadStatData(aAppPath, *aStatData);

	int32_t i, j;
	for (i = 0; i < LEVELSTATNUM; ++i)
	{
		if (aStatData->LevelStat[i].Name[0] == '\0')
			continue;

		ZCursor* cursor = NULL;
		cursor = RunTable->search(aStatData->LevelStat[i].Name, strlen(aStatData->LevelStat[i].Name) + 1);
		if (!cursor)
		{
			continue;
		}
		TRoleData* pRoleData = (TRoleData*)cursor->data;
		pRoleData->BaseInfo.nWorldStat = i + 1;

		RunTable->add(aStatData->LevelStat[i].Name, strlen(aStatData->LevelStat[i].Name) + 1, cursor->data, cursor->size);
		RunTable->closeCursor(cursor);
	}

	// Write the sect level ranking to the specified player character
	for (i = 0; i < (MAX_FACTION + 1); ++i)
	{
		for (j = 0; j < SECTLEVELSTATNUM; ++j)
		{
			if (aStatData->LevelStatBySect[i][j].Name[0] == '\0')
				continue;
			ZCursor* cursor = NULL;
			cursor = RunTable->search(aStatData->LevelStatBySect[i][j].Name, strlen(aStatData->LevelStatBySect[i][j].Name) + 1);
			if (!cursor)
			{
				continue;
			}
			TRoleData* pRoleData = (TRoleData*)cursor->data;
			pRoleData->BaseInfo.nSectStat = j + 1;

			RunTable->add(aStatData->LevelStatBySect[i][j].Name, strlen(aStatData->LevelStatBySect[i][j].Name) + 1, cursor->data, cursor->size);
			RunTable->closeCursor(cursor);
		}
	}

	// Giải phóng bộ nhớ động
	delete aStatData;
	delete[] aAppPath;
}

bool CDBBackup::LoadStatData(const char* filePath, CDBBackup::TStatData& statData)
{
	FILE* aStatFile = fopen(filePath, "rb");
	if (!aStatFile)
	{
		return false;
	}

	// Đọc các mảng tĩnh
	if (fread(statData.LevelStat, sizeof(TRoleList), LEVELSTATNUM, aStatFile) != LEVELSTATNUM ||
		fread(statData.MoneyStat, sizeof(TRoleList), MONEYSTATNUM, aStatFile) != MONEYSTATNUM ||
		fread(statData.KillerStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.ReputeStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.FuYuanStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.AccumStat1, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.AccumStat2, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.HonorStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.TimeStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.TongLvStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.TongMnStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fread(statData.TongEffStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM)
	{
		fclose(aStatFile);
		return false;
	}

	// Đọc các mảng động
	for (int i = 0; i <= MAX_FACTION; ++i)
	{
		if (fread(statData.MoneyStatBySect[i], sizeof(TRoleList), SECTMONEYSTATNUM, aStatFile) != SECTMONEYSTATNUM ||
			fread(statData.LevelStatBySect[i], sizeof(TRoleList), SECTLEVELSTATNUM, aStatFile) != SECTLEVELSTATNUM)
		{
			fclose(aStatFile);
			return false;
		}
	}

	// Đọc các thành phần tĩnh khác
	if (fread(statData.SectPlayerNum, sizeof(int32_t), MAX_FACTION + 1, aStatFile) != MAX_FACTION + 1 ||
		fread(statData.SectMoneyMost, sizeof(int32_t), MAX_FACTION + 1, aStatFile) != MAX_FACTION + 1 ||
		fread(statData.SectLevelMost, sizeof(int32_t), MAX_FACTION + 1, aStatFile) != MAX_FACTION + 1)
	{
		fclose(aStatFile);
		return false;
	}

	fclose(aStatFile);
	return true;
}

bool CDBBackup::SaveStatData(const char* filePath, const CDBBackup::TStatData& statData)
{
	FILE* aStatFile = fopen(filePath, "wb");
	if (!aStatFile)
	{
		return false;
	}

	// Ghi các mảng tĩnh
	if (fwrite(statData.LevelStat, sizeof(TRoleList), LEVELSTATNUM, aStatFile) != LEVELSTATNUM ||
		fwrite(statData.MoneyStat, sizeof(TRoleList), MONEYSTATNUM, aStatFile) != MONEYSTATNUM ||
		fwrite(statData.KillerStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.ReputeStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.FuYuanStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.AccumStat1, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.AccumStat2, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.HonorStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.TimeStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.TongLvStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.TongMnStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM ||
		fwrite(statData.TongEffStat, sizeof(TRoleList), DEFAULTSTATNUM, aStatFile) != DEFAULTSTATNUM)
	{
		fclose(aStatFile);
		return false;
	}

	// Ghi các mảng động
	for (int i = 0; i <= MAX_FACTION; ++i)
	{
		if (fwrite(statData.MoneyStatBySect[i], sizeof(TRoleList), SECTMONEYSTATNUM, aStatFile) != SECTMONEYSTATNUM ||
			fwrite(statData.LevelStatBySect[i], sizeof(TRoleList), SECTLEVELSTATNUM, aStatFile) != SECTLEVELSTATNUM)
		{
			fclose(aStatFile);
			return false;
		}
	}

	// Ghi các thành phần tĩnh khác
	if (fwrite(statData.SectPlayerNum, sizeof(int32_t), MAX_FACTION + 1, aStatFile) != MAX_FACTION + 1 ||
		fwrite(statData.SectMoneyMost, sizeof(int32_t), MAX_FACTION + 1, aStatFile) != MAX_FACTION + 1 ||
		fwrite(statData.SectLevelMost, sizeof(int32_t), MAX_FACTION + 1, aStatFile) != MAX_FACTION + 1)
	{
		fclose(aStatFile);
		return false;
	}

	fclose(aStatFile);
	return true;
}
void CDBBackup::GetLastBackupPath(char* buf, size_t bufSize)
{
    if (!buf || bufSize == 0) return;
    strncpy(buf, g_LastBackupPath, bufSize - 1);
    buf[bufSize - 1] = '\0';
}
bool CDBBackup::WasLastBackupOk()
{
    return g_LastBackupOk;
}
