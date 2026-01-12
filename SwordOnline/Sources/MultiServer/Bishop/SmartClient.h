/********************************************************************
	created:	2003/08/22
	file base:	SmartClient
	file ext:	h
	author:		liupeng
	
	purpose:	
*********************************************************************/
#ifndef __INCLUDE_SMARTCLIENT__
#define __INCLUDE_SMARTCLIENT__

#pragma once

#include "IClient.h"
#include "RainbowInterface.h"

#include "Library.h"
#include "Buffer.h"
#include "tstring.h"
#include "CriticalSection.h"
#include "Network.h"

#include <list>

class CSmartClient
{
public:

	CSmartClient();
	virtual ~CSmartClient();

	bool Open( const char * const pAddressToConnectServer, 
		unsigned short usPortToConnectServer, 
		HWND hwndContain,
		const char *pName,
		const char *pPwd,
		bool bReLink = true,
		CNetwork m_Network = CNetwork());

	void Close();

	bool Valid() { return ( NULL != m_pClient ); };

	bool Send( const void * const pData, const size_t &datalength );
	const void *Recv( size_t &datalength );	

	static DWORD WINAPI	WorkingThreadFunction( void *pV );

	HWND								m_hwndContain;

protected:

	bool CreateConnectThread();
	IClient* _Connect();
	void _Verify(IClient* pClient, bool bDisconnect);
	static void __stdcall EventNotify( LPVOID lpParam, 
				const ULONG &ulnEventType );

	void _EventNotify( const ULONG &ulnEventType );

	enum enumServerLoginErrorCode
	{
		enumConnectFailed	= 0xA1,
		enumUsrNamePswdErr,
		enumIPPortErr,
		enumException
	};

private:

	typedef HRESULT ( __stdcall * pfnCreateClientInterface )( REFIID riid, void **ppv );

	static OnlineGameLib::Win32::CLibrary	m_theRainbowLib;

	OnlineGameLib::Win32::CCriticalSection	m_csHistroyList;
	typedef std::list< OnlineGameLib::Win32::CBuffer * > stdList;
	stdList m_theHistroyList;

	HANDLE m_hWorkingThread;

	bool m_bReLink;		//ʹ�ñ�����Ҫ�Զ���������
	
	/*
	 * Config
	 */
	IClient		*m_pClient;

	OnlineGameLib::Win32::_tstring		m_sUserName;
	OnlineGameLib::Win32::_tstring		m_sUserPwd;

	OnlineGameLib::Win32::_tstring		m_sSvrIP;
	unsigned short						m_nSvrPort;
	OnlineGameLib::Win32::_tstring		m_sMAC;

	OnlineGameLib::Win32::CBuffer::Allocator	m_theAllocator;

	static CNetwork m_theNetwork;

	OnlineGameLib::Win32::CCriticalSection	m_csStatus;
	enum enumClientStatus
	{
		enumClientNormal = 0x00,
		enumClientConnect,
		enumClientVerify,
		enumClientError,
	};

	enumClientStatus m_ClientStatus;

	bool IsNormal();
	bool IsConnecting();
	bool IsVerifying();
	bool IsError();

	void SwitchToConnecting();		//�л�״̬������,���б�Ҫ�����������߳�
	bool SetClientStatus(enumClientStatus nNewS);
};

extern CSmartClient g_theSmartClient;

#endif // __INCLUDE_SMARTCLIENT__