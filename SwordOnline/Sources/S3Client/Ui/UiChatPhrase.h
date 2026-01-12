/*****************************************************************************************
//	界面--聊天快捷输入短语
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-3-23
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#define	MAX_PHRASE_COUNT	10

class KUiChatPhrase
{
public:
	KUiChatPhrase();
	~KUiChatPhrase();
	INT		GetPhrase(INT nIndex, char* pszPhrase);
	INT		SetPhrase(INT nIndex, char* pszPhrase, INT nLen);
	INT		GetPhraseCount();
	INT		SavePrivateSetting(KIniFile* pFile, LPCSTR lpSection, INT nStart);
	void	ClearAllPhrase();

	enum EMOTE_TYPE
	{
		EMOTE_T_PHRASE = 0,
		EMOTE_T_MENU,
	};
	//载入所有聊天动作文件
	INT     LoadEntireEmote();
	//设置聊天动作，有给出index的就改index的，没有就根据Cmd寻找要替换的
	INT     SetEmote(char *szCmd, char *szName, char *szStringTarget, char *szStringSelf, INT nIndex = -1);
	//
	INT     GetEmoteCount(EMOTE_TYPE eType);
	//根据命令获取一个动作描述(pBuff)，返回描述长度，这里是包括了MENU和PHRASE的
	INT     GetEmote(const char *szCmd, char *pBuff, INT nBuffLen, INT nType = 0);
	//根据索引获取一个动作描述(pBuff)，返回描述长度，，这里是包括了MENU和PHRASE的
	INT     GetEmote(INT nIndex, char *pBuff, INT nBuffLen, INT nType = 0);
	//获取一个用于菜单上的动作描述(pBuff)，返回描述长度，仅仅MENU
	INT     GetMenuEmote(const char *szCmd, char *pBuff, INT nBuffLen, INT nType = 0);
	INT     GetMenuEmote(INT nIndex, char *pBuff, INT nBuffLen, INT nType = 0);
	//取出指定索引的动作名字
	INT     GetMenuEmoteName(INT nIndex, char *pBuff, INT nBuffLen);
	//制作字符串
	INT     ConvertEmoteString(char *szString, INT nStringLen, const char *szMyName, const char *szTarName);

	//把读取到的Emote信息构造进去Shortcut系统中的函数别名表
	INT     ConstructFunctionAlias();

private:
	struct  EMOTE_PHRASE
	{
		char    szName[16];
		INT     nNameLen;
		char    szString[128];
		INT     nStringLen;
		char    szStringMe[128];
		INT     nStringMeLen;
		char    szCmd[64];
	} *m_pEmotePhrase, *m_pMenuEmote;
	
	INT     LoadEmotePhrase(KIniFile *pIni);
	INT     LoadMenuEmote(KIniFile *pIni);
	INT     HandleLoad(KIniFile *pIni, EMOTE_PHRASE *pData, INT nCount, const char *szSection);
	INT     FindEmote(EMOTE_TYPE eType, const char *szCmd);

private:
	struct	CHAT_PHRASE
	{
		char	szString[64];
		INT		nStringLen;
	}		m_PhraseList[MAX_PHRASE_COUNT];

	INT m_nEmotePhraseCount;
	INT m_nMenuEmoteCount;

	UINT m_uMyNameColor;
	UINT m_uTargetColor;
};

extern KUiChatPhrase	g_UiChatPhrase;