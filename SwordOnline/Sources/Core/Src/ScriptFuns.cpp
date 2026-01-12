/*******************************************************************************
// FileName			:	ScriptFuns.cpp
// FileAuthor		:	RomanDou
// FileCreateDate	:	2002-11-19 15:58:20
// FileDescription	:	½Å±¾Ö¸Áî¼¯
// Revision Count	:
*******************************************************************************/
#ifndef WIN32
#include <string>
#endif

#include "KWin32.h"
#include "KEngine.h"
#include "KDebug.h"
#include "KStepLuaScript.h"
#include "LuaLib.h"
#include "KScriptList.h"
#include <string.h>
#include "LuaFuns.h"
#include "KCore.h"
#include "KNpc.h"
#include "KSubWorld.h"
#include "KObjSet.h"
#include "KItemSet.h"
//#include "KNetClient.h"
#include "KScriptValueSet.h"
#include "KNpcSet.h"
#include "KItemChangeRes.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
#include "KSubWorldSet.h"
#include "KProtocolProcess.h"
#include "KBuySell.h"
#include "KTaskFuns.h"
#include "KPlayerDef.h"
#include "KGameData.h"
#include "KNpcTemplate.h"
#include "KTongData.h"
#include "KLadder.h"
#include "KMath.h"
#include "KItemGenerator.h"
#include "../../Engine/Src/Text.h"
#include "../../Engine/Src/KSG_StringProcess.h"
#include <time.h>
#include <shellapi.h>
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/interface/iServer.h"
#include "../../../Headers/KProtocolDef.h"
#include "../../../Headers/KProtocol.h"
#include "../../../Headers/KRelayProtocol.h"
#include "../../../Headers/KTongProtocol.h"
#include "KNewProtocolProcess.h"
#endif
#include "KSortScript.h"
#ifndef __linux
#include "Shlwapi.h"
#define WIN32_LEAN_AND_MEAN // Fixed By MrChuCong@gmail.com
#include "windows.h"
#include "Winsock2.h" // Fixed By MrChuCong@gmail.com
#include "winbase.h"
#include <direct.h>
#include <iostream>
#include <mutex>
#else
#include "unistd.h"
#endif

#ifdef _SERVER
std::mutex		mutex; // Khai báo một mutex để sử dụng cho việc đồng bộ hóa
#endif

#ifdef _STANDALONE
#include "KSG_StringProcess.h"
#else
#include "../../Engine/Src/KSG_StringProcess.h"
#endif

#include <sstream>
#include <KStrBase.h>

#ifndef WIN32
typedef struct  _SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
}	SYSTEMTIME;
typedef struct  _FILETIME
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
}	FILETIME;
#endif

inline const char* _ip2a(DWORD ip) { in_addr ia; ia.s_addr = ip; return inet_ntoa(ia); }
inline DWORD _a2ip(const char* cp) { return inet_addr(cp); }

KScriptList		g_StoryScriptList;
KStepLuaScript* LuaGetScript(Lua_State* L);
INT	GetPlayerIndex(Lua_State* L);
extern INT g_GetPriceToStation(INT, INT);
extern INT g_GetPriceToWayPoint(INT, INT);
extern INT g_GetPriceToDock(INT, INT);

// Hàm để tách chuỗi theo ký tự phân cách
std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

//BitValue = GetBit(Value, BitNo)
INT LuaGetBit(Lua_State* L)
{
	INT nBitValue = 0;
	INT nIntValue = (INT)Lua_ValueToNumber(L, 1);
	INT nBitNumber = (INT)Lua_ValueToNumber(L, 2);

	if (nBitNumber >= 32 || nBitNumber <= 0)
		goto lab_getbit;
	nBitValue = (nIntValue & (1 << (nBitNumber - 1))) != 0;
lab_getbit:
	Lua_PushNumber(L, nBitValue);
	return 1;
}

//NewBit = SetBit(Value, BitNo, BitValue)
INT LuaSetBit(Lua_State* L)
{
	INT nIntValue = (INT)Lua_ValueToNumber(L, 1);
	INT nBitNumber = (INT)Lua_ValueToNumber(L, 2);
	INT nBitValue = (INT)Lua_ValueToNumber(L, 3);
	nBitValue = (nBitValue == 1);

	if (nBitNumber > 32 || nBitNumber <= 0)
		goto lab_setbit;

	nIntValue = (nIntValue | (1 << (nBitNumber - 1)));
lab_setbit:
	Lua_PushNumber(L, nIntValue);
	return 1;
}

//ByteValue = GetByte(Value, ByteNo)
INT LuaGetByte(Lua_State* L)
{
	INT nByteValue = 0;
	INT nIntValue = (INT)Lua_ValueToNumber(L, 1);
	INT nByteNumber = (INT)Lua_ValueToNumber(L, 2);

	if (nByteNumber > 4 || nByteNumber <= 0)
		goto lab_getByte;
	nByteValue = (nIntValue & (0xff << ((nByteNumber - 1) * 8))) >> ((nByteNumber - 1) * 8);

lab_getByte:
	Lua_PushNumber(L, nByteValue);
	return 1;
}

//NewByte = SetByte(Value, ByteNo, ByteValue)
INT LuaSetByte(Lua_State* L)
{
	BYTE* pByte = NULL;
	INT nIntValue = (INT)Lua_ValueToNumber(L, 1);
	INT nByteNumber = (INT)Lua_ValueToNumber(L, 2);
	INT nByteValue = (INT)Lua_ValueToNumber(L, 3);
	nByteValue = (nByteValue & 0xff);

	if (nByteNumber > 4 || nByteNumber <= 0)
		goto lab_setByte;

	pByte = (BYTE*)&nIntValue;
	*(pByte + (nByteNumber - 1)) = (BYTE)nByteValue;
	//nIntValue = (nIntValue | (0xff << ((nByteNumber - 1) * 8) )) ;
lab_setByte:
	Lua_PushNumber(L, nIntValue);
	return 1;
}
//------------------------------

//============================================
INT GetSubWorldIndex(Lua_State* L)
{
	Lua_GetGlobal(L, SCRIPT_SUBWORLDINDEX);
	if (lua_isnil(L, Lua_GetTopIndex(L)))
		return -1;
	INT nIndex = (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	if (nIndex >= MAX_SUBWORLD || nIndex <= 0)
	{
		_ASSERT(0);
		return -1;
	}
	if (SubWorld[nIndex].m_nIndex >= MAX_SUBWORLD || SubWorld[nIndex].m_nIndex < 0)
	{
		_ASSERT(0);
		return -1;
	}
	return nIndex;
}

//Idx = SubWorldID2Idx(dwID)
INT LuaSubWorldIDToIndex(Lua_State* L)
{
	INT nTargetSubWorld = -1;
	INT nSubWorldID = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_subworldid2idx;

	nSubWorldID = (INT)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID);

lab_subworldid2idx:
	Lua_PushNumber(L, nTargetSubWorld);
	return 1;
}

INT LuaSubWorldIndexToID(Lua_State* L)
{
	INT nTargetSubWorld = -1;
	INT nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_subworldid2idx;

	nSubWorldIndex = (INT)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = SubWorld[nSubWorldIndex].m_SubWorldID;

lab_subworldid2idx:
	Lua_PushNumber(L, nTargetSubWorld);
	return 1;
}

/*
Say(sMainInfo, nSelCount, sSel1, sSel2, sSel3, .....,sSeln)
Say(nMainInfo, nSelCount, sSel1, sSel2, sSel3, .....,sSeln)
Say(nMainInfo, nSelCount, SelTab)
Èç¹ûÊÇ¿Í»§¶ËµÄÔò²»»áÏò·þÎñÆ÷¶Ë·¢ËÍÈÎºÎ²Ù×÷

  Say(100, 3, 10, 23,43)
  Say("Ñ¡ÔñÊ²Ã´£¿", 2, "ÊÇ/yes", "·ñ/no");
  Say("Ñ¡Ê²Ã´Ñ½", 2, SelTab);
*/
//**************************************************************************************************************************************************************
//												½çÃæ½Å±¾
//**************************************************************************************************************************************************************
INT LuaSelectUI(Lua_State* L)
{
	char* strMain = NULL;
	INT nMainInfo = 0;
	INT nDataType = 0;
	INT nOptionNum = 0;
	INT nReduceVal = 0;
	char* pImage = NULL;
	char* pContent = NULL;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	if (nParamNum > 3 && (pImage = strstr((char*)Lua_ValueToString(L, 1), "LINK:")))
	{
		nReduceVal = 2;
	}
	if (nParamNum > 2 && Lua_IsNumber(L, 2 + nReduceVal))
	{
		nOptionNum = (INT)Lua_ValueToNumber(L, 2 + nReduceVal);
		if (nReduceVal && nParamNum < 5) nOptionNum = 0;
	}
	else
	{
		//_ASSERT(0);
		//return 0;
		nOptionNum = 0;
	}

	if (Lua_IsNumber(L, 1 + nReduceVal))
	{
		nMainInfo = (INT)Lua_ValueToNumber(L, 1 + nReduceVal);
		nDataType = 1;
	}
	else if (Lua_IsString(L, 1 + nReduceVal)) 	//¼ì²éÖ÷ÐÅÏ¢ÊÇ×Ö·û´®»¹ÊÇ×Ö·û´®±êÊ¶ºÅ
	{
		strMain = (char*)Lua_ValueToString(L, 1 + nReduceVal);
		nDataType = 0;
	}
	else
		return 0;

	BOOL bStringTab = FALSE;//±êÊ¶´«½øÀ´µÄÑ¡ÏîÊý¾Ý´æ·ÅÔÚÒ»¸öÊý×éÖÐ£¬»¹ÊÇÐí¶à×Ö·û´®Àï

	if (Lua_IsString(L, 3 + nReduceVal))
		bStringTab = FALSE;
	else if (Lua_IsTable(L, 3 + nReduceVal))
	{
		bStringTab = TRUE;
	}
	else
	{
		if (nOptionNum > 0) return 0;
	}

	if (bStringTab == FALSE)
	{
		//»ñµÃÊµ¼Ê´«ÈëµÄÑ¡ÏîµÄ¸öÊý
		if (nOptionNum > nParamNum - 2 + nReduceVal) nOptionNum = nParamNum - 2 + nReduceVal;
	}

	if (nOptionNum > MAX_ANSWERNUM) nOptionNum = MAX_ANSWERNUM;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_SELECTDIALOG;
	UiInfo.m_bParam1 = nDataType;//Ö÷ÐÅÏ¢µÄÀàÐÍ£¬×Ö·û´®(0)»òÊý×Ö(1)
	UiInfo.m_bParam = (BOOL)nReduceVal;
	if (UiInfo.m_bParam)
	{
		g_StrCpyLen(UiInfo.m_szKey, pImage + 5, sizeof(UiInfo.m_szKey));
		UiInfo.m_nParam1 = (INT)Lua_ValueToNumber(L, 2);
	}
	else
	{
		memset(UiInfo.m_szKey, 0, sizeof(UiInfo.m_szKey));
		UiInfo.m_nParam1 = 0;
	}

	//Ö÷ÐÅÏ¢Îª×Ö·û´®
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;
	}
	else if (nDataType == 1) //Ö÷ÐÅÏ¢ÎªÊý×Ö±êÊ¶
	{
		*(INT*)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(INT);
		*pContent = 0;
	}

	INT nOptionCount = 0;
	for (INT i = 0; i < nOptionNum; i++)
	{
		char  pAnswer[100];
		pAnswer[0] = 0;

		if (bStringTab)
		{
			Lua_PushNumber(L, i + 1);
			Lua_RawGet(L, 3 + nReduceVal);
			char* pszString = (char*)Lua_ValueToString(L, Lua_GetTopIndex(L));
			if (pszString)
			{
				g_StrCpyLen(pAnswer, pszString, 100);
			}
		}
		else
		{
			char* pszString = (char*)Lua_ValueToString(L, i + 3 + nReduceVal);
			if (pszString)
				g_StrCpyLen(pAnswer, pszString, 100);
		}
		char* pFunName = strstr(pAnswer, "/");

		if (pFunName)
		{
			g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
			*pFunName = 0;
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
		else
		{
			//strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i], NORMAL_FUNCTION_NAME);
			//sprintf(pContent, "%s|%s", pContent, pAnswer);
			break;
		}
		nOptionCount++;
	}

	if (nParamNum > ((bStringTab ? 1 : nOptionCount) + 2 + nReduceVal + 1))
		UiInfo.m_nParam = (INT)Lua_ValueToNumber(L, (bStringTab ? 1 : nOptionCount) + 2 + nReduceVal + 1);
	else
		UiInfo.m_nParam = -1;
	UiInfo.m_bOptionNum = nOptionCount;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionCount;
	if (nDataType == 0)
		UiInfo.m_nBufferLen = strlen(pContent);
	else
		UiInfo.m_nBufferLen = strlen(pContent) + sizeof(INT);

#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif

	if (nOptionNum == 0)
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}
	Player[nPlayerIndex].m_SelUiScriptId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//AddGlobalNews(Newsstr)
INT LuaAddGlobalNews(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;
	INT nTimes = 1;
	if (Lua_GetTopIndex(L) > 1)
		nTimes = (INT)Lua_ValueToNumber(L, 2);

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(INT*)((char*)UiInfo.m_pContent + sizeof(INT)) = nTimes;
		UiInfo.m_nBufferLen = sizeof(INT) * 2;
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent));
		*(INT*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTimes;
		UiInfo.m_nBufferLen += sizeof(INT);
		UiInfo.m_bParam1 = 0;
	}

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}


INT LuaAddGlobalNews2(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;
	INT nTimes = 1;
	if (Lua_GetTopIndex(L) > 1)
		nTimes = (INT)Lua_ValueToNumber(L, 2);

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(INT*)((char*)UiInfo.m_pContent + sizeof(INT)) = nTimes;
		UiInfo.m_nBufferLen = sizeof(INT) * 2;
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent));
		*(INT*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTimes;
		UiInfo.m_nBufferLen += sizeof(INT);
		UiInfo.m_bParam1 = 0;
	}

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}
//AddLocalNews(Newsstr)
INT LuaAddLocalNews(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;
	INT nTimes = 1;
	if (Lua_GetTopIndex(L) > 1)
		nTimes = (INT)Lua_ValueToNumber(L, 2);

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(INT*)((char*)UiInfo.m_pContent + sizeof(INT)) = nTimes;
		UiInfo.m_nBufferLen = sizeof(INT) * 2;
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent));
		*(INT*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTimes;
		UiInfo.m_nBufferLen += sizeof(INT);
		UiInfo.m_bParam1 = 0;
	}

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddGlobalCountNews(strNew/newid, time)
INT LuaAddGlobalCountNews(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;

	INT nTime = (INT)Lua_ValueToNumber(L, 2);

	if (nTime <= 0)
		nTime = 1;

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(INT*)((char*)UiInfo.m_pContent + sizeof(INT)) = nTime;
		UiInfo.m_nBufferLen = sizeof(INT) * 2;
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent));
		*(INT*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(INT);
		UiInfo.m_bParam1 = 0;
	}

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

INT LuaAddGlobalCountNews2(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;

	INT nTime = (INT)Lua_ValueToNumber(L, 2);

	if (nTime <= 0)
		nTime = 1;

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(INT*)((char*)UiInfo.m_pContent + sizeof(INT)) = nTime;
		UiInfo.m_nBufferLen = sizeof(INT) * 2;
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent));
		*(INT*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(INT);
		UiInfo.m_bParam1 = 0;
	}

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}
//AddLocalCountNews(strNew/newid, time)
INT LuaAddLocalCountNews(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;

	INT nTime = (INT)Lua_ValueToNumber(L, 2);

	if (nTime <= 0)
		nTime = 1;

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(INT*)((char*)UiInfo.m_pContent + sizeof(INT)) = nTime;
		UiInfo.m_nBufferLen = sizeof(INT) * 2;
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent));
		*(INT*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(INT);
		UiInfo.m_bParam1 = 0;
	}

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddGlobalTimeNews(strNew/newid, year,month,day,hour,mins)
INT LuaAddGlobalTimeNews(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 6)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(INT) + sizeof(SYSTEMTIME);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}

	SYSTEMTIME* pSystemTime = (SYSTEMTIME*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));

	SYSTEMTIME LocalTime;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));

	LocalTime.wYear = (WORD)Lua_ValueToNumber(L, 2);
	LocalTime.wMonth = (WORD)Lua_ValueToNumber(L, 3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L, 5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L, 6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}


INT LuaAddGlobalTimeNews2(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 6)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO1;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(INT) + sizeof(SYSTEMTIME);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}

	SYSTEMTIME* pSystemTime = (SYSTEMTIME*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));

	SYSTEMTIME LocalTime;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));

	LocalTime.wYear = (WORD)Lua_ValueToNumber(L, 2);
	LocalTime.wMonth = (WORD)Lua_ValueToNumber(L, 3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L, 5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L, 6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastGlobal(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddLocalTimeNews(strNew/newid, year,month,day,hour,mins)
INT LuaAddLocalTimeNews(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 6)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(INT) + sizeof(SYSTEMTIME);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 512);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}

	SYSTEMTIME* pSystemTime = (SYSTEMTIME*)((char*)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));

	SYSTEMTIME LocalTime;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));

	LocalTime.wYear = (WORD)Lua_ValueToNumber(L, 2);
	LocalTime.wMonth = (WORD)Lua_ValueToNumber(L, 3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L, 5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L, 6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif

#ifndef _SERVER
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_NewProtocolProcess.BroadcastLocalServer(&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

//AddNote(str/strid)
INT LuaAddNote(Lua_State* L)
{
	//INT nMainInfo = 0;
	//INT nDataType = 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;


	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NOTEINFO;
	UiInfo.m_bParam1 = 0;//nDataType;//Ö÷ÐÅÏ¢µÄÀàÐÍ£¬×Ö·û´®(0)»òÊý×Ö(1)
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif

	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_bParam = FALSE;

	INT nParam2 = 0;

	if (Lua_IsNumber(L, 3))
	{
		UiInfo.m_bParam1 = 1;
	}
	else if (Lua_IsString(L, 3))
	{
		UiInfo.m_bParam1 = 0;
	}
	else
		return 0;

	if (nParamNum > 3)
	{
		nParam2 = (INT)Lua_ValueToNumber(L, 4);
	}

	sprintf(UiInfo.m_pContent, "%s|%s", (char*)Lua_ValueToString(L, 1),
		(char*)Lua_ValueToString(L, 3));
	UiInfo.m_nParam = (INT)Lua_ValueToNumber(L, 2);

	INT nLen = strlen(UiInfo.m_pContent);
	*(INT*)(UiInfo.m_pContent + nLen) = nParam2;
	UiInfo.m_nBufferLen = nLen + sizeof(INT) + 1;

	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

INT LuaAddMissionNote(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NOTEINFO;
	UiInfo.m_bParam1 = 0;//nDataType;//Ö÷ÐÅÏ¢µÄÀàÐÍ£¬×Ö·û´®(0)»òÊý×Ö(1)
#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif

	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_bParam = TRUE;

	INT nParam2 = 0;

	if (Lua_IsNumber(L, 2))
	{
		UiInfo.m_bParam1 = 1;
	}
	else if (Lua_IsString(L, 2))
	{
		UiInfo.m_bParam1 = 0;
	}
	else
		return 0;

	if (nParamNum > 2)
	{
		nParam2 = (INT)Lua_ValueToNumber(L, 3);
	}

	sprintf(UiInfo.m_pContent, "%s|%s", (char*)Lua_ValueToString(L, 1),
		(char*)Lua_ValueToString(L, 2));

	INT nLen = strlen(UiInfo.m_pContent);
	*(INT*)(UiInfo.m_pContent + nLen) = nParam2;
	UiInfo.m_nBufferLen = nLen + sizeof(INT) + 1;

	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}
/*
**
**¸ñÊ½1:Talk(SentenceNum, CallBack-Fun(½áÊøºóµÄ»Øµ÷º¯Êý), sTalk1, sTalk2, sTalk3, sTalk4,...sTalkN);
Talk(SentenceNum, CallBack-Fun(½áÊøºóµÄ»Øµ÷º¯Êý), nTalk1, nTalk2,nTalk3,nTalk4,...nTalkN);
**¸ñÊ½2:Talk(SentenceNum, CallBack-Fun, SentenceTab);
**Àý×Ó:Talk(3,"EndTalk", "Íæ¼Ò£ºÇëÎÊÏÖÔÚ¼¸µãÖÓÁË£¿", "¹ÍÔ±£ºÏÖÔÚ5µãÖÓÁË","Ì«Ð»Ð»ÄãÁË£¡");
**
*/

INT LuaTalkUI(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	INT nMainInfo = 0;
	INT nDataType = 0;
	INT nOptionNum = 0;
	char* pContent = NULL;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	if (Lua_IsNumber(L, 1))
	{
		nOptionNum = (INT)Lua_ValueToNumber(L, 1);
	}
	else
	{
		_ASSERT(0);
		return 0;
	}

	const char* pCallBackFun = Lua_ValueToString(L, 2);

	//¼ì²éÖ÷ÐÅÏ¢ÊÇ×Ö·û´®»¹ÊÇ×Ö·û´®±êÊ¶ºÅ

	if (Lua_IsNumber(L, 3))
	{
		nDataType = 1;
	}
	else if (Lua_IsString(L, 3))
	{
		nDataType = 0;
	}
	else
		return 0;


	//»ñµÃÊµ¼Ê´«ÈëµÄÑ¡ÏîµÄ¸öÊý
	if (nOptionNum > nParamNum - 2)
		nOptionNum = nParamNum - 2;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_TALKDIALOG;
	UiInfo.m_bParam1 = nDataType;//Ö÷ÐÅÏ¢µÄÀàÐÍ£¬×Ö·û´®(0)»òÊý×Ö(1)
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_bParam = FALSE;
	memset(UiInfo.m_szKey, 0, sizeof(UiInfo.m_szKey));
	UiInfo.m_nParam1 = 0;
	pContent = UiInfo.m_pContent;
	pContent[0] = 0;
	size_t nContentLen = 0;
	for (INT i = 0; i < nOptionNum; i++)
	{
		const char* pString = NULL;
		if (!nDataType)//StringInfo
		{
			pString = Lua_ValueToString(L, i + 3);
			if (nContentLen + strlen(pString) >= MAX_SCIRPTACTION_BUFFERNUM)
			{
				nOptionNum = i;
				UiInfo.m_bOptionNum = nOptionNum;
				break;
			}
			nContentLen += strlen(pString);
			sprintf(pContent, "%s%s|", pContent, pString);
		}
		else
		{
			INT j = (INT)Lua_ValueToNumber(L, i + 3);
			sprintf(pContent, "%s%d|", pContent, j);
		}
	}
	UiInfo.m_nBufferLen = strlen(pContent);

	if (!pCallBackFun || strlen(pCallBackFun) <= 0)
	{
		UiInfo.m_nParam = 0;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 0;
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		UiInfo.m_nParam = 1;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 1;
		g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[0], pCallBackFun, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}

#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	Player[nPlayerIndex].m_TalkUiScriptId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;

}

INT LuaSelUI(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	INT nMainInfo = 0;
	INT nDataType = 0;
	INT nOptionNum = 3;
	char* pContent = NULL;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4)
		return 0;

	const char* pCallBackFun = Lua_ValueToString(L, 1);

	//¼ì²éÖ÷ÐÅÏ¢ÊÇ×Ö·û´®»¹ÊÇ×Ö·û´®±êÊ¶ºÅ

	if (Lua_IsNumber(L, 2))
	{
		nDataType = 1;
	}
	else if (Lua_IsString(L, 2))
	{
		nDataType = 0;
	}
	else
		return 0;


	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_TALKDIALOG;
	UiInfo.m_bParam1 = nDataType;//Ö÷ÐÅÏ¢µÄÀàÐÍ£¬×Ö·û´®(0)»òÊý×Ö(1)
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_bParam = TRUE;
	memset(UiInfo.m_szKey, 0, sizeof(UiInfo.m_szKey));
	if (nParamNum > 5)
		UiInfo.m_nParam1 = (INT)Lua_ValueToNumber(L, 5);
	else
		UiInfo.m_nParam1 = -1;
	pContent = UiInfo.m_pContent;
	pContent[0] = 0;
	size_t nContentLen = 0;
	for (INT i = 0; i < nOptionNum; i++)
	{
		const char* pString = NULL;
		if (!nDataType)//StringInfo
		{
			pString = Lua_ValueToString(L, i + 2);
			if (nContentLen + strlen(pString) >= MAX_SCIRPTACTION_BUFFERNUM)
			{
				nOptionNum = i;
				UiInfo.m_bOptionNum = nOptionNum;
				break;
			}
			nContentLen += strlen(pString);
			sprintf(pContent, "%s%s|", pContent, pString);
		}
		else
		{
			INT j = (INT)Lua_ValueToNumber(L, i + 2);
			sprintf(pContent, "%s%d|", pContent, j);
		}
	}

	UiInfo.m_nBufferLen = strlen(pContent);

	if (!pCallBackFun || strlen(pCallBackFun) <= 0)
	{
		UiInfo.m_nParam = 0;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 0;
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		UiInfo.m_nParam = 1;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 2;
		g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[0], pCallBackFun, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}

#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	Player[nPlayerIndex].m_TalkUiScriptId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;

}

INT LuaIncludeLib(Lua_State* L)
{

	if (Lua_GetTopIndex(L) <= 0)
		return 0;

	if (Lua_IsString(L, 1))
	{
		const char* pFileDir = lua_tostring(L, 1);
		char lszCurrentDirectory[MAX_PATH];
	}
	return 0;
}

INT LuaIncludeFile(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;

	if (Lua_IsString(L, 1))
	{
		const char* pFileName = lua_tostring(L, 1);
		char lszCurrentDirectory[MAX_PATH];
		INT nLen = 0;
		if (pFileName[0] != '\\' && pFileName[0] != '/')
		{
			char* x = getcwd(lszCurrentDirectory, MAX_PATH);
			nLen = strlen(lszCurrentDirectory);
			if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
				lszCurrentDirectory[nLen - 1] = 0;
#ifdef WIN32
			g_StrCat(lszCurrentDirectory, "\\");
			g_StrCat(lszCurrentDirectory, (char*)pFileName);
#else
			g_StrCat(lszCurrentDirectory, "/");
			g_StrCat(lszCurrentDirectory, (char*)pFileName);
			for (INT i = 0; lszCurrentDirectory[i]; i++)
			{
				if (lszCurrentDirectory[i] == '\\')
					lszCurrentDirectory[i] = '/';
			}
#endif
		}
		else
		{
			g_GetRootPath(lszCurrentDirectory);
			nLen = strlen(lszCurrentDirectory);
			if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
				lszCurrentDirectory[nLen - 1] = 0;
#ifdef WIN32
			g_StrCat(lszCurrentDirectory, "\\");
			g_StrCat(lszCurrentDirectory, (char*)pFileName + 1);
#else
			g_StrCat(lszCurrentDirectory, "/");
			g_StrCat(lszCurrentDirectory, (char*)pFileName + 1);
			for (INT i = 0; lszCurrentDirectory[i]; i++)
			{
				if (lszCurrentDirectory[i] == '\\')
					lszCurrentDirectory[i] = '/';
			}
#endif
		}
		strlwr(lszCurrentDirectory + nLen);
		lua_dofile(L, lszCurrentDirectory);
		return 0;
	}
	else
		return 0;
}

//**************************************************************************************************************************************************************
//												ÈÎÎñ½Å±¾
//**************************************************************************************************************************************************************
INT LuaGetTaskValue(Lua_State* L)
{

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nValue = 0;
#ifdef _SERVER
		nValue = Player[nPlayerIndex].m_cTask.GetSaveVal((INT)Lua_ValueToNumber(L, 1));
#endif
		Lua_PushNumber(L, nValue);
	}
	else
		Lua_PushNil(L);

	return 1;
}

INT LuaGetTaskString(Lua_State* L)
{

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
#ifdef _SERVER
		Lua_PushString(L, Player[nPlayerIndex].m_cTask.GetSaveStr((INT)Lua_ValueToNumber(L, 1)));
#endif
	}
	else
		Lua_PushNil(L);

	return 1;
}

INT LuaSetTaskValue(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nValueIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nPlayerIndex <= 0) return 0;

	if (nValueIndex > MAX_TASK_SCRIPTFUNC)
		return 0;
#ifdef _SERVER
	Player[nPlayerIndex].m_cTask.SetSaveVal(nValueIndex, (char*)Lua_ValueToString(L, 2));
#endif
	return 0;
}

INT LuaSyncTaskValue(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nValueIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nPlayerIndex <= 0) return 0;
#ifdef _SERVER
	Player[nPlayerIndex].m_cTask.SyncTaskValue(nValueIndex);
#endif
	return 0;
}

#ifndef _SERVER
#define MAX_TEMPVALUENUM_INCLIENT 500
INT g_TempValue[MAX_TEMPVALUENUM_INCLIENT];
#endif

INT LuaGetTempTaskValue(Lua_State* L)
{
	INT nTempIndex = (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L));

#ifdef _SERVER
	if (nTempIndex >= MAX_TEMP_TASK)
	{
		Lua_PushNil(L);
		return 1;
	}
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNil(L);
		return 1;
	}

	INT nValue = Player[nPlayerIndex].m_cTask.GetClearVal(nTempIndex);
	Lua_PushNumber(L, nValue);
#else

	if (nTempIndex >= 0 && nTempIndex < MAX_TEMPVALUENUM_INCLIENT)
		Lua_PushNumber(L, g_TempValue[nTempIndex]);
	else
		Lua_PushNil(L);
#endif
	return 1;
}

INT LuaGetTempTaskString(Lua_State* L)
{
	INT nTempIndex = (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L));

#ifdef _SERVER
	if (nTempIndex >= MAX_TEMP_TASK)
	{
		Lua_PushNil(L);
		return 1;
	}
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNil(L);
		return 1;
	}

	Lua_PushString(L, (char*)Player[nPlayerIndex].m_cTask.GetClearStr(nTempIndex));
#else

	if (nTempIndex >= 0 && nTempIndex < MAX_TEMPVALUENUM_INCLIENT)
		Lua_PushNumber(L, g_TempValue[nTempIndex]);
	else
		Lua_PushNil(L);
#endif
	return 1;
}

INT LuaSetTempTaskValue(Lua_State* L)
{
	INT nTempIndex = (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L) - 1);
	char* szValue = (char*)Lua_ValueToString(L, Lua_GetTopIndex(L));
#ifdef _SERVER	
	Lua_GetGlobal(L, SCRIPT_PLAYERINDEX);
	INT nPlayerIndex = (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	if (nPlayerIndex <= 0) return 0;
	Player[nPlayerIndex].m_cTask.SetClearVal(nTempIndex, szValue);
#else
	g_TempValue[nTempIndex] = atoi(szValue);
#endif
	return 0;
}

#ifdef _SERVER
//---------------------------------½»Ò×¡¢ÂòÂô¡¢´ò¿ª´¢²ØÏä-----------------------
//Sale(id)
//------------------------------------------------------------------------------
INT LuaSale(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nShopId = (INT)Lua_ValueToNumber(L, 1);
		INT nShopMoneyUnit = moneyunit_money;
		//----ÔÚÒÔÏÂ¼ÓÈëÂòÂôµÄÊµ¼Ê´úÂë!
		if (Lua_GetTopIndex(L) > 2)
			nShopMoneyUnit = (INT)Lua_ValueToNumber(L, 2);
		BuySell.OpenSale(nPlayerIndex, nShopId - 1, nShopMoneyUnit);
	}
	return 0;
}

INT LuaNewSale(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4) return 0;

	INT nShopNum = (INT)Lua_ValueToNumber(L, 3);

	if (nShopNum > MAX_SUPERSHOP_SHOPTAB)
		nShopNum = MAX_SUPERSHOP_SHOPTAB;

	INT nShopId[MAX_SUPERSHOP_SHOPTAB];

	INT i = 0;
	for (i = 0; i < nShopNum; i++)
		nShopId[i] = (INT)Lua_ValueToNumber(L, 4 + i) - 1;

	for (i; i < MAX_SUPERSHOP_SHOPTAB; i++)
		nShopId[i] = -1;

	Player[nPlayerIndex].m_BuyInfo.m_nMoneyUnit = (INT)Lua_ValueToNumber(L, 2);

	BuySell.OpenSale(nPlayerIndex, (INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2), nShopNum, nShopId);
	return 0;
}

INT LuaOpenBox(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	SOME_BOX_SYNC command;
	command.ProtocolType = (BYTE)s2c_opensomebox;
	command.bEquipEx = FALSE;
	command.nBoxIndex = -1;
	if (Lua_GetTopIndex(L) > 1)
		command.nBoxIndex = (INT)Lua_ValueToNumber(L, 1) - 1;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &command, sizeof(SOME_BOX_SYNC));
	return 0;
}

//TamLTM kham nam xanh
//SetPItemID
INT LuaGetPOItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	BYTE nX, nY, nPos;
	INT nIndex = 0;
	nPos = (BYTE)Lua_ValueToNumber(L, 1);
	if (nPos < pos_equip /*|| nPos > pos_compound_item*/)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}
	nX = (BYTE)Lua_ValueToNumber(L, 2);

	//	g_DebugLog("Debug abc xyz %d + %d - %d", nPos, pos_equip, pos_builditem);
	if (nPos)
	{
		if (pos_equip == nPos)
		{
			nIndex = Player[nPlayerIndex].m_ItemList.GetEquipment(nX);
			//	g_DebugLog("Debug abc xyz");
		}

		if (pos_builditem == nPos) {
			nIndex = Player[nPlayerIndex].m_ItemList.GetBuildItem(nX);
			//	g_DebugLog("Debug abc xyz 2");
		}
	} // */

	  /*	switch (nPos)
	  {
	  case pos_equip:
	  nIndex = Player[nPlayerIndex].m_ItemList.GetEquipment(nX);
	  g_DebugLog("Debug abc xyz");
	  break;
	  case pos_builditem:
	  nIndex = Player[nPlayerIndex].m_ItemList.GetBuildItem(nX);
	  break;
	  /*	case pos_compound_item:
	  nIndex = Player[nPlayerIndex].m_ItemList.GetComPoundItem(nX);
	  break; */
	  /*	default:
	  //	g_DebugLog("Debug defaulf");
	  break;
	  } // */

	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaSetPItemID(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	BOOL bExist = FALSE;
	INT nIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	BYTE nX, nY, nPos;
	nPos = (BYTE)Lua_ValueToNumber(L, 2);
	if (nPos < pos_equip /*|| nPos > pos_compound_item*/)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}
	nX = (BYTE)Lua_ValueToNumber(L, 3);

	if (nPos == 12)
	{
		if (pos_equip == 2) {
			nIndex = Player[nPlayerIndex].m_ItemList.GetEquipment(nX);
			//	g_DebugLog("Debug abc xyz");
		}

		if (pos_builditem == 16) {
			bExist = (BOOL)Player[nPlayerIndex].m_ItemList.GetBuildItem(nX);
			//	g_DebugLog("Debug abc xyz 2");
		}
	} //*/

	  /*	switch (nPos)
	  {
	  case pos_equip:
	  bExist = (BOOL)Player[nPlayerIndex].m_ItemList.GetEquipment(nX);
	  break;
	  case pos_builditem:
	  bExist = (BOOL)Player[nPlayerIndex].m_ItemList.GetBuildItem(nX);
	  break;
	  /*	case pos_compound_item:
	  bExist = (BOOL)Player[nPlayerIndex].m_ItemList.GetComPoundItem(nX);
	  break; */
	  /*	default:
	  break;
	  } // */

	if (!bExist)
		Player[nPlayerIndex].m_ItemList.Add(nIndex, nPos, nX, 0, false);
	else
	{
		INT	nIdx = Player[nPlayerIndex].m_ItemList.Hand();
		if (nIdx)
		{
			Player[nPlayerIndex].m_ItemList.Remove(nIdx);

			KMapPos sMapPos;
			KObjItemInfo	sInfo;
			char	szNameTemp[OBJ_NAME_LENGHT];
			Player[nPlayerIndex].GetAboutPos(&sMapPos);

			sInfo.m_nItemID = nIdx;
			sInfo.m_nItemWidth = Item[nIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			if (Item[nIdx].GetStackNum() > 1)
			{
				sprintf(szNameTemp, "%s x %d", Item[nIdx].GetName(), Item[nIdx].GetStackNum());
				strcpy(sInfo.m_szName, szNameTemp);
			}
			else
				strcpy(sInfo.m_szName, Item[nIdx].GetName());
			sInfo.m_nColorID = Item[nIdx].GetColorItem();
			sInfo.m_nGenre = Item[nIdx].GetGenre();
			sInfo.m_nDetailType = Item[nIdx].GetDetailType();
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
			sInfo.m_dwNpcId = 0;

			INT nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), sMapPos, sInfo);
			if (nObj >= 0)
			{
				if (Item[nIdx].GetGenre() == item_task ||
					Item[nIdx].GetGenre() == item_mine)
					Object[nObj].SetEntireBelong(nPlayerIndex);
				else
					Object[nObj].SetItemBelong(nPlayerIndex);
			}
		}
		Player[nPlayerIndex].m_ItemList.Add(nIndex, pos_hand, 0, 0);
	}
	Lua_PushNumber(L, 1);
	return 1;
}


INT LuaOpenTrembleItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);

	S2C_OTHER_BOX NetCommand;
	NetCommand.ProtocolType = s2c_otherbox;
	NetCommand.nValue = 0;
	if (g_pServer && Player[nPlayerIndex].m_nNetConnectIdx != -1)
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &NetCommand, sizeof(S2C_OTHER_BOX));

	return 0;
}
//end code kham nam xanh


INT LuaOpenEquipEx(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	SOME_BOX_SYNC command;
	command.ProtocolType = (BYTE)s2c_opensomebox;
	command.bEquipEx = TRUE;
	command.nBoxIndex = 0;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &command, sizeof(SOME_BOX_SYNC));
	return 0;
}

//---------------------------------Ê±¼äÈÎÎñ-------------------------------------
//SetTimer(Time, TimerTaskId)
INT LuaSetTimer(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2) return 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	Player[nPlayerIndex].SetTimer((DWORD)(INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2));
	return 0;
}

INT LuaStopTimer(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	Player[nPlayerIndex].CloseTimer();
	return 0;
}

INT LuaGetCurTimerId(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nTimerId = Player[nPlayerIndex].m_TimerTask.GetTaskId();
	Lua_PushNumber(L, nTimerId);
	return 1;
}

INT LuaGetRestTime(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNil(L);
		return 1;
	}
	INT nRestTime = Player[nPlayerIndex].m_TimerTask.GetRestTime();//m_dwTimeTaskTime - g_SubWorldSet.GetGameTime();

	if (nRestTime > 0)
		Lua_PushNumber(L, nRestTime);
	else
		Lua_PushNumber(L, 0);

	return 1;
}

