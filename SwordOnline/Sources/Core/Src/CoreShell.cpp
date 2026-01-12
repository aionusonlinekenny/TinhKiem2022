/*****************************************************************************************
//	Íâ½ç·ÃÎÊCore½Ó¿Ú·½·¨
//	Copyright : Kingsoft 2002
//	Author	:
//	CreateTime:	2002-9-12
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KCore.h"
#include "GameDataDef.h"
#include "CoreShell.h"
#include "KPlayerTask.h"
#include "KEngine.h"
#include "KTaskFuns.h"

#include "CoreDrawGameObj.h"
#include "ImgRef.h"

#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KObjSet.h"
#include "KItemList.h"
#include "KSubWorldSet.h"
#include "KProtocolProcess.h"

#include "KNpcResList.h"
#include "Scene/KScenePlaceC.h"
#include "kskills.h"
#include "GameDataDef.h"
#include "MsgGenreDef.h"
#include "KOption.h"
#include "KSubWorld.h"
#include "KViewItem.h"
#include "KSellItem.h"
#include "KItemSet.h"
#include "KTongData.h"
#include "KTongProtocol.h"
#include "KMagicDesc.h"
#include "malloc.h"

#include "KBuySell.h"
#include "KThiefSkill.h"
#include "KItemChangeRes.h"
//
#include "KMath.h" // them g_Distance // find way by kinnox;
#include <math.h> // find way by kinnox;
#include "../KJXPathFinder.h" // find way by kinnox;
//
#define	NPC_TRADE_BOX_WIDTH		6
#define	NPC_TRADE_BOX_HEIGHT	10
#define	MAX_TRADE_ITEM_WIDTH	2
#define	MAX_TRADE_ITEM_HEIGHT	4
IClientCallback* l_pDataChangedNotifyFunc = 0;

class KCoreShell : public iCoreShell
{
public:
	INT	 GetProtocolSize(BYTE byProtocol);
	INT	 Debug(UINT uDataId, UINT uParam, INT nParam);
	INT	 OperationRequest(UINT uOper, UINT uParam, INT nParam);
	void ProcessInput(UINT uMsg, UINT uParam, INT nParam);
	void AutoRun(INT x, INT y);
	INT GetTargetNPC();
	INT	 FindSelectNPC(INT x, INT y, INT nRelation, bool bSelect, void* pReturn, INT& nKind, bool bFind);
	INT FindSelectObject(INT x, INT y, bool bSelect, INT& nObjectIdx, INT& nKind);
	INT FindSpecialNPC(char* Name, void* pReturn, INT& nKind);
	INT ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, uint16_t nMsgLength);
	void ApplyAddTeam(void* pPlayer);
	void TradeApplyStart(void* pPlayer, bool bFolkGame);
	void JoinTongReply(void* pPlayer);
	INT UseSkill(INT x, INT y, INT nSkillID);
	INT LockSomeoneUseSkill(INT nTargetIndex, INT nSkillID);
	INT LockSomeoneAction(INT nTargetIndex);
	INT LockObjectAction(INT nTargetIndex);
	void GotoWhere(INT x, INT y, INT mode);	//mode 0 is auto, 1 is walk, 2 is run
	void GetWhere(INT x, INT y, void* pInfo);
	void GotoWhereDirect(int x, int y, int mode);	//mode 0 is auto, 1 is walk, 2 is run // find way by kinnox;
	BOOL AutoMove();// find way by kinnox;
	void Goto(INT nDir, INT mode);	//nDir 0~63, mode 0 is auto, 1 is walk, 2 is run
	void Turn(INT nDir);	//nDir 0 is left, 1 is right, 2 is back
	INT ThrowAwayItem();
	INT GetNPCRelation(INT nIndex);
	BOOL CheckMouse();



	//ÓëµØÍ¼Ïà¹ØµÄ²Ù×÷,uOperµÄÈ¡ÖµÀ´×Ô GAME_SCENE_MAP_OPERATION_INDEX
	INT	SceneMapOperation(UINT uOper, UINT uParam, INT nParam);
	//Óë°ï»áÏà¹ØµÄ²Ù×÷, uOperµÄÈ¡ÖµÀ´×Ô GAME_TONG_OPERATION_INDEX
	INT	TongOperation(UINT uOper, UINT uParam, INT nParam);
	//Óë×é¶ÓÏà¹ØµÄ²Ù×÷£¬uOperµÄÈ¡ÖµÀ´×Ô GAME_TEAM_OPERATION_INDEX
	INT TeamOperation(UINT uOper, UINT uParam, INT nParam);

	INT PAIOperation(UINT uOper, UINT uParam, INT nParam, INT nParam1);

	INT	 GetGameData(UINT uDataId, UINT uParam, INT nParam);

	void DrawGameObj(UINT uObjGenre, UINT uId, INT x, INT y, INT Width, INT Height, INT nParam);
	void DrawGameSpace();
	DWORD GetPing();
	//void SendPing();
	INT	 SetCallDataChangedNofify(IClientCallback* pNotifyFunc);
	void NetMsgCallbackFunc(void* pMsgData);
	void SetRepresentShell(struct iRepresentShell* pRepresent);
	void SetMusicInterface(void* pMusicInterface);
	void SetRepresentAreaSize(INT nWidth, INT nHeight);
	void SetScreenSize(INT nWidth, INT nHeight);
	INT  Breathe(BOOL isDisconnected);
	void Release();	//ÊÍ·Å½Ó¿Ú¶ÔÏó
	void SetClient(LPVOID pClient);
	void SendNewDataToServer(void* pData, INT nLength);
	void DirectFindPos(UINT uParam, INT nParam, BOOL bSync, BOOL bPaintLine);
	void CheckHoverMouseMiniMap(BOOL hoverM);
	void SetAutoRun(BOOL autoRun);
	void SetFlagAutoRun(BOOL autoRunFlag, INT x, INT y);
	BOOL GetPaintMode();
	void SetPaintMode(BOOL nIndex);
	BOOL GetFlagMode();
	void SetFlagMode(BOOL nIndex);
	void GetSizeItem(UINT uId, INT* nW, INT* nH);
	INT	GetExtPoint();
	INT GetNatureItem(UINT uItemId, UINT uGenre);
	INT GetGenreItem(UINT uItemId, UINT uGenre);
	INT IsDamage(UINT uItemId);
	BOOL GetLockState();
	INT GetStackNum(UINT uItemId);
	BOOL GetSkillData(INT nSkillId, INT* nLevel);
	void GetSkillName(INT nSkillId, char* szSkillName);
	INT GetTradePrice(UINT uItemId);
	INT GetTradeState(INT nIndex, INT uId);
	INT	GetFightState();
	void BreakItem(UINT uParam, INT nParam, BOOL bIsBreakAll);
	DWORD GetdwIDPlayer(UINT uId);
	INT	GetOwnValue(INT nMoneyUnit);
	INT GetDataSuperShop(INT nSaleId, UINT uParam, INT nParam);
	INT GetDataDynamicShop(INT nSaleId, UINT uParam, INT nParam);
	INT GetNextSkill(INT nType, INT nIndex);
	BOOL IsSkillAura(INT nSkillId);

	//TamLTM kham nam xanh
	INT GetKindItem(UINT uId);
	INT GetDetailItem(UINT uId);
	INT GetParticularItem(UINT uId);
	INT GetLevelItem(UINT uId);
	INT GetSeriesItem(UINT uId);
	INT GetItemColor(UINT uItemId);
	//End code

	void GetWayPointName(INT nMapID, char* szMapName);
	void GetWayPointPos(INT nWayPointID, INT* nWorld, INT* nPosX, INT* nPosY);
};

static KCoreShell	g_CoreShell;

CORE_API void g_InitCore();
#ifndef _STANDALONE
extern "C" __declspec(dllexport)
#endif
iCoreShell * CoreGetShell()
{
	g_InitCore();
	return &g_CoreShell;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º·¢³öÓÎÏ·ÊÀ½çÊý¾Ý¸Ä±äµÄÍ¨Öªº¯Êý
//	·µ»Ø£ºÈçÎ´±»×¢²áÍ¨Öªº¯Êý£¬ÔòÖ±½Ó·µ»Ø0£¬·ñÔò·µ»ØÍ¨Öªº¯ÊýÖ´ÐÐ½á¹û¡£
//--------------------------------------------------------------------------

//--------------------------------------------------------------------
void CoreDataChanged(UINT uDataId, UINT uParam, INT nParam)
{
	if (l_pDataChangedNotifyFunc)
		l_pDataChangedNotifyFunc->CoreDataChanged(uDataId, uParam, nParam);
}

void KCoreShell::Release()
{
	g_ReleaseCore();
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º½ÓÊÜÓë·ÖÅÉ´¦ÀíÍøÂçÏûÏ¢
//--------------------------------------------------------------------------
void KCoreShell::NetMsgCallbackFunc(void* pMsgData)
{
	g_ProtocolProcess.ProcessNetMsg((BYTE*)pMsgData);
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉèÖÃÓÎÏ·ÊÀ½çÊý¾Ý¸Ä±äµÄÍ¨Öªº¯Êý
//	²ÎÊý£ºfnCoreDataChangedCallback pNotifyFunc --> Í¨Öªº¯ÊýµÄÖ¸Õë¡£
//	·µ»Ø£º·µ»ØÖµÎª·Ç0Öµ±íÊ¾×¢²á³É¹¦£¬·ñÔò±íÊ¾Ê§°Ü¡£
//--------------------------------------------------------------------------
INT	KCoreShell::SetCallDataChangedNofify(IClientCallback* pNotifyFunc)
{
	l_pDataChangedNotifyFunc = pNotifyFunc;
	return true;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´ÓÓÎÏ·ÊÀ½ç»ñÈ¡Êý¾Ý
//	²ÎÊý£ºunsigned INT uDataId --> ±íÊ¾»ñÈ¡ÓÎÏ·Êý¾ÝµÄÊý¾ÝÏîÄÚÈÝË÷Òý£¬ÆäÖµÎªÃ·¾ÙÀàÐÍ
//							GAMEDATA_INDEXµÄÈ¡ÖµÖ®Ò»¡£
//		  UINT uParam  --> ÒÀ¾ÝuDataIdµÄÈ¡ÖµÇé¿ö¶ø¶¨
//		  INT nParam --> ÒÀ¾ÝuDataIdµÄÈ¡ÖµÇé¿ö¶ø¶¨
//	·µ»Ø£ºÒÀ¾ÝuDataIdµÄÈ¡ÖµÇé¿ö¶ø¶¨¡£
//--------------------------------------------------------------------------
INT	KCoreShell::GetGameData(UINT uDataId, UINT uParam, INT nParam)
{
	INT nRet = 0;
	switch (uDataId)
	{
	case GDI_PLAYER_IS_MALE:
	{
		INT nIndex = 0;
		if (nParam == 0)
			nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		else
			nIndex = NpcSet.SearchID(nParam);

		if (nIndex)
			nRet = (Npc[nIndex].m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID);
		else
			nRet = 1;	//³ö´íÊ±ÊÇÄÐÐÔ
	}
	break;
	case GDI_REPAIR_ITEM_PRICE:
		if (uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion*)uParam;
			KItem* pItem = NULL;

			switch (pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
			{
				if (pObj->Obj.uId > 0)
				{
					pItem = &Item[pObj->Obj.uId];
				}
			}
			break;
			default:
				break;
			}

			if (!pItem)
				break;

			KUiItemBuySelInfo* pInfo = (KUiItemBuySelInfo*)nParam;

			if (pObj->eContainer == UOC_NPC_SHOP)
				break;

			pInfo->nItemNature = pItem->GetNature();
			pInfo->bNewArrival = FALSE;
			pInfo->nMoneyUnit = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nMoneyUnit;
			pInfo->nCurPrice = pItem->GetRepairPrice();
			strcpy(pInfo->szItemName, pItem->GetName());
			nRet = pItem->CanBeRepaired();
		}
		else
		{
			nParam = 0;
			nRet = 0;
		}
		break;

	case GDI_TRADE_ITEM_PRICE:
		if (uParam && !Player[CLIENT_PLAYER_INDEX].m_ItemList.IsLockOperation())
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion*)uParam;
			KItem* pItem = NULL;

			switch (pObj->Obj.uGenre)
			{
			case CGOG_PLAYERSELLITEM:
			{
				if (pObj->Obj.uId > 0)
				{
					KUiItemBuySelInfo* pInfo = (KUiItemBuySelInfo*)nParam;
					pInfo->nItemNature = Item[pObj->Obj.uId].GetNature();
					pInfo->bNewArrival = FALSE;
					pInfo->nMoneyUnit = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nMoneyUnit;
					pInfo->nCurPrice = Item[pObj->Obj.uId].GetTradePrice();
					strcpy(pInfo->szItemName, Item[pObj->Obj.uId].GetName());
					nRet = 1;
					return nRet;
				}
			}
			break;
			case CGOG_ITEM:
			{
				if (pObj->Obj.uId > 0)
				{
					pItem = &Item[pObj->Obj.uId];
				}
			}
			break;
			case CGOG_NPCSELLITEM:
			{
				INT	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop];
				if (nBuyIdx != -1)
				{
					INT nIndex = BuySell.GetItemIndex(nBuyIdx, pObj->Obj.uId);
					if (nIndex >= 0)
						pItem = BuySell.GetItem(nIndex);
				}
			}
			break;
			default:
				break;
			}

			if (!pItem)
				break;

			KUiItemBuySelInfo* pInfo = (KUiItemBuySelInfo*)nParam;
			pInfo->nItemNature = pItem->GetNature();
			pInfo->nOldPrice = pItem->GetOrgPrice();
			if (pObj->eContainer == UOC_NPC_SHOP)
				pInfo->nCurPrice = pItem->GetCurPrice();
			else
				pInfo->nCurPrice = pItem->GetSalePrice();

			pInfo->nOldPrice = pInfo->nCurPrice * 5;
			pInfo->bNewArrival = pItem->GetNewArrival();
			strcpy(pInfo->szItemName, pItem->GetName());
			pInfo->nMoneyUnit = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nMoneyUnit;
			nRet = 1;
		}
		else
		{
			nParam = 0;
			nRet = 0;
		}
		break;
		//ÓÎÏ·¶ÔÏóÃèÊöËµÃ÷ÎÄ±¾´®
		//uParam = (KUiGameObject*) ÃèÊöÓÎÏ·¶ÔÏóµÄ½á¹¹Êý¾ÝµÄÖ¸Õë
		//nParam = (char*) Ö¸ÏòÒ»¸ö»º³åÇøµÄÖ¸Õë£¬Æä¿Õ¼ä²»ÉÙÓÚ256×Ö½Ú¡£
	case GDI_GAME_OBJ_DESC_INCLUDE_REPAIRINFO:
	case GDI_GAME_OBJ_DESC_INCLUDE_TRADEINFO:
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion*)uParam;
			char* pszDescript = (char*)nParam;
			pszDescript[0] = 0;
			switch (pObj->Obj.uGenre)
			{
			case CGOG_PLAYERSELLITEM:
				Item[pObj->Obj.uId].GetDesc(pszDescript, true, true);
				break;
			case CGOG_ITEM:
			{
				if (pObj->eContainer == UOC_EQUIPTMENT)
				{
					Item[pObj->Obj.uId].GetDesc(pszDescript, true, true, Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(pObj->Obj.uId),
						Player[CLIENT_PLAYER_INDEX].m_ItemList.GetGoldActiveAttribNum(pObj->Obj.uId));
				}
				else
					Item[pObj->Obj.uId].GetDesc(pszDescript, true, true);
			}
			break;
			case CGOG_NPCSELLITEM:
			{
				INT nIdx = -1;
				if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop])
					break;
				nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop], pObj->Obj.uId);

				KItem* pItem = NULL;
				if (nIdx < 0)
					break;
				pItem = BuySell.GetItem(nIdx);

				if (!pItem)
					break;
				pItem->GetDesc(pszDescript, true);
			}
			break;
			}
		}
		break;
	case GDI_GAME_OBJ_DESC:
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion*)uParam;
			char* pszDescript = (char*)nParam;
			pszDescript[0] = 0;
			switch (pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
			{
				if (pszDescript)
					if (pObj->eContainer == UOC_EQUIPTMENT)
					{
						Item[pObj->Obj.uId].GetDesc(pszDescript, false, false, Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(pObj->Obj.uId),
							Player[CLIENT_PLAYER_INDEX].m_ItemList.GetGoldActiveAttribNum(pObj->Obj.uId));
					}
					else
						Item[pObj->Obj.uId].GetDesc(pszDescript);

				_ASSERT(pszDescript);
			}
			break;
			case CGOG_SKILL:
			case CGOG_SKILL_FIGHT:
			case CGOG_SKILL_SHORTCUT:
			{
				INT nLevel = 0;
				if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLevel(pObj->Obj.uId) > 0)
					nLevel = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(pObj->Obj.uId);
				_ASSERT(nLevel >= 0);
				if (pObj->Obj.uId > 0)
				{

					ISkill* pISkill = g_SkillManager.GetSkill(pObj->Obj.uId, 1);
					if (!pISkill)
						break;
					eSkillStyle eStyle = (eSkillStyle)pISkill->GetSkillStyle();

					switch (eStyle)
					{
					case SKILL_SS_Missles:			//	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
					case SKILL_SS_Melee:
					case SKILL_SS_InitiativeNpcState:	//	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
					case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
					{
						KSkill::GetDesc(
							pObj->Obj.uId,
							nLevel,
							pszDescript,
							Player[CLIENT_PLAYER_INDEX].m_nIndex,
							(pObj->Obj.uGenre == CGOG_SKILL_SHORTCUT) ? false : true
						);
					}
					break;
					case SKILL_SS_Thief:
					{
						((KThiefSkill*)pISkill)->GetDesc(
							pObj->Obj.uId,
							nLevel,
							pszDescript,
							Player[CLIENT_PLAYER_INDEX].m_nIndex,
							(pObj->Obj.uGenre == CGOG_SKILL_SHORTCUT) ? false : true
						);

					}break;

					}
				}

			}
			break;
			case CGOG_NPCSELLITEM:
			{
				INT nIdx = -1;
				if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop])
					break;
				nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop], pObj->Obj.uId);

				KItem* pItem = NULL;
				if (nIdx < 0)
					break;
				pItem = BuySell.GetItem(nIdx);

				if (!pItem)
					break;
				pItem->GetDesc(pszDescript);
			}
			break;
			case CGOG_IME_ITEM:
			{
				if (pObj->Obj.uId <= 0)
					return 0;
				Item[pObj->Obj.uId].GetDesc(pszDescript);
			}
			break;
			case CGOG_PLAYERSELLITEM:
				Item[pObj->Obj.uId].GetDesc(pszDescript, true, true, 0);
				break;
			case CGOG_PLAYERVIEWITEM:
				Item[pObj->Obj.uId].GetDesc(pszDescript);
				break;
			}
		}
		break;
	case GDI_GAME_OBJ_DESC_INCLUDE_MOUSEHOVER:
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion*)uParam;
			char* pszFileName = (char*)nParam;
			switch (pObj->Obj.uGenre)
			{
			case CGOG_PLAYERVIEWITEM:
			case CGOG_PLAYERSELLITEM:
			case CGOG_ITEM:
				if (Item[pObj->Obj.uId].GetGenre() == item_equip &&
					Item[pObj->Obj.uId].GetDetailType() == equip_mask)
				{
					INT nNpcSettingIdx;
					g_MaskChangeRes.GetInteger(Item[pObj->Obj.uId].GetParticular() + 2, 2, 0, &nNpcSettingIdx);
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetNpcResFile(nNpcSettingIdx, pszFileName);
				}
				break;
			case CGOG_NPCSELLITEM:
			{
				INT nIdx = -1;
				if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop])
					break;
				nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop], pObj->Obj.uId);

				KItem* pItem = NULL;
				if (nIdx < 0)
					break;
				pItem = BuySell.GetItem(nIdx);

				if (!pItem)
					break;
				if (pItem->GetGenre() == item_equip &&
					pItem->GetDetailType() == equip_mask)
				{
					INT nNpcSettingIdx;
					g_MaskChangeRes.GetInteger(pItem->GetParticular() + 2, 2, 0, &nNpcSettingIdx);
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetNpcResFile(nNpcSettingIdx, pszFileName);
				}
			}
			break;
			}
			if (pszFileName[0])
				nRet = 1;
		}
		break;
	case GDI_PLAYER_UID:
		nRet = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID;
		break;
	case GDI_PLAYER_CURCAMP:
		nRet = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp;
		break;
		//Ö÷½ÇµÄÒ»Ð©²»Ò×±äµÄÊý¾Ý
		//uParam = (KUiPlayerBaseInfo*)pInfo	
	case GDI_PLAYER_BASE_INFO:
		if (uParam)
		{
			KUiPlayerBaseInfo* pInfo = (KUiPlayerBaseInfo*)uParam;
			INT nIndex = 0;
			if (nParam == 0)
			{
				nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
				pInfo->nCurFaction = Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction;
				pInfo->nCurTong = Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongNameID();
				pInfo->nMissionGroup = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nMissionGroup;
				pInfo->nRoomId = Player[CLIENT_PLAYER_INDEX].m_cRoom.m_nID;
				pInfo->nRankInSect = Player[CLIENT_PLAYER_INDEX].m_nSectStat;
			}
			else
			{
				nIndex = NpcSet.SearchID(nParam);
				pInfo->nCurFaction = -1;
				pInfo->nCurTong = 0;
				pInfo->nMissionGroup = -1;
				pInfo->nRoomId = -1;
				pInfo->nRankInSect = 0;
			}
			if (nIndex)
			{
				strcpy(pInfo->Name, Npc[nIndex].Name);
				strcpy(pInfo->szMateName, Npc[nIndex].MateName);
				strcpy(pInfo->szTongName, Npc[nIndex].m_szTongName);
				pInfo->Title[0] = 0;
				if (Npc[nIndex].m_RankID)
				{
					char szRankId[5];
					itoa(Npc[nIndex].m_RankID, szRankId, 10);
					g_RankTabSetting.GetString(szRankId, "RANKSTR", "", pInfo->Title, sizeof(pInfo->Title));
				}
				pInfo->nRankInWorld = Npc[nIndex].m_nRankInWorld;
				pInfo->nFuYuan = Npc[nIndex].m_nFuYuan;
				pInfo->nRepute = Npc[nIndex].m_nRepute;
				pInfo->nPKValue = Npc[nIndex].m_nPKValue;
				pInfo->nTranslife = Npc[nIndex].m_byTranslife;
				memset(pInfo->StatusDesc, 0, sizeof(pInfo->StatusDesc));

				switch (Npc[nIndex].m_Series)
				{
				case series_metal:
					strcpy(pInfo->StatusDesc, "HÖ Kim");
					break;
				case series_wood:
					strcpy(pInfo->StatusDesc, "HÖ Méc");
					break;
				case series_water:
					strcpy(pInfo->StatusDesc, "HÖ Thñy");
					break;
				case series_fire:
					strcpy(pInfo->StatusDesc, "HÖ Háa");
					break;
				case series_earth:
					strcpy(pInfo->StatusDesc, "HÖ Thæ");
					break;
				default:
					strcpy(pInfo->StatusDesc, "V« HÖ");
					break;
				}
			}

		}
		break;

		//Ö÷½ÇµÄÒ»Ð©Ò×±äµÄÊý¾Ý
		//uParam = (KUiPlayerRuntimeInfo*)pInfo
	case GDI_PLAYER_RT_INFO:
		if (uParam)
		{
			KUiPlayerRuntimeInfo* pInfo = (KUiPlayerRuntimeInfo*)uParam;
			pInfo->nLifeFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax;		//ÉúÃüÂúÖµ
			pInfo->nLife = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife;				//ÉúÃü
			pInfo->nManaFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax;		//ÄÚÁ¦ÂúÖµ
			pInfo->nMana = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana;				//ÄÚÁ¦
			pInfo->nStaminaFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStaminaMax;//ÌåÁ¦ÂúÖµ
			pInfo->nStamina = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina;		//ÌåÁ¦	
			pInfo->nExperienceFull = Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp;		//¾­ÑéÂúÖµ
			pInfo->nExperience = Player[CLIENT_PLAYER_INDEX].m_nExp;					//¾­Ñé
			pInfo->nCurLevelExperience = Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp;

			pInfo->byAction = PA_NONE;

			if (Player[CLIENT_PLAYER_INDEX].m_RunStatus)
				pInfo->byAction |= PA_RUN;

			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_sit)
				pInfo->byAction |= PA_SIT;

			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse == TRUE)
				pInfo->byAction |= PA_RIDE;

		}
		break;

		//Ö÷½ÇµÄÒ»Ð©Ò×±äµÄÊôÐÔÊý¾Ý
		//uParam = (KUiPlayerAttribute*)pInfo
	case GDI_PLAYER_RT_ATTRIBUTE:
		if (uParam)
		{
			KUiPlayerAttribute* pInfo = (KUiPlayerAttribute*)uParam;
			KNpc* pNpc = &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex];
			pInfo->nMoney = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);				//ÒøÁ½
			pInfo->nBARemainPoint = Player[CLIENT_PLAYER_INDEX].m_nAttributePoint;						//»ù±¾ÊôÐÔÊ£ÓàµãÊý
			pInfo->nStrength = Player[CLIENT_PLAYER_INDEX].m_nCurStrength;								//Á¦Á¿
			pInfo->nDexterity = Player[CLIENT_PLAYER_INDEX].m_nCurDexterity;								//Ãô½Ý
			pInfo->nVitality = Player[CLIENT_PLAYER_INDEX].m_nCurVitality;								//»îÁ¦
			pInfo->nEnergy = Player[CLIENT_PLAYER_INDEX].m_nCurEngergy;									//¾«Á¦
			Player[CLIENT_PLAYER_INDEX].GetEchoDamage(&pInfo->nKillMIN, &pInfo->nKillMAX, 0);				//×î´ó×îÐ¡É±ÉËÁ¦
			Player[CLIENT_PLAYER_INDEX].GetEchoDamage(&pInfo->nRightKillMin, &pInfo->nRightKillMax, 1);

			INT nLeftSkillId = Player[CLIENT_PLAYER_INDEX].GetLeftSkill();
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLevel(nLeftSkillId) > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nLeftSkillId, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nLeftSkillId));
				if (pOrdinSkill && pOrdinSkill->IsUseAR())
					pInfo->nLeftAttack = pNpc->m_CurrentAttackRating;				//¹¥»÷Á¦
				else
					pInfo->nLeftAttack = 0;
			}
			else
				pInfo->nLeftAttack = 0;
			INT nRightSkillId = Player[CLIENT_PLAYER_INDEX].GetRightSkill();
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLevel(nRightSkillId) > 0)
			{
				KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nRightSkillId, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nRightSkillId));
				if (pOrdinSkill->IsUseAR())
					pInfo->nRightAttack = pNpc->m_CurrentAttackRating;				//¹¥»÷Á¦
				else
					pInfo->nRightAttack = 0;
			}
			else
				pInfo->nRightAttack = 0;
			pInfo->nDefence = pNpc->m_CurrentDefend;					//·ÀÓùÁ¦
			if (Player[CLIENT_PLAYER_INDEX].m_RunStatus)
				pInfo->nMoveSpeed = pNpc->m_CurrentRunSpeed;				//ÒÆ¶¯ËÙ¶È
			else
				pInfo->nMoveSpeed = pNpc->m_CurrentWalkSpeed;				//ÒÆ¶¯ËÙ¶È
			pInfo->nAttackSpeed = pNpc->m_CurrentAttackSpeed;
			pInfo->nCastSpeed = pNpc->m_CurrentCastSpeed;//¹¥»÷ËÙ¶È
			//ÎïÀí·ÀÓù
			if (pNpc->m_CurrentPhysicsResistMax >= pNpc->m_CurrentPhysicsResist)
			{
				pInfo->nPhyDef = pNpc->m_CurrentPhysicsResist;
				pInfo->nPhyDefPlus = 0;
			}
			else
			{
				pInfo->nPhyDef = pNpc->m_CurrentPhysicsResistMax;
				pInfo->nPhyDefPlus = (pNpc->m_CurrentPhysicsResist - pNpc->m_CurrentPhysicsResistMax) / RESIST_PLUS_SCALE;
			}
			//±ù¶³·ÀÓù
			if (pNpc->m_CurrentColdResistMax >= pNpc->m_CurrentColdResist)
			{
				pInfo->nCoolDef = pNpc->m_CurrentColdResist;
				pInfo->nCoolDefPlus = 0;
			}
			else
			{
				pInfo->nCoolDef = pNpc->m_CurrentColdResistMax;
				pInfo->nCoolDefPlus = (pNpc->m_CurrentColdResist - pNpc->m_CurrentColdResistMax) / RESIST_PLUS_SCALE;
			}
			//ÉÁµç·ÀÓù
			if (pNpc->m_CurrentLightResistMax >= pNpc->m_CurrentLightResist)
			{
				pInfo->nLightDef = pNpc->m_CurrentLightResist;
				pInfo->nLightDefPlus = 0;
			}
			else
			{
				pInfo->nLightDef = pNpc->m_CurrentLightResistMax;
				pInfo->nLightDefPlus = (pNpc->m_CurrentLightResist - pNpc->m_CurrentLightResistMax) / RESIST_PLUS_SCALE;
			}
			//»ðÑæ·ÀÓù
			if (pNpc->m_CurrentFireResistMax >= pNpc->m_CurrentFireResist)
			{
				pInfo->nFireDef = pNpc->m_CurrentFireResist;
				pInfo->nFireDefPlus = 0;
			}
			else
			{
				pInfo->nFireDef = pNpc->m_CurrentFireResistMax;
				pInfo->nFireDefPlus = (pNpc->m_CurrentFireResist - pNpc->m_CurrentFireResistMax) / RESIST_PLUS_SCALE;
			}
			//¶¾ËØ·ÀÓù
			if (pNpc->m_CurrentPoisonResistMax >= pNpc->m_CurrentPoisonResist)
			{
				pInfo->nPoisonDef = pNpc->m_CurrentPoisonResist;
				pInfo->nPoisonDefPlus = 0;
			}
			else
			{
				pInfo->nPoisonDef = pNpc->m_CurrentPoisonResistMax;
				pInfo->nPoisonDefPlus = (pNpc->m_CurrentPoisonResist - pNpc->m_CurrentPoisonResistMax) / RESIST_PLUS_SCALE;
			}
			pInfo->nLevel = pNpc->m_Level;

			// ¸ù¾ÝÍæ¼Ò×´Ì¬ÏÔÊ¾ ×´Ì¬ÎÄ×Ö not end ²»Ó¦¸ÃÊÇ×´Ì¬£¬Ó¦¸ÃÊÇÎåÐÐÊôÐÔ spe
			memset(pInfo->StatusDesc, 0, sizeof(pInfo->StatusDesc));
			switch (pNpc->m_Series)
			{
			case series_water:
				strcpy(pInfo->StatusDesc, "HÖ Thñy");
				break;
			case series_wood:
				strcpy(pInfo->StatusDesc, "HÖ Méc");
				break;
			case series_metal:
				strcpy(pInfo->StatusDesc, "HÖ Kim");
				break;
			case series_fire:
				strcpy(pInfo->StatusDesc, "HÖ Háa");
				break;
			case series_earth:
				strcpy(pInfo->StatusDesc, "HÖ Thæ");
				break;
			}

			pInfo->nPKValue = Player[CLIENT_PLAYER_INDEX].m_cPK.GetPKValue();
			pInfo->nRepute = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_STATTASK_REPUTE);
			pInfo->nFuYuan = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_STATTASK_FUYUAN);
			pInfo->nTranslife = pNpc->m_byTranslife;
			pInfo->nRankInWorld = Player[CLIENT_PLAYER_INDEX].m_nWorldStat;
			pInfo->Title[0] = 0;
			if (pNpc->m_RankID)
			{
				char szRankId[5];
				itoa(pNpc->m_RankID, szRankId, 10);
				g_RankTabSetting.GetString(szRankId, "RANKSTR", "", pInfo->Title, sizeof(pInfo->Title));
			}
		}
		break;

		//Ö÷½ÇµÄÁ¢¼´Ê¹ÓÃÎïÆ·ÓëÎä¹¦
		//uParam = (KUiPlayerImmedItemSkill*)pInfo
	case GDI_PLAYER_IMMED_ITEMSKILL:
		if (uParam)
		{
			KUiPlayerImmedItemSkill* pInfo = (KUiPlayerImmedItemSkill*)uParam;
			memset(pInfo, 0, sizeof(KUiPlayerImmedItemSkill));
			pInfo->IMmediaSkill[0].uGenre = CGOG_SKILL_SHORTCUT;
			pInfo->IMmediaSkill[0].uId = Player[CLIENT_PLAYER_INDEX].GetLeftSkill();
			pInfo->IMmediaSkill[1].uGenre = CGOG_SKILL_SHORTCUT;
			pInfo->IMmediaSkill[1].uId = Player[CLIENT_PLAYER_INDEX].GetRightSkill();

			for (INT i = 0; i < MAX_IMMEDIACY_ITEM; i++)
			{
				pInfo->ImmediaItem[i].uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_immediacy].FindItem(i, 0);
				if (pInfo->ImmediaItem[i].uId > 0)
				{
					pInfo->ImmediaItem[i].uGenre = CGOG_ITEM;
				}
				else
				{
					pInfo->ImmediaItem[i].uGenre = CGOG_NOTHING;
				}
			}
		}
		break;

		//Ö÷½ÇËæÉíÐ¯´øµÄÇ®
		//nRet = Ö÷½ÇËæÉíÐ¯´øµÄÇ®
	case GDI_PLAYER_HOLD_MONEY:
		nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);
		break;
		//Ö÷½ÇËæÉíÐ¯´øµÄÎïÆ·µÈ
		//uParam = (KUiObjAtRegion*) pInfo -> KUiObjAtRegion½á¹¹Êý×éµÄÖ¸Õë£¬KUiObjAtRegion
		//				½á¹¹ÓÃÓÚ´æ´¢ÎïÆ·»òÕßÎä¹¦µÄÊý¾Ý¼°Æä·ÅÖÃÇøÓòÎ»ÖÃÐÅÏ¢¡£
		//nParam = pInfoÊý×éÖÐ°üº¬KUiObjAtRegion½á¹¹µÄÊýÄ¿
		//Return = Èç¹û·µ»ØÖµÐ¡ÓÚµÈÓÚ´«Èë²ÎÊýnParam£¬ÆäÖµ±íÊ¾pInfoÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiObjAtRegion
		//			½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý£»·ñÔò±íÊ¾ÐèÒª´«Èë°üº¬¶àÉÙ¸öKUiObjAtRegion½á¹¹µÄÊý×é
		//			²Å¹»´æ´¢È«²¿µÄËæÉíÐ¯´øµÄÎïÆ·ÐÅÏ¢¡£

		// flyingÌí¼ÓÁËÒ»¸öÊÇ·ñ¿ÉÒÔÆïÂíµÄÇëÇó£¬ÔÚÕâÀïÊµÏÖ£¬gg¿ÉÀÖÒ»¹Þ£¬mmÇë³ÔKFCÅ¶
	case GDI_PLAYER_CAN_RIDE:
		// ÊµÏÖ·ÖÖ§µÄ´úÂëÔÚÕâÀïÌí¼Ó
		break;

	case GDI_ITEM_TAKEN_WITH:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			//if (pItem && pItem->nPlace == pos_equiproom)
			//{
			//	pInfo->Obj.uGenre = CGOG_ITEM;
			//	pInfo->Obj.uId = pItem->nIdx;
			//	pInfo->Region.h = pItem->nX;
			//	pInfo->Region.v = pItem->nY;
			//	pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
			//	pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
			//	nCount++;
			//	pInfo++;
			//}
			while (pItem)
			{
				if (pItem && pItem->nPlace == pos_equiproom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;

				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			//if (pItem && pItem->nPlace == pos_equiproom)
			//	nCount++;
			while (pItem)
			{
				if (pItem && pItem->nPlace == pos_equiproom)
					nCount++;

				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
			}
			nRet = nCount;
		}
		break;

		//Ö÷½Ç×°±¸ÎïÆ·
		//uParam = (KUiObjAtRegion*)pInfo -> °üº¬10¸öÔªËØµÄKUiObjAtRegion½á¹¹Êý×éÖ¸Õë£¬
		//			KUiObjAtRegion½á¹¹ÓÃÓÚ´æ´¢×°±¸µÄÊý¾ÝºÍ·ÅÖÃÎ»ÖÃÐÅÏ¢¡£
		//			KUiObjAtRegion::Region::h ±íÊ¾ÊôÓÚµÚ¼¸Ì××°±¸
		//			KUiObjAtRegion::Region::v ±íÊ¾ÊôÓÚÄÄ¸öÎ»ÖÃµÄ×°±¸,ÆäÖµÎªÃ·¾ÙÀàÐÍ
		//			UI_EQUIPMENT_POSITIONµÄÈ¡ÖµÖ®Ò»¡£Çë²Î¿´UI_EQUIPMENT_POSITIONµÄ×¢ÊÍ¡£
		//nParam =	Òª»ñÈ¡µÄÊÇµÚ¼¸Ì××°±¸ÐÅÏ¢
		//Return =  ÆäÖµ±íÊ¾pInfoÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiObjAtRegion½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý¡£
		// new add
			//
	case GDI_EQUIPMENT:
		nRet = 0;
		if (uParam)
		{
			// TODO£ºÔÝÊ±Ã»ÓÐ×öµÚ¶þÌ××°±¸
			if (nParam == 1)
				break;

			INT PartConvert[itempart_num] =
			{
				UIEP_HEAD,		UIEP_BODY,
				UIEP_WAIST,		UIEP_HAND,
				UIEP_FOOT,		UIEP_FINESSE,
				UIEP_NECK,		UIEP_FINGER1,
				UIEP_FINGER2,	UIEP_WAIST_DECOR,
				UIEP_HORSE,		UIEP_MASK,
				UIEP_MANTLE,	UIEP_SIGNET,
				UIEP_SHIPIN,	UIEP_HOODS,
				UIEP_CLOAK,
			};

			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (INT i = 0; i < itempart_num; i++)
			{
				pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(i);
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;

					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartConvert[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;
	case GDI_PARADE_EQUIPMENT:
		nRet = 0;
		if (uParam)
		{
			// TODO£ºÔÝÊ±Ã»ÓÐ×öµÚ¶þÌ××°±¸
			if (nParam == 1)
				break;

			INT PartConvert[itempart_num] =
			{
				UIEP_HEAD,		UIEP_BODY,
				UIEP_WAIST,		UIEP_HAND,
				UIEP_FOOT,		UIEP_FINESSE,
				UIEP_NECK,		UIEP_FINGER1,
				UIEP_FINGER2,	UIEP_WAIST_DECOR,
				UIEP_HORSE,		UIEP_MASK,
				UIEP_MANTLE,	UIEP_SIGNET,
				UIEP_SHIPIN,	UIEP_HOODS,
				UIEP_CLOAK,

			};

			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (INT i = 0; i < itempart_num; i++)
			{
				pInfo->Obj.uId = g_cViewItem.m_sItem[i].nIdx;
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_PLAYERVIEWITEM;

					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartConvert[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;

		//½»Ò×²Ù×÷Ïà¹ØµÄÊý¾Ý
		//uParam = (UI_TRADE_OPER_DATA)eOper ¾ßÌåº¬Òå¼ûUI_TRADE_OPER_DATA
		//nParam ¾ßÌåÓ¦ÓÃÓëº¬ÒåÓÉuParamµÄÈ¡Öµ×´¿ö¾ö¶¨,¼ûUI_TRADE_OPER_DATAµÄËµÃ÷
		//Return ¾ßÌåº¬ÒåÓÉuParamµÄÈ¡Öµ×´¿ö¾ö¶¨,¼ûUI_TRADE_OPER_DATAµÄËµÃ÷
		//Ö÷½ÇµÄÉú»î¼¼ÄÜÊý¾Ý
	case GDI_TRADE_OPER_DATA:
		if (uParam == UTOD_IS_LOCKED)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock;
		else if (uParam == UTOD_IS_TRADING)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeState;
		else if (uParam == UTOD_IS_OTHER_LOCKED)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeDestLock;
		else if (uParam == UTOD_IS_OTHER_TRADING)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeDestState;
		else if (uParam == UTOD_IS_WILLING)
		{
			nRet = (Player[CLIENT_PLAYER_INDEX].m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADEOPEN);
		}
		break;
		//Ö÷½ÇÕÆÎÕµÄ¸÷ÏîÉú»î¼¼ÄÜ
		//uParam = (UINT*) pSkills -> °üº¬10¸öunsigned intµÄÊý×éÓÃÓÚ´æ´¢¸÷ÏîÉú»î¼¼ÄÜµÄid¡£

		//Ê£ÓàÕ½¶·¼¼ÄÜµãÊý
		//Return = Ê£ÓàÕ½¶·¼¼ÄÜµãÊý
	case GDI_FIGHT_SKILL_POINT:
		//to do:no implements in this version;
		nRet = Player[CLIENT_PLAYER_INDEX].m_nSkillPoint;
		break;

		//Ö÷½ÇÕÆÎÕµÄ¸÷ÏîÕ½¶·¼¼ÄÜ
		//uParam = (UINT*) pSkills -> °üº¬30¸öunsigned intµÄÊý×éÓÃÓÚ´æ´¢¸÷ÏîÕ½¶·¼¼ÄÜµÄid¡£
	case GDI_FIGHT_SKILLS:
		if (uParam)
		{
			KUiSkillData* pSkills = (KUiSkillData*)uParam;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillSortList(pSkills);
		}
		break;
		//ÏÔÊ¾×ó¼ü¼¼ÄÜÁÐ±í
		//uParam = (KUiSkillData*) pSkills -> °üº¬65¸öKUiSkillDataµÄÊý×éÓÃÓÚ´æ´¢¸÷¼¼ÄÜµÄÊý¾Ý¡£
		//								KUiSkillData::nLevelÓÃÀ´±íÊö¼¼ÄÜÏÔÊ¾ÔÚµÚ¼¸ÐÐ
		//Return = ·µ»ØÓÐÐ§Êý¾ÝµÄSkillsµÄÊýÄ¿
	case GDI_LEFT_ENABLE_SKILLS:
	{
		KUiSkillData* pSkills = (KUiSkillData*)uParam;
		INT nCount = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLeftSkillSortList(pSkills);
		return nCount;

	}
	break;
	//ÏÔÊ¾ÓÒ¼ü¼¼ÄÜÁÐ±í
	//uParam = (KUiSkillData*) pSkills -> °üº¬65¸öKUiSkillDataµÄÊý×éÓÃÓÚ´æ´¢¸÷¼¼ÄÜµÄÊý¾Ý¡£
	//								KUiSkillData::nLevelÓÃÀ´±íÊö¼¼ÄÜÏÔÊ¾ÔÚµÚ¼¸ÐÐ
	//Return = ·µ»ØÓÐÐ§Êý¾ÝµÄSkillsµÄÊýÄ¿
	case GDI_RIGHT_ENABLE_SKILLS:
	{
		KUiSkillData* pSkills = (KUiSkillData*)uParam;
		INT nCount = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetRightSkillSortList(pSkills);
		return nCount;
	}
	break;
	//Ö÷½ÇµÄ×Ô´´Îä¹¦
	//uParam = (UINT*) pSkills -> °üº¬5¸öunsigned intµÄÊý×éÓÃÓÚ´æ´¢¸÷Ïî×Ô´´Îä¹¦µÄid¡£
	case GDI_CUSTOM_SKILLS:
		if (uParam)
		{
			//to do: no implements in this version;
			KUiSkillData* pSkills = (KUiSkillData*)uParam;
			memset(pSkills, 0, sizeof(KUiSkillData) * 5);
		}
		break;
		//»ñÈ¡ÖÜÎ§Íæ¼ÒµÄÁÐ±í
		//uParam = (KUiPlayerItem*)pList -> ÈËÔ±ÐÅÏ¢ÁÐ±í
		//			KUiPlayerItem::nData = 0
		//nParam = pListÊý×éÖÐ°üº¬KUiPlayerItem½á¹¹µÄÊýÄ¿
		//Return = Èç¹û·µ»ØÖµÐ¡ÓÚµÈÓÚ´«Èë²ÎÊýnParam£¬ÆäÖµ±íÊ¾pListÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiPlayerItem
		//			½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý£»·ñÔò±íÊ¾ÐèÒª´«Èë°üº¬¶àÉÙ¸öKUiPlayerItem½á¹¹µÄÊý×é
		//			²Å¹»´æ´¢È«²¿ÈËÔ±ÐÅÏ¢¡£
	case GDI_NEARBY_PLAYER_LIST:
		nRet = NpcSet.GetAroundPlayer((KUiPlayerItem*)uParam, nParam);
		break;

		//»ñÈ¡ÖÜÎ§¹Âµ¥¿ÉÊÜÑûÇëµÄÍæ¼ÒµÄÁÐ±í
		//²ÎÊýº¬ÒåÍ¬GDI_NEARBY_PLAYER_LIST
	case GDI_NEARBY_IDLE_PLAYER_LIST:
		nRet = NpcSet.GetAroundPlayerForTeamInvite((KUiPlayerItem*)uParam, nParam);
		break;

		//Ö÷½ÇÍ³Ë§ÄÜÁ¦Ïà¹ØµÄÊý¾Ý
		//uParam = (KUiPlayerLeaderShip*) -> Ö÷½ÇÍ³Ë§ÄÜÁ¦Ïà¹ØµÄÊý¾Ý½á¹¹Ö¸Õë
	case GDI_PLAYER_LEADERSHIP:
		if (uParam)
		{
			KUiPlayerLeaderShip* pInfo = (KUiPlayerLeaderShip*)uParam;
			pInfo->nLeaderShipExperience = Player[CLIENT_PLAYER_INDEX].m_nLeadExp;		//Í³Ë§Á¦¾­ÑéÖµ
			//to do: waiting for...;
			pInfo->nLeaderShipExperienceFull = Player[CLIENT_PLAYER_INDEX].m_nNextLevelLeadExp;//Í³Ë§Á¦¾­ÑéÖµ
			pInfo->nLeaderShipLevel = Player[CLIENT_PLAYER_INDEX].m_nLeadLevel;			//Í³Ë§Á¦µÈ¼¶
		}
		break;
		//»ñµÃÎïÆ·ÔÚÄ³¸ö»·¾³Î»ÖÃµÄÊôÐÔ×´Ì¬
		//uParam = (KUiGameObject*)pObj£¨µ±nParam==0Ê±£©ÎïÆ·µÄÐÅÏ¢
		//uParam = (KUiObjAtContRegion*)pObj£¨µ±nParam!=0Ê±£©ÎïÆ·µÄÐÅÏ¢
		//nParam = (INT)(bool)bJustTry  ÊÇ·ñÖ»ÊÇ³¢ÊÔ·ÅÖÃ
		//Return = (ITEM_IN_ENVIRO_PROP)eProp ÎïÆ·µÄÊôÐÔ×´Ì¬
	case GDI_ITEM_IN_ENVIRO_PROP:
	{
		if (!nParam)
		{
			KUiGameObject* pObj = (KUiGameObject*)uParam;
			if (pObj->uGenre != CGOG_ITEM && pObj->uGenre != CGOG_NPCSELLITEM && pObj->uGenre != CGOG_PLAYERSELLITEM && pObj->uGenre != CGOG_PLAYERVIEWITEM)
				break;

			KItem* pItem = NULL;

			if (pObj->uGenre == CGOG_ITEM || pObj->uGenre == CGOG_PLAYERSELLITEM || pObj->uGenre == CGOG_PLAYERVIEWITEM && pObj->uId > 0 && pObj->uId < MAX_ITEM)
			{
				pItem = &Item[pObj->uId];
			}
			else if (pObj->uGenre == CGOG_NPCSELLITEM)
			{
				INT nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop], pObj->uId);
				pItem = BuySell.GetItem(nIdx);
			}
			_ASSERT(pItem);

			if (!pItem)
				break;

			if (pItem->GetGenre() != item_equip)
			{
				//nRet = IIEP_SHOP;
				break;
			}

			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CanEquip(pItem))
			{
				nRet = IIEP_NORMAL;
			}
			else
			{
				nRet = IIEP_NOT_USEABLE;
			}
		}
		else if (nParam == 0)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion*)uParam;
			if (pObj->Obj.uGenre != CGOG_ITEM && pObj->Obj.uGenre != CGOG_NPCSELLITEM && pObj->Obj.uGenre != CGOG_PLAYERSELLITEM && pObj->Obj.uGenre != CGOG_PLAYERVIEWITEM && pObj->Obj.uId >= MAX_ITEM)
				break;

			INT PartConvert[itempart_num] =
			{
				itempart_head,		itempart_weapon,
				itempart_amulet,	itempart_cuff,
				itempart_body,		itempart_belt,
				itempart_ring1,		itempart_ring2,
				itempart_pendant,	itempart_foot,
				itempart_horse,		itempart_mask,
				itempart_mantle,	itempart_signet,
				itempart_shipin,	itempart_hoods,
				itempart_cloak,
			};

			_ASSERT(pObj->eContainer < itempart_num);
			if (pObj->eContainer >= itempart_num || pObj->eContainer < itempart_head)
				break;

			if (Item[pObj->Obj.uId].GetGenre() != item_equip)
				break;

			INT nPlace = PartConvert[pObj->eContainer];
			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CanEquip(pObj->Obj.uId, nPlace))
			{
				nRet = IIEP_NORMAL;
			}
			else
			{
				nRet = IIEP_NOT_USEABLE;
			}
		}
	}
	break;
	case GDI_ITEMDATAINFO:
	{
		if (uParam <= 0 || uParam >= MAX_ITEM)
			nRet = -1;
		else if (nParam == 2)
			nRet = Item[uParam].GetGenre();
		else if (nParam == 3)
			nRet = Item[uParam].IsStack();
		else if (nParam == 4)
			nRet = Item[uParam].GetVersion();
		else
			nRet = -1;
	}
	break;
	case GDI_IMMEDIATEITEM_NUM:
		if (uParam >= 0 && uParam < IMMEDIACY_ROOM_WIDTH)
			nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetSameDetailItemNum(uParam);
		break;
		//ÓëNPCµÄÂòÂô
		//uParam = (KUiObjAtContRegion*) pInfo -> KUiObjAtRegion½á¹¹Êý×éµÄÖ¸Õë£¬KUiObjAtRegion
		//				½á¹¹ÓÃÓÚ´æ´¢ÎïÆ·µÄÊý¾Ý¼°Æä·ÅÖÃÇøÓòÎ»ÖÃÐÅÏ¢¡£
		//				ÆäÖÐKUiObjAtContRegion::nContainerÖµ±íÊ¾µÚ¼¸Ò³µÄÎïÆ·
		//nParam = pInfoÊý×éÖÐ°üº¬KUiObjAtRegion½á¹¹µÄÊýÄ¿
		//Return = Èç¹û·µ»ØÖµÐ¡ÓÚµÈÓÚ´«Èë²ÎÊýnParam£¬ÆäÖµ±íÊ¾pInfoÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiObjAtRegion
		//			½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý£»·ñÔò±íÊ¾ÐèÒª´«Èë°üº¬¶àÉÙ¸öKUiObjAtRegion½á¹¹µÄÊý×é
		//			²Å¹»´æ´¢È«²¿µÄnpcÁÐ³öÀ´½»Ò×µÄÎïÆ·ÐÅÏ¢¡£
	case GDI_TRADE_NPC_ITEM:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			INT nPage = 0;
			INT nIndex = 0;
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion*)uParam;
			INT	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop];

			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			if (!BuySell.m_pShopRoom)
				break;
			BuySell.m_pShopRoom->Clear();
			for (INT i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);

				if (nIndex >= 0 && pItem)
				{
					// Set pInfo->Obj.uGenre
					pInfo->Obj.uGenre = CGOG_NPCSELLITEM;
					// Set pInfo->Obj.uId
					pInfo->Obj.uId = i;

					POINT	Pos;
					if (BuySell.m_pShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos))
					{
						// nIndex + 1±£Ö¤²»Îª0
						BuySell.m_pShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
					}
					else
					{
						nPage++;
						BuySell.m_pShopRoom->Clear();
						// ClearÍê³Éºó±ØÈ»³É¹¦£¬ËùÒÔÃ»ÓÐÅÐ¶Ï
						BuySell.m_pShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos);
						BuySell.m_pShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
					}
					pInfo->Region.h = Pos.x;
					pInfo->Region.v = Pos.y;
					pInfo->Region.Width = pItem->GetWidth();
					pInfo->Region.Height = pItem->GetHeight();
					pInfo->nContainer = nPage;
					nCount++;
					pInfo++;
				}
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			INT nIndex = 0;
			INT	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop];
			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			for (INT i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);

				if (nIndex >= 0 && pItem)
				{
					nCount++;
				}
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom)
				nCount++;
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX1:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom1)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom1)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom1)
				nCount++;
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom1)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX2:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom2)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom2)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom2)
				nCount++;
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom2)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX3:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom3)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom3)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom3)
				nCount++;
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom3)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX4:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom4)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom4)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom4)
				nCount++;
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom4)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_STORE_BOX5:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom5)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom5)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom5)
				nCount++;
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom5)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_EXPANDITEM:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipmentex);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproomex)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproomex)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproomex)
				nCount++;
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproomex)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_PK_SETTING:					//»ñÈ¡pkÉèÖÃ
		nRet = Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState();
		break;
	case GDI_SHOW_PLAYERS_NAME:			//»ñÈ¡ÏÔÊ¾¸÷Íæ¼ÒÈËÃû
		nRet = NpcSet.CheckShowName();
		break;
	case GDI_SHOW_PLAYERS_LIFE:			//»ñÈ¡ÏÔÊ¾¸÷Íæ¼ÒÉúÃü
		nRet = NpcSet.CheckShowLife();
		break;
	case GDI_ITEM_GIVE:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_give)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_give)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;

			//			nCount++;

			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_give)
			{
				nCount++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_give)
				{
					nCount++;
				}
			}
			nRet = nCount;
		}
		break;
	case GDI_EXBOX_NUM:
		nRet = Player[CLIENT_PLAYER_INDEX].m_btRepositoryNum;
		break;
	case GDI_EQUIPEX_TIME:
		nRet = Player[CLIENT_PLAYER_INDEX].m_dwEquipExpandTime - KSG_GetCurSec();
		break;
	case GDI_ITEM_NAME:
		if (uParam)
		{
			char* pzName = (char*)uParam;
			if (nParam)
				strcpy(pzName, Item[nParam].GetName());
		}
		break;
	case GDI_ITEM_QUALITY:
		if (uParam)
		{
			nRet = Item[uParam].GetQuality();
		}
		break;
	case GDI_ITEM_DESC:
		if (nParam && uParam)
		{
			char* pszDescript = (char*)uParam;
			pszDescript[0] = 0;
			Item[nParam].GetDesc(pszDescript);
		}
		break;
	case GDI_GET_ITEM_PARAM:
		if (uParam && nParam)
		{
			char* CIParam = (char*)uParam;

			char Buffer[16];
			INT nOffset = 0;
			itoa(Item[nParam].GetNature(), Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].GetGenre(), Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			if (Item[nParam].GetNature() == NATURE_GOLD || Item[nParam].GetNature() == NATURE_PLATINA)
				itoa(Item[nParam].GetRow(), Buffer, 10);
			else
				itoa(Item[nParam].GetDetailType(), Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].GetParticular(), Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].GetSeries(), Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].GetLevel(), Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].m_GeneratorParam.nLuck, Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].m_GeneratorParam.uRandomSeed, Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].m_GeneratorParam.nVersion, Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].GetDurability(), Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			itoa(Item[nParam].GetExpireTime(), Buffer, 10);
			strcat(&CIParam[nOffset], Buffer);
			nOffset += strlen(Buffer);
			CIParam[nOffset] = ',';
			nOffset++;
			for (INT j = 0; j < MAX_ITEM_MAGICLEVEL; j++)
			{
				sprintf(Buffer, "%d", Item[nParam].m_GeneratorParam.nGeneratorLevel[j]);
				strcat(&CIParam[nOffset], Buffer);
				nOffset += strlen(Buffer);
				if (j == MAX_ITEM_MAGICLEVEL - 1)
					break;
				CIParam[nOffset] = ',';
				nOffset++;
			}
			CIParam[nOffset] = '\0';
			nRet = 1;
		}
		break;
	case GDI_ITEM_CHAT:
		if (uParam)
		{
			if (nParam)
			{
				char* CIParam = (char*)nParam;
				INT CIParamV[11], nMagicParam[MAX_ITEM_MAGICLEVEL];
				char* cParam = strtok(CIParam, ",");
				INT i = 0, j = 0;
				while (cParam)
				{
					if (i >= 10 + MAX_ITEM_MAGICLEVEL)
						break;
					else if (i >= 11)
					{
						if (j >= MAX_ITEM_MAGICLEVEL)break;
						nMagicParam[j] = atoi(cParam);
						j++;
					}
					else
						CIParamV[i] = atoi(cParam);
					cParam = strtok(NULL, ",");
					i++;
				}
				nRet = ItemSet.Add(
					CIParamV[0],
					CIParamV[1],
					CIParamV[4],
					CIParamV[5],
					CIParamV[6],
					CIParamV[2],
					CIParamV[3],
					nMagicParam,
					CIParamV[8],
					CIParamV[7]);
				if (nRet)
				{
					Item[nRet].SetDurability(CIParamV[9]);
					Item[nRet].SetExpireTime(CIParamV[10]);
				}
			}
		}
		else
			ItemSet.Remove((INT)nParam);
		break;
	case GDI_SHOW_OBJ_NAME:
		nRet = NpcSet.CheckShowObj();
		break;
	case GDI_TRADE_PLAYER_ITEM:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion*)uParam;

			for (INT i = 0; i < MAX_EQUIPMENT_ITEM; i++)
			{
				if (g_cSellItem.m_sItem[i].nIdx)
				{
					pInfo->Obj.uGenre = CGOG_PLAYERSELLITEM;
					pInfo->Obj.uId = g_cSellItem.m_sItem[i].nIdx;

					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = g_cSellItem.m_sItem[i].nX;
					pInfo->Region.v = g_cSellItem.m_sItem[i].nY;
					nCount++;
					pInfo++;
				}
			}

			nRet = nCount;
		}
		else
		{
			nRet = g_cSellItem.GetCount();
		}
		break;
	case GDI_COMPOUND:
		nRet = 0;
		if (uParam)
		{
			// TODO£ºÔÝÊ±Ã»ÓÐ×öµÚ¶þÌ××°±¸
			if (nParam == 1)
				break;

			INT PartCompoundConvert[compound_num] =
			{
				MOSAICENCRUSTED_UIEP_BOX_1,
				MOSAICENCRUSTED_UIEP_BOX_2,
				MOSAICENCRUSTED_UIEP_BOX_3,
			};

			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (INT i = 0; i < MAX_COMPOUND_ITEM; i++)
			{
				pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetCompoundItem(i);
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;

					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartCompoundConvert[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;
		//TamLTM kham nam
	case GDI_BUILD_ITEM:
		nRet = 0;
		if (uParam)
		{
			if (nParam == 1)
				break;

			INT PartBuildItem[MAX_PART_BUILD] =
			{
				UIEP_BUILDITEM1,
				UIEP_BUILDITEM2,
				UIEP_BUILDITEM3,
				UIEP_BUILDITEM4,
				UIEP_BUILDITEM5,
				UIEP_BUILDITEM6,
				UIEP_BUILDITEM7,
				UIEP_BUILDITEM8,
				UIEP_BUILDITEM9,
			};

			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (INT i = 0; i < MAX_PART_BUILD; i++)
			{
				pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetBuildItem(i);
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;

					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartBuildItem[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;
		//End code
	case GDI_COMPOUND_BOX:
		nRet = 0;
		if (uParam)
		{
			INT nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_compoundroom)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_compoundroom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			INT nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_compoundroom)
				nCount++;
			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_compoundroom)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_LOCKMOVE:
		nRet = Player[CLIENT_PLAYER_INDEX].GetLockMove()->bLock;
		break;
	case GDI_NPC_STATE_SKILL:
		nRet = 0;
		if (uParam)
		{
			KStateNode* pNode;
			pNode = (KStateNode*)Npc[CLIENT_PLAYER_INDEX].m_StateSkillList.GetHead();
			KStateControl* pInfo = (KStateControl*)uParam;

			while (pNode)
			{
				if (pNode->m_SkillID > 0 && pNode->m_SkillID < MAX_SKILL && pNode->m_Level > 0 && pNode->m_Level < MAX_SKILLLEVEL)
				{
					KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(pNode->m_SkillID, pNode->m_Level);
					if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles ||
						pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState)
					{
						pInfo->nSkillId = pNode->m_SkillID;
						pInfo->nLeftTime = pNode->m_LeftTime;
						pInfo++;
					}
				}
				pNode = (KStateNode*)pNode->GetNext();
			}
		}
		else
		{
			INT nCount = 0;
			KStateNode* pNode;
			pNode = (KStateNode*)Npc[CLIENT_PLAYER_INDEX].m_StateSkillList.GetHead();
			while (pNode)
			{
				if (pNode->m_SkillID > 0 && pNode->m_SkillID < MAX_SKILL && pNode->m_Level > 0 && pNode->m_Level < MAX_SKILLLEVEL)
				{
					KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(pNode->m_SkillID, pNode->m_Level);
					if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles ||
						pOrdinSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState)
					{
						nCount++;
					}
				}
				pNode = (KStateNode*)pNode->GetNext();
			}
			nRet = nCount;
		}
		break;
	case GDI_MISSION_SELFDATA:
		if (uParam)
		{
			TMissionLadderSelfInfo* pRankData = (TMissionLadderSelfInfo*)uParam;
			*pRankData = Player[CLIENT_PLAYER_INDEX].m_MissionData;
		}
		break;
	case GDI_MISSION_RANKDATA:
		if (uParam)
		{
			TMissionLadderInfo* pRankData = (TMissionLadderInfo*)uParam;
			*pRankData = Player[CLIENT_PLAYER_INDEX].m_MissionRank[nParam];
		}
		break;
	case GDI_CHATROOM_NAME:
		if (uParam)
		{
			char* pRoomName = (char*)uParam;
			strcpy(pRoomName, Player[CLIENT_PLAYER_INDEX].m_cRoom.m_szRoomName);
		}
		break;

	case GDI_IS_CHECK_IMAGE:			//Kiem tra hinh anh nguoi choi da chon
		nRet = Player[CLIENT_PLAYER_INDEX].m_ImagePlayer;
		break;
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏòÓÎÏ··¢ËÍ²Ù×÷
//	²ÎÊý£ºunsigned INT uDataId --> CoreÍâ²¿¿Í»§¶ÔcoreµÄ²Ù×÷ÇëÇóµÄË÷Òý¶¨Òå
//							ÆäÖµÎªÃ·¾ÙÀàÐÍGAMEOPERATION_INDEXµÄÈ¡ÖµÖ®Ò»¡£
//		  UINT uParam  --> ÒÀ¾ÝuOperIdµÄÈ¡ÖµÇé¿ö¶ø¶¨
//		  INT nParam --> ÒÀ¾ÝuOperIdµÄÈ¡ÖµÇé¿ö¶ø¶¨
//	·µ»Ø£ºÈç¹û³É¹¦·¢ËÍ²Ù×÷ÇëÇó£¬º¯Êý·µ»Ø·Ç0Öµ£¬·ñÔò·µ»Ø0Öµ¡£
//--------------------------------------------------------------------------
INT	KCoreShell::OperationRequest(UINT uOper, UINT uParam, INT nParam)
{
	INT nRet = 1;
	switch (uOper)
	{
	case GOI_QUERY_RANK_INFORMATION:
		SendClientCmdQueryLadder(uParam);
		break;
		//uParam = (const char*)pszFileName
	case GOI_PLAY_SOUND:
		if (uParam)
		{
			static KCacheNode* pSndNode = NULL;
			KWavSound* pSound = NULL;
			pSndNode = (KCacheNode*)g_SoundCache.GetNode((char*)uParam, (KCacheNode*)pSndNode);
			pSound = (KWavSound*)pSndNode->m_lpData;
			if (pSound)
			{
				if (pSound->IsPlaying())
					break;
				pSound->Play(0, -10000 + Option.GetSndVolume() * 100, 0);
			}
		}
		break;
	case GOI_PLAYER_RENASCENCE:
	{
		INT nReviveType;
		if (nParam)	// bBackTown
		{
			nReviveType = REMOTE_REVIVE_TYPE;
		}
		else
		{
			nReviveType = LOCAL_REVIVE_TYPE;
		}
		//			SendClientCmdRevive(nReviveType);
		SendClientCmdRevive();
	}
	break;
	case GOI_MONEY_INOUT_STORE_BOX:
	{
		BOOL	bIn = (BOOL)uParam;
		INT		nMoney = nParam;
		INT		nSrcRoom, nDesRoom;


		if (bIn)
		{
			nSrcRoom = room_equipment;
			nDesRoom = room_repository;
		}
		else
		{
			nDesRoom = room_equipment;
			nSrcRoom = room_repository;
		}
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeMoney(nSrcRoom, nDesRoom, nMoney);
	}
	break;
	//Àë¿ªÓÎÏ·
	case GOI_EXIT_GAME:
		g_SubWorldSet.Close();
		g_ScenePlace.ClosePlace();
		break;
	case GOI_PLAYER_OFFLINE:
		g_SubWorldSet.Offline();
		g_ScenePlace.ClosePlace();
		break;
	case GOI_GAMESPACE_DISCONNECTED:
		g_SubWorldSet.Close();
		break;
	case GOI_TRADE_NPC_BUY:
	{
		KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
		if (pObject1->Obj.uGenre != CGOG_NPCSELLITEM)
			break;

		INT nIdx = 0;
		if (nParam <= 0)
			break;

		SendClientCmdBuy(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop, pObject1->Obj.uId, nParam);
	}
	break;
	case GOI_TRADE_NPC_SELL:
	{
		KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;

		if (CGOG_ITEM != pObject1->Obj.uGenre)
			break;
		//·ÅÏÂÈ¥µÄ¶«Î÷²»Îª¿Õ£¬ËùÒÔÊÇÂô¶«Î÷
		INT nIdx = pObject1->Obj.uId;	//Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
		if (nIdx > 0 && nIdx < MAX_ITEM)
		{
			SendClientCmdSell(Item[nIdx].GetID(), nParam);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	break;
	case GOI_TRADE_NPC_REPAIR:
	{
		KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;

		if (CGOG_ITEM != pObject1->Obj.uGenre)
			break;
		//·ÅÏÂÈ¥µÄ¶«Î÷²»Îª¿Õ£¬ËùÒÔÊÇÂô¶«Î÷
		INT nIdx = pObject1->Obj.uId;	//Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
		if (nIdx > 0 && nIdx < MAX_ITEM)
		{
			if (Item[nIdx].GetGenre() != item_equip)
				return 0;
			else if (Item[nIdx].GetDurability() == -1 || Item[nIdx].GetDurability() >= Item[nIdx].GetMaxDurability())
			{
				return 0;
			}
			else if (Item[nIdx].GetRepairPrice() <= Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney())
			{
				SendClientCmdRepair(Item[nIdx].GetID());
			}
			else
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (UINT)&sMsg, 0);
				return 0;
			}

			return 1;
		}
		else
		{
			return 0;
		}
	}
	break;

	case GOI_SWITCH_OBJECT:
		//uParam = (KUiObjAtContRegion*)pObject1 -> ÄÃÆðµÄÎïÆ·²Ù×÷Ç°µÄÐÅÏ¢
		//nParam = (KUiObjAtContRegion*)pObject2 -> ·ÅÏÂµÄÎïÆ·²Ù×÷ºóµÄÐÅÏ¢
	{
		ItemPos	P1, P2 = {};
		INT PartConvert[itempart_num] =
		{
			itempart_head,		itempart_weapon,
			itempart_amulet,	itempart_cuff,
			itempart_body,		itempart_belt,
			itempart_ring1,		itempart_ring2,
			itempart_pendant,	itempart_foot,
			itempart_horse,		itempart_mask,
			itempart_mantle,	itempart_signet,
			itempart_shipin,	itempart_hoods,
			itempart_cloak,
		};

		//TamLTM kham nam
		INT PartBuildItem[MAX_PART_BUILD] =
		{
			itempart_build1,	itempart_build2,
			itempart_build3,	itempart_build4,
			itempart_build5,	itempart_build6,
			itempart_build7,	itempart_build8,
			itempart_build9,
		};
		//End code

		INT PartCompoundConvert[MAX_COMPOUND_ITEM] =
		{
			compound_box1,	compound_box2,
			compound_box3,
		};
		KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
		KUiObjAtContRegion* pObject2 = (KUiObjAtContRegion*)nParam;

		if (!pObject1 && !pObject2)
			break;

		if (pObject1)
		{
			switch (pObject1->eContainer)
			{
			case UOC_STORE_BOX:
				P1.nPlace = pos_repositoryroom;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_STORE_BOX1:
				P1.nPlace = pos_repositoryroom1;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_STORE_BOX2:
				P1.nPlace = pos_repositoryroom2;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_STORE_BOX3:
				P1.nPlace = pos_repositoryroom3;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_STORE_BOX4:
				P1.nPlace = pos_repositoryroom4;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_STORE_BOX5:
				P1.nPlace = pos_repositoryroom5;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_IMMEDIA_ITEM:
				P1.nPlace = pos_immediacy;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_ITEM_TAKE_WITH:
				P1.nPlace = pos_equiproom;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_ITEM_TAKE_WITH_EX:
				P1.nPlace = pos_equiproomex;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_EQUIPTMENT:
			{
				// TODO:ÔÝÊ±Ã»ÓÐµÚ¶þ×°±¸
				if (pObject1->Region.h == 1)
					break;
				P1.nPlace = pos_equip;
				P1.nX = PartConvert[pObject1->Region.v];
			}
			break;
			case UOC_TO_BE_TRADE:
				P1.nPlace = pos_traderoom;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_ITEM_GIVE:
				P1.nPlace = pos_give;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_COMPOUND:
			{
				if (pObject1->Region.h == 1)
					break;
				P1.nPlace = pos_compound;
				P1.nX = PartCompoundConvert[pObject1->Region.v];
			}
			break;
			//TamLTM kham nam xanh
			case UOC_BUILD_ITEM:
			{
				//g_DebugLog("Region.h: %d", pObject1->Region.h);
				if (pObject1->Region.h == 1)
					break;
				P1.nPlace = pos_builditem;
				P1.nX = PartBuildItem[pObject1->Region.v];
				//g_DebugLog("P1.nX: %d", pObject1->Region.h);
			}
			break;
			//End code
			case UOC_COMPOUND_BOX:
				P1.nPlace = pos_compoundroom;
				P1.nX = pObject1->Region.h;
				P1.nY = pObject1->Region.v;
				break;
			case UOC_NPC_SHOP:
				if (CGOG_NPCSELLITEM != pObject1->Obj.uGenre)
					break;

				SendClientCmdBuy(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop, pObject1->Obj.uId, nParam);
				break;
			}
		}

		if (pObject2)
		{
			switch (pObject2->eContainer)
			{
			case UOC_STORE_BOX:
				P2.nPlace = pos_repositoryroom;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_STORE_BOX1:
				P2.nPlace = pos_repositoryroom1;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_STORE_BOX2:
				P2.nPlace = pos_repositoryroom2;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_STORE_BOX3:
				P2.nPlace = pos_repositoryroom3;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_STORE_BOX4:
				P2.nPlace = pos_repositoryroom4;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_STORE_BOX5:
				P2.nPlace = pos_repositoryroom5;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_IMMEDIA_ITEM:
				P2.nPlace = pos_immediacy;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_ITEM_TAKE_WITH:
				P2.nPlace = pos_equiproom;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_ITEM_TAKE_WITH_EX:
				P2.nPlace = pos_equiproomex;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_EQUIPTMENT:
			{
				// TODO:ÔÝÊ±Ã»ÓÐµÚ¶þ×°±¸
				if (pObject2->Region.h == 1)
					break;
				P2.nPlace = pos_equip;
				P2.nX = PartConvert[pObject2->Region.v];
			}
			break;
			case UOC_TO_BE_TRADE:
				P2.nPlace = pos_traderoom;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_ITEM_GIVE:
				P2.nPlace = pos_give;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_COMPOUND:
			{
				if (pObject2->Region.h == 1)
					break;
				P2.nPlace = pos_compound;
				P2.nX = PartCompoundConvert[pObject2->Region.v];
			}
			break;
			//TamLTM code kham nam xanh
			case UOC_BUILD_ITEM:
			{
				// TODO:check doi tuong region chieu cao cua khung box
				if (pObject2->Region.h == 1)
					break;
				P2.nPlace = pos_builditem;
				P2.nX = PartBuildItem[pObject2->Region.v];
			}
			break;
			//End code
			case UOC_COMPOUND_BOX:
				P2.nPlace = pos_compoundroom;
				P2.nX = pObject2->Region.h;
				P2.nY = pObject2->Region.v;
				break;
			case UOC_NPC_SHOP:
				break;
			}
			if (Player[CLIENT_PLAYER_INDEX].GetLockMove()->bLock &&
				P2.nPlace == Item[pObject2->Obj.uId].GetBackLocal()->nPlace)
			{
				P2.nPlace = Item[pObject2->Obj.uId].GetBackLocal()->nPlace;
				P2.nX = Item[pObject2->Obj.uId].GetBackLocal()->nX;
				P2.nY = Item[pObject2->Obj.uId].GetBackLocal()->nY;
			}
		}
		if (!pObject1)
		{
			memcpy(&P1, &P2, sizeof(P1));
		}
		if (!pObject2)
		{
			memcpy(&P2, &P1, sizeof(P1));
		}
		Player[CLIENT_PLAYER_INDEX].MoveItem(P1, P2);
	}
	break;

	//Íæ¼ÒµãÍê¶Ô»°¿ò
	case GOI_INFORMATION_CONFIRM_NOTIFY:
	{
		PLAYER_SELECTUI_COMMAND command;
		command.nSelectIndex = nParam;
		command.nSelectUi = SELECT_TALKUI;
		Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_TALKDIALOG);
		break;
	}

	//ÎÊÌâÑ¡Ôñ´ð°¸
	//nParma = nAnswerIndex
	case GOI_QUESTION_CHOOSE:
		if (g_bUISelLastSelCount == 0)
			break;
		{
			PLAYER_SELECTUI_COMMAND command;
			command.nSelectIndex = nParam;
			command.nSelectUi = SELECT_SELECTUI;
			Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_SELECTDIALOG);
		}
		break;
		//ÉèÖÃÁ¢¼´¼¼ÄÜ
		//uParam = (KUiGameObject*)pSKill, ¼¼ÄÜÐÅÏ¢
		//nParam = Á¢¼´Î»ÖÃ£¬0±íÊ¾ÎªÓÒ¼ü¼¼ÄÜ£¬1ÖÁ4±íÊ¾ÎªF1ÖÁF4¼¼ÄÜ
		//Ê¹ÓÃÎïÆ·
		//uParam = (KUiObjAtRegion*)pInfo -> ÎïÆ·µÄÊý¾ÝÒÔ¼°ÎïÆ·Ô­À´°Ú·ÅµÄÎ»ÖÃ
		//nParam = ÎïÆ·Ê¹ÓÃÇ°·ÅÖÃµÄÎ»ÖÃ£¬È¡ÖµÎªÃ¶¾ÙÀàÐÍUIOBJECT_CONTAINER¡£
	case GOI_USE_ITEM:
		//to do: waiting for...
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			INT nPlace = nParam;
			ItemPos	Pos;
			switch (nPlace)
			{
			case UOC_ITEM_TAKE_WITH:
				Pos.nPlace = pos_equiproom;
				break;
			case UOC_ITEM_TAKE_WITH_EX:
				Pos.nPlace = pos_equiproomex;
				break;
			case UOC_IMMEDIA_ITEM:
				Pos.nPlace = pos_immediacy;
				break;
			default:
				Pos.nPlace = -1;
				break;
			}
			Pos.nX = pInfo->Region.h;
			Pos.nY = pInfo->Region.v;
			if ((pInfo->Obj.uGenre == CGOG_ITEM || pInfo->Obj.uGenre == CGOG_IME_ITEM) && pInfo->Obj.uId > 0 && Pos.nPlace != -1)
				Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pInfo->Obj.uId, Pos);
		}
		break;
	case GOI_SET_IMMDIA_SKILL:
		if (uParam)
		{
			KUiGameObject* pSkill = (KUiGameObject*)uParam;
			if (nParam == 0)
				//to do : modify;
			{
				if ((INT)pSkill->uId > 0)
					Player[CLIENT_PLAYER_INDEX].SetLeftSkill((INT)pSkill->uId);
				else if ((INT)(pSkill->uId) == -1) //ÉèÖÃµ±Ç°ÎïÀí¼¼ÄÜÎª×ó¼¼ÄÜ
				{
					INT nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
					INT nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();

					//½üÉíÎäÆ÷
					if (nDetailType == 0)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nMeleeWeaponSkill[nParticularType]);
					}//Ô¶³ÌÎäÆ÷
					else if (nDetailType == 1)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nRangeWeaponSkill[nParticularType]);
					}//¿ÕÊÖ
					else if (nDetailType == -1)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nHandSkill);
					}

				}
			}
			else if (nParam == 1)
			{
				if ((INT)pSkill->uId > 0)
					Player[CLIENT_PLAYER_INDEX].SetRightSkill((INT)pSkill->uId);
				else if ((INT)(pSkill->uId) == -1)
				{
					INT nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
					INT nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();

					if (nDetailType == 0)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nMeleeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == 1)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nRangeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == -1)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nHandSkill);
					}
				}
			}
		}
		break;

		//ÔöÇ¿Ò»ÖÖ¼¼ÄÜ£¬£¬Ò»´Î¼ÓÒ»µã
		//uParam = ¼¼ÄÜÀàÊô
		//nParam = (uint)¼¼ÄÜid
	case GOI_TONE_UP_SKILL:
		Player[CLIENT_PLAYER_INDEX].ApplyAddSkillLevel((INT)nParam, 1);
		break;

		//ÔöÇ¿Ò»Ð©ÊôÐÔµÄÖµ£¬Ò»´Î¼ÓÒ»µã
		//uParam = ±íÊ¾ÒªÔöÇ¿µÄÊÇÄÄ¸öÊôÐÔ£¬È¡ÖµÎªUI_PLAYER_ATTRIBUTEµÄÃ·¾ÙÖµÖ®Ò»
	case GOI_TONE_UP_ATTRIBUTE:
		if (nParam)
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(uParam, nParam);
		break;

		//´ðÓ¦/¾Ü¾ø½»Ò×ÇëÇó
		//uParam = (KUiPlayerItem*)pRequestPlayer ·¢³öÇëÇóµÄÍæ¼Ò
		//nParam = (INT)(bool)bAccept ÊÇ·ñ½ÓÊÜÇëÇó
	case GOI_TRADE_INVITE_RESPONSE:
		if (uParam)
		{
			KTrade::ReplyInvite(((KUiPlayerItem*)uParam)->nIndex, nParam);
		}
		break;
	case GOI_TRADE_FOLKGAME_INVITE_RESPONSE:
		if (uParam)
		{
			KTrade::ReplyInvite(((KUiPlayerItem*)uParam)->nIndex, nParam, true);
		}
		break;
		//Ôö¼õÒ»¸öÓûÂô³öµÄÎïÆ·
		//uParam = (KUiObjAtRegion*) pObject -> ÎïÆ·ÐÅÏ¢£¬ÆäÖÐ×ø±êÐÅÏ¢ÎªÔÚ½»Ò×½çÃæÖÐµÄ×ø±ê
		//nParam = bAdd -> 0Öµ±íÊ¾¼õÉÙ£¬1Öµ±íÊ¾Ôö¼Ó
		//Remark : Èç¹ûÎïÆ·ÊÇ½ðÇ®µÄ»°£¬ÔòKUiObjAtRegion::Obj::uId±íÊ¾°Ñ½ðÇ®¶îµ÷ÕûÎªÕâ¸öÖµ£¬ÇÒnParamÎÞÒâÒå¡£
	case GOI_TRADE_DESIRE_ITEM:
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			if (pInfo->Obj.uGenre != CGOG_MONEY)
				break;
			Player[CLIENT_PLAYER_INDEX].TradeMoveMoney(pInfo->Obj.uId);
		}
		break;

		//ÓÐÎÞ½»Ò×ÒâÏò
		//nParam = bWilling
	case GOI_TRADE_WILLING:
		if (!nParam || Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMenuState() == PLAYER_MENU_STATE_TRADEOPEN)
			Player[CLIENT_PLAYER_INDEX].TradeApplyClose();
		else
			Player[CLIENT_PLAYER_INDEX].TradeApplyOpen((char*)uParam, nParam);
		break;

		//Ëø¶¨½»Ò×
		//nParam = (INT)(book)bLock ÊÇ·ñËø¶¨
	case GOI_TRADE_LOCK:
		if (!Player[CLIENT_PLAYER_INDEX].CheckTrading())
			break;
		Player[CLIENT_PLAYER_INDEX].TradeApplyLock();
		break;

		//½»Ò×
	case GOI_TRADE:
		if (!Player[CLIENT_PLAYER_INDEX].CheckTrading())
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock != 1 || Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeDestLock != 1)
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeState == 0)
			Player[CLIENT_PLAYER_INDEX].TradeDecision(1, uParam);		// ½»Ò×È·¶¨

		break;
	case GOI_TRADE_FOLKGAMEID:
		if (!Player[CLIENT_PLAYER_INDEX].CheckTrading())
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock != 1 || Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeDestLock != 1)
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeState)
			Player[CLIENT_PLAYER_INDEX].TradeDecision(2, uParam);
		break;
		//½»Ò×È¡Ïû
	case GOI_TRADE_CANCEL:
		if (!Player[CLIENT_PLAYER_INDEX].CheckTrading())
			break;
		Player[CLIENT_PLAYER_INDEX].TradeDecision(0);		// ½»Ò×È¡Ïû
		break;
	case GOI_SPAR_INVITE_RESPONSE:
		if (uParam)
			Player[CLIENT_PLAYER_INDEX].m_cPK.ReplyInvite(((KUiPlayerItem*)uParam)->uId, nParam);
		break;
		//ÐÂÌí¼ÓÁÄÌìºÃÓÑ
		//uParam = (KUiPlayerItem*)pFriend
		//			KUiPlayerItem::nData = 0
	case GOI_CHAT_FRIEND_ADD:
	{
		if (g_pClient)
		{
			size_t pckgsize = sizeof(tagExtendProtoHeader) + sizeof(ASK_ADDFRIEND_CMD);

			tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_malloca(pckgsize);
			pExHeader->ProtocolType = c2s_extendfriend;
			pExHeader->wLength = pckgsize - 1;

			ASK_ADDFRIEND_CMD* pAafCmd = (ASK_ADDFRIEND_CMD*)(pExHeader + 1);
			pAafCmd->ProtocolFamily = pf_friend;
			pAafCmd->ProtocolID = friend_c2c_askaddfriend;
			pAafCmd->pckgid = -1;
			strncpy(pAafCmd->dstrole, ((KUiPlayerItem*)uParam)->Name, _NAME_LEN);
			g_pClient->SendPackToServer(pExHeader, pckgsize);

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_APPLY_ADD_FRIEND, ((KUiPlayerItem*)uParam)->Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (UINT)&sMsg, 0);
		}
	}
	break;
	//¶Ô±ðÈËÒª¼Ó×Ô¼ºÎªºÃÓÑµÄ»Ø¸´
	//uParam = (KUiPlayerItem*)pRequestPlayer ·¢³öÇëÇóµÄÍæ¼Ò
	//nParam = (INT)(bool)bAccept ÊÇ·ñ½ÓÊÜÇëÇó
	case GOI_CHAT_FRIEND_INVITE:
		if (uParam)
		{
			if (g_pClient)
			{
				size_t pckgsize = sizeof(tagExtendProtoHeader) + sizeof(REP_ADDFRIEND_CMD);

				tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_malloca(pckgsize);
				pExHeader->ProtocolType = c2s_extendfriend;
				pExHeader->wLength = pckgsize - 1;

				REP_ADDFRIEND_CMD* pRafCmd = (REP_ADDFRIEND_CMD*)(pExHeader + 1);
				pRafCmd->ProtocolFamily = pf_friend;
				pRafCmd->ProtocolID = friend_c2c_repaddfriend;
				pRafCmd->pckgid = -1;
				strncpy(pRafCmd->dstrole, ((KUiPlayerItem*)uParam)->Name, _NAME_LEN);
				pRafCmd->answer = nParam ? answerAgree : answerDisagree;
				g_pClient->SendPackToServer(pExHeader, pckgsize);
			}
		}
		break;

	case GOI_OPTION_SETTING:			//Ñ¡ÏîÉèÖÃ
		if (uParam == OPTION_DYNALIGHT)
		{
			g_ScenePlace.EnableDynamicLights(nParam != 0);
			if (g_pRepresent)
				g_pRepresent->SetOption(DYNAMICLIGHT, nParam != 0);
		}
		else if (uParam == OPTION_PERSPECTIVE)
		{
			if (g_pRepresent)
				g_pRepresent->SetOption(PERSPECTIVE, nParam != 0);
		}
		else if (uParam == OPTION_MUSIC_VALUE)
			Option.SetMusicVolume(nParam);
		else if (uParam == OPTION_SOUND_VALUE)
			Option.SetSndVolume(nParam);
		else if (uParam == OPTION_BRIGHTNESS)
			Option.SetGamma(nParam);
		else if (uParam == OPTION_WEATHER)
			g_ScenePlace.EnableWeather(nParam);
		break;
	case GOI_VIEW_PLAYERITEM:
		g_cViewItem.ApplyViewEquip(uParam);
		break;
	case GOI_VIEW_PLAYERITEM_END:
		g_cViewItem.DeleteAll();
		break;
	case GOI_PLAYER_ACTION:
	{
		if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
			return 0;

		switch (uParam)
		{
		case PA_RUN:
			Player[CLIENT_PLAYER_INDEX].m_RunStatus = ~Player[CLIENT_PLAYER_INDEX].m_RunStatus;
			break;
		case PA_SIT:
			if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
			{
				if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_sit)
				{
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_sit);
					SendClientCmdSit(true);
				}
				else
				{
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
					SendClientCmdSit(false);
				}
			}
			else
			{
				KSystemMessage	sMsg;
				strcpy(sMsg.szMessage, MSG_CANT_SIT_RIDE);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (UINT)&sMsg, 0);
			}
			break;
		case PA_RIDE:
			SendClientCmdRide();
			break;
		}
	}
	break;
	case GOI_PK_SETTING:		//ÉèÖÃPK
	{
		Player[CLIENT_PLAYER_INDEX].m_cPK.ApplySetNormalPKState(nParam);
	}
	break;
	//³ðÉ±Ä³ÈË
	//uParam = (KUiPlayerItem*) pTarget	³ðÉ±Ä¿±ê
	case GOI_REVENGE_SOMEONE:
		if (uParam)
		{
			KUiPlayerItem* pTarget = (KUiPlayerItem*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cPK.ApplyEnmityPK(pTarget->uId);
		}
		break;
	case GOI_SPAR_SOMEONE:
		if (uParam)
		{
			KUiPlayerItem* pTarget = (KUiPlayerItem*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cPK.SparApplyStart(pTarget->uId);
		}
		break;
		//¸úËæÄ³ÈË
		//uParam = (KUiPlayerItem*) pTarget	¸úËæÄ¿±ê
	case GOI_FOLLOW_SOMEONE:
		if (uParam)
		{
			KUiPlayerItem* pTarget = (KUiPlayerItem*)uParam;
			if (Npc[pTarget->nIndex].m_Kind == kind_player &&
				!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PTrade.nTrade)
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = pTarget->nIndex;
		}
		break;
	case GOI_VIEW_PLAYERSHOP:
		if (uParam)
		{
			if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
				return 0;

			KUiPlayerItem* pTarget = (KUiPlayerItem*)uParam;
			if (Npc[pTarget->nIndex].m_Kind == kind_player)
				g_cSellItem.Update(TRUE, Npc[pTarget->nIndex].m_dwID);
		}
		break;
		//ÏÔÊ¾¸÷Íæ¼ÒÈËÃû
		//nParam = (INT)(bool)bShow	ÊÇ·ñÏÔÊ¾
	case GOI_SHOW_PLAYERS_NAME:
		NpcSet.SetShowNameFlag(nParam);
		break;
		//ÏÔÊ¾¸÷Íæ¼ÒÉúÃü
		//nParam = (INT)(bool)bShow	ÊÇ·ñÏÔÊ¾
	case GOI_SHOW_PLAYERS_LIFE:
		NpcSet.SetShowLifeFlag(nParam);
		break;
	case GOI_INPUT:
		if (uParam)
		{
			C2S_BUF_COMMAND sIP;
			sIP.ProtocolType = c2s_inputcommand;
			strcpy(sIP.szBuf, (char*)uParam);
			g_pClient->SendPackToServer((BYTE*)&sIP, sizeof(C2S_BUF_COMMAND));
		}
		break;
	case GOI_LOCKSTATE:
	{
		PLAYER_COMMAND	sLS;
		sLS.ProtocolType = c2s_playercommand;
		sLS.m_wMsgID = enumC2S_PLAYERCOMMAND_ID_LOCKSTATE;
		sLS.m_lpBuf = 0;
		sLS.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		g_pClient->SendPackToServer((BYTE*)&sLS, sLS.m_wLength + 1);
	}
	break;
	case GOI_GIVE:
	{
		PLAYER_COMMAND	sGv;
		sGv.ProtocolType = c2s_playercommand;
		sGv.m_wMsgID = enumC2S_PLAYERCOMMAND_ID_GIVE;
		sGv.m_lpBuf = (void*)uParam;
		sGv.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		g_pClient->SendPackToServer((BYTE*)&sGv, sizeof(PLAYER_COMMAND));
	}
	break;
	case GOI_LOCKITEM:
		if (uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion*)uParam;
			if (pObj->Obj.uId > 0 && pObj->Obj.uId < MAX_ITEM)
			{
				KItem* pItem = &Item[pObj->Obj.uId];
				if (pItem)
				{
					PLAYER_COMMAND	sLI;
					sLI.ProtocolType = c2s_playercommand;
					sLI.m_wMsgID = enumC2S_PLAYERCOMMAND_ID_LOCKITEM;
					sLI.m_lpBuf = (void*)MAKELONG(pItem->GetID(), nParam);
					sLI.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
					g_pClient->SendPackToServer((BYTE*)&sLI, sLI.m_wLength + 1);
				}
			}
		}
		break;
	case GOI_SUPERSHOP:
	{
		PLAYER_COMMAND	sSS;
		sSS.ProtocolType = c2s_playercommand;
		sSS.m_wMsgID = enumC2S_PLAYERCOMMAND_ID_SUPERSHOP;
		sSS.m_lpBuf = 0;
		sSS.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		g_pClient->SendPackToServer((BYTE*)&sSS, sSS.m_wLength + 1);
	}
	break;
	case GOI_SHOW_OBJ_NAME:
		NpcSet.SetShowObjFlag(nParam);
		break;
	case GOI_PICKUP_OBJ:
		Player[CLIENT_PLAYER_INDEX].PickObjectNear();
		break;
	case GOI_OPTIMAL:
		Option.SetLow((INT)uParam, (BOOL)nParam);
		break;
	case GOI_OFFLINE:
	{
		SendClientOffline(); // TamLTM Add offline script

		/*PLAYER_COMMAND	sOffL;
		sOffL.ProtocolType = c2s_playercommand;
		sOffL.m_wMsgID = enumC2S_PLAYERCOMMAND_ID_OFFLINE;
		sOffL.m_lpBuf = 0;
		sOffL.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		g_pClient->SendPackToServer((BYTE*)&sOffL, sOffL.m_wLength + 1);*/
	}
	break;
	case GOI_MASKFEATURE:
	{
		PLAYER_COMMAND	sMF;
		sMF.ProtocolType = c2s_playercommand;
		sMF.m_wMsgID = enumC2S_PLAYERCOMMAND_ID_MASKFEATURE;
		sMF.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMF.m_lpBuf = 0;
		g_pClient->SendPackToServer((BYTE*)&sMF, sMF.m_wLength + 1);
	}
	break;
	case GOI_PLAYER_SETPRICE:
		if (uParam)
		{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			PLAYER_TRADE_SET_COMMAND sTrade;
			sTrade.ProtocolType = c2s_playertradeset;
			sTrade.m_dwID = Item[pObject1->Obj.uId].GetID();
			sTrade.m_nPrice = nParam;
			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)&sTrade, sizeof(PLAYER_TRADE_SET_COMMAND));
		}
		break;
	case GOI_PLAYER_DOTRADE:
	{
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMenuState() == PLAYER_MENU_STATE_TRADEOPEN)
			Player[CLIENT_PLAYER_INDEX].TradeApplyClose();

		PLAYER_TRADE_START_COMMAND sTrade;
		sTrade.ProtocolType = c2s_playertradestart;
		strcpy(sTrade.Name, (char*)uParam);
		if (g_pClient)
			g_pClient->SendPackToServer((BYTE*)&sTrade, sizeof(PLAYER_TRADE_START_COMMAND));
	}
	break;
	case GOI_PLAYER_BUYTRADE:
	{
		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.IsLockOperation())
			return 0;
		KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
		if (CGOG_PLAYERSELLITEM != pObject1->Obj.uGenre)
			break;

		INT nWidth, nHeight;
		ItemPos	Pos;

		nWidth = Item[pObject1->Obj.uId].GetWidth();
		nHeight = Item[pObject1->Obj.uId].GetHeight();
		if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
		{
			nRet = 0;
			break;
		}
		if (Pos.nPlace != pos_equiproom)
		{
			nRet = 0;

			KSystemMessage	sMsg;

			strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
			sMsg.eType = SMT_SYSTEM;
			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (UINT)&sMsg, 0);
			break;
		}

		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < Item[pObject1->Obj.uId].GetTradePrice())
		{
			nRet = 0;
			KSystemMessage	sMsg;

			strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
			sMsg.eType = SMT_SYSTEM;
			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (UINT)&sMsg, 0);
			break;

		}
		PLAYER_TRADE_BUY_COMMAND sBuy;
		sBuy.ProtocolType = c2s_playertradebuy;
		sBuy.m_nIdx = g_cSellItem.FindIdx(pObject1->Obj.uId);
		sBuy.m_dwID = g_cSellItem.GetSellId();
		sBuy.m_Place = pos_equiproom;
		sBuy.m_X = Pos.nX;
		sBuy.m_Y = Pos.nY;
		if (g_pClient)
			g_pClient->SendPackToServer((BYTE*)&sBuy, sizeof(PLAYER_TRADE_BUY_COMMAND));
		Player[CLIENT_PLAYER_INDEX].m_ItemList.LockOperation();
		nRet = 1;
	}
	break;

	case GOI_PLAYER_RLTRADE:
	{
		g_cSellItem.Release();
		BYTE btProtocol = c2s_playertradeviewend;
		g_pClient->SendPackToServer((BYTE*)&btProtocol, sizeof(BYTE));
	}
	break;
	case GOI_LOCKMOVE:
		PLAYER_LOCKMOVE	LockMove;
		LockMove.ProtocolType = c2s_lockmove;
		LockMove.m_LockMove.bLock = (BOOL)uParam;
		LockMove.m_LockMove.nPlace = (INT)nParam;
		g_pClient->SendPackToServer((BYTE*)&LockMove, sizeof(PLAYER_LOCKMOVE));
		break;
	case GOI_UNLOCK:
		if (uParam)
		{
			C2S_BUF_COMMAND sUL;
			sUL.ProtocolType = c2s_unlockcommand;
			strcpy(sUL.szBuf, (char*)uParam);
			g_pClient->SendPackToServer((BYTE*)&sUL, sizeof(C2S_BUF_COMMAND));
		}
		break;
	case GOI_CHATROOM:
		if (uParam)
		{
			PLAYER_CHATROOM_DECISION_COMMAND	sRoom;
			sRoom.ProtocolType = c2s_chatroomdecision;
			sRoom.m_nDecision = (INT)uParam;
			sRoom.m_bRoomList = (BOOL)nParam;
			g_pClient->SendPackToServer((BYTE*)&sRoom, sizeof(PLAYER_CHATROOM_DECISION_COMMAND));
		}
		break;

	case GOI_CP_SET_IMAGE_PLAYER:
		SendClientCPSetImageCmd(uParam);
		break;

		//TamLTM da tau
	case GOI_CP_LOAD_SCRIPT:// Da Tau nhiem vu
		SendClientDaTauCmd(uParam);
		//g_DebugLog("uParam->nScript: %d", uParam);
		break;
		//end
		//TamLTM kham nam xanh
	case GOI_ADD_UI_CMD_SCRIPT:
		if (uParam)
		{
			if (uParam == 12)
			{
				SendUiCmdScript(uParam, (char*)nParam);
			}
			else
				SendUiCmdScript(uParam, (char*)nParam);
		}
		break;
	case GOI_RECOVERY_BOX_COMMAND:
		if (uParam)
		{
			DWORD dwID;
			INT nW, nH;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == uParam)
			{
				dwID = Item[pItem->nIdx].GetID();
				nW = Item[pItem->nIdx].GetWidth();
				nH = Item[pItem->nIdx].GetHeight();

				SendClientRecoveryBox(dwID, nW, nH);
			}

			while (pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == uParam)
				{
					dwID = Item[pItem->nIdx].GetID();
					nW = Item[pItem->nIdx].GetWidth();
					nH = Item[pItem->nIdx].GetHeight();

					SendClientRecoveryBox(dwID, nW, nH);
				}
			}
		}
		break;
		// end code
	//TamLTM Open Progress bar
	case GOI_CP_LOAD_SCRIPT_PROGRESS_BAR:
		SendClientOpenProgressBarCmd(uParam);
		break;
		//end
	case GOI_AUTO_CASTSKILL:
	{
		/*INT nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		if (nIndex > 0)
		{
			ISkill* pISkill = (KSkill*)g_SkillManager.GetSkill(Npc[nIndex].m_CastSkill->nSkillId, Npc[nIndex].m_CastSkill->nSkillLevel);

			if (pISkill)
			{
				if (Npc[nIndex].m_CastSkill->dwNextCastTime < SubWorld[Npc[nIndex].m_SubWorldIndex].m_dwCurrentTime)
				{
					Npc[nIndex].SendCommand(do_skill, Npc[nIndex].m_CastSkill->nSkillId, 0, 0);
					SendClientCmdSkill(Npc[nIndex].m_CastSkill->nSkillId, 0, 0);
				}
			}
		}*/
		break;
	}
	case GOI_COMPITEM_COM:
	{
		//if (uParam)
		{
			SendCompoundAction(nParam);
		}
	}
	break;
	case GOI_COMPITEM_DISTILL:
	case GOI_COMPITEM_ENCHASE:
	case GOI_COMPITEM_FORGE:
	{
		//if (uParam)
		{
			SendEnchaseAction(nParam);
		}
	}
	break;
	default:
		nRet = 0;
		break;
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º·¢ËÍÊäÈëÉè±¸µÄÊäÈë²Ù×÷ÏûÏ¢
//--------------------------------------------------------------------------
void KCoreShell::ProcessInput(UINT uMsg, UINT uParam, INT nParam)
{
	Player[CLIENT_PLAYER_INDEX].ProcessInputMsg(uMsg, uParam, nParam);
}

