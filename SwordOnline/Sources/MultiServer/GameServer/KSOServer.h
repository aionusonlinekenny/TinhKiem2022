#ifndef KSOServerH
#define	KSOServerH

#include <map>
#include "KEngine.h"
#include "IClient.h"
#ifndef _STANDALONE
#include "RainbowInterface.h"
#include "HeavenInterface.h"
#include "..\..\Core\src\CoreServerShell.h"
#include "UsesWinsock.h"
#include "Buffer.h"
using OnlineGameLib::Win32::CPackager;

#else
#include "CoreServerShell.h"
#endif

#include "IServer.h"
#include "KTimer.h"
#include "KTransferUnit.h"

#include <memory>
#include <vector>


#define	CHECK_IPADDRESS		0
#define IPADDRESS			"AD17CAD2885008B2510DBB1EE67A6E12"	//7FD59087F7EEE0A5A8DF9F0480254AC4	192.168.94.1

enum NetStatus
{
	enumNetUnconnect = 0,
	enumNetConnected,
	enumNetOffline,
};

struct GameStatus
{
	GameStatus()
	{
		nGameStatus = 0;
		nNetStatus = 0;
		nExchangeStatus = 0;
		nPlayerIndex = 0;
		nSendPingTime = 0;
		nReplyPingTime = 0;
		bIsOffline = FALSE;
		bIsLogoutGateway = FALSE;
		nLastSave = 0;
	};
	INT					nPlayerIndex;
	INT					nGameStatus;
	INT					nNetStatus;
	INT					nExchangeStatus;
	INT					nSendPingTime;
	INT					nReplyPingTime;
	BOOL				bIsOffline;
	BOOL				bIsLogoutGateway;
	INT					nLastSave;
};

class KSwordOnLineSever
{
private:
#ifdef _STANDALONE
	ZBuffer* net_buffer;
#endif
	INT					m_nMaxPlayerCount;
	INT					m_nPrecision;
	static const INT	m_snMaxBuffer;
	static const INT	m_snBufferSize;
	INT					m_nMaxPlayer;
	INT					m_nGameLoop;
	INT					m_nServerPort;
	INT					m_nGatewayPort;
	INT					m_nDatabasePort;
	INT					m_nTransferPort;
	INT					m_nChatPort;
	INT					m_nTongPort;
	DWORD				m_dwIntranetIp;
	DWORD				m_dwInternetIp;
	char				m_szGameServerIP[16];
	char				m_szGatewayIP[16];
	char				m_szDatabaseIP[16];
	char				m_szTransferIP[16];
	char				m_szChatIP[16];
	char				m_szTongIP[16];
	char				m_szIntranetIP[16];
	char				m_szInternetIP[16];
	char				m_szMAC[16];
	BOOL				m_bIsRunning;

	BOOL				m_bRunningDataBase;
	BOOL				m_bRunningGateway;
	//	BOOL				m_bSaveFlag;
	//	INT					m_nSaveCount;
	IServer* m_pServer;
	IClient* m_pGatewayClient;
	IClient* m_pDatabaseClient;
	IClient* m_pTransferClient;
	IClient* m_pChatClient;
	IClient* m_pTongClient;
	//GameStatus* m_pGameStatus;
	std::vector<std::unique_ptr<GameStatus>> m_pGameStatus;

	struct iCoreServerShell* m_pCoreServerShell;
	KTimer				m_Timer;
	typedef std::map<DWORD, KTransferUnit*>	IP2CONNECTUNIT;
	IP2CONNECTUNIT		m_mapIp2TransferUnit;
public:
	KSwordOnLineSever();
	~KSwordOnLineSever();
	BOOL				Init();
	BOOL				Breathe();
	void				AutoSave();
	INT					GetNetStatus(const ULONG lnID);
	void				SetNetStatus(const ULONG lnID, NetStatus nStatus);
	void				SetRunningStatus(BOOL bStatus);

	void				SetRunningDataBase(BOOL bStatus);
	BOOL				GetRunningDataBase();

	void                SetRunningGateway(BOOL bStatus);
	BOOL                GetRunningGateway();

