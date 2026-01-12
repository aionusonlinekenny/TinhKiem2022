// KTongSet.cpp: implementation of the CTongSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Global.h"
#include "TongDB.h"
#include "KTongSet.h"
#include "S3Relay.h"
#include <time.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTongSet::CTongSet()
{
	m_pcTong = NULL;
	m_nTongPointSize = 0;
	Init();
}

CTongSet::~CTongSet()
{
	DeleteAll();
}

void	CTongSet::Init()
{
	DeleteAll();

	m_pcTong = (CTongControl**)new LPVOID[defTONG_SET_INIT_POINT_NUM];
	m_nTongPointSize = defTONG_SET_INIT_POINT_NUM;
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		m_pcTong[i] = NULL;
	}
}

void	CTongSet::DeleteAll()
{
	if (m_pcTong)
	{
		for (INT i = 0; i < m_nTongPointSize; i++)
		{
			if (m_pcTong[i])
			{
				delete m_pcTong[i];
				m_pcTong[i] = NULL;
			}
		}
		delete[]m_pcTong;
		m_pcTong = NULL;
	}
	m_nTongPointSize = 0;
}

INT		CTongSet::Create(INT nCamp, BYTE btLevel, char* lpszPlayerName, char* lpszTongName, INT nSex, INT nJoinTm, INT nSaveEff, INT nTongJiyuParam, char* szTongJiyuNotify)
{
	// °ï»áÄ£¿é³ö´í
	if (!m_pcTong || m_nTongPointSize <= 0)
		return enumTONG_CREATE_ERROR_ID13;
	// Ãû×Ö×Ö·û´®³ö´í
	if (!lpszPlayerName || !lpszTongName)
		return enumTONG_CREATE_ERROR_ID14;
	// Ãû×Ö×Ö·û´®¹ý³¤
	if (strlen(lpszTongName) >= defTONG_NAME_LENGTH_32 || strlen(lpszPlayerName) >= defTONG_NAME_LENGTH_32)
		return enumTONG_CREATE_ERROR_ID14;

	INT		i, nPos;
	DWORD	dwTongNameID, dwPlayerNameID;

	dwTongNameID = g_String2Id(lpszTongName);
	dwPlayerNameID = g_String2Id(lpszPlayerName);

	rTRACE("<<<<<<<<<<<<<<:Nguoi choi(%s) da tao bang hoi(%s)>>>>>>>>>>>>>>", lpszPlayerName, lpszTongName);

	// Ãû³Æ¼ì²â£¬ÊÇ·ñÓÐÍ¬ÃûµÄ°ïÖ÷»òÕßÍ¬ÃûµÄ°ï»á
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && (m_pcTong[i]->m_dwNameID == dwTongNameID || m_pcTong[i]->m_dwMasterID == dwPlayerNameID))
			break;
	}
	// °ï»áÍ¬Ãû´íÎó
	if (i < m_nTongPointSize)
		return enumTONG_CREATE_ERROR_ID15;

	// ²éÕÒ¿ÕÎ»
	nPos = -1;
	for (i = 0; i < m_nTongPointSize; i++)
	{
		if (!m_pcTong[i])
		{
			nPos = i;
			break;
		}
	}
	// ÏÖÓÐÖ¸Õë¿Õ¼äÒÑÂú£¬·ÖÅäÐÂµÄ¸ü´óµÄÖ¸Õë¿Õ¼ä
	if (nPos < 0)
	{
		// ÁÙÊ±´æ´¢¾ÉµÄÖ¸Õë
		CTongControl** pTemp;
		pTemp = (CTongControl**)new LPVOID[m_nTongPointSize];
		for (i = 0; i < m_nTongPointSize; i++)
			pTemp[i] = m_pcTong[i];

		// ·ÖÅäÐÂµÄ¸ü´óµÄÖ¸Õë¿Õ¼ä£¬´óÐ¡ÊÇÔ­À´µÄÁ½±¶
		delete[]m_pcTong;
		m_pcTong = NULL;
		m_pcTong = (CTongControl**)new LPVOID[m_nTongPointSize * 2];
		for (i = 0; i < m_nTongPointSize; i++)
			m_pcTong[i] = pTemp[i];
		delete[]pTemp;
		m_nTongPointSize *= 2;
		for (i = m_nTongPointSize / 2; i < m_nTongPointSize; i++)
			m_pcTong[i] = NULL;
		nPos = m_nTongPointSize / 2;
	}

	// ²úÉúÒ»¸öÐÂµÄ°ï»á
	m_pcTong[nPos] = new CTongControl(nCamp, btLevel, lpszPlayerName, lpszTongName, nSex, nJoinTm, nSaveEff, nTongJiyuParam, szTongJiyuNotify);
	// ²úÉúÊ§°Ü
	if (m_pcTong[nPos]->m_dwNameID == 0)
	{
		delete m_pcTong[nPos];
		m_pcTong[nPos] = NULL;
		return enumTONG_CREATE_ERROR_ID13;
	}
	// ´æÅÌ£¬Êý¾Ý±£´æÖÁÊý¾Ý¿â
	TMemberStruct	sMember;
	sMember.MemberClass = enumTONG_FIGURE_MASTER;
	sMember.nTitleIndex = 0;
	strcpy(sMember.szTong, m_pcTong[nPos]->m_szName);
	strcpy(sMember.szName, m_pcTong[nPos]->m_szMasterName);
	sMember.nSex = nSex;
	sMember.nJoinTm = nJoinTm;
	sMember.nSaveEff = nSaveEff;

	try
	{
		g_cTongDB.ChangeTong(*m_pcTong[nPos]);
		g_cTongDB.ChangeMember(sMember);
	}
	catch (...)
	{
		char	szMsg[96];
		sprintf(szMsg, "\\O%u", m_pcTong[i]->m_dwNameID);

		DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
		if (channid != -1)
		{
			sprintf(szMsg, "%s ®· t¹o bang héi kh«ng thµnh c«ng", m_pcTong[nPos]->m_szName);
			g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(MESSAGE_TONG_ANNOUCE_HEAD), std::string(szMsg));
		}
		rTRACE("<<<<<<<<<<<<<<:Nguoi choi (%s) tao bang hoi (%s) that bai! >>>>>>>>>>>>>>", lpszPlayerName, lpszTongName);
	}
	rTRACE("<<<<<<<<<<<<<<:Nguoi choi (%s) tao bang hoi (%s) thanh cong! >>>>>>>>>>>>>>", lpszPlayerName, lpszTongName);

	return 0;
}

