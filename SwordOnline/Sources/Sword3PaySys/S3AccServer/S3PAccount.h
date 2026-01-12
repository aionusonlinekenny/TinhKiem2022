//-----------------------------------------//
//                                         //
//  File		: S3PAccount.h			   //
//	Author		: Yang Xiaodong            //
//	Modified	: 8/29/2002                //
//                                         //
//-----------------------------------------//

#if !defined(AFX_S3PACCOUNT_H__7222E304_3E04_44D2_A3D1_E277BEF07235__INCLUDED_)
#define AFX_S3PACCOUNT_H__7222E304_3E04_44D2_A3D1_E277BEF07235__INCLUDED_

#include "AccountLogindef.h"
#include "GlobalDTD.h"
#include <string>
#include <list>

class S3PDBConVBC;

using namespace std;
typedef std::list<KAccountUserTimeInfo> AccountTimeList;

class S3PAccount  
{
public:
	S3PAccount();
	virtual ~S3PAccount();

	static INT Login(S3PDBConVBC* pConn, const char* strAccName, const char* strPassword, DWORD ClientID, WORD& nExtPoint, DWORD& nLeftTime);
	static INT LoginGame(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName);
	static INT Logout(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName, WORD nExtPoint, BYTE nExtDay);
	static INT ElapseTime(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName, const DWORD dwDecSecond);
	static INT QueryTime(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName, DWORD& dwSecond);
	static INT ServerLogin(S3PDBConVBC* pConn, const char* strAccName, const char* strPassword, const DWORD Address, const int16_t Port, const BYTE Mac[6], ULONG& nGameID);
	static INT UnlockAll(S3PDBConVBC* pConn, DWORD ClientID);
	static INT ElapseAll(S3PDBConVBC* pConn, DWORD ClientID);
	static INT CheckAddress(S3PDBConVBC* pConn, const DWORD Address, const short Port);
	static INT GetAccountGameID(S3PDBConVBC* pConn, const char* strAccName, DWORD& ClientID);
	static INT VerifyUserModifyPassword(S3PDBConVBC* pConn, DWORD ClientID, const char* strAccName, const char* strPassword);
	static INT GetServerID(S3PDBConVBC* pConn, const char* strAccName, ULONG& nGameID);

protected:
	static INT GetLeftSecondsOfDeposit(S3PDBConVBC* pConn, const char* strAccName, LONG& liLeft, LONG& liExp);
	static INT GetLockAccount(S3PDBConVBC* pConn, const char* strAccName, DWORD& nLeftTime);
};

#endif // !defined(AFX_S3PACCOUNT_H__7222E304_3E04_44D2_A3D1_E277BEF07235__INCLUDED_)
