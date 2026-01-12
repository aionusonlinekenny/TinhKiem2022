/********************************************************************
	created:	2003/06/03
	file base:	IDBRoleServer
	file ext:	h
	author:		liupeng

	purpose:
*********************************************************************/
#ifndef __INCLUDE_IDBROLESERVER_H__
#define __INCLUDE_IDBROLESERVER_H__

#include "S3DBInterface.h"

BOOL InitDBInterface(size_t nMaxRoleCount);

void ReleaseDBInterface();

void* GetRoleInfo(char* pRoleBuffer, char* strUser, int32_t& nBufLen);
void* GetRoleInfoForGM(int32_t nInfoID, char* pRoleBuffer, char* strUser, int32_t& nBufLen);
void SetRoleInfoForGM(int32_t nInfoID, char* pRoleBuffer, char* strUser, int32_t nBufLen);

int32_t	SaveRoleInfo(char* pRoleBuffer, const char* strUser, BOOL bAutoInsertWhenNoExistUser, BOOL bAutoDeleteUser);

int CheckRoleName(char* pName); //检测角色信息

int32_t GetRoleListOfAccount(char* szAccountName, S3DBI_RoleBaseInfo* RoleBaseList, int32_t nMaxCount);

bool DeleteRole(const char* strUser);

int ChangeRoleName(char* nScrRoleName, char* nTarRoleName);

char* GetAccountByUser(char* strUser);//通过用户名查找帐户

//------------------------------------------------------------------------
//数据库备份与数据统计 Add By Fellow At 2003.08.14
bool InitBackupEngine(); 
bool StartBackupTimer(int32_t aTime, DWORD bTime);		//开始运行备份线程
bool StartBackupTimerByMinutes(DWORD minutes);       // m?i: theo ph鷗
bool StopBackupTimer();							//结束运行备份线程
bool SuspendBackupTimer();						//挂起线程
bool ResumeBackupTimer();						//继续运行线程
bool IsBackupThreadWorking();					//线程是否正在运行
bool IsBackupWorking();							//是否在备份
bool DoManualBackup();							//手工备份
bool GetLastBackupPath(char* buf, size_t bufSize);
bool WasLastBackupOk();
bool GetGameStat(TGAME_STAT_DATA* aStatData);//取得用户统计数据
//------------------------------------------------------------------------
void AddOutputString(HWND hListCtrl, char* aStr);//添加操作输出文字
#endif // __INCLUDE_IDBROLESERVER_H__