//----------------------------------------------------------------------
//	¹¦ÄÜ£ºÌí¼ÓÒ»¸ö°ï»á³ÉÔ±£¬if return == 0 ³É¹¦ else return error id
//----------------------------------------------------------------------
INT		CTongSet::AddMember(char* lpszPlayerName, char* lpszTongName, INT nSex, INT nJoinTm, INT nSaveEff)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return -1;
	if (!lpszPlayerName || !lpszPlayerName[0] || !lpszTongName || !lpszTongName[0])
		return -1;
	if (strlen(lpszTongName) >= defTONG_NAME_LENGTH_32 || strlen(lpszPlayerName) >= defTONG_NAME_LENGTH_32)
		return -1;

	INT		i;
	DWORD	dwTongNameID;

	dwTongNameID = g_String2Id(lpszTongName);

	// Ñ°ÕÒ°ï»á
	for (i = 0; i < m_nTongPointSize; i++)
	{
		// ÕÒµ½ÁË
		if (m_pcTong[i] && (m_pcTong[i]->m_dwNameID == dwTongNameID))
		{
			if (!m_pcTong[i]->AddMember(lpszPlayerName, nSex, nJoinTm, nSaveEff))
				return -1;
			else
			{
				// ´æÅÌ£¬Êý¾Ý±£´æÖÁÊý¾Ý¿â
				TMemberStruct	sMember;
				sMember.MemberClass = enumTONG_FIGURE_MEMBER;
				sMember.nTitleIndex = 0;
				strcpy(sMember.szTong, m_pcTong[i]->m_szName);
				strcpy(sMember.szName, lpszPlayerName);
				sMember.nSex = nSex;
				sMember.nJoinTm = nJoinTm;
				sMember.nSaveEff = nSaveEff;
				g_cTongDB.ChangeMember(sMember);

				// ¸ø°ï»áÆµµÀ·¢ÏûÏ¢
				char	szMsg[96];
				sprintf(szMsg, "\\O%u", m_pcTong[i]->m_dwNameID);

				DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
				if (channid != -1)
				{
					sprintf(szMsg, "%s ®· gia nhËp bæn bang ", lpszPlayerName);
					g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(MESSAGE_TONG_ANNOUCE_HEAD), std::string(szMsg));
				}

				return i;
			}
		}
	}

	return -1;
}