INT LuaGetMissionRestTime(Lua_State* L)
{
	INT RestTime = 0;
	if (Lua_GetTopIndex(L) >= 2)
	{
		INT nSubWorldIndex = GetSubWorldIndex(L);
		if (nSubWorldIndex >= 0)
		{
			INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
			INT nTimerId = (INT)Lua_ValueToNumber(L, 2);

			if (nMissionId < 0 || nTimerId < 0)
				goto lab_getmissionresttime;

			KMission Mission;
			Mission.SetMissionId(nMissionId);
			KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
			if (pMission)
			{
				RestTime = (INT)pMission->GetTimerRestTimer(nTimerId);
			}
		}
	}

lab_getmissionresttime:
	Lua_PushNumber(L, RestTime);
	return 1;
}



//**************************************************************************************************************************************************************
//												×é¶Ó½Å±¾
//**************************************************************************************************************************************************************
INT LuaIsLeader(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag && Player[nPlayerIndex].m_cTeam.m_nFigure == TEAM_CAPTAIN)
			Lua_PushNumber(L, 1);
		else
			Lua_PushNumber(L, 0);
	}
	else
		Lua_PushNumber(L, 0);

	return 1;
}

INT LuaGetTeamId(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag)
			Lua_PushNumber(L, Player[nPlayerIndex].m_cTeam.m_nID);
		else
			Lua_PushNil(L);
	}
	else
		Lua_PushNil(L);

	return 1;
}

INT LuaTeamDoScript(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	char* nValueIndex = (char*)Lua_ValueToString(L, 1);
	char* sFunc = (char*)Lua_ValueToString(L, 2);
	INT	Value1 = (INT)Lua_ValueToNumber(L, 3);
	INT	Value2 = (INT)Lua_ValueToNumber(L, 4);
	INT	Value3 = (INT)Lua_ValueToNumber(L, 5);
	DWORD ScriptID = g_FileName2Id(nValueIndex);
	INT Count = 0;
	if (nPlayerIndex < 0)
		return 0;
	if (Player[nPlayerIndex].m_cTeam.m_nID >= 0)
	{
		INT nTeamLeaderId = g_Team[Player[nPlayerIndex].m_cTeam.m_nID].m_nCaptain;
		if (nTeamLeaderId)
		{
			Player[nTeamLeaderId].ExecuteScript3Param(ScriptID, sFunc, 0, Value1, Value2, Value3);
			Count = Count + 1;
		}
		else
			return 0;
		for (INT i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			INT nMemberId = g_Team[Player[nPlayerIndex].m_cTeam.m_nID].m_nMember[i];
			if (nMemberId)
			{
				Player[nMemberId].ExecuteScript3Param(ScriptID, sFunc, 0, Value1, Value2, Value3);
				Count = Count + 1;
				Sleep(5);
			}
		}
	}
	Lua_PushNumber(L, Count);
	return Count;
}

INT LuaGetTeamSize(Lua_State* L)
{
	INT nTeamSize = 0;
	INT nTeamId = -1;
	if (Lua_GetTopIndex(L) >= 1)
	{
		nTeamId = Lua_ValueToNumber(L, 1);
	}
	else
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			if (Player[nPlayerIndex].m_cTeam.m_nFlag)
				nTeamId = Player[nPlayerIndex].m_cTeam.m_nID;
			else
				nTeamId = -1;
		}
	}

	if (nTeamId < 0)
		nTeamSize = 0;
	else
		nTeamSize = g_Team[nTeamId].m_nMemNum + 1;
	Lua_PushNumber(L, nTeamSize);
	return 1;
}

INT LuaGetTeamMem(Lua_State* L)
{
	INT nTeamId = -1;
	INT nMemId = 0;
	INT nResult = 0;
	if (Lua_GetTopIndex(L) >= 2)
	{
		nTeamId = Lua_ValueToNumber(L, 1);
		nMemId = Lua_ValueToNumber(L, 2);
	}
	else
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			if (Player[nPlayerIndex].m_cTeam.m_nFlag)
				nTeamId = Player[nPlayerIndex].m_cTeam.m_nID;
			else
				nTeamId = -1;
			nMemId = Lua_ValueToNumber(L, 1);
		}
	}

	if (nTeamId >= 0)
		nResult = nMemId == 0 ? g_Team[nTeamId].m_nCaptain : g_Team[nTeamId].m_nMember[nMemId - 1];

	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaLeaveTeam(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nFlag)
		{
			PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
			sLeaveTeam.ProtocolType = c2s_teamapplyleave;
			sLeaveTeam.bMySelf = TRUE;
			Player[nPlayerIndex].LeaveTeam((BYTE*)&sLeaveTeam);
		}
	}
	return 0;
}

INT LuaSetCreateTeamOption(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Player[nPlayerIndex].m_cTeam.SetCreatTeamFlag(nPlayerIndex, (INT)Lua_ValueToNumber(L, 1) > 0);
	}
	return 0;
}

INT LuaSetFreezeTeamOption(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Player[nPlayerIndex].m_cTeam.SetFreezeTeamFlag(nPlayerIndex, (INT)Lua_ValueToNumber(L, 1) > 0);
	}
	return 0;
}
//**************************************************************************************************************************************************************
//												ÁÄÌìÏûÏ¢½Å±¾
//**************************************************************************************************************************************************************
extern TLua_Funcs GameScriptFuns[];
extern TLua_Funcs WorldScriptFuns[];

INT g_GetGameScriptFunNum();
INT g_GetWorldScriptFunNum();
//**************************************************************************************************************************************************************
//												ÁÄÌìÏûÏ¢½Å±¾
//**************************************************************************************************************************************************************

INT	LuaMsgToPlayer(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		const char* szMsg = (char*)Lua_ValueToString(L, 1);
		if (szMsg)
		{
			INT nChannelID = -1;
			if (nParamNum > 1)
				nChannelID = (INT)Lua_ValueToNumber(L, 2);
			KPlayerChat::SendSystemInfo(1, nPlayerIndex, MESSAGE_BROADCAST_ANNOUCE_HEAD, (char*)szMsg, strlen(szMsg), nChannelID);
		}
	}

	return 0;
}

INT LuaMsgToTeam(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Player[nPlayerIndex].m_cTeam.m_nID >= 0)
		{
			const char* szMsg = Lua_ValueToString(L, 1);
			Player[nPlayerIndex].SendTeamMessage(Player[nPlayerIndex].m_cTeam.m_nID, szMsg);
		}
	}
	return 0;
}

INT LuaMsgToSubWorld(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);

	const char* szMsg = (char*)Lua_ValueToString(L, 1);
	if (szMsg)
	{
		INT nChannelID = -1;
		if (nParamNum > 1)
			nChannelID = (INT)Lua_ValueToNumber(L, 2);
		KPlayerChat::SendSystemInfo(0, 0, MESSAGE_BROADCAST_ANNOUCE_HEAD, (char*)szMsg, strlen(szMsg), nChannelID);
	}

	return 0;
}

INT LuaMsgToTong(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (!Player[nPlayerIndex].m_cTong.m_nFlag)
		return 0;

	STONG_MESSAGE_INFO_COMMAND	TongMsg;
	TongMsg.ProtocolID = enumC2S_MESSAGE_TO_TONG;
	TongMsg.ProtocolFamily = pf_tong;
	TongMsg.dwParam = Player[nPlayerIndex].m_cTong.m_dwTongNameID;
	strcpy(TongMsg.szName, Player[nPlayerIndex].Name);
	strcpy(TongMsg.szMsg, (char*)Lua_ValueToString(L, 1));
	if (g_pTongClient)
		g_pTongClient->SendPackToServer((const void*)&TongMsg, sizeof(STONG_MESSAGE_INFO_COMMAND));

	return 0;
}

INT LuaMsgToFaction(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_cFaction.m_nCurFaction < 0)
		return 0;

	STONG_MESSAGE_INFO_COMMAND	FactionMsg;
	FactionMsg.ProtocolID = enumC2S_MESSAGE_TO_FACTION;
	FactionMsg.ProtocolFamily = pf_tong;
	FactionMsg.dwParam = Player[nPlayerIndex].m_cFaction.m_nCurFaction;
	strcpy(FactionMsg.szName, Player[nPlayerIndex].Name);
	strcpy(FactionMsg.szMsg, (char*)Lua_ValueToString(L, 1));
	if (g_pTongClient)
		g_pTongClient->SendPackToServer((const void*)&FactionMsg, sizeof(STONG_MESSAGE_INFO_COMMAND));

	return 0;
}

INT LuaMsgToChatRoom(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (!Player[nPlayerIndex].m_cRoom.m_nFlag)
		return 0;

	STONG_MESSAGE_INFO_COMMAND	RoomMsg;
	RoomMsg.ProtocolID = enumC2S_MESSAGE_TO_CHATROOM;
	RoomMsg.ProtocolFamily = pf_tong;
	RoomMsg.dwParam = Player[nPlayerIndex].m_cRoom.m_nID;
	strcpy(RoomMsg.szName, Player[nPlayerIndex].Name);
	strcpy(RoomMsg.szMsg, (char*)Lua_ValueToString(L, 1));
	if (g_pTongClient)
		g_pTongClient->SendPackToServer((const void*)&RoomMsg, sizeof(STONG_MESSAGE_INFO_COMMAND));

	return 0;
}

INT LuaGetSubWorldConnectionByIP(Lua_State* L)
{
	//INT nPlayerIndex = GetPlayerIndex(L);
	//if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nSubWorldIndex = (INT)Lua_ValueToNumber(L, 1);
	char* szIP = (char*)Lua_ValueToString(L, 2);
	INT nCount = 0;

	if (nSubWorldIndex >= 0 && nSubWorldIndex < MAX_SUBWORLD)
	{
		INT nIndex = PlayerSet.GetFirstPlayer();
		while (nIndex > 0)
		{
			if (Npc[Player[nIndex].m_nIndex].m_SubWorldIndex == nSubWorldIndex)
			{
				if (Player[nIndex].m_szIpAddress[0] && szIP[0])
					if (strcmp(Player[nIndex].m_szIpAddress, szIP) == 0)
					{
						nCount++;
					}
			}
			nIndex = PlayerSet.GetNextPlayer();
		}
	}

	Lua_PushNumber(L, nCount);
	return 1;
}

INT LuaMsgToAroundRegion(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);

	INT nSubWorldIndex = g_SubWorldSet.SearchWorld((INT)Lua_ValueToNumber(L, 1));

	if (nSubWorldIndex >= 0 && nSubWorldIndex < MAX_SUBWORLD)
	{
		const char* szMsg = (char*)Lua_ValueToString(L, 2);
		INT nChannelID = -1;
		if (nParamNum > 3)
			nChannelID = (INT)Lua_ValueToNumber(L, 3);
		if (szMsg)
		{
			INT nIndex = PlayerSet.GetFirstPlayer();
			while (nIndex > 0)
			{
				if (Npc[Player[nIndex].m_nIndex].m_SubWorldIndex == nSubWorldIndex)
					KPlayerChat::SendSystemInfo(1, nIndex, MESSAGE_BROADCAST_ANNOUCE_HEAD, (char*)szMsg, strlen(szMsg), nChannelID);

				nIndex = PlayerSet.GetNextPlayer();
			}
		}
	}
	return 0;
}

//**************************************************************************************************************************************************************
//												Ö÷½Ç½Å±¾
//**************************************************************************************************************************************************************

//**************************************************************************************************************************************************************
//												Ö÷½Ç½Å±¾
//**************************************************************************************************************************************************************

/*¹¦ÄÜ£ºÈÃÍæ¼Ò½øÈëÐÂµÄÒ»¸öÓÎÏ·ÊÀ½ç
nPlayerIndex:Ö÷½ÇµÄIndex
nSubWorldIndex:ÓÎÏ·ÊÀ½çid
nPosX:
nPosY:
*/
//NewWorld(WorldId, X,Y)
INT LuaEnterNewWorld(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	INT nResult = 0;
	if (Lua_GetTopIndex(L) >= 3)
	{
		DWORD dwWorldId = (DWORD)Lua_ValueToNumber(L, 1);
		nResult = Npc[Player[nPlayerIndex].m_nIndex].ChangeWorld(dwWorldId, (INT)Lua_ValueToNumber(L, 2) * 32, (INT)Lua_ValueToNumber(L, 3) * 32);
	}
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaNpcEnterNewWorld(Lua_State* L)
{
	INT nParamCount = 0;
	if ((nParamCount = Lua_GetTopIndex(L)) < 3) return 0;
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0) return 0;

	INT nResult = 0;
	if (Lua_GetTopIndex(L) > 3)
	{
		DWORD dwWorldId = (DWORD)Lua_ValueToNumber(L, 2);
		nResult = Npc[nNpcIndex].ChangeWorld(dwWorldId, (INT)Lua_ValueToNumber(L, 3) * 32, (INT)Lua_ValueToNumber(L, 4) * 32);
	}
	Lua_PushNumber(L, nResult);
	return 1;
}

//SetPos(X,Y)
INT LuaSetPos(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount != 2) return 0;
	INT nPlayerIndex = GetPlayerIndex(L);

	INT nX = (INT)Lua_ValueToNumber(L, 1);
	INT nY = (INT)Lua_ValueToNumber(L, 2);

	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetPos(nX * 32, nY * 32);
	}
	return 0;
}

INT LuaGetPos(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex > 0)
	{
		INT nPosX = 0;
		INT nPosY = 0;
		Npc[Player[nPlayerIndex].m_nIndex].GetMpsPos(&nPosX, &nPosY);
		Lua_PushNumber(L, nPosX);
		Lua_PushNumber(L, nPosY);
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex);
	}
	else
		return 0;
	return 3;
}

//W,X,Y = GetWorldPos()
INT LuaGetNewWorldPos(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex > 0)
	{
		INT nPosX = 0;
		INT nPosY = 0;
		Npc[Player[nPlayerIndex].m_nIndex].GetMpsPos(&nPosX, &nPosY);

		INT nSubWorldIndex = Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex;
		INT nSubWorldID = 0;
		if (nSubWorldIndex >= 0 && nSubWorldIndex < MAX_SUBWORLD)
		{
			nSubWorldID = SubWorld[nSubWorldIndex].m_SubWorldID;
		}

		Lua_PushNumber(L, nSubWorldID);
		Lua_PushNumber(L, ((INT)(nPosX / 32)));
		Lua_PushNumber(L, ((INT)(nPosY / 32)));
	}
	else
	{
		Lua_PushNil(L);
		return 1;
	}
	return 3;
}


INT LuaGetNewWorldName(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
	{
		INT nPlayerIndex = GetPlayerIndex(L);

		if (nPlayerIndex > 0)
		{
			INT nSubWorldID = SubWorld[Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex].m_SubWorldID;
			Lua_PushString(L, g_SubWorldSet.m_sMapListInfo[nSubWorldID].szName);
			return 1;
		}
	}
	else
	{
		INT nSubWorldID = (INT)Lua_ValueToNumber(L, 1);
		Lua_PushString(L, g_SubWorldSet.m_sMapListInfo[nSubWorldID].szName);
		return 1;
	}
	return 0;
}

INT LuaGetNewWorldKind(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
	{
		INT nPlayerIndex = GetPlayerIndex(L);

		if (nPlayerIndex > 0)
		{
			INT nSubWorldID = SubWorld[Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex].m_SubWorldID;
			Lua_PushNumber(L, g_SubWorldSet.m_sMapListInfo[nSubWorldID].nKind);
			return 1;
		}
	}
	else
	{
		INT nSubWorldID = (INT)Lua_ValueToNumber(L, 1);
		Lua_PushNumber(L, g_SubWorldSet.m_sMapListInfo[nSubWorldID].nKind);
		return 1;
	}
	return 0;
}

INT LuaGetNpcPos(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex > 0)
	{
		INT nPosX = 0;
		INT nPosY = 0;
		Npc[nNpcIndex].GetMpsPos(&nPosX, &nPosY);

		INT nSubWorldIndex = Npc[nNpcIndex].m_SubWorldIndex;
		INT nSubWorldID = 0;
		if (nSubWorldIndex >= 0 && nSubWorldIndex < MAX_SUBWORLD)
		{
			nSubWorldID = SubWorld[nSubWorldIndex].m_SubWorldID;
		}

		Lua_PushNumber(L, nSubWorldID);
		Lua_PushNumber(L, ((INT)(nPosX / 32)));
		Lua_PushNumber(L, ((INT)(nPosY / 32)));
		return 3;
	}
	return 0;
}

INT LuaDropMoney(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nIndex = 0;
	INT nSubWorldIndex = 0;
	INT nX, nY;
	INT nMoney = 0;

	if (nParamNum > 1)
	{
		nIndex = Player[nPlayerIndex].m_nIndex;
		nMoney = (INT)Lua_ValueToNumber(L, 1);
	}
	if (nParamNum > 2)
	{
		nIndex = (INT)Lua_ValueToNumber(L, 1);
		nMoney = (INT)Lua_ValueToNumber(L, 2);
	}

	if (nIndex <= 0)
	{
		nIndex = Player[nPlayerIndex].m_nIndex;
	}
	if (nMoney <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Npc[nIndex].GetMpsPos(&nX, &nY);
	nSubWorldIndex = Npc[nIndex].GetSubWorldIndex();

	if (nSubWorldIndex < 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	POINT	ptLocal;
	KMapPos	Pos;
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);

	Pos.nSubWorld = nSubWorldIndex;
	SubWorld[nSubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y,
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY,
		&Pos.nOffX, &Pos.nOffY);
	INT nObj = ObjSet.AddMoneyObj(Pos, nMoney * g_MoneyRate);
	if (nObj > 0)
	{
		Object[nObj].SetItemBelong(nPlayerIndex);
		Lua_PushNumber(L, nObj);
		return 1;
	}
	return 0;
}

INT LuaDropItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nIndex = 0;
	INT nSubWorldIndex = 0;
	INT nX, nY;
	INT nIdx = 0;

	if (nParamNum > 1)
	{
		nIdx = (INT)Lua_ValueToNumber(L, 1);
	}
	if (nParamNum > 2)
	{
		nIndex = (INT)Lua_ValueToNumber(L, 1);
		nIdx = (INT)Lua_ValueToNumber(L, 2);
	}

	if (nIndex <= 0)
		nIndex = Player[nPlayerIndex].m_nIndex;

	if (nIdx <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Npc[nIndex].GetMpsPos(&nX, &nY);
	nSubWorldIndex = Npc[nIndex].GetSubWorldIndex();

	if (nSubWorldIndex < 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	if (nIdx)
	{
		POINT	ptLocal;
		KMapPos	Pos;
		KObjItemInfo	sInfo;
		ptLocal.x = nX;
		ptLocal.y = nY;
		SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);

		Pos.nSubWorld = nSubWorldIndex;
		SubWorld[nSubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, &Pos.nRegion, &Pos.nMapX, &Pos.nMapY, &Pos.nOffX, &Pos.nOffY);

		sInfo.m_nItemID = nIdx;
		sInfo.m_nItemWidth = Item[nIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		strcpy(sInfo.m_szName, Item[nIdx].GetName());
		sInfo.m_nColorID = Item[nIdx].GetQuality();
		sInfo.m_nGenre = Item[nIdx].GetGenre();
		sInfo.m_nDetailType = Item[nIdx].GetDetailType();
		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;
		sInfo.m_bOverLook = 0;
		INT nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), Pos, sInfo);
		if (nObj > 0)
		{
			if (Item[nIndex].LockPick())
			{
				Object[nObj].SetEntireBelong(nPlayerIndex);
			}
			else
			{
				Object[nObj].SetItemBelong(nPlayerIndex);
			}
			Lua_PushNumber(L, nObj);
			return 1;
		}
	}
	return 0;
}

INT LuaDropMapItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4)
		return 0;

	INT nIndex = 0;
	INT nSubWorldIndex = 0;
	INT nX, nY;
	INT nIdx = 0;

	if (nParamNum > 3)
	{
		nSubWorldIndex = g_SubWorldSet.SearchWorld((INT)Lua_ValueToNumber(L, 1));
		nX = (INT)Lua_ValueToNumber(L, 2);
		nY = (INT)Lua_ValueToNumber(L, 3);
		nIdx = (INT)Lua_ValueToNumber(L, 4);
	}

	if (nIdx <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	if (nSubWorldIndex < 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	if (nIdx)
	{
		POINT	ptLocal;
		KMapPos	Pos;
		KObjItemInfo	sInfo;
		ptLocal.x = nX;
		ptLocal.y = nY;
		SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);

		Pos.nSubWorld = nSubWorldIndex;
		SubWorld[nSubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y,
			&Pos.nRegion, &Pos.nMapX, &Pos.nMapY,
			&Pos.nOffX, &Pos.nOffY);
		sInfo.m_nItemID = nIdx;
		sInfo.m_nItemWidth = Item[nIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		strcpy(sInfo.m_szName, Item[nIdx].GetName());
		sInfo.m_nColorID = Item[nIdx].GetQuality();
		sInfo.m_nGenre = Item[nIdx].GetGenre();
		sInfo.m_nDetailType = Item[nIdx].GetDetailType();
		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;
		sInfo.m_bOverLook = 0;
		INT nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), Pos, sInfo);
		if (nObj > 0)
		{
			Object[nObj].SetItemBelong(-1);
			Lua_PushNumber(L, nObj);
			return 1;
		}
	}
	return 0;
}

KTabFile g_EventItemTab;
KTabFile g_QuestItemTab;
KTabFile g_TownPortalTab;
KTabFile g_MagicScriptTab;

INT LuaAddItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
		return 0;

	INT nItemNature = 0;
	INT nItemClass = 0;
	INT nDetailType = 0;
	INT nParticularType = 0;
	INT nLevel = 0;
	INT nSeries = series_num;
	INT nLuck = 0;
	INT nItemLevel[MAX_ITEM_MAGICLEVEL];
	ZeroMemory(nItemLevel, sizeof(nItemLevel));

	nItemNature = (INT)Lua_ValueToNumber(L, 1);
	nItemClass = (INT)Lua_ValueToNumber(L, 2);
	nDetailType = (INT)Lua_ValueToNumber(L, 3);
	nParticularType = (INT)Lua_ValueToNumber(L, 4);
	nLevel = (INT)Lua_ValueToNumber(L, 5);
	nSeries = (INT)Lua_ValueToNumber(L, 6);

	if (nParamNum > 6)
		nLuck = (INT)Lua_ValueToNumber(L, 7);

	nItemLevel[0] = 0;
	if (nParamNum > 7)
		nItemLevel[0] = (INT)Lua_ValueToNumber(L, 8);
	if (nParamNum > 12)
	{
		nItemLevel[1] = (INT)Lua_ValueToNumber(L, 9);
		nItemLevel[2] = (INT)Lua_ValueToNumber(L, 10);
		nItemLevel[3] = (INT)Lua_ValueToNumber(L, 11);
		nItemLevel[4] = (INT)Lua_ValueToNumber(L, 12);
		nItemLevel[5] = (INT)Lua_ValueToNumber(L, 13);
	}
	else
	{
		for (INT i = 1; i < MAX_ITEM_NORMAL_MAGICATTRIB; i++)
			nItemLevel[i] = nItemLevel[0];
	}

	//for (INT i = MAX_ITEM_NORMAL_MAGICATTRIB; i < MAX_ITEM_NORMAL_MAGICATTRIB; i++)
	//	nItemLevel[i] = 0;

	INT nIndex = 0;
	switch (nItemNature)
	{
	case NATURE_GOLD:
	case NATURE_PLATINA:
		nIndex = ItemSet.Add(nItemNature, 0, 0, 0, MAKELONG(nLevel, nLuck), nDetailType, 0, nItemLevel[0] == 0 ? NULL : nItemLevel, g_SubWorldSet.GetGameVersion());
		break;
	default:
		nIndex = ItemSet.Add(nItemNature, nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType, nItemLevel, g_SubWorldSet.GetGameVersion());
		break;
	}

	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaAddItemNew(Lua_State* L) {
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
		return 0;

	INT nItemNature = NATURE_NORMAL;
	INT nItemClass = 0;
	INT nDetailType = 0;
	INT nParticularType = 0;
	INT nLevel = 0;
	INT nSeries = series_num;
	INT nLuck = 0;
	INT nItemLevel[MAX_ITEM_MAGICLEVEL];
	ZeroMemory(nItemLevel, sizeof(nItemLevel));

	//nItemNature = (INT)Lua_ValueToNumber(L, 1);
	nItemClass = (INT)Lua_ValueToNumber(L, 1);
	nDetailType = (INT)Lua_ValueToNumber(L, 2);
	nParticularType = (INT)Lua_ValueToNumber(L, 3);
	nLevel = (INT)Lua_ValueToNumber(L, 4);
	nSeries = (INT)Lua_ValueToNumber(L, 5);
	nLuck = (INT)Lua_ValueToNumber(L, 6);

	nItemLevel[0] = 0;
	if (nParamNum > 6)
		nItemLevel[0] = (INT)Lua_ValueToNumber(L, 7);
	if (nParamNum > 12)
	{
		nItemLevel[1] = (INT)Lua_ValueToNumber(L, 8);
		nItemLevel[2] = (INT)Lua_ValueToNumber(L, 9);
		nItemLevel[3] = (INT)Lua_ValueToNumber(L, 10);
		nItemLevel[4] = (INT)Lua_ValueToNumber(L, 11);
		nItemLevel[5] = (INT)Lua_ValueToNumber(L, 12);
	}
	else
	{
		for (INT i = 1; i < MAX_ITEM_NORMAL_MAGICATTRIB; i++)
			nItemLevel[i] = nItemLevel[0];
	}

	//for (INT i = MAX_ITEM_NORMAL_MAGICATTRIB; i < MAX_ITEM_NORMAL_MAGICATTRIB; i++)
	//	nItemLevel[i] = 0;

	INT nIndex = 0;

	if (nItemClass == item_magicscript || nItemClass == item_task)
		nIndex = ItemSet.Add(nItemNature, nItemClass, series_num, 0, 0, nDetailType, nParticularType, 0, g_SubWorldSet.GetGameVersion());
	else
		nIndex = ItemSet.Add(nItemNature, nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType, nItemLevel, g_SubWorldSet.GetGameVersion());

	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	//Lua_PushNumber(L, nIndex);
	INT	P = pos_equiproom, x = 0, y = 0;
	POINT	pPos;

	if (P < pos_hand || P > pos_num)
		return 0;

	if (nParamNum == 5 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].CheckRoom(x, y, Item[nIndex].GetWidth(), Item[nIndex].GetHeight()))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, y, true);
	else if (nParamNum == 4 && Player[nPlayerIndex].m_ItemList.PositionToIndex(P, x) <= 0)
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, 0, true);
	else if (nParamNum == 3 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].FindRoom(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &pPos))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, pPos.x, pPos.y, true);
	else
		Player[nPlayerIndex].m_ItemList.InsertEquipment(nIndex, true);

	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaAddGoldItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nItemNature = NATURE_GOLD;
	INT nItemClass = 0;
	INT nDetailType = 0;
	INT nParticularType = 0;
	INT nLevel = 0;
	INT nSeries = series_num;
	INT nLuck = _GetRandomNumber(10, 100);
	INT nItemLevel[MAX_ITEM_MAGICLEVEL];
	ZeroMemory(nItemLevel, sizeof(nItemLevel));

	nDetailType = (INT)Lua_ValueToNumber(L, 1);

	if (nDetailType >= 1)
		nDetailType -= 1;

	nItemLevel[0] = 0;

	for (INT i = 1; i < MAX_ITEM_MAGICLEVEL; i++)
		nItemLevel[i] = nItemLevel[0];

	INT nIndex = 0;
	switch (nItemNature)
	{
	case NATURE_GOLD:
	case NATURE_PLATINA:
		nIndex = ItemSet.Add(nItemNature, 0, 0, 0, MAKELONG(nLevel, nLuck), nDetailType, 0, nItemLevel[0] == 0 ? NULL : nItemLevel, g_SubWorldSet.GetGameVersion());
		break;
	default:
		//nIndex = ItemSet.Add(nItemNature, nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType, nItemLevel, g_SubWorldSet.GetGameVersion());
		break;
	}

	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	//Lua_PushNumber(L, nIndex);
	INT	P = pos_equiproom, x = 0, y = 0;
	POINT	pPos;

	if (P < pos_hand || P > pos_num)
		return 0;

	if (nParamNum == 5 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].CheckRoom(x, y, Item[nIndex].GetWidth(), Item[nIndex].GetHeight()))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, y, false);
	else if (nParamNum == 4 && Player[nPlayerIndex].m_ItemList.PositionToIndex(P, x) <= 0)
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, 0, false);
	else if (nParamNum == 3 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].FindRoom(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &pPos))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, pPos.x, pPos.y, false);
	else
		Player[nPlayerIndex].m_ItemList.InsertEquipment(nIndex, false);

	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaAddPlatinaItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nItemNature = NATURE_PLATINA;
	INT nItemClass = 0;
	INT nDetailType = 0;
	INT nParticularType = 0;
	INT nLevel = 0;
	INT nSeries = series_num;
	INT nLuck = _GetRandomNumber(10, 100);
	INT nStart = 0;
	INT nItemLevel[MAX_ITEM_MAGICLEVEL];
	ZeroMemory(nItemLevel, sizeof(nItemLevel));

	nDetailType = (INT)Lua_ValueToNumber(L, 1);

	if (nDetailType >= 1)
		nDetailType -= 1;

	if (nParamNum >= 2)
		nLevel = (INT)Lua_ValueToNumber(L, 2);//GetRandomNumber(0, 10);

	for (INT i = 1; i < MAX_ITEM_MAGICLEVEL; i++)
		nItemLevel[i] = nItemLevel[0];

	INT nIndex = 0;
	switch (nItemNature)
	{
	case NATURE_GOLD:
	case NATURE_PLATINA:
		nIndex = ItemSet.Add(nItemNature, 0, 0, nLevel, MAKELONG(nStart, nLuck), nDetailType, 0, nItemLevel[0] == 0 ? NULL : nItemLevel, g_SubWorldSet.GetGameVersion());
		break;
	default:
		//nIndex = ItemSet.Add(nItemNature, nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType, nItemLevel, g_SubWorldSet.GetGameVersion());
		break;
	}

	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	//Lua_PushNumber(L, nIndex);
	INT	P = pos_equiproom, x = 0, y = 0;
	POINT	pPos;

	if (P < pos_hand || P > pos_num)
		return 0;

	if (nParamNum == 5 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].CheckRoom(x, y, Item[nIndex].GetWidth(), Item[nIndex].GetHeight()))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, y, false);
	else if (nParamNum == 4 && Player[nPlayerIndex].m_ItemList.PositionToIndex(P, x) <= 0)
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, 0, false);
	else if (nParamNum == 3 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].FindRoom(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &pPos))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, pPos.x, pPos.y, false);
	else
		Player[nPlayerIndex].m_ItemList.InsertEquipment(nIndex, false);

	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaGetGoldIndex(Lua_State* L)
{
	INT nRow = -1;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx < 0 || nIdx >= MAX_ITEM)
		return 0;

	if (Item[nIdx].GetNature() == NATURE_GOLD || Item[nIdx].GetNature() == NATURE_PLATINA)
		nRow = Item[nIdx].GetRow();

	Lua_PushNumber(L, nRow);
	return 1;
}

INT LuaGetItemName(Lua_State* L)
{
	std::lock_guard<std::mutex> lock(mutex);
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx < 0 || nIdx >= MAX_ITEM)
		return 0;

	Lua_PushString(L, Item[nIdx].GetName());
	return 1;
}

INT LuaAddItemIdx(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx < 0 || nIdx >= MAX_ITEM)
		return 0;
	INT nIndex = ItemSet.Add(&Item[nIdx]);
	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaAddItemID(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT	P = pos_equiproom, x = 0, y = 0;
	POINT	pPos;
	if (nParamNum > 2)
	{
		P = (INT)Lua_ValueToNumber(L, 2);
	}
	if (nParamNum > 3)
	{
		P = (INT)Lua_ValueToNumber(L, 2);
		x = (INT)Lua_ValueToNumber(L, 3);
	}
	if (nParamNum > 4)
	{
		P = (INT)Lua_ValueToNumber(L, 2);
		x = (INT)Lua_ValueToNumber(L, 3);
		y = (INT)Lua_ValueToNumber(L, 4);
	}
	if (P < pos_hand || P > pos_num)
		return 0;

	if (nParamNum == 5 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].CheckRoom(x, y, Item[nIndex].GetWidth(), Item[nIndex].GetHeight()))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, y, true);
	else if (nParamNum == 4 && Player[nPlayerIndex].m_ItemList.PositionToIndex(P, x) <= 0)
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, 0, true);
	else if (nParamNum == 3 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].FindRoom(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &pPos))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, pPos.x, pPos.y, true);
	else
		Player[nPlayerIndex].m_ItemList.InsertEquipment(nIndex, true);

	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaAddStackItem(Lua_State* L) {

	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
		return 0;

	INT nStackNumber = 0;
	INT nItemNature = NATURE_NORMAL;
	INT nItemClass = 0;
	INT nDetailType = 0;
	INT nParticularType = 0;
	INT nLevel = 0;
	INT nSeries = series_num;
	INT nLuck = 0;
	INT nItemLevel[MAX_ITEM_MAGICLEVEL];
	ZeroMemory(nItemLevel, sizeof(nItemLevel));

	nStackNumber = (INT)Lua_ValueToNumber(L, 1);
	//nItemNature = (INT)Lua_ValueToNumber(L, 2);
	nItemClass = (INT)Lua_ValueToNumber(L, 2);
	nDetailType = (INT)Lua_ValueToNumber(L, 3);
	nParticularType = (INT)Lua_ValueToNumber(L, 4);
	nLevel = (INT)Lua_ValueToNumber(L, 5);
	nSeries = (INT)Lua_ValueToNumber(L, 6);
	nLuck = (INT)Lua_ValueToNumber(L, 7);

	nItemLevel[0] = 0;
	//if (nParamNum > 8)
	//	nLock = (INT)Lua_ValueToNumber(L, 9);

	INT nIndex = 0;

	if (nItemNature == NATURE_NORMAL)// && (nItemClass == 6 || nItemClass == 4))
	{
		nIndex = ItemSet.Add(nItemNature, nItemClass, nSeries, nLevel, nLuck, nDetailType, nParticularType, nItemLevel, g_SubWorldSet.GetGameVersion(), nStackNumber);
	}

	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Item[nIndex].SetStackNum(nStackNumber);
	Player[nPlayerIndex].m_ItemList.SyncItem(nIndex);
	Player[nPlayerIndex].m_ItemList.InsertEquipment(nIndex, false);
	Lua_PushNumber(L, nIndex);

	return 1;
}

INT LuaAddItemIDStack(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT	P = pos_equiproom, x = 0, y = 0;
	POINT	pPos;
	if (nParamNum > 2)
	{
		P = (INT)Lua_ValueToNumber(L, 2);
	}
	if (nParamNum > 3)
	{
		P = (INT)Lua_ValueToNumber(L, 2);
		x = (INT)Lua_ValueToNumber(L, 3);
	}
	if (nParamNum > 4)
	{
		P = (INT)Lua_ValueToNumber(L, 2);
		x = (INT)Lua_ValueToNumber(L, 3);
		y = (INT)Lua_ValueToNumber(L, 4);
	}
	if (P < pos_hand || P > pos_num)
		return 0;
	if (nParamNum == 5 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].CheckRoom(x, y, Item[nIndex].GetWidth(), Item[nIndex].GetHeight()))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, y);
	else if (nParamNum == 4 && Player[nPlayerIndex].m_ItemList.PositionToIndex(P, x) <= 0)
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, x, 0);
	else if (nParamNum == 3 && Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom(P)].FindRoom(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &pPos))
		Player[nPlayerIndex].m_ItemList.Add(nIndex, P, pPos.x, pPos.y);
	else
		Player[nPlayerIndex].m_ItemList.InsertEquipment(nIndex, true);
	return 0;
}

INT LuaGetMagicAttrib(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	INT nIndex;
	if (nParamNum < 1)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	nIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nIndex > 0)
	{
		for (INT i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			Lua_PushNumber(L, Item[nIndex].m_aryMagicAttrib[i].nAttribType);
			Lua_PushNumber(L, Item[nIndex].m_aryMagicAttrib[i].nValue[0]);
			Lua_PushNumber(L, Item[nIndex].m_aryMagicAttrib[i].nValue[2]);
		}
		return MAX_ITEM_MAGICATTRIB * 3;
	}
	else
	{
		for (INT i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			Lua_PushNumber(L, 0);
			Lua_PushNumber(L, 0);
			Lua_PushNumber(L, 0);
		}
		return MAX_ITEM_MAGICATTRIB * 3;
	}
	return 0;
}

INT LuaGetMaxAttrib(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
		return 0;

	INT nIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nMagicAttrib = (INT)Lua_ValueToNumber(L, 2);

	Lua_PushNumber(L, Item[nIndex].m_aryMagicAttrib[nMagicAttrib].nMax);
	return 0;
}

INT LuaGetMinAttrib(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
		return 0;

	INT nIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nMagicAttrib = (INT)Lua_ValueToNumber(L, 2);
	INT nIdx = Player[nPlayerIndex].m_ItemList.GetEquipment(nIndex);

	Lua_PushNumber(L, Item[nIdx].m_aryMagicAttrib[nMagicAttrib].nMin);
	return 0;
}


INT LuaSetMagicAttrib(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
		return 0;

	INT nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 24)
		return 0;

	INT nIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nIndex > 0 && Item[nIndex].GetGenre() == item_equip)
	{
		INT i = 0, k = 2;
		for (i = 0; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			Item[nIndex].m_GeneratorParam.nGeneratorLevel[i] = (INT)Lua_ValueToNumber(L, k);
			Item[nIndex].m_GeneratorParam.nGeneratorLevel[i + MAX_ITEM_MAGICATTRIB] = MAKELONG((INT)Lua_ValueToNumber(L, k + 1),
				(INT)Lua_ValueToNumber(L, k + 2));
			Item[nIndex].m_aryMagicAttrib[i].nAttribType = (INT)Lua_ValueToNumber(L, k);
			Item[nIndex].m_aryMagicAttrib[i].nValue[0] = (INT)Lua_ValueToNumber(L, k + 1);
			Item[nIndex].m_aryMagicAttrib[i].nValue[1] = -1;
			Item[nIndex].m_aryMagicAttrib[i].nValue[2] = (INT)Lua_ValueToNumber(L, k + 2);
			k += 3;
		}
		for (NULL; i < MAX_ITEM_MAGICATTRIB; i++)
		{
			Item[nIndex].m_GeneratorParam.nGeneratorLevel[i] = 0;
			Item[nIndex].m_GeneratorParam.nGeneratorLevel[i + MAX_ITEM_MAGICATTRIB] = 0;
			Item[nIndex].m_aryMagicAttrib[i].nAttribType = 0;
			Item[nIndex].m_aryMagicAttrib[i].nValue[0] = 0;
			Item[nIndex].m_aryMagicAttrib[i].nValue[1] = 0;
			Item[nIndex].m_aryMagicAttrib[i].nValue[2] = 0;
		}
		Player[nPlayerIndex].m_ItemList.SyncItemMagicAttrib(nIndex);
	}
	return 0;
}

