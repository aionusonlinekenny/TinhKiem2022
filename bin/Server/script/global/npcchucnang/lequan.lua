--hoangnhk
Include("\\script\\header\\taskid.lua")
Include("\\script\\lib\\worldlibrary.lua")
END_TALK = "KÕt thóc ®èi tho¹i/no"
RESET_TALK =
	"§¹i hiÖp sÏ ®­îc xãa cµi ®Æt mËt m· vµo ngµy <color=Yellow>%s<color> nÕu sau 24h ngµy <color=Yellow>%s<color> kh«ng x¸c nhËn th× ®¨ng ký xãa cµi ®Æt mËt m· sÏ bÞ hñy bá "
function main(NpcIndex)
	-- if (SERVER_MODE == 1) then
	-- 	Talk(1, "", SERVER_NOTICE)
	-- 	return
	-- end
	local LEQUAN_ANSWER = {}
	tinsert(LEQUAN_ANSWER, "Mua §¹o cô/muadaocu")
	tinsert(LEQUAN_ANSWER, "Xãa cµi ®Æt mËt m·/xoamakhoa")
	--if(tonumber(GetLocalDate("%w")) == 6) then-- thu 7 hang tuan
	tinsert(LEQUAN_ANSWER, "§æi r­¬ng B¹ch Kim/doi_bk")
	tinsert(LEQUAN_ANSWER, "Thu mua HKMP r¸c/doi_hk")
	--end;
	tinsert(LEQUAN_ANSWER, "Hñy ®¹o cô khãa vÜnh viÔn/huyvp")
	tinsert(LEQUAN_ANSWER, END_TALK)
	Say("LINK:<npc>", 0, 11316, getn(LEQUAN_ANSWER), LEQUAN_ANSWER)
end

function muadaocu()
	Sale(102)
end

function huyvp()
	if (GetLockState() == 1) then
		Msg2Player("B¹n ph¶i më khãa b¶o vÖ tr­íc, míi cã thÓ hñy trang bÞ")
	end
	GiveItemUI("Hñy vËt phÈm", "§¹i hiÖp h·y cÈn träng trong viÖc hñy vËt phÈm", "huyvp1", "onCancel")
	--huyvp1()
end

function huyvp1()
	local count, nFIndex, nFGenre = 0, 0, 0
	for i = 0, 5 do
		for j = 0, 3 do
			nFIndex, nFGenre = FindItemEx(14, i, j)
			if (nFIndex > 0 and nFGenre == 0 and GetLockItem(nFIndex) == -2) then --trang bi, khoa vinh vien
				if (RemoveItem(nFIndex, 1) == 1) then
					count = count + 1
				end
			end
		end
	end
	if count > 0 then
		Msg2Player(format("§¹i hiÖp võa hñy thµnh c«ng %d vËt phÈm", count))
	end
	return 1
end

function xoamakhoa()
	local nTaskValue = GetTask(TASK_THOIGIAN7)
	if (GetNumber(4, nTaskValue, 2) > 0) then
		Say(
			"§¹i hiÖp t×m ta cã viÖc g×?",
			3,
			"Xem thêi gian hoµn thµnh xãa cµi ®Æt mËt m·/ktthoigian",
			"Hñy xãa cµi ®Æt mËt m·/huyxoa",
			END_TALK
		)
	else
		Say("§¹i hiÖp t×m ta cã viÖc g×?", 2, "§¨ng ký xãa cµi ®Æt mËt m·/xnxoa", END_TALK)
	end
end

function xnxoa()
	local nValue = GetCurServerSec() + 7 * 24 * 60 * 60
	SetTask(TASK_THOIGIAN7, nValue)
	local szMsg = GetTimeDate("%Y-%m-%d %H:%M:%S", nValue)
	Talk(1, "", format(RESET_TALK, szMsg, szMsg))
end

function ktthoigian()
	local nValue = GetTask(TASK_THOIGIAN7)
	local nCurrent = GetCurServerSec()
	if (nValue < nCurrent) then
		local szMsg = GetTimeDate("%Y-%m-%d %H:%M:%S", nValue)
		Talk(1, "", format(RESET_TALK, szMsg, szMsg))
	else
		SetSavePw("")
		SetTask(TASK_THOIGIAN7, 0)
		Talk(1, "", "§· hñy m· khãa r­¬ng!")
	end
end

function huyxoa()
	SetTask(TASK_THOIGIAN7, 0)
	Msg2Player("Hñy bá xãa cµi ®Æt mËt m·")
end

function no()
end

function doi_hk()
	GiveItemUI("Thu mua HKMP", "§¹i hiÖp h·y ®Æt mçi lÇn 1 trang bÞ", "doi_hk1", "onCancel")
end

function doi_hk1()
	local nCount, nFIndex = 0, 0
	for i = 0, 5 do
		for j = 0, 3 do
			nFIndex = FindItemEx(14, i, j)
			if (nFIndex > 0 and GetGoldIndex(nFIndex) >= 0 and GetGoldIndex(nFIndex) <= 141 and GetLockItem(nFIndex) == 0) then --trang bi, khoa vinh vien
				if (RemoveItem(nFIndex, 1) == 1) then
					nCount = nCount + 1
					AddStackItem(10, 6, 4815, 0, 0, 5, 0, 0)
				end
			end
		end
	end

	if (nCount > 0) then
		Msg2Player(format("B¸n thµnh c«ng %d trang bÞ HKMP!", nCount))
	end
end

function doi_bk()
	if (CalcFreeItemCellCount() < 10) then
		Say("CÇn tèi thiÓu 10 « trèng, xin h·y s¾p xÕp l¹i hµnh trang tr­íc ®·!")
		return
	end

	if (GetItemCount(0, 6, 4815) < 150) then
		Talk(1, "", "Ng­¬i kh«ng mang ®ñ B¹ch Kim LÖnh råi!")
		return
	end

	if (DelItem(150, 0, 6, 4815) ~= 0) then
		AddItemEx(6, 4816, 0, 0, 5, 0, 0)
		Msg2Player(format("NhËn ®­îc 1 <color=yellow>B¶o r­¬ng B¹ch Kim"))
		ExchangeLog()
	end
end

function ExchangeLog()
	local szAccount = GetAccount()
	local szName = GetName()
	local dwID = GetUUID()
	local nLevel = GetLevel()
	local nFaction = GetFaction()
	local nIP = GetIP()
	local log =
		"" ..
		date("%H:%M:%S") ..
			"\t Tµi kho¶n: " ..
				szAccount ..
					"\t Nh©n vËt: " ..
						szName ..
							"\t UUID: " ..
								dwID ..
									"\t CÊp: \t" ..
										nLevel ..
											"\t M«n ph¸i: " ..
												nFaction .. "\t IP: " .. nIP .. "\t ChuyÓn ®æi LÖnh bµi B¹ch Kim thµnh 1 Box B¹ch Kim thµnh c«ng!\n"
	logWrite("exchange_item", log)
end