//----------------------------------------------------------------------
//	¹¦ÄÜ£º»ñµÃ°ï»áÕóÓª
//----------------------------------------------------------------------
INT		CTongSet::GetTongCamp(INT nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return 0;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return 0;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_nCamp;

	return 0;
}

INT		CTongSet::GetTongMemberNum(INT nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return 0;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return 0;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_nMemberNum;

	return 0;
}

DWORD		CTongSet::GetTongMoney(INT nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return 0;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return 0;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_dwMoney;

	return 0;
}

INT		CTongSet::GetTongLevel(INT nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return 0;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return 0;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_btLevel;

	return 0;
}

DWORD		CTongSet::GetTongEff(INT nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return 0;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return 0;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_dwTotalEff;

	return 0;
}

BOOL		CTongSet::GetTongRecruit(INT nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return FALSE;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return FALSE;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_bRecruit;

	return FALSE;
}

INT		CTongSet::GetTongParam(INT nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return 0;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return 0;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_nTongParam;

	return 0;
}

INT		CTongSet::GetTongJiyuParam(INT nTongIdx)
{
	if (!m_pcTong || m_nTongPointSize <= 0)
		return 0;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return 0;

	if (m_pcTong[nTongIdx])
		return m_pcTong[nTongIdx]->m_nTongJiyuParam;

	return 0;
}

BOOL	CTongSet::GetMasterName(INT nTongIdx, char* lpszName)
{
	if (!lpszName)
		return FALSE;
	if (!m_pcTong || m_nTongPointSize <= 0)
		return FALSE;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return FALSE;
	if (!m_pcTong[nTongIdx])
		return FALSE;
	strcpy(lpszName, m_pcTong[nTongIdx]->m_szMasterName);
	return TRUE;
}

BOOL	CTongSet::GetMemberAgname(INT nTongIdx, char* lpszAgname, INT nSex)
{
	if (!lpszAgname)
		return FALSE;
	if (!m_pcTong || m_nTongPointSize <= 0)
		return FALSE;
	if (nTongIdx < 0 || nTongIdx >= m_nTongPointSize)
		return FALSE;
	if (!m_pcTong[nTongIdx])
		return FALSE;
	if (nSex > 0)
		strcpy(lpszAgname, m_pcTong[nTongIdx]->m_szMaleAgname);
	else
		strcpy(lpszAgname, m_pcTong[nTongIdx]->m_szFemaleAgname);
	if (!lpszAgname[0])
	{
		if (m_pcTong[nTongIdx]->m_szNormalAgname[0])
			strcpy(lpszAgname, m_pcTong[nTongIdx]->m_szNormalAgname);
		else
			strcpy(lpszAgname, defTONG_MEMBER_AGNAME);
	}
	return TRUE;
}

BOOL	CTongSet::GetTongHeadInfo(DWORD dwTongNameID, STONG_HEAD_INFO_SYNC* pInfo)
{
	if (!m_pcTong || m_nTongPointSize <= 0 || dwTongNameID == 0)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == dwTongNameID)
		{
			return m_pcTong[i]->GetTongHeadInfo(pInfo);
		}
	}

	return FALSE;
}

BOOL	CTongSet::GetTongManagerInfo(STONG_GET_MANAGER_INFO_COMMAND* pApply, STONG_MANAGER_INFO_SYNC* pInfo)
{
	if (!pApply || !pInfo)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == (DWORD)pApply->m_nParam1)
		{
			return m_pcTong[i]->GetTongManagerInfo(pApply, pInfo);
		}
	}

	return FALSE;
}