/*
AddMagic(nPlayerIndex, nMagicID, nLevel)
DelMagic(nPlayerIndex, nMagicId)
HaveMagic(nPlayerIndex, nMagicId)
GetMagicLevel(nPlayerIndex, nMagicId)
SetMagicLevel(nPlayerIndex, nMagicId, nLevel)
ModifyMagicLevel(nPlayerIndex ,nMagicId, nDLevel)
*/
INT LuaAddMagic(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	INT nPlayerIndex = 0;
	if (nParamCount < 1) return 0;
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (INT)Lua_ValueToNumber(L, 1);
	}
	else
	{
		const char* sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0) return 0;
	}
	INT nSkillLevel = 0;
	if (nParamCount >= 2)
		nSkillLevel = (INT)Lua_ValueToNumber(L, 2);
	else
		nSkillLevel = 0;

	INT nRet = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Add(nSkillId, nSkillLevel);
	if (nRet)
	{
		Player[nPlayerIndex].UpdataCurData();
		PLAYER_SKILL_LEVEL_SYNC NewSkill;
		NewSkill.ProtocolType = s2c_playerskilllevel;
		NewSkill.m_nSkillID = nSkillId;
		NewSkill.m_nSkillLevel = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetLevel(nSkillId);
		NewSkill.m_nAddLevel = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetAddLevel(nSkillId);
		NewSkill.m_nSkillExp = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetExp(nSkillId);
		NewSkill.m_bTempSkill = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.IsTempSkill(nSkillId);
		NewSkill.m_nLeavePoint = Player[nPlayerIndex].m_nSkillPoint;
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
	}

	Lua_PushNumber(L, nRet);
	return 1;
}

INT LuaDelMagic(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	INT nPlayerIndex = 0;
	if (nParamCount < 1) return 0;

	nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	INT nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (INT)Lua_ValueToNumber(L, 1);
	}
	else
	{
		const char* sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0) return 0;
	}
	Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.Remove(nSkillId);

	Player[nPlayerIndex].SendSyncData_Skill();
	return 0;
}

INT LuaDelAllMagic(Lua_State* L)
{
	INT nPlayerIndex = 0;

	nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	for (INT i = 1; i < MAX_NPCSKILL; i++)
	{
		if (!Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.IsBaseSkill(
			Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.m_Skills[i].SkillId))
			Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.RemoveIdx(i);
	}

	Player[nPlayerIndex].SendSyncData_Skill();
	return 0;
}

INT LuaHaveMagic(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	INT nPlayerIndex = 0;
	if (nParamCount < 1) return 0;

	nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	INT nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (INT)Lua_ValueToNumber(L, 1);
	}
	else
	{
		const char* sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0)
		{
			Lua_PushNumber(L, -1);
		}
		return 1;
	}

	if (Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.FindSame(nSkillId))
	{
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetLevel(nSkillId));
	}
	else
	{
		Lua_PushNumber(L, -1);
	}

	return 1;
}

INT LuaIncSkill(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	INT nPlayerIndex = 0;
	if (nParamCount < 2) return 0;

	nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	INT nSkillId = 0, nAddLevel = 0;
	nSkillId = (INT)Lua_ValueToNumber(L, 1);
	nAddLevel = (INT)Lua_ValueToNumber(L, 2);
	if (nSkillId <= 0 || nAddLevel <= 0)
		return 0;
	Player[nPlayerIndex].IncSkillLevel(nSkillId, nAddLevel);
	return 0;
}

INT LuaIncSkillExp(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	INT nPlayerIndex = 0;
	if (nParamCount < 2) return 0;

	nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	INT nSkillId = 0, nAddExp = 0;
	nSkillId = (INT)Lua_ValueToNumber(L, 1);
	nAddExp = (INT)Lua_ValueToNumber(L, 2);
	if (nSkillId <= 0 || nAddExp <= 0)
		return 0;
	Player[nPlayerIndex].IncSkillExp(nSkillId, nAddExp);
	return 0;
}

INT LuaGetSkillExp(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	INT nPlayerIndex = 0;
	INT nSkillExp = 0;
	if (nParamCount < 2) return 0;

	nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	INT nSkillId = 0;
	nSkillId = (INT)Lua_ValueToNumber(L, 1);

	if (nSkillId <= 0)
		return 0;

	nSkillExp = Player[nPlayerIndex].GetSkillExp(nSkillId);

	Lua_PushNumber(L, nSkillId);
	return 0;
}

INT LuaGetSkillIdInSkillList(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	INT nSkillIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nSkillId = 0;
	if (nSkillIndex > 0)
	{
		nSkillId = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetSkillId(nSkillIndex);
	}
	Lua_PushNumber(L, nSkillId);
	return 1;
}

INT LuaGetSkillName(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	INT nSkillId = (INT)Lua_ValueToNumber(L, 1);

	if (nSkillId > 0)
	{
		KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nSkillId, 1);
		if (!pOrdinSkill)
			return 0;
		Lua_PushString(L, pOrdinSkill->GetSkillName());
	}

	return 1;
}

INT LuaSetSkillLevel(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	INT nSkillId = 0;

	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (INT)Lua_ValueToNumber(L, 1);
	}
	else
	{
		const char* sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0) return 0;
	}
	INT nSkillLevel = (INT)Lua_ValueToNumber(L, 2);
	if (nSkillLevel >= 0)
		Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.SetSkillLevelDirectlyUsingId(nSkillId, nSkillLevel);
	return 0;
}

INT LuaSetSkillTemp(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	INT nSkillIdx = (INT)Lua_ValueToNumber(L, 1);
	BOOL bSkillTemp = (INT)Lua_ValueToNumber(L, 2) > 0;

	Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.SetTempSkill(nSkillIdx, bSkillTemp);
	return 0;
}

INT LuaRollBackSkills(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	bool bRbAll = false;
	if (Lua_GetTopIndex(L) > 1)
		bRbAll = (INT)Lua_ValueToNumber(L, 1) > 0;

	INT nTotalSkill = Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.RollBackSkills(bRbAll);

	Player[nPlayerIndex].SendSyncData_Skill();

	Lua_PushNumber(L, nTotalSkill);
	return 1;
}

INT LuaGetMagicLevel(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	INT nPlayerIndex = 0;

	if (nParamCount < 1) return 0;
	nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	INT nSkillId = 0;
	if (Lua_IsNumber(L, 1))
	{
		nSkillId = (INT)Lua_ValueToNumber(L, 1);
	}
	else
	{
		const char* sSkillName = Lua_ValueToString(L, 1);
		g_OrdinSkillsSetting.GetInteger((char*)sSkillName, "SkillId", 0, &nSkillId);
		if (nSkillId <= 0) return 0;
	}
	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_SkillList.GetLevel(nSkillId));
	return 1;

}
/*
INT LuaSetMagicLevel(Lua_State * L)
{
INT nParamCount = Lua_GetTopIndex(L);
INT nPlayerIndex = 0;
INT nTemp = 0;
if (nParamCount < 1) return 0;

  nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0) return 0;

	  INT nSkillId = 0;
	  if (Lua_IsNumber(L, nTemp))
	  {
	  nSkillId = (INT)Lua_ValueToNumber (L, 1);
	  }
	  else
	  {
	  const char * sSkillName = Lua_ValueToString(L, 1);
	  nSkillId = g_OrdinSkillsSetting.FindRow((char *)sSkillName) - 2;
	  if (nSkillId <= 0 ) return 0;
	  }
	  INT nNpcIndex = Player[nPlayerIndex].m_nIndex;
	  if (nNpcIndex > 0)
	  Lua_PushNumber(L,Npc[nNpcIndex].m_SkillList.SetSkillLevel(nSkillId, (INT)Lua_ValueToNumber(L, 2)));
	  return 0;
	  }
*/
//**************************************************************************************************************************************************************
//												NPC²Ù×÷½Å±¾
//**************************************************************************************************************************************************************
/*nNpcTemplateId GetNpcTmpId(sName)
¹¦ÄÜ´ÓNpcÄ£°åÖÐ»ñµÃÃû³ÆÎªsNameµÄNpcÔÚÄ£°åÖÐµÄId
sName:NpcÃû³Æ
nNpcTemplateID:Ä£°åÖÐId
*/

INT LuaGetNpcTemplateID(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex > 0)
	{
		Lua_PushNumber(L, Npc[nNpcIndex].m_NpcSettingIdx);
		return 1;
	}
	return 0;
}

INT LuaGetNpcTempName(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;

	INT nNpcSettingIdx = (INT)Lua_ValueToNumber(L, 1);
	char	Name[32];
	g_NpcSetting.GetString(nNpcSettingIdx + 2, "Name", "", Name, sizeof(Name));
	Lua_PushString(L, Name);
	return 1;
}

INT LuaGetNpcTempTypeName(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;

	INT nNpcSettingIdx = (INT)Lua_ValueToNumber(L, 1);
	char	szNpcTypeName[32];
	g_NpcSetting.GetString(nNpcSettingIdx + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));
	Lua_PushString(L, szNpcTypeName);
	return 1;
}
/*
nNpcIndex AddNpc(nNpcTemplateId,nLevel, nSubWorldIndex, nPosX, nPosY )

  ¹¦ÄÜ£ºÔö¼ÓÒ»¸öÌØ¶¨µÄNPC
  ²ÎÊý£º
  nNpcTemplateId: NPCÔÚNPCÄ£°åÖÐµÄid
  nLevel:NpcµÄµÈ¼¶
  nSubWorldIndex:Ëù´¦µÄÊÀ½çid
  nPosX£ºX (µã×ø±ê)
  nPosY£ºY (µã×ø±ê)
  nNpcIndex:Ôö¼Óºó£¬½«·µ»Ø¸ÃNpcÔÚÓÎÏ·ÊÀ½çµÄIndex£¬Èç¹û²»³É¹¦·µ»Ønil
*/

INT LuaAddNpc(Lua_State* L)
{
	char* pName = NULL;
	INT	   nId = 0;
	if (Lua_GetTopIndex(L) < 6) return 0;

	if (Lua_IsNumber(L, 1))
	{
		nId = (INT)Lua_ValueToNumber(L, 1);
	}
	else if (Lua_IsString(L, 1))
	{
		pName = (char*)lua_tostring(L, 1);
		nId = g_NpcSetting.FindRow((char*)pName) - 2;
	}
	else return 0;

	if (nId < 0 || nId >(g_NpcSetting.GetHeight() - 2))
		return 0;

	INT nLevel = (INT)lua_tonumber(L, 2);
	INT nSubWorld = (INT)lua_tonumber(L, 3);
	if (nLevel >= 128)
		nLevel = 127;
	if (nLevel < 0)
		nLevel = 1;

	INT	nNpcIdxInfo = MAKELONG(nLevel, nId);//(nId << 7) + nLevel;
	//question
	if (nSubWorld != -1)
	{
		INT nNpcIdx = NpcSet.Add(nNpcIdxInfo, nSubWorld, (INT)lua_tonumber(L, 4), (INT)lua_tonumber(L, 5), (BOOL)lua_tonumber(L, 6));

		if (nNpcIdx > 0)
			Lua_PushNumber(L, nNpcIdx);
	}

	return 1;
}

/*nResult DelNpc (nNpcIndex)
¹¦ÄÜ£ºÉ¾³ýÒ»¸öÌØ¶¨µÄNPC
nResult:·µ»Ø³É¹¦Óë·ñ,1Îª³É¹¦,0ÎªÊ§°Ü
*/
INT LuaDelNpc(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (Npc[nNpcIndex].m_Kind == kind_player) return 0;
	if (nNpcIndex > 0)
	{
		if (Npc[nNpcIndex].m_RegionIndex >= 0)
		{
			SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].RemoveNpc(nNpcIndex);
			SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].DecNpcRef(Npc[nNpcIndex].m_MapX, Npc[nNpcIndex].m_MapY, obj_npc);
		}
		NpcSet.Remove(nNpcIndex);
	}
	return 0;
}

INT LuaClearMapNpc(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nSubWorldIndex = g_SubWorldSet.SearchWorld((INT)Lua_ValueToNumber(L, 1));

	INT nResult = 0;
	if (nSubWorldIndex >= 0)
	{
		for (INT nNpcIndex = 1; nNpcIndex < MAX_NPC; ++nNpcIndex)
		{
			if (Npc[nNpcIndex].m_SubWorldIndex == nSubWorldIndex && Npc[nNpcIndex].m_Kind != kind_player)
			{
				if (Npc[nNpcIndex].m_RegionIndex >= 0)
				{
					SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].RemoveNpc(nNpcIndex);
					SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].DecNpcRef(Npc[nNpcIndex].m_MapX, Npc[nNpcIndex].m_MapY, obj_npc);
				}
				NpcSet.Remove(nNpcIndex);
				nResult++;
			}
		}
	}
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaClearMapNpcWithName(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nSubWorldIndex = g_SubWorldSet.SearchWorld((INT)Lua_ValueToNumber(L, 1));

	INT nResult = 0;
	if (nSubWorldIndex >= 0)
	{
		for (INT nNpcIndex = 1; nNpcIndex < MAX_NPC; ++nNpcIndex)
		{
			if (Npc[nNpcIndex].m_SubWorldIndex == nSubWorldIndex)
			{
				if (Npc[nNpcIndex].m_SubWorldIndex == nSubWorldIndex &&
					strcmp(Npc[nNpcIndex].Name, (char*)Lua_ValueToString(L, 2)) == 0 &&
					Npc[nNpcIndex].m_Kind != kind_player)
				{
					if (Npc[nNpcIndex].m_RegionIndex >= 0)
					{
						SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].RemoveNpc(nNpcIndex);
						SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_Region[Npc[nNpcIndex].m_RegionIndex].DecNpcRef(Npc[nNpcIndex].m_MapX, Npc[nNpcIndex].m_MapY, obj_npc);
					}
					NpcSet.Remove(nNpcIndex);
					nResult++;
				}
			}
		}
	}
	Lua_PushNumber(L, nResult);
	return 1;
}

/*
nDelCount DelNpcsInRgn(nSubWorld,nRegionId, nKind)
¹¦ÄÜ£ºÉ¾³ýÄ³¸öÓÎÏ·ÊÀ½çÖÐÄ³¸öRegionÄÚµÄËùÓÐÄ³ÀàµÄNPC
·µ»Ø:É¾³ýµÄNpc¸öÊý
*/

INT LuaDelNpcsInRgn(Lua_State* L)
{
	//Question
	return 0;
}
/*
nDelCount DelNpcsInWld(nSubWorldId, nKind)
¹¦ÄÜ£ºÉ¾³ýÄ³¸öÓÎÏ·ÊÀ½çÖÐµÄËùÓÐNpc
*/
INT LuaDelNpcsInWld(Lua_State* L)
{
	return 0;
}

INT LuaSyncNpc(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1) return 0;
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	Npc[nNpcIndex].SendSyncData(0, TRUE);
	return 0;
}

INT LuaSetNpcPos(Lua_State* L)
{
	INT nParamCount = 0;
	if ((nParamCount = Lua_GetTopIndex(L)) < 3) return 0;
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0) return 0;

	INT nX = (INT)Lua_ValueToNumber(L, 2);
	INT nY = (INT)Lua_ValueToNumber(L, 3);

	Npc[nNpcIndex].SetPos(nX * 32, nY * 32);
	return 0;
}


INT LuaSetNpcActionScript(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2) return 0;
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	strcpy(Npc[nNpcIndex].ActionScript, Lua_ValueToString(L, 2));
	Npc[nNpcIndex].m_ActionScriptID = g_FileName2Id((char*)Lua_ValueToString(L, 2));

	if (Npc[nNpcIndex].m_Kind == kind_normal)
		NpcSet.ExecuteScript(nNpcIndex, Npc[nNpcIndex].m_ActionScriptID, "Revive", nNpcIndex);
	return 0;
}

INT LuaSetNpcKind(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nKind = (INT)Lua_ValueToNumber(L, 2);
	if (nKind < kind_normal || nKind >= kind_num)
		return 0;

	Npc[nNpcIndex].m_Kind = nKind;
	return 0;
}

INT LuaSetNpcSeries(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].SetSeries((INT)Lua_ValueToNumber(L, 2));
	return 0;
}


INT LuaGetNpcSeries(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Lua_PushNumber(L, Npc[nNpcIndex].m_Series);
	return 1;
}

INT LuaSetNpcExp(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nExp = (INT)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentExperience = nExp;
	if (nParamNum > 2 && (INT)Lua_ValueToNumber(L, 3) > 0)
		Npc[nNpcIndex].m_Experience = nExp;
	return 0;
}

INT LuaSetNpcLife(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nLife = (INT)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentLife = nLife;
	Npc[nNpcIndex].m_CurrentLifeMax = nLife;
	Npc[nNpcIndex].m_LifeMax = nLife;

	//if (nParamNum > 2 && (INT)Lua_ValueToNumber(L, 3) > 0)
	//	Npc[nNpcIndex].m_LifeMax = nLife;
	return 0;
}


INT LuaGetNpcLife(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Lua_PushNumber(L, Npc[nNpcIndex].m_CurrentLifeMax);
	return 1;
}


INT LuaSetNpcLifeReplenish(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nLifeRep = (INT)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentLifeReplenish = nLifeRep;
	if (nParamNum > 2 && (INT)Lua_ValueToNumber(L, 3) > 0)
		Npc[nNpcIndex].m_LifeReplenish = nLifeRep;
	return 0;
}

INT LuaSetNpcAR(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nAttackR = (INT)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentAttackRating = nAttackR;
	if (nParamNum > 2 && (INT)Lua_ValueToNumber(L, 3) > 0)
		Npc[nNpcIndex].m_AttackRating = nAttackR;
	return 0;
}

INT LuaSetNpcDefense(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nDefense = (INT)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentDefend = nDefense;
	if (nParamNum > 2 && (INT)Lua_ValueToNumber(L, 3) > 0)
		Npc[nNpcIndex].m_Defend = nDefense;
	return 0;
}

INT LuaSetNpcDamage(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].SetPhysicsDamage((INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3));
	return 0;
}


INT LuaSetNpcDmgEx(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if ((nParamNum > 6) && (INT)Lua_ValueToNumber(L, 7))
	{
		Npc[nNpcIndex].m_CurrentAddPhysicsDamage = (INT)Lua_ValueToNumber(L, 2);

		Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[0] = (INT)Lua_ValueToNumber(L, 3);
		if (Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[0] > 0)
		{
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[1] = POISON_DAMAGE_TIME;//pMagic->nValue[1];
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[2] = POISON_DAMAGE_INTERVAL;//pMagic->nValue[2];
		}
		else
		{
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[0] = 0;
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[1] = 0;
			Npc[nNpcIndex].m_CurrentPoisonDamage.nValue[2] = 0;
		}

		Npc[nNpcIndex].m_CurrentColdDamage.nValue[0] = (INT)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_CurrentColdDamage.nValue[2] = (INT)Lua_ValueToNumber(L, 4);
		if (Npc[nNpcIndex].m_CurrentColdDamage.nValue[0] > 0 && Npc[nNpcIndex].m_CurrentColdMagic.nValue[2] > 0)
			Npc[nNpcIndex].m_CurrentColdDamage.nValue[1] = COLD_DAMAGE_TIME;
		else
		{
			Npc[nNpcIndex].m_CurrentColdDamage.nValue[0] = 0;
			Npc[nNpcIndex].m_CurrentColdDamage.nValue[2] = 0;
			Npc[nNpcIndex].m_CurrentColdDamage.nValue[1] = 0;
		}

		Npc[nNpcIndex].m_CurrentFireDamage.nValue[0] = (INT)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentFireDamage.nValue[2] = (INT)Lua_ValueToNumber(L, 5);

		Npc[nNpcIndex].m_CurrentLightDamage.nValue[0] = (INT)Lua_ValueToNumber(L, 6);
		Npc[nNpcIndex].m_CurrentLightDamage.nValue[2] = (INT)Lua_ValueToNumber(L, 6);
	}
	else
	{
		Npc[nNpcIndex].m_CurrentAddPhysicsMagic = (INT)Lua_ValueToNumber(L, 2);

		Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[0] = (INT)Lua_ValueToNumber(L, 3);
		if (Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[0] > 0)
		{
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[1] = POISON_DAMAGE_TIME;//pMagic->nValue[1];
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[2] = POISON_DAMAGE_INTERVAL;//pMagic->nValue[2];
		}
		else
		{
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[0] = 0;
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[1] = 0;
			Npc[nNpcIndex].m_CurrentPoisonMagic.nValue[2] = 0;
		}

		Npc[nNpcIndex].m_CurrentColdMagic.nValue[0] = (INT)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_CurrentColdMagic.nValue[2] = (INT)Lua_ValueToNumber(L, 4);
		if (Npc[nNpcIndex].m_CurrentColdMagic.nValue[0] > 0 && Npc[nNpcIndex].m_CurrentColdMagic.nValue[2] > 0)
			Npc[nNpcIndex].m_CurrentColdMagic.nValue[1] = COLD_DAMAGE_TIME;
		else
		{
			Npc[nNpcIndex].m_CurrentColdMagic.nValue[0] = 0;
			Npc[nNpcIndex].m_CurrentColdMagic.nValue[2] = 0;
			Npc[nNpcIndex].m_CurrentColdMagic.nValue[1] = 0;
		}
		Npc[nNpcIndex].m_CurrentFireMagic.nValue[0] = (INT)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentFireMagic.nValue[2] = (INT)Lua_ValueToNumber(L, 5);

		Npc[nNpcIndex].m_CurrentLightMagic.nValue[0] = (INT)Lua_ValueToNumber(L, 6);
		Npc[nNpcIndex].m_CurrentLightMagic.nValue[2] = (INT)Lua_ValueToNumber(L, 6);
	}
	return 0;
}

INT LuaSetNpcResist(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 6)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if (Npc[nNpcIndex].m_btSpecial == npc_gold || Npc[nNpcIndex].m_btSpecial == npc_blue)
	{
		Npc[nNpcIndex].m_CurrentPhysicsResistMax = MAX_BOSS_RESIST;
		Npc[nNpcIndex].m_CurrentPoisonResistMax = MAX_BOSS_RESIST;
		Npc[nNpcIndex].m_CurrentLightResistMax = MAX_BOSS_RESIST;
		Npc[nNpcIndex].m_CurrentFireResistMax = MAX_BOSS_RESIST;
		Npc[nNpcIndex].m_CurrentColdResistMax = MAX_BOSS_RESIST;

		Npc[nNpcIndex].m_CurrentPhysicsResist = (INT)Lua_ValueToNumber(L, 2);
		Npc[nNpcIndex].m_CurrentPoisonResist = (INT)Lua_ValueToNumber(L, 3);
		Npc[nNpcIndex].m_CurrentLightResist = (INT)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_CurrentFireResist = (INT)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentColdResist = (INT)Lua_ValueToNumber(L, 6);
	}
	else
	{
		Npc[nNpcIndex].m_CurrentPhysicsResistMax = (INT)Lua_ValueToNumber(L, 2);
		Npc[nNpcIndex].m_CurrentPoisonResistMax = (INT)Lua_ValueToNumber(L, 3);
		Npc[nNpcIndex].m_CurrentLightResistMax = (INT)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_CurrentFireResistMax = (INT)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentColdResistMax = (INT)Lua_ValueToNumber(L, 6);

		Npc[nNpcIndex].m_CurrentPhysicsResist = (INT)Lua_ValueToNumber(L, 2);
		Npc[nNpcIndex].m_CurrentPoisonResist = (INT)Lua_ValueToNumber(L, 3);
		Npc[nNpcIndex].m_CurrentLightResist = (INT)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_CurrentFireResist = (INT)Lua_ValueToNumber(L, 5);
		Npc[nNpcIndex].m_CurrentColdResist = (INT)Lua_ValueToNumber(L, 6);
	}

	if ((nParamNum > 6) && (INT)Lua_ValueToNumber(L, 7))
	{
		if (Npc[nNpcIndex].m_btSpecial == npc_gold || Npc[nNpcIndex].m_btSpecial == npc_blue)
		{
			Npc[nNpcIndex].m_PhysicsResist = MAX_BOSS_RESIST;
			Npc[nNpcIndex].m_PoisonResist = MAX_BOSS_RESIST;
			Npc[nNpcIndex].m_LightResist = MAX_BOSS_RESIST;
			Npc[nNpcIndex].m_FireResist = MAX_BOSS_RESIST;
			Npc[nNpcIndex].m_ColdResist = MAX_BOSS_RESIST;

			Npc[nNpcIndex].m_PhysicsResistMax = (INT)Lua_ValueToNumber(L, 2);
			Npc[nNpcIndex].m_PoisonResistMax = (INT)Lua_ValueToNumber(L, 3);
			Npc[nNpcIndex].m_LightResistMax = (INT)Lua_ValueToNumber(L, 4);
			Npc[nNpcIndex].m_FireResistMax = (INT)Lua_ValueToNumber(L, 5);
			Npc[nNpcIndex].m_ColdResistMax = (INT)Lua_ValueToNumber(L, 6);
		}
		else
		{
			Npc[nNpcIndex].m_PhysicsResist = (INT)Lua_ValueToNumber(L, 2);
			Npc[nNpcIndex].m_PoisonResist = (INT)Lua_ValueToNumber(L, 3);
			Npc[nNpcIndex].m_LightResist = (INT)Lua_ValueToNumber(L, 4);
			Npc[nNpcIndex].m_FireResist = (INT)Lua_ValueToNumber(L, 5);
			Npc[nNpcIndex].m_ColdResist = (INT)Lua_ValueToNumber(L, 6);

			Npc[nNpcIndex].m_PhysicsResistMax = (INT)Lua_ValueToNumber(L, 2);
			Npc[nNpcIndex].m_PoisonResistMax = (INT)Lua_ValueToNumber(L, 3);
			Npc[nNpcIndex].m_LightResistMax = (INT)Lua_ValueToNumber(L, 4);
			Npc[nNpcIndex].m_FireResistMax = (INT)Lua_ValueToNumber(L, 5);
			Npc[nNpcIndex].m_ColdResistMax = (INT)Lua_ValueToNumber(L, 6);
		}
	}
	return 0;
}

INT LuaSetNpcRevTime(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].SetReviveFrame((INT)Lua_ValueToNumber(L, 2));
	return 0;
}

INT LuaSetNpcSpeed(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentWalkSpeed = (INT)Lua_ValueToNumber(L, 2);
	if (nParamNum > 2 && (INT)Lua_ValueToNumber(L, 3) > 0)
		Npc[nNpcIndex].m_WalkSpeed = (INT)Lua_ValueToNumber(L, 2);
	return 0;
}

INT LuaSetNpcHitRecover(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_CurrentHitRecover = (INT)Lua_ValueToNumber(L, 2);

	if (nParamNum > 2 && (INT)Lua_ValueToNumber(L, 3) > 0)
		Npc[nNpcIndex].m_HitRecover = (INT)Lua_ValueToNumber(L, 2);
	return 0;
}

INT LuaSetNpcBoss(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_btSpecial = (INT)Lua_ValueToNumber(L, 2);

	if (Npc[nNpcIndex].m_btSpecial == npc_blue)
	{
		Npc[nNpcIndex].m_PhysicsResistMax = MAX_RESIST;
	}
	else if (Npc[nNpcIndex].m_btSpecial == npc_gold)
	{
		Npc[nNpcIndex].m_PhysicsResistMax = MAX_RESIST;
	}

	return 0;
}

INT LuaGetNpcBoss(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Lua_PushNumber(L, Npc[nNpcIndex].m_btSpecial);
	return 1;
}

INT LuaIsBlueBoss(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if (Npc[nNpcIndex].m_btSpecial == npc_blue)
		Lua_PushNumber(L, 1);
	else
		Lua_PushNumber(L, 0);
	return 1;
}

INT LuaGetNpcExpRate(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}
	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentExpEnhance);
	return 1;
}

INT LuaIsRideHorse(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}
	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_bRideHorse);
	return 1;
}

INT LuaSetNpcRemoveDeath(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_bNpcRemoveDeath = (BOOL)Lua_ValueToNumber(L, 2);
	return 0;
}

INT LuaSetNpcTimeout(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Npc[nNpcIndex].m_nNpcTimeout = (INT)Lua_ValueToNumber(L, 2) + g_SubWorldSet.GetGameTime();
	return 0;
}

INT LuaGetNpcTimeout(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Lua_PushNumber(L, g_SubWorldSet.GetGameTime() - Npc[nNpcIndex].m_nNpcTimeout < 0 ? 0 : g_SubWorldSet.GetGameTime() - Npc[nNpcIndex].m_nNpcTimeout);
	return 1;
}

INT LuaSetNpcParam(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if (nParamNum > 2)
		Npc[nNpcIndex].m_nNpcParam[(INT)Lua_ValueToNumber(L, 2)] = (INT)Lua_ValueToNumber(L, 3);
	else
		Npc[nNpcIndex].m_nNpcParam[0] = (INT)Lua_ValueToNumber(L, 2);
	return 0;
}

INT LuaGetNpcParam(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if (nParamNum > 1)
		Lua_PushNumber(L, Npc[nNpcIndex].m_nNpcParam[(INT)Lua_ValueToNumber(L, 2)]);
	else
		Lua_PushNumber(L, Npc[nNpcIndex].m_nNpcParam[0]);
	return 1;
}

INT LuaSetNpcOwner(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	strcpy(Npc[nNpcIndex].Owner, (char*)Lua_ValueToString(L, 2));

	//	g_DebugLog("%s %d",Npc[nNpcIndex].Owner,nNpcIndex);
	if (nParamNum >= 3)
		Npc[nNpcIndex].m_bNpcFollowFindPath = (BOOL)Lua_ValueToNumber(L, 3);

	if (nParamNum >= 4)
	{
		INT nPlayerIndex = (INT)Lua_ValueToNumber(L, 4);
		Npc[nNpcIndex].m_nOwnerIndex = Npc[Player[nPlayerIndex].m_nIndex].m_Index;
		Npc[Player[nPlayerIndex].m_nIndex].m_nFollowIndex = Npc[nNpcIndex].m_Index;
	}

	Npc[nNpcIndex].m_uFindPathTime = g_SubWorldSet.GetGameTime();
	return 0;
}

INT LuaGetNpcOwner(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	lua_pushstring(L, Npc[nNpcIndex].Owner);

	return 1;
}

INT LuaSetNpcFindPathTime(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	DWORD dwTime = (DWORD)Lua_ValueToNumber(L, 2);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if (dwTime != -1)
		Npc[nNpcIndex].m_uFindPathMaxTime = dwTime * 20;
	else
		Npc[nNpcIndex].m_uFindPathMaxTime = -1;
	//	g_DebugLog("%s %d",Npc[nNpcIndex].m_uFindPathMaxTime,nNpcIndex);	
	return 0;
}

INT LuaSetNpcName(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	if (Lua_IsNumber(L, 2))
	{
		KTabFile Replace;
		Replace.Load(NPC_NAME_FILE);
		Replace.GetString((INT)Lua_ValueToNumber(L, 2) + 2, "targetname", "", Npc[nNpcIndex].Name, sizeof(Npc[nNpcIndex].Name));
	}
	else if (Lua_IsString(L, 2))
		strcpy(Npc[nNpcIndex].Name, (char*)Lua_ValueToString(L, 2));

	return 0;
}

INT LuaGetNpcName(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Lua_PushString(L, Npc[nNpcIndex].Name);
	return 1;
}

INT LuaChangeRoleName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
	{
		return 0;
	}

	char* npName = (char*)Lua_ValueToString(L, 1);
	//m_PlayerName

	INT nLen = strlen(npName);

	if (nLen > 31 || nLen < 8)
	{

		Lua_PushNumber(L, 0);
		return 1;
	}

	strcpy(Player[nPlayerIndex].m_szLastName, Npc[Player[nPlayerIndex].m_nIndex].Name);
	strcpy(Npc[Player[nPlayerIndex].m_nIndex].Name, npName);
	Lua_PushNumber(L, 1);

	return 1;
}

INT LuaCheckDataName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)	return 0;

	char* npName = (char*)Lua_ValueToString(L, 1);

	INT nLen = strlen(npName);

	if (nLen > 31 || nLen < 8)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	//Npc[Player[nPlayerIndex].m_nIndex].ClearstrInfo(STR_CHECK_NAME_RET);
	TCheckNameData sProcessData;
	sProcessData.nProtoId = c2s_rolecheckname_result;	  //·¢Íùgoddes´æµµ
	sProcessData.ulIdentity = nPlayerIndex;
	sProcessData.nDataLen = 1;
	sprintf(sProcessData.nAccName, Player[nPlayerIndex].AccountName);
	sprintf(sProcessData.nRoleName, npName);

	if (g_pDataClient)
	{
		g_pDataClient->SendPackToServer(&sProcessData, sizeof(TCheckNameData));
	}

	Lua_PushNumber(L, 1);

	return 1;
}

INT LuaGetNpcID(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		return 0;

	Lua_PushNumber(L, Npc[nNpcIndex].m_dwID);
	return 1;
}

INT LuaSetNpcSkill(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4)
		return 0;

	INT nNpcIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
		return 0;
	INT nSkillID = (INT)Lua_ValueToNumber(L, 2);
	if (nSkillID <= 0)
		return 0;
	INT nSkillLevel = (INT)Lua_ValueToNumber(L, 3);
	if (nSkillLevel < 1)
		nSkillLevel = 1;
	INT nSkillPos = (INT)Lua_ValueToNumber(L, 4);
	if (nSkillPos < 0 || nSkillPos > MAX_NPCSKILL)
		return 0;
	Npc[nNpcIdx].m_SkillList.SetNpcSkill(nSkillPos, nSkillID, nSkillLevel);
	return 1;
}

INT LuaSetNpcDropScript(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2) return 0;
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC) return 0;
	Npc[nNpcIndex].m_DropScriptID = g_FileName2Id((char*)Lua_ValueToString(L, 2));
	return 0;
}

INT LuaSetPlayerRevivalPos(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1)
		return 0;
	INT nSubWorldId = 0;
	INT nRevId = 0;

	if (nParamCount > 2)
	{
		nSubWorldId = (INT)Lua_ValueToNumber(L, 1);
		nRevId = (INT)Lua_ValueToNumber(L, 2);
	}
	else if (nParamCount == 1)
	{
		//nSubWorldId = -1;
		nRevId = (int)Lua_ValueToNumber(L, 1);
	}
	else
	{
		nRevId = (INT)Lua_ValueToNumber(L, 1);
	}

	Player[nPlayerIndex].SetRevivalPos(nSubWorldId, nRevId);
	return 0;
}

INT LuaGetPlayerRevivalPos(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1)
		return 0;
	INT nSubWorldId = 0;
	INT nRevId = 0;

	if (nParamCount > 2)
	{
		nSubWorldId = (INT)Lua_ValueToNumber(L, 1);
		nRevId = (INT)Lua_ValueToNumber(L, 2);
	}
	else
	{
		nRevId = (INT)Lua_ValueToNumber(L, 1);
	}

	POINT Pos;
	g_SubWorldSet.GetRevivalPosFromId(nSubWorldId ? nSubWorldId : SubWorld[Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex].m_SubWorldID, nRevId, &Pos);
	Lua_PushNumber(L, nSubWorldId);
	Lua_PushNumber(L, Pos.x);
	Lua_PushNumber(L, Pos.y);
	return 3;
}

INT LuaGetPlayerRevival(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
	{
		return 0;
	}

	POINT Pos;
	Player[nPlayerIndex].GetLoginRevival(&Pos);
	Lua_PushNumber(L, Pos.x);
	Lua_PushNumber(L, Pos.y);
	return 2;
}

INT LuaGetPlayerRevivalID(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
	{
		return 0;
	}

	Lua_PushNumber(L, Player[nPlayerIndex].GetLoginRevivalID());
	return 1;
}
//**************************************************************************************************************************************************************
//												ÁÄÌìÏûÏ¢½Å±¾
//**************************************************************************************************************************************************************

//**********************************************************************************************
//							Ö÷½ÇÊôÐÔ»ñµÃ
//**********************************************************************************************


#define MacroFun_GetPlayerInfoInt(L, MemberName) { INT nPlayerIndex = GetPlayerIndex(L);\
	if (nPlayerIndex > 0){	INT nNpcIndex = Player[nPlayerIndex].m_nIndex;	if (nNpcIndex > 0)Lua_PushNumber(L, Npc[nNpcIndex].MemberName);\
	else Lua_PushNil(L);}\
	else Lua_PushNil(L);\
return 1;}														

//ÕóÓª
INT LuaGetPlayerCurrentCamp(Lua_State* L)
{
	MacroFun_GetPlayerInfoInt(L, m_CurrentCamp);
}

INT LuaGetPlayerCamp(Lua_State* L)
{
	MacroFun_GetPlayerInfoInt(L, m_Camp);
}

INT LuaSetPlayerCamp(Lua_State* L)
{
	INT nValue = (INT)Lua_ValueToNumber(L, 1);
	if (nValue < 0) return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetCamp(nValue);
	}
	return 0;
}

INT LuaSetPlayerCurrentCamp(Lua_State* L)
{
	INT nValue = (INT)Lua_ValueToNumber(L, 1);
	if (nValue < 0) return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].SetCurrentCamp(nValue);
	}
	return 0;
}

INT LuaSetNpcCurCamp(Lua_State* L)
{
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 && nNpcIndex > MAX_NPC) return 0;
	INT nValue = (INT)Lua_ValueToNumber(L, 2);
	if (nValue >= camp_num) return 0;
	Npc[nNpcIndex].SetCurrentCamp(nValue);
	return 0;
}

INT LuaRestorePlayerCamp(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreCurrentCamp();
	}
	return 0;
}

INT LuaOpenTong(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_OPENTONGUI;
	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;

	UiInfo.m_bParam1 = 0;
	UiInfo.m_nBufferLen = sizeof(INT);

#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
#else
	UiInfo.m_bParam2 = 1;
	UiInfo.ProtocolType = (BYTE)s2c_scriptaction;
	UiInfo.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + UiInfo.m_nBufferLen - 1;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, (BYTE*)&UiInfo, UiInfo.m_wProtocolLong + 1);
#endif
	return 0;
}

INT LuaJoinTong(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	if (Npc[Player[nPlayerIndex].m_nIndex].m_Camp == camp_begin)
		return 0;

	Player[nPlayerIndex].m_cTong.JoinTong((char*)Lua_ValueToString(L, 1));
	return 0;
}


INT LuaCreateTong(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	if (Npc[Player[nPlayerIndex].m_nIndex].m_Camp == camp_begin)
		return 0;

	if ((INT)Lua_ValueToNumber(L, 2) < camp_justice || (INT)Lua_ValueToNumber(L, 2) > camp_balance)
		return 0;

	char* lpszTongName = (char*)Lua_ValueToString(L, 1);

	TONG_CREATE_SYNC	sCreate;
	sCreate.ProtocolType = s2c_createtong;
	sCreate.m_btCamp = (INT)Lua_ValueToNumber(L, 2);
	if (strlen(lpszTongName) < sizeof(sCreate.m_szName))
		strcpy(sCreate.m_szName, lpszTongName);
	else
	{
		memcpy(sCreate.m_szName, lpszTongName, sizeof(sCreate.m_szName) - 1);
		sCreate.m_szName[sizeof(sCreate.m_szName) - 1] = 0;
	}
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sCreate, sizeof(TONG_CREATE_SYNC));

	return 0;
}

