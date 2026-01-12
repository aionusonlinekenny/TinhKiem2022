#ifdef USEOLD
#include "KWin32.h"
#include "KEngine.h"
#include "KDebug.h"
#include "KStepLuaScript.h"
#include "LuaLib.h"
#include "KScriptList.h"
#include "string.h"
#include "LuaFuns.h"
#include "KCore.h"
#include "KNpc.h"
#include "KSubWorld.h"
//#include "KNetClient.h"
#include "../../Headers/IClient.h"
#include "KScriptValueSet.h"
#include "KNpcSet.h"
#include "KPlayer.h"

KScriptList g_StoryScriptList;
KStepLuaScript* LuaGetScript(Lua_State* L);

INT LuaAddNpcFromTemplate(Lua_State* L);
INT LuaMessageBox(Lua_State* L);
INT LuaSendScriptMessage(Lua_State* L);
INT LuaWaitForEvent(Lua_State* L);
INT LuaSendMessage(Lua_State* L);
INT LuaSendScriptMessage(Lua_State* L);
INT LuaGetScriptState(Lua_State* L);
INT LuaWait(Lua_State* L);
INT LuaEndWait(Lua_State* L);
INT LuaNewScript(Lua_State* L);
INT LuaGotoLabel(Lua_State* L);
INT LuaGetValue(Lua_State* L);
INT LuaSetValue(Lua_State* L);
INT LuaReturn(Lua_State* L);
INT LuaResume(Lua_State* L);
INT LuaLabel(Lua_State* L);
INT LuaGetTempTaskValue(Lua_State* L);
INT LuaSetTempTaskValue(Lua_State* L);



CORE_API TLua_Funcs SysFuns[] =
{
{"Wait", LuaWait},
{"EndWait", LuaEndWait},
{"GetScriptState", LuaGetScriptState},
{"NewScript", LuaNewScript},
{"Goto", LuaGotoLabel},
{"Label",LuaLabel},
{"MessageBox", LuaMessageBox},
{"SendScriptMessage", LuaSendScriptMessage},
{"WaitForEvent", LuaWaitForEvent},
{"SendMessage",LuaSendMessage},
{"SetValue",LuaSetValue},
{"GetValue", LuaGetValue},
{"Exit",LuaReturn},
{"Resume", LuaResume},
};

CORE_API INT g_GetLuaFunsCount()
{
	return sizeof(SysFuns) / sizeof(SysFuns[0]);
}

INT LuaLabel(Lua_State* L)
{
	return 0;
}

INT LuaGotoLabel(Lua_State* L)
{
	KStepLuaScript* pScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	const char* szLabelName;
	szLabelName = lua_tostring(L, 1);
	pScript->GotoLabel((char*)szLabelName);
	return 0;

}

KStepLuaScript* LuaGetScript(Lua_State* L)
{
	KStepLuaScript* pOrScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	UINT Addr = (UINT)lua_tonumber(pOrScript->m_LuaState, 1);
	Lua_State* pResultState = (Lua_State*)Addr;
	return  (KStepLuaScript*)g_StoryScriptList.GetScript(pResultState);
}

INT LuaNewScript(Lua_State* L)
{

	KStepLuaScript* pOrScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	KStepLuaScript* pScript = new KStepLuaScript(0);
	KScriptNode* pNode = new KScriptNode;

	const char* szScriptFIle = NULL;
	szScriptFIle = lua_tostring(pOrScript->m_LuaState, 1);

	if (strlen(szScriptFIle) <= 0)
		return -1;

	if (!pScript->Init())
	{
		Lua_PushNil(L);
		return 1;
	}

	g_SetFilePath("\\script");
	char FilePath[200];
	g_GetFullPath(FilePath, (char*)szScriptFIle);
	if (!pScript->Load((char*)FilePath))
	{
		Lua_PushNil(L);
		return 1;
	}
	pScript->RegisterFunctions(LuaFuns, g_GetLuaFunsCount());
	pScript->SeekToExeBegin();
	pScript->RunMain();

	pNode->pScript = pScript;
	g_StoryScriptList.AddTail(pNode);
	pScript->RunMain();
	Lua_PushNumber(L, (UINT)pScript->m_LuaState);
	return 1;

}

