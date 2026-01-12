/************************************************ *****************************************
//	½çÃæ--login´°¿Ú
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-13
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KWin32.h"
#include "KEngine.h"
#include "LoginDef.h"
#include "Login.h"
#include "../NetConnect/NetConnectAgent.h"
#include "KProtocol.h"
#include "crtdbg.h"
#include "../Ui/UiBase.h"
#include "../Ui/UiShell.h"
#include "../../Engine/Src/Cryptography/EDOneTimePad.h"
#include "time.h"

#define	SERVER_LIST_FILE				"\\Settings\\ServerList.ini"

KLogin		g_LoginLogic;


bool GetIpAddress(const char* szAddress, unsigned char* pcAddress)
{
	_ASSERT(pcAddress);
	INT nValue[4];
	INT nRet = sscanf(szAddress, "%d.%d.%d.%d", &nValue[0], &nValue[1], &nValue[2], &nValue[3]);
	if (nRet == 4 &&
		nValue[0] >= 0 && nValue[0] < 256 &&
		nValue[1] >= 0 && nValue[1] < 256 &&
		nValue[2] >= 0 && nValue[2] < 256 &&
		nValue[3] >= 0 && nValue[3] < 256)
	{
		pcAddress[0] = nValue[0];
		pcAddress[1] = nValue[1];
		pcAddress[2] = nValue[2];
		pcAddress[3] = nValue[3];
		return true;
	}
	return false;
}
//static unsigned gs_holdrand = time(NULL);
//
//static inline unsigned _Rand()
//{
//    gs_holdrand = gs_holdrand * 244213L + 1541021L;
//     
//    return gs_holdrand;
//}

#include <openssl/rand.h>
#include <stdexcept>

// Hàm tạo khóa 32-bit bằng OpenSSL
static inline INT _Rand() {
	INT key;

	// Sử dụng RAND_bytes từ OpenSSL để tạo ngẫu nhiên và an toàn hơn
	if (RAND_bytes(reinterpret_cast<unsigned char*>(&key), sizeof(key)) != 1) {
		// Xử lý lỗi khi không thể tạo khóa
		throw std::runtime_error("Error generating key.");
	}

	return key;
}

void RandMemSet(INT nSize, unsigned char* pbyBuffer)
{
	_ASSERT(nSize);
	_ASSERT(pbyBuffer);

	while (nSize--)
	{
		*pbyBuffer++ = (unsigned char)_Rand();
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//--------------------------------------------------------------------------
KLogin::KLogin()
{
	m_Status = LL_S_IDLE;
	m_Result = LL_R_NOTHING;
	m_bInAutoProgress = false;
	m_nNumRole = 0;
	memset(&m_Choices, 0, sizeof(LOGIN_CHOICE));
	ClearAccountPassword(true, true);
	m_LeftTime = 0;
	m_LeftLockTime = 0;
}

void KLogin::ClearAccountPassword(bool bAccount, bool bPassword)
{
	if (bAccount)
		memset(m_Choices.Account, 0xff, sizeof(m_Choices.Account));
	if (bPassword)
		memset(&m_Choices.Password, 0xff, sizeof(m_Choices.Password));
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÎö¹¹º¯Êý
//--------------------------------------------------------------------------
KLogin::~KLogin()
{
	_ASSERT(m_Status == LL_S_IDLE);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÓë£¨ÕËºÅ£©·þÎñÆ÷½¨Á¢Á¬½Ó
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_IDLE -> LL_S_WAIT_INPUT_ACCOUNT
//			  Ê§°Ü ×´Ì¬±£³Ö
//--------------------------------------------------------------------------
INT KLogin::CreateConnection(const unsigned char* pAddress)
{
	INT nRet;
	if (m_Status == LL_S_IDLE && pAddress && ConnectAccountServer(pAddress))
	{
		RegistNetAgent();
		m_Status = LL_S_WAIT_INPUT_ACCOUNT;
		m_Result = LL_R_NOTHING;

		if (m_bInAutoProgress)
		{
			char	szAccount[32];
			KSG_PASSWORD Password;
			GetAccountPassword(szAccount, &Password);
			AccountLogin(szAccount, Password, false);
			memset(szAccount, 0, sizeof(szAccount));
			memset(&Password, 0, sizeof(Password));
		}
		nRet = true;
	}
	else
	{
		if (m_bInAutoProgress)
			m_bInAutoProgress = false;
		m_Result = LL_R_CONNECT_FAILED;
		nRet = false;
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´«ÈëÕÊºÅÃÜÂë£¬ÕËºÅµÇÂ½
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_WAIT_INPUT_ACCOUNT -> LL_S_ACCOUNT_CONFIRMING
//			  Ê§°Ü ×´Ì¬±£³Ö
//--------------------------------------------------------------------------
INT	KLogin::AccountLogin(const char* pszAccount, const KSG_PASSWORD& crPassword, bool bOrignPassword)
{
	INT nRet;
	if (m_Status == LL_S_WAIT_INPUT_ACCOUNT &&
		pszAccount &&
		Request(pszAccount, &crPassword, LOGIN_A_LOGIN))
	{
		if (bOrignPassword)
		{
			SetAccountPassword(pszAccount, &crPassword);
		}
		m_Status = LL_S_ACCOUNT_CONFIRMING;
		m_Result = LL_R_NOTHING;
		nRet = true;
	}
	else
	{
		if (m_bInAutoProgress)
			m_bInAutoProgress = false;
		m_Result = LL_R_CONNECT_FAILED;
		nRet = false;
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÑ¡ÖÐÓÎÏ·½ÇÉ«
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_ROLE_LIST_READY -> LL_S_WAIT_TO_LOGIN_GAMESERVER
//			  Ê§°Ü ×´Ì¬±£³Ö
//--------------------------------------------------------------------------
INT	KLogin::SelectRole(INT nIndex)
{
	INT nRet;
	if (m_Status == LL_S_ROLE_LIST_READY && nIndex >= 0 && nIndex < m_nNumRole)
	{
		tagDBSelPlayer	NetCommand;
		NetCommand.cProtocol = c2s_dbplayerselect;
		strcpy(NetCommand.szRoleName, m_RoleList[nIndex].Name);
		g_NetConnectAgent.SendMsg(&NetCommand, sizeof(tagDBSelPlayer));
		g_NetConnectAgent.UpdateClientRequestTime(false);
		strcpy(m_Choices.szProcessingRoleName, NetCommand.szRoleName);
		m_Status = LL_S_WAIT_TO_LOGIN_GAMESERVER;
		m_Result = LL_R_NOTHING;
		nRet = true;
	}
	else
	{
		if (m_bInAutoProgress)
			m_bInAutoProgress = false;
		m_Result = LL_R_CONNECT_FAILED;
		nRet = false;
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÇëÇóÐÂ½¨ÓÎÏ·½ÇÉ«
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_ROLE_LIST_READY -> LL_S_CREATING_ROLE
//			  Ê§°Ü ×´Ì¬±£³Ö
//--------------------------------------------------------------------------
INT	KLogin::CreateRole(KRoleChiefInfo* pCreateInfo)
{
	INT nRet = false;
	m_Result = LL_R_CONNECT_FAILED;

	if (m_Status == LL_S_ROLE_LIST_READY && pCreateInfo && m_nNumRole < MAX_PLAYER_PER_ACCOUNT &&
		pCreateInfo->Attribute >= 0 && pCreateInfo->Attribute < series_num)
	{
		INT nNameLen = strlen(pCreateInfo->Name);
		if (nNameLen >= LOGIN_ROLE_NAME_MIN_LEN && nNameLen <= LOGIN_ROLE_NAME_MAX_LEN)
		{
			char	Data[sizeof(TProcessData) + sizeof(NEW_PLAYER_COMMAND)];
			TProcessData* pNetCommand = (TProcessData*)&Data;
			NEW_PLAYER_COMMAND* pInfo = (NEW_PLAYER_COMMAND*)pNetCommand->pDataBuffer;
			pInfo->m_btRoleNo = pCreateInfo->Gender;
			pInfo->m_btSeries = pCreateInfo->Attribute;
			pInfo->m_NativePlaceId = pCreateInfo->NativePlaceId;
			memcpy(pInfo->m_szName, pCreateInfo->Name, nNameLen);
			pInfo->m_szName[nNameLen] = '\0';

			pNetCommand->nProtoId = c2s_newplayer;
			pNetCommand->nDataLen = sizeof(NEW_PLAYER_COMMAND) - sizeof(pInfo->m_szName) + nNameLen + 1/* sizeof( '\0' ) */;
			pNetCommand->ulIdentity = 0;

			g_NetConnectAgent.SendMsg(&Data, sizeof(TProcessData) - sizeof(pNetCommand->pDataBuffer) + pNetCommand->nDataLen);
			g_NetConnectAgent.UpdateClientRequestTime(false);

			memcpy(m_Choices.szProcessingRoleName, pCreateInfo->Name, nNameLen);
			m_Choices.szProcessingRoleName[nNameLen] = 0;

			m_Status = LL_S_CREATING_ROLE;
			m_Result = LL_R_NOTHING;
			nRet = true;
		}
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÇëÇóÉ¾³ýÓÎÏ·½ÇÉ«
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_ROLE_LIST_READY -> LL_S_DELETING_ROLE
//			  Ê§°Ü ×´Ì¬±£³Ö
//--------------------------------------------------------------------------
INT	KLogin::DeleteRole(INT nIndex, const KSG_PASSWORD& crSupperPassword)
{
	//return FALSE;
	INT nRet;

	if (m_Status == LL_S_ROLE_LIST_READY && nIndex >= 0 && nIndex < m_nNumRole)
	{
		tagDBDelPlayer	NetCommand;
		RandMemSet(sizeof(tagDBDelPlayer), (BYTE*)&NetCommand);	// random memory for make a cipher

		NetCommand.cProtocol = c2s_roleserver_deleteplayer;
		GetAccountPassword(NetCommand.szAccountName, NULL);
		NetCommand.Password = crSupperPassword;
		strncpy(NetCommand.szRoleName, m_RoleList[nIndex].Name, sizeof(NetCommand.szRoleName));
		NetCommand.szRoleName[sizeof(NetCommand.szRoleName) - 1] = '\0';

		g_NetConnectAgent.SendMsg(&NetCommand, sizeof(tagDBDelPlayer));
		memset(&NetCommand.Password, 0, sizeof(NetCommand.Password));
		g_NetConnectAgent.UpdateClientRequestTime(false);

		strcpy(m_Choices.szProcessingRoleName, m_RoleList[nIndex].Name);

		m_Status = LL_S_DELETING_ROLE;
		m_Result = LL_R_NOTHING;
		nRet = true;
	}
	else
	{
		nRet = false;
		m_Result = LL_R_CONNECT_FAILED;
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍ¨ÖªµÈ´ý·µ»Ø½á¹û³¬Ê±ÁË
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_??? -> LL_S_IDLE
//--------------------------------------------------------------------------
void KLogin::NotifyTimeout()
{
	if (m_Status != LL_S_IDLE)
	{
		ReturnToIdle();
		m_Result = LL_R_CONNECT_TIMEOUT;
	}
}

//Í¨ÖªÍøÂçÁ¬½Ó£¨ÒâÍâ£©¶Ï¿ªÁË
void KLogin::NotifyDisconnect()
{
	if (m_Status != LL_S_IDLE)
	{
		ReturnToIdle();
		m_Result = LL_R_CONNECT_FAILED;
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍ¨ÖªÒª¿ªÊ¼ÓÎÏ·ÁË
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_ENTERING_GAME -> LL_S_IN_GAME
//--------------------------------------------------------------------------
void KLogin::NotifyToStartGame()
{
	if (m_Status == LL_S_ENTERING_GAME)
	{
		g_NetConnectAgent.UpdateClientRequestTime(true);

		char	szAccount[32];
		GetAccountPassword(szAccount, NULL);
		g_UiBase.SetUserAccount(szAccount, m_Choices.szProcessingRoleName);

		m_Status = LL_S_IN_GAME;
		m_Result = LL_R_NOTHING;
		if (m_bInAutoProgress)
			m_bInAutoProgress = false;
		UiOnGameServerStartSyncEnd();
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»Øµ½¿ÕÏÐ×´Ì¬
//	×´Ì¬ÇÐ»»£ºLL_S_??? -> LL_S_IN_GAME
//--------------------------------------------------------------------------
void KLogin::ReturnToIdle()
{
	if (m_Status != LL_S_IDLE)
	{
		UnRegistNetAgent();
		g_NetConnectAgent.DisconnectGameSvr();
		g_NetConnectAgent.DisconnectClient();
		m_Status = LL_S_IDLE;
	}
	m_Choices.bIsRoleNewCreated = false;
	m_Result = LL_R_NOTHING;
	m_bInAutoProgress = false;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÈ«³Ì×Ô¶¯Á¬½Ó
//--------------------------------------------------------------------------
void KLogin::AutoLogin()
{
	ReturnToIdle();
	if (IsAutoLoginEnable())
	{
		m_bInAutoProgress = true;
		if (m_Choices.AccountServer.Address[0] == 0 &&
			m_Choices.AccountServer.Address[1] == 0 &&
			m_Choices.AccountServer.Address[2] == 0 &&
			m_Choices.AccountServer.Address[3] == 0)
		{
			INT nCount, nSel;
			KLoginServer* pList = GetServerList(-1, nCount, nSel);
			if (pList)
			{
				free(pList);
				pList = NULL;
			}
		}
		CreateConnection(m_Choices.AccountServer.Address);
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÅÐ¶ÏÊÇ·ñ¿ÉÒÔÖ´ÐÐÈ«³Ì×Ô¶¯Á¬½Ó
//--------------------------------------------------------------------------
INT	KLogin::IsAutoLoginEnable()
{
	return ((~m_Choices.Account[0]) &&
		(~m_Choices.Password.szPassword[0]) &&
		m_Choices.szProcessingRoleName[0] &&
		m_Choices.AccountServer.Title[0]);
}

void KLogin::SetLastInvisibleFlag(INT nEnable)
{
	m_Choices.nLastInvisible = nEnable;
}

//ÉèÖÃ¼ÍÂ¼±ê¼Ç
void KLogin::SetRememberAccountFlag(bool bEnable)
{
	m_Choices.bRememberAccount = bEnable;
	if (bEnable == false)
		m_Choices.bRememberAll = false;
}

//ÉèÖÃ¼ÍÂ¼±ê¼Ç
void KLogin::SetRememberAllFlag(bool bEnable)
{
	m_Choices.bRememberAll = bEnable;
	if (bEnable)
		m_Choices.bRememberAccount = true;
}

void KLogin::SetVirtualKeyboardFlag(bool bEnable)
{
	m_Choices.bVirtualKeyboard = bEnable;
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñÈ¡Ä³¸ö½ÇÉ«µÄÐÅÏ¢
//--------------------------------------------------------------------------
INT	KLogin::GetRoleInfo(INT nIndex, KRoleChiefInfo* pInfo)
{
	if (nIndex >= 0 && nIndex < m_nNumRole)
	{
		if (pInfo)
			*pInfo = m_RoleList[nIndex];
		return true;
	}
	return false;
}


//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´¦ÀíÕËºÅµÇÂ½µÄÏìÓ¦
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_ACCOUNT_CONFIRMING -> LL_S_WAIT_ROLE_LIST
//			  Ê§°Ü LL_S_ACCOUNT_CONFIRMING -> LL_S_IDLE
//--------------------------------------------------------------------------
void KLogin::ProcessAccountLoginResponse(KLoginStructHead* pResponse)
{
	//_ASSERT(m_Status == LL_S_ACCOUNT_CONFIRMING && pResponse != NULL);
	if (((pResponse->Param & LOGIN_ACTION_FILTER) == LOGIN_A_LOGIN) &&	//²Ù×÷ÐÔÎªÒªÆ¥Åä
		pResponse->Size >= sizeof(KLoginAccountInfo))				//Êý¾ÝÄÚÈÝµÄ´óÐ¡Ò²ÒªÆ¥Åä
	{
		INT nResult = ((pResponse->Param) & ~LOGIN_ACTION_FILTER);
		if (nResult == LOGIN_R_INVALID_PROTOCOLVERSION)
		{
			m_Result = LL_R_INVALID_PROTOCOLVERSION;
			m_Status = LL_S_IDLE;
		}
		else
		{
			KLoginAccountInfo* pInfo = (KLoginAccountInfo*)pResponse;
			char	szAccount[32];
			KSG_PASSWORD Password;
			GetAccountPassword(szAccount, &Password);
			if (strcmp(pInfo->Account, szAccount) == 0 && strcmp(pInfo->Password.szPassword, Password.szPassword) == 0)
			{
				if (nResult == LOGIN_R_SUCCESS)
				{
					g_NetConnectAgent.UpdateClientRequestTime(false);
					m_Status = LL_S_WAIT_ROLE_LIST;
					m_Result = LL_R_ACCOUNT_CONFIRM_SUCCESS;
					m_LeftTime = pInfo->nLeftTime;
				}
				else
				{
					LOGIN_LOGIC_RESULT_INFO eResult = LL_R_NOTHING;
					switch (nResult)
					{
					case LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR:
						eResult = LL_R_ACCOUNT_PWD_ERROR;
						m_Status = LL_S_WAIT_INPUT_ACCOUNT;
						break;
					case LOGIN_R_ACCOUNT_EXIST:
						eResult = LL_R_ACCOUNT_LOCKED;
						m_Status = LL_S_WAIT_INPUT_ACCOUNT;
						break;
					case LOGIN_R_FREEZE:
						m_LeftLockTime = pInfo->nLeftTime;
						eResult = LL_R_ACCOUNT_FREEZE;
						m_Status = LL_S_WAIT_INPUT_ACCOUNT;
						break;
					case LOGIN_R_INVALID_PROTOCOLVERSION:
						eResult = LL_R_INVALID_PROTOCOLVERSION;
						break;
					case LOGIN_R_FAILED:
						eResult = LL_R_CONNECT_SERV_BUSY;
						break;
					case LOGIN_R_TIMEOUT:
						eResult = LL_R_ACCOUNT_NOT_ENOUGH_POINT;
						m_Status = LL_S_WAIT_INPUT_ACCOUNT;
						break;
					default:
						eResult = LL_R_CONNECT_FAILED;
						break;
					}
					if (eResult == LL_R_ACCOUNT_PWD_ERROR ||
						m_Status != LL_S_WAIT_INPUT_ACCOUNT ||
						m_bInAutoProgress)
					{
						ReturnToIdle();
					}
					m_Result = eResult;
				}
			}
			memset(szAccount, 0, sizeof(szAccount));
			memset(&Password, 0, sizeof(Password));
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º½ÇÉ«ÁÐ±í·µ»Ø
//	×´Ì¬ÇÐ»»£º³É¹¦ LL_S_WAIT_ROLE_LIST -> LL_S_ROLE_LIST_READY
//			  Ê§°Ü ×´Ì¬±£³Ö
//--------------------------------------------------------------------------
void KLogin::ProcessRoleListResponse(TProcessData* pResponse)
{
	//_ASSERT(m_Status == LL_S_WAIT_ROLE_LIST && pResponse != NULL);
	if (pResponse->nProtoId == s2c_roleserver_getrolelist_result)
	{
		m_nNumRole = pResponse->pDataBuffer[0];
		if (m_nNumRole > MAX_PLAYER_PER_ACCOUNT)
			m_nNumRole = MAX_PLAYER_PER_ACCOUNT;
		else if (m_nNumRole < 0)
			m_nNumRole = 0;

		RoleBaseInfo* pList = (RoleBaseInfo*)&pResponse->pDataBuffer[1];
		for (INT i = 0; i < m_nNumRole; i++)
		{
			if (pList->szName[0])
			{
				strcpy(m_RoleList[i].Name, pList->szName);
				m_RoleList[i].Attribute = pList->Series;
				m_RoleList[i].Gender = pList->Sex;
				m_RoleList[i].Faction = pList->Faction;
				m_RoleList[i].nLevel = pList->Level;
				pList++;
			}
			else
			{
				m_nNumRole = i;
				break;
			}
		}

		g_NetConnectAgent.UpdateClientRequestTime(true);
		m_Status = LL_S_ROLE_LIST_READY;
		m_Result = LL_R_NOTHING;

		if (m_bInAutoProgress)
		{
			INT nAdviceChoice;
			GetRoleCount(nAdviceChoice);
			SelectRole(nAdviceChoice);
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÐÂ½¨½ÇÉ«ÁÐ±íµÄÏìÓ¦
//	×´Ì¬ÇÐ»»£ºLL_S_CREATING_ROLE -> LL_S_ROLE_LIST_READY
//--------------------------------------------------------------------------
void KLogin::ProcessCreateRoleResponse(tagNewDelRoleResponse* pResponse)
{
	//_ASSERT(m_Status == LL_S_DELETING_ROLE && pResponse != NULL);
	if (pResponse->cProtocol == s2c_rolenewdelresponse)
	{
		//	if (strcmp(pResponse->szRoleName, m_Choices.szProcessingRoleName) == 0)	//to be check/* ÁõÅôµ÷ÊÔ°æ */
		{
			if (pResponse->bSucceeded)
			{
				g_NetConnectAgent.UpdateClientRequestTime(false);
				m_Choices.bIsRoleNewCreated = true;
				m_Status = LL_S_WAIT_TO_LOGIN_GAMESERVER;
				m_Result = LL_R_CREATE_ROLE_SUCCESS;
			}
			else
			{
				g_NetConnectAgent.UpdateClientRequestTime(true);
				m_Status = LL_S_ROLE_LIST_READY;
				m_Result = LL_R_INVALID_ROLENAME;
			}
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉ¾³ý½ÇÉ«ÁÐ±íµÄÏìÓ¦
//	×´Ì¬ÇÐ»»£ºLL_S_DELETING_ROLE -> LL_S_ROLE_LIST_READY
//--------------------------------------------------------------------------
void KLogin::ProcessDeleteRoleResponse(tagNewDelRoleResponse* pResponse)
{
	//_ASSERT(m_Status == LL_S_DELETING_ROLE && pResponse != NULL);
	if (pResponse->cProtocol == s2c_rolenewdelresponse)
	{
		//		if (strcmp(pResponse->szRoleName, m_Choices.szProcessingRoleName) == 0)	//to be check/* ÁõÅôµ÷ÊÔ°æ */
		{
			g_NetConnectAgent.UpdateClientRequestTime(true);
			m_Status = LL_S_ROLE_LIST_READY;
			if (pResponse->bSucceeded)
			{
				char	szAccount[32];
				GetAccountPassword(szAccount, NULL);
				g_UiBase.SetUserAccount(szAccount, m_Choices.szProcessingRoleName);
				g_UiBase.CleanPrivateDataFolder();

				for (INT i = 0; i < m_nNumRole; i++)
				{
					if (strcmp(m_RoleList[i].Name, m_Choices.szProcessingRoleName) == 0)
					{
						m_nNumRole--;
						for (; i < m_nNumRole; i++)
							m_RoleList[i] = m_RoleList[i + 1];
						break;
					}
				}
				m_Result = LL_R_NOTHING;
			}
			else
			{
				m_Result = LL_R_INVALID_PASSWORD;
			}
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º×¼±¸µÇÂ½ÓÎÏ··þÎñÆ÷µÄÏìÓ¦
//	×´Ì¬ÇÐ»»£ºLL_S_WAIT_TO_LOGIN_GAMESERVER -> LL_S_ENTERING_GAME
//--------------------------------------------------------------------------
void KLogin::ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse)
{
	//_ASSERT(m_Status == LL_S_WAIT_TO_LOGIN_GAMESERVER && pResponse != NULL);
	if (pResponse->cProtocol == s2c_notifyplayerlogin)
	{
		//		g_DebugLog("%s,%s",(const char*)pResponse->szRoleName, m_Choices.szProcessingRoleName);
		if (strcmp((const char*)pResponse->szRoleName, m_Choices.szProcessingRoleName) == 0)
		{
			g_NetConnectAgent.UpdateClientRequestTime(true);

			// ¿ªÊ¼ÓëGameSvr½øÐÐÁ¬½Ó
			if (g_NetConnectAgent.ConnectToGameSvr(
				(const unsigned char*)&pResponse->nIPAddr,
				pResponse->nPort, &pResponse->guid))
			{
				m_Status = LL_S_ENTERING_GAME;
				m_Result = LL_R_NOTHING;
			}
			else
			{
				ReturnToIdle();
				m_Result = LL_R_CONNECT_FAILED;
			}

			// ¶Ï¿ªÓëÍø¹ØµÄÁ¬½Ó
			g_NetConnectAgent.DisconnectClient();
		}
		else
		{
			ReturnToIdle();
			m_Result = LL_R_SERVER_SHUTDOWN;
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º½ÓÊÜÍøÂçÏûÏ¢
//--------------------------------------------------------------------------
void KLogin::AcceptNetMsg(void* pMsgData)
{
	if (pMsgData == NULL)
		return;

	switch (m_Status)
	{
	case LL_S_ACCOUNT_CONFIRMING:
		ProcessAccountLoginResponse((KLoginStructHead*)(((char*)pMsgData) + PROTOCOL_MSG_SIZE));
		break;
	case LL_S_WAIT_ROLE_LIST:
		ProcessRoleListResponse((TProcessData*)pMsgData);
		break;
	case LL_S_CREATING_ROLE:
		ProcessCreateRoleResponse((tagNewDelRoleResponse*)pMsgData);
		break;
	case LL_S_DELETING_ROLE:
		ProcessDeleteRoleResponse((tagNewDelRoleResponse*)pMsgData);
		break;
	case LL_S_WAIT_TO_LOGIN_GAMESERVER:
		ProcessToLoginGameServResponse((tagNotifyPlayerLogin*)pMsgData);
		break;
	}
}

//»ñÈ¡²Ù×÷µÄ½á¹ûÐÅÏ¢
LOGIN_LOGIC_RESULT_INFO KLogin::GetResult()
{
	LOGIN_LOGIC_RESULT_INFO eReturn = m_Result;
	m_Result = LL_R_NOTHING;
	return eReturn;
}


//»ñÈ¡½ÇÉ«µÄÊýÄ¿
INT KLogin::GetRoleCount(INT& nAdviceChoice)
{
	nAdviceChoice = 0;
	if (m_Choices.szProcessingRoleName[0])
	{
		for (INT i = 0; i < m_nNumRole; i++)
		{
			if (strcmp(m_Choices.szProcessingRoleName, m_RoleList[i].Name) == 0)
			{
				nAdviceChoice = i;
				break;
			}
		}
	}
	return m_nNumRole;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñÈ¡½¨Òé£¨¾ÉµÄ£©µÇÂ½ÕËºÅ
//--------------------------------------------------------------------------
bool KLogin::GetLoginAccount(char* pszAccount)
{
	if (pszAccount)
		GetAccountPassword(pszAccount, NULL);
	return m_Choices.bRememberAccount;
}

bool KLogin::GetLoginVirtualKeyboard()
{
	return m_Choices.bVirtualKeyboard;
}


#define	$LOGIN					"Login"
#define	$LAST_ACCOUNT			"LastAccount"
#define	$LAST_PASSWORD			"LastPassword"

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¶ÁÈ¡ÒÔÇ°µÄµÄµÇÂ½Ñ¡Ôñ
//--------------------------------------------------------------------------
void KLogin::LoadLoginChoice()
{
	if (m_Choices.bLoaded)
		return;
	memset(&m_Choices, 0, sizeof(m_Choices));
	ClearAccountPassword(true, true);

	m_Choices.bLoaded = true;

	KIniFile* pSetting = g_UiBase.GetCommSettingFile();
	char	szAccount[32];
	KSG_PASSWORD Password;
	if (pSetting)
	{
		INT nValue;
		pSetting->GetString($LOGIN, "LastRegionName", "", m_Choices.RegionName, sizeof(m_Choices.RegionName));
		pSetting->GetString($LOGIN, "LastGameServer", "", m_Choices.AccountServer.Title, sizeof(m_Choices.AccountServer.Title));
		pSetting->GetInteger($LOGIN, "VirtualKeyboard", 0, &nValue);
		pSetting->GetInteger($LOGIN, "LastInvisible", 0, &m_Choices.nLastInvisible);
		m_Choices.bVirtualKeyboard = nValue > 0;

		szAccount[0] = 0;
		pSetting->GetStruct($LOGIN, $LAST_ACCOUNT, szAccount, sizeof(szAccount));
		if (szAccount[0])
		{
			EDOneTimePad_Decipher(szAccount, strlen(szAccount));
			m_Choices.bRememberAccount = true;
			SetAccountPassword(szAccount, NULL);

			Password.szPassword[0] = '\0';
			pSetting->GetStruct($LOGIN, $LAST_PASSWORD, Password.szPassword, sizeof(Password.szPassword));
			if (Password.szPassword[0])
			{
				EDOneTimePad_Decipher(Password.szPassword, strlen(Password.szPassword));
				m_Choices.bRememberAll = true;
				SetAccountPassword(NULL, &Password);
				memset(&Password, 0, sizeof(Password));
			}
		}

		if (szAccount[0])
		{
			KIniFile* pPrivate = g_UiBase.GetPrivateSettingFile();
			if (pPrivate)
			{
				if (pPrivate->GetString("Main", "LastSelCharacter", "",
					m_Choices.szProcessingRoleName, sizeof(m_Choices.szProcessingRoleName)))
				{
					EDOneTimePad_Decipher(m_Choices.szProcessingRoleName, strlen(m_Choices.szProcessingRoleName));
				}
			}
			g_UiBase.ClosePrivateSettingFile(false);
		}

		g_UiBase.CloseCommSettingFile(false);
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º±£´æµÇÂ½Ñ¡ÔñÉèÖÃ
//--------------------------------------------------------------------------
void KLogin::SaveLoginChoice()
{

	KIniFile* pSetting = g_UiBase.GetCommSettingFile();
	INT	i;
	if (pSetting)
	{
		if (m_Choices.RegionName[0])
		{
			pSetting->WriteString($LOGIN, "LastRegionName", m_Choices.RegionName);
		}
		if (m_Choices.AccountServer.Title[0])
		{
			pSetting->WriteString($LOGIN, "LastGameServer", m_Choices.AccountServer.Title);
		}
		pSetting->WriteInteger($LOGIN, "VirtualKeyboard", m_Choices.bVirtualKeyboard);
		pSetting->GetInteger($LOGIN, "LastInvisible", 0, &m_Choices.nLastInvisible);

		char	szBuffer[32];
		//----¼ÍÂ¼×îºóÒ»´ÎµÇÂ½ÕËºÅ----
		pSetting->EraseKey($LOGIN, $LAST_ACCOUNT);

		if (m_Choices.bRememberAccount)
		{
			GetAccountPassword(szBuffer, NULL);
			i = strlen(szBuffer);
			EDOneTimePad_Encipher(szBuffer, i);
			pSetting->WriteStruct($LOGIN, $LAST_ACCOUNT, szBuffer, sizeof(szBuffer));
			INT j = 0, k = 0;
			char szKeyName[32], szAccountName[32], szSaveBuffer[$MAX_ACCOUNT_LIST][32];
			szAccountName[0] = 0;
			memset(szSaveBuffer, 0, sizeof(szSaveBuffer));
			pSetting->GetStruct($ACCOUNT_LIST, $RECENT_FIRST_ACCOUNT, szAccountName, sizeof(szAccountName));
			if (strcmp(szAccountName, szBuffer) != 0)
			{
				for (j = 1; j < $MAX_ACCOUNT_LIST; j++)
				{
					sprintf(szKeyName, $RECENT_ACCOUNT, j);
					pSetting->GetStruct($ACCOUNT_LIST, szKeyName, szAccountName, sizeof(szAccountName));
					if (strcmp(szAccountName, szBuffer) == 0)
					{
						memset(szAccountName, 0, sizeof(szAccountName));
						pSetting->WriteStruct($ACCOUNT_LIST, szKeyName, szAccountName, sizeof(szAccountName));
					}
				}
				for (j = 0; j < $MAX_ACCOUNT_LIST; j++)
				{
					sprintf(szKeyName, $RECENT_ACCOUNT, j);
					pSetting->GetStruct($ACCOUNT_LIST, szKeyName, szSaveBuffer[j], sizeof(szSaveBuffer[j]));
				}
				pSetting->WriteStruct($ACCOUNT_LIST, $RECENT_FIRST_ACCOUNT, szBuffer, sizeof(szBuffer));
				for (j = 1; j < $MAX_ACCOUNT_LIST; j++)
				{
					sprintf(szKeyName, $RECENT_ACCOUNT, j);
					while (k < $MAX_ACCOUNT_LIST)
					{
						if (szSaveBuffer[k])
						{
							pSetting->WriteStruct($ACCOUNT_LIST, szKeyName, szSaveBuffer[k], sizeof(szSaveBuffer[k]));
							k++;
							break;
						}
						k++;
					}
				}
			}
			if (m_Choices.bRememberAll)
			{
				KSG_PASSWORD Password;
				GetAccountPassword(NULL, &Password);
				i = strlen(Password.szPassword);
				EDOneTimePad_Encipher(Password.szPassword, i);
				pSetting->WriteStruct($LOGIN, $LAST_PASSWORD, Password.szPassword, sizeof(Password.szPassword));
			}

			KIniFile* pPrivate = g_UiBase.GetPrivateSettingFile();
			if (pPrivate)
			{
				if (m_Choices.szProcessingRoleName[0])
				{
					i = strlen(m_Choices.szProcessingRoleName);
					memcpy(szBuffer, m_Choices.szProcessingRoleName, i);
					szBuffer[i] = 0;
					EDOneTimePad_Encipher(szBuffer, i);
					//pPrivate->WriteString("Main", "LastSelCharacter", szBuffer);
					pPrivate->WriteString("Main", "LastSelCharacter", m_Choices.szProcessingRoleName);
				}
				g_UiBase.ClosePrivateSettingFile(true);
			}
		}

		g_UiBase.CloseCommSettingFile(true);
	}
}


void KLogin::GetRegionServer(char* pszRegion, char* pszServer)
{
	strcpy(pszRegion, m_Choices.RegionName);
	strcpy(pszServer, m_Choices.AccountServer.Title);
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñÈ¡·þÎñÆ÷ÇøÓòµÄÁÐ±í
//--------------------------------------------------------------------------
KLoginServer* KLogin::GetServerRegionList(INT& nCount, INT& nAdviceChoice)
{
	KLoginServer* pServers = NULL;
	nCount = 0;
	nAdviceChoice = 0;

	KIniFile	File;
	INT			i;
	if (File.Load(SERVER_LIST_FILE))
	{
		INT		nReadCount = 0;
		char	szKey[32];
		File.GetInteger("List", "RegionCount", 0, &nReadCount);
		if (nReadCount > 0)
		{
			pServers = (KLoginServer*)malloc(sizeof(KLoginServer) * nReadCount);
			if (pServers)
			{
				for (i = 0; i < nReadCount; i++)
				{
					sprintf(szKey, "Region_%d", i);
					if (File.GetString("List", szKey, "", pServers[nCount].Title,
						sizeof(pServers[nCount].Title)) &&
						pServers[nCount].Title[0])
					{
						nCount++;
					}
				}
				if (nCount == 0)
				{
					free(pServers);
					pServers = NULL;
				}
			}
		}
	}
	if (nCount)
	{
		for (i = 0; i < nCount; i++)
		{
			if (strcmp(pServers[i].Title, m_Choices.RegionName) == 0)
			{
				nAdviceChoice = i;
				break;
			}
		}
		strcpy(m_Choices.RegionName, pServers[nAdviceChoice].Title);
	}
	return pServers;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºµÇÂ½·þÎñÆ÷ÁÐ±í»ñÈ¡
//--------------------------------------------------------------------------
KLoginServer* KLogin::GetServerList(INT nRegion, INT& nCount, INT& nAdviceChoice)
{
	KLoginServer* pServers = NULL;
	nCount = 0;
	nAdviceChoice = 0;

	KIniFile	File;
	INT			i;
	if (File.Load(SERVER_LIST_FILE))
	{
		INT		nReadCount = 0;
		char	szSection[32], szKey[32], szBuffer[32];

		File.GetInteger("List", "RegionCount", 0, &nReadCount);	//ÇøÓòµÄÊýÄ¿

		if (nRegion < 0 || nRegion >= nReadCount)
		{
			nRegion = 0;
		}
		sprintf(szSection, "Region_%d", nRegion);
		File.GetString("List", szSection, "", m_Choices.RegionName, sizeof(m_Choices.RegionName));
		File.GetInteger(szSection, "Count", 0, &nReadCount);	//¸ÃÇøÓò·þÎñÆ÷µÄÊýÄ¿
		if (nReadCount > 0)
		{
			pServers = (KLoginServer*)malloc(sizeof(KLoginServer) * nReadCount);
			if (pServers)
			{
				for (i = 0; i < nReadCount; i++)
				{
					sprintf(szKey, "%d_Address", i);
					if (!File.GetString(szSection, szKey, "", szBuffer, sizeof(szBuffer)) ||
						GetIpAddress(szBuffer, pServers[nCount].Address) == false)
					{
						continue;
					}
					sprintf(szKey, "%d_Title", i);
					if (File.GetString(szSection, szKey, "", pServers[nCount].Title,
						sizeof(pServers[nCount].Title)) &&
						pServers[nCount].Title[0])
					{
						nCount++;
					}
				}
				if (nCount == 0)
				{
					free(pServers);
					pServers = NULL;
				}
			}
		}
	}

	if (nCount)
	{
		for (i = 0; i < nCount; i++)
		{
			if (strcmp(pServers[i].Title, m_Choices.AccountServer.Title) == 0)
			{
				nAdviceChoice = i;
				break;
			}
		}
		strcpy(m_Choices.AccountServer.Title, pServers[nAdviceChoice].Title);
		m_Choices.AccountServer.Address[0] = pServers[nAdviceChoice].Address[0];
		m_Choices.AccountServer.Address[1] = pServers[nAdviceChoice].Address[1];
		m_Choices.AccountServer.Address[2] = pServers[nAdviceChoice].Address[2];
		m_Choices.AccountServer.Address[3] = pServers[nAdviceChoice].Address[3];
	}
	return pServers;
}

INT KLogin::SetAccountServer(const KLoginServer& rcSelectServer)
{
	m_Choices.AccountServer = rcSelectServer;
	return true;
}


extern void RandMemSet(INT nSize, unsigned char* pbyBuffer);
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉêÇëÕËºÅ
//--------------------------------------------------------------------------
INT KLogin::Request(const char* pszAccount, const KSG_PASSWORD* pcPassword, INT nAction)
{
	BYTE		Buff[sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE];

	RandMemSet(sizeof(Buff), (BYTE*)Buff);		// random memory for make a cipher

	if (pszAccount && pcPassword)
	{
		(*(PROTOCOL_MSG_TYPE*)Buff) = c2s_login;
		KLoginAccountInfo* pInfo = (KLoginAccountInfo*)&Buff[PROTOCOL_MSG_SIZE];
		pInfo->Size = sizeof(KLoginAccountInfo);
		pInfo->Param = nAction | LOGIN_R_REQUEST;
		strncpy(pInfo->Account, pszAccount, sizeof(pInfo->Account));
		pInfo->Account[sizeof(pInfo->Account) - 1] = '\0';
		pInfo->Password = *pcPassword;
		pInfo->nCheckSum = CalculateChecksum("Game.exe");
		pInfo->nCheckSumCore = CalculateChecksum("CoreClient.dll");

#ifdef USE_KPROTOCOL_VERSION
		// Add by Freeway Chen in 2003.7.1
		pInfo->ProtocolVersion = KPROTOCOL_VERSION;    //  ´«ÊäÐ­Òé°æ±¾£¬ÒÔ±ãÐ£ÑéÊÇ·ñ¼æÈÝ
#endif

		if (g_NetConnectAgent.SendMsg(Buff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE))
		{
			g_NetConnectAgent.UpdateClientRequestTime(false);
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÁ¬½ÓÓÎÏ··þÎñ
//--------------------------------------------------------------------------
INT KLogin::ConnectAccountServer(const unsigned char* pIpAddress)
{
	KIniFile	IniFile;
	if (pIpAddress && IniFile.Load("\\Config.ini"))
	{
		INT nPort;
		IniFile.GetInteger("Server", "GameServPort", 8888, &nPort);
		return g_NetConnectAgent.ClientConnectByNumericIp(pIpAddress, nPort);
	}
	return false;
}

void KLogin::RegistNetAgent()
{
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_login, this);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_roleserver_getrolelist_result, this);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_notifyplayerlogin, this);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_rolenewdelresponse, this);
}

void KLogin::UnRegistNetAgent()
{
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_login, NULL);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_roleserver_getrolelist_result, NULL);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_notifyplayerlogin, NULL);
	g_NetConnectAgent.RegisterMsgTargetObject(s2c_rolenewdelresponse, NULL);
}

void KLogin::SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword)
{
	INT i;
	if (pszAccount)
	{
		strncpy(m_Choices.Account, pszAccount, sizeof(m_Choices.Account));
		for (i = 0; i < 32; i++)
			m_Choices.Account[i] = ~m_Choices.Account[i];
	}
	if (pcPassword)
	{
		m_Choices.Password = *pcPassword;
		for (i = 0; i < KSG_PASSWORD_MAX_SIZE; i++)
			m_Choices.Password.szPassword[i] = ~m_Choices.Password.szPassword[i];
	}
}

void KLogin::GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword)
{
	INT i;
	if (pszAccount)
	{
		memcpy(pszAccount, m_Choices.Account, sizeof(m_Choices.Account));
		for (i = 0; i < 32; i++)
			pszAccount[i] = ~pszAccount[i];
	}
	if (pPassword)
	{
		*pPassword = m_Choices.Password;
		for (i = 0; i < KSG_PASSWORD_MAX_SIZE; i++)
			pPassword->szPassword[i] = ~pPassword->szPassword[i];
	}
}

INT KLogin::GetLoginLastInvisible()
{
	return m_Choices.nLastInvisible;
}

unsigned long KLogin::CalculateChecksum(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return 0;
	}

	unsigned long checksum = 0;
	char buffer[1024];
	while (file.read(buffer, sizeof(buffer))) {
		for (size_t i = 0; i < file.gcount(); ++i) {
			checksum += static_cast<unsigned char>(buffer[i]);
		}
	}

	if (!file.eof()) {
		std::cerr << "Error reading file: " << filename << std::endl;
		return 0;
	}

	file.close();
	return checksum;
}