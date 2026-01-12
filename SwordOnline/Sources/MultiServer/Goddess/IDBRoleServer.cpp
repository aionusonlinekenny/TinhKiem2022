#include "stdafx.h"
#include "IDBRoleServer.h"
#include "DBTable.h"
#include "DBBackup.h"
#include "../../../../Headers/KGmProtocol.h"
#include "CRC32.h"

#include <iostream>
#include <strstream>

using namespace std;

#include "Macro.h"
#include <tchar.h>

static ZDBTable* db_table = NULL;
static size_t nMaxRoleCount_InAccount = 3;
static CDBBackup::TStatData GameStatData;//ÓÎÏ·Í³¼ÆÊý¾Ý£¨·þÎñÆ÷³õÊ¼»¯Ê±ÌîÈë£©
static CDBBackup* DBBackup = NULL;

static HANDLE hDeadLockThread = NULL;		//¼ì²éÊý¾Ý¿âÓÐÃ»ÓÐËÀËøµÄÏß³Ì
static HANDLE hRemoveLogThread = NULL;	//É¾³ýÃ»ÓÃÈÕÖ¾ÎÄ¼þµÄÏß³Ì

HWND hListOutput = NULL;	//Êä³ölist

int32_t get_account(DB* db, const DBT* pkey, const DBT* pdata, DBT* ikey)
{
	//¸ø¶¨Ò»¸öÍêÕûµÄbuffer£¬µÃµ½account×÷ÎªË÷Òý
	memset(ikey, 0, sizeof(DBT));
	TRoleData* pRoleData = (TRoleData*)pdata->data;

	ikey->data = pRoleData->BaseInfo.caccname;
	ikey->size = strlen(pRoleData->BaseInfo.caccname) + 1;

	return 0;
}

//==========¼ì²éÊý¾Ý¿âÓÐÃ»ÓÐËÀËøµÄÏß³Ì Add By Fellow 2003.9.10==============
DWORD WINAPI DeadlockProc(LPVOID lpParameter) {
	while (!db_table->bStop) {
		Sleep(5 * 1000);	//5ÃëÖÓÒ»´Î
		db_table->deadlock();
	}
	return 0;
}
//==========É¾³ýÃ»ÓÃÈÕÖ¾ÎÄ¼þµÄÏß³Ì Add By Fellow 2003.9.10==============
DWORD WINAPI RemoveLogProc(LPVOID lpParameter) {
	while (!db_table->bStop) {
		db_table->removeLog();
		Sleep(60 * 60 * 1000);			//1Ð¡Ê±Ò»´Î
	}
	return 0;
}

BOOL InitDBInterface(size_t nMaxRoleCount)
{
	nMaxRoleCount_InAccount = nMaxRoleCount;

	db_table = new ZDBTable("database", "roledb");

	db_table->addIndex(get_account);
	if (db_table->open())
	{
		DWORD dwThreadId, dwThrdParam = 1;
		/*

				hDeadLockThread = CreateThread(
					NULL,				// no security attributes
					0,					// use default stack size
					DeadlockProc,		// thread function
					&dwThrdParam,		// argument to thread function
					0,					// use default creation flags
					&dwThreadId);		// returns the thread identifier
				if(!hDeadLockThread)
				{
					//´´½¨Ïß³ÌÊ§°Ü,ÔÝÊ±Ã»ÓÐ´¦Àí
				}
		*/

		hRemoveLogThread = CreateThread(
			NULL,				// no security attributes 
			0,					// use default stack size  
			RemoveLogProc,		// thread function 
			&dwThrdParam,		// argument to thread function 
			0,					// use default creation flags 
			&dwThreadId);		// returns the thread identifier 
		if (!hRemoveLogThread)
		{
			//´´½¨Ïß³ÌÊ§°Ü,ÔÝÊ±Ã»ÓÐ´¦Àí
		}

		return TRUE;
	}

	return FALSE;
}

void ReleaseDBInterface()		//ÊÍ·ÅÊý¾Ý¿âÒýÇæ
{
	if (db_table)
	{
		db_table->commit();
		/*

				if(!hDeadLockThread)
					TerminateThread(hDeadLockThread, 0);
		*/

		if (!hRemoveLogThread)
			TerminateThread(hRemoveLogThread, 0);

		db_table->removeLog();

		StopBackupTimer();//Í£Ö¹±¸·ÝÏß³Ì

		db_table->close();
		delete db_table;
	}
}

