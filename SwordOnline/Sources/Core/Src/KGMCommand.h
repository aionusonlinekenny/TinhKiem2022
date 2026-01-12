#ifndef KGMCOMMAND_H
#define KGMCOMMAND_H

#include "KCore.h"
enum	EGameMasterCommand
{
		GMDoScriptAction,			//指对某个玩家执行某个脚本指令
		GMDoWorldScriptAction,		//指对某台游戏世界执行某
		GMRunScriptFile,
		GMReloadScriptFile,
		GMReloadAllScriptFile,
};

struct TGameMaster_Command
{
	char				Command[20];
	EGameMasterCommand	 eCommandId;
};

extern INT GetNextUnit(const char * szString , const char cDiv, INT nLen, char * szResult);
extern BOOL TextMsgProcessGMCmd(INT nPlayerIdx, const char * pGMCmd, INT nLen);
#ifdef _DEBUG
extern CORE_API BOOL TextGMFilter(INT nPlayerIdx, const char* pText, INT nLen);
#else
extern BOOL TextGMFilter(INT nPlayerIdx, const char* pText, INT nLen);
#endif

extern BOOL  ProcessGMCommand(INT nPlayerIdx, EGameMasterCommand eCommand, const char * pParam, INT nLen);

#endif