INT LuaGetTongFlag(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_nFlag);
		return 1;
	}
	return 0;
}

INT LuaGetTongName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushString(L, Player[nPlayerIndex].m_cTong.m_szName);
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_dwTongNameID);
		return 2;
	}
	return 0;
}

INT LuaLeaveTong(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		STONG_SERVER_TO_CORE_LEAVE	sLeave;

		sLeave.m_nPlayerIdx = nPlayerIndex;
		strcpy(sLeave.m_szName, Player[nPlayerIndex].Name);
		Player[nPlayerIndex].m_cTong.Leave(&sLeave);

		return 1;
	}
	return 0;
}

INT LuaGetTongCamp(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_nCamp);
		return 1;
	}
	return 0;
}

INT LuaGetTongMemNum(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_dwMemberNum);
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_btManagerNum);
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_btDirectorNum);
		return 3;
	}
	return 0;
}

INT LuaGetTongFigure(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_nFigure);
		return 1;
	}
	return 0;
}

INT LuaGetTongMoney(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_dwMoney);
		return 1;
	}
	return 0;
}

INT LuaGetTongLevel(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_btLevel);
		return 1;
	}
	return 0;
}

INT LuaGetTongEff(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_dwTotalEff);
		return 1;
	}
	return 0;
}

INT LuaGetTongParam(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_cTong.m_nTongParam);
		return 1;
	}
	return 0;
}

INT LuaGetTongJoinTm(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (Lua_GetTopIndex(L) > 1)
		nPlayerIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, (INT)Player[nPlayerIndex].m_cTong.m_nJoinTm);
		return 1;
	}
	return 0;
}

INT LuaCommendMaster(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		STONG_ACCEPT_MASTER_COMMAND	sAccept;

		sAccept.ProtocolFamily = pf_tong;
		sAccept.ProtocolID = enumC2S_TONG_ACCEPT_MASTER;
		sAccept.m_dwParam = nPlayerIndex;
		sAccept.m_dwTongNameID = g_FileName2Id(Player[nPlayerIndex].m_cTong.m_szName);
		sAccept.m_btFigure = Player[nPlayerIndex].m_cTong.m_nFigure;
		sAccept.m_btPos = 1;
		sAccept.m_btAcceptFalg = Player[nPlayerIndex].m_cTong.CheckGetMasterPower();
		memcpy(sAccept.m_szName, Npc[Player[nPlayerIndex].m_nIndex].Name, sizeof(Npc[Player[nPlayerIndex].m_nIndex].Name));

		if (g_pTongClient)
			g_pTongClient->SendPackToServer((const void*)&sAccept, sizeof(STONG_ACCEPT_MASTER_COMMAND));
	}
	return 0;
}


INT LuaSetTongLevel(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nParamNum = Lua_GetTopIndex(L);
		if (nParamNum > 1)
		{
			STONG_CHANGE_TONG_INFO_COMMAND	sLevel;
			sLevel.ProtocolID = enumC2S_CHANGE_TONG_LEVEL;
			sLevel.ProtocolFamily = pf_tong;
			sLevel.m_dwTongNameID = g_FileName2Id(Player[nPlayerIndex].m_cTong.m_szName);
			sLevel.m_dwParam = nPlayerIndex;
			sLevel.m_nValue = (INT)Lua_ValueToNumber(L, 1);
			if (g_pTongClient)
				g_pTongClient->SendPackToServer((const void*)&sLevel, sizeof(STONG_CHANGE_TONG_INFO_COMMAND));
		}
	}
	return 0;
}

INT LuaSetTongMoney(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nParamNum = Lua_GetTopIndex(L);
		if (nParamNum > 1)
		{
			STONG_CHANGE_TONG_INFO_COMMAND	sMoney;
			sMoney.ProtocolID = enumC2S_CHANGE_TONG_MONEY;
			sMoney.ProtocolFamily = pf_tong;
			sMoney.m_dwTongNameID = g_FileName2Id(Player[nPlayerIndex].m_cTong.m_szName);
			sMoney.m_dwParam = nPlayerIndex;
			sMoney.m_nValue = (INT)Lua_ValueToNumber(L, 1);
			if (g_pTongClient)
				g_pTongClient->SendPackToServer((const void*)&sMoney, sizeof(STONG_CHANGE_TONG_INFO_COMMAND));
		}
	}
	return 0;
}

INT LuaSetTongEff(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nParamNum = Lua_GetTopIndex(L);
		if (nParamNum > 1)
		{
			STONG_CHANGE_TONG_INFO_COMMAND	sEff;
			sEff.ProtocolID = enumC2S_CHANGE_TONG_EFF;
			sEff.ProtocolFamily = pf_tong;
			sEff.m_dwTongNameID = g_FileName2Id(Player[nPlayerIndex].m_cTong.m_szName);
			sEff.m_dwParam = nPlayerIndex;
			sEff.m_nValue = (INT)Lua_ValueToNumber(L, 1);
			if (g_pTongClient)
				g_pTongClient->SendPackToServer((const void*)&sEff, sizeof(STONG_CHANGE_TONG_INFO_COMMAND));
		}
	}
	return 0;
}

INT LuaSetTongMemEffLW(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nParamNum = Lua_GetTopIndex(L);
		if (nParamNum > 1)
		{
			STONG_CHANGE_TONG_MEMBEREFF_COMMAND	sMemEff;
			sMemEff.ProtocolFamily = pf_tong;
			sMemEff.ProtocolID = enumC2S_CHANGE_TONG_MEMBEREFF;
			sMemEff.m_nValue1 = KSG_StringSetValue(3, Player[nPlayerIndex].m_cTong.m_nSaveEff, defTONG_EFF_LASTWEEK, (INT)Lua_ValueToNumber(L, 1));
			sMemEff.m_nValue2 = 0;
			sMemEff.m_dwParam = nPlayerIndex;
			sMemEff.m_dwTongNameID = g_FileName2Id(Player[nPlayerIndex].m_cTong.m_szName);
			sMemEff.m_btFigure = Player[nPlayerIndex].m_cTong.m_nFigure;
			memcpy(sMemEff.m_szName, Npc[Player[nPlayerIndex].m_nIndex].Name, sizeof(Npc[Player[nPlayerIndex].m_nIndex].Name));
			if (g_pTongClient)
				g_pTongClient->SendPackToServer((const void*)&sMemEff, sizeof(sMemEff));
		}
	}
	return 0;
}

INT LuaSetTongMemEffTW(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nParamNum = Lua_GetTopIndex(L);
		if (nParamNum > 1)
		{
			STONG_CHANGE_TONG_MEMBEREFF_COMMAND	sMemEff;
			sMemEff.ProtocolFamily = pf_tong;
			sMemEff.ProtocolID = enumC2S_CHANGE_TONG_MEMBEREFF;
			sMemEff.m_nValue1 = KSG_StringSetValue(3, Player[nPlayerIndex].m_cTong.m_nSaveEff, defTONG_EFF_THISWEEK, (INT)Lua_ValueToNumber(L, 1));
			sMemEff.m_nValue1 = KSG_StringSetValue(3, sMemEff.m_nValue1, defTONG_EFF_USEABLE,
				KSG_StringGetValue(3, Player[nPlayerIndex].m_cTong.m_nSaveEff, defTONG_EFF_USEABLE) + (INT)Lua_ValueToNumber(L, 1));
			sMemEff.m_nValue2 = (INT)Lua_ValueToNumber(L, 1);
			sMemEff.m_dwParam = nPlayerIndex;
			sMemEff.m_dwTongNameID = g_FileName2Id(Player[nPlayerIndex].m_cTong.m_szName);
			sMemEff.m_btFigure = Player[nPlayerIndex].m_cTong.m_nFigure;
			memcpy(sMemEff.m_szName, Npc[Player[nPlayerIndex].m_nIndex].Name, sizeof(Npc[Player[nPlayerIndex].m_nIndex].Name));
			if (g_pTongClient)
				g_pTongClient->SendPackToServer((const void*)&sMemEff, sizeof(sMemEff));
		}
	}
	return 0;
}

INT LuaSetTongMemEffUB(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nParamNum = Lua_GetTopIndex(L);
		if (nParamNum > 1)
		{
			STONG_CHANGE_TONG_MEMBEREFF_COMMAND	sMemEff;
			sMemEff.ProtocolFamily = pf_tong;
			sMemEff.ProtocolID = enumC2S_CHANGE_TONG_MEMBEREFF;
			sMemEff.m_nValue1 = KSG_StringSetValue(3, Player[nPlayerIndex].m_cTong.m_nSaveEff, defTONG_EFF_USEABLE, (INT)Lua_ValueToNumber(L, 1));
			sMemEff.m_nValue2 = 0;
			sMemEff.m_dwParam = nPlayerIndex;
			sMemEff.m_dwTongNameID = g_FileName2Id(Player[nPlayerIndex].m_cTong.m_szName);
			sMemEff.m_btFigure = Player[nPlayerIndex].m_cTong.m_nFigure;
			memcpy(sMemEff.m_szName, Npc[Player[nPlayerIndex].m_nIndex].Name, sizeof(Npc[Player[nPlayerIndex].m_nIndex].Name));
			if (g_pTongClient)
				g_pTongClient->SendPackToServer((const void*)&sMemEff, sizeof(sMemEff));
		}
	}
	return 0;
}

INT LuaGetTongMemEff(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (Lua_GetTopIndex(L) > 1)
		nPlayerIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, KSG_StringGetValue(3, (INT)Player[nPlayerIndex].m_cTong.m_nSaveEff, defTONG_EFF_LASTWEEK));
		Lua_PushNumber(L, KSG_StringGetValue(3, (INT)Player[nPlayerIndex].m_cTong.m_nSaveEff, defTONG_EFF_THISWEEK));
		Lua_PushNumber(L, KSG_StringGetValue(3, (INT)Player[nPlayerIndex].m_cTong.m_nSaveEff, defTONG_EFF_USEABLE));
		return 3;
	}
	return 0;
}

INT LuaSetTongParam(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nParamNum = Lua_GetTopIndex(L);
		if (nParamNum > 1)
		{
			STONG_CHANGE_TONG_INFO_COMMAND	TongParam;
			TongParam.ProtocolID = enumC2S_TONG_CHANGE_TONGPARAM;
			TongParam.ProtocolFamily = pf_tong;
			TongParam.m_dwTongNameID = g_FileName2Id(Player[nPlayerIndex].m_cTong.m_szName);
			TongParam.m_dwParam = nPlayerIndex;
			TongParam.m_nValue = (INT)Lua_ValueToNumber(L, 1);
			if (g_pTongClient)
				g_pTongClient->SendPackToServer((const void*)&TongParam, sizeof(STONG_CHANGE_TONG_INFO_COMMAND));
		}
	}
	return 0;
}

//ÃÅÅÉ
INT LuaGetPlayerFaction(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		char szName[32];
		Player[nPlayerIndex].GetFaction(szName);
		Lua_PushString(L, szName);
	}
	else
	{
		Lua_PushString(L, "");
	}
	return 1;
}

INT LuaGetPlayerFactionName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		char szName[32];
		Player[nPlayerIndex].GetFactionName(szName);
		Lua_PushString(L, szName);
	}
	else
	{
		Lua_PushString(L, "");
	}
	return 1;
}

INT LuaGetPlayerFactionCamp(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
		Lua_PushNumber(L, Player[nPlayerIndex].GetFactionCamp());
	else
		Lua_PushNumber(L, -1);

	return 1;
}

INT LuaGetPlayerFactionNo(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
		Lua_PushNumber(L, Player[nPlayerIndex].GetFactionNo());
	else
		Lua_PushNumber(L, -1);

	return 1;
}

INT LuaChangePlayerFaction(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nResult = 0;
	if (nPlayerIndex > 0)
	{
		const char* szFactionName = Lua_ValueToString(L, 1);
		Player[nPlayerIndex].LeaveCurFaction();
		if (strlen(szFactionName) == 0)
		{
			nResult = 1;
		}
		else
		{
			nResult = Player[nPlayerIndex].AddFaction((char*)szFactionName);
		}
	}
	Lua_PushNumber(L, nResult);
	return 1;
}

//¿¹ÐÔ *************************************************************************************
//0±íÊ¾µ±Ç°,1±íÊ¾Ô­Ê¼µÄ,2±íÊ¾×î´óµÄ
INT LuaGetPlayerColdResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentColdResist); break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ColdResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax); break;
		default:
			Lua_PushNil(L);
		}
	}
	else
		Lua_PushNil(L);
	return 1;
}

INT LuaSetPlayerColdResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax;

		INT nType = (INT)Lua_ValueToNumber(L, 1);

		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentColdResist = nValue;
			break;

		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_ColdResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_ColdResistMax = nValue;
			break;
		}


	}
	return 0;

}

INT LuaGetPlayerFireResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentFireResist); break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FireResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax); break;
		default:
			Lua_PushNil(L);
		}
	}
	else
		Lua_PushNil(L);
	return 1;
}

INT LuaSetPlayerFireResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax)
			nValue = Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax;

		INT nType = (INT)Lua_ValueToNumber(L, 1);

		switch (nType)
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentFireResist = nValue;
			break;

		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_FireResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_FireResistMax = nValue;
			break;
		}
	}
	return 0;
}


INT LuaGetPlayerLightResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLightResist); break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LightResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax); break;
		default:
			Lua_PushNil(L);
		}
	}
	else
		Lua_PushNil(L);
	return 1;
}

INT LuaSetPlayerLightResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		INT nValue = (INT)Lua_ValueToNumber(L, 2);
		if (nValue < 0) nValue = 0;
		if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax;

		INT nType = (INT)Lua_ValueToNumber(L, 1);

		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLightResist = nValue;
			break;

		case 1:
			Npc[Player[nPlayerIndex].m_nIndex].m_LightResist = nValue;
			break;
		case 2:
			Npc[Player[nPlayerIndex].m_nIndex].m_LightResistMax = nValue;
			break;
		}
	}
	else
		Lua_PushNil(L);
	return 0;
}


INT LuaGetPlayerPoisonResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPoisonResist); break;
		case 1:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResist); break;
		case 2:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax); break;
		default:
			Lua_PushNil(L);
		}
	}
	else
		Lua_PushNil(L);
	return 1;
}

INT LuaSetPlayerPoisonResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		{
			INT nValue = (INT)Lua_ValueToNumber(L, 2);
			if (nValue < 0) nValue = 0;
			if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax;

			INT nType = (INT)Lua_ValueToNumber(L, 1);

			switch ((INT)Lua_ValueToNumber(L, 1))
			{
			case 0:
				Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPoisonResist = nValue;
				break;

			case 1:
				Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResist = nValue;
				break;
			case 2:
				Npc[Player[nPlayerIndex].m_nIndex].m_PoisonResistMax = nValue;
				break;
			}
		}

	}


	return 0;

}

INT LuaGetPlayerPhysicsResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		{
			INT nType = (INT)Lua_ValueToNumber(L, 1);
			switch ((INT)Lua_ValueToNumber(L, 1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPhysicsResist); break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResist); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax); break;
			default:
				Lua_PushNil(L);
			}
		}

	}
	else
		Lua_PushNil(L);
	return 1;
}



INT LuaSetPlayerPhysicsResist(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		{
			INT nValue = (INT)Lua_ValueToNumber(L, 2);
			if (nValue < 0) nValue = 0;
			if (nValue > Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax) nValue = Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax;

			INT nType = (INT)Lua_ValueToNumber(L, 1);

			switch ((INT)Lua_ValueToNumber(L, 1))
			{
			case 0:
				Npc[Player[nPlayerIndex].m_nIndex].m_CurrentPhysicsResist = nValue;
				break;

			case 1:
				Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResist = nValue;
				break;
			case 2:
				Npc[Player[nPlayerIndex].m_nIndex].m_PhysicsResistMax = nValue;
				break;
			}
		}

	}
	return 0;

}

INT LuaGetNextExp(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_nNextLevelExp);
	}
	else
		Lua_PushNil(L);
	return 1;
}

//¾­ÑéÖµ*********************************************************************
INT LuaGetPlayerExp(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_nExp);
	}
	else
		Lua_PushNil(L);
	return 1;
}

//AddExp(200,10,0)
INT LuaModifyPlayerExp(Lua_State* L)
{
	INT bAllTeamGet = 0;
	if (Lua_GetTopIndex(L) >= 3)
		bAllTeamGet = (INT)Lua_ValueToNumber(L, 3);

	DWORD nDValue = (DWORD)Lua_ValueToNumber(L, 1);
	DWORD nTarLevel = (DWORD)Lua_ValueToNumber(L, 2);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (bAllTeamGet)
			Player[nPlayerIndex].AddExp(nDValue, nTarLevel);
		else
			Player[nPlayerIndex].AddSelfExp(nDValue, nTarLevel);
	}
	return 0;
}

INT LuaAddOwnExp(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nExp = (INT)Lua_ValueToNumber(L, 1);
		if (nExp < MAX_INT)
			Player[nPlayerIndex].DirectAddExp(nExp);
	}
	return 0;
}

INT LuaAddStackExp(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	INT nExp = (DWORD)Lua_ValueToNumber(L, 1);
	if (nExp > 0)
	{
		while (nExp > 0)
		{
			INT nExpAdd = Player[nPlayerIndex].m_nNextLevelExp - Player[nPlayerIndex].m_nExp;
			if (nExp >= nExpAdd)
			{
				nExp = nExp - nExpAdd;
			}
			else
			{
				nExpAdd = nExp;
				nExp = 0;
			}
			Player[nPlayerIndex].DirectAddExp(nExpAdd);
		}
	}
	return 0;
}


INT LuaGetPlayerLevel(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_Level);
	}
	else
		Lua_PushNil(L);
	return 1;
}

INT LuaGetPlayerLife(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		{
			INT nType = (INT)Lua_ValueToNumber(L, 1);
			switch ((INT)Lua_ValueToNumber(L, 1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLife); break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLifeMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_LifeMax); break;
			default:
				Lua_PushNil(L);
			}
		}

	}
	else
		Lua_PushNil(L);
	return 1;
}

INT LuaRestorePlayerLife(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreLife();
	}
	return 0;
}

INT LuaRestorePlayerMana(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreMana();
	}
	return 0;
}

INT LuaRestorePlayerStamina(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].RestoreStamina();
	}
	return 0;
}

INT LuaGetPlayerLifeReplenish(Lua_State* L)
{
	MacroFun_GetPlayerInfoInt(L, m_LifeReplenish);
}


INT LuaGetPlayerMana(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		{
			INT nType = (INT)Lua_ValueToNumber(L, 1);
			switch ((INT)Lua_ValueToNumber(L, 1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentMana); break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ManaMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_ManaMax); break;
			default:
				Lua_PushNil(L);
			}
		}

	}
	else
		Lua_PushNil(L);
	return 1;
}

INT LuaGetPlayerStamina(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		{
			INT nType = (INT)Lua_ValueToNumber(L, 1);
			switch ((INT)Lua_ValueToNumber(L, 1))
			{
			case 0:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentStamina); break;
			case 1:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_StaminaMax); break;
			case 2:
				Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_StaminaMax); break;
			default:
				Lua_PushNil(L);
			}
		}

	}
	else
		Lua_PushNil(L);
	return 1;
}


INT LuaGetPlayerManaReplenish(Lua_State* L)
{
	MacroFun_GetPlayerInfoInt(L, m_ManaReplenish);
}

INT LuaGetPlayerDefend(Lua_State* L)
{
	MacroFun_GetPlayerInfoInt(L, m_Defend);
}

INT LuaGetPlayerSex(Lua_State* L)
{
	MacroFun_GetPlayerInfoInt(L, m_nSex);
}

INT LuaSetPlayerSex(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nSex = 0;
	if (nPlayerIndex > 0)
	{
		if (Lua_GetTopIndex(L) > 1)
			nSex = (INT)Lua_ValueToNumber(L, 1);

		Npc[Player[nPlayerIndex].m_nIndex].SetSex(nSex);
	}
	return 1;
}

INT LuaGetPlayerIndex(Lua_State* L)
{
	MacroFun_GetPlayerInfoInt(L, GetPlayerIdx());
	return 0;
}


INT LuaGetPlayerNpcIdx(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nIndex = nPlayerIndex;
	if (nPlayerIndex > 0)
	{
		if (Lua_GetTopIndex(L) > 1)
			nIndex = (INT)Lua_ValueToNumber(L, 1);
		Lua_PushNumber(L, Player[nIndex].m_nIndex);
	}
	else
		Lua_PushNil(L);
	return 1;
}

INT LuaGetPlayerAccount(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nIndex = nPlayerIndex;
	if (nPlayerIndex > 0)
	{
		if (Lua_GetTopIndex(L) > 1)
			nIndex = (INT)Lua_ValueToNumber(L, 1);
		Lua_PushString(L, Player[nIndex].AccountName);
	}
	else
		Lua_PushNil(L);

	return 1;

}

INT LuaGetPlayerSeries(Lua_State* L)
{
	MacroFun_GetPlayerInfoInt(L, m_Series);
}

INT LuaSetPlayerSeries(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 1);
		Npc[Player[nPlayerIndex].m_nIndex].SetSeries(nValue);
	}
	return 0;

}

INT LuaGetPlayerCount(Lua_State* L)
{
	Lua_PushNumber(L, PlayerSet.GetPlayerNumber());
	return 1;
}

INT LuaGetNpcCount(Lua_State* L)
{
	Lua_PushNumber(L, NpcSet.GetNpcNumber());
	return 1;
}

INT LuaGetTotalItem(Lua_State* L)
{
	Lua_PushNumber(L, ItemSet.GetItemNumber());
	return 1;
}

INT LuaGetPlayerName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nIndex = nPlayerIndex;
	if (nPlayerIndex > 0)
	{
		if (Lua_GetTopIndex(L) > 1)
			nIndex = (INT)Lua_ValueToNumber(L, 1);
		Lua_PushString(L, Player[nIndex].Name);
	}
	else
		Lua_PushNil(L);

	return 1;

}

INT LuaGetPlayerID(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_dwID);
	}
	else
		Lua_PushNil(L);

	return 1;
}

INT LuaGetMateName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	Lua_PushString(L, Player[nPlayerIndex].m_cTask.GetSaveStr(TASKVALUE_BASEDATA_MATENAME));
	return 1;
}

INT LuaFindPlayer(Lua_State* L)
{
	INT nIndex = 0;
	if (Lua_IsNumber(L, 1))
	{
		DWORD dwID = (DWORD)Lua_ValueToNumber(L, 1);
		nIndex = NpcSet.SearchUUID(dwID);
	}
	else if (Lua_IsString(L, 1))
	{
		char* pszName = (char*)Lua_ValueToString(L, 1);
		nIndex = PlayerSet.GetFirstPlayer();
		while (nIndex >= 0)
		{
			if (strcmp(Player[nIndex].Name, pszName) == 0)
				break;

			nIndex = PlayerSet.GetNextPlayer();
		}
	}
	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaFindPlayerAccount(Lua_State* L)
{
	INT nIndex = 0;
	INT nTotalOnline = 0;
	if (Lua_IsNumber(L, 1))
	{
		DWORD dwID = (DWORD)Lua_ValueToNumber(L, 1);
		nIndex = NpcSet.SearchUUID(dwID);
	}
	else if (Lua_IsString(L, 1))
	{
		const char* pszName = (const char*)Lua_ValueToString(L, 1);

		nIndex = PlayerSet.GetFirstPlayer();
		while (nIndex >= 0 && nIndex < MAX_PLAYER)
		{
			if (strcmp(Player[nIndex].AccountName, pszName) == 0)
				break;

			nIndex = PlayerSet.GetNextPlayer();
		}
	}
	Lua_PushNumber(L, nIndex);
	return 1;
}

INT LuaFindNearNpc(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nIndex = 0;
		if (Lua_IsNumber(L, 1))
			nIndex = Player[nPlayerIndex].FindNearNpc((INT)Lua_ValueToNumber(L, 1), Lua_GetTopIndex(L) > 2 ? (INT)Lua_ValueToNumber(L, 2) : 0);
		else if (Lua_IsString(L, 1))
			nIndex = Player[nPlayerIndex].FindNearNpc((char*)Lua_ValueToString(L, 1), Lua_GetTopIndex(L) > 2 ? (INT)Lua_ValueToNumber(L, 2) : 0);
		Lua_PushNumber(L, nIndex);
		return 1;
	}
	return 0;
}

INT LuaFindAroundNpc(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Lua_IsNumber(L, 1))
		{
			INT nIndex = Player[nPlayerIndex].FindAroundNpc((DWORD)Lua_ValueToNumber(L, 1));
			Lua_PushNumber(L, nIndex);
			return 1;
		}
		else
			Lua_PushNil(L);
	}
	return 0;
}

INT LuaGetPlayerLeadExp(Lua_State* L)
{

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_nLeadExp);
	}
	else
		Lua_PushNil(L);

	return 1;
}

INT LuaGetPlayerLeadLevel(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_nLeadLevel);
	}
	else
		Lua_PushNil(L);

	return 1;
}



INT LuaGetPlayerRestAttributePoint(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_nAttributePoint);
	}
	else
		Lua_PushNil(L);

	return 1;
}

INT LuaGetPlayerRestSkillPoint(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_nSkillPoint);
	}
	else
		Lua_PushNil(L);

	return 1;
}

/*
INT LuaModifyPlayerRestSkillPoint(Lua_State *L)
{
//Question
INT nPlayerIndex = GetPlayerIndex(L);
if (nPlayerIndex > 0)
{
INT nDValue = (INT)Lua_ValueToNumber(L, 1);

  INT nNewSkillPoint = Player[nPlayerIndex].m_nSkillPoint + nDValue;
  if (nNewSkillPoint < 0 ) return 0;

	Player[nPlayerIndex].m_nSkillPoint = nNewSkillPoint;
	}

	  return 0;
	  }
*/

//»ù±¾ÊôÐÔ
INT LuaGetPlayerLucky(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurrentLucky);
			break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nLucky);
			break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nLucky);
			break;
		default:
			Lua_PushNil(L);
		}

	}
	return 1;
}

INT LuaGetPlayerEngergy(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurEngergy); break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nEngergy); break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nEngergy); break;
		default:
			Lua_PushNil(L);
		}
	}

	return 1;
}

INT LuaResetBaseAttribute(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	INT nParamCount = 0;
	if ((nParamCount = Lua_GetTopIndex(L)) < 2) return 0;

	if (nPlayerIndex > 0)
	{
		PLAYER_ADD_BASE_ATTRIBUTE_COMMAND cmd;
		cmd.m_btAttribute = (INT)Lua_ValueToNumber(L, 1);;
		cmd.m_nAddNo = (INT)Lua_ValueToNumber(L, 2);;
		cmd.ProtocolType = c2s_playeraddbaseattribute;
		Player[nPlayerIndex].ResetBaseAttribute((BYTE*)&cmd);
	}
	return 1;
}

INT LuaSetProtectTime(Lua_State* L)
{
	return 1;
}

INT LuaResetProp(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	INT nResult = 0;
	if (nPlayerIndex > 0)
		nResult = Player[nPlayerIndex].ResetProp();

	Lua_PushNumber(L, nResult);
	return 1;

}

INT LuaSetPlayerEngergy(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 1);
		if (nValue < 0 && Player[nPlayerIndex].m_nEngergy - nValue <= 0)
			nValue = 0;
		Player[nPlayerIndex].m_nAttributePoint -= nValue;
		Player[nPlayerIndex].SetBaseEngergy(nValue);
	}
	return 1;
}

INT LuaGetPlayerDexterity(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurDexterity); break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nDexterity); break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nDexterity); break;
		default:
			Lua_PushNil(L);
		}

	}
	return 1;
}

INT LuaSetPlayerDexterity(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 1);
		if (nValue < 0 && Player[nPlayerIndex].m_nDexterity - nValue <= 0)
			nValue = 0;
		Player[nPlayerIndex].m_nAttributePoint -= nValue;
		Player[nPlayerIndex].SetBaseDexterity(nValue);
	}
	return 1;
}

INT LuaGetPlayerStrength(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurStrength); break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nStrength); break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nStrength); break;
		default:
			Lua_PushNil(L);
		}

	}
	return 1;

}

INT LuaSetPlayerStrength(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 1);
		if (nValue < 0 && Player[nPlayerIndex].m_nStrength - nValue <= 0)
			nValue = 0;
		Player[nPlayerIndex].m_nAttributePoint -= nValue;
		Player[nPlayerIndex].SetBaseStrength(nValue);
	}
	return 1;
}

INT LuaGetPlayerVitality(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{

		INT nType = (INT)Lua_ValueToNumber(L, 1);
		switch ((INT)Lua_ValueToNumber(L, 1))
		{
		case 0:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nCurVitality); break;
		case 1:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nVitality); break;
		case 2:
			Lua_PushNumber(L, Player[nPlayerIndex].m_nVitality); break;
		default:
			Lua_PushNil(L);
		}

	}
	return 1;

}

INT LuaSetPlayerVitality(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 1);
		if (nValue < 0 && Player[nPlayerIndex].m_nVitality - nValue <= 0)
			nValue = 0;
		Player[nPlayerIndex].m_nAttributePoint -= nValue;
		Player[nPlayerIndex].SetBaseVitality(nValue);
	}
	return 1;
}

INT LuaGetPlayerCashMoney(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex > 0)
	{
		if (Lua_GetTopIndex(L) > 1)
		{
			INT nType = (INT)Lua_ValueToNumber(L, 1);
			if (nType == 1)
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetMoney(room_repository));
			else if (nType == 2)
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetMoney(room_equipment) + Player[nPlayerIndex].m_ItemList.GetMoney(room_repository));
		}
		else
			Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetMoney(room_equipment));
	}
	else Lua_PushNumber(L, 0);

	return 1;
}

INT LuaPlayerPayMoney(Lua_State* L)
{

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nMoney = (INT)Lua_ValueToNumber(L, 1);
		if (nMoney <= 0) return 0;
		if (Lua_GetTopIndex(L) > 2)
		{
			INT nType = (INT)Lua_ValueToNumber(L, 2);
			if (nType == 1)
				Player[nPlayerIndex].m_ItemList.AddMoney(room_repository, -nMoney);
			else if (nType == 2)
			{
				INT nLMoney = nMoney - Player[nPlayerIndex].m_ItemList.GetMoney(room_equipment);
				if (nLMoney > 0)
				{
					Player[nPlayerIndex].m_ItemList.AddMoney(room_repository, Player[nPlayerIndex].m_ItemList.GetMoney(room_equipment));
					Player[nPlayerIndex].m_ItemList.AddMoney(room_equipment, -nLMoney);
				}
				else
					Player[nPlayerIndex].m_ItemList.AddMoney(room_repository, nMoney);
			}
			Lua_PushNumber(L, 1);
		}
		else
		{
			if (Player[nPlayerIndex].Pay(nMoney))
				Lua_PushNumber(L, 1);
			else
				Lua_PushNumber(L, 0);
		}
	}
	else
		Lua_PushNumber(L, 0);

	return 1;
}

INT LuaPlayerEarnMoney(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nMoney = (INT)Lua_ValueToNumber(L, 1);
		if (nMoney <= 0) return 0;
		Player[nPlayerIndex].Earn(nMoney);
	}
	return 0;
}

INT LuaPlayerPrePayMoney(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nMoney = (INT)Lua_ValueToNumber(L, 1);
		if (nMoney <= 0) return 0;
		if (Player[nPlayerIndex].PrePay(nMoney))
			Lua_PushNumber(L, 1);
		else
			Lua_PushNumber(L, 0);
	}
	else
		Lua_PushNumber(L, 0);

	return 1;
}

INT LuaGetPlayerFortune(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex > 0)
	{
		Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetPlayerFortune());
	}
	else Lua_PushNumber(L, 0);

	return 1;
}

INT LuaPlayerExecuteScript(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0 || Player[nPlayerIndex].m_dwID <= 0) return FALSE;
	char* nScriptfiled = NULL;
	char* nFunName = NULL;
	INT  nVal = 0, nReg = 0;
	nScriptfiled = (char*)Lua_ValueToString(L, 1);
	nFunName = (char*)Lua_ValueToString(L, 2);
	nVal = (INT)Lua_ValueToNumber(L, 3);
	nReg = Player[nPlayerIndex].ExecuteScriptA(nScriptfiled, nFunName, nVal, "空");
	Lua_PushNumber(L, nReg);
	return 1;
}

//重载全部脚本
INT LuaLocalAllScript(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0 || Player[nPlayerIndex].m_dwID <= 0) return FALSE;
	ReLoadAllScript();
	return 0;
}

//重载脚本
INT LuaLocalScript(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nRegm = 0;
	if (nPlayerIndex < 0 || Player[nPlayerIndex].m_dwID <= 0) goto goout;
	char* nScriptid = (char*)Lua_ValueToString(L, 1);

	if (!nScriptid) goto goout;

	INT nLoadModel = 0;
	if (nParamCount >= 2)
		nLoadModel = (INT)Lua_ValueToNumber(L, 2);

	char szScriptFile[256];
	ZeroMemory(szScriptFile, sizeof(szScriptFile));
	switch (nLoadModel)
	{
	case 1:
		sprintf(szScriptFile, Utf8ToGbk(nScriptid, szScriptFile));
		break;
	case 2:
		sprintf(szScriptFile, GbkToUtf8(nScriptid).c_str());
		break;
	default:
		sprintf(szScriptFile, "%s", nScriptid);
		break;
	}

	if (szScriptFile[0] == 0) goto goout;
	nRegm = ReLoadScript(szScriptFile);
goout:
	Lua_PushNumber(L, nRegm);
	return 1;
}

//Attack dwID, Damage
INT LuaAttackNpc(Lua_State* L)
{
	INT nParamCount = 0;
	if ((nParamCount = Lua_GetTopIndex(L)) < 2) return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	DWORD nNpcID = (DWORD)Lua_ValueToNumber(L, 1);
	INT nNpcIndex = Player[nPlayerIndex].FindAroundNpc(nNpcID);//NpcSet.SearchID(nNpcID);
	if (nNpcIndex <= 0) return 0;

	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));

	DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
	DamageMagicAttribs[0].nValue[0] = Npc[nNpcIndex].m_CurrentLife;

	DamageMagicAttribs[1].nAttribType = magic_ignoredefense_p;
	DamageMagicAttribs[1].nValue[0] = 1;

	for (INT i = 0; i < nParamCount - 1; i++)
	{
		INT nVlau = (INT)Lua_ValueToNumber(L, 2 + i);
		DamageMagicAttribs[i + 2].nValue[0] = (INT)Lua_ValueToNumber(L, 2 + i);
		DamageMagicAttribs[i + 2].nValue[2] = (INT)Lua_ValueToNumber(L, 2 + i);
	}

	Npc[nNpcIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, -1, 0, DamageMagicAttribs, 0, 1, 0);
	return 0;
}

INT LuaSetPlayerChatForbiddenFlag(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	INT nFlag = Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].SetChatForbiddenTm(nFlag);
	return 0;
}

// SetRoleChatFlag(roleName,flag)
INT LuaSetRoleChatFlag(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);

	if (nParamCount < 1)
		return 0;

	const char* pszName = (const char*)Lua_ValueToString(L, 1);
	INT nIndex = PlayerSet.GetFirstPlayer();
	while (nIndex > 0)
	{
		if (strcmp(Player[nIndex].Name, pszName) == 0)
			break;

		nIndex = PlayerSet.GetNextPlayer();
	}


	if (nIndex >= 0 && nIndex < MAX_PLAYER && Player[nIndex].m_nNetConnectIdx >= 0)
	{
		INT nFlag;
		if (nParamCount >= 2)
		{
			nFlag = (INT)Lua_ValueToNumber(L, 2);
		}
		else
			nFlag = 1;
		Player[nIndex].SetChatForbiddenTm(nFlag);
	}
	return 0;
}

//ShutDownServer bIsSavePlayer
INT LuaShutDownServer(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);

	g_ReleaseCore();

	return 0;
}

INT LuaKickOutPlayer(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	const char* pszName = (const char*)Lua_ValueToString(L, 1);

	INT nIndex = PlayerSet.GetFirstPlayer();
	while (nIndex > 0)
	{
		if (strcmp(Player[nIndex].Name, pszName) == 0)
			break;

		nIndex = PlayerSet.GetNextPlayer();
	}
	if (nIndex >= 0 && nIndex < MAX_PLAYER && Player[nIndex].m_nNetConnectIdx >= 0)
	{
		printf("GM Kick out specific player.\n");
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);
	}
	return 0;
}

INT LuaKickOutAccount(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	const char* pszName = (const char*)Lua_ValueToString(L, 1);

	INT nIndex = PlayerSet.GetFirstPlayer();
	while (nIndex > 0)
	{
		if (strcmpi(Player[nIndex].AccountName, pszName) == 0)
			break;

		nIndex = PlayerSet.GetNextPlayer();
	}
	if (nIndex >= 0 && nIndex < MAX_PLAYER && Player[nIndex].m_nNetConnectIdx >= 0)
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);

	return 0;
}

INT LuaKickOutSelf(Lua_State* L)
{
	INT nIndex = GetPlayerIndex(L);

	if (nIndex <= 0)
		return 0;

	if (Player[nIndex].m_nNetConnectIdx >= 0)
		g_pServer->ShutdownClient(Player[nIndex].m_nNetConnectIdx);
	return 0;
}

INT LuaKillNpc(Lua_State* L)
{
	INT nParamCount = 0;
	if ((nParamCount = Lua_GetTopIndex(L)) < 1) return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	DWORD nNpcID = (DWORD)Lua_ValueToNumber(L, 1);
	INT nNpcIndex = Player[nPlayerIndex].FindAroundNpc(nNpcID);
	if (nNpcIndex <= 0) return 0;
	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
	DamageMagicAttribs[9].nAttribType = magic_physicsdamage_v;
	DamageMagicAttribs[9].nValue[0] = 100000;
	DamageMagicAttribs[9].nValue[2] = 100000;
	Npc[nNpcIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, -1, 0, DamageMagicAttribs, 0, 1, 0);
	return 0;
}

INT LuaKillPlayer(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
	memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
	DamageMagicAttribs[9].nAttribType = magic_physicsdamage_v;
	DamageMagicAttribs[9].nValue[0] = 100000;
	DamageMagicAttribs[9].nValue[2] = 100000;
	Npc[Player[nPlayerIndex].m_nIndex].ReceiveDamage(Player[nPlayerIndex].m_nIndex, -1, 0, DamageMagicAttribs, 0, 1, 0);
	return 0;
}

INT LuaSetFightState(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Npc[Player[nPlayerIndex].m_nIndex].SetFightMode(Lua_ValueToNumber(L, 1) != 0);
	return 0;
}

INT LuaGetFightState(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_FightMode);
	return 1;
}

INT LuaSetLevel(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}

	if (Lua_IsNumber(L, 1))
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 1);
		Player[nPlayerIndex].SetLevel(nValue);
		Lua_PushNumber(L, 1);
	}
	else
		Lua_PushNumber(L, 0);

	return 1;
}

