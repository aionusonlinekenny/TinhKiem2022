Include("\\script\\header\\event_endtime.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");
-----------------EVENT CONFIG-----------------
EVENT_ACTIVE1   = 1
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
		Say2("Ng­¬i cã muèn tham gia Event Ch©n Long §¬n?", 3, 1, "",
			"Ho¹t §éng Ch©n Long §¬n/batdau",
			"TÝch Lòy NhËn Li L©m Th¶o/onlttt",
			END_TALK)
	else
		Say2("Sù kiÖn ®· kÕt thóc!", 2, 1, "",
			END_TALK)
	end
end

function onlttt()
	Say2(
		"Mçi 2 giê online sÏ nhËn ®­îc 6 Li L©m Th¶o, ng­¬i cã muèn nhËn kh«ng?\n\n- Thêi gian online cña ng­¬i lµ: <color=blue>" ..
		GioOnline() .. "<color> giê <color=blue>" .. PhutOnline() .. "<color> phót", 3, 1, "",
		"T¹i h¹ ®ång ý/okeonlttt",
		END_TALK)
end

function okeonlttt()
	if GetOnlineMin() < 120 then
		Talk(1, "", "Ng­¬i ch­a ®ñ <color=yellow>2 giê online<color> kh«ng thÓ nhËn th­ëng.")
		return
	end
	SetOnlineMin(GetOnlineMin() - 60)
	AddEventItem(107, 6)
	Msg2Player("<color=yellow>B¹n ®· nhËn ®­îc 6 <color=red>Li L©m Th¶o.");
end

function GioOnline()
	if GetOnlineMin() < 60 then
		return 0
	else
		gio = floor((GetOnlineMin() / 60), 2)
		return gio
	end
end

function PhutOnline()
	if GetOnlineMin() < 60 then
		return GetOnlineMin()
	else
		gio = floor((GetOnlineMin() / 60), 2)
		phut = GetOnlineMin() - gio * 60
		return phut
	end
end

