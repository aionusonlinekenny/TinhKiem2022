#include "stdafx.h"
#include "DBTable.h"
#ifndef WIN32
#include <sys/stat.h>
#else
#include <direct.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Khai báo hằng số cho cấu hình
#define DB_PATH "player_data.db"
#define CACHE_SIZE (128 * 1024 * 1024) // 128 MB
#define ENV_FLAGS (DB_CREATE | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN | DB_RECOVER | DB_THREAD | DB_PRIVATE )	//| DB_INIT_LOCK

ZDBTable::ZDBTable(const char* path, const char* name) {
#ifdef WIN32
	char* r = getcwd(env_path, MAX_TABLE_NAME);
#else
#endif
	strcat(env_path, "\\");
	strcat(env_path, path);
#ifdef WIN32
	int32_t ret = mkdir(env_path);
#else
	int32_t ret = mkdir(env_path, 0);
#endif
	if (!db_env_create(&dbenv, 0)) {
		dbenv->set_errpfx(dbenv, "index_db");
		dbenv->set_lg_regionmax(dbenv, 512 * 1024);
		dbenv->set_lg_max(dbenv, 16 * 1024 * 1024); //16
		dbenv->set_lg_bsize(dbenv, 2 * 1024 * 1024);
		dbenv->set_cachesize(dbenv, 0, CACHE_SIZE, 1);
		dbenv->set_flags(dbenv, DB_LOG_AUTO_REMOVE, 1);

		if (!dbenv->open(dbenv, env_path, ENV_FLAGS, 0)) {	//0
			ret = dbenv->set_flags(dbenv, DB_AUTO_COMMIT | DB_TXN_NOSYNC, 1); //DB_TXN_NOSYNC
			index_number = 0;
			strcpy(table_name, name);
			return;				//³É¹¦ÁË
		}
		dbenv->close(dbenv, 0);
	}
	dbenv = NULL;
}

ZDBTable::~ZDBTable() {
	if (dbenv)
		dbenv->close(dbenv, 0);
}

int32_t ZDBTable::addIndex(GetIndexFunc func, bool isUnique) {
	if (!dbenv)
		return -1;
	if (index_number + 1 >= MAX_INDEX)
		return index_number;
	get_index_funcs[index_number] = func;
	is_index_unique[index_number] = isUnique;
	return index_number++;
}

bool ZDBTable::open() {
	if (!dbenv)
		return false;
	bStop = false;
	char index_table_name[MAX_TABLE_NAME];
	int32_t index;
	int32_t ret;
	if (!db_create(&primary_db, dbenv, 0)) {
		if (!primary_db->open(primary_db, NULL, table_name, NULL, DB_BTREE, DB_CREATE | DB_AUTO_COMMIT | DB_THREAD, 0664)) {	//´ò¿ªÖ÷Êý¾Ý¿â
			for (index = 0; index < index_number; index++) {
				sprintf(index_table_name, "%s.%d", table_name, index);
				if (!db_create(&index_db[index], dbenv, 0)) {
					if (!is_index_unique[index]) {
						if (index_db[index]->set_flags(index_db[index], DB_DUP | DB_DUPSORT)) break;
					}
					if (index_db[index]->open(index_db[index], NULL, index_table_name, NULL, DB_BTREE, DB_CREATE | DB_AUTO_COMMIT | DB_THREAD, 0664))
						break;
					if (ret = primary_db->associate(primary_db, NULL, index_db[index], get_index_funcs[index], DB_AUTO_COMMIT)) {
						index_db[index]->close(index_db[index], 0);
						break;
					}
				}
				else break;
			}
			if (index == index_number)
				return true;										//³É¹¦ÁË
			else while (--index && index >= 0)
				(index_db[index])->close(index_db[index], 0);				//³ö´í£¬¹Ø±ÕÇ°ÃæµÄË÷Òý±í
			primary_db->close(primary_db, 0);
		}
	}
	return false;
}

void ZDBTable::close() {
	if (!dbenv)
		return;

	primary_db->close(primary_db, 0);

	for (int32_t index = 0; index < index_number; index++)
		index_db[index]->close(index_db[index], 0);
}

bool ZDBTable::commit() {
	return true;
}

bool ZDBTable::add(const char* key_ptr, int32_t key_size, const char* data_ptr, int32_t data_size) {
	DBT data, key;
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));
	key.data = (void*)key_ptr;
	key.size = key_size;
	data.data = (void*)data_ptr;
	data.size = data_size;

	int32_t ret;
	int32_t retry = 0;
