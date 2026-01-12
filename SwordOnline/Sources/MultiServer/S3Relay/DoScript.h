// -------------------------------------------------------------------------
//	文件名		：	DoScript.h
//	创建者		：	万里
//	创建时间	：	2003-9-16 21:06:45
//	功能描述	：	
//
// -------------------------------------------------------------------------
#ifndef __DOSCRIPT_H__
#define __DOSCRIPT_H__

#include "../../Engine/src/KWin32.h"
#include "../../Engine/src/KLuaScript.h"

BOOL InitScript();
BOOL ExcuteScript(DWORD nIP, DWORD nRelayID, const char * ScriptCommand);
BOOL UninitScript();

void BroadGlobal(const void* pData, UINT size, DWORD nFromIP, DWORD nFromRelayID);
void BroadGame(DWORD nToIP, const void* pData, UINT size, DWORD nFromIP, DWORD nFromRelayID);
void RootSend(DWORD nToIP, DWORD nToRelayID, const void* pData, UINT size);

#endif // __DOSCRIPT_H__