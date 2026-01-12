//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerChat.cpp
// Date:	2002.10.05
// Code:	±ß³ÇÀË×Ó
// Desc:	PlayerChat Class
//---------------------------------------------------------------------------

#ifdef _STANDALONE
#include <string>
#endif

#include	"KCore.h"
#ifdef _SERVER
//#include	"KNetServer.h"
//#include "../MultiServer/Heaven/Interface/iServer.h"
#include	"KGMCommand.h"
#else
//#include	"KNetClient.h"
#include "../../Headers/IClient.h"
#include	"CoreShell.h"
#endif
#include	"KNpc.h"
#include	"KNpcSet.h"
#include	"KSubWorld.h"
#include	"KPlayer.h"
#include	"KPlayerSet.h"
#include	"KPlayerTeam.h"
#include	"KPlayerChat.h"
#include	"MsgGenreDef.h"
#include	"CoreUseNameDef.h"
#include	"Text.h"
#include	"malloc.h"
#ifdef _SERVER
#include "KNewProtocolProcess.h"
#endif
#include "KTongProtocol.h"

#define		CHAT_TEAM_DEFAULT_NAME		"H¶o h÷u"

UINT	g_dwChannelOr[CHAT_CUR_CHANNEL_NUM] =
{
	0x00000000,		// CHAT_CUR_CHANNEL_NONE
	0x00000001,		// CHAT_CUR_CHANNEL_ALL
	0x00000002,		// CHAT_CUR_CHANNEL_SCREEN
	0x00000004,		// CHAT_CUR_CHANNEL_SINGLE
	0x00000008,		// CHAT_CUR_CHANNEL_TEAM
	0x00000010,		// CHAT_CUR_CHANNEL_FACTION
	0x00000020,		// CHAT_CUR_CHANNEL_TONG
	0x00000040,		// CHAT_CUR_CHANNEL_SCREENSINGLE
	0x00000080,		// CHAT_CUR_CHANNEL_SYSTEM
};

UINT	g_dwChannelAnd[CHAT_CUR_CHANNEL_NUM] =
{
	0xffffffff,		// CHAT_CUR_CHANNEL_NONE
	0xfffffffe,		// CHAT_CUR_CHANNEL_ALL
	0xfffffffd,		// CHAT_CUR_CHANNEL_SCREEN
	0xfffffffb,		// CHAT_CUR_CHANNEL_SINGLE
	0xfffffff7,		// CHAT_CUR_CHANNEL_TEAM
	0xffffffef,		// CHAT_CUR_CHANNEL_FACTION
	0xffffffdf,		// CHAT_CUR_CHANNEL_TONG
	0xffffffbf,		// CHAT_CUR_CHANNEL_SCREENSINGLE
	0xffffff7f,		// CHAT_CUR_CHANNEL_SYSTEM
};


//----------------------------------------------------------------------------
//Function: Clear
//----------------------------------------------------------------------------
void	KPlayerChat::Release()
{
#ifndef _SERVER
	m_btIniLoadFlag = FALSE;
	m_nCurChannel = CHAT_CUR_CHANNEL_SCREEN;
	CChatApplyListNode* pNode;
	pNode = (CChatApplyListNode*)m_cApplyAddList.GetHead();
	while (pNode)
	{
		m_cApplyAddList.RemoveHead();
		delete pNode;
		pNode = NULL;
		pNode = (CChatApplyListNode*)m_cApplyAddList.GetHead();
	}
	m_nLoginGetFirstOneFriendFlag = 0;
#else
	memset(m_nAddFriendList, 0, sizeof(this->m_nAddFriendList));
	m_nListPos = 0;
	m_nSyncState = 0;
	m_pStateSendPos = NULL;
	m_nTimer = CHAT_SPEED;
#endif
	m_dwTakeChannel = 0;
	this->SetTakeChannel(g_dwChannelOr[CHAT_CUR_CHANNEL_SCREEN]);
	m_dwTargetID = 0;
	m_nTargetIdx = 0;
	for (INT i = 0; i < MAX_FRIEND_TEAM; ++i)
	{
		m_cFriendTeam[i].Release();
	}
	strcpy(m_cFriendTeam[0].m_szTeamName, CHAT_TEAM_DEFAULT_NAME);
}

#ifdef _SERVER
void	KPlayerChat::SendSystemInfo(INT nType, INT nTargetIdx, char* lpszSendName, char* lpszSentence, INT nSentenceLength, INT nChannedID/* = -1*/)
{
	if (!lpszSendName || !lpszSentence)
		return;

	if (nTargetIdx)
	{
		TReplaceText(lpszSentence, PLAYERNAME_KEY, Npc[Player[nTargetIdx].m_nIndex].Name);
		TReplaceText(lpszSentence, SEXNAME_KEY, Npc[Player[nTargetIdx].m_nIndex].m_nSex ? "C« n­¬ng" : "ThiÕu hiÖp");
		nSentenceLength = strlen(lpszSentence);
	}

	if (nSentenceLength >= MAX_SENTENCE_LENGTH)
		nSentenceLength = MAX_SENTENCE_LENGTH;

	BOOL bAll = nType == 0;


	size_t chatsize = sizeof(CHAT_CHANNELCHAT_SYNC) + nSentenceLength;
	size_t pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

#ifdef WIN32
	tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_malloca(pckgsize);
#else
	tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)(new char[pckgsize]);
#endif
	if (pExHeader != NULL)
	{
		pExHeader->ProtocolType = s2c_extendchat;
		pExHeader->wLength = pckgsize - 1;

		CHAT_CHANNELCHAT_SYNC* pCccSync = (CHAT_CHANNELCHAT_SYNC*)(pExHeader + 1);
		pCccSync->ProtocolType = chat_channelchat;
		pCccSync->wSize = chatsize - 1;
		pCccSync->packageID = -1;
		strncpy(pCccSync->someone, lpszSendName, _NAME_LEN - 1); // ¿ÉÄÜÐèÒª¸ù¾ÝÍæ¼ÒÉí·Ý¸Ä¶¯
		pCccSync->channelid = nChannedID;
		pCccSync->sentlen = nSentenceLength;
		memcpy(pCccSync + 1, lpszSentence, nSentenceLength);
		memset(pCccSync->item, 0, sizeof(pCccSync->item));

		pCccSync->someflag = 0U;

		if (bAll)	// ¸øÈ«ÌåÍæ¼Ò·¢ËÍ
		{
			INT nTargetIdx;
			nTargetIdx = PlayerSet.GetFirstPlayer();
			while (nTargetIdx)
			{
				g_pServer->PackDataToClient(Player[nTargetIdx].m_nNetConnectIdx, pExHeader, pckgsize);
				nTargetIdx = PlayerSet.GetNextPlayer();
			}
		}
		else			// ¸øÄ³¸öÌØ¶¨Íæ¼Ò·¢ËÍ
		{
			if (nTargetIdx <= 0)
			{
#ifndef WIN32
				if (pExHeader)
					delete ((char*)pExHeader);
#endif
				return;
			}
			g_pServer->PackDataToClient(Player[nTargetIdx].m_nNetConnectIdx, pExHeader, pckgsize);
		}
	}
