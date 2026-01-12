Include("\\script\\header\\event_endtime.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");
-----------------EVENT CONFIG-----------------
EVENT_ACTIVE1   = 0
EVENT_ACTIVE2   = 0
ACTIVE_MOCEVENT = 1
END_TALK        = "Tho¸t/no";
NHANROI         = "Ng­¬i ®· nhËn phÇn th­ëng nµy råi.";
MAX_RESTTIME    = 1
NOT_LEVEL       = "B¹n cÇn ph¶i <color=yellow>®¹t %d cÊp<color> ®Ó nhËn th­ëng!"
NHANROI         = "B¹n ®· nhËn phÇn th­ëng nµy råi."
NHAN_OK         = "B¹n ®· nhËn th­ëng <color=yellow>cÊp %d<color>."
CHUA_RUONG      = "B¹n ph¶i chõa <color=yellow>%d hµng « trèng<color> trong hµnh trang ®Ó nhËn th­ëng!"
NOT_OK          = "B¹n ph¶i vµo ph¸i míi cã thÓ nhËn th­ëng."
----------------------------------------------

function main(NpcIndex)
	if (EVENT_ACTIVE1 == 1) then
		Say2("Ta lµ thÇn tµi ®©y", 3, 1, "",
			"Ho¹t ®éng Hoa Hång May M¾n/HoatDong1",
			"Nh©n th­ëng mèc Event Hoa Hång/mocevent",
			-- "Ho¹t ®éng TrÈy Nªu §Çu N¨m/HoatDong2",
			END_TALK)
	else
		Say2("Ta lµ thÇn tµi ®©y", 2, 1, "",
			-- "Nh©n th­ëng mèc Event Hoa Hång/mocevent",
			END_TALK)
	end
end

function mocevent()
	if (GetNumber(GetTask(TASK_MOC700), 2) == 0) then
		Say2("Ta lµ thÇn tµi ®©y", 3, 1, "",
			"Mèc event 700/moc7001",
			END_TALK)
	elseif (GetNumber(GetTask(TASK_MOC1000), 2) == 0) then
		Say2("Ta lµ thÇn tµi ®©y", 3, 1, "",
			"Mèc event 999/moc9991",
			END_TALK)
	else
		Say2("Ta lµ thÇn tµi ®©y", 3, 1, "",
			END_TALK)
	end
end

function moc7001()
	nTaskValue = GetTask(TASK_HOAHONG);
	if (GetNumber3(nTaskValue, 3)) < 700 then
		Talk(1, "", "Ng­¬i ph¶i sö dông 700 Vßng Hoa Trë lªn míi cã thÓ nhËn phÇn th­ëng nµy.")
		return
	end
	------------------------------------------------------------
	moc700()
end

function moc700()
	local nTaskValue = GetTask(TASK_MOC700);
	if (GetNumber(nTaskValue, 2) > 0) then
		Talk(1, "", NHANROI)
		return
	end
	if (CheckRoom(6, 3) == 0) then
		Talk(1, "", format(CHUA_RUONG, 3))
		return
	end
	SetTask(TASK_MOC700, SetNumber(nTaskValue, 2, 1));
	------------------------------------------------------------
	ThuongNgua()
end

function ThuongNgua()
	a = random(1, 4)
	local Ngua = ItemSetAdd(0, 0, 10, NGUA[a][1], 10, 0, 0)
	SetItemDate(Ngua, 86400)
	AddItemID(Ngua)
	Msg2Player("B¹n nhËn ®­îc <color=yellow>" .. NGUA[a][2] .. "")
	logMoc("" .. GetName() .. "\t\t\t" .. GetAccount() .. "\t\t\tMoc 777 nhan duoc trang bi " .. NGUA[a][2] .. "")
end

NGUA = {
	{ 15, "Háa Tinh Kim Hæ V­¬ng" },
	{ 16, "Kim Tinh B¹ch Hæ V­¬ng" },
	{ 17, "Long Tinh H¾c Hæ V­¬ng" },
	{ 22, "S­ tö " },
}

function moc9991()
	nTaskValue = GetTask(TASK_HOAHONG);
	if (GetNumber3(nTaskValue, 3)) < 999 then
		Talk(1, "", "Ng­¬i ph¶i sö dông 999 Vßng Hoa Trë lªn míi cã thÓ nhËn phÇn th­ëng nµy.")
		return
	end
	------------------------------------------------------------
	moc999()
end

function moc999()
	local nTaskValue = GetTask(TASK_MOC1000);
	if (GetNumber(nTaskValue, 2) > 0) then
		Talk(1, "", NHANROI)
		return
	end
	if (CheckRoom(6, 3) == 0) then
		Talk(1, "", format(CHUA_RUONG, 3))
		return
	end
	SetTask(TASK_MOC1000, SetNumber(nTaskValue, 2, 1));
	------------------------------------------------------------
	TrangBiHKKO()
end