//dungnd Click minimap run
void KCoreShell::AutoRun(INT x, INT y)
{
	Player[CLIENT_PLAYER_INDEX].AutoRun(x, y);
}

INT KCoreShell::GetTargetNPC()
{
	return Player[CLIENT_PLAYER_INDEX].GetTargetNpc();
}

INT KCoreShell::FindSelectNPC(INT x, INT y, INT nRelation, bool bSelect, void* pReturn, INT& nKind, bool bFind)
{
	Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, nRelation);
	INT nT = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();

	if (!bSelect)
		Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);

	if (nT > 0)
	{
		if (pReturn)
		{
			if (bFind && (Npc[nT].Name[0] == 0 && (Npc[nT].m_nTeamServerID == -1 ||
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nTeamServerID != Npc[nT].m_nTeamServerID)))
				return false;
			KUiPlayerItem* p = (KUiPlayerItem*)pReturn;
			strncpy(p->Name, Npc[nT].Name, 32);
			p->nIndex = Npc[nT].m_Index;
			p->uId = Npc[nT].m_dwID;
			p->nData = Npc[nT].GetMenuState();
			p->nTeamID = Npc[nT].m_nTeamServerID;
		}
		nKind = Npc[nT].m_Kind;
		return true;
	}
	return false;
}

INT KCoreShell::FindSelectObject(INT x, INT y, bool bSelect, INT& nObjectIdx, INT& nKind)
{
	Player[CLIENT_PLAYER_INDEX].FindSelectObject(x, y);
	INT nT = Player[CLIENT_PLAYER_INDEX].GetTargetObj();

	if (!bSelect)
		Player[CLIENT_PLAYER_INDEX].SetTargetObj(0);

	if (nT > 0)
	{
		nObjectIdx = nT;
		nKind = Object[nT].m_nKind;
		return true;
	}
	return false;
}