RETRY:
	ret = primary_db->put(primary_db, NULL, &key, &data, DB_AUTO_COMMIT);
	if (ret == DB_LOCK_DEADLOCK && ++retry < MAX_RETRY) {
		if (bStop)
			return false;
		goto RETRY;
	}
	if (ret)
		return false;
	else
		return true;
}

bool ZDBTable::remove(const char* key_ptr, int32_t key_size, int32_t index) {
	DBT data, key;
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));
	key.data = (void*)key_ptr;
	key.size = key_size;
	int32_t ret;
	int32_t retry = 0;
RETRY:
	ret = primary_db->del(primary_db, NULL, &key, DB_AUTO_COMMIT);
	if (ret == DB_LOCK_DEADLOCK && ++retry < MAX_RETRY) {
		if (bStop) return false;
		goto RETRY;
	}
	if (ret)
		return false;
	else
		return true;
}

//ZCursor* ZDBTable::_search(bool bKey, const char* key_ptr, int32_t key_size, int32_t index) {
//	DBT key, data, pkey;
//	if (index < -1 || index >= index_number) return NULL;
//	memset(&key, 0, sizeof(key));
//	memset(&data, 0, sizeof(data));
//	memset(&pkey, 0, sizeof(pkey));
//	key.flags = DB_DBT_MALLOC;
//	data.flags = DB_DBT_MALLOC;
//	pkey.flags = DB_DBT_MALLOC;
//	key.data = (void*)key_ptr;
//	key.size = key_size;
//	DBC* dbcp = new DBC();
//	if (!key_ptr || !key_size) {							//Ã»ÓÐÉè¶¨Ë÷ÒýÖµ£¬ÒªÇó±éÀúÊý¾Ý¿â
//		if (index_db[index]->cursor(index_db[index], NULL, &dbcp, 0)) {
//			return NULL;
//		}
//		if (dbcp->c_get(dbcp, &key, &data, DB_FIRST)) {
//			dbcp->c_close(dbcp);
//			return NULL;
//		}
//	}
//	else {
//		if (index == -1) {									//Ö÷¼üËÑË÷
//			if (primary_db->get(primary_db, NULL, &key, &data, 0))
//				return NULL;
//		}
//		else if (is_index_unique[index]) {					//Ã»ÓÐÖØ¸´Ë÷Òý
//			if (bKey) {
//				if (index_db[index]->pget(index_db[index], NULL, &key, &pkey, &data, 0))
//					return NULL;
//			}
//			else {
//				if (index_db[index]->get(index_db[index], NULL, &key, &data, 0))
//					return NULL;
//			}
//		}
//		else {												//´ò¿ªÓÎ±ê
//			if (index_db[index]->cursor(index_db[index], NULL, &dbcp, 0)) {
//				return NULL;
//			}
//			if (bKey) {
//				if (dbcp->c_pget(dbcp, &key, &pkey, &data, DB_SET)) {
//					dbcp->c_close(dbcp);
//					return NULL;
//				}
//			}
//			else {
//				if (dbcp->c_get(dbcp, &key, &data, DB_SET)) {
//					dbcp->c_close(dbcp);
//					return NULL;
//				}
//			}
//		}
//	}
//	ZCursor* result = new ZCursor;
//	result->index = 0;
//	result->dbcp = dbcp;
//	if (bKey)
//	{
//		result->data = (char*)pkey.data;
//		result->size = pkey.size;
//
//		pkey.data = NULL;
//	}
//	else
//	{
//		result->data = (char*)data.data;
//		result->size = data.size;
//
//		data.data = NULL;
//	}
//
//	if (!key_ptr || !key_size)
//	{
//		result->key = (char*)key.data;
//		result->key_size = key.size;
//		result->bTravel = true;
//
//		key.data = NULL;
//	}
//	else
//	{
//		result->bTravel = false;
//	}
//
//	if (pkey.data)
//		free(pkey.data);
//	if (data.data)
//		free(data.data);
//
//	return result;
//}