BOOL	CTongSet::GetTongMemberInfo(STONG_GET_MEMBER_INFO_COMMAND* pApply, STONG_MEMBER_INFO_SYNC* pInfo)
{
	if (!pApply || !pInfo)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == (DWORD)pApply->m_nParam1)
		{
			return m_pcTong[i]->GetTongMemberInfo(pApply, pInfo);
		}
	}

	return FALSE;
}

BOOL	CTongSet::Instate(STONG_INSTATE_COMMAND* pInstate, STONG_INSTATE_SYNC* pSync)
{
	if (!pInstate || !pSync)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pInstate->m_dwTongNameID)
		{
			return m_pcTong[i]->Instate(pInstate, pSync);
		}
	}

	return FALSE;
}

BOOL	CTongSet::Kick(STONG_KICK_COMMAND* pKick, STONG_KICK_SYNC* pSync)
{
	if (!pKick || !pSync)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pKick->m_dwTongNameID)
		{
			return m_pcTong[i]->Kick(pKick, pSync);
		}
	}

	return FALSE;
}

BOOL	CTongSet::Leave(STONG_LEAVE_COMMAND* pLeave, STONG_LEAVE_SYNC* pSync)
{
	if (!pLeave || !pSync)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pLeave->m_dwTongNameID)
		{
			return m_pcTong[i]->Leave(pLeave, pSync);
		}
	}

	return FALSE;
}

BOOL	CTongSet::AcceptMaster(STONG_ACCEPT_MASTER_COMMAND* pAccept)
{
	if (!pAccept)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pAccept->m_dwTongNameID)
		{
			return m_pcTong[i]->AcceptMaster(pAccept);
		}
	}

	return FALSE;
}

BOOL	CTongSet::InitFromDB()
{
	INT		nTongNum;

	nTongNum = g_cTongDB.GetTongCount();

	rTRACE("------------ nTongNum %d 个 -------------", nTongNum);

	if (nTongNum < 0)
		return FALSE;
	if (nTongNum == 0)
		return TRUE;
	CTongControl** m_tmpTong;

	m_tmpTong = (CTongControl**)new LPVOID[nTongNum];
	for (INT i = 0; i < nTongNum; i++)
	{
		m_tmpTong[i] = NULL;
	}


	INT i, j;
	TTongList* pList = new TTongList[nTongNum];
	memset(pList, 0, sizeof(TTongList) * nTongNum);

	INT nGetNum = g_cTongDB.GetTongList(pList, nTongNum);
	rTRACE("------------ nGetNum %d 个 -------------", nGetNum);

	if (nGetNum <= 0)
		return TRUE;

	for (i = 0; i < nGetNum && nGetNum <= nTongNum; i++)
	{
		m_tmpTong[i] = new CTongControl(pList[i]);
		if (!m_tmpTong[i]->m_szName[0])
		{
			delete m_tmpTong[i];
			m_tmpTong[i] = NULL;
			continue;
		}

		rTRACE("------------ No: %d Name(%s) MemCount:%d Master:%s -------------", i, m_tmpTong[i]->m_szName, pList[i].MemberCount, pList[i].m_szMasterName);
	}
	Init();
	g_cTongDB.Close();
	g_cTongDB.OpenNew();
	for (i = 0; i < nGetNum && nGetNum <= nTongNum; i++)
	{
		if (m_tmpTong[i] && m_tmpTong[i]->m_szName[0])
		{
			Create(m_tmpTong[i]->m_nCamp, m_tmpTong[i]->m_btLevel, m_tmpTong[i]->m_szMasterName, m_tmpTong[i]->m_szName, m_tmpTong[i]->m_psMember[0].m_nSex, m_tmpTong[i]->m_psMember[0].m_nMemberJoinTm, m_tmpTong[i]->m_psMember[0].m_nMemberSaveEff, m_tmpTong[i]->m_nTongJiyuParam, m_tmpTong[i]->m_szTongJiyuNotify);
			for (INT j = 0; j < m_tmpTong[i]->m_nMemberPointSize; j++)
			{
				AddMember(m_tmpTong[i]->m_psMember[j].m_szName, m_tmpTong[i]->m_szName, m_tmpTong[i]->m_psMember[j].m_nSex, m_tmpTong[i]->m_psMember[j].m_nMemberJoinTm, m_tmpTong[i]->m_psMember[j].m_nMemberSaveEff);
			}
		}
	}
	for (i = 0; i < nGetNum && nGetNum <= nTongNum; i++)
	{
		m_pcTong[i] = new CTongControl(pList[i]);
		if (!m_pcTong[i]->m_szName[0])
		{
			delete m_pcTong[i];
			m_pcTong[i] = NULL;
		}
	}
	for (i = 0; i < nGetNum && nGetNum <= nTongNum; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_szName[0])
		{
			g_cTongDB.SearchTong(m_pcTong[i]->m_szName, *(m_pcTong[i]));
			for (INT j = 0; j < m_tmpTong[i]->m_nManagerNum; j++)
			{
				m_pcTong[i]->DBInstate(m_tmpTong[i]->m_szManagerName[j], 1);
			}

			for (j = 0; j < m_tmpTong[i]->m_nDirectorNum; j++)
			{
				m_pcTong[i]->DBInstate(m_tmpTong[i]->m_szDirectorName[j], 2);
			}
		}
	}
	if (m_tmpTong)
	{
		delete[] m_tmpTong;
	}
	if (pList)
	{
		delete[] pList;
	}

	return TRUE;
}