INT KCoreShell::FindSpecialNPC(char* Name, void* pReturn, INT& nKind)
{
	if (Name == NULL || Name[0] == 0)
		return false;
	for (INT nT = 0; nT < MAX_NPC; nT++)
	{
		if (strcmp(Npc[nT].Name, Name) == 0)
		{
			if (pReturn)
			{
				KUiPlayerItem* p = (KUiPlayerItem*)pReturn;
				strncpy(p->Name, Npc[nT].Name, 32);
				p->nIndex = Npc[nT].m_Index;
				p->uId = Npc[nT].m_dwID;
				p->nData = Npc[nT].GetMenuState();
				p->nTeamID = Npc[nT].m_nTeamServerID;
			}
			nKind = Npc[nT].m_Kind;
			return true;
		}
	}
	return false;
}

INT KCoreShell::ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, uint16_t nMsgLength)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC)
		{
			INT nTalker = p->nIndex;
			if (Npc[nTalker].m_Kind == kind_player &&
				Npc[nTalker].m_dwID == p->uId)
			{
				Npc[nTalker].SetChatInfo(p->Name, pMsgBuff, nMsgLength);
				return true;
			}
		}
	}

	return false;
}

void KCoreShell::ApplyAddTeam(void* pPlayer)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC && !Player[CLIENT_PLAYER_INDEX].CheckTrading())
		{
			Player[CLIENT_PLAYER_INDEX].ApplyAddTeam(p->nIndex);
		}
	}
}

