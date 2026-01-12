#ifndef KLadderH
#define	KLadderH

#include "KProtocol.h"

class KLadder
{
public:
	KLadder();
	~KLadder();
	const TRoleList* TopTenRich();
	const TRoleList* TopTenMasterHand();
	const TRoleList* TopTenKiller();
	const TRoleList* TopTenRepute();
	const TRoleList* TopTenFuYuan();
	const TRoleList* TopTenAccumStat1();
	const TRoleList* TopTenAccumStat2();
	const TRoleList* TopTenHonorStat();
	const TRoleList* TopTenTimeStat();
	const TRoleList* TopTenTongLv();
	const TRoleList* TopTenTongMn();
	const TRoleList* TopTenTongEff();
	const TRoleList* TopTenFacRich(INT nFac);
	const TRoleList* TopTenFacMasterHand(INT nFac);
	const TRoleList* GetTopTen(DWORD	dwLadderID);
	INT					GetFacMemberCount(INT nFac);
	INT					GetFacMasterHandPercent(INT nFac);
	INT					GetFacMoneyPercent(INT nFac);
	BOOL				Init(void* pData, size_t uSize);
private:
	TGAME_STAT_DATA	GameStatData;
};

extern KLadder	Ladder;
#endif