//[wxb 2003-7-23]
void SetRoleInfoForGM(int32_t nInfoID, char* pRoleBuffer, char* strUser, int32_t nBufLen)
{
	char* aBuffer = new char[128 * 1024];	//64
	TRoleData* pRoleData = NULL;
	int32_t size;

	GetRoleInfo(aBuffer, strUser, size);
	if (size)
	{
		pRoleData = (TRoleData*)aBuffer;
		switch (nInfoID)
		{
		case gm_role_entergame_position:
			ASSERT(nBufLen == sizeof(GM_ROLE_DATA_SUB_ENTER_POS));
			memcpy(&(pRoleData->BaseInfo.ientergameid), pRoleBuffer, sizeof(GM_ROLE_DATA_SUB_ENTER_POS));
			break;
		default:
			ASSERT(0);
			return;
			break;
		}
		SaveRoleInfo(aBuffer, strUser, FALSE, FALSE);
	}

	delete[] aBuffer;
}

//[wxb 2003-7-22]
void* GetRoleInfoForGM(int32_t nInfoID, char* pRoleBuffer, char* strUser, int32_t& nBufLen)
{
	int32_t size = 0;
	ZCursor* cursor = db_table->search(strUser, strlen(strUser) + 1);
	//	char *buffer = db_table->search( strUser, strlen( strUser ) + 1, size );
	if (cursor)
	{
		TRoleData* pRole = (TRoleData*)cursor->data;
		switch (nInfoID) {
		case gm_role_entergame_position:
			nBufLen = sizeof(pRole->BaseInfo.ientergameid) + sizeof(pRole->BaseInfo.ientergamex) + sizeof(pRole->BaseInfo.ientergamey);
			memcpy(pRoleBuffer, &(pRole->BaseInfo.ientergameid), nBufLen);
			break;
		default:
			ASSERT(0);
			nBufLen = 0;
			break;
		}

		db_table->closeCursor(cursor);
	}
	else
	{
		nBufLen = 0;
	}

	return pRoleBuffer;
}

void* GetRoleInfo(char* pRoleBuffer, char* strUser, int32_t& nBufLen)
{
	//Êä³öÊý¾Ý======
	char aStr[1024];
	sprintf(aStr, "GetRoleInfo:%s", strUser);
	AddOutputString(hListOutput, aStr);
	//===============
	int32_t size = 0;
	ZCursor* cursor = {};
	cursor = db_table->search(strUser, strlen(strUser) + 1);
	//	char *buffer = db_table->search( strUser, strlen( strUser ) + 1, size );
	if (cursor)
	{
		nBufLen = size;
		memcpy(pRoleBuffer, cursor->data, cursor->size);
		nBufLen = cursor->size;
		memset(&cursor, 0, sizeof(cursor));
		db_table->closeCursor(cursor);
	}
	else
	{
		nBufLen = 0;
	}

	return pRoleBuffer;
}

int  CheckRoleName(char* pName)
{
	/*
	 * Role of same name only  //¼ì²â½ÇÉ«ÃûÎ¨Ò»ÐÔ
	 */

	if (!db_table)
		return 0;

	ZCursor* user_cursor = db_table->search(pName, strlen(pName) + 1);
	//		char *user_data = db_table->search( pName, strlen( pName ) + 1, size );
	if (user_cursor)
	{
		db_table->closeCursor(user_cursor);

		return 1;
	}
	return 0;
}