void KCoreShell::TradeApplyStart(void* pPlayer, bool bFolkGame)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC && !Player[CLIENT_PLAYER_INDEX].CheckTrading())
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyStart(p->nIndex, bFolkGame);
		}
	}
}

void KCoreShell::JoinTongReply(void* pPlayer)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC && !Player[CLIENT_PLAYER_INDEX].CheckTrading())
		{
			Player[CLIENT_PLAYER_INDEX].m_cTong.JoinTongReply(p->nIndex);
		}
	}
}

INT KCoreShell::UseSkill(INT x, INT y, INT nSkillID)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;

	INT nX = x;
	INT nY = y;
	INT nZ = 0;
	g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
	INT nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (Npc[nIndex].IsCanInput())
	{
		INT nIdx = 0;

		nIdx = Npc[nIndex].m_SkillList.FindSame(nSkillID);
		Npc[nIndex].SetActiveSkill(nIdx);
	}
	else
	{
		return 0;
	}

	if (Npc[nIndex].m_ActiveSkillID > 0)
	{
		ISkill* pISkill = g_SkillManager.GetSkill(Npc[nIndex].m_ActiveSkillID, 1);
		if (!pISkill)
			return 0;

		if (pISkill->IsAura())
			return 0;

		INT nAttackRange = pISkill->GetAttackRadius();

		INT nTargetIdx = 0;
		//°´ÕÕObject / Enemy / Ally µÄÓÅÏÈ¼¶ÕÒµ½ÐèÒª´òµÄ¶ÔÏóid

		if (pISkill->IsTargetAlly())
		{
			Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, relation_ally);
			if (Player[CLIENT_PLAYER_INDEX].GetTargetNpc())
			{
				nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();
			}
		}

		if (pISkill->IsTargetEnemy())
		{
			Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, relation_enemy);
			if (Player[CLIENT_PLAYER_INDEX].GetTargetNpc())
			{
				nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();
			}
		}

		if (pISkill->IsTargetObj())
		{
			Player[CLIENT_PLAYER_INDEX].FindSelectObject(x, y);
			if (Player[CLIENT_PLAYER_INDEX].GetTargetObj())
			{
				nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetObj();
			}
		}
		//Èç¹û¼¼ÄÜ±ØÐëÖ¸¶¨¶ÔÏó£¬¶øµ±Ç°Î»ÖÃÎÞ¶ÔÏóµÄ»°£¬Ö±½ÓÍË³ö
		if (pISkill->IsTargetOnly() && !nTargetIdx)
		{
			Npc[nIndex].m_nPeopleIdx = 0;
			Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
			return 0;
		}

		if (nIndex == nTargetIdx)
		{
			Npc[nIndex].m_nPeopleIdx = 0;
			Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
			return 0;
		}
		if ((!Npc[nIndex].m_SkillList.CanCast(Npc[nIndex].m_ActiveSkillID, SubWorld[Npc[nIndex].m_SubWorldIndex].m_dwCurrentTime))
			||
			(!Npc[nIndex].Cost(pISkill->GetSkillCostType(), pISkill->GetSkillCost(&Npc[nIndex]), TRUE))
			)
		{
			Npc[nIndex].m_nPeopleIdx = 0;
			Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
			return 0;
		}

		//ÎÞ¶ÔÏó£¬Ö±½Ó·¢×ø±ê
		if (!nTargetIdx)
		{
			INT nNpcX, nNpcY;
			Npc[nIndex].GetMpsPos(&nNpcX, &nNpcY);

			SendClientCmdSkill(Npc[nIndex].m_ActiveSkillID, nX, nY, nNpcX, nNpcY);
			Npc[nIndex].SendCommand(do_skill, Npc[nIndex].m_ActiveSkillID, nX, nY);
		}
		else
		{
			if (pISkill->IsTargetOnly())
			{
				if (NpcSet.GetDistance(nIndex, nTargetIdx) >= nAttackRange)
				{
					Player[CLIENT_PLAYER_INDEX].SetTargetNpc(nTargetIdx);
					return 0;
				}
			}
			if (nIndex == nTargetIdx && pISkill->GetSkillStyle() == SKILL_SS_Missles)
				return 0;

			if (NpcSet.GetDistance(nIndex, nTargetIdx) < nAttackRange)
			{
				INT nNpcX, nNpcY;
				Npc[nIndex].GetMpsPos(&nNpcX, &nNpcY);

				SendClientCmdSkill(Npc[nIndex].m_ActiveSkillID, -1, Npc[nTargetIdx].m_dwID, nNpcX, nNpcY);
				Npc[nIndex].SendCommand(do_skill, Npc[nIndex].m_ActiveSkillID, -1, nTargetIdx);
			}
		}
	}

	Npc[nIndex].m_nPeopleIdx = 0;
	return 1;
}