#ifndef WIN32
	if (pExHeader)
		delete ((char*)pExHeader);
#endif
	return;
}
#endif

#ifdef _SERVER
void KPlayerChat::SendGlobalSystemInfo(char* lpszSendName, char* lpszSentence, INT nSentenceLength)
{
	if (!lpszSendName || !lpszSentence)
		return;
	if (nSentenceLength >= MAX_SENTENCE_LENGTH)
		nSentenceLength = MAX_SENTENCE_LENGTH;

	size_t chatsize = sizeof(CHAT_CHANNELCHAT_SYNC) + nSentenceLength;
	size_t pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

#ifdef WIN32
	tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_malloca(pckgsize);
#else
	tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)(new char[pckgsize]);
#endif
	if (pExHeader != NULL)
	{
		pExHeader->ProtocolType = s2c_extendchat;
		pExHeader->wLength = pckgsize - 1;

		CHAT_CHANNELCHAT_SYNC* pCccSync = (CHAT_CHANNELCHAT_SYNC*)(pExHeader + 1);
		pCccSync->ProtocolType = chat_channelchat;
		pCccSync->wSize = chatsize - 1;
		pCccSync->packageID = -1;
		strncpy(pCccSync->someone, lpszSendName, _NAME_LEN - 1); // ¿ÉÄÜÐèÒª¸ù¾ÝÍæ¼ÒÉí·Ý¸Ä¶¯
		pCccSync->channelid = -1;
		pCccSync->sentlen = nSentenceLength;
		memcpy(pCccSync + 1, lpszSentence, nSentenceLength);
		memset(pCccSync->item, 0, sizeof(pCccSync->item));
		pCccSync->someflag = 0U;

		g_NewProtocolProcess.BroadcastGlobal(pExHeader, pckgsize);
	}
#ifndef WIN32
	if (pExHeader)
		delete ((char*)pExHeader);
#endif
}
#endif

#ifdef _SERVER
void KPlayerChat::SendInfoToGM(char* lpszAccName, char* lpszRoleName, char* lpszSentence, INT nSentenceLength)
{
	if (!lpszAccName ||
		!lpszRoleName ||
		!lpszSentence)
		return;
	if (nSentenceLength >= MAX_SENTENCE_LENGTH)
		nSentenceLength = MAX_SENTENCE_LENGTH;

	size_t chatsize = sizeof(CHAT_MSG_EX) + nSentenceLength;
	size_t pckgsize = sizeof(RELAY_ASKWAY_DATA) + chatsize;


#ifdef WIN32
	RELAY_ASKWAY_DATA* pExHeader = (RELAY_ASKWAY_DATA*)_malloca(pckgsize);
#else
	RELAY_ASKWAY_DATA* pExHeader = (RELAY_ASKWAY_DATA*)(new char[pckgsize]);
#endif
	if (pExHeader != NULL)
	{
		pExHeader->ProtocolFamily = pf_relay;
		pExHeader->ProtocolID = relay_c2c_askwaydata;
		pExHeader->nFromIP = 0;
		pExHeader->nFromRelayID = 0;
		pExHeader->seekRelayCount = 0;
		pExHeader->seekMethod = rm_gm;
		pExHeader->wMethodDataLength = 0;
		pExHeader->routeDateLength = chatsize;

		CHAT_MSG_EX* pChatMsgEx = (CHAT_MSG_EX*)(pExHeader + 1);
		pChatMsgEx->ProtocolFamily = pf_playercommunity;
		pChatMsgEx->ProtocolID = playercomm_channelchat;
		strcpy(pChatMsgEx->m_szSourceName, lpszRoleName);
		strcpy(pChatMsgEx->m_szAccountName, lpszAccName);
		pChatMsgEx->SentenceLength = nSentenceLength;
		memcpy(pChatMsgEx + 1, lpszSentence, nSentenceLength);

		g_NewProtocolProcess.PushMsgInTransfer(pExHeader, pckgsize);
	}
#ifndef WIN32
	if (pExHeader)
		delete ((char*)pExHeader);
#endif
}
#endif

#ifdef _SERVER
void KPlayerChat::SendInfoToIP(DWORD nIP, DWORD nID, char* lpszAccName, char* lpszRoleName, char* lpszSentence, INT nSentenceLength)
{
	if (!lpszAccName ||
		!lpszRoleName ||
		!lpszSentence)
		return;
	if (nSentenceLength >= MAX_SENTENCE_LENGTH)
		nSentenceLength = MAX_SENTENCE_LENGTH;

	size_t chatsize = sizeof(CHAT_MSG_EX) + nSentenceLength;
	size_t pckgsize = sizeof(RELAY_DATA) + chatsize;


#ifdef WIN32
	RELAY_DATA* pExHeader = (RELAY_DATA*)_malloca(pckgsize);
#else
	RELAY_DATA* pExHeader = (RELAY_DATA*)(new char[pckgsize]);
#endif
	if (pExHeader != NULL)
	{
		pExHeader->ProtocolFamily = pf_relay;
		pExHeader->ProtocolID = relay_c2c_data;
		pExHeader->nToIP = nIP;
		pExHeader->nToRelayID = nID;
		pExHeader->nFromIP = 0;
		pExHeader->nFromRelayID = 0;
		pExHeader->routeDateLength = chatsize;

		CHAT_MSG_EX* pChatMsgEx = (CHAT_MSG_EX*)(pExHeader + 1);
		pChatMsgEx->ProtocolFamily = pf_playercommunity;
		pChatMsgEx->ProtocolID = playercomm_channelchat;
		strcpy(pChatMsgEx->m_szSourceName, lpszRoleName);
		strcpy(pChatMsgEx->m_szAccountName, lpszAccName);
		pChatMsgEx->SentenceLength = nSentenceLength;
		memcpy(pChatMsgEx + 1, lpszSentence, nSentenceLength);

		g_NewProtocolProcess.PushMsgInTransfer(pExHeader, pckgsize);
	}
#ifndef WIN32
	if (pExHeader)
		delete ((char*)pExHeader);
#endif
}
#endif