//±£´æ½ÇÉ«µÄÐÅÏ¢£¬Èç¹ûÊý¾Ý¿â²»´æÔÚ¸ÃÍæ¼Ò£¬ÔòÔö¼Ó¸ÃÍæ¼Ò
//bAutoInsertWhenNoExistUser ÉèÎªTRUEÊ±±íÊ¾£¬Èç¹ûÐèÒª±£´æµÄ¸ÃÍæ¼ÒÔÚÊý¾Ý¿âÖÐ²¢²»´æÔÚÔò×Ô¶¯¼ÓÈëµ½Êý¾Ý¿âÖÐ£¬FALSEÔò²»Ôö¼ÓÖ±½Ó·µ»Ø´íÎó
//×¢ÒâINIÎÄ¼þÖ»Ðë´æ·Å½«ÐèÒª¸Ä¶¯µÄÊý¾Ý£¬²»Ðè¸Ä¶¯µÄÊý¾Ý½«×Ô¶¯±£´æÔ­×´¡£
int32_t	SaveRoleInfo(char* pRoleBuffer, const char* strUser, BOOL bAutoInsertWhenNoExistUser, BOOL bAutoDeleteUser)
{
	ASSERT(pRoleBuffer);

	//ÐèÒª´æ·ÅÕÊºÅÊ×ÏÈÕÒµ½Êý¾Ý
	TRoleData* pRoleData = (TRoleData*)pRoleBuffer;

	//Êä³öÊý¾Ý======
	char aStr[1024];
	sprintf(aStr, "SaveRoleInfo:%s dwDataLen=%d", pRoleData->BaseInfo.szName, pRoleData->dwDataLen);
	AddOutputString(hListOutput, aStr);
	//===============

	if (pRoleData->dwDataLen >= 128 * 1024) //64
		return 0;//Èç¹ûÊý¾Ý´óÓÚ64K¾Í²»Ìí¼Óµ½Êý¾Ý¿â

	if (bAutoInsertWhenNoExistUser)
	{//Èç¹ûÊÇÐÂÔö½ÇÉ«¾Í°ÑÕËºÅÃû×ª³ÉÐ¡Ð´
		char* ptr = pRoleData->BaseInfo.caccname;	//°ÑÕËºÅÃû×ª³ÉÐ¡Ð´
		while (*ptr) {
			if (*ptr >= 'A' && *ptr <= 'Z') *ptr += 'a' - 'A';
			ptr++;
		}
	}

	if (!bAutoInsertWhenNoExistUser)
	{
		//DWORD	dwCRC = 0;
		//dwCRC = CRC32(dwCRC, pRoleData, pRoleData->dwDataLen - 4);
		//DWORD	dwOrigCRC = *(DWORD*)(pRoleBuffer + pRoleData->dwDataLen - 4);
		//if (dwCRC != dwOrigCRC)
		//{
		//	// TODO:
		//	FILE* fLog = fopen("crc_error", "a+b");
		//	if (fLog) {
		//		char buffer[255];
		//		sprintf(buffer, "----\r\n%s\r\b%s\r\n", pRoleData->BaseInfo.szName, pRoleData->BaseInfo.caccname);
		//		fwrite(buffer, 1, strlen(buffer), fLog);
		//		fwrite(pRoleBuffer, 1, pRoleData->dwDataLen, fLog);
		//		fclose(fLog);
		//	}
		//	return 0;
		//}
	}

	char szAccountName[32];

	int32_t nLength = strlen(pRoleData->BaseInfo.caccname);

	ASSERT(nLength > 0);

	nLength = nLength > 31 ? 31 : nLength;

	memcpy(szAccountName, pRoleData->BaseInfo.caccname, nLength);
	szAccountName[nLength] = '\0';

	char szName[32], szNewName[32];
	ZeroMemory(szName, sizeof(szName));
	ZeroMemory(szNewName, sizeof(szNewName));
	const char* pName = szName;
	const char* pNewName = szNewName;

	if (NULL == strUser || strUser[0] == 0)
	{
		int32_t len = strlen(pRoleData->BaseInfo.szName);

		ASSERT(len > 0);

		len = len > 31 ? 31 : len;

		memcpy(szName, pRoleData->BaseInfo.szName, len);
		szName[len] = '\0';
	}
	else
	{
		int32_t len = strlen(strUser);

		ASSERT(len > 0);

		memcpy(szName, strUser, len);
		szName[len] = '\0';
	}

	if (bAutoDeleteUser)
	{
		int32_t len = strlen(pRoleData->BaseInfo.szName);

	//	ASSERT(len > 0);

	//	len = len > 31 ? 31 : len;

		memcpy(szNewName, pRoleData->BaseInfo.szName, len);
		szNewName[len] = '\0';
	}

	if (bAutoInsertWhenNoExistUser)
	{
		int32_t nCount = 0;
		int32_t size = 0;

		/*
		 * Role of same name only
		 */
		//if (bAutoDeleteUser)
		//{
		//	//Êä³öÊý¾Ý======
		//	char aStr[1024];
		//	sprintf(aStr, "Search:%s->%s", pName, pNewName);
		//	AddOutputString(hListOutput, aStr);
		//	//===============
		//	ZCursor* user_cursor = db_table->search(pNewName, strlen(pNewName) + 1);
		//	if (user_cursor)
		//	{
		//		db_table->closeCursor(user_cursor);
		//		return 0;
		//	}
		//}
		//else
		{
			ZCursor* user_cursor = db_table->search(pName, strlen(pName) + 1);
			if (user_cursor)
			{
				db_table->closeCursor(user_cursor);
				return 0;
			}
		}

		/*
		 * Get count of role by the key of account
		 */
		ZCursor* cursor = db_table->search(szAccountName, strlen(szAccountName) + 1, 0);
		//		char *buffer = db_table->search( szAccountName, strlen( szAccountName ) + 1, size, 0 );

		while (cursor)
		{
			nCount++;
			if (!db_table->next(cursor))
				break;
		}

		if (nCount >= nMaxRoleCount_InAccount)
		{
			return 0;
		}
	}
	//if (bAutoDeleteUser)
	//{
	//	DeleteRole(pName);
	//	if (db_table->add(pNewName, strlen(pNewName) + 1, pRoleBuffer, pRoleData->dwDataLen))
	//	{
	//		//Êä³öÊý¾Ý======
	//		char aStr[1024];
	//		sprintf(aStr, "Rename:%s->%s", pName, pNewName);
	//		AddOutputString(hListOutput, aStr);
	//		//===============
	//		return 1;
	//	}
	//}
	//else
	{
		if (db_table->add(pName, strlen(pName) + 1, pRoleBuffer, pRoleData->dwDataLen))
		{
			return 1;
		}
	}

	return 0;
}

