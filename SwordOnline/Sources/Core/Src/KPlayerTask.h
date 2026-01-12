//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerTask.h
// Date:	2002.10.05
// Code:	边城浪子
// Desc:	PlayerTask Class
//---------------------------------------------------------------------------
#include 	"GameDataDef.h"

#ifndef KPLAYERTASK_H
#define KPLAYERTASK_H


#ifdef _SERVER
#define TASKGLOBALVALUENUM 5000
extern INT		g_TaskGlobalValue[TASKGLOBALVALUENUM];

extern KTabFile g_WayPointTabFile;
extern KTabFile g_StationTabFile;
extern KTabFile g_DockTabFile;

extern KTabFile g_StationPriceTabFile;
extern KTabFile g_WayPointPriceTabFile;
extern KTabFile g_DockPriceTabFile;

extern INT* g_pStationPriceTab;
extern INT* g_pWayPointPriceTab;
extern INT* g_pDockPriceTab;

#endif


class KPlayerTask
{
	friend class KPlayer;
private:
	INT			m_nPlayerIndex;
public:
	char		szSave[MAX_TASK][16];					// 用于记录任务是否完成，须保存到数据库
	char		szClear[MAX_TEMP_TASK][16];
	// 用于记录任务过程中的中间步骤的完成情况，不保存到数据库，玩家下线后次数据清空
public:
	KPlayerTask();									// 构造函数
	void		Release();							// 清空
	void		ClearTempVar();						// 清除临时过程控制变量
	// 得到任务完成情况
	INT			GetClearVal(INT nNo);				// 得到临时过程控制变量值
	const char* GetClearStr(INT nNo);				// 得到临时过程控制变量值
	void		SetClearVal(INT nNo, char* szVal);		// 设定临时过程控制变量值
	void		SetSaveVal(INT nTaskID, char* szTaskVal, BOOL bSync = FALSE);
	void		SetSaveVal(INT nTaskID, INT nTaskVal, BOOL bSync = FALSE);
	INT			GetSaveVal(INT nNo);
	const char* GetSaveStr(INT nNo);

	BOOL	SetValue(INT nIndex, INT nValue);
	INT		GetValue(INT nIndex);
	void		Init(INT nPlayerIdx);
#ifdef _SERVER
	BOOL		SyncTaskValue(INT nTaskID);
#endif

};


#endif