#define 	ADDBROTHER_UNITNAME 			"Th©n nh©n\n"
#define 	ADDENEMY_UNITNAME	 			"Cõu nh©n\n"
#define 	ADDMATE_UNITNAME	 			"Phèi ngÉu\n"

#ifdef _SERVER
void KPlayerChat::MakeBrother(const STRINGLIST& brothers)
{
	if (brothers.size() == 0)
		return;

	static const size_t max_packagesize = 1000;
	char buffer[max_packagesize];	//max package size
	size_t maxsize = max_packagesize - 1;	//Áô¸ö0µÄÎ»ÖÃ
	size_t basesize = sizeof(FRIEND_ASSOCIATEBEVY);

	FRIEND_ASSOCIATEBEVY* pGf = (FRIEND_ASSOCIATEBEVY*)(buffer);
	pGf->ProtocolFamily = pf_friend;
	pGf->ProtocolID = friend_c2s_associatebevy;

	INT nG = strlen(ADDBROTHER_UNITNAME) + 1;
	strcpy(buffer + basesize, ADDBROTHER_UNITNAME);
	basesize += nG;

	size_t cursor = basesize;

	for (STRINGLIST::const_iterator itFriend = brothers.begin(); itFriend != brothers.end(); itFriend++)
	{
		std::string dst = *itFriend;
		size_t appendsize = dst.size() + 1;

		if (cursor + appendsize > maxsize)
		{
			buffer[cursor++] = specOver;	//¼Ó¸ö½áÎ²,·¢×ß

			g_NewProtocolProcess.PushMsgInTong(buffer, cursor);

			cursor = basesize;	//´ÓÍ·¿ªÊ¼
		}

		strcpy(buffer + cursor, dst.c_str());
		cursor += appendsize;
	}

	if (cursor > basesize)
	{
		buffer[cursor++] = specOver;

		g_NewProtocolProcess.PushMsgInTong(buffer, cursor);
	}
}
#endif

#ifdef _SERVER
void KPlayerChat::MakeEnemy(char* szPlayer, char* szEnemy)
{
	if (!szPlayer || szPlayer[0] == 0 ||
		!szEnemy || szEnemy[0] == 0)
		return;

	INT nG = strlen(ADDENEMY_UNITNAME) + 1;
	INT nP = strlen(szPlayer) + 1;
	INT nE = strlen(szEnemy) + 1;
	size_t fsize = sizeof(FRIEND_ASSOCIATE) + nG + nP + nE;

#ifdef WIN32
	FRIEND_ASSOCIATE* pCccSync = (FRIEND_ASSOCIATE*)_malloca(fsize);
#else
	FRIEND_ASSOCIATE* pCccSync = (FRIEND_ASSOCIATE*)(new char[fsize]);
#endif
	if (pCccSync != NULL)
	{
		pCccSync->ProtocolFamily = pf_friend;
		pCccSync->ProtocolID = friend_c2s_associate;
		pCccSync->bidir = 0;
		char* pBuf = (char*)(pCccSync + 1);
		strcpy(pBuf, ADDENEMY_UNITNAME);
		pBuf += nG;
		strcpy(pBuf, szPlayer);
		pBuf += nP;
		strcpy(pBuf, szEnemy);
		pBuf += nE;

		g_NewProtocolProcess.PushMsgInTong(pCccSync, fsize);
	}
#ifndef WIN32
	if (pCccSync)
		delete ((char*)pCccSync);
#endif
}
#endif

#ifdef _SERVER
void KPlayerChat::MakeMate(char* szPlayer, char* szMate)
{
	if (!szPlayer || szPlayer[0] == 0 ||
		!szMate || szMate[0] == 0)
		return;

	INT nG = strlen(ADDMATE_UNITNAME) + 1;
	INT nP = strlen(szPlayer) + 1;
	INT nE = strlen(szMate) + 1;
	size_t fsize = sizeof(FRIEND_ASSOCIATE) + nG + nP + nE;

#ifdef WIN32
	FRIEND_ASSOCIATE* pCccSync = (FRIEND_ASSOCIATE*)_malloca(fsize);
#else
	FRIEND_ASSOCIATE* pCccSync = (FRIEND_ASSOCIATE*)(new char[fsize]);
#endif
	if (pCccSync != NULL)
	{
		pCccSync->ProtocolFamily = pf_friend;
		pCccSync->ProtocolID = friend_c2s_associate;
		pCccSync->bidir = 1;
		char* pBuf = (char*)(pCccSync + 1);
		strcpy(pBuf, ADDMATE_UNITNAME);
		pBuf += nG;
		strcpy(pBuf, szPlayer);
		pBuf += nP;
		strcpy(pBuf, szMate);
		pBuf += nE;

		g_NewProtocolProcess.PushMsgInTong(pCccSync, fsize);
	}
#ifndef WIN32
	if (pCccSync)
		delete ((char*)pCccSync);
#endif
}
#endif

BOOL	KPlayerChat::CheckExist(DWORD dwID)
{
	CChatFriend* pNode;
	for (INT i = 0; i < MAX_FRIEND_TEAM; ++i)
	{
		if (m_cFriendTeam[i].m_nFriendNo <= 0)
			continue;
		pNode = (CChatFriend*)m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pNode)
		{
			if (pNode->m_dwID == dwID)
				return TRUE;
			pNode = (CChatFriend*)pNode->GetNext();
		}
	}
	return FALSE;
}

BOOL	KPlayerChat::CheckExist(char* lpszName)
{
	if (!lpszName || !lpszName[0])
		return FALSE;
	CChatFriend* pNode;
	for (INT i = 0; i < MAX_FRIEND_TEAM; ++i)
	{
		if (m_cFriendTeam[i].m_nFriendNo <= 0)
			continue;
		pNode = (CChatFriend*)m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pNode)
		{
			if (strcmp(lpszName, pNode->m_szName) == 0)
				return TRUE;
			pNode = (CChatFriend*)pNode->GetNext();
		}
	}
	return FALSE;
}