function TrangBiHKKO()
	local g = random(5, 6)
	local DoHK = ItemSetAdd(2, 0, random(ITEMGOLD[g][1], ITEMGOLD[g][2]) - 2, 0, 0, 0, 5)
	SetItemRich(DoHK, ITEMGOLD[g][4])
	AddItemID(DoHK)
	logMoc("" .. GetName() .. "\t\t\t" .. GetAccount() .. "\t\t\tMoc 999 nhan duoc trang bi " .. ITEMGOLD[g][3] .. "")
end

ITEMGOLD = {
	{ 906,  1135, "Thanh C©u",   100 },
	{ 1136, 1365, "V©n Léc",     200 },
	{ 1366, 1595, "Th­¬ng Lang", 300 },
	{ 1596, 1825, "HuyÒn Viªn",  500 },
	{ 1826, 2055, "Tö M·ng",     1000 },
	{ 2056, 2285, "Kim ¤ ",      3000 },
	{ 2286, 2515, "B¹ch Hæ ",    5000 },
	{ 2516, 2745, "XÝch L©n",    10000 },
	{ 2746, 2975, "Minh Ph­îng", 20000 },
}

function logMoc(str)
	local gm_Log = "dulieu/SuKien/MocEvent.txt"
	local fs_log = openfile(gm_Log, "a");
	write(fs_log, date("%H:%M:%S_%d-%m-%y") .. "\t" .. str .. "\n");
	closefile(fs_log);
end

function ghepcapneu(sel)
	local nTaskTime = GetTask(TASK_THOIGIAN5);
	local nCurTime = GetTimeMin();
	if (CheckRoom(2, 1) == 0) then
		Talk(1, "", "Vui lßng chõa Ýt nhÊt 2 « trèng.")
		return
	end
	if (nCurTime - nTaskTime < MAX_RESTTIME) then
		Talk(1, "", 15678)
		return
	end
	SetTaskTemp(EVENT_TET, SetNumber(GetTaskTemp(EVENT_TET), 6, sel));
	Input("GhepCayNeu");
end

function HoatDong1()
	local nTask = GetTask(TASK_HOAHONG);
	local TAB_MSG = {
		"<color=red>ThÇn Tµi<color>: ho¹t ®éng <color=blue>Hoa Hång May M¾n <color> tiÕp tôc diÔn ra tõ ngµy <bclr=blue>22-04-2015 ®Õn 22-05-2015<bclr>, tham gia sù kiÖn ®ång ®¹o sÏ nhËn rÊt nhiÒu kinh nghiÖm vµ vËt phÈm gi¸ trÞ.\n\n<color=yellow>Vßng Hoa Vµng <color>\t\t\t\t\t\t\t\t" ..
		GetNumber3(nTask, 1) ..
		"\n<color=cyan>Vßn Hoa Hång <color>\t\t\t\t\t\t\t\t\t" ..
		GetNumber3(nTask, 2) .. "\n<color=red>Vßng Hoa Xanh <color>\t\t\t\t\t\t\t\t\t" .. GetNumber3(nTask, 3) .. "",
		"Vßng Hoa Vµng/ghepevent",
		"Vßn Hoa Hång/ghepevent",
		"Vßng Hoa Xanh/ghepevent",
	}
	Say2(TAB_MSG[1], 4, 1, "",
		TAB_MSG[2],
		TAB_MSG[3],
		TAB_MSG[4],
		END_TALK)
end

function ghepevent(sel)
	local n = sel + 1
	local nTaskTime = GetTask(TASK_THOIGIAN5);
	local nCurTime = GetTimeMin();
	if (CheckRoom(2, 1) == 0) then
		Talk(1, "", 15680)
		return
	end
	if (nCurTime - nTaskTime < MAX_RESTTIME) then
		Talk(1, "", 15678)
		return
	end
	if (n == 1) then
		Input("GhepEvent1");
	elseif (n == 2) then
		Input("GhepEvent2");
	elseif (n == 3) then
		Input("GhepEvent3");
	end
end

function GhepEvent1(num)
	EVENT = {
		{ 91, "Hoa cóc vµng " },
		{ 92, "Hoa cóc tÝm " },
		{ 96, "Vßng Hoa Vµng " } }
	KiemTraNL1 = GetItemCount(EVENT[1][1], 5); KiemTraNL2 = GetItemCount(EVENT[2][1], 5)
	NL1YC = num * 5; NL2YC = num * 3
	if (num < 1 or num > 50) then
		Talk(1, "", "Mçi lÇn ®æi chØ ®­îc <color=yellow>50 vËt phÈm<color> mµ th«i.")
		return
	end
	if (KiemTraNL1 < NL1YC) then
		Talk(1, "",
			"Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>" ..
			KiemTraNL1 .. "/" .. NL1YC .. "<color> <color=pink>" .. EVENT[1][2] .. "<color>, vui lßng kiÓm tra l¹i.")
		return
	end
	if (KiemTraNL2 < NL2YC) then
		Talk(1, "",
			"Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>" ..
			KiemTraNL2 .. "/" .. NL2YC .. "<color> <color=pink>" .. EVENT[2][2] .. "<color>, vui lßng kiÓm tra l¹i.")
		return
	end
	if (CheckRoom(1, 1) == 0) then
		Talk(1, "", 15679)
		return
	end
	local Item = ItemSetAdd(0, 5, EVENT[3][1], 0, 0, 5, num, 0);
	SetItemDate(Item, ENAM, ETHANG, ENGAY, 0, 0);
	AddItemID(Item);
	DelItem(EVENT[1][1], 5, NL1YC)
	DelItem(EVENT[2][1], 5, NL2YC)
	local nCurTime = GetTimeMin();
	SetTask(TASK_THOIGIAN5, nCurTime);
	Msg2Player("B¹n ®æi thµnh c«ng " .. num .. " <color=yellow>" .. EVENT[3][2] .. "<color>.")
