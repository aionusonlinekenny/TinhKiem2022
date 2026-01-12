#include "KCore.h"
#include "KSortScript.h"
#include "LuaFuns.h"
#include "KFilePath.h"
#include "KDebug.h"
#include <map>
#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#else 
#include <io.h>
#include <direct.h>
#endif
//#include "Shlwapi.h"

KLuaScript g_ScriptSet[MAX_SCRIPT_IN_SET];
KScriptBinTree g_ScriptBinTree;
UINT   nCurrentScriptNum;
char g_szCurScriptDir[MAX_PATH];

void	LoadScriptInDirectory(LPSTR lpszRootDir, LPSTR lpszSubDir);
BOOL	LoadScriptToSortListA(char* szRelativeFile);
#include <algorithm>
#include "../../MultiServer/Common/Macro.h"

class KLuaScriptRateMap : public std::map<DWORD, KLuaScript*>
{
private:
	static void _FreeNode(value_type& cValue)
	{
		SAFE_DELETE(cValue.second);
		//if (cValue.second)
		//{ // KEY值
		//	delete (cValue.second);
		//	(cValue.second) = NULL;
		//}
	}

public:
	~KLuaScriptRateMap()
	{ // 删除全部节点
#ifdef _SERVER
		std::for_each(begin(), end(), _FreeNode);
#endif
	}
};


INT	operator<(KSortScriptNode ScriptLeft, KSortScriptNode ScriptRight)
{
	return ScriptLeft.GetScriptID() < ScriptRight.GetScriptID();
};

INT operator==(KSortScriptNode ScriptLeft, KSortScriptNode ScriptRight)
{
	return ScriptLeft.GetScriptID() == ScriptRight.GetScriptID();
};

//½«szFilePathÄ¿Â¼ÏÂµÄËùÓÐ½Å±¾ÎÄ¼þ¼ÓÔØ½øScriptBinTree¶þ²æÊ÷ÖÐ
static ULONG LoadAllScript(char* szFilePath, BOOL nIsSer)
{
	g_SetFilePath("\\");
	char szRootPath[MAX_PATH];
	char szOldRootPath[MAX_PATH];

	//	GetCurrentDirectory(MAX_PATH, szOldRootPath);
	char* x = getcwd(szOldRootPath, MAXPATH);
	g_GetFullPath(szRootPath, szFilePath);
	LoadScriptInDirectory(szRootPath, "");
	INT r = chdir(szOldRootPath);
	//	SetCurrentDirectory(szOldRootPath);

	return nCurrentScriptNum;
}

ULONG g_IniScriptEngine(char* nScriptDir, BOOL nIsSer, BOOL nIsClear)
{
	// g_szCurScriptDir[0] = 0;
	if (nIsClear)
	{
		nCurrentScriptNum = 0;
		// g_ScriptBinTree.ClearList();
		ClearAllScript();
	}
	return LoadAllScript(nScriptDir, nIsSer);
}

static KLuaScriptRateMap g_ScriptRateMap; // In order to ensure the security of memory management, this variable is no longer exposed to the outside world.

const KScript* g_GetScript(DWORD dwScriptId)
{
	// The script has been loaded. Find the node in the stack.
// KSortScriptNode ScriptNode;
// ScriptNode.SetScriptID(dwScriptId);
#ifdef _SERVER
  // INT Nmu;
  //     Nmu=g_ScriptBinTree.ListSize();//BiTreeDepth(g_ScriptBinTree.GetRoot());	 //节点数

	// printf("-----g_ScriptBinTree 节点数:%d ----- \n", Nmu);
#endif

	/*if (g_ScriptBinTree.Find(ScriptNode))
	{
		return ScriptNode.GetScript();
	} */

	if (g_ScriptRateMap.count(dwScriptId) > 0)
	{
		return g_ScriptRateMap[dwScriptId];
	}

	return NULL;
}
const KScript* g_GetScript(const char* szRelativeScriptFile)
{
	DWORD dwScriptId = g_FileName2Id((LPSTR)szRelativeScriptFile);
	return g_GetScript(dwScriptId);
}