function batdau()
	local nTask = GetTask(TASK_CHANLONGDON);
	local TAB_MSG = {
		"<color=red>§¹i S­ L·o L·o<color>: ho¹t ®éng <color=blue>Ch©n Long §¬n<color> diÔn ra tõ ngµy <color=blue>21-10-2018 ®Õn 25-11-2018<color>, tham gia sù kiÖn ®ång ®¹o sÏ nhËn rÊt nhiÒu kinh nghiÖm vµ vËt phÈm gi¸ trÞ.\n\n<color=blue>Hæ Khª §¬n<color>\t\t\t\t\t\t\t\t" ..
		GetNumber3(nTask, 1) ..
		"\n<color=yellow>S­ B× §¬n<color>\t\t\t\t\t\t\t\t" ..
		GetNumber3(nTask, 2) .. "\n<color=cyan>Ch©n Long §¬n<color>\t\t\t\t\t\t\t\t\t" .. GetNumber3(nTask, 3) .. "",
		"§æi Hæ Khª §¬n/ghepevent",
		"§æi S­ B× §¬n/ghepevent",
		"§æi Ch©n Long §¬n/ghepevent",
	}
	Say2(TAB_MSG[1], 5, 1, "",
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
	else
		Input("GhepEvent3");
	end
end

function GhepEvent1(num)
	EVENT = {
		{ 104, 5, "§­êng Quy" },
		{ 105, 2, "X¹ H­¬ng" },
		{ 107, 2, "Li L©m Th¶o" },
		{ 109, 1, "Hæ Khª §¬n" }, }
	local tong = getn(EVENT) - 1;
	if (num < 1 or num > 50) then
		Talk(1, "", 15685)
		return
	end
	for i = 1, tong do
		local sl = EVENT[i][2] * num;
		if (GetItemCount(EVENT[i][1], 5) < sl) then
			Talk(1, "",
				"Kh«ng ®ñ <color=red>" ..
				GetItemCount(EVENT[i][1], 5) ..
				"/" .. sl .. " <color> <color=pink>" .. EVENT[i][3] .. " <color>, vui lßng kiÓm tra l¹i.")
			return
		end
	end
	local Item = ItemSetAdd(0, 5, EVENT[getn(EVENT)][1], 0, 0, 5, num, 0);
	SetItemDate(Item, ITEM_NAM, ITEM_THANG, ITEM_NGAY, 0, 0);
	AddItemID(Item);
	for i = 1, tong do
		DelItem(EVENT[i][1], 5, EVENT[i][2] * num)
	end
	SetTaskTemp(TEMP_TIMEEVENT, GetTimeLoad() + MAX_RESTTIME);
	Msg2Player("B¹n ®æi thµnh c«ng " .. num .. " <color=yellow>" .. EVENT[getn(EVENT)][3] .. "<color>.")
end

function GhepEvent2(num)
	EVENT = {
		{ 104, 5, "§­êng Quy" },
		{ 105, 3, "X¹ H­¬ng" },
		{ 107, 3, "Li L©m Th¶o" },
		{ 106, 1, "TrÇn B×" },
		{ 110, 1, "S­ B× §¬n" }, }
	local tong = getn(EVENT) - 1;
	if (num < 1 or num > 50) then
		Talk(1, "", 15685)
		return
	end
	for i = 1, tong do
		local sl = EVENT[i][2] * num;
		if (GetItemCount(EVENT[i][1], 5) < sl) then
			Talk(1, "",
				"Kh«ng ®ñ <color=red>" ..
				GetItemCount(EVENT[i][1], 5) ..
				"/" .. sl .. " <color> <color=pink>" .. EVENT[i][3] .. " <color>, vui lßng kiÓm tra l¹i.")
			return
		end
	end
	local Item = ItemSetAdd(0, 5, EVENT[getn(EVENT)][1], 0, 0, 5, num, 0);
	SetItemDate(Item, ITEM_NAM, ITEM_THANG, ITEM_NGAY, 0, 0);
	AddItemID(Item);
	for i = 1, tong do
		DelItem(EVENT[i][1], 5, EVENT[i][2] * num)
	end
	SetTaskTemp(TEMP_TIMEEVENT, GetTimeLoad() + MAX_RESTTIME);
	Msg2Player("B¹n ®æi thµnh c«ng " .. num .. " <color=yellow>" .. EVENT[getn(EVENT)][3] .. " <color>.")
end

function GhepEvent3(num)
	EVENT = {
		{ 104, 5, "§­êng Quy" },
		{ 105, 3, "X¹ H­¬ng" },
		{ 107, 3, "Li L©m Th¶o" },
		{ 108, 1, "D­îc v­¬ng ®Ønh" },
		{ 111, 1, "Ch©n Long §¬n" }, }
	local tong = getn(EVENT) - 1;
	if (num < 1 or num > 50) then
		Talk(1, "", 15685)
		return
	end
	for i = 1, tong do
		local sl = EVENT[i][2] * num;
		if (GetItemCount(EVENT[i][1], 5) < sl) then
			Talk(1, "",
				"Kh«ng ®ñ <color=red>" ..
				GetItemCount(EVENT[i][1], 5) ..
				"/" .. sl .. " <color> <color=pink>" .. EVENT[i][3] .. " <color>, vui lßng kiÓm tra l¹i.")
			return
		end
	end
	local Item = ItemSetAdd(0, 5, EVENT[getn(EVENT)][1], 0, 0, 5, num, 0);
	SetItemDate(Item, ITEM_NAM, ITEM_THANG, ITEM_NGAY, 0, 0);
	AddItemID(Item);
	for i = 1, tong do
		DelItem(EVENT[i][1], 5, EVENT[i][2] * num)
	end
	SetTaskTemp(TEMP_TIMEEVENT, GetTimeLoad() + MAX_RESTTIME);
	Msg2Player("B¹n ®æi thµnh c«ng " .. num .. " <color=yellow>" .. EVENT[getn(EVENT)][3] .. " <color>.")
end

function noinput()
end

function no()
end