int ChangeRoleName(char* nScrRoleName, char* nTarRoleName)
{
	if (!db_table || !nScrRoleName || !nTarRoleName)
		return 0;
	char aStr[128] = { 0 };
	ZCursor* user_cursor = db_table->search(nScrRoleName, strlen(nScrRoleName) + 1);	//pName		strlen(szName) + 1  Ö÷Êý¾Ý¿â²éÑ¯
	if (user_cursor)
	{//Èç¹ûÓÐÕâ¸ö½ÇÉ«ÃûÁË

		ZCursor* Tar_cursor = db_table->search(nTarRoleName, strlen(nTarRoleName) + 1);
		if (Tar_cursor)
		{
			db_table->closeCursor(Tar_cursor);
			return 1;
		}

		TRoleData* pRoleData = (TRoleData*)user_cursor->data;

		if (pRoleData)
		{
			sprintf(pRoleData->BaseInfo.szName, nTarRoleName);
			db_table->remove(nScrRoleName, strlen(nScrRoleName) + 1);
			if (db_table->add(pRoleData->BaseInfo.szName, strlen(pRoleData->BaseInfo.szName) + 1, user_cursor->data, pRoleData->dwDataLen))
			{
				sprintf_s(aStr, sizeof(aStr) - 1, _TEXT("[Change RoleName]:From %s---->To:%s successful!"), nScrRoleName, nTarRoleName);
				AddOutputString(hListOutput, aStr);

				db_table->closeCursor(user_cursor);
				return 3;
			}
			else
			{
				db_table->closeCursor(user_cursor);
				return 4;
			}
		}

		db_table->closeCursor(user_cursor);
	}

	return 2;
}

int32_t GetRoleListOfAccount(char* szAccountName, S3DBI_RoleBaseInfo* RoleBaseList, int32_t nMaxCount)
{
	int32_t size = 0;
	int32_t count = 0;

	char* ptr = szAccountName;	//°ÑÕËºÅÃû×ª³ÉÐ¡Ð´
	while (*ptr) {
		if (*ptr >= 'A' && *ptr <= 'Z') *ptr += 'a' - 'A';
		ptr++;
	}

	//Êä³öÊý¾Ý======
	char aStr[1024];
	sprintf(aStr, "GetRoleListOfAccount:%s", szAccountName);
	AddOutputString(hListOutput, aStr);
	//===============

	S3DBI_RoleBaseInfo* base_ptr = RoleBaseList;

	ZCursor* cursor = db_table->search(szAccountName, strlen(szAccountName) + 1, 0);
	//	char *buffer = db_table->search( szAccountName, strlen( szAccountName ) + 1, size, 0 );

	while (count < nMaxCount && cursor)
	{
		TRoleData* pRoleData = (TRoleData*)cursor->data;

		strncpy(base_ptr->szName, pRoleData->BaseInfo.szName, 32);
		base_ptr->szName[31] = '\0';

		base_ptr->Sex = pRoleData->BaseInfo.bSex;
		base_ptr->Series = pRoleData->BaseInfo.ifiveprop;
		base_ptr->Faction = pRoleData->BaseInfo.nSect;
		base_ptr->Level = pRoleData->BaseInfo.ifightlevel + pRoleData->BaseInfo.ifighttranslife * MAX_LEVEL;

		base_ptr++;

		/*
		 * Get next info from database
		 */
		count++;
		if (!db_table->next(cursor))break;
	}
	return count;
}