extern INT LuaIncludeFile(Lua_State* L);
extern INT LuaIncludeLib(Lua_State* L);

//¼ÓÔØ½Å±¾£¬¸ÃÎÄ¼þÃû²ÎÊýÎªÏà¶ÔÄ¿Â¼
static BOOL LoadScriptToSortListA(char* szRelativeFile)
{
	if (!szRelativeFile || !szRelativeFile[0]) return FALSE;
	KSortScriptNode ScriptNode;
	ScriptNode.SetScriptID(g_FileName2Id(szRelativeFile));
	INT t = strlen(szRelativeFile);
	if (t >= 90)
		t++;

#ifdef _DEBUG
	//strcpy(ScriptNode.m_szScriptName, szRelativeFile);
#endif
	if (nCurrentScriptNum < MAX_SCRIPT_IN_SET)
	{
		g_ScriptSet[nCurrentScriptNum].Init();
		g_ScriptSet[nCurrentScriptNum].RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum());
		g_StrCpyLen(g_ScriptSet[nCurrentScriptNum].m_szScriptName, szRelativeFile, 100);
		if (g_ScriptSet[nCurrentScriptNum].Load(szRelativeFile))
		{

		}
		else
		{
			g_DebugLog("[KSortScript]!Load %s", szRelativeFile);
			return FALSE;
		}
	}
	else
	{
		g_DebugLog("[½Å±¾]ÑÏÖØ´íÎó!½Å±¾ÊýÁ¿³¬ÏÞÖÆ%d£¡ÇëÁ¢¼´½â¾ö£¡£¡", MAX_SCRIPT_IN_SET);
		return FALSE;
	}

	g_ScriptRateMap[g_FileName2Id(szRelativeFile)] = &g_ScriptSet[nCurrentScriptNum];
	nCurrentScriptNum++;

	/*ScriptNode.SetScriptIndex(nCurrentScriptNum++);
	g_ScriptBinTree.Insert(ScriptNode);*/
	return TRUE;
}

//¼ÓÔØ½Å±¾£¬¸ÃÎÄ¼þÃû²ÎÊýÎªÊµ¼ÊÄ¿Â¼
static BOOL LoadScriptToSortList(char* szFileName)
{
	if (!szFileName || !szFileName[0]) return FALSE;
	if (nCurrentScriptNum >= MAX_SCRIPT_IN_SET)
	{
		return FALSE;
	}

	INT nFileNameLen = strlen(szFileName);

	char szRootPath[MAX_PATH];
	g_GetRootPath(szRootPath);
	//	char szRelativePath[MAX_PATH];
	char* szRelativePath;
	char szCurrentDirectory[MAX_PATH];


	//	GetCurrentDirectory(MAX_PATH, szCurrentDirectory);
	char* x = getcwd(szCurrentDirectory, MAX_PATH);
	szRelativePath = szCurrentDirectory + strlen(szRootPath);
	//	PathRelativePathTo(szRelativePath,szRootPath, FILE_ATTRIBUTE_DIRECTORY,szCurrentDirectory , FILE_ATTRIBUTE_NORMAL );
	char szRelativeFile[MAX_PATH];
	if (szRelativePath[0] == '.' && szRelativePath[1] == '\\')
		sprintf(szRelativeFile, "%s\\%s", szRelativePath + 1, szFileName);
	else
		sprintf(szRelativeFile, "%s\\%s", szRelativePath, szFileName);
	g_StrLower(szRelativeFile);
	//	g_DebugLog("[Script]Loading Script %s %d", szRelativeFile, g_FileName2Id(szRelativeFile));
	//
	return LoadScriptToSortListA(szRelativeFile);
	//	return FALSE;
}