INT KCoreShell::LockSomeoneUseSkill(INT nTargetIndex, INT nSkillID)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;

	INT nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (nTargetIndex == nIndex)
		return 0;

	if (Npc[nIndex].IsCanInput())
	{
		INT nIdx = 0;

		nIdx = Npc[nIndex].m_SkillList.FindSame(nSkillID);
		Npc[nIndex].SetActiveSkill(nIdx);
	}
	else
	{
		return 0;
	}

	INT nRelation = NpcSet.GetRelation(nIndex, nTargetIndex);
	if (nRelation == relation_enemy)
	{
		Npc[nIndex].m_nPeopleIdx = nTargetIndex;
		return 1;
	}

	return 0;
}

INT KCoreShell::LockSomeoneAction(INT nTargetIndex)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;

	INT nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (nTargetIndex == nIndex)
		return 0;
	if (nTargetIndex <= 0 || nTargetIndex >= MAX_NPC)	//È¡ÏûLock
	{
		Npc[nIndex].m_nPeopleIdx = 0;
		return 1;
	}

	INT nRelation = NpcSet.GetRelation(nIndex, nTargetIndex);
	if (nRelation != relation_enemy)
	{
		Npc[nIndex].m_nPeopleIdx = nTargetIndex;
		return 1;
	}

	return 0;
}