INT LuaResume(Lua_State* L)
{
	KStepLuaScript* pScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	if (pScript)
		pScript->RunResume();
	return 0;
}

INT LuaWait(Lua_State* L)
{

	KStepLuaScript* pScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	pScript->PosUp();
	pScript->RunFunc();
	return 0;
}

INT LuaEndWait(Lua_State* L)
{
	KStepLuaScript* pScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	pScript->RunMain();
	return 0;
}

INT LuaGetScriptState(Lua_State* L)
{
	KStepLuaScript* pScript;

	pScript = LuaGetScript(L);

	//不存在
	if (pScript == NULL)
	{
		Lua_PushNumber(L, -1);
		return -1;
	}
	Lua_PushNumber(L, pScript->GetStatus());
	return 1;
}




INT LuaSendScriptMessage(Lua_State* L)
{

	KStepLuaScript* pScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	UINT stateid;

	stateid = (UINT)lua_tonumber(L, 1);
	const char* szMessage = lua_tostring(L, 2);
	const char* szData = lua_tostring(L, 3);
	KStepLuaScript* pSendedScript = LuaGetScript((Lua_State*)stateid);

	if (pSendedScript == NULL)
	{
		g_DebugLog("无法找到指定的脚本对象");
		return 0;
	}

	pScript->SendMessage(pSendedScript, (char*)szMessage, (char*)szData);
	return 0;
}

INT LuaSendMessage(Lua_State* L)
{
	g_StoryScriptList.SendMessage((char*)lua_tostring(L, 1), (char*)lua_tostring(L, 2));
	return 0;
}

/*!*****************************************************************************
// Function		: LuaWaitForEvent
// Purpose		:
// Return		: INT
// Argumant		: Lua_State * L
// Comments		:
// Author		: RomanDou
*****************************************************************************/
INT LuaWaitForEvent(Lua_State* L)
{
	KStepLuaScript* pScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	strcpy(pScript->GetWaitingMsg(), lua_tostring(L, 1));
	pScript->RunWaitMsg();
	return 0;
}

INT LuaGetValue(Lua_State* L)
{
	INT nArg = lua_gettop(L);
	if (nArg == 0) return 0;

	char* pValueName = NULL;
	pValueName = (char*)lua_tostring(L, 1);

	if (nArg == 1)
	{
		INT nValue = 0;
		char  strValue[100];
		if (!g_ScriptValueSet.GetValue(pValueName, &nValue))
		{
			if (!g_ScriptValueSet.GetValue(pValueName, strValue))
			{
				lua_pushnumber(L, 0);
			}
			else
			{
				lua_pushstring(L, strValue);
			}
		}
		else
		{
			lua_pushnumber(L, nValue);
		}
	}

	if (nArg == 2)
	{
		INT nType = (INT)lua_tonumber(L, 2);
		if (nType = 0)
		{
			char strValue[100];
			if (!g_ScriptValueSet.GetValue(pValueName, strValue))
			{
				strcpy(strValue, "");
			}
			lua_pushstring(L, strValue);
		}
		else
		{
			INT nValue = 0;
			if (!g_ScriptValueSet.GetValue(pValueName, &nValue))
				nValue = 0;
			lua_pushnumber(L, nValue);
		}
	}

	//	INT itop = lua_gettop(L);
	return 1;
}

INT LuaSetValue(Lua_State* L)
{
	char* szValueName = NULL;
	szValueName = (char*)lua_tostring(L, 1);
	if (!strcmp(szValueName, "")) return 0;

	if (lua_isnumber(L, 2))
	{
		INT nValue = (INT)lua_tonumber(L, 2);
		g_ScriptValueSet.SetValue(szValueName, nValue);

	}
	else
	{
		if (lua_isstring(L, 2))
		{
			char* pStr = (char*)lua_tostring(L, 2);
			g_ScriptValueSet.SetValue(szValueName, pStr);
		}
	}

	return 0;
}

INT LuaReturn(Lua_State* L)
{
	KStepLuaScript* pScript = (KStepLuaScript*)g_StoryScriptList.GetScript(L);
	if (pScript)
		pScript->RunIdle();
	return 0;
}

CORE_API FILE* stream1;
CORE_API FILE* stream;
#endif