void	LoadScriptInDirectory(LPSTR lpszRootDir, LPSTR lpszSubDir)
{
	INT				nFlag;
	char			szRealDir[MAX_PATH];
#ifdef WIN32
	sprintf(szRealDir, "%s\\%s", lpszRootDir, lpszSubDir);
#else
	sprintf(szRealDir, "%s/%s", lpszRootDir, lpszSubDir);
	char* ptr = szRealDir;
	while (*ptr) { if (*ptr == '\\') *ptr = '/';  ptr++; }
#endif

#ifdef WIN32
	if (chdir(szRealDir)) return;
	_finddata_t FindData;
	LONG dir = _findfirst("*.*", &FindData);
	while (dir != -1) {
		if (strcmp(FindData.name, ".") == 0 || strcmp(FindData.name, "..") == 0) {
			if (_findnext(dir, &FindData)) break;
			continue;
		}
		if (FindData.attrib == _A_SUBDIR)
		{
			LoadScriptInDirectory(szRealDir, FindData.name);
		}
		else
		{
			nFlag = 0;
			for (INT i = 0; i < (INT)strlen(FindData.name); i++)
			{
				if (FindData.name[i] == '.')
					break;
				if (FindData.name[i] == '\\')
				{
					nFlag = 1;
					break;
				}
			}
			if (nFlag == 1)
			{
				LoadScriptInDirectory(szRealDir, FindData.name);
			}
			else
			{
				char szExt[128];
				if (strlen(FindData.name) >= 4)
				{
					strcpy(szExt, FindData.name + strlen(FindData.name) - 4);
					_strupr(szExt);
					if ((!strcmp(szExt, ".LUA")) || (!strcmp(szExt, ".TXT")))
						if (!LoadScriptToSortList(FindData.name))
						{
							//							g_DebugLog("Not LoadScriptToSortList: %s", FindData.name);
						}
				}
			}
		}
		if (_findnext(dir, &FindData)) break;
	}
	_findclose(dir);
	INT x = chdir(lpszRootDir);
#else
	DIR* dp;
	INT i;
	struct dirent* ep;
	if (chdir(szRealDir)) return;
	dp = opendir(".");
	if (dp) {
		while (ep = readdir(dp)) {
			if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

			if (ep->d_type == 4) {
				LoadScriptInDirectory(szRealDir, ep->d_name);
			}
			else {
				nFlag = 0;
				for (i = 0; i < (INT)strlen(ep->d_name); i++)
				{
					if (ep->d_name[i] == '.')
						break;
					if (ep->d_name[i] == '\\')
					{
						nFlag = 1;
						break;
					}
				}
				if (nFlag == 1)
				{
					LoadScriptInDirectory(szRealDir, ep->d_name);
				}
				else
				{
					char szExt[128];
					if (strlen(ep->d_name) >= 4)
					{
						strcpy(szExt, ep->d_name + strlen(ep->d_name) - 4);
						g_StrUpper(szExt);
						///					_strupr(szExt);
						if ((!strcmp(szExt, ".LUA")) || (!strcmp(szExt, ".TXT")))
							if (!LoadScriptToSortList(ep->d_name))
							{
								//							g_DebugLog("Not LoadScriptToSortList: %s", ep->d_name);
							}
					}
				}
			}
		}
		closedir(dp);
	}
	chdir(lpszRootDir);
#endif
}

void UnLoadScript(DWORD dwScriptID)
{
	/*KSortScriptNode ScriptNode;
	ScriptNode.SetScriptID(dwScriptID);
	g_ScriptBinTree.Delete(ScriptNode);*/
	if (g_ScriptRateMap.count(dwScriptID) > 0)
	{ // 如果存在就 删除
		// return g_ScriptRateMap[dwScriptID];
		printf("-- 脚本存在,清除成功 --\n");
		g_ScriptRateMap.erase(dwScriptID);
	}
}

BOOL ReLoadScript(const char* szRelativePathScript)
{
	if (!szRelativePathScript || !szRelativePathScript[0])
		return FALSE;
	char script[MAX_PATH];
	strcpy(script, szRelativePathScript);
	//	_strlwr(script);
	g_StrLower(script);
	UnLoadScript(g_FileName2Id(script));
	return LoadScriptToSortListA(script);
}

ULONG  ReLoadAllScript()
{
	ClearAllScript();
	return g_IniScriptEngine("\\script");
}

void ClearAllScript()
{
	for (KLuaScriptRateMap::iterator it = g_ScriptRateMap.begin(); it != g_ScriptRateMap.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	g_ScriptRateMap.clear();
}