INT KCoreShell::LockObjectAction(INT nTargetIndex)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;

	INT nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (nTargetIndex <= 0)	//È¡ÏûLock
		Npc[nIndex].m_nObjectIdx = 0;
	else
		Npc[nIndex].m_nObjectIdx = nTargetIndex;

	return 1;
}

void KCoreShell::GotoWhere(INT x, INT y, INT mode)
{

	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return;

	if (mode < 0 || mode > 2)
		return;

	if (Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
	{
		INT bRun = false;

		if ((mode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) ||
			mode == 2)
			bRun = true;

		INT nX = x;
		INT nY = y;
		INT nZ = 0;

		g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
		INT nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

		if (!bRun)
		{
			Npc[nIndex].SendCommand(do_walk, nX, nY);
			// Send to Server
			SendClientCmdWalk(nX, nY);
		}
		else
		{
			Npc[nIndex].SendCommand(do_run, nX, nY);
			// Send to Server
			SendClientCmdRun(nX, nY);
		}
		Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
	}
}
//
// find way by kinnox;
void KCoreShell::GotoWhereDirect(int x, int y, int mode)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return;

	if (mode < 0 || mode > 2)
		return;

	int nPlayerX, nPlayerY;
	int nDestX, nDestY, dX, dY;

	if (Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
	{
		int bRun = false;

		if ((mode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) ||
			mode == 2)
			bRun = true;

		int nX = x;
		int nY = y;
		int nZ = 0;

		int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nPlayerX, &nPlayerY);
		dX = nPlayerX & 0x1F;
		dY = nPlayerY & 0x1F;
		nDestX = nX + 0x10 - dX; // Anti-LAG
		nDestY = nY + 0x10 - dY; // Anti-LAG	

		if (!bRun)
		{
			Npc[nIndex].SendCommand(do_walk, nDestX, nDestY);
			// Send to Server
			SendClientCmdWalk(nX, nY);
		}
		else
		{
			Npc[nIndex].SendCommand(do_run, nDestX, nDestY);
			// Send to Server
			SendClientCmdRun(nX, nY);
		}
		Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
	}
}

//find way by kinnox
bool CloseToTarget(const FindPathNode& point1, const FindPathNode& point2, int delta) 
 {
	int distance = sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
	//g_DebugLog("Distance %d\n", distance);
	return distance <= delta;
}

BOOL KCoreShell::AutoMove() //find way by kinnox
{
	FindPathNode nextPoint;
	FindPathNode currentPoint;
	FindPathNode endPoint;
	INT nCurX, nCurY;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMpsPos(&nCurX, &nCurY);
	BOOL nRet = FALSE;
	if (!GetPaintMode())
		return nRet;
	if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.empty())
	{
		//go to the next point in auto move path
		nextPoint = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.front();
		currentPoint.x = nCurX;
		currentPoint.y = nCurY;
		if (g_JXPathFinder.GetNextStep(currentPoint, nextPoint, 2) == emKNEXTSTEP_RESULT_SUCCESS)
		{
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.empty())
				return nRet;
			nextPoint = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.front();
			endPoint = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.back();
			if ((CloseToTarget(currentPoint, nextPoint, 20) || (nextPoint.x == endPoint.x) && (nextPoint.y == endPoint.y)))
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.erase(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.begin());
			else if ((CloseToTarget(currentPoint, nextPoint, 189) || (g_JXPathFinder.getStand().x == currentPoint.x && g_JXPathFinder.getStand().y == currentPoint.y)))
			{
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.erase(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PathFind.begin());
			}
			else
				g_JXPathFinder.setStand(currentPoint.x, currentPoint.y);
			//g_DebugLog("Running to %d, %d from Current %d, %d\n", nextPoint.x, nextPoint.y, currentPoint.x, currentPoint.y);
			if ((nextPoint.x - currentPoint.x) < 0 && (nextPoint.y - currentPoint.y > 0) || (nextPoint.x - currentPoint.x) < 0 && (nextPoint.y == currentPoint.y))
			{
				int nBarrier = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].GetBarrier(nextPoint.x, nextPoint.y);
				if (Obstacle_NULL == nBarrier)
					GotoWhereDirect(nextPoint.x, nextPoint.y, 0);
			}
			else
			{
				int nBarrier = SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].GetBarrier(nextPoint.x, nextPoint.y);
				if (Obstacle_NULL == nBarrier)
					GotoWhereDirect(nextPoint.x, nextPoint.y, 0);
			}
			nRet = TRUE;

		}
		else if (g_JXPathFinder.GetNextStep(currentPoint, nextPoint, 1) == emKNEXTSTEP_RESULT_ARRIVAL)
		{
			GotoWhereDirect(nextPoint.x, nextPoint.y, 0);
			return nRet; //Still moving
		}
	}
	else
	{
		return nRet; //Nothing in auto move path
	}
	return 1;
}

//
void KCoreShell::GetWhere(INT x, INT y, void* pInfo)
{
	KPosition3* p = (KPosition3*)pInfo;
	INT nX = x;
	INT nY = y;
	INT nZ = 0;
	g_ScenePlace.ViewPortCoordToSpaceCoordNew(p->nX, nX, nY, nZ);
	p->nY = nX;
	p->nZ = nY;
}

void KCoreShell::Goto(INT nDir, INT mode)
{
	if (nDir < 0 || nDir > 63)
		return;

	if (mode < 0 || mode > 2)
		return;

	INT bRun = false;

	if ((mode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) ||
		mode == 2)
		bRun = true;

	INT nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	INT nSpeed;
	if (bRun)
		nSpeed = Npc[nIndex].m_CurrentRunSpeed;
	else
		nSpeed = Npc[nIndex].m_CurrentWalkSpeed;

	Player[CLIENT_PLAYER_INDEX].Walk(nDir, nSpeed);

	Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
}

void KCoreShell::Turn(INT nDir)
{
	if (nDir < 0 || nDir > 3)
		return;

	if (nDir == 0)
		Player[CLIENT_PLAYER_INDEX].TurnLeft();
	else if (nDir == 1)
		Player[CLIENT_PLAYER_INDEX].TurnRight();
	else
		Player[CLIENT_PLAYER_INDEX].TurnBack();
}

INT KCoreShell::ThrowAwayItem()
{
	return Player[CLIENT_PLAYER_INDEX].ThrowAwayItem();
}

INT KCoreShell::GetNPCRelation(INT nIndex)
{
	return NpcSet.GetRelation(Player[CLIENT_PLAYER_INDEX].m_nIndex, nIndex);
}

BOOL KCoreShell::CheckMouse()
{
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand())
		return TRUE;
	return FALSE;
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»æÖÆÓÎÏ·¶ÔÏó
//--------------------------------------------------------------------------
void KCoreShell::DrawGameObj(UINT uObjGenre, UINT uId, INT x, INT y, INT Width, INT Height, INT nParam)
{
	if (g_pRepresent)
		CoreDrawGameObj(uObjGenre, uId, x, y, Width, Height, nParam);
}