INT LuaSTLevelUp(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 0;
	}

	if (Lua_IsNumber(L, 1))
	{
		INT nValue = (INT)Lua_ValueToNumber(L, 1);
		if (nValue >= 1)
		{
			for (INT i = 1; i <= nValue; i++) {
				Player[nPlayerIndex].LevelUp();
			}
		}
		Lua_PushNumber(L, 1);
	}
	else
		Lua_PushNumber(L, 0);

	return 1;
}


INT LuaGetLevel(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)		Lua_PushNumber(L, 0);

	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_Level);
	return 1;
}
//
INT	LuaUseTownPortal(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	INT nResult = Player[nPlayerIndex].UseTownPortal();
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaReturnFromTownPortal(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	if (Player[nPlayerIndex].BackToTownPortal())
		Lua_PushNumber(L, 1);
	else
		Lua_PushNumber(L, 0);
	return 1;
}
#endif

INT GetPlayerIndex(Lua_State* L)
{
	Lua_GetGlobal(L, SCRIPT_PLAYERINDEX);
	if (lua_isnil(L, Lua_GetTopIndex(L)))
		return -1;
	INT nIndex = (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	if (nIndex >= MAX_PLAYER || nIndex <= 0)
	{
		_ASSERT(0);
		return -1;
	}	if (Player[nIndex].m_nIndex >= MAX_NPC || Player[nIndex].m_nIndex < 0)
	{
		_ASSERT(0);
		return -1;
	}
	return nIndex;
}

INT GetObjIndex(Lua_State* L)
{
	Lua_GetGlobal(L, SCRIPT_OBJINDEX);
	if (lua_isnil(L, Lua_GetTopIndex(L)))
		return -1;
	INT nIndex = (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	if (nIndex >= MAX_OBJECT || nIndex <= 0)
	{
		_ASSERT(0);
		return -1;
	}
	if (Object[nIndex].m_nIndex != nIndex)
	{
		_ASSERT(0);
		return -1;
	}
	return nIndex;
}

INT  LuaMessage(Lua_State* L)
{
	const char* szString;
	szString = lua_tostring(L, 1);
	g_DebugLog((char*)szString);
	return 0;
}
#ifdef _SERVER
//AddStation(N)
INT LuaAddPlayerWayPoint(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nWayPoint = (INT)Lua_ValueToNumber(L, 1);
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	{
		KIndexNode* pNode = (KIndexNode*)Player[nPlayerIndex].m_PlayerWayPointList.GetHead();
		while (pNode)
		{
			if (pNode->m_nIndex == nWayPoint) return 0;
			pNode = (KIndexNode*)pNode->GetNext();
		}

		KIndexNode* pNewNode = new KIndexNode;
		pNewNode->m_nIndex = nWayPoint;
		INT nCount = Player[nPlayerIndex].m_PlayerWayPointList.GetNodeCount();
		for (INT i = 0; i < nCount - 2; i++)
		{
			KIndexNode* pDelNode = (KIndexNode*)Player[nPlayerIndex].m_PlayerWayPointList.RemoveHead();
			delete pDelNode;
		}
		Player[nPlayerIndex].m_PlayerWayPointList.AddTail(pNewNode);
	}
	return 0;
}

INT LuaAddPlayerStation(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nStation = (INT)Lua_ValueToNumber(L, 1);
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	{
		KIndexNode* pNode = (KIndexNode*)Player[nPlayerIndex].m_PlayerStationList.GetHead();
		while (pNode)
		{
			if (pNode->m_nIndex == nStation) return 0;
			pNode = (KIndexNode*)pNode->GetNext();
		}

		KIndexNode* pNewNode = new KIndexNode;
		pNewNode->m_nIndex = nStation;
		Player[nPlayerIndex].m_PlayerStationList.AddTail(pNewNode);
	}
	return 0;
}

INT LuaGetPlayerStationCount(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	Lua_PushNumber(L, Player[nPlayerIndex].m_PlayerStationList.GetNodeCount());
	return 1;
}

//»ñµÃµ±Ç°Íæ¼ÒÓÐÐ§³ÇÊÐÁÐ±íÖÐµÄµÚn¸ö£¨³ýÈ¥µ±Ç°ËùÔÚ³ÇÊÐ£©
INT LuaGetPlayerStation(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nStationId = 0;
	KIndexNode* pNode = (KIndexNode*)Player[nPlayerIndex].m_PlayerStationList.GetHead();
	if (pNode)
	{
		INT nNo = (INT)Lua_ValueToNumber(L, 1);
		INT nCurStation = (INT)Lua_ValueToNumber(L, 2);
		INT nVisitNo = 0;
		while (pNode)
		{
			if (pNode->m_nIndex != nCurStation && g_GetPriceToStation(nCurStation, pNode->m_nIndex) > 0)
			{
				nVisitNo++;
				if (nVisitNo == nNo)
				{
					nStationId = pNode->m_nIndex;
					break;
				}
			}
			pNode = (KIndexNode*)pNode->GetNext();
		}
	}

	Lua_PushNumber(L, nStationId);

	return 1;
}

INT LuaGetPlayerWayPoint(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	KIndexNode* pNode = (KIndexNode*)Player[nPlayerIndex].m_PlayerWayPointList.GetHead();
	if (pNode)
	{
		INT nNo = (INT)Lua_ValueToNumber(L, 1);
		if (nNo > TASKVALUE_MAXWAYPOINT_COUNT)
			Lua_PushNumber(L, 0);
		else
		{
			for (INT i = 0; i < nNo - 1; i++)
			{
				if (pNode == NULL) break;
				pNode = (KIndexNode*)pNode->GetNext();
			}

			if (pNode)
				Lua_PushNumber(L, pNode->m_nIndex);
			else
				Lua_PushNumber(L, 0);

		}
	}
	else
		Lua_PushNumber(L, 0);

	return 1;
}
//¸ù¾Ý´«ÈëµÄ³ÇÊÐÎ¨Ò»id»ñµÃÆäÃû³Æ
INT LuaGetStationName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	if (Lua_GetTopIndex(L) <= 0)
	{
		Lua_PushString(L, "");
		return 1;
	}
	INT nStationId = (INT)Lua_ValueToNumber(L, 1);
	char szName[50];
	g_StationTabFile.GetString(nStationId + 1, "DESC", NORMAL_UNCLEAR_WORD, szName, 50);
	Lua_PushString(L, szName);
	return 1;
}

INT LuaGetWayPointName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	if (Lua_GetTopIndex(L) <= 0)
	{
		Lua_PushString(L, "");
		return 1;
	}
	INT nWayPointId = (INT)Lua_ValueToNumber(L, 1);
	char szName[50];
	g_WayPointTabFile.GetString(nWayPointId + 1, "DESC", NORMAL_UNCLEAR_WORD, szName, 50);
	Lua_PushString(L, szName);
	return 1;
}

//GetCityCount 
INT LuaGetAllStationCount(Lua_State* L)
{
	INT nCityCount = g_StationTabFile.GetHeight() - 1;
	if (nCityCount < 0) nCityCount = 0;
	Lua_PushNumber(L, nCityCount);
	return 1;
}

//cityid, price = GetCity(citynum, curcity)
INT LuaGetCity(Lua_State* L)
{
	return 0;
}


INT LuaGetPriceToWayPoint(Lua_State* L)
{

	INT nCurStation = (INT)Lua_ValueToNumber(L, 1);
	INT nDesWayPoint = (INT)Lua_ValueToNumber(L, 2);
	Lua_PushNumber(L, g_GetPriceToWayPoint(nCurStation, nDesWayPoint));
	return 1;
}

INT LuaGetPriceToStation(Lua_State* L)
{
	INT nCurStation = (INT)Lua_ValueToNumber(L, 1);
	INT nNextStation = (INT)Lua_ValueToNumber(L, 2);
	Lua_PushNumber(L, g_GetPriceToStation(nCurStation, nNextStation));
	return 1;
}

INT LuaGetStationPos(Lua_State* L)
{
	INT nStationId = (INT)Lua_ValueToNumber(L, 1);
	char szPos[100];
	INT nCount = 0;
	INT nRow = g_StationTabFile.FindColumn("COUNT");
	g_StationTabFile.GetInteger(nStationId + 1, nRow, 0, &nCount);
	if (nCount <= 0) return 0;
	INT nRandSect = g_Random(100) % nCount + 1;
	char szSectName[32];
	sprintf(szSectName, "SECT%d", nRandSect);
	char szValue[100];
	nRow = g_StationTabFile.FindColumn(szSectName);
	g_StationTabFile.GetString(nStationId + 1, nRow, "0,0,0", szValue, 100);

	INT nX, nY, nWorld;
	const char* pcszTemp = szValue;

	nWorld = KSG_StringGetInt(&pcszTemp, 0);
	KSG_StringSkipSymbol(&pcszTemp, ',');
	nX = KSG_StringGetInt(&pcszTemp, 0);
	KSG_StringSkipSymbol(&pcszTemp, ',');
	nY = KSG_StringGetInt(&pcszTemp, 0);
	//sscanf(szValue, "%d,%d,%d", &nWorld, &nX, &nY);

	Lua_PushNumber(L, nWorld);
	Lua_PushNumber(L, nX);
	Lua_PushNumber(L, nY);
	return 3;
}

INT LuaGetWayPointPos(Lua_State* L)
{
	INT nWayPointId = (INT)Lua_ValueToNumber(L, 1);
	char szPos[100];
	INT nCount = 0;
	char szValue[30];
	INT nRow;
	nRow = g_WayPointTabFile.FindColumn("SECT");
	g_WayPointTabFile.GetString(nWayPointId + 1, nRow, "0,0,0", szValue, 30);
	INT nX, nY, nWorld;
	const char* pcszTemp = szValue;

	nWorld = KSG_StringGetInt(&pcszTemp, 0);
	KSG_StringSkipSymbol(&pcszTemp, ',');
	nX = KSG_StringGetInt(&pcszTemp, 0);
	KSG_StringSkipSymbol(&pcszTemp, ',');
	nY = KSG_StringGetInt(&pcszTemp, 0);
	//sscanf(szValue, "%d,%d,%d", &nWorld, &nX, &nY);

	Lua_PushNumber(L, nWorld);
	Lua_PushNumber(L, nX);
	Lua_PushNumber(L, nY);
	return 3;
}

INT LuaGetWayPointFight(Lua_State* L)
{
	INT nWayPointId = (INT)Lua_ValueToNumber(L, 1);
	INT nFight;
	INT nRow;
	nRow = g_WayPointTabFile.FindColumn("FightState");
	g_WayPointTabFile.GetInteger(nWayPointId + 1, nRow, 0, &nFight);

	Lua_PushNumber(L, nFight);
	return 1;
}

INT LuaGetRank(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_RankID);
	return 1;
}

INT LuaSetRank(Lua_State* L)
{
	BYTE nRankID = (BYTE)Lua_ValueToNumber(L, 1);
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	Npc[Player[nPlayerIndex].m_nIndex].m_RankID = nRankID;
	return 1;
}

INT LuaGetExpandRank(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	Lua_PushString(L, Npc[Player[nPlayerIndex].m_nIndex].m_CurExpandRank.szName);
	return 1;
}

INT LuaSetExpandRank(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	KExpandRank tmp;
	tmp.Release();

	strcpy(tmp.szName, (char*)Lua_ValueToString(L, 1));
	if (nParamNum > 2)
		tmp.dwColor = TGetColor((char*)Lua_ValueToString(L, 2));
	if (nParamNum > 3)
		tmp.nStateGraphics = (INT)Lua_ValueToNumber(L, 3);
	if (nParamNum > 4)
		tmp.dwLeftTime = (DWORD)Lua_ValueToNumber(L, 4);

	Npc[Player[nPlayerIndex].m_nIndex].SetExpandRank(&tmp);
	return 1;
}

INT LuaRestoreExpandRank(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	Npc[Player[nPlayerIndex].m_nIndex].SetExpandRank(&Npc[Player[nPlayerIndex].m_nIndex].m_ExpandRank);
	return 1;
}

INT LuaGetEquipItemEx(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_dwEquipExpandTime - KSG_GetCurSec() > 0)
		Lua_PushNumber(L, Player[nPlayerIndex].m_dwEquipExpandTime);
	else
		Lua_PushNumber(L, 0);
	return 1;
}

INT LuaSetEquipItemEx(Lua_State* L)
{
	INT nValue = (INT)Lua_ValueToNumber(L, 1);
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	Player[nPlayerIndex].SetEquipExpandTime((INT)Lua_ValueToNumber(L, 1));
	return 0;
}

INT LuaGetExpandBox(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	Lua_PushNumber(L, Player[nPlayerIndex].m_btRepositoryNum);
	return 1;
}

INT LuaSetExpandBox(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;

	Player[nPlayerIndex].SetExpandBoxNum((INT)Lua_ValueToNumber(L, 1));
	return 0;
}



INT LuaSetObjPropState(Lua_State* L)
{
	INT  nParamNum = (INT)Lua_GetTopIndex(L);
	INT nState = 1;

	if (nParamNum >= 1)
	{
		nState = (INT)Lua_ValueToNumber(L, 1);
		nState = (nState == 0) ? 0 : 1;
	}

	INT nIndex = 0;
	if ((nIndex = GetObjIndex(L)) < 0)
		return 0;

	Object[nIndex].SetState(nState);
	return 0;
}


INT	LuaGetServerName(Lua_State* L)
{
	char szServerName[100];
	ULONG   stServerNameLen = 100;

#ifndef __linux
	if (GetComputerName(szServerName, &stServerNameLen))
	{
		Lua_PushString(L, szServerName);
	}
	else
#else
	if (SOCKET_ERROR != gethostname(szServerName, sizeof(szServerName)))
	{
		Lua_PushString(L, szServerName);
	}
	else
#endif
		Lua_PushString(L, "");

	return 1;
}

//GetWharfCount(nDock)
INT LuaGetDockCount(Lua_State* L)
{
	INT nCount = 0;
	INT nCurStation = 0;
	INT nTotalCount = 0;
	INT i = 0;
	if (Lua_GetTopIndex(L) < 1)
	{
		goto DockCount;
	}

	nCurStation = (INT)Lua_ValueToNumber(L, 1);
	nTotalCount = g_DockPriceTabFile.GetHeight() - 1;

	for (i = 0; i < nTotalCount; i++)
	{
		INT nPrice = g_GetPriceToDock(nCurStation, i + 1);
		if (nPrice > 0) nCount++;
	}

DockCount:
	Lua_PushNumber(L, nCount);
	return 1;
}

INT LuaGetDockPrice(Lua_State* L)
{
	INT nCurDock = (INT)Lua_ValueToNumber(L, 1);
	INT nDesDock = (INT)Lua_ValueToNumber(L, 2);
	Lua_PushNumber(L, g_GetPriceToDock(nCurDock, nDesDock));
	return 1;
}

INT LuaGetDock(Lua_State* L)
{
	INT nCurDock = (INT)Lua_ValueToNumber(L, 1);
	INT nDock = (INT)Lua_ValueToNumber(L, 2);
	INT nCount = 0;
	INT nTotalCount = g_DockPriceTabFile.GetHeight() - 1;
	INT nGetDock = 0;

	for (INT i = 0; i < nTotalCount; i++)
	{
		INT nPrice = g_GetPriceToDock(nCurDock, i + 1);
		if (nPrice > 0)
		{
			nCount++;
			if (nCount == nDock)
			{
				nGetDock = i + 1;
				break;
			}
		}
	}
	Lua_PushNumber(L, nGetDock);
	return 1;
}

INT LuaGetDockName(Lua_State* L)
{
	INT nDock = (INT)Lua_ValueToNumber(L, 1);
	char szName[100];

	if (nDock > g_DockPriceTabFile.GetHeight() - 1)
	{
		strcpy(szName, "Î´ÖªÂëÍ·");
		goto DockName;
	}

	g_DockTabFile.GetString(nDock + 1, "DESC", "Î´ÖªÂëÍ·", szName, 100);

DockName:
	Lua_PushString(L, szName);
	return 1;
}

INT LuaGetDockPos(Lua_State* L)
{
	INT nDock = (INT)Lua_ValueToNumber(L, 1);
	if (nDock > g_DockTabFile.GetHeight() - 1)
	{
		printf("GetWharfPos Script Is Error!");
		return 0;
	}

	char szPos[100];
	INT nCount = 0;
	INT nRow = g_DockTabFile.FindColumn("COUNT");
	g_DockTabFile.GetInteger(nDock + 1, nRow, 0, &nCount);
	if (nCount <= 0) return 0;
	INT nRandSect = g_Random(100) % nCount + 1;
	char szSectName[32];
	sprintf(szSectName, "SECT%d", nRandSect);
	char szValue[100];
	nRow = g_DockTabFile.FindColumn(szSectName);
	g_DockTabFile.GetString(nDock + 1, nRow, "0,0,0", szValue, 100);
	INT nX, nY, nWorld;
	const char* pcszTemp = szValue;

	nWorld = KSG_StringGetInt(&pcszTemp, 0);
	KSG_StringSkipSymbol(&pcszTemp, ',');
	nX = KSG_StringGetInt(&pcszTemp, 0);
	KSG_StringSkipSymbol(&pcszTemp, ',');
	nY = KSG_StringGetInt(&pcszTemp, 0);
	//sscanf(szValue, "%d,%d,%d", &nWorld, &nX, &nY);

	Lua_PushNumber(L, nWorld);
	Lua_PushNumber(L, nX);
	Lua_PushNumber(L, nY);
	return 3;
}

INT LuaGetWayPointFightState(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0) return 0;
	INT nFightState = 0;
	if (Lua_GetTopIndex(L) > 0)
	{
		INT nWayPointId = (INT)Lua_ValueToNumber(L, 1);
		g_WayPointTabFile.GetInteger(nWayPointId + 1, "FightState", 0, &nFightState);
	}
	Lua_PushNumber(L, nFightState);
	return 1;
}

// SetMission(valueid, value)
INT LuaSetMission(Lua_State* L)
{
	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0)
		return 0;

	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;

	INT nValueId = (INT)Lua_ValueToNumber(L, 1);
	char* szValue = (char*)Lua_ValueToString(L, 2);

	if (nValueId < 0)
		return 0;

	SubWorld[nSubWorldIndex].m_MissionArray.SetMission(nValueId, szValue);
	return 0;
}

INT LuaGetMissionValue(Lua_State* L)
{
	INT nResultValue = 0;
	INT nSubWorldIndex = -1;
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1)
		goto lab_getmissionvalue;

	nSubWorldIndex = GetSubWorldIndex(L);
	//printf("nSubWorldIndex: %d\n", nSubWorldIndex);

	if (nSubWorldIndex >= 0)
	{
		INT  nValueId = (INT)Lua_ValueToNumber(L, 1);

		//printf("nValueId: %d\n", nValueId);
		if (nValueId >= 0)
			nResultValue = SubWorld[nSubWorldIndex].m_MissionArray.GetMissionValue(nValueId);

		//printf("nResultValue: %d\n", nResultValue);
	}

lab_getmissionvalue:
	Lua_PushNumber(L, nResultValue);
	return 1;
}

INT LuaGetMissionString(Lua_State* L)
{
	INT nSubWorldIndex = -1;
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1)
		goto lab_getmissionstring;

	nSubWorldIndex = GetSubWorldIndex(L);

	if (nSubWorldIndex >= 0)
	{
		INT  nValueId = (INT)Lua_ValueToNumber(L, 1);
		if (nValueId >= 0)
		{
			Lua_PushString(L, SubWorld[nSubWorldIndex].m_MissionArray.GetMissionString(nValueId));
			return 1;
		}
	}

lab_getmissionstring:
	Lua_PushNil(L);
	return 1;
}

// SetMissionValue(mapid/mapname, valueid, value)
INT LuaSetGlobalMission(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;

	INT nValueId = (INT)Lua_ValueToNumber(L, 1);
	char* szValue = (char*)Lua_ValueToString(L, 2);

	if (nValueId < 0)
		return 0;
	g_GlobalMissionArray.SetMission(nValueId, szValue);
	return 0;
}

INT LuaGetGlobalMissionValue(Lua_State* L)
{
	INT nResultValue = 0;
	INT nValueId = 0;
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1)
		goto lab_getglobalmissionvalue;
	nValueId = (INT)Lua_ValueToNumber(L, 1);
	if (nValueId < 0)
		goto lab_getglobalmissionvalue;

	nResultValue = g_GlobalMissionArray.GetMissionValue(nValueId);

lab_getglobalmissionvalue:
	Lua_PushNumber(L, nResultValue);
	return 1;
}

INT LuaGetGlobalMissionString(Lua_State* L)
{
	INT nValueId = 0;
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1)
		goto lab_getglobalmissionstring;
	nValueId = (INT)Lua_ValueToNumber(L, 1);
	if (nValueId < 0)
		goto lab_getglobalmissionstring;

	Lua_PushString(L, g_GlobalMissionArray.GetMissionString(nValueId));

lab_getglobalmissionstring:
	Lua_PushNil(L);
	return 1;
}

INT LuaGetMissionName(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0)
		return 0;

	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);

	if (pMission)
	{
		Lua_PushString(L, pMission->GetMissionName());
		return 1;
	}
	return 0;
}


//StartMission(missionid)
INT LuaInitMission(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0)
		return 0;

	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
	{
		_ASSERT(0);
		return 0;
	}

	pMission = SubWorld[nSubWorldIndex].m_MissionArray.Add();
	if (pMission)
	{
		pMission->m_MissionPlayer.Clear();
		pMission->m_MissionNpc.Clear();
		pMission->SetMissionId(nMissionId);
		if (Lua_GetTopIndex(L) > 4)
		{
			INT nParam[MAX_GLBMISSION_PARAM];
			for (INT i = 0; i < MAX_GLBMISSION_PARAM; i++)
				nParam[i] = (INT)Lua_ValueToNumber(L, 4 + i);
			pMission->SetMissionLadder((char*)Lua_ValueToString(L, 2), (INT)Lua_ValueToNumber(L, 3), nParam);
		}
		char szScript[MAX_PATH];
		sprintf(szScript, MISSIONTASK_SCRIPTFILE, nMissionId);
		if (szScript[0])
		{
			KLuaScript* pScript = (KLuaScript*)g_GetScript(szScript);
			Lua_PushNumber(pScript->m_LuaState, nSubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			pScript->CallFunction("InitMission", 0, "d", nMissionId);
		}
	}
	return 1;
}

INT LuaRunMission(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0)
		return 0;

	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);

	if (pMission)
	{
		char szScript[MAX_PATH];
		sprintf(szScript, MISSIONTASK_SCRIPTFILE, nMissionId);
		if (szScript[0])
		{
			KLuaScript* pScript = (KLuaScript*)g_GetScript(szScript);
			Lua_PushNumber(pScript->m_LuaState, nSubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			pScript->CallFunction("RunMission", 0, "d", nMissionId);
		}
	}
	return 1;
}


INT LuaIsMission(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0)
		return 0;

	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
		Lua_PushNumber(L, 1);
	else
		Lua_PushNumber(L, 0);
	return 1;
}


INT LuaGetMSLadder(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;

	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0)
		return 0;
	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
	{
		INT nOrdinal = (INT)Lua_ValueToNumber(L, 2);
		if (nOrdinal >= 0 && nOrdinal < MISSION_STATNUM)
		{
			Lua_PushString(L, pMission->m_MissionLadder[nOrdinal].Name);
			Lua_PushNumber(L, pMission->m_MissionLadder[nOrdinal].ucGroup);
			Lua_PushNumber(L, pMission->m_MissionLadder[nOrdinal].nParam[pMission->GetMissionLadderParam()]);
			return 3;
		}

	}
	return 1;
}


INT LuaGetMSTop10(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1)
		return 0;

	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0)
		return 0;
	KMission Mission;
	Mission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
	if (pMission)
	{
		for (INT nOrdinal = 0; nOrdinal < MISSION_STATNUM; nOrdinal++)
		{
			Lua_PushString(L, pMission->m_MissionLadder[nOrdinal].Name);
		}

	}
	return 1;
}


//CloseMission(missionId)
INT LuaCloseMission(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nMissionId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex < 0)
		return 0;
	KMission StopMission;
	StopMission.SetMissionId(nMissionId);
	KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&StopMission);
	if (pMission)
	{
		char szScript[MAX_PATH];
		sprintf(szScript, MISSIONTASK_SCRIPTFILE, nMissionId);
		if (szScript[0])
		{
			KLuaScript* pScript = (KLuaScript*)g_GetScript(szScript);
			Lua_PushNumber(pScript->m_LuaState, nSubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			pScript->CallFunction("EndMission", 0, "d", nMissionId);
		}
		pMission->StopMission();
		SubWorld[nSubWorldIndex].m_MissionArray.Remove(pMission);

	}
	return 1;
}

//StopMissionTimer(missionid, timerid)
INT LuaStopMissionTimer(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;
	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	INT nTimerId = (INT)Lua_ValueToNumber(L, 2);
	INT nSubWorldIndex = GetSubWorldIndex(L);

	if (nMissionId < 0 || nTimerId < 0)
		return 0;

	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			KTimerTaskFun StopTimer;
			StopTimer.SetTimer(1, nTimerId);
			KTimerTaskFun* pTimer = pMission->m_cTimerTaskSet.GetData(&StopTimer);
			if (pTimer)
			{
				pTimer->CloseTimer();
				pMission->m_cTimerTaskSet.Remove(pTimer);
			}
		}

	}

	return 1;
}


//StartMissionTimer(missionid, timerid, time)
INT LuaStartMissionTimer(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 3)
		return 0;
	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	INT nTimerId = (INT)Lua_ValueToNumber(L, 2);
	INT nTimeInterval = (INT)Lua_ValueToNumber(L, 3);
	INT nSubWorldIndex = GetSubWorldIndex(L);

	if (nMissionId < 0 || nTimerId < 0 || nTimeInterval < 0)
		return 0;

	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			KTimerTaskFun* pTimer = pMission->m_cTimerTaskSet.Add();
			if (pTimer)
			{
				pTimer->SetTimer(nTimeInterval, nTimerId);
			}
		}

	}
	return 1;
}


//SetTempRev(worldid, x, y)
INT LuaSetDeathRevivalPos(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
		return 0;
	INT nParamCount = Lua_GetTopIndex(L);

	PLAYER_REVIVAL_POS* pTempRev = Player[nPlayerIndex].GetDeathRevivalPos();

	if (nParamCount > 2)
	{
		pTempRev->m_nSubWorldID = (INT)Lua_ValueToNumber(L, 1);
		pTempRev->m_nMpsX = (INT)Lua_ValueToNumber(L, 2);
		pTempRev->m_nMpsY = (INT)Lua_ValueToNumber(L, 3);
	}
	else if (nParamCount == 1)
	{
		pTempRev->m_nSubWorldID = SubWorld[Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex].m_SubWorldID;
		POINT Pos;
		INT nRevId = (INT)Lua_ValueToNumber(L, 1);
		g_SubWorldSet.GetRevivalPosFromId(pTempRev->m_nSubWorldID, nRevId, &Pos);
		pTempRev->m_ReviveID = nRevId;
		pTempRev->m_nMpsX = Pos.x;
		pTempRev->m_nMpsY = Pos.y;
	}
	else
	{
		return 0;
	}

	return 1;
}

INT LuaPlayerExecuteRevive(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
		return 0;
	INT nParamCount = Lua_GetTopIndex(L);

	INT nReviveType = REMOTE_REVIVE_TYPE;

	if (nParamCount > 1)
		nReviveType = (INT)Lua_ValueToNumber(L, 1);

	Player[nPlayerIndex].Revive(nReviveType);
	return 1;
}

//AddMSPlayer(MissionId, PlayerIndex, groupid); / AddMSPlayer(MissionId, groupid)
INT LuaAddMissionPlayer(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;
	INT nMissionId = 0;
	INT nPlayerIndex = 0;
	INT nGroupId = 0;
	if (nParamCount >= 3)
	{
		nMissionId = (INT)Lua_ValueToNumber(L, 1);
		nPlayerIndex = (INT)Lua_ValueToNumber(L, 2);
		nGroupId = (INT)Lua_ValueToNumber(L, 3);
	}
	else
	{
		nMissionId = (INT)Lua_ValueToNumber(L, 1);
		nGroupId = (INT)Lua_ValueToNumber(L, 2);
		nPlayerIndex = GetPlayerIndex(L);
	}

	if (nMissionId < 0 || nPlayerIndex <= 0 || nGroupId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			INT nPlayerDataIdx = pMission->AddPlayer(nPlayerIndex, Player[nPlayerIndex].m_dwID, nGroupId);
			Lua_PushNumber(L, nPlayerDataIdx);
			return 1;
		}
	}
	return 1;
}


INT LuaAddMissionNpc(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;

	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	INT	nNpcIndex = (INT)Lua_ValueToNumber(L, 2);
	INT nGroupId = 0;
	if (nParamCount >= 3)
		nGroupId = (INT)Lua_ValueToNumber(L, 3);

	if (nMissionId < 0 || nNpcIndex <= 0 || nGroupId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			INT nNpcDataIdx = pMission->AddNpc(nNpcIndex, Npc[nNpcIndex].m_dwID, nGroupId);
			Lua_PushNumber(L, nNpcDataIdx);
			return 1;
		}
	}
	return 1;
}

INT LuaSetMissionGroup(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;

	Npc[Player[nPlayerIndex].m_nIndex].m_nMissionGroup = (INT)Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].SendMSGroup();
	return 1;
}

INT LuaRevivalAllNpc(Lua_State* L)
{
	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		SubWorld[nSubWorldIndex].RevivalAllNpc();
	}
	return 1;
}

//DelMSPlayer(MissionId, PlayerIndex)
INT LuaRemoveMissionPlayer(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;
	INT nMissionId = 0;
	INT nPlayerIndex = 0;
	INT nGroupId = 0;
	if (nParamCount >= 2)
	{
		nMissionId = (INT)Lua_ValueToNumber(L, 1);
		nPlayerIndex = (INT)Lua_ValueToNumber(L, 2);
	}
	else
	{
		nMissionId = (INT)Lua_ValueToNumber(L, 1);
		nPlayerIndex = GetPlayerIndex(L);
	}

	if (nMissionId < 0 || nPlayerIndex <= 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->RemovePlayer(nPlayerIndex, Player[nPlayerIndex].m_dwID);
		}
	}
	return 1;
}


INT LuaRemoveMissionNpc(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;
	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	INT	nNpcIndex = (INT)Lua_ValueToNumber(L, 2);

	if (nMissionId < 0 || nNpcIndex <= 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			pMission->RemoveNpc(nNpcIndex);
		}
	}
	return 1;
}

//MSMsg2Group(missionid, string , group)
INT LuaMissionMsg2Group(Lua_State* L)
{
	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	char* strMsg = (char*)Lua_ValueToString(L, 2);
	INT	nGroupId = (INT)Lua_ValueToNumber(L, 3);

	if (nMissionId < 0 || !strMsg || nGroupId < 0)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			INT nChannelID = -1;
			if (Lua_GetTopIndex(L) > 4)
				nChannelID = (INT)Lua_ValueToNumber(L, 4);
			pMission->Msg2Group(strMsg, nGroupId, nChannelID);
		}
	}

	return 1;
}


//MSMsg2Group(missionid, string)
INT LuaMissionMsg2All(Lua_State* L)
{
	INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
	char* strMsg = (char*)Lua_ValueToString(L, 2);

	if (nMissionId < 0 || !strMsg)
		return 0;

	INT nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			INT nChannelID = -1;
			if (Lua_GetTopIndex(L) > 3)
				nChannelID = (INT)Lua_ValueToNumber(L, 3);
			pMission->Msg2All(strMsg, nChannelID);
		}
	}

	return 1;
}


INT LuaMissionPlayerCount(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	ULONG ulCount = 0;
	INT nMissionId = 0;
	INT nGroupId = -1;
	INT nSubWorldIndex = 0;
	if (nParamCount < 1)
		goto lab_getmissionplayercount;

	nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nParamCount > 1)
		nGroupId = (INT)Lua_ValueToNumber(L, 2);

	if (nMissionId < 0)
		goto lab_getmissionplayercount;

	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			if (nGroupId >= 0)
				ulCount = pMission->GetGroupPlayerCount(nGroupId);
			else
				ulCount = pMission->GetPlayerCount();
		}
	}

lab_getmissionplayercount:
	Lua_PushNumber(L, ulCount);
	return 1;
}


INT LuaMissionNpcCount(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	ULONG ulCount = 0;
	INT nMissionId = 0;
	INT nGroupId = -1;
	INT nSubWorldIndex = 0;
	if (nParamCount < 1)
		goto lab_getmissionnpccount;

	nMissionId = (INT)Lua_ValueToNumber(L, 1);
	if (nParamCount > 1)
		nGroupId = (INT)Lua_ValueToNumber(L, 2);

	if (nMissionId < 0)
		goto lab_getmissionnpccount;

	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			if (nGroupId >= 0)
				ulCount = pMission->GetGroupNpcCount(nGroupId);
			else
				ulCount = pMission->GetNpcCount();
		}
	}

lab_getmissionnpccount:
	Lua_PushNumber(L, ulCount);
	return 1;
}


INT LuaSetPlayerDeathScript(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	char* szScript = (char*)Lua_ValueToString(L, 1);
	Player[nPlayerIndex].m_dwDeathScriptId = g_FileName2Id(szScript);
	return 1;
}

INT LuaSetPlayerDamageScript(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	char* szScript = (char*)Lua_ValueToString(L, 1);
	Player[nPlayerIndex].m_dwDamageScriptId = g_FileName2Id(szScript);
	return 1;
}

INT LuaNpcIndexToPlayerIndex(Lua_State* L)
{
	INT nResult = 0;
	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
		goto lab_npcindextoplayerindex;

	if (Npc[nNpcIndex].m_Index > 0 && Npc[nNpcIndex].IsPlayer())
	{
		if (Npc[nNpcIndex].GetPlayerIdx() > 0)
			nResult = Npc[nNpcIndex].GetPlayerIdx();
	}

lab_npcindextoplayerindex:
	Lua_PushNumber(L, nResult);
	return 1;

}


INT LuaGetMissionPlayer_PlayerIndex(Lua_State* L)
{
	ULONG nResult = 0;
	INT nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2)
		goto lab_getmissionplayer_npcindex;

	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
		INT nDataIndex = (INT)Lua_ValueToNumber(L, 2);
		if (nMissionId < 0 || nDataIndex < 0)
			goto lab_getmissionplayer_npcindex;

		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_PlayerIndex(nDataIndex);
		}
	}

lab_getmissionplayer_npcindex:
	Lua_PushNumber(L, nResult);
	return 1;
}


INT LuaGetMissionPlayer_DataIndex(Lua_State* L)
{
	ULONG nResult = 0;
	INT nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2)
		goto lab_getmissionplayer_dataindex;

	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
		INT nPlayerIndex = (INT)Lua_ValueToNumber(L, 2);
		if (nMissionId < 0 || nPlayerIndex < 0)
			goto lab_getmissionplayer_dataindex;

		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_DataIndex(nPlayerIndex, Player[nPlayerIndex].m_dwID);
		}
	}

lab_getmissionplayer_dataindex:
	Lua_PushNumber(L, nResult);
	return 1;
}

//SetMPParam(missionid, nDidx, vid, v)
INT LuaSetMissionPlayerParam(Lua_State* L)
{
	INT nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 4)
		return 0;

	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
		INT nDataIndex = (INT)Lua_ValueToNumber(L, 2);
		INT nParamId = (INT)Lua_ValueToNumber(L, 3);
		INT nValue = (INT)Lua_ValueToNumber(L, 4);

		if (nMissionId < 0 || nDataIndex < 0 || nParamId > MAX_MISSION_PARAM)
			return 0;

		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			//pMission->m_MissionPlayer.SetParam(nDataIndex, nParamId, nValue);
			pMission->SetPlayerParam(nDataIndex, nParamId, nValue);
		}
	}
	return 1;
}

INT LuaGetMissionPlayerParam(Lua_State* L)
{
	INT nResult = 0;
	INT nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 3)
		goto lab_getmissionplayerparam;

	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
		INT nDataIndex = (INT)Lua_ValueToNumber(L, 2);
		INT nParamId = (INT)Lua_ValueToNumber(L, 3);

		if (nMissionId < 0 || nDataIndex < 0 || nParamId > MAX_MISSION_PARAM)
			goto lab_getmissionplayerparam;

		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->m_MissionPlayer.GetParam(nDataIndex, nParamId);
		}
	}
lab_getmissionplayerparam:
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaGetPlayerMissionGroup(Lua_State* L)
{
	INT nResult = 0;
	INT nSubWorldIndex = 0;
	if (Lua_GetTopIndex(L) < 2)
		goto lab_getmissionplayergroup;

	nSubWorldIndex = GetSubWorldIndex(L);
	if (nSubWorldIndex >= 0)
	{
		INT nMissionId = (INT)Lua_ValueToNumber(L, 1);
		INT nNpcIndex = (INT)Lua_ValueToNumber(L, 2);

		if (nMissionId < 0 || nNpcIndex < 0)
			goto lab_getmissionplayergroup;

		KMission Mission;
		Mission.SetMissionId(nMissionId);
		KMission* pMission = SubWorld[nSubWorldIndex].m_MissionArray.GetData(&Mission);
		if (pMission)
		{
			nResult = pMission->GetMissionPlayer_GroupId(nNpcIndex);
		}
	}
lab_getmissionplayergroup:
	Lua_PushNumber(L, nResult);
	return 1;
}

//SetLogoutRV(type)
INT LuaSetPlayerRevivalOptionWhenLogout(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	INT nType = (INT)Lua_ValueToNumber(L, 1);

	if (nType)
		Player[nPlayerIndex].SetLoginType(1);
	else
		Player[nPlayerIndex].SetLoginType(0);

	return 1;
}

INT LuaSetPlayerPKValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nPKValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cPK.SetPKValue(nPKValue);
	return 1;
}

INT LuaGetPlayerPKValue(Lua_State* L)
{
	INT nPKValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayerpkvalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayerpkvalue;
	nPKValue = Player[nPlayerIndex].m_cPK.GetPKValue();

lab_getplayerpkvalue:
	Lua_PushNumber(L, nPKValue);
	return 1;
}

INT LuaGetGameTime(Lua_State* L)
{
	Lua_PushNumber(L, g_SubWorldSet.GetGameTime());
	return 1;
}

INT LuaGetPlayerLoginTime(Lua_State* L)
{
	INT nResult = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayerlogintime;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayerlogintime;
	nResult = Player[nPlayerIndex].m_dwLoginTime;

lab_getplayerlogintime:
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaGetPlayerOnlineTime(Lua_State* L)
{
	INT nResult = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayeronlinetime;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayeronlinetime;
	nResult = g_SubWorldSet.GetGameTime() - Player[nPlayerIndex].m_dwLoginTime;

lab_getplayeronlinetime:
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaOfflineLive(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	//Player[nPlayerIndex].m_bPlayerOffline = TRUE;
	Player[nPlayerIndex].m_byLixian = LIXIAN_REQUEST;

	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_lpBuf = 0;
	//sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_OFFLINE;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_EXIT;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	return 1;
}

INT LuaSetValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;
	INT nValueIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nValue = (INT)Lua_ValueToNumber(L, 2);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;

	if (nValueIndex < 0 || nValueIndex >= MAX_STATTASK)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(nValueIndex, nValue);
	return 1;
}

INT LuaAddValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;
	INT nValueIndex = (INT)Lua_ValueToNumber(L, 1);
	INT nValue = (INT)Lua_ValueToNumber(L, 2);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;

	if (nValueIndex < 0 || nValueIndex >= MAX_STATTASK)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(nValueIndex,
		Player[nPlayerIndex].m_cTask.GetSaveVal(nValueIndex) + nValue);
	return 1;
}

