Include("\\script\\header\\event_endtime.lua");
Include("\\script\\header\\taskid.lua");

-----------------EVENT CONFIG-----------------
EVENT_NPCACTIVE = 1		-- on/off event
END_TALK = "Tho¸t/no";
MAX_RESTTIME = 10		-- thoi gian gian cach
ITEM_NAM = 2016
ITEM_THANG = 9
ITEM_NGAY = 15
EVENT_NAME = "<color=black><bclr=red>R­íc §Ìn Tr«ng Tr¨ng<bclr><color>"
EVENT_TIME = "<color=black><bclr=yellow>16-08-2016 - 10-09-2016<bclr><color>"
----------------------------------------------

function GetTimeLoad()
	Yr,Mth,Dy,Hr,Mn,Se = GetTime();
	TimeNow = Mth * 31 * 24 * 60 * 60 + Dy * 24 * 60 * 60 + Hr * 60 * 60 + Mn * 60 + Se;
	return TimeNow;
end

function main(NpcIndex)
	local nTask = GetTask(TASK_RUOCDEN);
	if (EVENT_NPCACTIVE == 1) then
	Say2("Tõ "..EVENT_TIME..", ta lµ ng­êi qu¶n lý ho¹t ®éng "..EVENT_NAME..", cÇn g× cø ®Õn gÆp ta.\n\n<color=blue>Lång §Ìn Gµ Con<color>\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,1).."/400\n<color=yellow>Lång §Ìn Ngùa Con<color>\t\t\t\t\t\t\t\t"..GetNumber3(nTask,3).."/500\n<color=red>Lång §Ìn Thá Con<color>\t\t\t\t\t\t\t\t"..GetNumber3(nTask,2).."/600",5,1,"",
	"Lµm Lång §Ìn Gµ Con/ghepevent",
	"Lµm Lång §Ìn Ngùa Con/ghepevent",
	"Lµm Lång §Ìn Thá Con/ghepevent",
	"NhËn Thanh Tre/OnlineGift",
		END_TALK)
	elseif (EVENT_NPCACTIVE == 2) then
	Say2("HiÖn t¹i ho¹t ®éng "..EVENT_NAME.." ®· kÕt thóc.",1,1,"",
		END_TALK)
	else
	Say2("Ho¹t ®éng "..EVENT_NAME.." sÏ b¾t ®Çu tõ ngµy "..EVENT_TIME..".",1,1,"",
		END_TALK)
	end
end

function OnlineGift()
	if (GetLevel() < 100) then Talk(1,"","Nh©n vËt d­íi cÊp 100 kh«ng thÓ tham gia.") return end
	Say2("Mçi 3 giê online sÏ nhËn ®­îc 15 Thanh Tre, <color=red>tèi ®a nhËn th­ëng trong ngµy lµ 3 lÇn<color>, ng­¬i cã muèn nhËn kh«ng?\n\n- Thêi gian online cña ng­¬i hiÖn lµ: <color=blue>"..GioOnline().."<color> giê <color=blue>"..PhutOnline().."<color> phót\n- H«m nay ng­¬i cßn nhËn ®­îc: <color=yellow>"..(3-GetNumber(GetTask(TASK_RESET2),7)).." lÇn",2,1,"",
	"T¹i h¹ ®ång ý/OkOnlineGift",
		END_TALK)
end

function OkOnlineGift()
	local nTask = GetTask(TASK_RESET2);
	local SLNhan = GetNumber(nTask,7);
	if (GetOnlineMin() < 180) then Talk(1,"","Ng­¬i ch­a ®ñ <color=yellow>3 giê online<color> kh«ng thÓ nhËn th­ëng.") return end
	if (SLNhan >= 3) then Talk(1,"","Ta ®· ph¸t hÕt sè l­îng Thanh Tre cña ngµy h«m nay, mai h·y quay l¹i nhÐ.") return end
	SetTask(TASK_RESET2,SetNumber(nTask,7,SLNhan+1));
	SetOnlineMin(GetOnlineMin()-180)
	local Item = ItemSetAdd(0,5,202,0,0,5,15,0);
	SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
	AddItemID(Item);
	Msg2Player("NhËn <color=yellow>15 Thanh Tre<color> thµnh c«ng.");
end

function GioOnline()
if GetOnlineMin() < 60 then
return 0
else
gio = floor((GetOnlineMin()/60),2)
return gio
end
end

function PhutOnline()
if GetOnlineMin() < 60 then
return GetOnlineMin()
else
gio = floor((GetOnlineMin()/60),2)
phut = GetOnlineMin() - gio*60
return phut
end
end

function ghepevent(sel)
	local n = sel+1
	local TimeGhep = GetTaskTemp(TEMP_TIMEEVENT)
	if (TimeGhep > GetTimeLoad()) then Talk(1,"","Vui lßng chê thªm <color=yellow>"..(TimeGhep-GetTimeLoad()).." gi©y<color> n÷a h·y ®æi tiÕp.") return end
	if (CheckRoom(1,1) == 0) then Talk(1,"",15679) return end
	if (n == 1) then
	Input("GhepEvent1");
	elseif (n == 2) then
	Input("GhepEvent2");
	elseif (n == 3) then
	Input("GhepEvent3");
	end
end

function GhepEvent1(num)
	EVENT = {	-- 2 nguyen lieu
	{240,5,"GiÊy Mµu Vµng"},
	{202,5,"Thanh Tre"},
	{203,1,"Lång §Ìn Gµ Con"}}
	local tong = getn(EVENT)-1;
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh«ng ®ñ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.."<color> <color=pink>"..EVENT[i][3].."<color>, vui lßng kiÓm tra l¹i.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("B¹n chÕ t¹o thµnh c«ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].."<color>.")
end

function GhepEvent2(num)
	EVENT = {	-- 2 nguyen lieu
	{240,3,"GiÊy Mµu Vµng"},
	{202,5,"Thanh Tre"},
	{241,3,"Hßa ThÞ BÝch"},
	{205,1,"Lång §Ìn Ngùa Con"}}
	local tong = getn(EVENT)-1;
	local TienYC = num*50000
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	if (GetCash() < TienYC) then Talk(1,"","B¹n kh«ng ®ñ <color=red>"..TienYC.." l­îng<color>, vui lßng kiÓm tra l¹i.") return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh«ng ®ñ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.."<color> <color=pink>"..EVENT[i][3].."<color>, vui lßng kiÓm tra l¹i.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		Pay(TienYC)
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("B¹n chÕ t¹o thµnh c«ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].."<color>.")
end

function GhepEvent3(num)
	EVENT = {	-- 3 nguyen lieu
	{240,3,"GiÊy Mµu Vµng"},
	{202,5,"Thanh Tre"},
	{199,1,"GiÊy Mµu Tr¾ng"},
	{204,1,"Lång §Ìn Thá Con"}}
	local tong = getn(EVENT)-1;
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh«ng ®ñ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.."<color> <color=pink>"..EVENT[i][3].."<color>, vui lßng kiÓm tra l¹i.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("B¹n chÕ t¹o thµnh c«ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].."<color>.")
end

function noinput()
end

function no()
end