#include "../../Represent/iRepresent/iRepresentshell.h"

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»æÖÆÓÎÏ·ÊÀ½ç
//--------------------------------------------------------------------------
void KCoreShell::DrawGameSpace()
{
	if (g_pRepresent)
	{
		g_ScenePlace.Paint();
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉèÖÃ»æÍ¼½Ó¿ÚÊµÀýµÄÖ¸Õë
//--------------------------------------------------------------------------
void KCoreShell::SetRepresentShell(struct iRepresentShell* pRepresent)
{
	g_pRepresent = pRepresent;
	g_ScenePlace.RepresentShellReset();
	if (g_pAdjustColorTab && g_ulAdjustColorCount && g_pRepresent)
		g_pRepresent->SetAdjustColorList(g_pAdjustColorTab, g_ulAdjustColorCount);
}

void KCoreShell::SetMusicInterface(void* pMusicInterface)
{
	g_pMusic = (KMusic*)pMusicInterface;
	Option.SetMusicVolume(Option.GetMusicVolume());
}

//ÈÕ³£»î¶¯£¬coreÈç¹ûÒªÊÙÖÕÕýÇÞÔò·µ»Ø0£¬·ñÔò·µ»Ø·Ç0Öµ
INT KCoreShell::Breathe(BOOL isDisconnected)
{
	if (!isDisconnected)
	{
		//OperationRequest(GOI_AUTO_CASTSKILL, 0, 0);

		g_SubWorldSet.MessageLoop();
		g_SubWorldSet.MainLoop();
		g_ScenePlace.Breathe();
	}
	return true;
}

INT KCoreShell::GetProtocolSize(BYTE byProtocol)
{
	if (byProtocol <= s2c_clientbegin || byProtocol >= s2c_end)
		return -1;
	return g_nProtocolSize[byProtocol - s2c_clientbegin - 1];
}

//Debug
#ifdef SWORDONLINE_SHOW_DBUG_INFO
extern INT		g_bShowObstacle;
extern bool		g_bShowGameInfo;
#endif

INT KCoreShell::Debug(UINT uDataId, UINT uParam, INT nParam)
{
#ifdef SWORDONLINE_SHOW_DBUG_INFO
	switch (uDataId)
	{
	case DEBUG_SHOWINFO:
		Player[CLIENT_PLAYER_INDEX].m_bDebugMode = !Player[CLIENT_PLAYER_INDEX].m_bDebugMode;
		g_bShowGameInfo = !g_bShowGameInfo;
		break;
	case DEBUG_SHOWOBSTACLE:
		g_bShowObstacle = !g_bShowObstacle;
		break;
	}
#endif
	return 0;
}

//end code

DWORD KCoreShell::GetPing()
{
	return g_SubWorldSet.GetPing();
}

void KCoreShell::SetRepresentAreaSize(INT nWidth, INT nHeight)
{
	g_ScenePlace.SetRepresentAreaSize(nWidth, nHeight);
}

void KCoreShell::SetScreenSize(INT nWidth, INT nHeight)
{
	g_ScenePlace.SetScreenSize(nWidth, nHeight);
}

void KCoreShell::SetClient(LPVOID pClient)
{
	g_SetClient(pClient);
}

void KCoreShell::SendNewDataToServer(void* pData, INT nLength)
{
	if (g_pClient)
		g_pClient->SendPackToServer(pData, nLength);
}

//ÓëµØÍ¼Ïà¹ØµÄ²Ù×÷
INT	KCoreShell::SceneMapOperation(UINT uOper, UINT uParam, INT nParam)
{
	INT nRet = 0;
	switch (uOper)
	{
	case GSMOI_SCENE_TIME_INFO:
		if (uParam)
		{
			KUiSceneTimeInfo* pInfo = (KUiSceneTimeInfo*)uParam;
			g_ScenePlace.GetSceneNameAndFocus(pInfo->szSceneName, pInfo->nSceneId,
				pInfo->nScenePos0, pInfo->nScenePos1);
			pInfo->nGameSpaceTime = (SubWorld[0].m_dwCurrentTime / 100) % 1440;
		}
		break;
	case GSMOI_SCENE_TIME_INFO_OFTEN:
		if (uParam)
		{
			KUiSceneTimeInfoOften* pInfo = (KUiSceneTimeInfoOften*)uParam;
			g_ScenePlace.GetSceneNameAndFocusOften(pInfo->szSceneName, pInfo->nSceneId,
				pInfo->nScenePos0, pInfo->nScenePos1);
			pInfo->nGameSpaceTime = (SubWorld[0].m_dwCurrentTime / 100) % 1440;
		}
		break;
	case GSMOI_SCENE_MAP_INFO:
		nRet = g_ScenePlace.GetMapInfo((KSceneMapInfo*)uParam);
		break;
	case GSMOI_IS_SCENE_MAP_SHOWING:
		g_ScenePlace.SetMapParam(uParam, nParam);
		break;
	case GSMOI_PAINT_SCENE_MAP:
		g_ScenePlace.PaintMap(uParam, nParam);
		break;
	case GSMOI_SCENE_MAP_FOCUS_OFFSET:
		g_ScenePlace.SetMapFocusPositionOffset((INT)uParam, nParam);
		break;
	case GSMOI_SCENE_FOLLOW_WITH_MAP:	//ÉèÖÃ³¡¾°ÊÇ·ñËæ×ÅµØÍ¼µÄÒÆ¶¯¶øÒÆ¶¯
		g_ScenePlace.FollowMapMove(nParam);
		break;
	case GSMOI_IS_SCENE_DIRECT_MAP:
		nRet = g_ScenePlace.OnDirectMap((INT)uParam, nParam);
		break;
	case GSMOI_IS_SCENE_DO_DIRECT_MAP:
		g_ScenePlace.DoDirectMap((INT)uParam, nParam);
		break;
	}
	return nRet;
}

//Óë°ï»áÏà¹ØµÄ²Ù×÷, uOperµÄÈ¡ÖµÀ´×Ô GAME_TONG_OPERATION_INDEX
INT	KCoreShell::TongOperation(UINT uOper, UINT uParam, INT nParam)
{
	INT nRet = 0;
	switch (uOper)
	{
	case GTOI_TONG_CREATE:		//´´½¨°ï»á
		//uParam = (const char*) pszTongName °ï»áµÄÃû×Ö
		//nParam = (NPCCAMP)enFaction °ï»áÕóÓª
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyCreateTong(nParam, (char*)uParam);
		break;
	case GTOI_TONG_FLAG:		//²éÑ¯Ä³ÈËµÄÕÐÈË¿ª¹Ø
		//uParam = (KUiPlayerItem*) Òª²éË­
		//Return = (INT)(bool)		ÊÇ·ñ¿ª×ÅµÄÕÐÈË¿ª¹Ø
		if (uParam)
		{
			KUiPlayerItem* pItem = (KUiPlayerItem*)uParam;
			nRet = Npc[pItem->nIndex].m_bTongFlag;
		}
		else
			nRet = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bTongFlag;
		break;
	case GTOI_TONG_FIGURE:		//²éÑ¯Ä³ÈËµÄÕÐÈË¿ª¹Ø
		//uParam = (KUiPlayerItem*) Òª²éË­
		//Return = (INT)(bool)		ÊÇ·ñ¿ª×ÅµÄÕÐÈË¿ª¹Ø
		if (uParam)
		{
			KUiPlayerItem* pItem = (KUiPlayerItem*)uParam;
			nRet = Npc[pItem->nIndex].m_nFigure;
		}
		else
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFigure >= enumTONG_FIGURE_MEMBER && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFigure < enumTONG_FIGURE_NUM)
				nRet = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFigure;
		break;
	case GTOI_TONG_VICEROY:		//²éÑ¯Ä³ÈËµÄÕÐÈË¿ª¹Ø
		//uParam = (KUiPlayerItem*) Òª²éË­
		//Return = (INT)(bool)		ÊÇ·ñ¿ª×ÅµÄÕÐÈË¿ª¹Ø
		if (uParam)
		{
			KUiPlayerItem* pItem = (KUiPlayerItem*)uParam;
			if (Npc[pItem->nIndex].m_nFigure == enumTONG_FIGURE_MASTER)
			{
				nRet = 1;
			}
		}
		else
		{
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFigure == enumTONG_FIGURE_MASTER)
			{
				nRet = 1;
			}
		}
		break;
	case GTOI_TONG_RECRUIT:     //ÕÐÈË¿ª¹Ø
		if (uParam)
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyRecruit(1);
		else
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyRecruit(0);
		//uParam = (INT)(bool)bRecruit ÊÇ·ñÔ¸ÒâÕÐÈË
		break;
	case GTOI_TONG_ACTION:         //¶Ô°ïÄÚ³ÉÔ±×öµÄ¶¯×÷£¬»ò×Ô¼ºÓë°ï»áµÄ¹ØÏµµÄ¸Ä±ä
		//uParam = (KTongOperationParam*) pOperParam ¶¯×÷Ê±µÄ²ÎÊý
		//nParam = (KTongMemberItem*) pMember Ö¸³öÁË²Ù×÷£¨°ï»á³ÉÔ±£©¶ÔÏó£¬
	{
		KTongOperationParam* Oper = (KTongOperationParam*)uParam;
		KTongMemberItem* TargetInfo = (KTongMemberItem*)nParam;
		KUiPlayerItem TargetPlayer;
		INT nKind;

		switch (Oper->eOper)
		{
		case TONG_ACTION_DISMISS:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyKick(Oper->nData[0], Oper->nData[1], TargetInfo->Name);
			break;
		case TONG_ACTION_ASSIGN:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInstate(Oper->nData[0], Oper->nData[1], Oper->nData[2], Oper->nData[3], TargetInfo->Name);
			break;
		case TONG_ACTION_DEMISE:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeMaster(Oper->nData[0], Oper->nData[1], TargetInfo->Name);
			break;
		case TONG_ACTION_LEAVE:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyLeave();
			break;
		case TONG_ACTION_RECRUIT:
			break;
		case TONG_ACTION_APPLY:
			if (!FindSpecialNPC(TargetInfo->Name, &TargetPlayer, nKind))
			{
				break;
			}
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyAddTong(TargetPlayer.uId);
			break;
		case TONG_ACTION_CHANGE_AGNAME:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeAgname(Oper->nData[0], Oper->nData[1], TargetInfo->Name, Oper->szData);
			break;
		case TONG_ACTION_CHANGE_MALE_AGNAME:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeSexAgname(Oper->szData, OBJ_G_MALE);
			break;
		case TONG_ACTION_CHANGE_FEMALE_AGNAME:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeSexAgname(Oper->szData, OBJ_G_FEMALE);
			break;
		case TONG_ACTION_CHANGE_CAMP_JUSTIE:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyTongChangeCamp(camp_justice);
			break;
		case TONG_ACTION_CHANGE_CAMP_EVIL:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyTongChangeCamp(camp_evil);
			break;
		case TONG_ACTION_CHANGE_CAMP_BALANCE:
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyTongChangeCamp(camp_balance);
			break;
		}
	}
	break;

	//¶ÔÉêÇë¼ÓÈëµÄ´ð¸´
	//uPAram = (KUiPlayerItem *) pTarget   ÉêÇë·½
	//nParam : != 0  Í¬Òâ   ==0  ¾Ü¾ø
	case GTOI_TONG_JOIN:
		if (uParam)
		{
			KUiPlayerItem* pItem = (KUiPlayerItem*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cTong.AcceptMember(pItem->nIndex, g_FileName2Id(pItem->Name), nParam);
		}
		break;

	case GTOI_TONG_JOIN_REPLY:
		if (uParam)
		{
			KUiPlayerItem* pItem = (KUiPlayerItem*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cTong.ReplyMember(pItem->nIndex, g_FileName2Id(pItem->Name), nParam);
		}
		break;

	case GTOI_REQUEST_PLAYER_TONG:	//²éÑ¯Ä³ÈËÊÇÄÄ°ï»á
		//uParam = (KUiPlayerItem*) Òª²éË­
		//nParam = (INT)(bool)bReturnTongDetail ÊÇ·ñÒª·µ»ØÄÇ¸ö°ï»áµÄÐÅÏ¢
		if (uParam)
		{
			KUiPlayerItem* pItem = (KUiPlayerItem*)uParam;
			if (pItem->nIndex == Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					pItem->nIndex, 0, 0);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					pItem->nIndex, 0, 0);
			}
		}
		break;
	case GTOI_REQUEST_TONG_DATA:     //ÒªÇóÄ³¸ö°ï»áµÄ¸÷ÖÖ×ÊÁÏ
		//uParam = (KUiGameObjectWithName*)pTong Òª²éÑ¯µÄ°ï»á
		//KUiGameObjectWithName::szName °ï»áµÄÃû×Ö
		//KUiGameObjectWithName::nData ×ÊÁÏµÄÖÖÀà£¬ÖµÈ¡×ÔÃ¶¾ÙTONG_MEMBER_FIGURE
		//			ÁÐ±íµÄÖÖÀàÊÇenumTONG_FIGURE_MASTERµÄ»°´ú±íÒªÇóµÄÊÇ°ï»áµÄ×ÊÑ¶¡£
		//KUiGameObjectWithName::nParam ¿ªÊ¼µÄË÷Òý
		if (uParam)
		{
			if (Player[CLIENT_PLAYER_INDEX].m_cTong.CheckIn() == 0)
				break;

			KUiGameObjectWithName* pObj = (KUiGameObjectWithName*)uParam;
			char	szTongName[32];
			DWORD	dwTongNameID;

			szTongName[0] = 0;
			Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongName(szTongName);
			if (!szTongName[0])
				break;

			// Ö»ÄÜ²éÑ¯×Ô¼º°ï»áÐÅÏ¢
			dwTongNameID = g_FileName2Id(pObj->szName);
			if (dwTongNameID != Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongNameID())
				break;

			switch (pObj->nData)
			{
			case enumTONG_FIGURE_MASTER:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					Player[CLIENT_PLAYER_INDEX].m_nIndex, 0, 0);
				break;
			case enumTONG_FIGURE_DIRECTOR:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					Player[CLIENT_PLAYER_INDEX].m_nIndex, pObj->nParam, defTONG_ONE_PAGE_MAX_NUM);
				break;
			case enumTONG_FIGURE_MANAGER:
				if (!Player[CLIENT_PLAYER_INDEX].m_cTong.CanGetManagerInfo(dwTongNameID))
					break;
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_MANAGER,
					dwTongNameID, pObj->nParam, defTONG_ONE_PAGE_MAX_NUM);
				break;
			case enumTONG_FIGURE_MEMBER:
				if (!Player[CLIENT_PLAYER_INDEX].m_cTong.CanGetMemberInfo(dwTongNameID))
					break;
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_MEMBER,
					dwTongNameID, pObj->nParam, defTONG_ONE_PAGE_MAX_NUM);
				break;
			}
		}
		break;
	case GTOI_TONG_MONEY_ACTION:
		if (uParam)
		{
			KUiPlayerItem* pItem = (KUiPlayerItem*)uParam;
			switch (nParam)
			{
			case TONG_ACTION_SAVE:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplySaveMoney(pItem->nData);
				break;
			case TONG_ACTION_GET:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyGetMoney(pItem->nData);
				break;
			}
		}
		break;
	case GTOI_TONG_CHANGE_CREDIT:
		if (uParam)
		{
			KTongInfo* pItem = (KTongInfo*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeJiyu(pItem->szName, pItem->nTongJiyuParam, pItem->szTongJiyuNotify);
		}
		break;
	case GTOI_TOTAL_EFF:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTong.m_dwTotalEff;
		break;
	case GTOI_TONG_PARAM:
		KTongParam* pParam = (KTongParam*)uParam;
		*pParam = TongData.m_sTongParam;
		break;
	}
	return nRet;
}

//Óë×é¶ÓÏà¹ØµÄ²Ù×÷£¬uOperµÄÈ¡ÖµÀ´×Ô GAME_TEAM_OPERATION_INDEX
INT KCoreShell::TeamOperation(UINT uOper, UINT uParam, INT nParam)
{
	INT nRet = 0;
	switch (uOper)
	{
	case TEAM_OI_GD_INFO:		//Ö÷½ÇËùÔÚµÄ¶ÓÎéÐÅÏ¢
		if (uParam)
		{
			KUiPlayerTeam* pTeam = (KUiPlayerTeam*)uParam;
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.GetInfo(pTeam);
		}
		break;
	case TEAM_OI_GD_FIGURE:
		if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure;
		else
			nRet = -1;
		break;
	case TEAM_OI_GD_MEMBER_LIST://»ñÈ¡Ö÷½ÇËùÔÚ¶ÓÎé³ÉÔ±ÁÐ±í
		nRet = g_Team[0].GetMemberInfo((KUiPlayerItem*)uParam, nParam);
		break;
	case TEAM_OI_GD_REFUSE_INVITE_STATUS://»ñÈ¡¾Ü¾øÑûÇëµÄ×´Ì¬
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.GetAutoRefuseState();
		break;
	case TEAM_OI_COLLECT_NEARBY_LIST://»ñÈ¡ÖÜÎ§¶ÓÎéµÄÁÐ±í
		NpcSet.GetAroundOpenCaptain(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp);
		break;
	case TEAM_OI_APPLY:			//ÉêÇë¼ÓÈëËýÈË¶ÓÎé
		if (uParam)
			Player[CLIENT_PLAYER_INDEX].ApplyAddTeam(((KUiTeamItem*)uParam)->Leader.nIndex);
		break;
	case TEAM_OI_CREATE:		//ÐÂ×é¶ÓÎé
		Player[CLIENT_PLAYER_INDEX].ApplyCreateTeam();//(char*)uParam);
		break;
	case TEAM_OI_APPOINT:		//ÈÎÃü¶Ó³¤£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		Player[CLIENT_PLAYER_INDEX].ApplyTeamChangeCaptain(((KUiPlayerItem*)uParam)->uId);
		break;
	case TEAM_OI_INVITE:		//ÑûÇë±ðÈË¼ÓÈë¶ÓÎé£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		if (uParam)
		{
			Player[CLIENT_PLAYER_INDEX].TeamInviteAdd(((KUiPlayerItem*)uParam)->uId);
		}
		break;
	case TEAM_OI_KICK:			//Ìß³ý¶ÓÀïµÄÒ»¸ö¶ÓÔ±£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		Player[CLIENT_PLAYER_INDEX].TeamKickMember(((KUiPlayerItem*)uParam)->uId);
		break;
	case TEAM_OI_LEAVE:			//Àë¿ª¶ÓÎé
		Player[CLIENT_PLAYER_INDEX].LeaveTeam();
		break;
	case TEAM_OI_CLOSE:			//¹Ø±Õ×é¶Ó£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		Player[CLIENT_PLAYER_INDEX].ApplyTeamOpenClose(nParam);
		break;
	case TEAM_OI_PKFOLLOW:			//¹Ø±Õ×é¶Ó£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		Player[CLIENT_PLAYER_INDEX].ApplyTeamPKFollow(nParam);
		break;
	case TEAM_OI_MODEPICK:			//¹Ø±Õ×é¶Ó£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		Player[CLIENT_PLAYER_INDEX].ApplyTeamModePick(nParam);
		break;
	case TEAM_OI_REFUSE_INVITE:		//¾Ü¾ø±ðÈËÑûÇë×Ô¼º¼ÓÈë¶ÓÎé
		Player[CLIENT_PLAYER_INDEX].m_cTeam.SetAutoRefuseInvite(nParam);
		break;
	case TEAM_OI_APPLY_RESPONSE:		//Åú×¼ËûÈË¼ÓÈë¶ÓÎé£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		if (uParam)
		{
			if (nParam)
			{
				Player[CLIENT_PLAYER_INDEX].AcceptTeamMember(((KUiPlayerItem*)uParam)->uId);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cTeam.DeleteOneFromApplyList(((KUiPlayerItem*)uParam)->uId);
				//				Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
			}
		}
		break;
	case TEAM_OI_INVITE_RESPONSE://¶Ô×é¶ÓÑûÇëµÄ»Ø¸´
		if (uParam)
			Player[CLIENT_PLAYER_INDEX].m_cTeam.ReplyInvite(((KUiPlayerItem*)uParam)->nIndex, nParam);
		break;
	case TEAM_OI_GETUID:		//ÈÎÃü¶Ó³¤£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		nRet = ((KUiPlayerItem*)uParam)->uId;
		break;
	}
	return nRet;
}


