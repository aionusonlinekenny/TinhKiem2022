--hoangnhk
Include("\\script\\lib\\worldlibrary.lua")
Include("\\script\\lib\\awardtemplet.lua")
Include("\\script\\header\\factionhead.lua")
Include("\\script\\header\\taskid.lua")
Include("\\script\\header\\dialogsay.lua")
Include("\\script\\header\\testgame.lua")
Include("\\script\\header\\itemblue.lua")
Include("\\script\\header\\thuongtop.lua")
Include("\\script\\header\\vuotai.lua")
Include("\\script\\feature\\van_tieu\\lib.lua")

NEED_EMPTY = "Xin s¾p xÕp hµnh trang 6x6 « trèng"
function main(nIndex)
	dofile("script/global/npcchucnang/trogiup.lua");
	local tb = {}
	local nTaskValue = GetTask(TASK_TANTHU)
	if (SERVER_MODE ~= 0) then
		--tinsert(tb, "NhËn hç trî T§P, THP/hotro1");
		-- if (GetLevel() >= 120 and GetTask(TASK_TOP120) ~= 1) then
		-- 	tinsert(tb, "NhËn quµ cÊp 120/hotro120");
		-- end
		-- if (GetLevel() >= 90 and GetTask(TASK_HOTRO_LOI) ~= 1) then
		-- 	tinsert(tb, "NhËn quµ hç trî m¸y chñ lçi/hotroloi");
		-- end
		if (nTaskValue <= 0) then
			tinsert(tb, "NhËn hç trî t©n thñ/hotronew")
		end
		if (GetFactionNo() >= 0 and GetTask(TASK_KYNANG) ~= 1) then
			tinsert(tb, "NhËn hç trî kü n¨ng 9x/hotrokn")
		end
	else
		if (GetNumber(1, nTaskValue, 4) <= 0) then
			tinsert(tb, "Hç trî thö nghiÖm/test")
		end
		--if (GetFactionNo() >= 0 and GetTask(TASK_KYNANG) ~= 1) then
			tinsert(tb, "Hç trî kü n¨ng/kynangtest")
		--end
		if (GetNumber(1, nTaskValue, 5) <= 0) then
			tinsert(tb, "Hç trî lËp bang/bangtest");
		end;
		tinsert(tb, "NhËn trang bÞ xanh/nhantrangbi")
		--tinsert(tb, "NhËn ng©n l­îng/tientest")
		--tinsert(tb, "NhËn ®iÓm kü n¨ng/nhankynang");
		--tinsert(tb, "NhËn ®iÓm tiÒm n¨ng/nhantiemnang");
		tinsert(tb, "Thanh lý hµnh trang/thanhly")
	end
	-- if (GetLevel() >= 120 and GetTask(TASK_TOP120) == 0) then
	-- 	tinsert(tb, "NhËn th­ëng cÊp 120/duatop");
	-- end;

	--tinsert(tb, "Fix lçi khãa Thæ ®Þa phï/fix_phu");
	tinsert(tb, "Test Game/testrank")
	if (GetAccount() == "dungnd" or GetAccount() == "frankenstein") then
		tinsert(tb, "LÊy lÖnh bµi GM/add_gm")
	end

	tinsert(tb, "KÕt thóc ®èi tho¹i/no")
	Say("LINK:<npc>", random(0, 3), 15277, getn(tb), tb)
end

function nhankynang()
	AddMagicPoint(100)
end

function nhantiemnang()
	AddPropPoint(1000)
end

function fix_phu()
	ForbidUseTownP(0)
end