bool DeleteRole(const char* strUser)
{
	if (db_table && strUser && strUser[0])
	{
		return db_table->remove(strUser, strlen(strUser) + 1);
	}
	return false;
}

// ÏÂÃæµÄº¯Êý¿ÉÄÜ´æÔÚÄÚ´æÐ¹Â©ÎÊÌâ
//char* GetAccountByUser(char * strUser)
//{//Í¨¹ýÓÃ»§Ãû²éÕÒÕÊ»§
//	char* aBuffer = new char[64 * 1024];
//	TRoleData* pRoleData;
//	int size;
//
//	GetRoleInfo(aBuffer, strUser, size);
//	if(size)
//	{
//		pRoleData = (TRoleData*)aBuffer;
//		return pRoleData->BaseInfo.caccname;
//	}
//	else
//	{
//		return NULL;
//	}
//}

//------------------------------------------------------------------------
//Êý¾Ý¿â±¸·ÝÓëÊý¾ÝÍ³¼Æ Add By Fellow At 2003.08.14
bool StartBackupTimer(int32_t aTime, DWORD bTime)
{//¿ªÊ¼ÔËÐÐ±¸·ÝÏß³Ì
	DBBackup = new CDBBackup("database", "roledb", db_table);
	DBBackup->SaveStatInfo();
	bool aStartResult = DBBackup->Open(aTime, bTime);
	return aStartResult;
}
bool StartBackupTimerByMinutes(DWORD minutes)
{
    DBBackup = new CDBBackup("database", "roledb", db_table);
    DBBackup->SaveStatInfo();
    return DBBackup->OpenEveryMinutes(minutes);
}

bool StopBackupTimer()
{//½áÊøÔËÐÐ±¸·ÝÏß³Ì
	if (!DBBackup)return false;
	while (IsBackupWorking()) {}//µÈ´ý±¸·ÝÏß³Ì½áÊø

	bool aResult = DBBackup->Close();
	delete DBBackup;
	DBBackup = NULL;
	return aResult;
}

bool SuspendBackupTimer()
{//¹ÒÆðÏß³Ì
	if (!DBBackup)return false;
	return DBBackup->Suspend();
}

bool ResumeBackupTimer()
{//¼ÌÐøÔËÐÐÏß³Ì
	if (!DBBackup)return false;
	return DBBackup->Resume();
}

bool IsBackupThreadWorking()
{//Ïß³ÌÊÇ·ñÕýÔÚÔËÐÐ
	if (!DBBackup)return false;
	return DBBackup->IsWorking();
}

bool IsBackupWorking()
{//ÊÇ·ñÔÚ±¸·Ý
	if (!DBBackup)return false;
	return DBBackup->IsBackuping();
}

bool DoManualBackup()
{//ÊÖ¹¤±¸·Ý
	if (!DBBackup)return false;
	return DBBackup->ManualBackup();
}

bool GetGameStat(TGAME_STAT_DATA* aStatData)
{
	if (!DBBackup)
		return false;
	TGAME_STAT_DATA tmpStatData = DBBackup->GetSendStatData();
	if (aStatData)
	{
		memset(aStatData, 0, sizeof(TGAME_STAT_DATA));
		memcpy(aStatData, &tmpStatData, sizeof(TGAME_STAT_DATA));
		return true;
	}
	else
		return false;
}

void AddOutputString(HWND hListCtrl, char* aStr)
{//Ìí¼Ó²Ù×÷Êä³öÎÄ×Ö
	if (hListCtrl && ::IsWindow(hListCtrl))
	{
		int32_t nCount = ::SendMessage(hListCtrl, LB_GETCOUNT, 0, 0);
		if (nCount >= 100)
		{
			::SendMessage(hListCtrl, LB_DELETESTRING, 100, 0);
		}
		int32_t nIndex = ::SendMessage(hListCtrl, LB_INSERTSTRING, 0, (LPARAM)aStr);//°ÑÐÂµÄÐÅÏ¢Ìíµ½µÚÒ»¸ö
	}
}
bool GetLastBackupPath(char* buf, size_t bufSize)
{
    CDBBackup::GetLastBackupPath(buf, bufSize);
    return true;
}
bool WasLastBackupOk()
{
    return CDBBackup::WasLastBackupOk();
}
bool InitBackupEngine()
{
    if (DBBackup) return true;

    // Kh?i t?o engine backup dùng b?ng hi?n có
    DBBackup = new CDBBackup("database", "roledb", db_table);
    DBBackup->SaveStatInfo();
    return true;
}