BOOL	CTongSet::GetLoginData(STONG_GET_LOGIN_DATA_COMMAND* pLogin, STONG_LOGIN_DATA_SYNC* pSync)
{
	if (!pLogin || !pSync)
		return FALSE;

	memset(pSync, 0, sizeof(STONG_LOGIN_DATA_SYNC));
	pSync->ProtocolFamily = pf_tong;
	pSync->ProtocolID = enumS2C_TONG_LOGIN_DATA;
	pSync->m_btFlag = 0;
	pSync->m_dwParam = pLogin->m_dwParam;

	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pLogin->m_dwTongNameID)
		{
			return m_pcTong[i]->GetLoginData(pLogin, pSync);
		}
	}

	return TRUE;
}

BOOL	CTongSet::SearchOne(DWORD dwTongNameID, char* lpszName, STONG_ONE_LEADER_INFO* pInfo)
{
	if (!m_pcTong)
		return FALSE;
	if (dwTongNameID == 0 || !lpszName || !lpszName[0] || !pInfo)
		return FALSE;

	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == dwTongNameID)
		{
			return m_pcTong[i]->SearchOne(lpszName, pInfo);
		}
	}

	return FALSE;
}

BOOL CTongSet::ChangeMoney(STONG_MONEY_COMMAND* pMoney, STONG_MONEY_SYNC* Sync)
{
	if (!pMoney)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pMoney->m_dwTongNameID)
		{
			if (m_pcTong[i]->ChangeMoney(pMoney, Sync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL	CTongSet::AcceptAgname(STONG_ACCEPT_AGNAME_COMMAND* pAccept)
{
	if (!pAccept)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pAccept->m_dwTongNameID)
		{
			BOOL bRet = m_pcTong[i]->DBChangeAgname(pAccept);
			if (bRet)
			{
				g_cTongDB.ChangeTong(*m_pcTong[i]);
				return TRUE;
			}
			return FALSE;
		}
	}

	return FALSE;
}

BOOL	CTongSet::AcceptSexAgname(STONG_ACCEPT_SEX_AGNAME_COMMAND* pAccept)
{
	if (!pAccept)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	// Ñ°ÕÒ°ï»á
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pAccept->m_dwTongNameID)
		{
			BOOL bRet = m_pcTong[i]->DBChangeSexAgname(pAccept);
			if (bRet)
			{
				g_cTongDB.ChangeTong(*m_pcTong[i]);
				return TRUE;
			}
			return FALSE;
		}
	}

	return FALSE;
}