function testrank()
	for i = 1, 10 do
		AddStackItem(500, 6, 4814, 0, 0, 5, 0, 0)
	end

	for i = 1, 10 do
		AddStackItem(50, 6, 4816, 0, 0, 5, 0, 0)
	end
	--AddMagic(1060,1)
	--Sale(102,0)
	--hotrokn()
	--DelMagic(53)
	--AddMagic(1055,1)
	--SetSeries(3)
	--gianhapmonphai(7)
	--AddMagicPoint(105)
	--hotrokn()
	--Msg2Player(GetFactionNo())
	--ForbidUseTownP(0);
	--AddStackItem(50, 4, 343, 0, 0, 5, 0, 0)
	--NewWorld(950, 1592, 3195)
	--SetFightState(1)
	--tbAwardTemplet:GiveAwardByList({ szName = "LTT", tbProp = { 4, 240, 0, 0, 0, 0, 0 }, nCount = 50 }, GetName(), 1)
	--local szItemName = GetItemName(AddGoldItem(194))
	--AddCountNews(format("Ng­êi ch¬i <color=red>%s<color> hoµn thµnh nhiÖm vô d· tÈu nªn nhËn ®­îc <color=yellow>%s<color>", GetName(),szItemName), 1)
	--AddItemEx(6,4812,0,0,0,0,0,0)
	--Msg2Player(GetNumberPlayerByIp(GetIP2()))
	--local nNewItem = AddStackItem(10, 4, 417, 0, 0, 0, 0, 0)
	--AddStackItem(100,4,417,0,0,5,0,0)
	--AddStackItem(10, 4, 353, 0, 0, 5, 0, 0)
	--AddGoldItem(0)
	--LeaveTong()
	--ForbidParty(0)
	--CreateTong("SABC111",3,"")
	---DelMagic(710)
	--AddMagic(710,1)
	--AddViprank(10)
	--SetRankEx(241,255)
	--AddMagic(630,20)
	--RemoveSkillState(1447);
	--AddSkillState(726,20,0,1800);
	--local nNewItem = AddItemID(AddItem(0,4,417,0,0,5,0,0)) --tien dong 10 cai
	--LockItem(SetStackItem(nNewItem,500),-2);
	-- for i = 1, 5 do
	-- 	local FronOP1                    = {};
	-- 	local FronOP2                    = {};
	-- 	nNewItem                         = AddItem(0, 0, 1, 2, 10, 0, 100, 10)
	-- 	--nItemGenre, nDetail, nParticur, nLevel, nSeries = FindItemEx(nItem);
	-- 	FronOP1[1], FronOP2[1], value1_3, --1
	-- 	FronOP1[2], FronOP2[2], value2_3,
	-- 	FronOP1[3], FronOP2[3], value3_3, --3
	-- 	FronOP1[4], FronOP2[4], value4_3,
	-- 	FronOP1[5], FronOP2[5], value5_3, --5
	-- 	FronOP1[6], FronOP2[6], value6_3,
	-- 	FronOP1[7], FronOP2[7], value7_3,
	-- 	FronOP1[8], FronOP2[8], value8_3 = GetMagicAttrib(nNewItem)

	-- 	if (FronOP1[1] == 139 or FronOP1[3] == 139 or FronOP1[5] == 139) then
	-- 		AddItemID(nNewItem)
	-- 	else
	-- 		DelItem(nNewItem)
	-- 	end
	-- end
	-- Msg2Player("Completed")
	--SetTask(TASK_TANTHU, SetNumber(1, nTaskValue, 3, 0));
	--if (FindItem(0, 14, 9) <= 0 and GetLevel() >= 60) then
	--	LockItem(AddItemID(AddItem(0, 0, 14, 9, 10, 4, 0, 0)), -3);
	--end;
	--LockItem(AddItemID(AddItem(0, 2, 0, 0, 0, 5, 0, 0)), -3);
	--for i = 176, 184 do
	--	LockItem(AddItemID(AddItem(2, 0, i, 0, 0, 0, 5, 0)), 1, 0, 1);
	--end
	--AddItemID(AddItem(0, 6, 4812, 0, 0, 0, 0, 0))
	--AddItemEx(4, 489, 0, 0, 0, 0, 0)
	--AddItemEx(6, 399, 0, 0, 0, 0, 0)
	--AddItemID(AddItem(0, 0, 12, 9, 0, 0, 0, 0))
	--AddItemID(AddItem(2, 0, 3486, 0, 0, 0, 10, 0))
	--AddTranslife(5)
	--SetTranslife(11)
	--AddItemID(AddItem(0, 0, 0, 8, 10, 0, 0, 0))
	--AddItemID(AddItem(0, 0, 1, 3, 10, 4, 0, 0))
	--AddMagicPoint(100)
	--0 normal
	--1 huyen tinh
	--2 HK
	--3 BK
	--SetLevel(200)
	--AddViprank(0)
	--	local w,x,y = GetWorldPos()
	--	local nCamp = GetCurCamp();
	--	local nName = GetName();
	--  local nRand = RANDOMC(TIEUXA_RATE[1],RANDOM(100)<10 and RANDOM(2,3) or RANDOM(1,2));
	--	local nId = AddNpcNew(TIEUXA_TEMPLET[nRand][1],1,w,x*32,y*32,"\\script\\feature\\van_tieu\\tieuxa.lua",nCamp,nil,nil,random(0,4),0,500000)
	--	SetNpcOwner(nId,GetName(),1)
	--	SetNpcBoss(nId,2);
	--	SetNpcName(nId,nName..TIEUXA_TEMPLET[nRand][2]);
	--	SetNpcTimeout(nId,32400)