ZCursor* ZDBTable::_search(bool bisKey, const char* key_ptr, int key_size, int index)
{
	DBT key, data, pkey;
	if (index < -1 || index >= index_number)
		return NULL;

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));

	key.flags = DB_DBT_MALLOC;
	data.flags = DB_DBT_MALLOC;
	pkey.flags = DB_DBT_MALLOC;
	key.data = (void*)key_ptr;
	key.size = key_size;

	DBC* dbcp = NULL;


	//char nMsg[128]={0};
	//sprintf_s(nMsg,sizeof(nMsg)-1,"ËÑË÷:%s",key_ptr);
	//::MessageBox(NULL,nMsg, "Warning", MB_OK | MB_ICONSTOP );

	if (!key_ptr || !key_size) //Ã»ÓÐÖ¸¶¨Êý¾ÝµÄ
	{//Ã»ÓÐÉè¶¨Ë÷ÒýÖµ£¬ÒªÇó±éÀúÊý¾Ý¿â  
		if (index == -1)
		{//Ö÷Êý¾Ý¿âËÑË÷
			if (primary_db->cursor(primary_db, NULL, &dbcp, 0))
			{
				return NULL;
			}

			if (dbcp->c_get(dbcp, &key, &data, DB_FIRST))
			{
				dbcp->c_close(dbcp);
				return NULL;
			}
		}
		else
		{
			if (index_db[index]->cursor(index_db[index], NULL, &dbcp, 0))
			{
				return NULL;
			}

			if (dbcp->c_get(dbcp, &key, &data, DB_FIRST))
			{
				dbcp->c_close(dbcp);
				return NULL;
			}

		}
	}
	else
	{//Ö¸¶¨¹Ø¼ü×Ö²éÑ¯
		if (index == -1)
		{//Ö÷Êý¾Ý¿âËÑË÷  
			if (!primary_db)
				return NULL;

			if (bisKey)
			{//Ö÷Êý¾Ý¿â
				if (primary_db->pget(primary_db, NULL, &key, &pkey, &data, 0))
					return NULL;
			}
			else
			{
				if (primary_db->get(primary_db, NULL, &key, &data, 0))
					return NULL;
			}
		}
		else if (is_index_unique[index]) //Ä¬ÈÏÎª false
		{//Ã»ÓÐÖØ¸´Ë÷Òý
			if (!index_db[index])
				return NULL;

			if (bisKey)
			{
				if (index_db[index]->pget(index_db[index], NULL, &key, &pkey, &data, 0))
					return NULL;
			}
			else
			{
				if (index_db[index]->get(index_db[index], NULL, &key, &data, 0))
					return NULL;
			}
		}
		else
		{//´ò¿ªÓÎ±ê	¶þ¼¶Êý¾Ý¿â  ´Óindex0 Êý¾Ý¿âËÑË÷
			if (!index_db[index])
				return NULL;

			if (index_db[index]->cursor(index_db[index], NULL, &dbcp, 0))
			{
				return NULL;
			}

			if (bisKey)
			{
				if (dbcp->c_pget(dbcp, &key, &pkey, &data, DB_SET))
				{
					dbcp->c_close(dbcp);
					return NULL;
				}
			}
			else
			{
				if (dbcp->c_get(dbcp, &key, &data, DB_SET))
				{
					dbcp->c_close(dbcp);
					return NULL;
				}
			}
		}
	}

	ZCursor* result = new ZCursor;

	if (!result)
		return NULL;

	result->index = 0;
	result->dbcp = dbcp;

	if (bisKey)
	{
		result->data = (char*)pkey.data;
		result->size = pkey.size;
		pkey.data = NULL;

	}
	else
	{
		result->data = (char*)data.data;
		result->size = data.size;
		data.data = NULL;
	}

	if (!key_ptr || !key_size)
	{//Èç¹ûÃ»ÓÐ¹Ø¼ü×Ö ¾ÍÀú±é
		result->key = (char*)key.data;
		result->key_size = key.size;
		result->bTravel = true; //À÷±é
		key.data = NULL;
	}
	else
	{//²»Àú±é
		result->bTravel = false;
	}

	//if(pkey.data) free(pkey.data);
	//if(data.data) free(data.data);

	return result;
}