end

function GhepEvent2(num)
	EVENT = {
		{ 91, "Hoa cóc vµng " },
		{ 93, "Hoa cóc tr¾ng " },
		{ 97, "Vßng Hoa Hång " } }
	KiemTraNL1 = GetItemCount(EVENT[1][1], 5); KiemTraNL2 = GetItemCount(EVENT[2][1], 5)
	NL1YC = num * 3; NL2YC = num * 2
	TienYC = num * 50000
	if (num < 1 or num > 50) then
		Talk(1, "", "Mçi lÇn ®æi chØ ®­îc <color=yellow>50 vËt phÈm<color> mµ th«i.")
		return
	end
	if (KiemTraNL1 < NL1YC) then
		Talk(1, "",
			"Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>" ..
			KiemTraNL1 .. "/" .. NL1YC .. "<color> <color=pink>" .. EVENT[1][2] .. "<color>, vui lßng kiÓm tra l¹i.")
		return
	end
	if (KiemTraNL2 < NL2YC) then
		Talk(1, "",
			"Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>" ..
			KiemTraNL2 .. "/" .. NL2YC .. "<color> <color=pink>" .. EVENT[2][2] .. "<color>, vui lßng kiÓm tra l¹i.")
		return
	end
	if (GetCash() < TienYC) then
		Talk(1, "", "B¹n kh«ng ®ñ <color=red>" .. TienYC .. " l­îng<color>, vui lßng kiÓm tra l¹i.")
		return
	end
	if (CheckRoom(1, 1) == 0) then
		Talk(1, "", 15679)
		return
	end
	local Item = ItemSetAdd(0, 5, EVENT[3][1], 0, 0, 5, num, 0);
	SetItemDate(Item, ENAM, ETHANG, ENGAY, 0, 0);
	AddItemID(Item);
	Pay(TienYC)
	DelItem(EVENT[1][1], 5, NL1YC)
	DelItem(EVENT[2][1], 5, NL2YC)
	local nCurTime = GetTimeMin();
	SetTask(TASK_THOIGIAN5, nCurTime);
	Msg2Player("B¹n ®æi thµnh c«ng " .. num .. " <color=yellow>" .. EVENT[3][2] .. "<color>.")
end

function GhepEvent3(num)
	EVENT = {
		{ 91, "Hoa cóc vµng " },
		{ 94, "Hoa hång " },
		{ 98, "Vßng Hoa Xanh " } }
	KiemTraNL1 = GetItemCount(EVENT[1][1], 5); KiemTraNL2 = GetItemCount(EVENT[2][1], 5)
	NL1YC = num * 5
	if (num < 1 or num > 50) then
		Talk(1, "", "Mçi lÇn ®æi chØ ®­îc <color=yellow>50 vËt phÈm<color> mµ th«i.")
		return
	end
	if (KiemTraNL1 < NL1YC) then
		Talk(1, "",
			"Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>" ..
			KiemTraNL1 .. "/" .. NL1YC .. "<color> <color=pink>" .. EVENT[1][2] .. "<color>, vui lßng kiÓm tra l¹i.")
		return
	end
	if (KiemTraNL2 < num) then
		Talk(1, "",
			"Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>" ..
			KiemTraNL2 .. "/" .. num .. "<color> <color=pink>" .. EVENT[2][2] .. "<color>, vui lßng kiÓm tra l¹i.")
		return
	end
	if (CheckRoom(1, 1) == 0) then
		Talk(1, "", 15679)
		return
	end
	local Item = ItemSetAdd(0, 5, EVENT[3][1], 0, 0, 5, num, 0);
	SetItemDate(Item, ENAM, ETHANG, ENGAY, 0, 0);
	AddItemID(Item);
	DelItem(EVENT[1][1], 5, NL1YC)
	DelItem(EVENT[2][1], 5, num)
	local nCurTime = GetTimeMin();
	SetTask(TASK_THOIGIAN5, nCurTime);
	Msg2Player("B¹n ®æi thµnh c«ng " .. num .. " <color=yellow>" .. EVENT[3][2] .. "<color>.")
end

function noinput()
end

function no()
end