BOOL	CTongSet::ChangeCamp(STONG_CHANGE_CAMP_COMMAND* pData, STONG_BE_CHANGED_CAMP_SYNC* pSync)
{
	if (!pData)
		return FALSE;

	if (!m_pcTong)
		return FALSE;

	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pData->m_dwTongNameID)
		{
			if (m_pcTong[i]->DBChangeCamp(pData, pSync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL	CTongSet::ChangeTongRecruit(STONG_CHANGE_TONG_INFO_COMMAND* pData, STONG_CHANGE_TONG_INFO_SYNC* pSync)
{
	if (!pData)
		return FALSE;

	if (!m_pcTong)
		return FALSE;
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pData->m_dwTongNameID)
		{
			if (m_pcTong[i]->DBChangeTongRecruit(pData, pSync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL	CTongSet::ChangeTongParam(STONG_CHANGE_TONG_INFO_COMMAND* pData, STONG_CHANGE_TONG_INFO_SYNC* pSync)
{
	if (!pData)
		return FALSE;

	if (!m_pcTong)
		return FALSE;
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pData->m_dwTongNameID)
		{
			if (m_pcTong[i]->DBChangeTongParam(pData, pSync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL	CTongSet::ChangeTongLevel(STONG_CHANGE_TONG_INFO_COMMAND* pData, STONG_CHANGE_TONG_INFO_SYNC* pSync)
{
	if (!pData)
		return FALSE;

	if (!m_pcTong)
		return FALSE;
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pData->m_dwTongNameID)
		{
			if (m_pcTong[i]->DBChangeTongLevel(pData, pSync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL	CTongSet::ChangeTongMoney(STONG_CHANGE_TONG_INFO_COMMAND* pData, STONG_CHANGE_TONG_INFO_SYNC* pSync)
{
	if (!pData)
		return FALSE;

	if (!m_pcTong)
		return FALSE;
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pData->m_dwTongNameID)
		{
			if (m_pcTong[i]->DBChangeTongMoney(pData, pSync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL	CTongSet::ChangeTongEff(STONG_CHANGE_TONG_INFO_COMMAND* pData, STONG_CHANGE_TONG_INFO_SYNC* pSync)
{
	if (!pData)
		return FALSE;

	if (!m_pcTong)
		return FALSE;
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pData->m_dwTongNameID)
		{
			if (m_pcTong[i]->DBChangeTongEff(pData, pSync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL	CTongSet::ChangeJiyu(STONG_CHANGE_TONG_INFO_COMMAND* pData, STONG_CHANGE_TONG_INFO_SYNC* pSync)
{
	if (!pData)
		return FALSE;

	if (!m_pcTong)
		return FALSE;
	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pData->m_dwTongNameID)
		{
			if (m_pcTong[i]->DBChangeJiyu(pData, pSync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL CTongSet::ChangeMemberEff(STONG_CHANGE_TONG_MEMBEREFF_COMMAND* pInfo, STONG_CHANGE_TONG_MEMBEREFF_SYNC* sSync)
{
	if (!pInfo)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pInfo->m_dwTongNameID)
		{
			if (m_pcTong[i]->DBChangeMemberEff(pInfo, sSync))
			{
				return g_cTongDB.ChangeTong(*m_pcTong[i]);
			}
		}
	}

	return FALSE;
}

BOOL CTongSet::MessageToTong(STONG_MESSAGE_INFO_COMMAND* pData)
{
	if (!pData)
		return FALSE;
	if (!m_pcTong)
		return FALSE;

	for (INT i = 0; i < m_nTongPointSize; i++)
	{
		if (m_pcTong[i] && m_pcTong[i]->m_dwNameID == pData->dwParam)
		{
			return m_pcTong[i]->MessageToTong(pData);
		}
	}

	return FALSE;
}

BOOL CTongSet::MessageToFaction(STONG_MESSAGE_INFO_COMMAND* pData)
{
	if (!pData)
		return FALSE;
	if (pData->dwParam >= 0)
	{
		char	szMsg[96];
		sprintf(szMsg, "\\F%u", pData->dwParam);

		DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
		if (channid != -1)
		{
			g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(pData->szName), std::string(pData->szMsg));
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CTongSet::MessageToChatRoom(STONG_MESSAGE_INFO_COMMAND* pData)
{
	if (!pData)
		return FALSE;
	if (pData->dwParam >= 0)
	{
		char	szMsg[96];
		sprintf(szMsg, "\\C%u", pData->dwParam);

		DWORD channid = g_ChannelMgr.GetChannelID(szMsg, 0);
		if (channid != -1)
		{
			g_ChannelMgr.SayOnChannel(channid, TRUE, std::string(), std::string(pData->szName), std::string(pData->szMsg));
		}
		return TRUE;
	}
	return FALSE;
}