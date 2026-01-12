// KTongControl.h: interface for the CTongControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_)
#define AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct STONG_MEMBER
{
	DWORD	m_dwNameID;
	char	m_szName[defTONG_NAME_LENGTH_32];
	INT		m_nSex;
	INT 	m_nMemberJoinTm;
	INT 	m_nMemberSaveEff;
};

typedef struct
{
	char		szName[defTONG_NAME_LENGTH_32];			// 帮会名
	INT			MemberCount;						//一般帮众数
	char		m_szMasterName[defTONG_NAME_LENGTH_32];
	char		m_szMasterTitle[defTONG_NAME_LENGTH_32];	// 帮主称号
	char		m_szDirectorTitle[defTONG_MAX_DIRECTOR][defTONG_NAME_LENGTH_32];// 各长老称号
	char		m_szManagerTitle[defTONG_MAX_MANAGER][defTONG_NAME_LENGTH_32];	// 各队长称号
	char		m_szNormalTitle[defTONG_NAME_LENGTH_32];	// 帮会帮众称号
}TTongList;	//用作帮会列表

class CTongControl
{
	friend class CTongSet;
	friend class CTongDB;
public:
	// 用于创建帮会
	CTongControl(INT nCamp, BYTE btLevel, char *lpszPlayerName, char *lpszTongName, INT nMasterSex, INT nMasterJoinTm, INT nMasterSaveEff, INT nTongJiyuParam, char *szTongJiyuNotify);
	// 用于数据库载入帮会数据
	CTongControl(TTongList sList);
	virtual ~CTongControl();

private:

	INT			m_nCamp;								// 帮会阵营
	DWORD		m_dwMoney;								// 帮会资金
	INT			m_nCredit;								// 帮会声望
	BYTE		m_btLevel;								// 帮会等级
	INT			m_nDirectorNum;							// 长老人数
	INT			m_nManagerNum;							// 队长人数
	INT			m_nMemberNum;							// 帮众人数
	DWORD		m_dwTotalEff;
	INT			m_nTongJiyuParam;
	char 		m_szTongJiyuNotify[defTONG_NOTIFY_LENGTH];
	INT 		m_nSaveEff;
	BOOL		m_bRecruit;
	INT 		m_nTongParam;

	DWORD		m_dwNameID;								// 帮会名ID
	char		m_szName[defTONG_NAME_LENGTH_32];			// 帮会名

	char		m_szMasterAgname[defTONG_NAME_LENGTH_32];	// 帮主称号
	char		m_szDirectorAgname[defTONG_MAX_DIRECTOR][defTONG_NAME_LENGTH_32];// 各长老称号
	char		m_szManagerAgname[defTONG_MAX_MANAGER][defTONG_NAME_LENGTH_32];	// 各队长称号
	char		m_szNormalAgname[defTONG_NAME_LENGTH_32];	// 帮会帮众称号

	DWORD		m_dwMasterID;							// 帮主名ID
	char		m_szMasterName[defTONG_NAME_LENGTH_32];		// 帮主名
	INT			m_nMasterSex;
	INT			m_nMasterJoinTm;
	INT 		m_nMasterSaveEff;

	DWORD		m_dwDirectorID[defTONG_MAX_DIRECTOR];	// 各长老名ID
	char		m_szDirectorName[defTONG_MAX_DIRECTOR][defTONG_NAME_LENGTH_32];	// 各长老名
	INT			m_nDirectorSex[defTONG_MAX_DIRECTOR];
	INT 		m_nDirectorJoinTm[defTONG_MAX_DIRECTOR];
	INT 		m_nDirectorSaveEff[defTONG_MAX_DIRECTOR];

	DWORD		m_dwManagerID[defTONG_MAX_MANAGER];		// 各队长名ID
	char		m_szManagerName[defTONG_MAX_MANAGER][defTONG_NAME_LENGTH_32];	// 各队长名
	INT			m_nManagerSex[defTONG_MAX_DIRECTOR];
	INT 		m_nManagerJoinTm[defTONG_MAX_MANAGER];
	INT 		m_nManagerSaveEff[defTONG_MAX_MANAGER];