	void				Release();
private:
	void				MessageLoop();
	void				GatewayMessageProcess(const char* pChar, size_t nSize);
	void				DatabaseMessageProcess(const char* pChar, size_t nSize);
	void				DatabaseLargePackProcess(const char* pChar, size_t nSize);
	void				TransferMessageProcess(const char* pChar, size_t nSize);
	void				PlayerMessageProcess(const ULONG lnID, const char* pChar, size_t nSize);
	void				GatewayLargePackProcess(const void* pData, size_t dataLength);
	void				GatewaySmallPackProcess(const void* pData, size_t dataLength);
	void				GatewayBoardCastProcess(const char* pData, size_t dataLength);
	void				GatewayPunishProcess(const char* pData, size_t dataLength);
	void				TransferLargePackProcess(const void* pData, size_t dataLength, KTransferUnit* pUnit);
	void				TransferSmallPackProcess(const void* pData, size_t dataLength, KTransferUnit* pUnit);
	void				TransferAskWayMessageProcess(const char* pData, size_t dataLength);
	void				TransferLoseWayMessageProcess(const char* pData, size_t dataLength);
	void				ChatMessageProcess(const char* pChar, size_t nSize);
	void				ChatGroupMan(const void* pData, size_t dataLength);
	void				ChatSpecMan(const void* pData, size_t dataLength);

	BOOL				ConformAskWay(const void* pData, INT nSize, DWORD* pdwID);
	void				MainLoop();
	INT					ProcessLoginProtocol(const ULONG lnID, const char* pData, size_t dataLength);
	BOOL				ProcessSyncReplyProtocol(const ULONG lnID, const char* pData, size_t dataLength);
	void				PingClient(const ULONG lnID);
	void				ProcessPingReply(const ULONG lnID, const char* pData, size_t dataLength);

	BOOL				SendGameDataToClient(const ULONG lnID, const INT nPlayerIndex);
	void				SavePlayerData();
	BOOL				SavePlayerData(UINT nIndex, bool bUnLock);
	void				ExitAllPlayer();
	BOOL				ExitPlayer(INT lnID);
	void				PlayerLogoutGateway();
	void				PlayerExchangeServer();
	BOOL 				GetLocalIpAddress(DWORD* pIntranetAddr, DWORD* pInternetAddr, DWORD nMask, DWORD nMacLenght);

	// ���Э�鴦��( relay ��������Э��)
	void				TongMessageProcess(const char* pChar, size_t nSize);
	// ���Э�鴦��(�ͻ���ͨ�������չЭ�鷢������Э��)
	void				ProcessPlayerTongMsg(const ULONG nPlayerIdx, const char* pData, size_t dataLength);

	BOOL				CheckPlayerID(ULONG netidx, DWORD nameid);
	INT				NumberConnection();
};
void WriteFile(const char* szLinks, const char* szStr);
#endif

#ifndef _CCRC32_H
#define _CCRC32_H

#define CRC32_POLYNOMIAL 0x04C11DB7
#define CRC32BUFSZ 1024

class CCRC32
{
public:
	void Initialize(void) {
		memset(&this->ulTable, 0, sizeof(this->ulTable));
		for (INT iCodes = 0; iCodes <= 0xFF; iCodes++)
		{
			this->ulTable[iCodes] = this->Reflect(iCodes, 8) << 24;
			for (INT iPos = 0; iPos < 8; iPos++)
			{
				this->ulTable[iCodes] = (this->ulTable[iCodes] << 1) ^
					(this->ulTable[iCodes] & (1 << 31) ? CRC32_POLYNOMIAL : 0);
			}
			this->ulTable[iCodes] = this->Reflect(this->ulTable[iCodes], 32);
		}
	};
	ULONG FileCRC(const char* sFileName)
	{
		ULONG ulCRC = 0xffffffff;
		FILE* fSource = NULL;
		unsigned char sBuf[CRC32BUFSZ];
		INT iBytesRead = 0;
		if ((fSource = fopen(sFileName, "rb")) == NULL)
		{
			return 0xffffffff;
		}
		do
		{
			iBytesRead = fread(sBuf, sizeof(char), CRC32BUFSZ, fSource);
			this->PartialCRC(&ulCRC, sBuf, iBytesRead);
		} while (iBytesRead == CRC32BUFSZ);
		fclose(fSource);
		return(ulCRC ^ 0xffffffff);
	};
	ULONG FullCRC(unsigned char* sData, ULONG ulLength)
	{
		ULONG ulCRC = 0xffffffff;
		this->PartialCRC(&ulCRC, sData, ulLength);
		return ulCRC ^ 0xffffffff;
	};
	void PartialCRC(ULONG* ulInCRC, unsigned char* sData, ULONG ulLength)
	{
		while (ulLength--)
		{
			*ulInCRC = (*ulInCRC >> 8) ^ this->ulTable[(*ulInCRC & 0xFF) ^ *sData++];
		}
	};

private:
	ULONG Reflect(ULONG ulReflect, char cChar)
	{
		ULONG ulValue = 0;
		for (INT iPos = 1; iPos < (cChar + 1); iPos++)
		{
			if (ulReflect & 1) ulValue |= 1 << (cChar - iPos);
			ulReflect >>= 1;
		}
		return ulValue;
	};
	ULONG ulTable[256];
};
#endif
