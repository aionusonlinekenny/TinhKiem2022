// NetServer.h: interface for the CNetServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETSERVER_H__312A07AD_96A8_40C4_914B_C13DD706CB40__INCLUDED_)
#define AFX_NETSERVER_H__312A07AD_96A8_40C4_914B_C13DD706CB40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "../../Multiserver/Heaven/Interface/IServer.h"

#include "NetConnect.h"

class CNetServer 
{
public:
	CNetServer();
	virtual ~CNetServer();

private:
	IServer* m_pServer;
	BOOL m_ready;

private:
	static void __stdcall ServerEventNotify (
				LPVOID lpParam,
				const ULONG &ulnID,
				const ULONG &ulnEventType );

public:
	BOOL Startup(UINT nPlayerMaxCount, UINT nPrecision, UINT maxFreeBuffers_Cache, UINT bufferSize_Cache, ULONG ulnAddressToListenOn, unsigned short usnPortToListenOn);
	BOOL Shutdown();

	UINT GetConnectCount();

	BOOL IsConnectReady(ULONG id);

	BOOL Disconnect(ULONG id);

	IServer* GetServer() const {return m_ready ? m_pServer : NULL;}

	CNetConnectDup FindNetConnect(ULONG id);

	BOOL BroadPackage(const void* pData, UINT size);

private:
	typedef std::map<ULONG, CNetConnect*>	ID2CONNECTMAP;
	ID2CONNECTMAP m_mapId2Connect;

private:
	void _NotifyClientConnectCreate(ULONG ulnID);
	void _NotifyClientConnectClose(ULONG ulnID);

protected:
	virtual CNetConnect* CreateConnect(CNetServer* pNetServer, ULONG id) = NULL;
	virtual void DestroyConnect(CNetConnect* pConn) = NULL;

	virtual void OnBuildup() {}
	virtual void OnClearup() {}

	virtual void OnClientConnectCreate(CNetConnect* pConn) {}
	virtual void OnClientConnectClose(CNetConnect* pConn) {}

private:
	CLockMRSW m_lockAccess;

public:
	BOOL Route();

protected:
	BOOL DoSendPackage(const void* pData, UINT size);

	friend class CNetConnect;
	friend class CNetConnectDup;
};

#endif // !defined(AFX_NETSERVER_H__312A07AD_96A8_40C4_914B_C13DD706CB40__INCLUDED_)