end

function thanhly()
	local nT, nG = 0, 0
	for i = 0, 5 do
		for j = 0, 9 do
			nT, nG = FindItemEx(3, i, j)
			if (nT > 0 and nG == 0) then
				RemoveItem(nT, 1)
			end
		end
	end
end

function no()
end

function tientest()
	if (GetCash() < 1000000000) then
		Earn(10000000)
		SetExtPoint(5000)
	end
end

function bangtest()
	local nTaskValue = GetTask(TASK_TANTHU)
	if (GetNumber(1, nTaskValue, 5) > 0) then
		Talk(1, "", 15278)
		return
	end
	if (GetCamp() == 4 and GetTongFlag() == 0) then
		if (GetCash() < 20000000) then
			Earn(20000000 - GetCash())
		end
		AddRepute(300 - GetRepute())
		AddFuYuan(500 - GetFuYuan())
		SetLeadLevel(30)
		AddItemID(AddItem(0, 4, 195, 0, 0, 5, 0, 0))
		NewWorld(2, 2350, 3499)
		SetFightState(1)
		SetTask(TASK_TANTHU, SetNumber(1, nTaskValue, 5, 1))
	else
		Talk(1, "", "B¹n ch­a xuÊt s­ hoÆc ®· gia nhËp bang héi")
	end
end

function kynangtest()
	local nTaskValue = GetTask(TASK_KYNANG)
	-- if (nTaskValue > 0) then
	-- 	Talk(1, "", "B¹n ®· nhËn hç trî nµy råi!")
	-- 	return
	-- end
	local nCurFac = GetFactionNo()
	if (nCurFac < 0 or nCurFac > 12) then
		Msg2Player("B¹n ch­a gia nhËp m«n ph¸i!")
		return
	end
	nCurFac = nCurFac + 1
	for i = 1, getn(SKILL90_ARRAY[nCurFac]) do
		if (SKILL90_ARRAY[nCurFac][i][3] > 0) then
			DelMagic(SKILL90_ARRAY[nCurFac][i][2])
			AddMagic(SKILL90_ARRAY[nCurFac][i][2], 20)
		else
			AddMagic(SKILL90_ARRAY[nCurFac][i][2], 0)
		end
	end
	for i = 1, getn(SKILL150_ARRAY[nCurFac]) do
		if (SKILL150_ARRAY[nCurFac][i][3] > 0) then
			AddMagic(SKILL150_ARRAY[nCurFac][i][2], 20)
		else
			AddMagic(SKILL150_ARRAY[nCurFac][i][2], 0)
		end
	end
	for i = 1, getn(SKILL180_ARRAY[nCurFac]) do
		if (SKILL180_ARRAY[nCurFac][i][3] > 0) then
			AddMagic(SKILL180_ARRAY[nCurFac][i][2], 20)
		else
			AddMagic(SKILL180_ARRAY[nCurFac][i][2], 0)
		end
	end

	AddMagic(SKILL120AR[nCurFac], 20)
	AddMagic(210, 1)
	SetTask(TASK_KYNANG, 1)
	Talk(1, "", "Vâ c«ng ®· ®­îc truyÒn thô, h·y thö vËn c«ng xem sao!")