INT LuaGetValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayervalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayervalue;
	nValue = Player[nPlayerIndex].m_cTask.GetSaveVal((INT)Lua_ValueToNumber(L, 1));

lab_getplayervalue:
	Lua_PushNumber(L, nValue);
	return 1;
}

INT LuaSetPlayerReputeValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nReputeValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_STATTASK_REPUTE, nReputeValue);
	return 1;
}

INT LuaAddPlayerReputeValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nReputeValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_STATTASK_REPUTE,
		Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_REPUTE) + nReputeValue);
	return 1;
}

INT LuaGetPlayerReputeValue(Lua_State* L)
{
	INT nReputeValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayerreputevalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayerreputevalue;
	nReputeValue = Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_REPUTE);

lab_getplayerreputevalue:
	Lua_PushNumber(L, nReputeValue);
	return 1;
}

INT LuaSetPlayerFuYuanValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nFuYuanValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_STATTASK_FUYUAN, nFuYuanValue);
	return 1;
}

INT LuaAddPlayerFuYuanValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nFuYuanValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_STATTASK_FUYUAN,
		Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_FUYUAN) + nFuYuanValue);
	return 1;
}

INT LuaGetPlayerFuYuanValue(Lua_State* L)
{
	INT nFuYuanValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayerfuyuanvalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayerfuyuanvalue;
	nFuYuanValue = Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_FUYUAN);

lab_getplayerfuyuanvalue:
	Lua_PushNumber(L, nFuYuanValue);
	return 1;
}

INT LuaAddPlayerAccumValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nAccumValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_STATTASK_ACCUM,
		Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_ACCUM) + nAccumValue);
	return 1;
}

INT LuaGetPlayerAccumValue(Lua_State* L)
{
	INT nAccumValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayeraccumvalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayeraccumvalue;
	nAccumValue = Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_ACCUM);

lab_getplayeraccumvalue:
	Lua_PushNumber(L, nAccumValue);
	return 1;
}

INT LuaAddPlayerHonorValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nHonorValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_STATTASK_HONOR,
		Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_HONOR) + nHonorValue);
	return 1;
}

INT LuaGetPlayerHonorValue(Lua_State* L)
{
	INT nHonorValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayerhonorvalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayerhonorvalue;
	nHonorValue = Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_HONOR);

lab_getplayerhonorvalue:
	Lua_PushNumber(L, nHonorValue);
	return 1;
}

INT LuaAddPlayerRespectValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nRespectValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_STATTASK_RESPECT,
		Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_RESPECT) + nRespectValue);
	return 1;
}

INT LuaGetPlayerRespectValue(Lua_State* L)
{
	INT nRespectValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayerrespectvalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayerrespectvalue;
	nRespectValue = Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_RESPECT);

lab_getplayerrespectvalue:
	Lua_PushNumber(L, nRespectValue);
	return 1;
}

INT LuaAddPlayerTranslifeValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nTranslifeValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Npc[Player[nPlayerIndex].m_nIndex].m_byTranslife += nTranslifeValue;
	return 1;
}

INT LuaSetPlayerTranslifeValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nTranslifeValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Npc[Player[nPlayerIndex].m_nIndex].m_byTranslife = nTranslifeValue;
	return 1;
}

INT LuaGetPlayerTranslifeValue(Lua_State* L)
{
	INT nTranslifeValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayertranslifevalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayertranslifevalue;
	nTranslifeValue = Npc[Player[nPlayerIndex].m_nIndex].m_byTranslife;

lab_getplayertranslifevalue:
	Lua_PushNumber(L, nTranslifeValue);
	return 1;
}

INT LuaAddPlayerViprankValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	INT nViprankValue = (INT)Lua_ValueToNumber(L, 1);

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_STATTASK_VIPRANK,
		Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_VIPRANK) + nViprankValue);
	return 1;
}

INT LuaGetPlayerViprankValue(Lua_State* L)
{
	INT nViprankValue = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getplayerviprankvalue;

	if (Player[nPlayerIndex].m_nIndex <= 0)
		goto lab_getplayerviprankvalue;
	nViprankValue = Player[nPlayerIndex].m_cTask.GetSaveVal(TASKVALUE_STATTASK_VIPRANK);

lab_getplayerviprankvalue:
	Lua_PushNumber(L, nViprankValue);
	return 1;
}

INT	LuaGetCurNpcIndex(Lua_State* L)
{
	INT nNpcIndex = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getcurnpcindex;

lab_getcurnpcindex:
	Lua_PushNumber(L, Player[nPlayerIndex].m_nIndex);
	return 1;
}

//showladder(count, ladderid1, ladderid2.....)
INT LuaShowLadder(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;
	INT nLadderCount = (DWORD)Lua_ValueToNumber(L, 1);
	if (nLadderCount <= 0)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	if (nLadderCount > nParamCount - 1)
		nLadderCount = nParamCount - 1;
	BYTE Buffer[sizeof(LADDER_LIST) + 50 * sizeof(DWORD)];
	LADDER_LIST* pLadderList = (LADDER_LIST*)&Buffer;
	pLadderList->ProtocolType = s2c_ladderlist;
	pLadderList->nCount = nLadderCount;
	pLadderList->wSize = sizeof(LADDER_LIST) + nLadderCount * sizeof(DWORD) - 1;
	for (INT i = 0; i < nLadderCount; i++)
	{
		pLadderList->dwLadderID[i] = (DWORD)Lua_ValueToNumber(L, i + 2);
	}
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &Buffer, pLadderList->wSize + 1);
	return 1;

}

INT LuaGetLadder(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;

	void* pData = (void*)Ladder.GetTopTen((DWORD)Lua_ValueToNumber(L, 1));
	if (pData)
	{
		TRoleList	StatData[10];
		memcpy(StatData, pData, sizeof(StatData));
		INT nOrdinal = (INT)Lua_ValueToNumber(L, 2);
		if (nOrdinal >= 0 && nOrdinal < 10)
		{
			Lua_PushString(L, StatData[nOrdinal].Name);
			Lua_PushNumber(L, StatData[nOrdinal].nValue);
			Lua_PushNumber(L, StatData[nOrdinal].bySort);
			return 3;
		}
	}
	return 1;
}

INT LuaSwearBrother(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nTeamID = Lua_ValueToNumber(L, 1);

	if (nTeamID >= MAX_TEAM || nTeamID < 0)
		return 0;

	KPlayerChat::STRINGLIST BrotherList;
	_ASSERT(g_Team[nTeamID].m_nCaptain > 0);

	std::string strCapName;
	strCapName = Npc[Player[g_Team[nTeamID].m_nCaptain].m_nIndex].Name;
	BrotherList.push_back(strCapName);

	for (INT i = 0; i < g_Team[nTeamID].m_nMemNum; i++)
	{
		INT nPlayerIndex = g_Team[nTeamID].m_nMember[i];
		if (nPlayerIndex > 0 && nPlayerIndex < MAX_PLAYER)
		{
			std::string strName;
			strName = Npc[Player[nPlayerIndex].m_nIndex].Name;
			BrotherList.push_back(strName);
		}
	}

	KPlayerChat::MakeBrother(BrotherList);
	return 1;
}

INT LuaMakeEnemy(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;
	char* szEnemy = (char*)Lua_ValueToString(L, 1);

	if (szEnemy[0])
	{
		KPlayerChat::MakeEnemy(Player[nPlayerIndex].Name, szEnemy);
	}
	return 1;
}

INT LuaMakeMate(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;

	Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_BASEDATA_MATENAME, (char*)Lua_ValueToString(L, 1));
	return 1;
}

INT LuaDeleteMate(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	const char* szMateName = Player[nPlayerIndex].m_cTask.GetSaveStr(TASKVALUE_BASEDATA_MATENAME);
	if (szMateName[0])
		Player[nPlayerIndex].m_cTask.SetSaveVal(TASKVALUE_BASEDATA_MATENAME, "");

	return 1;
}

INT LuaAddLeadExp(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	INT nLeadExp = (INT)Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].AddLeadExp(nLeadExp);
	return 1;
}

INT LuaSetLeadLevel(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	INT nLeadLevel = (INT)Lua_ValueToNumber(L, 1);
	Player[nPlayerIndex].SetLeadLevel(nLeadLevel);
	return 1;
}

INT LuaGetLeadLevel(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nLeadLevel = 0;
	if (nPlayerIndex <= 0)
		goto lab_getleadlevel;
	nLeadLevel = (INT)Player[nPlayerIndex].m_nLeadLevel;

lab_getleadlevel:
	Lua_PushNumber(L, nLeadLevel);
	return 1;
}

INT LuaAddMagicPoint(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	Player[nPlayerIndex].m_nSkillPoint += (INT)Lua_ValueToNumber(L, 1);
	if (Player[nPlayerIndex].m_nSkillPoint < 0)
		Player[nPlayerIndex].m_nSkillPoint = 0;

	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_lpBuf = (LPVOID)Player[nPlayerIndex].m_nSkillPoint;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_MAGICPOINT;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	return 1;
}

INT LuaRemoveMagicPoint(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	Player[nPlayerIndex].m_nSkillPoint -= (INT)Lua_ValueToNumber(L, 1);
	if (Player[nPlayerIndex].m_nSkillPoint < 0)
		Player[nPlayerIndex].m_nSkillPoint = 0;

	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_lpBuf = (LPVOID)Player[nPlayerIndex].m_nSkillPoint;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_MAGICPOINT;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	return 1;
}

INT LuaGetMagicPoint(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nSkillPoint = 0;
	if (nPlayerIndex <= 0)
		goto lab_getmagicpoint;

lab_getmagicpoint:
	Lua_PushNumber(L, Player[nPlayerIndex].m_nSkillPoint);
	return 1;
}

INT LuaAddPropPoint(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nPropPoint = 0;
	if (nPlayerIndex <= 0)
		return 0;

	Player[nPlayerIndex].m_nAttributePoint += (INT)Lua_ValueToNumber(L, 1);
	if (Player[nPlayerIndex].m_nAttributePoint < 0)
		Player[nPlayerIndex].m_nAttributePoint = 0;

	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_lpBuf = (LPVOID)Player[nPlayerIndex].m_nAttributePoint;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_PROPPOINT;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	return 1;
}

INT LuaRemovePropPoint(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nPropPoint = 0;
	if (nPlayerIndex <= 0)
		return 0;

	Player[nPlayerIndex].m_nAttributePoint -= (INT)Lua_ValueToNumber(L, 1);
	if (Player[nPlayerIndex].m_nAttributePoint < 0)
		Player[nPlayerIndex].m_nAttributePoint = 0;

	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_lpBuf = (LPVOID)Player[nPlayerIndex].m_nAttributePoint;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_PROPPOINT;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	return 1;
}

INT LuaSetExtPoint(Lua_State* L)
{
	INT nResult = 0;
	INT nExtPoint = 0;
	INT nChange = 1;
	INT nPlayerIndex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_setextpoint;

	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_setextpoint;
	nExtPoint = Lua_ValueToNumber(L, 1);
	if (nExtPoint < 0)
		goto lab_setextpoint;
	Player[nPlayerIndex].SetExtPoint(nExtPoint, nChange);

lab_setextpoint:
	Lua_PushNumber(L, 0);
	return 1;
}

INT LuaAddExtPoint(Lua_State* L)
{
	INT nResult = 0;
	INT nExtPoint = 0;
	INT nChange = 1;
	INT nPlayerIndex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_setextpoint;

	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_setextpoint;
	nExtPoint = Lua_ValueToNumber(L, 1);
	if (nExtPoint < 0)
		goto lab_setextpoint;
	Player[nPlayerIndex].AddExtPoint(nExtPoint, nChange);

lab_setextpoint:
	Lua_PushNumber(L, 0);
	return 1;
}

INT LuaPayExtPoint(Lua_State* L)
{
	INT nResult = 0;
	INT nPay = 0;
	INT nPlayerIndex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_payextpoint;

	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_payextpoint;
	nPay = Lua_ValueToNumber(L, 1);
	if (nPay < 0)
		goto lab_payextpoint;
	nResult = Player[nPlayerIndex].PayExtPoint(nPay);

lab_payextpoint:
	Lua_PushNumber(L, nResult);
	return 1;
}

//PayExtPoint
INT LuaGetExtPoint(Lua_State* L)
{
	INT nResult = 0;
	INT nPlayerIndex = 0;
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_getextpoint;

	nResult = Player[nPlayerIndex].GetExtPoint();

lab_getextpoint:
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaExchangeExtDay(Lua_State* L)
{
	INT nResult = 0;
	INT nPay = 0;
	INT nPlayerIndex = 0;
	nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		goto lab_exit;

	nPay = (INT)Lua_ValueToNumber(L, 1);
	if (nPay < 0)
	{
		goto lab_exit;
	}

	Player[nPlayerIndex].SetExtDay(nPay);

	if (Player[nPlayerIndex].GetExtDay() > 0)
		nResult = 1;

lab_exit:
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaGetRestPropPoint(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nPropPoint = 0;
	if (nPlayerIndex <= 0)
		goto lab_getrestproppoint;
	nPropPoint = Player[nPlayerIndex].m_nAttributePoint;

lab_getrestproppoint:
	Lua_PushNumber(L, nPropPoint);
	return 1;
}

//Msg2GM(str, id)
INT LuaMsgToGameMaster(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 1)
		return 0;
	INT nParamID = 0;
	const char* szMsg = Lua_ValueToString(L, 1);
	if (!szMsg)
		return 0;

	if (nParamCount < 2)
	{
		nParamID = 0;
	}
	else
	{
		nParamID = (INT)Lua_ValueToNumber(L, 2);
	}

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		char szID[32];

		sprintf(szID, "%d", nParamID);
		//KPlayerChat::SendInfoToGM(Player[nPlayerIndex].AccountName, Npc[Player[nPlayerIndex].m_nIndex].Name, (char *) szMsg, strlen(szMsg) );
		KPlayerChat::SendInfoToGM(MESSAGE_SYSTEM_ANNOUCE_HEAD, szID, (char*)szMsg, strlen(szMsg));
	}
	return 0;
}

//Msg2IP(IP, ID, str)
INT LuaMsgToIP(Lua_State* L)
{
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 3)
		return 0;
	INT nIP = 0;
	const char* szIP = Lua_ValueToString(L, 1);
	nIP = _a2ip(szIP);
	if (nIP == 0)
		return 0;

	INT nID = (INT)Lua_ValueToNumber(L, 2);
	const char* szMsg = Lua_ValueToString(L, 3);
	if (!szMsg)
		return 0;

	INT nParamID = 0;
	if (nParamCount < 4)
	{
		nParamID = 0;
	}
	else
	{
		nParamID = (INT)Lua_ValueToNumber(L, 4);
	}
	char szID[32];
	sprintf(szID, "%d", nParamID);
	KPlayerChat::SendInfoToIP(nIP, nID, MESSAGE_SYSTEM_ANNOUCE_HEAD, szID, (char*)szMsg, strlen(szMsg));
	return 0;
}

INT LuaGetIP(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	char szDesMsg[200];
	szDesMsg[0] = 0;
	std::string szIpAddress = "";
	KPlayer* pPlayer = NULL;
	if (nPlayerIndex <= 0)
		goto lab_getplayerip;
	pPlayer = &Player[nPlayerIndex];
	szIpAddress = g_pServer->GetClientInfo(pPlayer->m_nNetConnectIdx);
	if (!szIpAddress.empty()) {
		strcpy(szDesMsg, szIpAddress.c_str());
	}
	else
	{
		strcpy(szDesMsg, "127.0.0.1");
	}

lab_getplayerip:
	Lua_PushString(L, szDesMsg);
	return 1;
}

INT LuaGetIP2(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
		goto lab_getplayerip;

lab_getplayerip:
	Lua_PushString(L, Player[nPlayerIndex].m_szIpAddress);
	return 1;
}

INT LuaGetNumberPlayerByIp(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	char* szIP = (char*)Lua_ValueToString(L, 1);
	INT nCount = 0;


	INT nIndex = PlayerSet.GetFirstPlayer();
	while (nIndex > 0)
	{
		if (strcmp(Player[nIndex].m_szIpAddress, szIP) == 0)
		{
			nCount++;
		}

		nIndex = PlayerSet.GetNextPlayer();
	}

	Lua_PushNumber(L, nCount);
	return 1;
}

INT LuaSetDeathPunish(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if ((INT)Lua_ValueToNumber(L, 1))
			Npc[Player[nPlayerIndex].m_nIndex].m_nCurPKPunishState = enumDEATH_MODE_PKBATTLE_PUNISH;
		else
			Npc[Player[nPlayerIndex].m_nIndex].m_nCurPKPunishState = 0;

	}
	return 0;
}

INT LuaSetReviveNow(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		Npc[Player[nPlayerIndex].m_nIndex].m_bReviveNow = (BOOL)Lua_ValueToNumber(L, 1);
	}
	return 0;
}

INT LuaNpcChat(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex > 0 || nNpcIndex < MAX_NPC)
	{
		char* szMsg = (char*)Lua_ValueToString(L, 2);
		INT nLen = TEncodeText(szMsg, strlen(szMsg));
		if (nLen >= MAX_MESSAGE_LENGTH)
			szMsg[nLen - 1] = '\0';
		NPC_CHAT_SYNC	command;
		command.ProtocolType = s2c_npcchat;
		command.ID = Npc[nNpcIndex].m_dwID;
		strcpy(command.szMsg, szMsg);
		command.nMsgLen = nLen;
		Npc[nNpcIndex].SendDataToNearRegion(&command, sizeof(NPC_CHAT_SYNC));
	}

	return 0;
}

INT LuaHideNpc(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;

	INT nNpcIndex = 0;

	if (Lua_IsNumber(L, 1))
	{
		nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	}
	else
	{
		const char* szName = Lua_ValueToString(L, 1);
		INT nSubWorldIndex = GetSubWorldIndex(L);
		if (nSubWorldIndex < 0)
			return 0;

		nNpcIndex = SubWorld[nSubWorldIndex].FindNpcFromName(szName);
	}

	if (nNpcIndex > 0 || nNpcIndex < MAX_NPC)
	{
		INT nFrame = Lua_ValueToNumber(L, 2);
		if (nFrame <= 0)
			nFrame = 1;

		Npc[nNpcIndex].ExecuteRevive();
		Npc[nNpcIndex].m_Frames.nTotalFrame = nFrame;
		Npc[nNpcIndex].m_Frames.nCurrentFrame = 0;
	}

	return 0;
}

INT LuaAddSkillState(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 4)
		return 0;
	INT nSkillId = (INT)Lua_ValueToNumber(L, 1);
	INT nSkillLevel = (INT)Lua_ValueToNumber(L, 2);
	INT nIfMagic = (INT)Lua_ValueToNumber(L, 3);
	INT nTime = (INT)Lua_ValueToNumber(L, 4);
	BOOL bOverLook = FALSE;
	if (nParamNum > 5)
		bOverLook = (BOOL)Lua_ValueToNumber(L, 5);

	if (nTime <= 0)
		nTime = -1;

	if (nIfMagic)
	{
		KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(nSkillId, nSkillLevel);
		pSkill->CastStateSkill(Player[nPlayerIndex].m_nIndex, 0, 0, nTime, bOverLook);
	}
	else
	{
		KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
		memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
		DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
		DamageMagicAttribs[0].nValue[0] = 0;
		Npc[Player[nPlayerIndex].m_nIndex].SetStateSkillEffect(Player[nPlayerIndex].m_nIndex, nSkillId, nSkillLevel, DamageMagicAttribs, 1, nTime, bOverLook);
	}
	return 0;
}

INT LuaAddNpcSkillState(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 5)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nNpcIndex < 0 || nNpcIndex >= MAX_NPC) return 0;

	INT nSkillId = (INT)Lua_ValueToNumber(L, 2);
	INT nSkillLevel = (INT)Lua_ValueToNumber(L, 3);
	INT nIfMagic = (INT)Lua_ValueToNumber(L, 4);
	INT nTime = (INT)Lua_ValueToNumber(L, 5);

	if (nIfMagic)
	{
		KSkill* pSkill = (KSkill*)g_SkillManager.GetSkill(nSkillId, nSkillLevel);
		pSkill->CastStateSkill(nNpcIndex, 0, 0, nTime, TRUE);
	}
	else
	{
		KMagicAttrib DamageMagicAttribs[MAX_MISSLE_DAMAGEATTRIB];
		memset(DamageMagicAttribs, 0, sizeof(DamageMagicAttribs));
		DamageMagicAttribs[0].nAttribType = magic_attackrating_v;
		DamageMagicAttribs[0].nValue[0] = 0;
		Npc[nNpcIndex].SetStateSkillEffect(nNpcIndex, nSkillId, nSkillLevel, DamageMagicAttribs, 1, nTime, TRUE);
	}
	return 0;
}

INT LuaIgnoreState(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	if (Player[nPlayerIndex].m_nIndex <= 0)
		return 0;
	Npc[Player[nPlayerIndex].m_nIndex].IgnoreState(FALSE);
	return 0;
}

INT LuaCastSkill(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 3)
		return 0;

	INT nSkillId = (INT)Lua_ValueToNumber(L, 1);
	INT nSkillLevel = (INT)Lua_ValueToNumber(L, 2);
	if (nSkillId < MAX_SKILL && nSkillLevel < MAX_SKILLLEVEL)
		Npc[Player[nPlayerIndex].m_nIndex].Cast(nSkillId, nSkillLevel, Npc[Player[nPlayerIndex].m_nIndex].m_Index);
	return 0;
}

INT LuaNpcCastSkill(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 3)
		return 0;

	INT nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nNpcIndex > 0 || nNpcIndex < MAX_NPC)
	{
		INT nSkillId = (INT)Lua_ValueToNumber(L, 2);
		INT nSkillLevel = (INT)Lua_ValueToNumber(L, 3);
		if (nSkillId < MAX_SKILL && nSkillLevel < MAX_SKILLLEVEL)
			Npc[nNpcIndex].Cast(nSkillId, nSkillLevel, Npc[nNpcIndex].m_Index);
	}
	return 0;
}

INT LuaSetMask(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	INT nMaskType = (INT)Lua_ValueToNumber(L, 1);
	if (nMaskType > 0)
	{
		Player[nPlayerIndex].m_ItemList.SetMaskLock(TRUE);
		Npc[Player[nPlayerIndex].m_nIndex].m_MaskType = nMaskType;
	}
	else
	{
		Player[nPlayerIndex].m_ItemList.SetMaskLock(FALSE);
		INT nMaskIdx = Player[nPlayerIndex].m_ItemList.GetEquipment(itempart_mask);
		if (nMaskIdx > 0)
			g_MaskChangeRes.GetInteger(Item[nMaskIdx].GetParticular() + 2, 2, 0, &Npc[Player[nPlayerIndex].m_nIndex].m_MaskType);
		else
			Npc[Player[nPlayerIndex].m_nIndex].m_MaskType = 0;
	}
	return 0;
}

INT LuaAddTrap(Lua_State* L)
{
	INT nPosX, nPosY, nCellNum = 0;
	INT nSubWorldIndex = g_SubWorldSet.SearchWorld((INT)Lua_ValueToNumber(L, 1));
	nPosX = (INT)Lua_ValueToNumber(L, 2);
	nPosY = (INT)Lua_ValueToNumber(L, 3);
	nCellNum = (INT)Lua_ValueToString(L, 4);
	DWORD dwTrapId = g_FileName2Id((char*)Lua_ValueToString(L, 5));

	if (nSubWorldIndex == -1)
		return 0;

	if (dwTrapId == 0)
		return 0;

	INT nRegion, nMapX, nMapY, nOffX, nOffY;

	SubWorld[nSubWorldIndex].Mps2Map(nPosX, nPosY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion == -1)
		return 0;

	SubWorld[nSubWorldIndex].m_Region[nRegion].SetTrap(nMapX, nMapY, 1, dwTrapId);
	return 0;
}

INT LuaAddObj(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 5)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nObjID = (INT)Lua_ValueToNumber(L, 1);
	if (nObjID <= 0 || nObjID >= ObjSet.m_cTabFile.GetHeight())
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nSubWorldIndex = g_SubWorldSet.SearchWorld((INT)Lua_ValueToNumber(L, 2));

	if (nSubWorldIndex == -1)
		return 0;

	KMapPos	Pos;

	Pos.nSubWorld = nSubWorldIndex;
	SubWorld[nSubWorldIndex].Mps2Map((INT)Lua_ValueToNumber(L, 3), (INT)Lua_ValueToNumber(L, 4),
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY,
		&Pos.nOffX, &Pos.nOffY);
	KObjItemInfo	sInfo;
	sInfo.m_nItemID = 0;
	sInfo.m_nItemWidth = 0;
	sInfo.m_nItemHeight = 0;
	sInfo.m_nMoneyNum = 0;
	sInfo.m_szName[0] = 0;
	sInfo.m_nColorID = 0;
	sInfo.m_nGenre = 0;
	sInfo.m_nDetailType = 0;
	sInfo.m_nMovieFlag = 1;
	sInfo.m_nSoundFlag = 1;
	sInfo.m_bOverLook = 0;
	INT nObj = ObjSet.Add(nObjID, Pos, sInfo);
	if (nObj <= 0) return 0;

	Object[nObj].SetScriptFile((char*)Lua_ValueToString(L, 5));
	if (nParamNum > 5)
		Object[nObj].SetImageDir((INT)Lua_ValueToNumber(L, 6));
	if (nParamNum > 6)
		Object[nObj].SetState((INT)Lua_ValueToNumber(L, 7));

	Lua_PushNumber(L, nObj);
	return 1;
}

INT LuaDelObj(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;

	INT nObjIndex = 0;
	if (Lua_GetTopIndex(L) >= 3)
	{
		INT nSubWorldIndex = g_SubWorldSet.SearchWorld((INT)Lua_ValueToNumber(L, 1));

		if (nSubWorldIndex == -1)
			return 0;

		KMapPos	Pos;

		Pos.nSubWorld = nSubWorldIndex;
		SubWorld[nSubWorldIndex].Mps2Map((INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3),
			&Pos.nRegion, &Pos.nMapX, &Pos.nMapY,
			&Pos.nOffX, &Pos.nOffY);
		nObjIndex = ObjSet.SearchObjAt(Pos);
	}
	else
		nObjIndex = (INT)Lua_ValueToNumber(L, 1);

	if (nObjIndex > 0)
	{
		Object[nObjIndex].SyncRemove(FALSE);
		if (Object[nObjIndex].m_nRegionIdx >= 0)
			SubWorld[Object[nObjIndex].m_nSubWorldID].m_Region[Object[nObjIndex].m_nRegionIdx].RemoveObj(nObjIndex);
		ObjSet.Remove(nObjIndex);
	}
	return 0;
}

INT LuaSetObjScript(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nObjId;
	char* szScriptName;
	nObjId = (INT)Lua_ValueToNumber(L, 1);
	szScriptName = (char*)Lua_ValueToString(L, 2);

	if (nObjId > 0)
	{
		if (szScriptName)
		{
			Object[nObjId].SetScriptFile(szScriptName);
			Object[nObjId].m_dwScriptID = g_FileName2Id((char*)Lua_ValueToString(L, 2));
		}
	}
	return 0;
}

INT LuaSetObjValue(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nId, nValue;
	nId = (INT)Lua_ValueToNumber(L, 1);
	nValue = (INT)Lua_ValueToNumber(L, 2);

	if (nId > 0)
	{
		Object[nId].SetObjValue(nValue);
	}
	return 0;
}

INT LuaGetObjValue(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;
	INT nObjIndex = (INT)Lua_ValueToNumber(L, 1);
	if (nObjIndex > 0)
	{
		Lua_PushNumber(L, Object[nObjIndex].GetObjValue());
	}
	return 1;
}

INT LuaSetObjPickExecute(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nId, nValue;
	nId = (INT)Lua_ValueToNumber(L, 1);
	nValue = (INT)Lua_ValueToNumber(L, 2);

	if (nId > 0)
	{
		Object[nId].SetObjPickExecute(nValue > 0);
	}
	return 0;
}

INT LuaOpenGive(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	Player[nPlayerIndex].m_szTaskExcuteFun[0] = 0;

	PLAYER_GIVE PGive;
	PGive.ProtocolType = (BYTE)s2c_opengive;
	strcpy(PGive.m_szName, (char*)Lua_ValueToString(L, 1));
	strcpy(PGive.m_szInitString, (char*)Lua_ValueToString(L, 2));
	strcpy(Player[nPlayerIndex].m_szTaskExcuteFun, (char*)Lua_ValueToString(L, 3));
	Player[nPlayerIndex].m_dwTaskExcuteScriptId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &PGive, sizeof(PLAYER_GIVE));

	return 0;
}

//TamLTM hien thi nhan do da tau VNG
INT LuaFinishQuest(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nDT = 0;
	if (Lua_IsNumber(L, 1))
	{
		nDT = (INT)Lua_ValueToNumber(L, 1);
	}
	if (nDT)
	{
		FINISH_QUEST_SYNC DTSync;
		DTSync.ProtocolType = s2c_finishquest;
		DTSync.nIdQuestIndex = nDT;
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &DTSync, sizeof(FINISH_QUEST_SYNC));
	}
	return 0;
}
//end code

//TamLTM LuaOpenProgressBar
INT LuaOpenProgressBar(Lua_State* L)
{
	if (Lua_GetTopIndex(L) <= 0) return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nPrBar = 0;
	if (Lua_IsNumber(L, 1))
	{
		nPrBar = (INT)Lua_ValueToNumber(L, 1);
	}
	if (nPrBar)
	{
		OPEN_PROGRESS_BAR_SYNC ProBarSync;
		ProBarSync.ProtocolType = s2c_openprogressbar;
		ProBarSync.nIdQuestIndex = nPrBar;
		g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &ProBarSync, sizeof(OPEN_PROGRESS_BAR_SYNC));
	}
	return 0;
}
//end code

INT LuaRemoveRoom(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	if (Lua_GetTopIndex(L) < 1)
	{
		return 0;
	}
	INT nRoom = (INT)Lua_ValueToNumber(L, 1);
	if (nRoom < room_equipment || nRoom > room_num)
		return 0;
	Player[nPlayerIndex].m_ItemList.RemoveRoom(nRoom);
	return 0;
}

INT LuaCalcFreeItemCellCount(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nWidth = 1;
	INT nHeight = 1;
	INT nRoom = room_equipment;
	if (Lua_GetTopIndex(L) > 2)
	{
		nWidth = (INT)Lua_ValueToNumber(L, 1);
		nHeight = (INT)Lua_ValueToNumber(L, 2);
	}
	if (Lua_GetTopIndex(L) > 3)
	{
		nRoom = (INT)Lua_ValueToNumber(L, 3);
	}
	if (nRoom < room_equipment || nRoom > room_num)
		return 0;
	INT nCount = Player[nPlayerIndex].m_ItemList.CalcFreeItemCellCount(nWidth, nHeight, nRoom);
	Lua_PushNumber(L, nCount);
	return 1;
}

INT LuaIsNumber(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	if (Lua_IsNumber(L, 1))
	{
		Lua_PushNumber(L, 1);
		return 1;
	}
	else
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	return 1;
}

INT LuaIsTable(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	if (Lua_IsTable(L, 1))
	{
		Lua_PushNumber(L, 1);
		return 1;
	}
	else
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	return 1;
}

INT LuaOpenURL(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	if (Lua_IsString(L, 1))
	{
		ShellExecute(NULL, "open", (char*)Lua_ValueToString(L, 1), NULL, NULL, SW_SHOWNORMAL);
		return 0;
	}
	return 0;
}

INT LuaOpenExplore(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	if (Lua_IsString(L, 1))
	{
		ShellExecute(NULL, "explore", (char*)Lua_ValueToString(L, 1), NULL, NULL, SW_SHOWNORMAL);
		return 0;
	}
	return 0;
}

INT LuaModifyAttrib(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 5)
		return 0;
	KNpc* pNPC = &Npc[Player[nPlayerIndex].m_nIndex];
	KMagicAttrib pBaseAttrib;
	pBaseAttrib.nAttribType = (INT)Lua_ValueToNumber(L, 1);
	pBaseAttrib.nValue[0] = (INT)Lua_ValueToNumber(L, 2);
	pBaseAttrib.nValue[1] = (INT)Lua_ValueToNumber(L, 3);
	pBaseAttrib.nValue[2] = (INT)Lua_ValueToNumber(L, 4);
	KMagicAttrib* pAttrib = &pBaseAttrib;
	if (-1 != pAttrib->nAttribType)
	{
		pNPC->ModifyAttrib(pNPC->m_Index, (void*)pAttrib);
	}
	return 0;
}

INT LuaRANDOM(Lua_State* L)
{
	INT nMin = 0, nMax = 0;

	INT nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 1)
		return 0;
	INT nResult = 0;
	/*if (nParamNum > 1)
		nResult = GetRandomNumber((INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2));
	else
		nResult = GetRandomNumber(0, (INT)Lua_ValueToNumber(L, 1));*/
	if (nParamNum > 1)
	{
		nMin = (INT)Lua_ValueToNumber(L, 1);
		nMax = (INT)Lua_ValueToNumber(L, 2);
	}
	else
	{
		nMin = 0;
		nMax = (INT)Lua_ValueToNumber(L, 1);
	}

	nResult = _GetRandomNumber(nMin, nMax);

	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaRANDOMC(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 2)
		return 0;

	srand((UINT)time(NULL));
	if (Lua_IsTable(L, 1))
	{
		Lua_PushNumber(L, (INT)Lua_ValueToNumber(L, 2));
		Lua_RawGet(L, 1);
		Lua_PushNumber(L, (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L)));
	}
	else if (Lua_IsTable(L, 2))
	{
		INT nResult = _GetRandomNumber(1, (INT)Lua_ValueToNumber(L, 1));
		Lua_PushNumber(L, nResult);
		Lua_RawGet(L, 2);
		Lua_PushNumber(L, (INT)Lua_ValueToNumber(L, (INT)Lua_ValueToNumber(L, Lua_GetTopIndex(L))));
	}
	else
	{
		INT nResult = _GetRandomNumber(1, nParamNum);
		Lua_PushNumber(L, (INT)Lua_ValueToNumber(L, nResult));
	}
	return 1;
}

INT LuaIsMyItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		if (Lua_GetTopIndex(L) > 1)
		{
			INT nGameIdx = (INT)Lua_ValueToNumber(L, 1);
			if (Player[nPlayerIndex].m_ItemList.SearchID(Item[nGameIdx].GetID()))
				Lua_PushNumber(L, 1);
			else
				Lua_PushNumber(L, 0);
		}
	}
	return 1;
}

INT LuaInput(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	Player[nPlayerIndex].m_dwTaskExcuteScriptId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;
	strcpy(Player[nPlayerIndex].m_szTaskExcuteFun, (char*)Lua_ValueToString(L, 1));
	INT max = 0;
	if (nParamNum > 2)
		max = (INT)Lua_ValueToNumber(L, 2);

	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_INPUT;
	sMsg.m_lpBuf = (LPVOID)max;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);

	return 0;
}

INT LuaGetInput(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	Lua_PushString(L, Player[nPlayerIndex].m_szLastInput);
	return 1;
}

INT LuaOpenEnchase(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;

	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_ENCHASE;
	sMsg.m_lpBuf = 0;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	return 0;
}

INT LuaEnchase(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	Player[nPlayerIndex].Enchase(2, (INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3));
	return 0;
}

INT LuaCheckRoom(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nIndex, nWidth = 0, nHeight = 0;;

	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	if (Lua_GetTopIndex(L) < 2)
	{
		nIndex = (INT)Lua_ValueToNumber(L, 1);
		nWidth = Item[nIndex].GetWidth();
		nHeight = Item[nIndex].GetHeight();
	}
	if (Lua_GetTopIndex(L) >= 2)
	{
		nWidth = (INT)Lua_ValueToNumber(L, 1);
		nHeight = (INT)Lua_ValueToNumber(L, 2);
	}
	if (nPlayerIndex <= 0)
		return 0;

	if (nWidth <= 0 || nHeight <= 0)
		return 0;

	POINT ItemSize;
	ItemSize.x = nWidth;
	ItemSize.y = nHeight;
	ItemPos	sItemPos;
	if (FALSE == Player[nPlayerIndex].m_ItemList.SearchPosition(ItemSize, &sItemPos))
		Lua_PushNumber(L, 0);
	else
		Lua_PushNumber(L, 1);

	return 1;
}

INT LuaSendMessageInfo(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_MSGINFO;
	UiInfo.m_bOptionNum = 1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_nParam = SMT_NORMAL;
	UiInfo.m_nParam1 = SMCT_NONE;
	if (nParamNum > 1)
		UiInfo.m_nParam = (INT)Lua_ValueToNumber(L, 2);
	if (nParamNum > 2)
		UiInfo.m_nParam1 = (INT)Lua_ValueToNumber(L, 3);

	INT nMsgId = 0;

	if (Lua_IsNumber(L, 1))
	{
		nMsgId = (INT)Lua_ValueToNumber(L, 1);
		*((INT*)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(INT);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), 256);
		UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}

#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

INT LuaRemoveServerItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (!nIdx)
		return 0;

	ItemSet.Remove(nIdx);
	return 0;
}

INT LuaRemoveItemIdx(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nItemIdx = 0;
	INT nNum = 1;
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum == 1)
	{
		nItemIdx = (INT)Lua_ValueToNumber(L, 1);
	}
	else
	{
		nItemIdx = (INT)Lua_ValueToNumber(L, 1);
		nNum = (INT)Lua_ValueToNumber(L, 2);
	}

	if (Player[nPlayerIndex].m_ItemList.RemoveItem(nItemIdx, nNum))
		Lua_PushNumber(L, 1);

	return 1;
}

INT LuaLockMoveItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	LockMoveItem LockMove;
	LockMove.bLock = (BOOL)Lua_ValueToNumber(L, 1);
	LockMove.nPlace = 0;
	Player[nPlayerIndex].SetLockMove(&LockMove);
	return 0;
}

INT LuaSyncItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nItemIdx = (INT)Lua_ValueToNumber(L, 1);


	if (nItemIdx <= 0 || nItemIdx > MAX_ITEM)
		return 0;

	Player[nPlayerIndex].m_ItemList.SyncItem(nItemIdx);
	return 0;
}