//bool ZDBTable::_next(bool bKey, ZCursor* cursor) {
//	DBT key, data, pkey;
//	//cursor->index = 0;
//	memset(&key, 0, sizeof(key));
//	memset(&data, 0, sizeof(data));
//	memset(&pkey, 0, sizeof(pkey));
//	key.flags = DB_DBT_MALLOC;
//	data.flags = DB_DBT_MALLOC;
//	pkey.flags = DB_DBT_MALLOC;
//	if (!cursor || !cursor->dbcp)
//		return false;
//	//free(cursor->data);
//	memset(&data, 0, sizeof(data));
//	cursor->index++;
//	if (cursor->bTravel) {
//		//free(cursor->key);
//		memset(cursor->key, 0, sizeof(cursor->key));
//		if (cursor->dbcp->c_get(cursor->dbcp, &key, &data, DB_NEXT)) {
//			cursor->dbcp->c_close(cursor->dbcp);
//			delete cursor;
//			return false;
//		}
//	}
//	else {
//		if (bKey) {
//			if (cursor->dbcp->c_pget(cursor->dbcp, &key, &pkey, &data, DB_NEXT_DUP)) {
//				cursor->dbcp->c_close(cursor->dbcp);
//				delete cursor;
//				return false;
//			}
//		}
//		else {
//			if (cursor->dbcp->c_get(cursor->dbcp, &key, &data, DB_NEXT_DUP)) {
//				cursor->dbcp->c_close(cursor->dbcp);
//				delete cursor;
//				return false;
//			}
//		}
//	}
//	if (bKey)
//	{
//		cursor->data = (char*)pkey.data;
//		cursor->size = pkey.size;
//
//		pkey.data = NULL;
//	}
//	else {
//		cursor->data = (char*)data.data;
//		cursor->size = data.size;
//
//		data.data = NULL;
//	}
//
//	if (cursor->bTravel)
//	{
//		cursor->key = (char*)key.data;
//		cursor->key_size = key.size;
//
//		key.data = NULL;
//	}
//
//	if (key.data)
//		memset(key.data, 0, sizeof(key.data)); //free(key.data);
//	if (pkey.data)
//		memset(pkey.data, 0, sizeof(pkey.data)); //free(pkey.data);
//	if (data.data)
//		memset(data.data, 0, sizeof(data.data)); //free(data.data);
//
//	return true;
//}

bool ZDBTable::_next(bool bisKey, ZCursor* cursor)
{
	DBT key, data, pkey;

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));

	key.flags = DB_DBT_MALLOC;
	data.flags = DB_DBT_MALLOC;
	pkey.flags = DB_DBT_MALLOC;

	if (!cursor || !cursor->dbcp)
		return false;

	/*if (cursor->data)
	{//ÓÐBUG£¿±ÀÀ£ÁË
	   free(cursor->data); //ÊÍ·ÅÔ­À´µÄÊý¾Ý
	   cursor->data = NULL;
	}*/

	cursor->index++;

	//char nMsg[128]={0};
	//sprintf_s(nMsg,sizeof(nMsg)-1,"_nextÏÂÒ»¸ö¼ÇÂ¼ËÑË÷:%s",cursor->key);
	//::MessageBox(NULL,nMsg, "Warning", MB_OK | MB_ICONSTOP );

	if (cursor->bTravel) {
		//free(cursor->key);
		if (cursor->dbcp->c_get(cursor->dbcp, &key, &data, DB_NEXT)) {
			cursor->dbcp->c_close(cursor->dbcp);
			cursor->dbcp = NULL;
			delete cursor;
			cursor = NULL;
			return false;
		}
	}
	else {
		if (bisKey) {
			if (cursor->dbcp->c_pget(cursor->dbcp, &key, &pkey, &data, DB_NEXT_DUP)) {
				cursor->dbcp->c_close(cursor->dbcp);
				cursor->dbcp = NULL;
				delete cursor;
				cursor = NULL;
				return false;
			}
		}
		else {
			if (cursor->dbcp->c_get(cursor->dbcp, &key, &data, DB_NEXT_DUP)) {
				cursor->dbcp->c_close(cursor->dbcp);
				cursor->dbcp = NULL;
				delete cursor;
				cursor = NULL;
				return false;
			}
		}
	}

	if (bisKey)
	{
		cursor->data = (char*)pkey.data;
		cursor->size = pkey.size;
		pkey.data = NULL;
	}
	else {
		cursor->data = (char*)data.data;
		cursor->size = data.size;
		data.data = NULL;

	}

	if (cursor->bTravel)
	{
		cursor->key = (char*)key.data;
		cursor->key_size = key.size;
		key.data = NULL;
	}

	//sprintf_s(nMsg,sizeof(nMsg)-1,"_nextBÏÂÒ»¸ö¼ÇÂ¼ËÑË÷:%s",cursor->key);
	//::MessageBox(NULL,nMsg, "Warning", MB_OK | MB_ICONSTOP );

	/*if (key.data)  free(key.data);
	if (pkey.data)   free(pkey.data);
	if (data.data)   free(data.data);*/

	return true;
}

void ZDBTable::removeLog() {						//Çå³ýÈÕÖ¾ÎÄ¼þ
	int32_t ret;
	char** begin, ** list;

	if (dbenv == NULL)
		return;

	if ((ret = dbenv->txn_checkpoint(dbenv, 0, 0, 0)) != 0)
		return;

	if ((ret = dbenv->log_archive(dbenv, &list, DB_ARCH_ABS)) != 0) {
		return;
	}

	if (list != NULL) {
		for (begin = list; *list != NULL; ++list)
			if ((ret = ::remove(*list)) != 0) {
				free(begin);
				return;
			}
		//free(begin);
		//memset(begin, 0, sizeof(begin));
	}
}