end

function test()
	local nTaskValue = GetTask(TASK_TANTHU)
	if (GetNumber(1, nTaskValue, 4) > 0) then
		Talk(1, "", 15278)
		return
	end
	if (CheckRoom(6, 6) == 0) then
		Talk(1, "", 12266)
		return
	end
	SetTask(TASK_TANTHU, SetNumber(1, nTaskValue, 4, 1))
	AddItemID(AddItem(0, 6, 2405, 0, 0, 5, 0, 0)) --mabai
	AddItemID(AddItem(0, 5, 1, 0, 0, 0, 0, 0)) --thp
	AddItemID(AddItem(0, 5, 2, 0, 0, 0, 0, 0)) --tdpvh
	AddItemID(AddItem(0, 4, 240, 0, 0, 0, 0, 0)) --LTT
	LockItem(AddItemID(AddItem(0, 0, 14, 0, 10, 0, 0, 0)), -2)
	for i = 158, 166 do
		AddItemID(AddItem(2, 0, i, 0, 10, 0, 10, 0))
	end
	--	SetExtPoint(50000);
	Earn(10000000)
	SetLevel(150)
end

function duatop()
	if (GetLevel() < 120) then
		Talk(1, "", format(NOT_LEVEL, 120))
		return
	end
	local nTaskValue = GetTask(TASK_TOP120)
	if (nTaskValue > 0) then
		Talk(1, "", "Ng­¬i ®· nhËn th­ëng råi")
		return
	end
	local nDone = GetDataInt(DATA_DUATOP)
	-- if (nDone >= 5) then
	-- 	Talk(1, "", 10288)
	-- 	return
	-- end
	if (CheckRoom(6, 6) == 0) then
		Talk(1, "", 12266)
		return
	end
	nDone = nDone + 1
	SetData(DATA_DUATOP, nDone)
	SetTask(TASK_TOP120, 1)
	Talk(1, "", "NhËn ®­îc phÇn th­ëng cÊp 120!")
	Msg2SubWorld(format("<color=green>%s<color>nhËn th­ëng top 120 thø <color=yellow>%d", GetName(), nDone))
	nhanvatpham(nDone)
	SaveDataFile()
end

function nhanvatpham(nNo)
	if (nNo > 0 and nNo <= 5) then
		--local nNewItem = AddItemID(AddItem(0, 4, 240, 0, 0, 5, 0, 0)) --LTT
		--LockItem(SetStackItem(nNewItem, 50), -2);
		AddItemID(AddItem(0, 0, 10, 6, 5, 0, 0, 0))
	else
		LockItem(AddItemEx(0, 10, 8, 5, 0, 0, 0), -2)
	end
end

NEED_6X5 = "Hµnh trang ph¶i cã 6x7 « trèng (7 hµng)"

function hotronew()
	local nTaskVal = GetTask(TASK_TANTHU)
	if (nTaskVal > 0) then
		Talk(1, "", 15278)
		return
	end
	--Msg2Player("Check!")
	if (CheckRoom(6, 7) == 0) then
		Talk(1, "", NEED_6X7)
		return
	end
	SetTask(TASK_TANTHU, 1)
	--Msg2Player("Check!")
	Earn(1000)
	LockItem(AddItemEx(5, 1, 0, 0, 0, 0, 0), -2) --thp
	LockItem(AddItemEx(5, 2, 0, 0, 0, 0, 0), -2) --tdpvh
	LockItem(AddItemEx(0, 14, 0, 2, 0, 0, 0), -2)
	--Msg2Player("Check!")
	--for i = 176, 184 do
	--	LockItem(AddGoldItem(i), -2);
	--end

	LockItem(AddItemEx(0, 10, RANDOM(1, 3), 7, 0, 0, 0), -2)
	--for i = 1, 5 do
	LockItem(AddStackItem(5, 6, 1182, 0, 0, 5, 0, 0), -2)
	--end;

	--ST_LevelUp(190 - GetLevel())
end