INT LuaSetTempItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		Item[nIdx].SetTemp((BOOL)Lua_ValueToNumber(L, 2));
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaSetLevelItem(Lua_State* L)
{
	INT nLevel = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	nLevel = (INT)Lua_ValueToNumber(L, 2);

	if (nIdx > 0)
	{
		Item[nIdx].SetLevel(nLevel);
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaSetSeriesItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		Item[nIdx].SetSeries((INT)Lua_ValueToNumber(L, 2));
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaSetParamItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		Item[nIdx].SetParam((INT)Lua_ValueToNumber(L, 2));
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaSetFortuneItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		Item[nIdx].SetFortune((INT)Lua_ValueToNumber(L, 2));
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaSetTimeItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	INT nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 3)
		return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		Item[nIdx].SetExpireTime(KSG_GetCurSec() + (INT)Lua_ValueToNumber(L, 2));
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaGetTimeMin(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	Lua_PushNumber(L, KSG_GetCurMin());
	return 1;
}

INT LuaSetExpiredDateItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;
	INT nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 2)
		return 0;

	INT nSec = 0;
	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nParamNum >= 7)
		nSec = KSG_GetExpiredSec((INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3), (INT)Lua_ValueToNumber(L, 4), (INT)Lua_ValueToNumber(L, 5), (INT)Lua_ValueToNumber(L, 6), (INT)Lua_ValueToNumber(L, 7));
	else if (nParamNum > 6)
		nSec = KSG_GetExpiredSec((INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3), (INT)Lua_ValueToNumber(L, 4), (INT)Lua_ValueToNumber(L, 5), (INT)Lua_ValueToNumber(L, 6), 0);
	else if (nParamNum > 5)
		nSec = KSG_GetExpiredSec((INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3), (INT)Lua_ValueToNumber(L, 4), (INT)Lua_ValueToNumber(L, 5), 0, 0);
	else if (nParamNum > 4)
		nSec = KSG_GetExpiredSec((INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3), (INT)Lua_ValueToNumber(L, 4), 0, 0, 0);
	else if (nParamNum > 3)
		nSec = KSG_GetExpiredSec((INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3), 0, 0, 0, 0);
	else
		nSec = KSG_GetExpiredSec((INT)Lua_ValueToNumber(L, 2), 0, 0, 0, 0, 0);

	if (nIdx > 0)
	{
		Item[nIdx].SetExpireTime(nSec);
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaAddTimeItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		if (Item[nIdx].GetExpireTime())
			Item[nIdx].SetExpireTime(Item[nIdx].GetExpireTime() + (INT)Lua_ValueToNumber(L, 2));
		else
			Item[nIdx].SetExpireTime(KSG_GetCurSec() + (INT)Lua_ValueToNumber(L, 2));

		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaLockItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	if (nParamNum > 4)
	{
		INT nIdx = (INT)Lua_ValueToNumber(L, 1);
		if (nIdx > 0)
		{
			Item[nIdx].SetLockTrade((BOOL)Lua_ValueToNumber(L, 2));
			Item[nIdx].SetLockSell((BOOL)Lua_ValueToNumber(L, 3));
			Item[nIdx].SetLockDrop((BOOL)Lua_ValueToNumber(L, 4));
			Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
			Lua_PushNumber(L, nIdx);
			return 1;
		}
	}
	else
	{
		INT nIdx = (INT)Lua_ValueToNumber(L, 1);
		INT nLock = LOCK_STATE_FOREVER;
		if (nIdx > 0)
		{
			if (nParamNum > 2)
				nLock = (INT)Lua_ValueToNumber(L, 2);
			Item[nIdx].LockItem(nLock);
			Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
			Lua_PushNumber(L, nIdx);
			return 1;
		}
	}
	return 0;
}

INT LuaSetStackItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		Item[nIdx].SetStackNum((INT)Lua_ValueToNumber(L, 2));
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaSetMantleItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		if (Item[nIdx].GetGenre() == item_equip &&
			Item[nIdx].GetDetailType() == equip_armor)
		{
			Item[nIdx].SetMantle((INT)Lua_ValueToNumber(L, 2));
			Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);

			if (Player[nPlayerIndex].m_ItemList.GetEquipment(itempart_mantle) <= 0)
				Npc[Player[nPlayerIndex].m_nIndex].m_MantleType = Item[nIdx].GetMantle();
		}
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaSetFlashItem(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);

	if (nIdx > 0)
	{
		Item[nIdx].SetFlash(TGetColor((char*)Lua_ValueToString(L, 2)));
		Player[nPlayerIndex].m_ItemList.SyncItem(nIdx);
		Lua_PushNumber(L, nIdx);
		return 1;
	}
	return 0;
}

INT LuaGetFreeObjPos(Lua_State* L)
{
	INT nSubWorldIndex = 0, nNpcIndex = -1;

	POINT	ptLocal;
	if (Lua_GetTopIndex(L) > 2)
	{
		nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
		ptLocal.x = (INT)Lua_ValueToNumber(L, 2);
		ptLocal.y = (INT)Lua_ValueToNumber(L, 3);
	}
	else if (Lua_GetTopIndex(L) > 1)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex <= 0) return 0;
		nNpcIndex = Player[nPlayerIndex].m_nIndex;
		nSubWorldIndex = Npc[nNpcIndex].m_SubWorldIndex;
		ptLocal.x = (INT)Lua_ValueToNumber(L, 1);
		ptLocal.y = (INT)Lua_ValueToNumber(L, 2);
	}
	else
		return 0;

	SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);
	Lua_PushNumber(L, (INT)(ptLocal.x / 32));
	Lua_PushNumber(L, (INT)(ptLocal.y / 32));
	return 2;
}

INT LuaOpenRankData(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	S2C_PLAYER_SYNC	sMsg;
	sMsg.ProtocolType = s2c_playersync;
	sMsg.m_wLength = sizeof(S2C_PLAYER_SYNC) - 1;
	sMsg.m_wMsgID = enumS2C_PLAYERSYNC_ID_RANKDATA;
	sMsg.m_lpBuf = 0;
	g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);

	return 0;
}

INT LuaSetSavePw(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	Player[nPlayerIndex].SetSavePw((char*)Lua_ValueToString(L, 1), TRUE);
	return 0;
}

INT LuaGetSavePw(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
		return 0;

	Lua_PushString(L, (char*)Player[nPlayerIndex].m_cTask.GetSaveStr(TASKVALUE_BASEDATA_PASSWORD));
	return 1;
}

INT LuaSetLockState(Lua_State* L)
{
	INT bLock = (INT)Lua_ValueToNumber(L, 1);
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	Player[nPlayerIndex].SetLockState(bLock > 0);
	return 0;
}

INT LuaGetLockState(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	if (Player[nPlayerIndex].GetLockState())
	{
		Lua_PushNumber(L, 1);
	}
	else
	{
		Lua_PushNumber(L, 0);
	}
	return 1;
}

INT LuaIsHideNpc(Lua_State* L)
{
	INT nRet = -1;
	if (Lua_GetTopIndex(L) == 1)
	{
		INT nNpcIndex = 0;
		nNpcIndex = (INT)Lua_ValueToNumber(L, 1);
		if (nNpcIndex > 0 || nNpcIndex < MAX_NPC)
		{
			if (Npc[nNpcIndex].IsAlive())
				nRet = 0;
			else
				nRet = 1;
		}
	}
	Lua_PushNumber(L, nRet);
	return 1;
}

INT LuaSetPKState(Lua_State* L)
{
	INT nPKSet = 0;
	INT nPKState = 0;
	INT nPlayerIndex = GetPlayerIndex(L);
	INT nParamNum = Lua_GetTopIndex(L);
	if (nPlayerIndex > 0)
	{
		if (nParamNum <= 2)
		{
			nPKSet = (INT)Lua_ValueToNumber(L, 1);
			nPKState = Player[nPlayerIndex].m_cPK.GetLockPKState();
		}
		else
		{
			nPKSet = (INT)Lua_ValueToNumber(L, 1);
			nPKState = (INT)Lua_ValueToNumber(L, 2);
		}
		if (nPKSet < 0 || nPKSet >= enumPKNum)
			return 0;

		if (nPKState < enumPKLogNothing || nPKSet >= enumPKLogNum)
			return 0;

		Player[nPlayerIndex].m_cPK.SetLockPKState(nPKSet, nPKState);
		Lua_PushNumber(L, 1);
	}
	else
	{
		Lua_PushNumber(L, 0);
	}
	return 1;
}

INT LuaForbidChangePK(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 1)
	{
		Player[nPlayerIndex].m_cPK.SetLockPKState(Player[nPlayerIndex].m_cPK.GetNormalPKState(), (INT)Lua_ValueToNumber(L, 1));
	}
	return 0;
}

INT LuaGetPKState(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nResult = Player[nPlayerIndex].m_cPK.GetLockPKState();
		Lua_PushNumber(L, nResult);
	}
	else
	{
		Lua_PushNumber(L, 0);
	}
	return 1;
}

INT LuaGetNormalPKState(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex > 0)
	{
		INT nResult = Player[nPlayerIndex].m_cPK.GetNormalPKState();
		Lua_PushNumber(L, nResult);
	}
	else
	{
		Lua_PushNumber(L, 0);
	}
	return 1;
}

INT LuaPaceBar(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nParamCount = Lua_GetTopIndex(L);
	if (nParamCount < 2)
		return 0;

	strcpy(Player[nPlayerIndex].m_szTaskExcuteFun, (char*)Lua_ValueToString(L, 1));
	Player[nPlayerIndex].m_nPaceBarTime = (INT)Lua_ValueToNumber(L, 2);
	Player[nPlayerIndex].m_nPaceBarTimeMax = (INT)Lua_ValueToNumber(L, 2);
	Player[nPlayerIndex].m_dwTaskExcuteScriptId = Npc[Player[nPlayerIndex].m_nIndex].m_ActionScriptID;

	return 0;
}

INT LuaRepairBroken(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			INT nIdx = (INT)Lua_ValueToNumber(L, 1);
			INT nMaxDur = Item[nIdx].GetMaxDurability();

			Item[nIdx].SetDurability(nMaxDur);
			ITEM_DURABILITY_CHANGE	IDC;
			IDC.ProtocolType = s2c_itemdurabilitychange;
			IDC.dwItemID = Item[nIdx].GetID();
			IDC.nChange = nMaxDur;
			if (g_pServer)
				g_pServer->PackDataToClient(Player[nPlayerIndex].m_nNetConnectIdx, &IDC, sizeof(ITEM_DURABILITY_CHANGE));
		}
	}

	return 1;
}

INT LuaGetDurability(Lua_State* L)
{
	INT nDur = 0;
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			INT nIdx = (INT)Lua_ValueToNumber(L, 1);
			nDur = Item[nIdx].GetDurability();

			Lua_PushNumber(L, nDur);
		}
	}

	return 1;
}

INT LuaDelItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			INT nItemNature, nItemGenre, nDetailType, nLevel, nSeries, P = pos_equiproom, nResult = 0;
			nItemNature = nItemGenre = nDetailType = nLevel = nSeries = -1;

			//P = (INT)Lua_ValueToNumber(L, 1);
			INT nDelNum = (INT)Lua_ValueToNumber(L, 1);

			if (nDelNum)
			{
				nItemNature = (INT)Lua_ValueToNumber(L, 2);
				nItemGenre = (INT)Lua_ValueToNumber(L, 3);

				if (nParamNum > 3)
					nDetailType = (INT)Lua_ValueToNumber(L, 4);

				if (nParamNum > 4)
					nLevel = (INT)Lua_ValueToNumber(L, 5);

				if (nParamNum > 5)
					nSeries = (INT)Lua_ValueToNumber(L, 6);

				if (nParamNum > 6)
					P = (INT)Lua_ValueToNumber(L, 7);

				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.RemoveCommonItem(nDelNum, nItemNature, nItemGenre, nDetailType, nLevel, nSeries, P));
				return 1;
			}
		}
	}
	Lua_PushNumber(L, 0);
	return 1;
}

INT LuaDelItemEx(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			INT nItemNature, nItemGenre, nDetailType, nLevel, nSeries, P = pos_equiproom, nResult = 0;
			nItemNature = nItemGenre = nDetailType = nLevel = nSeries = -1;

			nItemGenre = (INT)Lua_ValueToNumber(L, 1);
			nDetailType = (INT)Lua_ValueToNumber(L, 2);

			if (nItemGenre && nDetailType)
			{
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.RemoveCommonItem(1, 0, nItemGenre, nDetailType));
				return 1;
			}
		}
	}
	Lua_PushNumber(L, 0);
	return 1;
}

INT LuaGetItemCount(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			INT nItemNature, nItemGenre, nDetailType, nLevel, nSeries, P = pos_equiproom;
			nItemNature = nItemGenre = nDetailType = nLevel = nSeries = -1;

			nItemNature = (INT)Lua_ValueToNumber(L, 1);
			nItemGenre = (INT)Lua_ValueToNumber(L, 2);

			if (nParamNum > 2)
				nDetailType = (INT)Lua_ValueToNumber(L, 3);

			if (nParamNum > 3)
				nLevel = (INT)Lua_ValueToNumber(L, 4);

			if (nParamNum > 4)
				nSeries = (INT)Lua_ValueToNumber(L, 5);

			if (nParamNum > 5)
				P = (INT)Lua_ValueToNumber(L, 6);

			Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.CountCommonItem(nItemNature, nItemGenre, nDetailType, nLevel, nSeries, P));
			return 1;
		}
	}
	Lua_PushNumber(L, 0);
	return 1;
}

INT LuaGetItemCountNew(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			INT nItemNature, nItemGenre, nDetailType, nLevel, nSeries, P = pos_equiproom;
			nItemNature = nItemGenre = nDetailType = nLevel = nSeries = -1;

			nItemNature = NATURE_NORMAL;//(INT)Lua_ValueToNumber(L, 1);
			nItemGenre = (INT)Lua_ValueToNumber(L, 1);

			if (nParamNum > 2)
				nDetailType = (INT)Lua_ValueToNumber(L, 2);

			/*if (nParamNum > 3)
				nLevel = (INT)Lua_ValueToNumber(L, 4);

			if (nParamNum > 4)
				nSeries = (INT)Lua_ValueToNumber(L, 5);

			if (nParamNum > 5)
				P = (INT)Lua_ValueToNumber(L, 6);*/

			Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.CountCommonItem(nItemNature, nItemGenre, nDetailType, nLevel, nSeries, P));
			return 1;
		}
	}
	Lua_PushNumber(L, 0);
	return 1;
}

INT LuaGetItemCountRoom(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
			Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.GetItemCountRoom((INT)Lua_ValueToNumber(L, 1)));
	}
	return 1;
}

INT LuaGetTrade(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Player[nPlayerIndex].m_PTrade.nTrade)
		Lua_PushNumber(L, 1);
	else
		Lua_PushNumber(L, 0);
	return 1;
}

INT LuaForbidUseTownP(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;
	Player[nPlayerIndex].ForbidUseTownP(Lua_ValueToNumber(L, 1) > 0);
	return 0;
}

INT LuaIsForbidUseTownP(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	//if (Lua_GetTopIndex(L) < 2)
	//	return 0;

	if (Player[nPlayerIndex].IsForbidUseTownP())
	{
		Lua_PushNumber(L, 1);
		return 1;
	}

	return 0;
}

INT LuaForbidTrade(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;

	Player[nPlayerIndex].ForbidTrade((BOOL)Lua_ValueToNumber(L, 1));
	return 0;
}

INT LuaForbidEnmity(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;

	Player[nPlayerIndex].ForbidEnmity((BOOL)Lua_ValueToNumber(L, 1));
	return 0;
}

INT LuaForbidName(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;

	Player[nPlayerIndex].ForbidName((INT)Lua_ValueToNumber(L, 1));
	return 0;
}

INT LuaForbidCamp(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;

	Player[nPlayerIndex].ForbidCamp((BOOL)Lua_ValueToNumber(L, 1));
	return 0;
}

INT LuaForbidParty(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;

	if (Lua_GetTopIndex(L) < 2)
		return 0;

	Player[nPlayerIndex].ForbidParty((BOOL)Lua_ValueToNumber(L, 1));
	return 0;
}

INT LuaGetDistance(Lua_State* L)
{
	Lua_PushNumber(L, g_GetDistance((INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2),
		(INT)Lua_ValueToNumber(L, 3), (INT)Lua_ValueToNumber(L, 4)));
	return 1;
}

INT LuaGetDir(Lua_State* L)
{
	Lua_PushNumber(L, g_GetDirIndex((INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2),
		(INT)Lua_ValueToNumber(L, 3), (INT)Lua_ValueToNumber(L, 4)));
	return 1;
}

static INT LuaDirSin(Lua_State* L)
{
	Lua_PushNumber(L, g_DirSin((INT)Lua_ValueToNumber(L, 1), 64));
	return 1;
}

static INT LuaDirCos(Lua_State* L)
{
	Lua_PushNumber(L, g_DirCos((INT)Lua_ValueToNumber(L, 1), 64));
	return 1;
}

INT LuaFileExists(Lua_State* L)
{
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	if (g_FileExists(szFileName))
		Lua_PushNumber(L, 1);
	else
		Lua_PushNumber(L, 0);
	return 1;
}

INT LuaFileName2Id(Lua_State* L)
{
	char* szScript = (char*)Lua_ValueToString(L, 1);
	Lua_PushNumber(L, g_FileName2Id(szScript));
	return 1;
}

INT LuaTabFile_Load(Lua_State* L)
{
	KTabFile TabFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	if (TabFile.Load(szFileName))
		return 1;
	else
		return 0;
	return 1;
}

INT LuaIniFile_IsSectionExist(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	char* szSection = (char*)Lua_ValueToString(L, 2);
	if (IniFile.Load(szFileName))
	{
		if (IniFile.IsSectionExist(szSection))
		{
			Lua_PushNumber(L, 1);
			return 1;
		}
	}
	Lua_PushNumber(L, 0);
	return 1;
}

INT LuaIniFile_Load(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	if (IniFile.Load(szFileName))
		Lua_PushNumber(L, 1);
	else
		Lua_PushNumber(L, 0);
	return 1;
}

INT LuaIniFile_GetStr(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	char* szSectName = (char*)Lua_ValueToString(L, 2);
	char* szKeyName = (char*)Lua_ValueToString(L, 3);
	char szValue[128];
	if (IniFile.Load(szFileName))
	{
		IniFile.GetString(szSectName, szKeyName, "", szValue, sizeof(szValue));
		Lua_PushString(L, szValue);
		return 1;
	}
	return 0;
}

INT LuaIniFile_GetInt(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	char* szSectName = (char*)Lua_ValueToString(L, 2);
	char* szKeyName = (char*)Lua_ValueToString(L, 3);
	INT nValue;
	if (IniFile.Load(szFileName))
	{
		IniFile.GetInteger(szSectName, szKeyName, 0, &nValue);
		Lua_PushNumber(L, nValue);
		return 1;
	}
	return 0;
}

INT LuaIniFile_GetInt2(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	char* szSectName = (char*)Lua_ValueToString(L, 2);
	char* szKeyName = (char*)Lua_ValueToString(L, 3);
	INT nValue1, nValue2;
	if (IniFile.Load(szFileName))
	{
		IniFile.GetInteger2(szSectName, szKeyName, &nValue1, &nValue2);
		Lua_PushNumber(L, nValue1);
		Lua_PushNumber(L, nValue2);
		return 2;
	}
	return 0;
}

INT LuaIniFile_Save(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);

	IniFile.Save(szFileName);
	return 0;
}

INT LuaIniFile_SaveStr(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	char* szSectName = (char*)Lua_ValueToString(L, 2);
	char* szKeyName = (char*)Lua_ValueToString(L, 3);
	char* szValue = (char*)Lua_ValueToString(L, 4);
	if (IniFile.Load(szFileName))
	{
		IniFile.WriteString(szSectName, szKeyName, szValue);
		IniFile.Save(szFileName);
	}
	return 0;
}

INT LuaIniFile_SaveInt(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	char* szSectName = (char*)Lua_ValueToString(L, 2);
	char* szKeyName = (char*)Lua_ValueToString(L, 3);
	INT nValue = (INT)Lua_ValueToNumber(L, 4);
	if (IniFile.Load(szFileName))
	{
		IniFile.WriteInteger(szSectName, szKeyName, nValue);
		IniFile.Save(szFileName);
	}
	return 0;
}

INT LuaIniFile_SaveInt2(Lua_State* L)
{
	KIniFile IniFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	char* szSectName = (char*)Lua_ValueToString(L, 2);
	char* szKeyName = (char*)Lua_ValueToString(L, 3);
	INT nValue1 = (INT)Lua_ValueToNumber(L, 4);
	INT nValue2 = (INT)Lua_ValueToNumber(L, 5);
	if (IniFile.Load(szFileName))
	{
		IniFile.WriteInteger2(szSectName, szKeyName, nValue1, nValue2);
		IniFile.Save(szFileName);
	}
	return 0;
}

INT LuaTabFile_GetRowCount(Lua_State* L)
{
	KTabFile TabFile;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	INT nCount = 0;
	TabFile.Load(szFileName);
	nCount = TabFile.GetHeight();
	Lua_PushNumber(L, nCount);
	return 1;
}

INT LuaTabFile_GetCell(Lua_State* L)
{
	KTabFile TabFile;
	char szString[128];
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	if (TabFile.Load(szFileName))
	{
		if (Lua_IsNumber(L, 2) && Lua_IsNumber(L, 3))
		{
			INT nRow = (INT)Lua_ValueToNumber(L, 2);
			INT nColumn = (INT)Lua_ValueToNumber(L, 3);
			TabFile.GetString(nRow, nColumn, "", szString, sizeof(szString));
		}
		else if (Lua_IsNumber(L, 2) && Lua_IsString(L, 3))
		{
			INT nRow = (INT)Lua_ValueToNumber(L, 2);
			char szColumn[32];
			strcpy(szColumn, Lua_ValueToString(L, 3));
			TabFile.GetString(nRow, szColumn, "", szString, sizeof(szString));
		}
		else if (Lua_IsString(L, 2) && Lua_IsString(L, 3))
		{
			char szRow[32];
			char szColumn[32];
			strcpy(szRow, Lua_ValueToString(L, 2));
			strcpy(szColumn, Lua_ValueToString(L, 3));
			TabFile.GetString(szRow, szColumn, "", szString, sizeof(szString));
		}
		else
			return 0;

		Lua_PushString(L, szString);
		TabFile.Clear();
		return 1;
	}
	return 0;
}

INT LuaGetDataInt(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1) return 0;
	INT nId = (INT)Lua_ValueToNumber(L, 1);
	Lua_PushNumber(L, GameData.GetDataInt(nId));
	return 1;
}

INT LuaGetDataStr(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1) return 0;
	INT nId = (INT)Lua_ValueToNumber(L, 1);
	Lua_PushString(L, GameData.GetDataStr(nId));
	return 1;
}

INT LuaSetData(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 2) return 0;
	INT nNo = (INT)Lua_ValueToNumber(L, 1);
	char* szValue = (char*)Lua_ValueToString(L, 2);
	GameData.SetData(nNo, szValue);
	return 0;
}

INT LuaSendReport(Lua_State* L)
{
	return 0;
}

INT LuaGetTime(Lua_State* L)
{
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	Lua_PushNumber(L, timeinfo->tm_year + 1900);
	Lua_PushNumber(L, timeinfo->tm_mon + 1);
	Lua_PushNumber(L, timeinfo->tm_mday);
	Lua_PushNumber(L, timeinfo->tm_hour);
	Lua_PushNumber(L, timeinfo->tm_min);
	Lua_PushNumber(L, timeinfo->tm_sec);
	Lua_PushNumber(L, timeinfo->tm_wday);
	Lua_PushNumber(L, timeinfo->tm_yday);
	return 8;
}

INT LuaGetTimeDate(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2)
		return 0;

	time_t rawtime = (INT)Lua_ValueToNumber(L, 2) + 1451581200;

	struct tm* timeinfo = localtime(&rawtime);

	const char* pszKey = (char*)Lua_ValueToString(L, 1);

	char pszTimeFormat[256];
	if (strftime(pszTimeFormat, sizeof(pszTimeFormat), pszKey, timeinfo))
	{
		Lua_PushString(L, pszTimeFormat);
		return 1;
	}
	return 0;
}

INT LuaGetLocalDate(Lua_State* L)
{
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	const char* pszKey = (char*)Lua_ValueToString(L, 1);

	char pszTimeFormat[256];
	if (strftime(pszTimeFormat, sizeof(pszTimeFormat), pszKey, timeinfo))
	{
		Lua_PushString(L, pszTimeFormat);
		return 1;
	}
	return 0;
}

INT LuaAddDataGr(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1) return 0;

	char* szName1 = (char*)Lua_ValueToString(L, 1);
	INT nNameId = g_FileName2Id(szName1);

	KDataGroup Info;
	Info.nNameId = nNameId;
	memset(Info.nValue, 0, sizeof(Info.nValue));
	strcpy(Info.szName1, szName1);
	memset(Info.szName2, 0, sizeof(Info.szName2));

	Lua_PushNumber(L, GameData.AddDataGr(&Info));
	return 1;
}

INT LuaSetDataGr(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 8) return 0;
	INT i = 0, j = 0;
	KDataGroup Info;
	j++;
	INT nGroup = (INT)Lua_ValueToNumber(L, j);
	j++;
	Info.nNameId = (INT)Lua_ValueToNumber(L, j);
	j++;
	for (i = 0; i < MAX_DATAGROUP_VALUE; i++)
		Info.nValue[i] = (INT)Lua_ValueToNumber(L, j + i);
	strcpy(Info.szName1, (char*)Lua_ValueToString(L, MAX_DATAGROUP_VALUE + j));
	strcpy(Info.szName2, (char*)Lua_ValueToString(L, MAX_DATAGROUP_VALUE + j + 1));
	GameData.SetDataGr(nGroup, &Info);
	return 0;
}

INT LuaGetDataGr(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1) return 0;

	INT nNo = (INT)Lua_ValueToNumber(L, 1);

	Lua_PushNumber(L, GameData.GetDataGr_dwName(nNo));
	for (INT j = 0; j < MAX_DATAGROUP_VALUE; j++)
		Lua_PushNumber(L, GameData.GetDataGr_nValue(nNo, j));
	Lua_PushString(L, GameData.GetStrDataGr_Name1(nNo));
	Lua_PushString(L, GameData.GetStrDataGr_Name2(nNo));
	return 7;

}

INT LuaFindDataId(Lua_State* L)
{
	if (Lua_GetTopIndex(L) < 1) return 0;

	INT nNo = (INT)Lua_ValueToNumber(L, 1);

	Lua_PushNumber(L, GameData.FindDataId((DWORD)Lua_ValueToNumber(L, 1)));
	return 1;
}

INT LuaSaveDataFile(Lua_State* L)
{
	GameData.Save();
	return 0;
}

INT LuaChatRoom_Create(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Lua_GetTopIndex(L) < 4) return 0;
	BOOL bIsGmRoom = FALSE;
	if (Lua_GetTopIndex(L) > 4)
		bIsGmRoom = (BOOL)Lua_ValueToNumber(L, 4);
	if (Player[nPlayerIndex].m_cRoom.CreateChatRoom((char*)Lua_ValueToString(L, 1), (INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3), bIsGmRoom))
		Lua_PushNumber(L, 1);
	else
		Lua_PushNumber(L, 0);
	return 1;
}

INT LuaChatRoom_AddTime(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex <= 0) return 0;
	if (Lua_GetTopIndex(L) < 2) return 0;
	if (Player[nPlayerIndex].m_cRoom.AddTime((char*)Lua_ValueToString(L, 1), (INT)Lua_ValueToNumber(L, 2)))
		Lua_PushNumber(L, 1);
	else
		Lua_PushNumber(L, 0);
	return 1;
}
#endif

INT LuaGetCurServerSec(Lua_State* L)
{
	Lua_PushNumber(L, KSG_GetCurSec());
	return 1;
}

INT LuaSetNumber(Lua_State* L)
{
	INT nResult = 0;
	if (Lua_GetTopIndex(L) < 4)
		goto lab_setnumber;

	nResult = KSG_StringSetValue((INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3), (INT)Lua_ValueToNumber(L, 4));

lab_setnumber:
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaGetNumber(Lua_State* L)
{
	INT nResult = 0;
	if (Lua_GetTopIndex(L) < 3)
		goto lab_getnumber;
	nResult = KSG_StringGetValue((INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3));

lab_getnumber:
	Lua_PushNumber(L, nResult);
	return 1;
}

INT LuaGetNameItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx > 0)
	{
		Lua_PushString(L, Item[nIdx].GetName());
		return 1;
	}
	return 0;
}

INT LuaGetParamItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx > 0)
	{
		Lua_PushNumber(L, Item[nIdx].GetParam());
		return 1;
	}
	return 0;
}

INT LuaGetFortuneItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx > 0)
	{
		Lua_PushNumber(L, Item[nIdx].GetFortune());
		return 1;
	}
	return 0;
}

INT LuaGetLockItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx > 0)
	{
		Lua_PushNumber(L, Item[nIdx].GetLock()->nState);
		if (KSG_GetCurSec() < Item[nIdx].GetLock()->dwLockTime)
			Lua_PushNumber(L, Item[nIdx].GetLock()->dwLockTime - KSG_GetCurSec());
		else
			Lua_PushNumber(L, Item[nIdx].GetLock()->dwLockTime);
		return 2;
	}
	Lua_PushNumber(L, 0);
	Lua_PushNumber(L, 0);
	return 2;
}

INT LuaGetStackItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx > 0)
	{
		Lua_PushNumber(L, Item[nIdx].GetStackNum());
		return 1;
	}
	return 0;
}

INT LuaGetMaxStackItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx > 0)
	{
		Lua_PushNumber(L, Item[nIdx].GetMaxStackNum());
		return 1;
	}
	return 0;
}

INT LuaGetMantleItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1) return 0;

	INT nIdx = (INT)Lua_ValueToNumber(L, 1);
	if (nIdx > 0)
	{
		Lua_PushNumber(L, Item[nIdx].GetMantle());
		return 1;
	}
	return 0;
}

INT LuaFindItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			if (nParamNum <= 1)
			{
				INT nIdx = Player[nPlayerIndex].m_ItemList.FindSame((INT)Lua_ValueToNumber(L, 1));
				if (nIdx)
				{
					Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nPlace);
					Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nX);
					Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nY);
					return 3;
				}
				else
					return 0;
			}
			INT nItemGenre, nDetailType, nLevel, nSeries;
			nItemGenre = nDetailType = nLevel = nSeries = -1;

			nItemGenre = (INT)Lua_ValueToNumber(L, 1);

			if (nParamNum > 1)
				nDetailType = (INT)Lua_ValueToNumber(L, 2);

			if (nParamNum > 2)
				nLevel = (INT)Lua_ValueToNumber(L, 3);

			if (nParamNum > 3)
				nSeries = (INT)Lua_ValueToNumber(L, 4);

			INT nIdx = Player[nPlayerIndex].m_ItemList.FindItem(NATURE_NORMAL, nItemGenre, nDetailType, nLevel, nSeries);

			if (nIdx)
			{
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nIdx);
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nPlace);
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nX);
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nY);
				return 4;
			}
		}
	}
	Lua_PushNumber(L, 0);
	Lua_PushNumber(L, 0);
	Lua_PushNumber(L, 0);
	Lua_PushNumber(L, 0);
	return 4;
}

INT LuaFindGoldItem(Lua_State* L)
{
	INT nParamNum = Lua_GetTopIndex(L);
	if (nParamNum > 0)
	{
		INT nPlayerIndex = GetPlayerIndex(L);
		if (nPlayerIndex > 0)
		{
			INT nIndex = 0;

			nIndex = (INT)Lua_ValueToNumber(L, 1);

			INT nIdx = Player[nPlayerIndex].m_ItemList.FindItemByIndex(NATURE_GOLD, nIndex);

			if (nIdx)
			{
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nIdx);
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nPlace);
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nX);
				Lua_PushNumber(L, Player[nPlayerIndex].m_ItemList.m_Items[nIdx].nY);
				return 4;
			}
		}
	}
	Lua_PushNil(L);
	return 1;
}

INT LuaFindItemEx(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}
	INT nParamNum = Lua_GetTopIndex(L);

	if (nParamNum <= 2)
	{
		INT nIndex, nItemGenre = -1, nDetail = -1, nParticur = -1, nLevel = -1, nSeries = series_num, nLuck = 0, nStackNum = 0;
		nIndex = (INT)Lua_ValueToNumber(L, 1);
		if (nIndex > 0)
		{
			nItemGenre = Item[nIndex].GetGenre();
			nDetail = Item[nIndex].GetDetailType();
			nParticur = Item[nIndex].GetParticular();
			nLevel = Item[nIndex].GetLevel();
			nSeries = Item[nIndex].GetSeries();
			nLuck = Item[nIndex].m_GeneratorParam.nLuck;
			nStackNum = Item[nIndex].GetStackNum();
		}

		Lua_PushNumber(L, nItemGenre);
		Lua_PushNumber(L, nDetail);
		Lua_PushNumber(L, nParticur);
		Lua_PushNumber(L, nLevel);
		Lua_PushNumber(L, nSeries);
		Lua_PushNumber(L, nLuck);
		Lua_PushNumber(L, nStackNum);
		return 7;
	}

	INT nIndex = 0, nItemGenre = -1, nDetail = -1, nParticur = -1, nLevel = -1, nSeries = series_num, nLuck = 0, nStackNum = 0;
	if (nParamNum > 2)
	{
		nIndex = Player[nPlayerIndex].m_ItemList.PositionToIndex((INT)Lua_ValueToNumber(L, 1), (INT)Lua_ValueToNumber(L, 2));
	}
	if (nParamNum > 3)
	{
		nIndex = Player[nPlayerIndex].m_ItemList.m_Room[PositionToRoom((INT)Lua_ValueToNumber(L, 1))].FindItem((INT)Lua_ValueToNumber(L, 2), (INT)Lua_ValueToNumber(L, 3));
	}

	if (nIndex > 0)
	{
		nItemGenre = Item[nIndex].GetGenre();
		nDetail = Item[nIndex].GetDetailType();
		nParticur = Item[nIndex].GetParticular();
		nLevel = Item[nIndex].GetLevel();
		nSeries = Item[nIndex].GetSeries();
		nLuck = Item[nIndex].m_GeneratorParam.nLuck;
		nStackNum = Item[nIndex].GetStackNum();
	}
	Lua_PushNumber(L, nIndex);
	Lua_PushNumber(L, nItemGenre);
	Lua_PushNumber(L, nDetail);
	Lua_PushNumber(L, nParticur);
	Lua_PushNumber(L, nLevel);
	Lua_PushNumber(L, nSeries);
	Lua_PushNumber(L, nLuck);
	Lua_PushNumber(L, nStackNum);
	return 8;
}

INT LuaPlayMusic(Lua_State* L)//PlayMusic(musicname,loop=1, vol );
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	INT nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_PLAYMUSIC;
	UiInfo.m_bOptionNum = 1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	INT nMsgId = 0;

	g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L, 1), sizeof(UiInfo.m_pContent));
	UiInfo.m_nBufferLen = strlen(((char*)UiInfo.m_pContent));
	UiInfo.m_bParam1 = 0;

#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif

	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

INT LuaFadeInMusic(Lua_State* L)
{
	return 0;
}

INT LuaFadeOutMusic(Lua_State* L)
{
	return 0;
}

INT LuaReloadScript(Lua_State* L)
{
	INT nPlayerIndex = GetPlayerIndex(L);

	if (nPlayerIndex <= 0)
	{
		Lua_PushNumber(L, 0);
		return 1;
	}

	INT nNumberScript = 0;
	char* szFileName = (char*)Lua_ValueToString(L, 1);
	if (szFileName[0] == 0)
	{
		nNumberScript = ReLoadAllScript();
		printf("Reloaded %d Script\n", nNumberScript);
	}
	else
	{
		ReLoadScript(szFileName);
		printf("Reloaded Script: %s\n", szFileName);
	}

	return 1;
}

INT LuaReloadAllScript(Lua_State* L)
{
	INT nNumberScript = 0;

	nNumberScript = ReLoadAllScript();
	printf("Reloaded %d Script\n", nNumberScript);

	return 1;
}

INT LuaSplitString(Lua_State* L)
{
	return 1;
}


#ifndef _SERVER

INT LuaPlaySound(Lua_State* L)
{
	return 0;
}

