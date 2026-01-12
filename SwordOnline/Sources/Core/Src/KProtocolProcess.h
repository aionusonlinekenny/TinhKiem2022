#ifndef KProtocolProcessH
#define	KProtocolProcessH

#include "../Headers/KProtocol.h"
class KProtocolProcess
{
private:
#ifndef _SERVER
	void (KProtocolProcess::* ProcessFunc[s2c_end])(BYTE* pMsg);
#else
	void (KProtocolProcess::* ProcessFunc[c2s_end])(INT nIndex, BYTE* pMsg);
#endif
public:
	KProtocolProcess();
	~KProtocolProcess();
#ifndef _SERVER
	void ProcessNetMsg(BYTE* pMsg);
#else
	void ProcessNetMsg(INT nIndex, BYTE* pMsg);
#endif
private:
#ifdef TOOLVERSION
public:
#endif
#ifndef _SERVER
	void	s2cAccoutLoginResult(BYTE* pMsg);
	void	SyncCurNormalData(BYTE* pMsg);
	void	SyncWorld(BYTE* pMsg);
	void	SyncNpc(BYTE* pMsg);
	void	SyncNpcPos(BYTE* pMsg);
	void	SyncNpcMin(BYTE* pMsg);
	void	SyncNpcMinPlayer(BYTE* pMsg);
	void	SyncPlayer(BYTE* pMsg);
	void	SyncPlayerMin(BYTE* pMsg);
	void	SyncCurPlayer(BYTE* pMsg);
	void	SyncObjectAdd(BYTE* pMsg);
	void	SyncObjectState(BYTE* pMsg);
	void	SyncObjectDir(BYTE* pMsg);
	void	SyncObjectRemove(BYTE* pMsg);
	void	NetCommandWalk(BYTE* pMsg);
	void	NetCommandRun(BYTE* pMsg);
	void	NetCommandJump(BYTE* pMsg);
	void	NetCommandSkill(BYTE* pMsg);
	void	NetCommandHurt(BYTE* pMsg);
	void	NetCommandDeath(BYTE* pMsg);
	void	NetCommandRemoveNpc(BYTE* pMsg);
	void	NetCommandChgCurCamp(BYTE* pMsg);
	void	NetCommandChgCamp(BYTE* pMsg);
	void	NetCommandSit(BYTE* pMsg);
	void	NetCommandStand(BYTE* pMsg);
	void	NetCommandChat(BYTE* pMsg);
	void	NetCommandSetPos(BYTE* pMsg);
	void	OpenSaleBox(BYTE* pMsg);
	void	OpenSomeBox(BYTE* pMsg);
	void	s2cShowTeamInfo(BYTE* pMsg);
	void	s2cUpdataSelfTeamInfo(BYTE* pMsg);
	void	s2cApplyTeamInfoFalse(BYTE* pMsg);
	void	s2cCreateTeam(BYTE* pMsg);
	void	s2cApplyCreateTeamFalse(BYTE* pMsg);
	void	s2cSetTeamState(BYTE* pMsg);
	void	s2cApplyAddTeam(BYTE* pMsg);
	void	s2cTeamAddMember(BYTE* pMsg);
	void	s2cLeaveTeam(BYTE* pMsg);
	void	s2cTeamChangeCaptain(BYTE* pMsg);
	void	s2cSetFactionData(BYTE* pMsg);
	void	s2cLeaveFaction(BYTE* pMsg);
	void	s2cSetMissionData(BYTE* pMsg);
	void	s2cGetLeadExp(BYTE* pMsg);
	void	s2cGetTeammateLevel(BYTE* pMsg);
	void	s2cLevelUp(BYTE* pMsg);
	void	s2cGetCurAttribute(BYTE* pMsg);
	void	s2cGetSkillLevel(BYTE* pMsg);
	void	s2cSyncAllSkill(BYTE* pMsg);
	void	s2cSyncMoney(BYTE* pMsg);
	void	s2cMoveItem(BYTE* pMsg);
	void	s2cRemoveItem(BYTE* pMsg);
	void	s2cSyncItem(BYTE* pMsg);
	void	s2cSyncMagic(BYTE* pMsg);
	void	SyncScriptAction(BYTE* pMsg);
	void	s2cChatGetApplyAddFriend(BYTE* pMsg);
	void	s2cSyncRoleList(BYTE* pMsg);
	void	SyncEnd(BYTE* pMsg);
	void	s2cTradeChangeState(BYTE* pMsg);
	void	s2cNpcSetMenuState(BYTE* pMsg);
	void	s2cTradeMoneySync(BYTE* pMsg);
	void	s2cTradeDecision(BYTE* pMsg);
	void	s2cPlayerExp(BYTE* pMsg);
	void	s2cTeamInviteAdd(BYTE* pMsg);
	void	s2cTradePressOkSync(BYTE* pMsg);
	void	s2cPing(BYTE* pMsg);
	void	s2cDirectlyCastSkill(BYTE* pMsg);
	void	s2cShowMsg(BYTE* pMsg);
	void	SyncStateEffect(BYTE* pMsg);
	void	IgnoreState(BYTE* pMsg);
	void	PlayerRevive(BYTE* pMsg);
	void	RequestNpcFail(BYTE* pMsg);
	void	s2cTradeApplyStart(BYTE* pMsg);
	void	s2cCreateTong(BYTE* pMsg);
	void	s2cJoinTong(BYTE* pMsg);
	void	s2cJoinTongReply(BYTE* pMsg);
	void	s2cItemAutoMove(BYTE* pMsg);
	void	s2cChangeWeather(BYTE* pMsg);
	void	s2cPKSyncNormalFlag(BYTE* pMsg);
	void	s2cPKSyncEnmityState(BYTE* pMsg);
	void	s2cPKSyncExerciseState(BYTE* pMsg);
	void	s2cPKValueSync(BYTE* pMsg);
	void	NpcSleepSync(BYTE* pMsg);
	void	s2cViewEquip(BYTE* pMsg);
	void	LadderResult(BYTE* pMsg);
	void	LadderList(BYTE* pMsg);
	void	s2cTongCreate(BYTE* pMsg);
	void	ServerReplyClientPing(BYTE* pMsg);
	void	ItemChangeDurability(BYTE* pMsg);
	void	SendTradeAction(BYTE* pMsg);
	void	SendTradeItem(BYTE* pMsg);
	void	SendTradeCount(BYTE* pMsg);
	void	SendTradeFail(BYTE* pMsg);
	void	NetCommandSetRank(BYTE* pMsg);
	void	NetCommandSetExpandRank(BYTE* pMsg);
	void	NetCommandSetHorse(BYTE* pMsg);
	void	s2cSyncTaskValue(BYTE* pMsg);
	void	s2cPlayerSync(BYTE* pMsg);
	void	SetExtPoint(BYTE* pMsg);
	void	SyncGive(BYTE* pMsg);
	void	SyncMaskLock(BYTE* pMsg);
	void	SyncDataRes(BYTE* pMsg);
	void	s2cSyncRankData(BYTE* pMsg);
	void	s2cSyncSuperShop(BYTE* pMsg);
	void	s2cSparApplyStart(BYTE* pMsg);
	void	s2cLockMove(BYTE* pMsg);
	void	s2cChatRoomState(BYTE* pMsg);
	void	s2cApplyCreateChatRoomFalse(BYTE* pMsg);
	void	s2cRequestChatRoomList(BYTE* pMsg);
	void	s2cRequestMemberList(BYTE* pMsg);
	void	s2cRequestBlackList(BYTE* pMsg);
	void	s2cOpenMake(BYTE* pMsg);
	void	OpenDTBox(BYTE* pMsg);//TamLTM end code // nhan phan thuong da tau
	void	s2cOpenOtherBox(BYTE* pMsg); // TamLTM kham nam xanh
	void	OpenProgressBar(BYTE* pMsg);//TamLTM open OpenProgressBar
	void	SysNcPosMin(BYTE* pMsg);//TamLTM fix lag posu
	void	s2cUpdateNpcState(BYTE* pMsg);
	void	ExitGame(BYTE* pMsg);