function hotrokn()
	local nTaskValue = GetTask(TASK_KYNANG)
	-- if (nTaskValue > 0) then
	-- 	Talk(1, "", "B¹n ®· nhËn hç trî nµy råi!")
	-- 	return
	-- end
	local nCurFac = GetFactionNo()
	if (nCurFac < 0 or nCurFac > 12) then
		Msg2Player("B¹n ch­a gia nhËp m«n ph¸i!")
		return
	end
	nCurFac = nCurFac + 1
	for i = 1, getn(SKILL90_ARRAY[nCurFac]) do
		if (SKILL90_ARRAY[nCurFac][i][3] > 0) then
			DelMagic(SKILL90_ARRAY[nCurFac][i][2])
			AddMagic(SKILL90_ARRAY[nCurFac][i][2], 1)
		else
			AddMagic(SKILL90_ARRAY[nCurFac][i][2], 0)
		end
	end
	-- for i = 1, getn(SKILL150_ARRAY[nCurFac]) do
	-- 	if (SKILL150_ARRAY[nCurFac][i][3] > 0) then
	-- 		AddMagic(SKILL150_ARRAY[nCurFac][i][2], 1);
	-- 	else
	-- 		AddMagic(SKILL150_ARRAY[nCurFac][i][2], 0);
	-- 	end
	-- end
	-- for i = 1, getn(SKILL180_ARRAY[nCurFac]) do
	-- 	if (SKILL180_ARRAY[nCurFac][i][3] > 0) then
	-- 		AddMagic(SKILL180_ARRAY[nCurFac][i][2], 1);
	-- 	else
	-- 		AddMagic(SKILL180_ARRAY[nCurFac][i][2], 0);
	-- 	end
	-- end

	--AddMagic(SKILL120AR[nCurFac], 1);
	AddMagic(210, 1)
	SetTask(TASK_KYNANG, 1)
	Talk(1, "", "Vâ c«ng ®· ®­îc truyÒn thô, h·y thö vËn c«ng xem sao!")
end

function add_gm()
	AddItemEx(6, 4812, 0, 0, 0, 0, 0)
end

function newyear()
	if (GetTask(TASK_NEWYEAR) == 1) then
		Talk(1, "", "Ng­¬i ®· nhËn th­ëng råi!")
		return
	end

	SetTask(TASK_NEWYEAR, 1)
	LockItem(AddItemEx(0, 10, 6, 10, 0, 0, 0), -2)
	LockItem(AddStackItem(50, 4, 240, 0, 0, 0, 0, 0), -2)
	Msg2Player(format("NhËn ®­îc <color=yellow>%d<color> Lôc Thñy Tinh", 50))
	Msg2Player(format("NhËn ®­îc <color=yellow>%d<color> B«n Tiªu", 1))
end

function hotro120()
	if (GetTask(TASK_TOP120) == 1) then
		Talk(1, "", "Ng­¬i ®· nhËn hç trî råi!")
		return
	end

	SetTask(TASK_TOP120, 1)
	LockItem(AddStackItem(50, 4, 240, 0, 0, 5, 0, 0), -2)
	LockItem(AddItemEx(0, 10, 6, 10, 0, 0, 0), -2)
end

function hotroloi()
	if (GetTask(TASK_HOTRO_LOI) == 1) then
		Talk(1, "", "Ng­¬i ®· nhËn hç trî råi!")
		return
	end

	SetTask(TASK_HOTRO_LOI, 1)
	LockItem(AddStackItem(50, 6, 4814, 0, 0, 5, 0, 0), -2)
	LockItem(AddStackItem(50, 6, 4814, 0, 0, 5, 0, 0), -2)
	LockItem(AddStackItem(50, 6, 4814, 0, 0, 5, 0, 0), -2)
	LockItem(AddStackItem(50, 6, 4814, 0, 0, 5, 0, 0), -2)
end

function hotro1()
	LockItem(AddItemEx(5, 1, 0, 0, 0, 0, 0), -2) --thp
	LockItem(AddItemEx(5, 2, 0, 0, 0, 0, 0), -2) --tdpvh
end