CChatFriend* KPlayerChat::GetFriendNode(DWORD dwID)
{
	CChatFriend* pNode;
	for (INT i = 0; i < MAX_FRIEND_TEAM; ++i)
	{
		if (m_cFriendTeam[i].m_nFriendNo <= 0)
			continue;
		pNode = (CChatFriend*)m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pNode)
		{
			if (pNode->m_dwID == dwID)
			{
				return pNode;
			}
			pNode = (CChatFriend*)pNode->GetNext();
		}
	}
	return NULL;
}

INT		KPlayerChat::GetTeamNo(DWORD dwID)
{
	CChatFriend* pNode;
	for (INT i = 0; i < MAX_FRIEND_TEAM; ++i)
	{
		if (i != 0 && !m_cFriendTeam[i].m_szTeamName[0])
			continue;
		if (m_cFriendTeam[i].m_nFriendNo <= 0)
			continue;
		pNode = (CChatFriend*)m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pNode)
		{
			if (pNode->m_dwID == dwID)
			{
				return i;
			}
			pNode = (CChatFriend*)pNode->GetNext();
		}
	}
	return -1;
}

BOOL	KPlayerChat::GetName(DWORD dwID, char* lpszName)
{
	if (!lpszName)
		return FALSE;
	CChatFriend* pNode;
	for (INT i = 0; i < MAX_FRIEND_TEAM; ++i)
	{
		if (m_cFriendTeam[i].m_nFriendNo <= 0)
			continue;
		pNode = (CChatFriend*)m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pNode)
		{
			if (pNode->m_dwID == dwID)
			{
				g_StrCpy(lpszName, pNode->m_szName);
				return TRUE;
			}
			pNode = (CChatFriend*)pNode->GetNext();
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
// Function: The server receives the subscription chat channel information from the client
//----------------------------------------------------------------------------
void KPlayerChat::SetTakeChannel(DWORD dwChannel)
{
	// Player identity judgment, different players have different permissions granted by the system not end
	m_dwTakeChannel = dwChannel | g_dwChannelOr[CHAT_CUR_CHANNEL_ALL] | g_dwChannelOr[CHAT_CUR_CHANNEL_SCREEN] | g_dwChannelOr[CHAT_CUR_CHANNEL_SYSTEM];
}

#ifdef _SERVER
//----------------------------------------------------------------------------
// Function: The server receives the chat message sent by the client
//----------------------------------------------------------------------------
void KPlayerChat::ServerSendChat(INT nPlayerIdx, BYTE* pProtocol)
{
	if (!pProtocol)
		return;

	PLAYER_SEND_CHAT_COMMAND* pChat = (PLAYER_SEND_CHAT_COMMAND*)pProtocol;
	INT nLen = pChat->m_wSentenceLen;
	if (pChat->m_btCurChannel <= CHAT_CUR_CHANNEL_NONE || pChat->m_btCurChannel >= CHAT_CUR_CHANNEL_NUM)
		return;
	if (pChat->m_btChatPrefixLen > CHAT_MSG_PREFIX_MAX_LEN || pChat->m_wSentenceLen >= MAX_SENTENCE_LENGTH)
		return;
	if (pChat->m_wLength != sizeof(PLAYER_SEND_CHAT_COMMAND) - 1 - sizeof(pChat->m_szSentence) + pChat->m_btChatPrefixLen + pChat->m_wSentenceLen)
	{
		printf("[chat]聊天数据包错误！\n");
		return;
	}

	if (pChat->m_btType == MSG_G_CHAT)
	{
		switch (pChat->m_btCurChannel)
		{
		case CHAT_CUR_CHANNEL_ALL:
		{
			// 检查玩家权限，普通玩家没有这个权限 not end
			break;

			if (m_nTimer < CHAT_SPEED)
				break;
			m_nTimer = 0;

			PLAYER_SEND_CHAT_SYNC sChat;
			sChat.ProtocolType = s2c_playersendchat;
			sChat.m_btCurChannel = pChat->m_btCurChannel;
			sChat.m_btNameLen = strlen(Npc[Player[nPlayerIdx].m_nIndex].Name);
			sChat.m_btChatPrefixLen = pChat->m_btChatPrefixLen;
			sChat.m_wSentenceLen = pChat->m_wSentenceLen;
			sChat.m_dwSourceID = Player[nPlayerIdx].m_dwID; // 发送者的player id 可能需要根据玩家身份改动
			sChat.m_wLength = sizeof(PLAYER_SEND_CHAT_SYNC) - 1 - sizeof(sChat.m_szSentence) + sChat.m_btNameLen + sChat.m_btChatPrefixLen + sChat.m_wSentenceLen;
			memcpy(&sChat.m_szSentence[0], Npc[Player[nPlayerIdx].m_nIndex].Name, sChat.m_btNameLen); // 可能需要根据玩家身份改动
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen], &pChat->m_szSentence[0], sChat.m_btChatPrefixLen);
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen + sChat.m_btChatPrefixLen], &pChat->m_szSentence[pChat->m_btChatPrefixLen], sChat.m_wSentenceLen); // 可能需要根据玩家身份改动
			INT nTargetIdx;
			nTargetIdx = PlayerSet.GetFirstPlayer();
			while (nTargetIdx)
			{
				g_pServer->PackDataToClient(Player[nTargetIdx].m_nNetConnectIdx, (BYTE*)&sChat, sChat.m_wLength + 1);
				nTargetIdx = PlayerSet.GetNextPlayer();
			}
			printf("[chat]聊天数据CHANNEL_ALL！\n");
		}
		break;
		case CHAT_CUR_CHANNEL_SCREEN:
		{
			if (m_nTimer < CHAT_SPEED)
				break;
			m_nTimer = 0;

			PLAYER_SEND_CHAT_SYNC sChat;
			sChat.ProtocolType = s2c_playersendchat;
			sChat.m_btCurChannel = pChat->m_btCurChannel;
			sChat.m_btNameLen = strlen(Npc[Player[nPlayerIdx].m_nIndex].Name);
			sChat.m_btChatPrefixLen = pChat->m_btChatPrefixLen;
			sChat.m_wSentenceLen = pChat->m_wSentenceLen;
			sChat.m_dwSourceID = Npc[Player[nPlayerIdx].m_nIndex].m_dwID; // 此处传的是发送者的 npc id
			sChat.m_wLength = sizeof(PLAYER_SEND_CHAT_SYNC) - 1 - sizeof(sChat.m_szSentence) + sChat.m_btNameLen + sChat.m_btChatPrefixLen + sChat.m_wSentenceLen;
			memcpy(&sChat.m_szSentence[0], Npc[Player[nPlayerIdx].m_nIndex].Name, sChat.m_btNameLen); // 可能需要根据玩家身份改动
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen], &pChat->m_szSentence[0], sChat.m_btChatPrefixLen);
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen + sChat.m_btChatPrefixLen], &pChat->m_szSentence[pChat->m_btChatPrefixLen], sChat.m_wSentenceLen); // 可能需要根据玩家身份改动
#ifdef _CHAT_SCRIPT_OPEN
			if (TextGMFilter(nPlayerIdx, (const char*)(pChat->m_szSentence + pChat->m_btChatPrefixLen), sChat.m_wSentenceLen))
				return;
