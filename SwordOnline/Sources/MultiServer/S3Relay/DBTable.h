#ifndef DBTABLE_H
#define DBTABLE_H
//定义支持复制、事务处理的，多索引记录的数据表类

#include "db_cxx.h"

#define MAX_INDEX			16
#define MAX_TABLE_NAME		256

typedef INT (*GetIndexFunc)(DB*, const DBT*, const DBT*, DBT*);

class ZDBTable {
	DB* primary_db;												//存放主键-数据的数据库
	DB* index_db[MAX_INDEX];									//存放索引-主键的数据库
	GetIndexFunc get_index_funcs[MAX_INDEX];					//从数据中获得二级key的的函数列表
	bool is_index_unique[MAX_INDEX];							//索引是否唯一
	INT index_number;											//二级索引数目
	char table_name[MAX_TABLE_NAME];
	DBC* dbcp;													//目前考虑单线程，使用一个CURSOR
protected:
	char env_path[MAX_TABLE_NAME] = {};
	DB_ENV* dbenv;												//数据库环境
	char* _search(bool bKey, const char* key_ptr, INT key_size, INT& size, INT index);		//搜索指定记录
	char* _next(bool bKey, INT& size);															//下一个记录
public:
	ZDBTable(const char* path, const char* name);			//环境目录和数据表的名字
	virtual ~ZDBTable();

	INT addIndex(GetIndexFunc func, bool isUnique = false);		//增加二级索引
	bool open();												//打开数据表
	void close();												//关闭数据表
	bool commit();												//基于事务的提交
	//基本记录操作
	bool add(const char* key_ptr, INT key_size, const char* data_ptr, INT data_size);
	char* search(const char* key_ptr, INT key_size, INT& size, INT index = -1) {
		return _search(false, key_ptr, key_size, size, index);		//搜索指定记录
	}
	char* next(INT& size) {															//下一个记录
		return _next(false, size);
	}
	char* search_key(const char* key_ptr, INT key_size, INT& size, INT index = -1) {		//搜索指定记录，返回主键值
		return _search(true, key_ptr, key_size, size, index);		//搜索指定记录
	}
	char* next_key(INT& size) {															//下一个记录，返回主键值
		return _next(true, size);
	}
	bool remove(const char* key_ptr, INT key_size, INT index = -1);

	//遍历纪录记录(by Fellow)
	enum CursorPointer { cpFirst = DB_FIRST, cpCurrent = DB_CURRENT, cpNext = DB_NEXT, cpLast = DB_LAST };
	char* GetRecord(INT& size, CursorPointer cpMode, INT index = -1);//取得按游标某一个数据
	char* GetRecord_key(INT& size, CursorPointer cpMode, INT index = -1);	//取得按游标某一个数据的Key值

	bool GetRecordEx(char* aBuffer, INT& size, char* aKeyBuffer, INT& keysize, CursorPointer cpMode, INT index = -1);		//取得按游标某一个数据和key(新版函数)
};


//数据库操作类（只读）		Add By Fellow At 2003.08.14
//支持并发读取
class CDBTableReadOnly
{
	DB* primary_db = NULL;												//存放主键-数据的数据库
	DBC* dbcp = NULL;													//目前考虑单线程，使用一个CURSOR
	char table_name[MAX_TABLE_NAME];
protected:
	char env_path[MAX_TABLE_NAME] = {};
	DB_ENV* dbenv = NULL;												//数据库环境
	char* _search(bool bKey, const char* key_ptr, INT key_size, INT& size);		//搜索指定记录
	char* _next(bool bKey, INT& size);							//下一个记录
public:
	CDBTableReadOnly(const char* path, const char* name);			//环境目录和数据表的名字
	virtual ~CDBTableReadOnly();

	bool open();												//打开数据表
	void close();												//关闭数据表
	//基本记录操作
	char* search(const char* key_ptr, INT key_size, INT& size) {
		return _search(false, key_ptr, key_size, size);		//搜索指定记录
	}
	char* next(INT& size) {											//下一个记录
		return _next(false, size);
	}

	//遍历纪录记录(by Fellow)
	//enum CursorPointer { cpFirst = 10, cpCurrent = 8, cpNext = 19, cpLast = 18 };
	enum CursorPointer { cpFirst = DB_FIRST, cpCurrent = DB_CURRENT, cpNext = DB_NEXT, cpLast = DB_LAST };
	char* GetRecord(INT& size, CursorPointer cpMode);//取得按游标某一个数据
	char* GetRecord_key(INT& size, CursorPointer cpMode);	//取得按游标某一个数据的Key值
	bool GetRecordEx(char* aBuffer, INT& size, char* aKeyBuffer, INT& keysize, CursorPointer cpMode);

	char* search_key(const char* key_ptr, INT key_size, INT& size) {		//搜索指定记录，返回主键值
		return _search(true, key_ptr, key_size, size);		//搜索指定记录
	}
	char* next_key(INT& size) {															//下一个记录，返回主键值
		return _next(true, size);
	}
};
#endif