	void	s2cExtend(BYTE* pMsg);
	void	s2cExtendChat(BYTE* pMsg);
	void	s2cExtendFriend(BYTE* pMsg);
	void	s2cExtendTong(BYTE* pMsg);


#else
	void	RemoveRole(INT nIndex, BYTE* pProtocol);
	void	NpcRequestCommand(INT nIndex, BYTE* pMsg);
	void	ObjRequestCommand(INT nIndex, BYTE* pProtocol);
	void	NpcWalkCommand(INT nIndex, BYTE* pProtocol);
	void	NpcRunCommand(INT nIndex, BYTE* pProtocol);
	void	NpcSkillCommand(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamInfo(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyCreateTeam(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamChangeState(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyAddTeam(INT nIndex, BYTE* pProtocol);
	void	PlayerAcceptTeamMember(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyLeaveTeam(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamKickMember(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamChangeCaptain(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyTeamDismiss(INT nIndex, BYTE* pProtocol);
	void	PlayerApplySetPK(INT nIndex, BYTE* pProtocol);
	void	PlayerSendChat(INT nIndex, BYTE* pProtocol);
	void	PlayerAddBaseAttribute(INT nIndex, BYTE* pProtocol);
	void	PlayerApplyAddSkillPoint(INT nIndex, BYTE* pProtocol);
	void	PlayerEatItem(INT nIndex, BYTE* pProtocol);
	void	PlayerPickUpItem(INT nIndex, BYTE* pProtocol);
	void	PlayerMoveItem(INT nIndex, BYTE* pProtocol);
	void	PlayerSellItem(INT nIndex, BYTE* pProtocol);
	void	PlayerBuyItem(INT nIndex, BYTE* pProtocol);
	void	PlayerDropItem(INT nIndex, BYTE* pProtocol);
	void	PlayerSelUI(INT nIndex, BYTE* pProtocol);
	void	TradeApplyOpen(INT nIndex, BYTE* pProtocol);
	void	TradeApplyClose(INT nIndex, BYTE* pProtocol);
	void	TradeApplyStart(INT nIndex, BYTE* pProtocol);
	void	TradeMoveMoney(INT nIndex, BYTE* pProtocol);
	void	TradeDecision(INT nIndex, BYTE* pProtocol);
	void	DialogNpc(INT nIndex, BYTE* pProtocol);
	void	TeamInviteAdd(INT nIndex, BYTE* pProtocol);
	void	ChangeAuraSkill(INT nIndex, BYTE* pProtocol);
	void	TeamReplyInvite(INT nIndex, BYTE* pProtocol);
	//void	ReplyPing(INT nIndex, BYTE* pProtocol);
	void	NpcSitCommand(INT nIndex, BYTE* pProtocol);
	void	ObjMouseClick(INT nIndex, BYTE* pProtocol);
	void	StoreMoneyCommand(INT nIndex, BYTE* pProtocol);
	void	NpcReviveCommand(INT nIndex, BYTE* pProtocol);
	void	c2sTradeReplyStart(INT nIndex, BYTE* pProtocol);
	void	c2sPKApplyChangeNormalFlag(INT nIndex, BYTE* pProtocol);
	void	c2sPKApplyEnmity(INT nIndex, BYTE* pProtocol);
	void	c2sViewEquip(INT nIndex, BYTE* pProtocol);
	void	LadderQuery(INT nIndex, BYTE* pProtocol);
	void	ItemRepair(INT nIndex, BYTE* pProtocol);
	void	DirectSell(INT nIndex, BYTE* pProtocol);
	void	TradeSet(INT nIndex, BYTE* pProtocol);
	void	TradeStart(INT nIndex, BYTE* pProtocol);
	void	TradeView(INT nIndex, BYTE* pProtocol);
	void	TradeBuy(INT nIndex, BYTE* pProtocol);
	void	TradeEnd(INT nIndex, BYTE* pProtocol);
	void	NpcHorseCommand(INT nIndex, BYTE* pProtocol);
	void	PlayerCommand(INT nIndex, BYTE* pProtocol);
	void	InputCommand(INT nIndex, BYTE* pProtocol);
	void	UnlockCommand(INT nIndex, BYTE* pProtocol);
	void	BreakCommand(INT nIndex, BYTE* pProtocol);
	void	JoinTongReply(INT nIndex, BYTE* pProtocol);
	void	SparApplyStart(INT nIndex, BYTE* pProtocol);
	void	LockMove(INT nIndex, BYTE* pProtocol);
	void	ChatRoomDecision(INT nIndex, BYTE* pProtocol);
	void	c2sSetImage(INT nIndex, BYTE* pProtocol);
	void	NpcDaTauCommand(INT nIndex, BYTE* pProtocol);//TamLTM da tau
	void	UiCommandScript(INT nIndex, BYTE* pProtocol);	// TamLTM kham nam
	void	RecoveryBoxCmd(INT nIndex, BYTE* pProtocol);	//TamLTM Kham nam
	void	c2sInputCommand(INT nIndex, BYTE* pProtocol);	//TamLTM Kham nam
	void	NpcOpenProgressBarCommand(INT nIndex, BYTE* pProtocol);//TamLTM progress bar
	void	NpcOfflineCommand(INT nIndex, BYTE* pProtocol);//TamLTM Uy thac offline
	void	CompoundItem(INT nIndex, BYTE* pProtocol);//compound item
	void	EnchaseItem(INT nIndex, BYTE* pProtocol);//enchase item
	void	WithDrawaMoneyCommand(INT nIndex, BYTE* pProtocol); // rut tien;
#endif
};

extern KProtocolProcess g_ProtocolProcess;
#endif
