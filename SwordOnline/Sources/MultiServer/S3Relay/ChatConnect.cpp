// ChatConnect.cpp: implementation of the CChatConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatConnect.h"
#include "ChatServer.h"
#include "S3Relay.h"
#include "Global.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatConnect::CChatConnect(CChatServer* pChatServer, ULONG id)
	: CNetConnect(pChatServer, id)
{

}

CChatConnect::~CChatConnect()
{

}


void CChatConnect::OnClientConnectCreate()
{
	rTRACE("chat connect create: %s", _ip2a(GetIP()));
}

void CChatConnect::OnClientConnectClose()
{
	rTRACE("chat connect close: %s", _ip2a(GetIP()));
}

void CChatConnect::RecvPackage(const void* pData, UINT size)
{
	BYTE protocol = *(BYTE*)pData;

	if (protocol == chat_someonechat)
	{
		Proc0_SomeoneChat(pData, size);
	}
	else if (protocol == chat_channelchat)
	{
		Proc0_ChannelChat(pData, size);
	}
	else if (protocol == chat_groupman)
	{
		Proc0_GroupMan(pData, size);
	}
}

void CChatConnect::Proc0_SomeoneChat(const void* pData, UINT size)
{
	CHAT_SOMEONECHAT_CMD* pCsc = (CHAT_SOMEONECHAT_CMD*)pData;
	tagPlusSrcInfo* pSrcInfo = (tagPlusSrcInfo*)((BYTE*)pData + size) - 1;

	g_ChannelMgr.SomeoneChat(GetIP(), *pSrcInfo, pCsc);
}

void CChatConnect::Proc0_ChannelChat(const void* pData, UINT size)
{
	CHAT_CHANNELCHAT_CMD* pCcc = (CHAT_CHANNELCHAT_CMD*)pData;
	tagPlusSrcInfo* pSrcInfo = (tagPlusSrcInfo*)((BYTE*)pData + size) - 1;

	g_ChannelMgr.ChannelChat(GetIP(), *pSrcInfo, pCcc);
}

void CChatConnect::Proc0_GroupMan(const void* pData, UINT size)
{
	CHAT_GROUPMAN* pCgc = (CHAT_GROUPMAN*)pData;
	CHAT_CHANNELCHAT_SYNC* pCccSync = (CHAT_CHANNELCHAT_SYNC*)(pCgc + 1);
	if (pCccSync->ProtocolType != chat_channelchat)
		return;	//unexpect

	void* pSent = pCccSync + 1;
	void* pPlayersData = (BYTE*)pSent + pCccSync->sentlen;


	DWORD srcIP = GetIP();

	const UINT playercount = pCgc->wPlayerCount;
	pCgc->wPlayerCount = 0;

	if (pCgc->byHasIdentify)
	{
		tagPlusSrcInfo* pPlayers = (tagPlusSrcInfo*)(pPlayersData);
		for (UINT i = 0; i < playercount; i++)
		{
			if (g_ChannelMgr.IsSubscribed(srcIP, pPlayers[i].lnID, pCccSync->channelid))
			{
				if (pCgc->wPlayerCount < i)
					pPlayers[pCgc->wPlayerCount] = pPlayers[i];
				++pCgc->wPlayerCount;
			}
		}
	}
	else
	{
		WORD* pPlayers = (WORD*)(pPlayersData);
		for (UINT i = 0; i < playercount; i++)
		{
			if (g_ChannelMgr.IsSubscribed(srcIP, (ULONG)pPlayers[i], pCccSync->channelid))
			{
				if (pCgc->wPlayerCount < i)
					pPlayers[pCgc->wPlayerCount] = pPlayers[i];
				++pCgc->wPlayerCount;
			}
		}
	}

	if (pCgc->wPlayerCount <= 0)
		return;


	UINT size2 = size - (playercount - pCgc->wPlayerCount) * (pCgc->byHasIdentify ? sizeof(tagPlusSrcInfo) : sizeof(WORD));
	pCgc->wSize = size2 - 1;

	SendPackage(pCgc, size2);
}