#endif
			Npc[Player[nPlayerIdx].m_nIndex].SendDataToNearRegion((BYTE*)&sChat, sChat.m_wLength + 1);
			printf("[chat]聊天数据CHANNEL_SCREEN！\n");
		}
		break;
		case CHAT_CUR_CHANNEL_SINGLE:
		{
			DWORD dwCheckID = 0;
			INT nIndex = 0;
			if (pChat->m_nTargetIdx <= 0 || pChat->m_nTargetIdx >= MAX_PLAYER)
			{
				INT nSubWorld = Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex;
				INT nRegion = Npc[Player[nPlayerIdx].m_nIndex].m_RegionIndex;
				nIndex = SubWorld[nSubWorld].m_Region[nRegion].FindPlayer(pChat->m_dwTargetID);
				if (nIndex <= 0)
				{
					for (INT i = 0; i < 8; ++i)
					{
						INT nConRegion = SubWorld[nSubWorld].m_Region[nRegion].m_nConnectRegion[i];
						if (nConRegion == -1)
							continue;
						nIndex = SubWorld[nSubWorld].m_Region[nConRegion].FindPlayer(pChat->m_dwTargetID);
						if (nIndex > 0)
							break;
					}
					if (nIndex <= 0)
						return;
				}
				dwCheckID = Player[nIndex].m_dwID;
				//	return;
			}
			else
			{
				dwCheckID = pChat->m_dwTargetID;
				nIndex = pChat->m_nTargetIdx;
			}
			if (Player[nIndex].m_nIndex == 0 ||
				Player[nIndex].m_dwID != dwCheckID)
				return;
			PLAYER_SEND_CHAT_SYNC sChat;
			sChat.ProtocolType = s2c_playersendchat;
			sChat.m_btCurChannel = pChat->m_btCurChannel;
			sChat.m_btNameLen = 0;
			sChat.m_btChatPrefixLen = pChat->m_btChatPrefixLen;
			sChat.m_wSentenceLen = pChat->m_wSentenceLen;
			sChat.m_dwSourceID = Player[nPlayerIdx].m_dwID;
			sChat.m_wLength = sizeof(PLAYER_SEND_CHAT_SYNC) - 1 - sizeof(sChat.m_szSentence) + sChat.m_btNameLen + sChat.m_btChatPrefixLen + sChat.m_wSentenceLen;
			// QQ聊天不传名字
			//				memcpy(&sChat.m_szSentence[0], Npc[Player[nPlayerIdx].m_nIndex].Name, sChat.m_btNameLen);
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen], &pChat->m_szSentence[0], sChat.m_btChatPrefixLen);
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen + sChat.m_btChatPrefixLen], &pChat->m_szSentence[pChat->m_btChatPrefixLen], sChat.m_wSentenceLen); // 可能需要根据玩家身份改动
			g_pServer->PackDataToClient(Player[nIndex].m_nNetConnectIdx, (BYTE*)&sChat, sChat.m_wLength + 1);
			printf("[chat]聊天数据CHANNEL_SINGLE！\n");
		}
		break;
		case CHAT_CUR_CHANNEL_TEAM:
		{
			if (m_nTimer < CHAT_SPEED)
				break;
			m_nTimer = 0;

			if (!Player[nPlayerIdx].m_cTeam.m_nFlag) // 不在队伍中
				return;
			PLAYER_SEND_CHAT_SYNC sChat;
			sChat.ProtocolType = s2c_playersendchat;
			sChat.m_btCurChannel = pChat->m_btCurChannel;
			sChat.m_btNameLen = strlen(Npc[Player[nPlayerIdx].m_nIndex].Name);
			sChat.m_btChatPrefixLen = pChat->m_btChatPrefixLen;
			sChat.m_wSentenceLen = pChat->m_wSentenceLen;
			sChat.m_dwSourceID = Npc[Player[nPlayerIdx].m_nIndex].m_dwID; // 此处传的是发送者的 npc id
			sChat.m_wLength = sizeof(PLAYER_SEND_CHAT_SYNC) - 1 - sizeof(sChat.m_szSentence) + sChat.m_btNameLen + sChat.m_btChatPrefixLen + sChat.m_wSentenceLen;
			memcpy(&sChat.m_szSentence[0], Npc[Player[nPlayerIdx].m_nIndex].Name, sChat.m_btNameLen); // 可能需要根据玩家身份改动
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen], &pChat->m_szSentence[0], sChat.m_btChatPrefixLen);
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen + sChat.m_btChatPrefixLen], &pChat->m_szSentence[pChat->m_btChatPrefixLen], sChat.m_wSentenceLen); // 可能需要根据玩家身份改动
			INT nTargetIdx;
			// 给队长发
			nTargetIdx = g_Team[Player[nPlayerIdx].m_cTeam.m_nID].m_nCaptain;
			if (nTargetIdx != nPlayerIdx && (Player[nTargetIdx].m_cChat.m_dwTakeChannel & g_dwChannelOr[CHAT_CUR_CHANNEL_TEAM]))
				g_pServer->PackDataToClient(Player[nTargetIdx].m_nNetConnectIdx, (BYTE*)&sChat, sChat.m_wLength + 1);
			// 给队员发
			for (INT i = 0; i < MAX_TEAM_MEMBER; ++i)
			{
				nTargetIdx = g_Team[Player[nPlayerIdx].m_cTeam.m_nID].m_nMember[i];
				if (nTargetIdx < 0 || nTargetIdx == nPlayerIdx)
					continue;
				if (Player[nTargetIdx].m_cChat.m_dwTakeChannel & g_dwChannelOr[CHAT_CUR_CHANNEL_TEAM])
					g_pServer->PackDataToClient(Player[nTargetIdx].m_nNetConnectIdx, (BYTE*)&sChat, sChat.m_wLength + 1);
			}
			printf("[chat]聊天数据CHANNEL_TEAM！\n");
		}
		break;
		case CHAT_CUR_CHANNEL_FACTION:
		{
			if (m_nTimer < CHAT_SPEED)
				break;
			m_nTimer = 0;

			INT nFaction = Player[nPlayerIdx].m_cFaction.m_nCurFaction;
			if (nFaction < 0) // 不在门派中
				return;
			PLAYER_SEND_CHAT_SYNC sChat;
			sChat.ProtocolType = s2c_playersendchat;
			sChat.m_btCurChannel = pChat->m_btCurChannel;
			sChat.m_btNameLen = strlen(Npc[Player[nPlayerIdx].m_nIndex].Name);
			sChat.m_btChatPrefixLen = pChat->m_btChatPrefixLen;
			sChat.m_wSentenceLen = pChat->m_wSentenceLen;
			sChat.m_dwSourceID = Npc[Player[nPlayerIdx].m_nIndex].m_dwID; // 此处传的是发送者的 npc id
			sChat.m_wLength = sizeof(PLAYER_SEND_CHAT_SYNC) - 1 - sizeof(sChat.m_szSentence) + sChat.m_btNameLen + sChat.m_btChatPrefixLen + sChat.m_wSentenceLen;
			memcpy(&sChat.m_szSentence[0], Npc[Player[nPlayerIdx].m_nIndex].Name, sChat.m_btNameLen); // 可能需要根据玩家身份改动
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen], &pChat->m_szSentence[0], sChat.m_btChatPrefixLen);
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen + sChat.m_btChatPrefixLen], &pChat->m_szSentence[pChat->m_btChatPrefixLen], sChat.m_wSentenceLen); // 可能需要根据玩家身份改动
			INT nTargetIdx;
			nTargetIdx = PlayerSet.GetFirstPlayer();
			while (nTargetIdx)
			{
				if (Player[nTargetIdx].m_cFaction.m_nCurFaction == nFaction && (Player[nTargetIdx].m_cChat.m_dwTakeChannel & g_dwChannelOr[CHAT_CUR_CHANNEL_FACTION]))
					g_pServer->PackDataToClient(Player[nTargetIdx].m_nNetConnectIdx, (BYTE*)&sChat, sChat.m_wLength + 1);
				nTargetIdx = PlayerSet.GetNextPlayer();
			}
			printf("[chat]聊天数据CHANNEL_FACTION！\n");
		}
		break;
		case CHAT_CUR_CHANNEL_TONG:
		{
			if (m_nTimer < CHAT_SPEED)
				break;
			m_nTimer = 0;
		}
		break;
		case CHAT_CUR_CHANNEL_SCREENSINGLE:
		{
			if (m_nTimer < CHAT_SPEED)
				break;
			m_nTimer = 0;

			INT nTargetIdx;
			nTargetIdx = Player[nPlayerIdx].FindAroundPlayer(pChat->m_dwTargetID);
			if (nTargetIdx == -1)
				return;
			if (!Player[nTargetIdx].m_cChat.CheckTalkChannel(CHAT_CUR_CHANNEL_SCREENSINGLE))
			{
				CHAT_SCREENSINGLE_ERROR_SYNC sError;
				sError.ProtocolType = s2c_chatscreensingleerror;
				sError.m_btError = 0;
				memset(sError.m_szName, 0, sizeof(sError.m_szName));
				g_StrCpy(sError.m_szName, Npc[Player[nTargetIdx].m_nIndex].Name);
				sError.m_wLength = 2 + 1 + strlen(sError.m_szName);
				g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sError, sError.m_wLength + 1);
				return;
			}
			PLAYER_SEND_CHAT_SYNC sChat;
			sChat.ProtocolType = s2c_playersendchat;
			sChat.m_btCurChannel = pChat->m_btCurChannel;
			sChat.m_btNameLen = strlen(Npc[Player[nPlayerIdx].m_nIndex].Name);
			sChat.m_btChatPrefixLen = pChat->m_btChatPrefixLen;
			sChat.m_wSentenceLen = pChat->m_wSentenceLen;
			sChat.m_dwSourceID = Npc[Player[nPlayerIdx].m_nIndex].m_dwID; // 此处传的是发送者的 npc id
			sChat.m_wLength = sizeof(PLAYER_SEND_CHAT_SYNC) - 1 - sizeof(sChat.m_szSentence) + sChat.m_btNameLen + sChat.m_btChatPrefixLen + sChat.m_wSentenceLen;
			memcpy(&sChat.m_szSentence[0], Npc[Player[nPlayerIdx].m_nIndex].Name, sChat.m_btNameLen); // 可能需要根据玩家身份改动
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen], &pChat->m_szSentence[0], sChat.m_btChatPrefixLen);
			memcpy(&sChat.m_szSentence[sChat.m_btNameLen + sChat.m_btChatPrefixLen], &pChat->m_szSentence[pChat->m_btChatPrefixLen], sChat.m_wSentenceLen); // 可能需要根据玩家身份改动
			g_pServer->PackDataToClient(Player[nTargetIdx].m_nNetConnectIdx, (BYTE*)&sChat, sChat.m_wLength + 1);

			printf("[chat]聊天数据CHANNEL_SCREENSINGLE！\n");
		}
		break;
		case CHAT_CUR_CHANNEL_SYSTEM:
			// 在此种情况下普通玩家不允许使用此频道 not end
			if (pChat->m_dwTargetID == 0) // to all
			{
				char szSentence[MAX_SENTENCE_LENGTH], szPrefix[CHAT_MSG_PREFIX_MAX_LEN];
				memcpy(szSentence, &pChat->m_szSentence[pChat->m_btChatPrefixLen], nLen); // 可能需要根据玩家身份改动
				szSentence[nLen] = 0;
				memcpy(szPrefix, &pChat->m_szSentence[0], pChat->m_btChatPrefixLen);
				INT nLength;
				nLength = TEncodeText(szSentence, strlen(szSentence));
				KPlayerChat::SendSystemInfo(0, 0, MESSAGE_SYSTEM_ANNOUCE_HEAD, szSentence, nLength);
			}
			else
			{
				char szSentence[MAX_SENTENCE_LENGTH], szPrefix[CHAT_MSG_PREFIX_MAX_LEN];
				INT nIdx;
				memcpy(szSentence, &pChat->m_szSentence[pChat->m_btChatPrefixLen], nLen); // 可能需要根据玩家身份改动
				szSentence[nLen] = 0;
				memcpy(szPrefix, &pChat->m_szSentence[0], pChat->m_btChatPrefixLen);
				nIdx = PlayerSet.FindSame(pChat->m_dwTargetID);
				if (nIdx > 0)
				{
					INT nLength;
					nLength = TEncodeText(szSentence, strlen(szSentence));
					KPlayerChat::SendSystemInfo(1, nIdx, MESSAGE_SYSTEM_ANNOUCE_HEAD, szSentence, nLength);
				}
			}
			printf("[chat]聊天数据CHANNEL_SYSTEM！\n");
			break;
		}
	}
	else if (pChat->m_btType == MSG_G_CMD)
	{
	}
	else
	{
		return;
	}
}
#endif