INT LuaPlaySprMovie(Lua_State* L)
{
	return 0;
}
#endif
TLua_Funcs GameScriptFuns[] =
{
	{"Say", LuaSelectUI},
	{"Talk", LuaTalkUI},
	{"Sel", LuaSelUI},
	{"GetTaskTemp", LuaGetTempTaskValue},
	{"GetTaskTempS", LuaGetTempTaskString},
	{"SetTaskTemp", LuaSetTempTaskValue},
	{"Message", LuaMessage},

	{"GetBit",	LuaGetBit},
	{"GetByte",	LuaGetByte},
	{"SetBit",	LuaSetBit},
	{"SetByte",	LuaSetByte},
	{"Include",LuaIncludeFile},
	{"IncludeLib",LuaIncludeLib},
	{"AddNews",LuaAddGlobalNews},
	{"AddNews2",LuaAddGlobalNews2},
	{"AddTimeNews",LuaAddGlobalTimeNews},
	{"AddTimeNews2",LuaAddGlobalTimeNews2},
	{"AddCountNews",LuaAddGlobalCountNews},
	{"AddCountNews2",LuaAddGlobalCountNews2},
	{"AddLocalNews",LuaAddLocalNews},
	{"AddLocalTimeNews",LuaAddLocalTimeNews},
	{"AddLocalCountNews",LuaAddLocalCountNews	},


#ifdef _SERVER
	{"PutMessage"	,	LuaSendMessageInfo},
	{"GetGameTime",	LuaGetGameTime},
	{"GetLoginTime", LuaGetPlayerLoginTime},
	{"GetOnlineTime", LuaGetPlayerOnlineTime},
	{"OfflineLive", LuaOfflineLive},

	{"SetValue", LuaSetValue},
	{"AddValue", LuaAddValue},
	{"GetValue",LuaGetValue},
	{"SetRepute", LuaSetPlayerReputeValue},
	{"AddRepute", LuaAddPlayerReputeValue},
	{"GetRepute",LuaGetPlayerReputeValue},
	{"AddTranslife", LuaAddPlayerTranslifeValue},
	{"SetTranslife", LuaSetPlayerTranslifeValue},
	{"GetTranslife",LuaGetPlayerTranslifeValue},
	{"AddViprank", LuaAddPlayerViprankValue},
	{"GetViprank",LuaGetPlayerViprankValue},
	{"SetFuYuan", LuaSetPlayerFuYuanValue},
	{"AddFuYuan", LuaAddPlayerFuYuanValue},
	{"GetFuYuan",LuaGetPlayerFuYuanValue},
	{"AddAccum", LuaAddPlayerAccumValue},
	{"GetAccum",LuaGetPlayerAccumValue},
	{"AddHonor", LuaAddPlayerHonorValue},
	{"GetHonor",LuaGetPlayerHonorValue},
	{"AddRespect", LuaAddPlayerRespectValue},
	{"GetRespect",LuaGetPlayerRespectValue},
	{"GetNpcIdx", LuaGetCurNpcIndex},

	{"SetTimer",		LuaSetTimer},		//SetTimer(Ê±¼äÁ¿, Ê±¼äTaskId):¸øÍæ¼Ò´ò¿ª¼ÆÊ±Æ÷,Ê±¼äµ½Ê±½«×Ô¶¯µ÷ÓÃOnTimerº¯Êý
	{"StopTimer",		LuaStopTimer},		//StopTimer()£º¹Ø±Õµ±Ç°Íæ¼ÒµÄ¼ÆÊ±Æ÷
	{"GetRestTime",		LuaGetRestTime},	//GetRestTime:»ñµÃ¼ÆÊ±Æ÷½«´¥·¢µÄÊ£ÓÚÊ±¼ä	
	{"GetTimerId",		LuaGetCurTimerId},	//CurTimerId = GetTimerId():»ñµÃµ±Ç°Ö´ÐÐµÄ¼ÆÊ±Æ÷µÄid,Èç¹ûÃ»ÓÐÔò·µ»Ø0
	{"GetTask",			LuaGetTaskValue},	//GetTask(ÈÎÎñºÅ):»ñµÃµ±Ç°Íæ¼Ò¸ÃÈÎÎñºÅµÄÖµ
	{"GetTaskS",		LuaGetTaskString},	//GetTask(ÈÎÎñºÅ):»ñµÃµ±Ç°Íæ¼Ò¸ÃÈÎÎñºÅµÄÖµ
	{"SetTask",			LuaSetTaskValue},	//SetTask(ÈÎÎñºÅ,Öµ):ÉèÖÃÈÎÎñÖµ
	{"SyncTaskValue",	LuaSyncTaskValue},	//SetTask(ÈÎÎñºÅ,Öµ):ÉèÖÃÈÎÎñÖµ
	{"IsCaptain",		LuaIsLeader},		//IsCaptain()ÊÇ·ñÎª¶Ó³¤
	{"GetTeam",			LuaGetTeamId},		//GetTeam()·µ»ØÍæ¼Ò¶ÓÎéID
	{"GetTeamSize",		LuaGetTeamSize},	//GetTeamSize()
	{"GetTeamMem",		LuaGetTeamMem},
	{"LeaveTeam",		LuaLeaveTeam},		//LeaveTeam()ÈÃÍæ¼ÒÀë¿ª×ÔÉí¶ÓÎé
	{"TeamDoScript",	LuaTeamDoScript},	//TeamDoScript(links,func) return number player has set
	{"Msg2Player",		LuaMsgToPlayer	},	//Msg2Player(ÏûÏ¢)
	{"Msg2Team",		LuaMsgToTeam},		//Msg2Team(ÏûÏ¢)Í¨ÖªÍæ¼ÒµÄ×é
	{"Msg2SubWorld",	LuaMsgToSubWorld},	//Msg2SubWorld(ÏûÏ¢)Í¨ÖªÊÀ½ç
	{"Msg2Faction",		LuaMsgToFaction},
	{"Msg2Tong",		LuaMsgToTong},
	{"Msg2Room",		LuaMsgToChatRoom},
	{"Msg2Region",		LuaMsgToAroundRegion},//Msg2Region(ÏûÏ¢)Í¨ÖªÖÜÎ§Region
	{"Msg2GM",			LuaMsgToGameMaster}, //Msg2GM(StrInfo)
	{"Msg2IP",			LuaMsgToIP}, //Msg2IP(IP, ID, StrInfo)
	{"GetIP",			LuaGetIP},
	{"GetIP2",			LuaGetIP2},
	{"GetNumberPlayerByIp",			LuaGetNumberPlayerByIp},//LuaGetNumberPlayerByIp
	{"GetSubWorldConnectionByIP",			LuaGetSubWorldConnectionByIP},
	{"SetPos",			LuaSetPos},			//SetPos(x,y)½øÈëÄ³µã	
	{"GetPos",			LuaGetPos},			//GetPos() return x,y,subworldindex
	{"GetWorldPos",		LuaGetNewWorldPos},	//W,X,Y = GetWorldPos()·µ»ØÓÚNewWorldÅäÅãµÄ×ø±ê
	{"GetWorldName",	LuaGetNewWorldName},
	{"GetWorldKind",	LuaGetNewWorldKind},
	{"GetFreePos",		LuaGetFreeObjPos},
	{"GetNpcPos",		LuaGetNpcPos},
	{"NewWorld",		LuaEnterNewWorld},
	{"NpcNewWorld",		LuaNpcEnterNewWorld},

	{"SetProtectTime",		LuaSetProtectTime},
	{"DropMoney",		LuaDropMoney},
	{"DropItem",		LuaDropItem},
	{"DropMapItem",		LuaDropMapItem},
	///////////////////////////////////////////
		{"AddItem",				LuaAddItem},
		{"GetItemName",			LuaGetItemName},
		{"GetGoldIndex",		LuaGetGoldIndex},
		{"AddItemEx",			LuaAddItemNew},
		{"AddGoldItem",			LuaAddGoldItem},
		{"AddPlatinaItem",		LuaAddPlatinaItem},
		{"AddItemIdx",			LuaAddItemIdx},
		{"AddItemID",			LuaAddItemID},
		{"AddItemIDStack",		LuaAddItemIDStack},
		{"AddStackItem",		LuaAddStackItem },

		{"DelItem",				LuaDelItem},
		{"ConsumeItem",			LuaDelItem },
		{"DelItemEx",			LuaDelItemEx },
		{"RepairBroken",		LuaRepairBroken },
		{"GetDurability",		LuaGetDurability },
		{"GetItemCount",		LuaGetItemCount},
		{"GetItemCountNew",		LuaGetItemCountNew}, //GetItemCountNew(Genre, P)
		{"GetItemCountRoom",	LuaGetItemCountRoom},

		{"GetMagicAttrib",		LuaGetMagicAttrib},
		{"SetMagicAttrib",		LuaSetMagicAttrib },
		{"GetMaxAttrib",		LuaGetMaxAttrib },
		{"GetMinAttrib",		LuaGetMinAttrib },


		{"AddMagic",		LuaAddMagic},		//AddMagic(Ä§·¨id»òÄ§·¨Ãû)¸øÍæ¼Ò¼ÓÄ³¸öÄ§·¨
		{"DelMagic",		LuaDelMagic},		//DelMagic(Ä§·¨id»òÄ§·¨Ãû)
		{"DelAllMagic",		LuaDelAllMagic},		//DelMagic(Ä§·¨id»òÄ§·¨Ãû)
		{"HaveMagic",		LuaHaveMagic},		//HaveMagic(Ä§·¨id»òÄ§·¨Ãû)·µ»Ø0»ò1
		{"IncSkill",		LuaIncSkill},
		{"IncSkillExp",		LuaIncSkillExp },
		{"GetSkillExp",		LuaGetSkillExp },
		{"GetMagicLevel",	LuaGetMagicLevel},	//GetMagicLevel(Ä§·¨id»òÄ§·¨Ãû)·µ»ØµÈ¼¶
		{"AddMagicPoint",	LuaAddMagicPoint },
		{"DelMagicPoint",	LuaRemoveMagicPoint },
		{"GetMagicPoint",	LuaGetMagicPoint},

		{"SubWorldID2Idx",	LuaSubWorldIDToIndex}, //SubWorldID2Idx
		{"SubWorldIdx2ID",	LuaSubWorldIndexToID}, //SubWorldID2Idx


		{"AddLeadExp",		LuaAddLeadExp},
		{"SetLeadLevel",	LuaSetLeadLevel},
		{"GetLeadLevel",	LuaGetLeadLevel},

		{"SetFightState",	LuaSetFightState},
		{"GetFightState",	LuaGetFightState},

		{"GetNpcTemplateID",LuaGetNpcTemplateID},
		{"GetNpcSettingIdx",LuaGetNpcTemplateID},
		{"GetNpcTempName",	LuaGetNpcTempName},
		{"GetNpcTempTypeName",	LuaGetNpcTempTypeName},
		{"AddNpc",			LuaAddNpc},			//AddNpc(ÈËÎïÄ£°åid»òÈËÎïÄ£°åÃû,Ëù´¦ÊÀ½çid£¬µã×ø±êx,µã×ø±êy),·µ»ØnpcidÖµ
		{"DelNpc",			LuaDelNpc},			//DelNpc(Npcid)
		{"ClearMapNpc",		LuaClearMapNpc},
		{"ClearMapNpcWithName",		LuaClearMapNpcWithName},
		{"SetNpcKind",		LuaSetNpcKind},
		{"SetNpcSeries",	LuaSetNpcSeries},
		{"SetNpcSer",		LuaSetNpcSeries},
		{"GetNpcSer",		LuaGetNpcSeries},
		{"SetNpcExp",		LuaSetNpcExp},
		{"SetNpcLife",		LuaSetNpcLife},
		{"GetNpcLife",		LuaGetNpcLife},
		{"SetNpcReplenish",	LuaSetNpcLifeReplenish},
		{"SetNpcAR",		LuaSetNpcAR},
		{"SetNpcDefense",	LuaSetNpcDefense},
		{"SetNpcDamage",	LuaSetNpcDamage},
		{"SetNpcDmgEx",		LuaSetNpcDmgEx},
		{"SetNpcResist",	LuaSetNpcResist},
		{"SetNpcRevTime",	LuaSetNpcRevTime},
		{"SetNpcSpeed",		LuaSetNpcSpeed},
		{"SetNpcHitRecover",LuaSetNpcHitRecover},
		{"SetNpcBoss",		LuaSetNpcBoss},
		{"GetNpcBoss",		LuaGetNpcBoss},
		{"IsBlueBoss",		LuaIsBlueBoss},
		{"GetNpcExpRate",	LuaGetNpcExpRate},
		{"IsRideHorse",		LuaIsRideHorse},
		{"SyncNpc",			LuaSyncNpc},
		{"SetNpcPos",		LuaSetNpcPos},
		{"SetNpcScript",	LuaSetNpcActionScript},	//SetNpcScript(npcid, ½Å±¾ÎÄ¼þÃû)ÉèÖÃnpcµ±Ç°µÄ½Å±¾
		{"SetNpcName",		LuaSetNpcName},
		{"GetNpcName",		LuaGetNpcName },
		{"ChangeRoleName",	LuaCheckDataName },
		{"CheckDataName",	LuaCheckDataName },
		{"GetNpcID",		LuaGetNpcID},
		{"SetNpcSkill",		LuaSetNpcSkill},
		{"SetNpcDropScript",LuaSetNpcDropScript},
		{"SetNpcRemoveDeath",	LuaSetNpcRemoveDeath},
		{"SetNpcTimeout",	LuaSetNpcTimeout},
		{"GetNpcTimeout",	LuaGetNpcTimeout},
		{"SetNpcParam",		LuaSetNpcParam},
		{"SetNpcValue",		LuaSetNpcParam},
		{"GetNpcParam",		LuaGetNpcParam},
		{"GetNpcValue",		LuaGetNpcParam},
		{"SetNpcOwner",		LuaSetNpcOwner},
		{"GetNpcOwner",		LuaGetNpcOwner},
		{"SetNpcFindPathTime",LuaSetNpcFindPathTime},
		{"SetNpcCurCamp",	LuaSetNpcCurCamp},
		{"SetRevPos",		LuaSetPlayerRevivalPos},//SetRevPos(µãÎ»ÖÃX£¬µãÎ»ÖÃY)ÉèÖÃÍæ¼ÒµÄµ±Ç°ÊÀ½çµÄµÈÈëµãÎ»ÖÃ
		{"Rev2Pos",			LuaGetPlayerRevivalPos},//SetRevPos(µãÎ»ÖÃX£¬µãÎ»ÖÃY)ÉèÖÃÍæ¼ÒµÄµ±Ç°ÊÀ½çµÄµÈÈëµãÎ»ÖÃ
		{"GetCurRev",		LuaGetPlayerRevival},
		{"GetCurRevID",		LuaGetPlayerRevivalID},
		{"SetTempRevPos",	LuaSetDeathRevivalPos}, //SetTempRevPos(subworldid, x, y ) or SetTempRevPos(id);
		{"Revive",			LuaPlayerExecuteRevive}, //SetTempRevPos(subworldid, x, y ) or SetTempRevPos(id);
		{"GetCurCamp",		LuaGetPlayerCurrentCamp},//GetCurCamp()»ñµÃÍæ¼ÒµÄµ±Ç°ÕóÓª
		{"GetCamp",			LuaGetPlayerCamp},//GetCamp()»ñµÃÍæ¼ÒÕóÓª
		{"SetCurCamp",		LuaSetPlayerCurrentCamp},//SetCurCamp(ÕóÓªºÅ):ÉèÖÃÍæ¼Òµ±Ç°ÕóÓª
		{"SetCamp",			LuaSetPlayerCamp},		  //SetCamp(ÕóÓªºÅ):ÉèÖÃÕóÓª	
		{"RestoreCamp",		LuaRestorePlayerCamp},//RestoreCamp()»Ö¸´ÕóÓª
		{"GetFaction",		LuaGetPlayerFaction,},//GetFaction()»ñµÃÍæ¼ÒµÄÃÅÅÉÃû 
		{"GetFactionName",	LuaGetPlayerFactionName,},
		{"GetFactionCamp",	LuaGetPlayerFactionCamp,},
		{"GetFactionNo",	LuaGetPlayerFactionNo},
		{"SetFaction",		LuaChangePlayerFaction},  //SetFaction(ÃÅÅÉÃû):ÉèÖÃÍæ¼ÒÃÅÅÉÃû
		{"GetColdR",		LuaGetPlayerColdResist},
		{"SetColdR",		LuaSetPlayerColdResist},
		{"GetFireR",		LuaGetPlayerFireResist},
		{"SetFireR",		LuaSetPlayerFireResist},
		{"GetLightR",		LuaGetPlayerLightResist},
		{"SetLightR",		LuaSetPlayerLightResist},
		{"GetPoisonR",		LuaGetPlayerPoisonResist},
		{"SetPoisonR",		LuaSetPlayerPoisonResist},
		{"GetPhyR",			LuaGetPlayerPhysicsResist},
		{"SetPhyR",			LuaSetPlayerPhysicsResist},
		{"GetNextExp",		LuaGetNextExp},			//GetExp():»ñµÃÍæ¼ÒµÄµ±Ç°¾­ÑéÖµ
		{"GetExp",			LuaGetPlayerExp	},			//GetExp():»ñµÃÍæ¼ÒµÄµ±Ç°¾­ÑéÖµ
		{"AddExp",			LuaModifyPlayerExp},		//AddExp(¾­ÑéÖµ£¬¶Ô·½µÈ¼¶£¬ÊÇ·ñ×é¶Ó¹²Ïí¾­ÑéÖµ)
		{"AddOwnExp",		LuaAddOwnExp},				//AddOwnExp(Exp)£¬¸øÍæ¼ÒÖ±½Ó¼Ó¾­Ñé
		{"AddStackExp",		LuaAddStackExp},				//AddSumExp(Exp)£¬¸øÍæ¼ÒÖ±½Ó¼Ó¾­Ñé
		{"GetLife",			LuaGetPlayerLife},			//GetLife()»ñµÃÍæ¼ÒµÄÉúÃüÖµ
		{"RestoreLife",		LuaRestorePlayerLife},		//RestoreLife()»Ö¸´Íæ¼ÒµÄÉúÃü
		{"GetMana",			LuaGetPlayerMana},			//GetMana()»ñµÃÍæ¼ÒµÄMana
		{"RestoreMana",		LuaRestorePlayerMana},		//RestoreMana()»Ö¸´Íæ¼ÒµÄMana
		{"GetStamina",		LuaGetPlayerStamina},		//GetStamina()»ñµÃÍæ¼ÒStamina
		{"RestoreStamina",	LuaRestorePlayerStamina},	//RestoreMana()»Ö¸´Íæ¼ÒµÄStamina
		{"GetDefend",		LuaGetPlayerDefend},		//GetDefend()»ñµÃÍæ¼ÒµÄ·ÀÓùÁ¦
		{"GetSex",			LuaGetPlayerSex },			//GetSex()»ñµÃÍæ¼ÒµÄÐÔ±
		{"SetSex",			LuaSetPlayerSex },			//GetSex()»ñµÃÍæ¼ÒµÄÐÔ±ðLuaSetPlayerSex
		{"GetSeries",		LuaGetPlayerSeries},		//GetSeries()»ñµÃÍæ¼ÒµÄÏµ0man/1woman
		{"SetSeries",		LuaSetPlayerSeries},		//SetSeries(ÐÔ±ðºÅ)
		{"GetName",			LuaGetPlayerName},			//GetName()»ñµÃÍæ¼ÒµÄÐÕÃû
		{"GetPlayerNpcIdx",	LuaGetPlayerNpcIdx},
		{"GetMateName",		LuaGetMateName},
		{"GetAccount",		LuaGetPlayerAccount},
		{"GetUUID",			LuaGetPlayerID},			//GetUUID()»ñµÃÍæ¼ÒµÄÎ¨Ò»ID
		{"FindPlayer",		LuaFindPlayer },			//GetUUID()»ñµÃÍæ¼ÒµÄÎ¨Ò»ID 
		{"FindPlayerAccount",		LuaFindPlayerAccount},			//GetUUID()»ñµÃÍæ¼ÒµÄÎ¨Ò»ID LuaFindPlayerByAccount
		{"FindNearNpc",		LuaFindNearNpc},
		{"FindAroundNpc",	LuaFindAroundNpc},
		{"GetLeadExp",		LuaGetPlayerLeadExp},		//GetLeadExp()»ñµÃÍæ¼ÒµÄÍ³ÂÊ¾­ÑéÖµ
		{"GetLeadLevel",	LuaGetPlayerLeadLevel},		//GetLeadLevel()»ñµÃÍæ¼ÒµÄÍ³ÂÊµÈ¼¶
		{"SetLevel",		LuaSetLevel },
		{"ST_LevelUp",		LuaSTLevelUp },
		{"GetLevel",		LuaGetLevel},				//GetLevel()GetPlayers Level
		{"GetPlayerLevel",	LuaGetLevel},				//GetLevel()GetPlayers Level
		{"GetRestAP",		LuaGetPlayerRestAttributePoint},//GetRestAP()»ñµÃÍæ¼ÒµÄÊ£ÓÚÊôÐÔµãÊý
		{"GetRestSP",		LuaGetPlayerRestSkillPoint},	//GetRestSP()»ñµÃÍæ¼ÒµÄÊ£ÓÚ¼¼ÄÜµãÊý	
		{"GetLucky",		LuaGetPlayerLucky},			//GetLucky()»ñµÃÍæ¼ÒµÄÐÒÔËÖµ
		{"GetEng",			LuaGetPlayerEngergy},		//GetEng()»ñµÃÍæ¼ÒµÄÁ¦Á¿ÖµEng
		{"AddEng",			LuaSetPlayerEngergy},		//AddEng(Value)»ñµÃÍæ¼ÒµÄÁ¦Á¿ÖµEng
		{"GetDex",			LuaGetPlayerDexterity},		//GetDex()»ñµÃÍæ¼ÒµÄDex
		{"AddDex",			LuaSetPlayerDexterity},		//AddDex(Value)»ñµÃÍæ¼ÒµÄÁ¦Á¿ÖµEng
		{"GetStrg",			LuaGetPlayerStrength},		//GetStrg()
		{"AddStrg",			LuaSetPlayerStrength},		//AddStrg(Value)»ñµÃÍæ¼ÒµÄÁ¦Á¿ÖµEng
		{"GetVit",			LuaGetPlayerVitality},		//GetVit()
		{"AddVit",			LuaSetPlayerVitality},		//AddVit(Value)»ñµÃÍæ¼ÒµÄÁ¦Á¿ÖµEng
		{"ResetBaseAttrib",	LuaResetBaseAttribute},		// ÉèÖÃ»ù±¾ÊôÐÔ
		{"ResetProp",		LuaResetProp},
		{"GetCash",			LuaGetPlayerCashMoney},		//GetCash()»ñµÃÍæ¼ÒµÄÏÖ½ð
		{"Pay",				LuaPlayerPayMoney},			//Pay(½ð¶îÊý)¿Û³ýÍæ¼Ò½ðÇ®³É¹¦·µ»Ø1£¬Ê§°Ü·µ»Ø0
		{"Earn",			LuaPlayerEarnMoney},		//Earn(½ð¶îÊý)Ôö¼ÓÍæ¼Ò½ðÇ®
		{"PrePay",			LuaPlayerPrePayMoney},		//¸¶¶¨½ð£¬³É¹¦·µ»Ø1£¬Ê§°Ü·µ»Ø0
		{"GetPlayerFortune",LuaGetPlayerFortune},		//GetCash()»ñµÃÍæ¼ÒµÄÏÖ½ð
		{"AttackNpc",		LuaAttackNpc},				//AttackNpc(NpcDwid,ÎïÀíÉËº¦Öµ£¬±ù£¬»ð£¬µç£¬¶¾£©
		{"KillNpc",			LuaKillNpc},				//KillNpc(NpcDWID)
		{"KillPlayer",		LuaKillPlayer},				//KillPlayer();
		{"Sale",			LuaSale},					//Sale(SaleId)ÂòÂô£¬SaleIdÎª±ãÂôµÄÎïÆ·ÐÅÏ¢ÁÐ±íid
		{"NewSale",			LuaNewSale},
		{"UseTownPortal",	LuaUseTownPortal},
		{"ReturnFromPortal",LuaReturnFromTownPortal},
		{"OpenBox",			LuaOpenBox},
		{"OpenEquipEx",		LuaOpenEquipEx},
		{"AddStation",		LuaAddPlayerStation},
		{"AddTermini",		LuaAddPlayerWayPoint},
		{"GetStation",		LuaGetPlayerStation	},
		{"GetStationCount", LuaGetPlayerStationCount},
		{ "ExeScript",		LuaPlayerExecuteScript },	      //ExeScript(脚本文件名,参数),执行时会调用main函数	
		{ "LocalScript",	LuaLocalScript },	              //重载脚本
		{ "LocalAllScript",	LuaLocalAllScript },	              //重载全部脚本

		{"TrembleItem",		LuaOpenTrembleItem }, // TamLTM Kham nam Xanh
		{"GetPOItem",		LuaGetPOItem }, // TamLTM Kham nam
		{"SetPItemID",		LuaSetPItemID }, // TamLTM Kham nam

		{"GetCityCount",	LuaGetAllStationCount},
		{"GetCity",			LuaGetCity},

		{"GetWayPoint",			LuaGetPlayerWayPoint},
		{"GetStationName",		LuaGetStationName},
		{"GetWayPointName",		LuaGetWayPointName},
		{"GetPrice2Station",	LuaGetPriceToStation},
		{"GetPrice2WayPoint",	LuaGetPriceToWayPoint	},
		{"GetStationPos",		LuaGetStationPos},
		{"GetWayPointPos",		LuaGetWayPointPos},
		{"GetWayPointFight",	LuaGetWayPointFight},
		{"GetPlayerCount",		LuaGetPlayerCount},
		{"GetNpcCount",			LuaGetNpcCount},
		{"GetTotalItem",		LuaGetTotalItem},
		{"GetRank",				LuaGetRank},
		{"SetRank",				LuaSetRank},
		{"SetRankEx",			LuaSetExpandRank},
		{"GetRankEx",			LuaGetExpandRank},
		{"RestoreRankEx",		LuaRestoreExpandRank},
		{"GetEquipItemEx",		LuaGetEquipItemEx},
		{"SetEquipItemEx",		LuaSetEquipItemEx},
		{"GetExpandBox",		LuaGetExpandBox},
		{"SetExpandBox",		LuaSetExpandBox},
		{"SetPropState",		LuaSetObjPropState},
		{"GetServerName",		LuaGetServerName},

		//------------------Station Script ---------------
		{"GetWharfName",	LuaGetDockName},
		{"GetWharfCount",	LuaGetDockCount},
		{"GetWharfPrice",	LuaGetDockPrice},
		{"GetWharf",		LuaGetDock},
		{"GetWharfPos",		LuaGetDockPos},
		{"GetTerminiFState", LuaGetWayPointFightState},
		//------------------------------------------------
		{"KickOutPlayer",	LuaKickOutPlayer},
		{"KickOutSelf",		LuaKickOutSelf},
		{"KickOutAccount",	LuaKickOutAccount},
		{"GetSkillId",		LuaGetSkillIdInSkillList },
		{"GetSkillName",		LuaGetSkillName },
		{"SetSkillLevel",	LuaSetSkillLevel},
		{"SetSkillTemp",	LuaSetSkillTemp},
		{"SetChatFlag",		LuaSetPlayerChatForbiddenFlag},
		//------------------------------------------------

		{"AddNote", LuaAddNote},
		{"AddMissionNote", LuaAddMissionNote},
		//-----------------Mission Script-----------------
		{"GetMissionV", LuaGetMissionValue},//GetMissionV(Vid)
		{"GetMissionS", LuaGetMissionString},
		{"SetMission", LuaSetMission},//SetMissionV(Vid, Value)
		{"GetGlbMissionV", LuaGetGlobalMissionValue	},
		{"GetGlbMissionS", LuaGetGlobalMissionString	},
		{"SetGlbMission", LuaSetGlobalMission	},
		{"GetMissionName", LuaGetMissionName},//OpenMission(missionid)
		{"OpenMission", LuaInitMission},//OpenMission(missionid)
		{"RunMission", LuaRunMission},
		{"CloseMission", LuaCloseMission},//CloseMission(missionid)
		{"IsMission", LuaIsMission},
		{"GetMSLadder",LuaGetMSLadder },
		{"GetMSTop10",LuaGetMSTop10 },
		{"StartMissionTimer", LuaStartMissionTimer},////StartMissionTimer(missionid, timerid, time)
		{"StopMissionTimer", LuaStopMissionTimer},
		{"GetMSRestTime", LuaGetMissionRestTime}, //GetMSRestTime(missionid, timerid)
		{"GetMSIdxGroup",LuaGetPlayerMissionGroup},//GetPlayerGroup(missionid, playerid);

		{"AddMSPlayer", LuaAddMissionPlayer},
		{"AddMSNpc", LuaAddMissionNpc},
		{"DelMSPlayer", LuaRemoveMissionPlayer},
		{"DelMSNpc", LuaRemoveMissionNpc},
		{"SetMSGroup", LuaSetMissionGroup},
		{"PIdx2MSDIdx", LuaGetMissionPlayer_DataIndex},//(missionid, pidx)
		{"MSDIdx2PIdx", LuaGetMissionPlayer_PlayerIndex},//(missionid, dataidx)
		{"NpcIdx2PIdx", LuaNpcIndexToPlayerIndex},
		{"GetMSPlayerCount", LuaMissionPlayerCount},//GetMSPlayerCount(missionid, group = 0)
		{"GetMSNpcCount", LuaMissionNpcCount},//GetMSPlayerCount(missionid, group = 0)

		{"RevivalAllNpc",	LuaRevivalAllNpc},

		{"SetPMParam", LuaSetMissionPlayerParam},
		{"GetPMParam", LuaGetMissionPlayerParam},
		{"Msg2MSGroup", LuaMissionMsg2Group},
		{"Msg2MSAll", LuaMissionMsg2All},
		{"SetDeathScript", LuaSetPlayerDeathScript},
		{"SetDmgScript", LuaSetPlayerDamageScript},

		{"NpcChat", LuaNpcChat}	,
		{"HideNpc", LuaHideNpc}	,//HideNpc(npcindex/npcname, hidetime)
		{"SetLogoutRV", LuaSetPlayerRevivalOptionWhenLogout},
		{"SetCreateTeam",LuaSetCreateTeamOption},
		{"ForbidTeam",LuaSetFreezeTeamOption},
		{"GetPK", LuaGetPlayerPKValue},  //pkValue = GetPK() 
		{"SetPK", LuaSetPlayerPKValue}, //SetPK(pkValue)
		//------------------------------------------------
		//ÅÅÃûÏà¹Øº¯Êý
		{"ShowLadder", LuaShowLadder}, //ShowLadder(LadderCount, LadderId1,LadderId2,...);
		{"GetLadder",  LuaGetLadder},
		//------------------------------------------------

		{"OpenTong",			LuaOpenTong},	//OpenTong()Í¨ÖªÍæ¼Ò´ò¿ª°ï»á½çÃæ
		{"CreateTong",			LuaCreateTong },
		{"LeaveTong",			LuaLeaveTong },
		{"JoinTong",			LuaJoinTong},	//OpenTong()Í¨ÖªÍæ¼Ò´ò¿ª°ï»á½çÃæ
		{"CommendMaster",		LuaCommendMaster},
		{"GetTongFlag",			LuaGetTongFlag},
		{"GetTongName",			LuaGetTongName},
		{"GetTongCamp",			LuaGetTongCamp},
		{"GetTongMemNum",		LuaGetTongMemNum},
		{"GetTongFigure",		LuaGetTongFigure},
		{"GetTongMoney",		LuaGetTongMoney},
		{"GetTongLevel",		LuaGetTongLevel},
		{"GetTongEff",			LuaGetTongEff},
		{"GetTongParam",		LuaGetTongParam},
		{"GetTongJoinTm",		LuaGetTongJoinTm},
		{"SetTongLevel",		LuaSetTongLevel},
		{"SetTongMoney",		LuaSetTongMoney},
		{"SetTongEff",			LuaSetTongEff},
		{"SetTongParam",		LuaSetTongParam},
		{"SetTongMemELW",		LuaSetTongMemEffLW},
		{"SetTongMemETW",		LuaSetTongMemEffTW},
		{"SetTongMemEUB",		LuaSetTongMemEffUB},
		{"GetTongMemEff",		LuaGetTongMemEff},
		{"SetPunish",			LuaSetDeathPunish},// SetPunish(0/1)
		{"SetReviveNow",		LuaSetReviveNow},
		//-------------------------------------------------
		//½á°Ý
		//{"SwearBrother", LuaSwearBrother}, // ret = SwearBrother(TeamId);
		//{"MakeEnemy",	LuaMakeEnemy}, //½á³ð MakeEnemy(enemyname)
		{"MakeMate", LuaMakeMate}, // ret = MakeMate(matename);
		//{"UnMarry", LuaDeleteMate}, // ret = DeleteMate(matename);
		{"RollbackSkill", LuaRollBackSkills},
		//-------------------------------------------------

		{"AddProp",		LuaAddPropPoint},//¼ÓÍæ¼ÒÊôÐÔµã
		{"AddPropPoint",LuaAddPropPoint },//¼ÓÍæ¼ÒÊôÐÔµã 
		{"DelPropPoint",LuaRemovePropPoint },//¼ÓÍæ¼ÒÊôÐÔµã LuaRemovePropPoint
		{"GetProp",		LuaGetRestPropPoint},

		{"SetExtPoint",	LuaSetExtPoint},
		{"AddExtPoint",	LuaAddExtPoint},
		{"GetExtPoint",	LuaGetExtPoint},
		{"PayExtPoint",	LuaPayExtPoint },
		{"ExchangeExtDay",	LuaExchangeExtDay },

		{"AddSkillState",	LuaAddSkillState},
		{"AddNpcSkillState",LuaAddNpcSkillState},
		{"IgnoreState",		LuaIgnoreState},
		{"CastSkill",		LuaCastSkill},
		{"NpcCastSkill",	LuaNpcCastSkill},
		{"SetMask",			LuaSetMask},

		{"AddTrap",			LuaAddTrap},

		{"AddObj",			LuaAddObj},
		{"SetObjValue",		LuaSetObjValue},
		{"GetObjValue",		LuaGetObjValue},
		{"SetObjPickExecute",	LuaSetObjPickExecute},
		{"SetObjScript",	LuaSetObjScript},
		{"DelObj",			LuaDelObj},
		{"RemoveItem",		LuaRemoveItemIdx},
		{"LockMoveItem",	LuaLockMoveItem},
		{"SyncItem",		LuaSyncItem},

		{"SetTempItem",		LuaSetTempItem},
		{"SetLevelItem",	LuaSetLevelItem},
		{"SetSeriesItem",	LuaSetSeriesItem},
		{"SetParamItem",	LuaSetParamItem},
		{"SetFortuneItem",	LuaSetFortuneItem},
		{"SetExpireTimeItem",LuaSetTimeItem},
		{"SetTimeItem",		LuaSetTimeItem },
		{"SetExpiredTime",	LuaSetExpiredDateItem },
		{"GetTimeMin",		LuaGetTimeMin },
		{"AddExpireTimeItem",LuaAddTimeItem},
		{"AddTimeItem",		LuaAddTimeItem},
		{"LockItem",		LuaLockItem},
		{"SetStackItem",	LuaSetStackItem},
		{"SetMantleItem",	LuaSetMantleItem},
		{"SetFlashItem",	LuaSetFlashItem},

		{"SetSavePw",			LuaSetSavePw},
		{"GetSavePw",			LuaGetSavePw},
		{"SetLockState",		LuaSetLockState},
		{"GetLockState",		LuaGetLockState},
		{"CheckRoom",			LuaCheckRoom},

		{"OpenRankData",		LuaOpenRankData},
		{"Input",				LuaInput},
		{"GetInput",			LuaGetInput},
		{"OpenEnchase",			LuaOpenEnchase},
		{"Enchase",				LuaEnchase},
		{"GiveItemUI",			LuaOpenGive},

		{ "OpenFinishDatauBox",	LuaFinishQuest }, //TamLTM hien thi khung da tau finish cach su dung OpenFinishDatauBox(1); 	local indexRan = RANDOM(8)	OpenFinishDatauBox(indexRan);

		{"SetPKMode",			LuaSetPKState},
		{"ForbidChangePK",		LuaForbidChangePK},
		{"GetPKState",			LuaGetPKState},
		{"GetNormalPKState",	LuaGetNormalPKState},

		{"IsHideNpc",			LuaIsHideNpc},
		{"PaceBar",				LuaPaceBar},
		{"GetTrade",			LuaGetTrade},
		{"ForbidUseTownP",		LuaForbidUseTownP },
		{"IsForbidUseTownP",	LuaIsForbidUseTownP },
		{"ForbidTrade",			LuaForbidTrade},
		{"ForbidEnmity",		LuaForbidEnmity},
		{"ForbidName",			LuaForbidName},
		{"ForbidCamp",			LuaForbidCamp },
		{"ForbidParty",			LuaForbidParty},

		{"GetDistance",			LuaGetDistance},
		{"GetDir",				LuaGetDir},
		{"DirSin",				LuaDirSin},
		{"DirCos",				LuaDirCos},
		{"FileExists",			LuaFileExists},
		{"FileName2Id",			LuaFileName2Id},
		{"TabFile_Load",		LuaTabFile_Load},
		{"IniFile_IsSectionExist",		LuaIniFile_IsSectionExist},
		{"IniFile_Load",		LuaIniFile_Load},
		{"IniFile_GetStr",		LuaIniFile_GetStr},
		{"IniFile_GetInt",		LuaIniFile_GetInt},
		{"IniFile_GetInt2",		LuaIniFile_GetInt2},
		{"IniFile_Save",		LuaIniFile_Save},
		{"IniFile_SaveStr",		LuaIniFile_SaveStr},
		{"IniFile_SaveInt",		LuaIniFile_SaveInt},
		{"IniFile_SaveInt2",		LuaIniFile_SaveInt2},
		{"TabFile_GetRowCount",	LuaTabFile_GetRowCount},
		{"TabFile_GetCell",		LuaTabFile_GetCell},
		{"RemoveRoom",			LuaRemoveRoom},
		{"CalcFreeItemCellCount",LuaCalcFreeItemCellCount},
		{"IsNumber",			LuaIsNumber},
		{"IsTable",				LuaIsTable},
		//{"OpenURL",				LuaOpenURL},
		//{"OpenExplore",			LuaOpenExplore},
		{"ModifyAttrib",		LuaModifyAttrib},
		{"RANDOM",				LuaRANDOM},
		{"RANDOMC",				LuaRANDOMC},
		{"IsMyItem",			LuaIsMyItem},
		{"GetDataInt",			LuaGetDataInt},
		{"GetDataStr",			LuaGetDataStr},
		{"SetData",				LuaSetData},
		{"SendReport",			LuaSendReport},
		{"GetTime",				LuaGetTime},
		{"GetTimeDate",			LuaGetTimeDate},
		{"GetLocalDate",		LuaGetLocalDate},

		{"AddDataGr",			LuaAddDataGr},
		{"SetDataGr",			LuaSetDataGr},
		{"GetDataGr",			LuaGetDataGr},
		{"FindDataId",			LuaFindDataId},
		{"SaveDataFile",		LuaSaveDataFile},

		{"ChatRoom_Create",		LuaChatRoom_Create},
		{"ChatRoom_AddTime",	LuaChatRoom_AddTime},
	#else 
		{"PlaySound", LuaPlaySound}, //PlaySound(Sound);
		{"PlaySprMovie",LuaPlaySprMovie},//PlaySprMovie(npcindex, Movie, times)
	#endif
		{"GetCurServerSec",		LuaGetCurServerSec},
		{"SetNumber",			LuaSetNumber},
		{"GetNumber",			LuaGetNumber},

		{"GetNameItem",		LuaGetNameItem},
		{"GetParamItem",	LuaGetParamItem},
		{"GetFortuneItem",	LuaGetFortuneItem},
		{"GetLockItem",		LuaGetLockItem},
		{"GetStackItem",	LuaGetStackItem},
		{"GetMaxStackItem",	LuaGetMaxStackItem},
		{"GetMantleItem",	LuaGetMantleItem},
		{"FindItem",		LuaFindItem},
		{"FindGoldItem",	LuaFindGoldItem},
		{"FindItemEx",		LuaFindItemEx},

		{"PlayMusic",		LuaPlayMusic}, //PlayMusic(Music,Loop)
		{"FadeInMusic",		LuaFadeInMusic},
		{"FadeOutMusic",	LuaFadeOutMusic},
		{"ReloadScript",	LuaReloadScript },
};


TLua_Funcs WorldScriptFuns[] =// ·ÇÖ¸¶ÔÍæ¼ÒµÄ½Å±¾Ö¸Áî¼¯
{
	//Í¨ÓÃÖ¸Áî

	{"AddLocalNews",LuaAddLocalNews},
	{"AddLoaclTimeNews",LuaAddLocalTimeNews},
	{"AddLocalCountNews",LuaAddLocalCountNews	},
	//·þÎñÆ÷¶Ë½Å±¾Ö¸Áî
#ifdef _SERVER
	{"Msg2SubWorld",	LuaMsgToSubWorld},	//Msg2SubWorld(ÏûÏ¢)Í¨ÖªÊÀ½ç
	{"Msg2IP",			LuaMsgToIP}, //Msg2IP(IP, ID, StrInfo)	
	{"SubWorldID2Idx",	LuaSubWorldIDToIndex}, //SubWorldID2Idx
	{"GetServerName",	LuaGetServerName},
	{"KickOutPlayer",	LuaKickOutPlayer},
	{"KickOutAccount",	LuaKickOutAccount},
	{"SetRoleChatFlag",	LuaSetRoleChatFlag},
	{"ShutDownServer",	LuaShutDownServer},
#endif
};

INT g_GetGameScriptFunNum()
{
	return sizeof(GameScriptFuns) / sizeof(GameScriptFuns[0]);
}

INT g_GetWorldScriptFunNum()
{
	return sizeof(WorldScriptFuns) / sizeof(WorldScriptFuns[0]);
}