	STONG_MEMBER	*m_psMember;
	INT				m_nMemberPointSize;
	char		m_szMaleAgname[defTONG_NAME_LENGTH_32];
	char		m_szFemaleAgname[defTONG_NAME_LENGTH_32];

public:
	BOOL		SearchOne(char *lpszName, STONG_ONE_LEADER_INFO *pInfo);

	BOOL		AddMember(char *lpszPlayerName, INT nSex = 0, INT nJoinTm = 0, INT nSaveEff = 0);		// 添加一个成员

	BOOL		GetTongHeadInfo(STONG_HEAD_INFO_SYNC *pInfo);	// 帮会信息

	BOOL		GetTongManagerInfo(STONG_GET_MANAGER_INFO_COMMAND *pApply, STONG_MANAGER_INFO_SYNC *pInfo);

	BOOL		GetTongMemberInfo(STONG_GET_MEMBER_INFO_COMMAND *pApply, STONG_MEMBER_INFO_SYNC *pInfo);

	BOOL		Instate(STONG_INSTATE_COMMAND *pInstate, STONG_INSTATE_SYNC *pSync);

	BOOL		Kick(STONG_KICK_COMMAND *pKick, STONG_KICK_SYNC *pSync);

	BOOL		Leave(STONG_LEAVE_COMMAND *pLeave, STONG_LEAVE_SYNC *pSync);

	BOOL		AcceptMaster(STONG_ACCEPT_MASTER_COMMAND *pAccept);

	BOOL		GetLoginData(STONG_GET_LOGIN_DATA_COMMAND *pLogin, STONG_LOGIN_DATA_SYNC *pSync);
	
	BOOL		DBInstate(char *lpszPlayerName, BYTE nSite);

	BOOL		ChangeMoney(STONG_MONEY_COMMAND* pMoney,STONG_MONEY_SYNC *Sync);
	BOOL		DBChangeAgname(STONG_ACCEPT_AGNAME_COMMAND *pAccept);
	BOOL		DBChangeTongLevel(STONG_CHANGE_TONG_INFO_COMMAND *pInfo, STONG_CHANGE_TONG_INFO_SYNC *sInfo);
	BOOL		DBChangeTongMoney(STONG_CHANGE_TONG_INFO_COMMAND *pInfo, STONG_CHANGE_TONG_INFO_SYNC *sInfo);
	BOOL		DBChangeTongEff(STONG_CHANGE_TONG_INFO_COMMAND *pInfo, STONG_CHANGE_TONG_INFO_SYNC *sInfo);
	BOOL		DBChangeTongRecruit(STONG_CHANGE_TONG_INFO_COMMAND *pInfo, STONG_CHANGE_TONG_INFO_SYNC *sInfo);
	BOOL		DBChangeTongParam(STONG_CHANGE_TONG_INFO_COMMAND *pInfo, STONG_CHANGE_TONG_INFO_SYNC *sInfo);
	BOOL		DBChangeJiyu(STONG_CHANGE_TONG_INFO_COMMAND *pInfo, STONG_CHANGE_TONG_INFO_SYNC *sInfo);
	BOOL		DBChangeMemberEff(STONG_CHANGE_TONG_MEMBEREFF_COMMAND *pInfo, STONG_CHANGE_TONG_MEMBEREFF_SYNC *sInfo);

	BOOL		DBChangeMoney(DWORD dwMoney) { m_dwMoney = dwMoney;};
	
	BOOL		GetMasterAgname(char *lpszAgname);
	BOOL		GetDirectorAgname(char *lpszAgname, INT nPos);
	BOOL		GetManagerAgname(char *lpszAgname, INT nPos);
	BOOL		GetMemberAgname(char *lpszAgname, INT nSex);	
	BOOL		DBChangeSexAgname(STONG_ACCEPT_SEX_AGNAME_COMMAND *pAccept);
	BOOL		DBChangeCamp(STONG_CHANGE_CAMP_COMMAND *pInfo, STONG_BE_CHANGED_CAMP_SYNC *sInfo);
	BOOL		MessageToTong(STONG_MESSAGE_INFO_COMMAND *pInfo);
};

#endif // !defined(AFX_KTONGCONTROL_H__62D04F9A_67CD_419B_B475_BF0F8727A91E__INCLUDED_)