#ifdef _SERVER
//----------------------------------------------------------------------------
//Function: Exclusively used to obtain data from the database when the player logs in
//Parameters: nSelfIdx This player’s position in Player dwID Friend ID lpszName Friend’s name
//----------------------------------------------------------------------------
void KPlayerChat::DataBaseAddOne(INT nSelfIdx, DWORD dwID, char* lpszName)
{
	this->m_cFriendTeam[0].DataBaseAddOne(nSelfIdx, dwID, lpszName);
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：添加好友数据
//---------------------------------------------------------------------------
void KPlayerChat::AddFriendData(INT nSelfIdx, INT nDestIdx)
{
	if (this->CheckExist(Player[nDestIdx].m_dwID))
		return;
	CChatFriend* pFriend = NULL;
	pFriend = new CChatFriend;
	if (!pFriend)
		return;
	pFriend->m_dwID = Player[nDestIdx].m_dwID;
	pFriend->m_nPlayerIdx = nDestIdx;
	g_StrCpy(pFriend->m_szName, Npc[Player[nDestIdx].m_nIndex].Name);
	m_cFriendTeam[0].m_cEveryOne.AddTail(pFriend);
	m_cFriendTeam[0].m_nFriendNo++;

	CHAT_ADD_FRIEND_SYNC sAdd;
	sAdd.ProtocolType = s2c_chataddfriend;
	sAdd.m_dwID = Player[nDestIdx].m_dwID;
	sAdd.m_nIdx = nDestIdx;
	memset(sAdd.m_szName, 0, 32);
	g_StrCpy(sAdd.m_szName, Npc[Player[nDestIdx].m_nIndex].Name);

	g_pServer->PackDataToClient(Player[nSelfIdx].m_nNetConnectIdx, (BYTE*)&sAdd, sizeof(CHAT_ADD_FRIEND_SYNC));
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：重新发送某个好友数据
//  dwID 好友 id   nPlayerIdx 玩家自己
//---------------------------------------------------------------------------
void KPlayerChat::ResendOneFriendData(DWORD dwID, INT nPlayerIdx)
{
	BOOL bFind = FALSE;
	CChatFriend* pFriend;
	pFriend = (CChatFriend*)m_cFriendTeam[0].m_cEveryOne.GetHead();
	while (pFriend)
	{
		if (pFriend->m_dwID == dwID)
		{
			bFind = TRUE;
			break;
		}
		pFriend = (CChatFriend*)pFriend->GetNext();
	}
	if (!bFind)
		return;
	CHAT_ONE_FRIEND_DATA_SYNC sData;
	sData.ProtocolType = s2c_chatonefrienddata;
	sData.m_dwID = dwID;
	sData.m_nPlayerIdx = pFriend->m_nPlayerIdx;
	memset(sData.m_szName, 0, sizeof(sData.m_szName));
	g_StrCpy(sData.m_szName, pFriend->m_szName);

	g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sData, sizeof(CHAT_ONE_FRIEND_DATA_SYNC));
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：玩家登录时加载数据库玩家好友数据完成后自动依次发送好友数据给客户端
//---------------------------------------------------------------------------
BOOL KPlayerChat::SyncFriendData(INT nPlayerIdx, UINT& nParam)
{
	BOOL bRet = FALSE;
	if (!m_nSyncState) // 平时不做任何处理 m_nSyncState == 0
		return TRUE;

	if (m_nSyncState == 1) // 玩家刚登录，发送不带名字的聊天好友数据
	{
		if (!m_pStateSendPos)
		{
			m_nSyncState = 0;
			nParam = 2;
			return TRUE;
		}

		CHAT_LOGIN_FRIEND_NONAME_SYNC sFriend;
		sFriend.ProtocolType = s2c_chatloginfriendnoname;
		sFriend.m_dwID = m_pStateSendPos->m_dwID;
		sFriend.m_nPlayerIdx = m_pStateSendPos->m_nPlayerIdx;
		if (SUCCEEDED(g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sFriend, sizeof(CHAT_LOGIN_FRIEND_NONAME_SYNC))))
		{
			bRet = TRUE;
		}
		else
		{
			printf("player Packing friend sync data failed...\n");
			bRet = FALSE;
		}

		m_pStateSendPos = (CChatFriend*)m_pStateSendPos->GetNext();
		return bRet;
	}

	if (m_nSyncState == 2) // 玩家刚登录，发送带名字的聊天好友数据(玩家客户端没有好友分组和名字信息)
	{
		if (!m_pStateSendPos)
		{
			m_nSyncState = 0;
			nParam = 2;
			return TRUE;
		}

		CHAT_LOGIN_FRIEND_NAME_SYNC sFriend;
		sFriend.ProtocolType = s2c_chatloginfriendname;
		sFriend.m_dwID = m_pStateSendPos->m_dwID;
		sFriend.m_nPlayerIdx = m_pStateSendPos->m_nPlayerIdx;
		g_StrCpy(sFriend.m_szName, m_pStateSendPos->m_szName);
		sFriend.m_wLength = sizeof(CHAT_LOGIN_FRIEND_NAME_SYNC) - 1 - sizeof(sFriend.m_szName) + strlen(sFriend.m_szName);
		if (SUCCEEDED(g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sFriend, sFriend.m_wLength + 1)))
		{
			bRet = TRUE;
		}
		else
		{
			printf("player Packing friend 2 sync data failed...\n");
			bRet = FALSE;
		}

		m_pStateSendPos = (CChatFriend*)m_pStateSendPos->GetNext();
		return bRet;
	}
	return TRUE;
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：玩家登录时加载数据库玩家好友数据完成后调用次方法开始发送给客户端好友数据
//---------------------------------------------------------------------------
void KPlayerChat::StartSendFriendData()
{
	this->SetTakeChannel(g_dwChannelOr[CHAT_CUR_CHANNEL_SCREEN]);
	m_pStateSendPos = (CChatFriend*)m_cFriendTeam[0].m_cEveryOne.GetHead();
	m_nSyncState = 2;
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：删除某个好友数据（会通知对方相应处理）
//---------------------------------------------------------------------------
void KPlayerChat::DeleteFriend(DWORD dwID, DWORD dwSelfID)
{
	CChatFriend* pFriend;
	pFriend = (CChatFriend*)m_cFriendTeam[0].m_cEveryOne.GetHead();
	while (pFriend)
	{
		if (pFriend->m_dwID == dwID)
		{
			pFriend->Remove();
			m_cFriendTeam[0].m_nFriendNo--;

			// 通知被删除方执行相应处理
			if (pFriend->m_nPlayerIdx)
			{ // 如果对方在线，id 正确
				if (pFriend->m_nPlayerIdx < MAX_PLAYER && Player[pFriend->m_nPlayerIdx].m_nIndex && Player[pFriend->m_nPlayerIdx].m_dwID == dwID)
				{
					Player[pFriend->m_nPlayerIdx].m_cChat.DeleteFriendData(dwSelfID, pFriend->m_nPlayerIdx);
				}
			}

			delete pFriend;
			pFriend = NULL;
			return;
		}
		pFriend = (CChatFriend*)pFriend->GetNext();
	}
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：删除某个好友数据（会通知自己客户端）
//---------------------------------------------------------------------------
void KPlayerChat::DeleteFriendData(DWORD dwID, INT nSelfIndex)
{
	CChatFriend* pFriend;
	pFriend = (CChatFriend*)this->m_cFriendTeam[0].m_cEveryOne.GetHead();
	while (pFriend)
	{
		if (pFriend->m_dwID == dwID)
		{
			pFriend->Remove();
			delete pFriend;
			pFriend = NULL;
			m_cFriendTeam[0].m_nFriendNo--;
			// 通知客户端
			CHAT_DELETE_FRIEND_SYNC sDelete;
			sDelete.ProtocolType = s2c_chatdeletefriend;
			sDelete.m_dwID = dwID;
			g_pServer->PackDataToClient(Player[nSelfIndex].m_nNetConnectIdx, (BYTE*)&sDelete, sizeof(CHAT_DELETE_FRIEND_SYNC));

			return;
		}
		pFriend = (CChatFriend*)pFriend->GetNext();
	}
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：再次删除某好友
//---------------------------------------------------------------------------
void KPlayerChat::RedeleteFriend(DWORD dwID, INT nSelfIndex)
{
	if (!CheckExist(dwID))
	{
		INT nIdx;
		nIdx = PlayerSet.FindSame(dwID);
		if (nIdx > 0)
			Player[nIdx].m_cChat.DeleteFriendData(Player[nSelfIndex].m_dwID, nIdx);
	}
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：本玩家下线
//---------------------------------------------------------------------------
void KPlayerChat::OffLine(DWORD dwSelfID)
{
	CChatFriend* pNode;
	pNode = (CChatFriend*)m_cFriendTeam[0].m_cEveryOne.GetHead();
	while (pNode)
	{
		if (pNode->m_nPlayerIdx)
		{
			Player[pNode->m_nPlayerIdx].m_cChat.GetMsgOffLine(dwSelfID, pNode->m_nPlayerIdx);
		}
		pNode = (CChatFriend*)pNode->GetNext();
	}
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：得到通知有好友下线
//---------------------------------------------------------------------------
void KPlayerChat::GetMsgOffLine(DWORD dwID, INT nSelfIdx)
{
	CChatFriend* pNode;
	pNode = (CChatFriend*)m_cFriendTeam[0].m_cEveryOne.GetHead();
	while (pNode)
	{
		if (pNode->m_nPlayerIdx && pNode->m_dwID == dwID)
		{
			pNode->m_nPlayerIdx = 0;
			// 通知客户端
			CHAT_FRIEND_OFFLINE_SYNC sSync;
			sSync.ProtocolType = s2c_chatfriendoffline;
			sSync.m_dwID = dwID;
			g_pServer->PackDataToClient(Player[nSelfIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(CHAT_FRIEND_OFFLINE_SYNC));
			return;
		}
		pNode = (CChatFriend*)pNode->GetNext();
	}
}
#endif

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：聊天时间计数加一
//---------------------------------------------------------------------------
void KPlayerChat::TimerAdd()
{
	this->m_nTimer++;
}
#endif

BOOL KPlayerChat::CheckTalkChannel(INT nChannel)
{
	if (nChannel <= CHAT_CUR_CHANNEL_NONE || nChannel >= CHAT_CUR_CHANNEL_NUM)
		return FALSE;
	return (this->m_dwTakeChannel & g_dwChannelOr[nChannel]);
}

#ifdef _SERVER
//---------------------------------------------------------------------------
//	功能：玩家登陆的时候从数据库获得数据时专用
//---------------------------------------------------------------------------
void CChatFriendTeam::DataBaseAddOne(INT nSelfIdx, DWORD dwID, char* lpszName)
{
	if (!lpszName || !lpszName[0])
		return;

	// 判断好友是否在线
	CChatFriend* pFriend = NULL;
	pFriend = new CChatFriend;
	if (!pFriend)
		return;
	pFriend->m_dwID = dwID;
	pFriend->m_nPlayerIdx = PlayerSet.FindSame(dwID);
	g_StrCpy(pFriend->m_szName, lpszName);
	m_cEveryOne.AddTail(pFriend);
	m_nFriendNo++;

	// 通知好友自己上线了
	if (pFriend->m_nPlayerIdx > 0)
		Player[pFriend->m_nPlayerIdx].ChatFriendOnLine(Player[nSelfIdx].m_dwID, nSelfIdx);
}
#endif

//---------------------------------------------------------------------------
//	功能：清空
//---------------------------------------------------------------------------
void CChatFriendTeam::Release()
{
	m_nFriendNo = 0;
	memset(m_szTeamName, 0, 32);
	CChatFriend* pFriend = NULL;
	pFriend = (CChatFriend*)m_cEveryOne.GetHead();
	while (pFriend)
	{
		m_cEveryOne.RemoveHead();
		delete pFriend;
		pFriend = NULL;
		pFriend = (CChatFriend*)m_cEveryOne.GetHead();
	}
}

//---------------------------------------------------------------------------
//	功能：构造函数
//---------------------------------------------------------------------------
CChatFriendTeam::CChatFriendTeam()
{
	m_nFriendNo = 0;
	memset(m_szTeamName, 0, 32);
}