#ifndef S3ClientH
#define	S3ClientH

#include "KEngine.h"
#include "KMp3Music.h"
#include "../../Core/Src/CoreShell.h"

#include <commctrl.h>

class KInlinePicSink;
class KMyApp : public KWin32App
{
private:
	KMp3Music			m_Music;
	KDirectSound		m_Sound;
	KTimer				m_Timer;
	DWORD				m_GameCounter;
	KInlinePicSink*		m_pInlinePicSink;
	
public:
	KMyApp();
	BOOL				GameInit();
	BOOL				GameLoop();
	BOOL				GameExit();
	BOOL				Game_AntiVMW();
	BOOL				GameLimitWindown();
	
protected:
	INT					HandleInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

struct KClientCallback : public IClientCallback
{
	void CoreDataChanged(UINT uDataId, UINT uParam, INT nParam);
	void ChannelMessageArrival(DWORD nChannelID, char* szSendName, const char* pMsgBuff, uint16_t nMsgLength, const char* pItem = 0, BYTE btSomeFlag = 0, bool bSucc = false);
	void MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, uint16_t nMsgLength, const char* pItem = 0, bool bSucc = false);
	void NotifyChannelID(char* ChannelName, DWORD channelid, BYTE cost);
	void FriendInvite(char* roleName);
	void AddFriend(char* roleName, BYTE answer);	//添加好友到好友组
	void DeleteFriend(char* roleName);	//添加好友到好友组
	void FriendStatus(char* roleName, BYTE state);	//有关系人的状态,找到第一个为止
	void FriendInfo(char* roleName, char* unitName, char* groupName, BYTE state);	//同步关系人数据
	void AddPeople(char* unitName, char* roleName);	//添加关系人到指定组
};

void	UpdateGameVersion();
void	HexNumberCheatHack(INT limit);
char* HexStringCheatHack(char hexStr[255]);

#endif