INT KCoreShell::PAIOperation(UINT uOper, UINT uParam, INT nParam, INT nParam1)
{
	INT nRet = 0;
	switch (uOper)
	{
	case GPI_SWITCH_ACTIVE:
	{

		if (nParam == 1)
		{
			if (Player[CLIENT_PLAYER_INDEX].m_cAI.m_bIsActive == 1)
			{
				uParam = 0;
			}
			else
			{
				uParam = 1;
			}
		}

		if (uParam == 1)
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bIsActive = 1;
			Player[CLIENT_PLAYER_INDEX].m_cAI.SetSortEquipment(FALSE);
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_bIsActive = 0;
			Player[CLIENT_PLAYER_INDEX].m_cAI.SetSortEquipment(FALSE);
		}
	}
	/*Player[CLIENT_PLAYER_INDEX].m_cAI.m_bIsActive = !Player[CLIENT_PLAYER_INDEX].m_cAI.m_bIsActive;
	Player[CLIENT_PLAYER_INDEX].m_cAI.SetSortEquipment(FALSE);*/
	break;
	case GPI_ISACTIVE:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cAI.m_bIsActive;
		break;
	case GPI_PRIORITY_USE_MOUSE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPriorityUseMouse = uParam;
		break;
	case GPI_HOLDING_SPACEBAR:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bHoldSpaceBar = uParam;
		break;
	case GPI_FOLLOW_PEOPLE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFollowPeople = uParam;
		break;
	case GPI_FOLLOW_PEOPLE_NAME:
		strcpy(Player[CLIENT_PLAYER_INDEX].m_cAI.m_szFollowName, (char*)uParam);
		break;
	case GPI_FOLLOW_PEOPLE_RADIUS:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFollowRadius = uParam;
		break;
	case GPI_FIGHTNEAR_CHECK:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFightNear = uParam;
		break;
	case GPI_ATTACK_NPC_CHECK:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAttackNpc = uParam;
		break;
	case GPI_AUTO_PARTY:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoParty = uParam;
		break;
	case GPI_AUTO_INVITE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoInvite = uParam;
		break;
	case GPI_AUTO_INVITE_LIST:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoInviteList = uParam;
		break;
	case GPI_SET_INVITE_LIST:
		if (nParam >= 0 && nParam < MAX_AUTO_LIST)
			strcpy(Player[CLIENT_PLAYER_INDEX].m_cAI.m_szPartyList[nParam], (char*)uParam);
		break;
	case GPI_AUTO_ACCEPT:
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_bAutoAccept = uParam;
		break;
	case GPI_AUTO_ACCEPT_WITH_NAME:
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_bAutoAcceptWithName = uParam;
		break;
	case GPI_FIGHT_AUTO:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nDistanceAuto = uParam;
		break;
	case GPI_RANGER_AUTO:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nRadiusAuto = uParam;
		break;
	case GPI_FIGHT_USE_SB:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFightUseSB = uParam;
		break;
	case GPI_SUPPORT_SKILLS:
		if (uParam >= 0 && uParam < MAX_AUTO_STATESKILL && nParam < MAX_SKILL)
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nSupportSkills[uParam] = nParam;
		break;
	case GPI_FIGHT_SKILLS:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFightSkill = uParam;
		break;
	case GPI_FIGHTBACK:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFightBack = uParam;
		break;
	case GPI_FIGHTBOSS:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFightBoss = uParam;
		break;
	case GPI_AURA_SKILLS1:
		if (nParam >= 0 && nParam < MAX_SKILL)
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nActiveAuraID1 = uParam;
		break;
	case GPI_AURA_SKILLS2:
		if (nParam >= 0 && nParam < MAX_SKILL)
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nActiveAuraID2 = uParam;
		break;
	case GPI_OVER_TARGET:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bOverTarget = uParam;
		break;
	case GPI_FOLLOW_TARGET:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFollowTarget = uParam;
		break;
		//TamLTM QuanhDiem
	case GPI_QUANH_DIEM_TARGET:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bQuanhDiem = uParam;
		break;
	case GPI_SORT_EQUIPMENT:
		//Player[CLIENT_PLAYER_INDEX].m_cAI.m_bSortEquipment = uParam;
		Player[CLIENT_PLAYER_INDEX].m_cAI.SetSortEquipment(uParam);
		break;
	case GPI_FILTER_EQUIPMENT:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFilterEquipment = uParam;
		break;
	case GPI_PICK_FIGHTNONE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPickFightNone = uParam;
		break;
	case GPI_PICK_MONEY:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPickMoney = uParam;
		break;
	case GPI_PICK_NOTEQUIP:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPickNotEquip = uParam;
		break;
	case GPI_PICK_EQUIP:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bPickEquip = uParam;
		break;
	case GPI_FOLLOW_PICK:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bFollowPick = uParam;
		break;
	case GPI_PICK_KIND:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nPickEquipKind = uParam;
		break;
	case GPI_AUTO_FILTERMAGIC:
	{
		INT nIndex = (INT)g_MagicDesc.String2MagicID((char*)uParam);
		if (nIndex >= magic_skill_begin && nIndex < magic_normal_end)
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFilterMagic[nParam1][0] = nIndex;
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFilterMagic[nParam1][1] = nParam;
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFilterMagic[nParam1][0] = 0;
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFilterMagic[nParam1][1] = 0;
		}
	}
	break;
	case GPI_SAVE_TRASH:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bSaveTrash = uParam;
		break;
	case GPI_SELL_EQUIPMENT:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bSellEquipment = uParam;
		break;
	case GPI_SAVE_TRASH_PRICE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nSaveTrashPrice = uParam;
		break;
	case GPI_SAVE_JEWELRY:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bSaveJewelry = uParam;
		break;
	case GPI_EAT_LIFE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bEatLife = uParam;
		break;
	case GPI_EAT_MANA:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bEatMana = uParam;
		break;
	case GPI_LIFE_VALUE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nLifeAutoV = uParam;
		break;
	case GPI_LIFE_PERCENT:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nLifeAutoP = uParam;
		break;
	case GPI_LIFE_TIME:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_uLifeTimeUse = uParam;
		break;
	case GPI_MANA_VALUE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nManaAutoV = uParam;
		break;
	case GPI_MANA_PERCENT:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nManaAutoP = uParam;
		break;
	case GPI_MANA_TIME:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_uManaTimeUse = uParam;
		break;
	case GPI_TP_LIFE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bTPLife = uParam;
		break;
	case GPI_TP_LIFEV:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nTPLifeV = uParam;
		break;
	case GPI_TP_MANA:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bTPMana = uParam;
		break;
	case GPI_TP_MANAV:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nTPManaV = uParam;
		break;
	case GPI_TP_NOT_MEDICINEBLOOD:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bTPNotMedicineBlood = uParam;
		break;
	case GPI_TP_NOT_MEDICINEMANA:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bTPNotMedicineMana = uParam;
		break;
	case GPI_TP_HIGHTMONEY:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bTPHightMoney = uParam;
		break;
	case GPI_TP_HIGHTMONEYV:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nTPHightMoneyV = uParam;
		break;
	case GPI_TP_DAMAGEEQUIP:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bTPDamageEquip = uParam;
		break;
	case GPI_TP_DAMAGEEQUIPV:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nTPDamageEquipV = uParam;
		break;
	case GPI_TP_NOT_EQUIPMENT:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bTPNotEquipment = uParam;
		break;
	case GPI_TP_NOT_EQUIPMENTV:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nTPNotEquipmentV = uParam;
		break;
	case GPI_ANTI_POISON:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAntiPoison = uParam;
		break;
	case GPI_ENCHASEEXP:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bEnchaseExp = uParam;
		break;
	case GPI_LIFEREPLENISH:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bLifeReplenish = uParam;
		break;
	case GPI_LIFEREPLENISHP:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nLifeReplenishP = uParam;
		break;
	case GPI_INVENTORYMONEY:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bInventoryMoney = uParam;
		break;
	case GPI_INVENTORYITEM:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bInventoryItem = uParam;
		break;
	case GPI_RETURNPORTAL:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bReturnPortal = uParam;
		break;
	case GPI_RETURNPORTALSEC:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nReturnPortalSec = uParam;
		break;
	case GPI_OPEN_MEDICINE:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bOpenMedicine = uParam;
		break;
	case GPI_AUTO_REPAIR:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bRepairEquip = uParam;
		break;
	case GPI_WITHDRAW:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bWithDrawMoney = uParam;
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nWithDrawMoneyV = nParam;
		break;
	case GPI_AUTO_MOVEMPS:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_bMoveMps = uParam;
		break;
	case GPI_AUTO_MOVEMPSID:
		if (uParam >= 0 && uParam < MAX_AUTO_LIST)
		{
			if (uParam == 0)
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_nMoveCount = 0;

			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nMoveMps[uParam][0] = nParam;
			if (nParam)
				Player[CLIENT_PLAYER_INDEX].m_cAI.m_nMoveCount++;
		}
		break;
	case GPI_AUTO_MOVEMPSX:
		if (uParam >= 0 && uParam < MAX_AUTO_LIST)
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nMoveMps[uParam][1] = nParam;
		break;
	case GPI_AUTO_MOVEMPSY:
		if (uParam >= 0 && uParam < MAX_AUTO_LIST)
			Player[CLIENT_PLAYER_INDEX].m_cAI.m_nMoveMps[uParam][2] = nParam;
		break;
	case GPI_GOTO_MAP:
		Player[CLIENT_PLAYER_INDEX].m_cAI.m_nWayPointId = nParam;
		break;
	}
	return nRet;
}

void KCoreShell::DirectFindPos(UINT uParam, INT nParam, BOOL bSync, BOOL bPaintLine)
{
	g_ScenePlace.DirectFindPos(uParam, nParam, bSync, bPaintLine);

	//TamLTM check lam auto run move mini map
	//Player[CLIENT_PLAYER_INDEX].m_cAI.m_nMapX = uParam; // auto run;
	//Player[CLIENT_PLAYER_INDEX].m_cAI.m_nMapY = nParam; // auto run;
	//end code
}

//TamLTM Check Hover mini map
void KCoreShell::CheckHoverMouseMiniMap(BOOL hoverM)
{
	g_ScenePlace.CheckHoverMouseMiniMap(hoverM);
}
//end code

//Nhap box toa do thi set true
void KCoreShell::SetAutoRun(BOOL autoRun)
{
	Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoRunMap = autoRun; // auto run;
	//	g_DebugLog("%d ", autoRun);
}

//Cam co toa do thi set true
void KCoreShell::SetFlagAutoRun(BOOL autoRunFlag, INT x, INT y)
{
	Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoRunFlagMap = autoRunFlag; // auto run;
	Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFlagMapX = x; // auto run;
	Player[CLIENT_PLAYER_INDEX].m_cAI.m_nFlagMapY = y; // auto run;
	//	g_DebugLog("autoRunFlag %d ", autoRunFlag);
}
//end code

BOOL KCoreShell::GetPaintMode()
{
	return g_ScenePlace.bPaintMode;
}

void KCoreShell::SetPaintMode(BOOL bFlag)
{
	g_ScenePlace.DirectFindPos(0, 0, FALSE, FALSE);
	g_ScenePlace.bPaintMode = bFlag;
}

BOOL KCoreShell::GetFlagMode()
{
	return g_ScenePlace.bFlagMode;
}

void KCoreShell::SetFlagMode(BOOL bFlag)
{
	g_ScenePlace.bFlagMode = bFlag;

	//Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoRunMap = FALSE; // auto run;
	//Player[CLIENT_PLAYER_INDEX].m_cAI.m_bAutoRunFlagMap = FALSE;
}

void KCoreShell::GetSizeItem(UINT uItemId, INT* nW, INT* nH)
{
	if (nW && nH)
	{
		*nW = Item[uItemId].GetWidth();
		*nH = Item[uItemId].GetHeight();
	}
}

INT KCoreShell::GetExtPoint()
{
	return  Player[CLIENT_PLAYER_INDEX].GetExtPoint();
}

INT KCoreShell::GetNatureItem(UINT uItemId, UINT uGenre /* = CGOG_ITEM*/)
{
	INT nIndex = uItemId;
	KItem* pItem = NULL;

	// Kiểm tra xem chỉ số mục nằm trong phạm vi hợp lệ
	if (nIndex >= 0 && nIndex < MAX_ITEM) {
		// Trả về thuộc tính tự nhiên của mục nếu thể loại là CGOG_ITEM
		if (uGenre == CGOG_ITEM)
			return Item[nIndex].GetNature();
		// Xử lý các trường hợp khác nhau nếu mục thuộc loại NPCSELL, PLAYERSELL hoặc PLAYERVIEW
		else if (pItem && (uGenre == CGOG_NPCSELLITEM || uGenre == CGOG_PLAYERSELLITEM || uGenre == CGOG_PLAYERVIEWITEM))
		{
			// Lấy chỉ số cửa hàng mà người chơi đang thăm
			INT nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop];
			// Kiểm tra nếu người chơi đang ở trong cửa hàng
			if (nBuyIdx != -1)
			{
				// Lấy chỉ số của mục trong cửa hàng bán hàng
				nIndex = BuySell.GetItemIndex(nBuyIdx, uItemId);
				// Kiểm tra xem mục có tồn tại trong cửa hàng không
				if (nIndex >= 0)
				{
					// Trả về thuộc tính tự nhiên của mục trong cửa hàng bán hàng
					pItem = BuySell.GetItem(nIndex);
					return pItem->GetNature();
				}
			}
			// Trả về thuộc tính tự nhiên của mục nếu không thỏa mãn các điều kiện trên
			return Item[nIndex].GetNature();
		}
	}
	// Trả về -1 nếu chỉ số mục không hợp lệ hoặc không thỏa mãn các điều kiện trên
	return -1;
}


INT KCoreShell::GetGenreItem(UINT uItemId, UINT uGenre /* = CGOG_ITEM*/)
{
	INT nIndex = uItemId;
	KItem* pItem = NULL;
	if (nIndex >= 0 && nIndex < MAX_ITEM) {
		if (uGenre == CGOG_ITEM)
			return Item[nIndex].GetGenre();
		else if (pItem && (uGenre == CGOG_NPCSELLITEM || uGenre == CGOG_PLAYERSELLITEM || uGenre == CGOG_PLAYERVIEWITEM))
		{
			INT	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop];
			if (nBuyIdx != -1)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, uItemId);
				if (nIndex >= 0)
				{
					pItem = BuySell.GetItem(nIndex);
					return pItem->GetGenre();
				}
			}
		}
		return Item[nIndex].GetGenre();
	}
	else
		return -1;
}

INT KCoreShell::IsDamage(UINT uItemId)
{
	if (Item[uItemId].GetDurability() > 0)
		return Item[uItemId].GetDurability() < Item[uItemId].GetMaxDurability();

	return FALSE;
}

BOOL KCoreShell::GetLockState()
{
	return Player[CLIENT_PLAYER_INDEX].GetLockState();
}

INT KCoreShell::GetStackNum(UINT uItemId)
{
	return Item[uItemId].GetStackNum();
}

BOOL KCoreShell::GetSkillData(INT nSkillId, INT* nLevel)
{
	INT nList = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(nSkillId);
	if (nList > 0)
	{
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLevel(nSkillId) > 0)
		{
			*nLevel = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillId);
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetAddLevelIndex(nList) > 0)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}

void KCoreShell::GetSkillName(INT nSkillId, char* szSkillName)
{
	if (nSkillId)
	{
		ISkill* pISkill = g_SkillManager.GetSkill(nSkillId, 1);
		strcpy(szSkillName, pISkill->GetSkillName());
	}
}

INT KCoreShell::GetTradePrice(UINT uItemId)
{
	return Item[uItemId].GetTradePrice();
}

INT KCoreShell::GetTradeState(INT nIndex, INT uId)
{
	if (nIndex || uId)
	{
		if (nIndex)
			return Npc[nIndex].m_PTrade.nTrade;
		else
		{
			INT nIndex = NpcSet.SearchID(uId);
			return Npc[nIndex].m_PTrade.nTrade;
		}
	}
	else
		return Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_PTrade.nTrade;

}

INT KCoreShell::GetFightState()
{
	KNpc* pNpc = &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex];
	return pNpc->m_FightMode;
}

void KCoreShell::BreakItem(UINT uParam, INT nParam, BOOL bIsBreakAll)
{
	KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
	if (CGOG_ITEM != pObject1->Obj.uGenre)
		return;
	//·ÅÏÂÈ¥µÄ¶«Î÷²»Îª¿Õ£¬ËùÒÔÊÇÂô¶«Î÷
	INT nIdx = pObject1->Obj.uId;	//Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
	if (nIdx > 0 && nIdx < MAX_ITEM)
	{
		if (nParam <= 0)
		{
			SendClientCmdBreak(Item[nIdx].GetID(), nParam, FALSE);
		}
		else
		{
			if (Item[nIdx].GetStackNum() > nParam)
				SendClientCmdBreak(Item[nIdx].GetID(), nParam, bIsBreakAll);
			else
				return;
		}
	}
}

DWORD KCoreShell::GetdwIDPlayer(UINT uId)
{
	INT nIndex = 0;
	if (uId)
		nIndex = NpcSet.SearchID(uId);
	else
		nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (nIndex)
		return Npc[nIndex].m_dwID;

	return 0;
}

INT KCoreShell::GetOwnValue(INT nMoneyUnit)
{
	INT nRet = 0;
	switch (nMoneyUnit)
	{
	case moneyunit_money:
		nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);
		break;
	case moneyunit_coin:
		nRet = Player[CLIENT_PLAYER_INDEX].GetCoin();
		break;
	case moneyunit_extpoint:
		nRet = Player[CLIENT_PLAYER_INDEX].GetExtPoint();
		break;
	case moneyunit_fuyuan:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_STATTASK_FUYUAN);
		break;
	case moneyunit_repute:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_STATTASK_REPUTE);
		break;
	case moneyunit_accum:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_STATTASK_ACCUM);
		break;
	case moneyunit_honor:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_STATTASK_HONOR);
		break;
	case moneyunit_respect:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_STATTASK_RESPECT);
		break;
	default:
		break;
	}
	return nRet;
}

INT KCoreShell::GetDataSuperShop(INT nSaleId, UINT uParam, INT nParam)
{
	INT nRet = 0;
	INT	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[nSaleId];
	if (nBuyIdx == -1)
		return 0;
	if (nBuyIdx >= BuySell.GetHeight())
		return 0;
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop = nSaleId;
	INT nIndex = 0;
	INT nCount = 0;
	if (uParam)
	{
		INT nPage = 0;
		KUiObjAtContRegion* pInfo = (KUiObjAtContRegion*)uParam;
		for (INT i = 0; i < BuySell.GetWidth(); i++)
		{
			nIndex = BuySell.GetItemIndex(nBuyIdx, i);
			KItem* pItem = BuySell.GetItem(nIndex);

			if (nIndex >= 0 && pItem)
			{
				pInfo->Obj.uGenre = CGOG_NPCSELLITEM;
				pInfo->Obj.uId = i;
				pInfo->Region.h = 0;
				pInfo->Region.v = 0;
				pInfo->Region.Width = pItem->GetWidth();
				pInfo->Region.Height = pItem->GetHeight();
				pInfo->nContainer = nPage;
				nCount++;
				pInfo++;
				if (nCount % 15 == 0)
					nPage++;
			}
		}
		nRet = nPage;
	}
	else
	{
		for (INT i = 0; i < BuySell.GetWidth(); i++)
		{
			nIndex = BuySell.GetItemIndex(nBuyIdx, i);
			KItem* pItem = BuySell.GetItem(nIndex);

			if (nIndex >= 0 && pItem)
			{
				nCount++;
			}
		}
		nRet = nCount;
	}
	return nRet;
}


INT KCoreShell::GetDataDynamicShop(INT nSaleId, UINT uParam, INT nParam)
{
	INT nRet = 0;
	INT	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nShopIdx[nSaleId];
	if (nBuyIdx == -1)
		return 0;
	if (nBuyIdx >= BuySell.GetHeight())
		return 0;
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nCurShop = nSaleId;
	INT nIndex = 0;
	INT nCount = 0;
	if (uParam)
	{
		INT nPage = 0;
		KUiObjAtContRegion* pInfo = (KUiObjAtContRegion*)uParam;
		if (!BuySell.m_pSShopRoom)
			return 0;
		BuySell.m_pSShopRoom->Clear();
		for (INT i = 0; i < BuySell.GetWidth(); i++)
		{
			nIndex = BuySell.GetItemIndex(nBuyIdx, i);
			KItem* pItem = BuySell.GetItem(nIndex);

			if (nIndex >= 0 && pItem)
			{
				pInfo->Obj.uGenre = CGOG_NPCSELLITEM;
				pInfo->Obj.uId = i;
				POINT	Pos;
				if (BuySell.m_pSShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos))
				{
					BuySell.m_pSShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
				}
				else
				{
					nPage++;
					BuySell.m_pSShopRoom->Clear();
					BuySell.m_pSShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos);
					BuySell.m_pSShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
				}
				pInfo->Region.h = Pos.x;
				pInfo->Region.v = Pos.y;
				pInfo->Region.Width = pItem->GetWidth();
				pInfo->Region.Height = pItem->GetHeight();
				pInfo->nContainer = nPage;
				nCount++;
				pInfo++;
			}
		}
		nRet = nPage;
	}
	else
	{
		for (INT i = 0; i < BuySell.GetWidth(); i++)
		{
			nIndex = BuySell.GetItemIndex(nBuyIdx, i);
			KItem* pItem = BuySell.GetItem(nIndex);

			if (nIndex >= 0 && pItem)
			{
				nCount++;
			}
		}
		nRet = nCount;
	}
	return nRet;
}

INT KCoreShell::GetNextSkill(INT nType, INT nIndex)
{
	if (nType == 0)
		return Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetNextSkillState(nIndex);
	else if (nType == 1)
		return Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetNextSkillFight(nIndex);
	else if (nType == 2)
		return Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetNextSkillAura(nIndex);

	return 0;
}

BOOL KCoreShell::IsSkillAura(INT nSkillId)
{
	if (nSkillId > 0)
	{
		KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(nSkillId, 1);
		if (!pOrdinSkill)
			return FALSE;

		return pOrdinSkill->IsAura();
	}
	return FALSE;
}

//TamLTM kham nam xanh
INT KCoreShell::GetKindItem(UINT uId)
{
	return Item[uId].GetKind();
}

INT KCoreShell::GetDetailItem(UINT uId)
{
	return Item[uId].GetDetailType();
}

INT KCoreShell::GetLevelItem(UINT uId)
{
	return Item[uId].GetLevel();
}

INT KCoreShell::GetSeriesItem(UINT uId)
{
	return Item[uId].GetSeries();
}

INT KCoreShell::GetItemColor(UINT uItemId)
{
	return Item[uItemId].GetColorItem();
}

INT KCoreShell::GetParticularItem(UINT uId)
{
	return Item[uId].GetParticular();
}
//end code

void KCoreShell::GetWayPointName(INT nWayPointID, char* szMapName) {
	KTabFile g_WayPointTabFile;

	if (!g_WayPointTabFile.Load(WORLD_WAYPOINT_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD WayPoint Setting File %s", WORLD_WAYPOINT_TABFILE);
	}
	g_WayPointTabFile.GetString(nWayPointID + 1, "DESC", NORMAL_UNCLEAR_WORD, szMapName, 50);
}

void KCoreShell::GetWayPointPos(INT nWayPointID, INT* nWorld, INT* nPosX, INT* nPosY) {
	KTabFile g_WayPointTabFile;

	if (!g_WayPointTabFile.Load(WORLD_WAYPOINT_TABFILE))
	{
		g_DebugLog("[TASK]CAN NOT LOAD WayPoint Setting File %s", WORLD_WAYPOINT_TABFILE);
	}
	char szValue[30];
	INT nRow;
	nRow = g_WayPointTabFile.FindColumn("SECT");
	g_WayPointTabFile.GetString(nWayPointID + 1, nRow, "0,0,0", szValue, 30);

	INT nX, nY, nW;
	const char* pcszTemp = szValue;

	nW = KSG_StringGetInt(&pcszTemp, 0);
	KSG_StringSkipSymbol(&pcszTemp, ',');
	nX = KSG_StringGetInt(&pcszTemp, 0);
	KSG_StringSkipSymbol(&pcszTemp, ',');
	nY = KSG_StringGetInt(&pcszTemp, 0);

	nWorld = &nW;
	nPosX = &nX;
	nPosY = &nY;
}