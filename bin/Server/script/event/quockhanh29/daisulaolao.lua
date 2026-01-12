Include("\\script\\header\\event_endtime.lua");
Include("\\script\\header\\taskid.lua");

-----------------EVENT CONFIG-----------------
EVENT_NPCACTIVE = 1		-- on/off event
END_TALK = "KÕt thóc ®èi tho¹i/no";
MAX_RESTTIME = 10		-- thoi gian gian cach
ITEM_NAM = 2017
ITEM_THANG = 09
ITEM_NGAY = 25
EVENT_NAME = "<color=black><bclr=red>Quèc Kh¸nh 2/9<bclr><color>"
EVENT_TIME = "<color=black><bclr=yellow>30-08-2017 - 20-09-2017<bclr><color>"
----------------------------------------------

function GetTimeLoad()
	Yr,Mth,Dy,Hr,Mn,Se = GetTime();
	TimeNow = Mth * 31 * 24 * 60 * 60 + Dy * 24 * 60 * 60 + Hr * 60 * 60 + Mn * 60 + Se;
	return TimeNow;
end

function main(NpcIndex)
	local nTask = GetTask(TASK_EVENTGOLDKEN2);
	if (EVENT_NPCACTIVE == 1) then
	Say2("Tõ "..EVENT_TIME..", khi mang nguyªn liÖu ®Õn ®©y ta sÏ gióp ng­¬i t×m hiÓu t¸c dông tiÒm Èn cña "..EVENT_NAME..".\n\n<color=yellow>Quèc Kh¸nh(xanh)<color>\t\t\t\t\t"..GetNumber3(nTask,1).."\n<color=red>Quèc Kh¸nh(vµng)<color>\t\t\t\t\t\t"..GetNumber3(nTask,2).."\n<color=orange>Quèc Kh¸nh(®á)<color>\t\t\t\t"..GetNumber3(nTask,3),6,1,"",
	"Quèc Kh¸nh Xanh/ghepevent",
	"Quèc Kh¸nh Vµng/ghepevent",
	"Quèc Kh¸nh §á/ghepevent",
	"NhËn Ch÷ L©m/OnlineGift",
	"H­íng dÉn sù kiÖn/ShowEvent",
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
	Say2("Mçi 2 giê online sÏ nhËn ®­îc 4 ch÷ L©m, ng­¬i cã muèn nhËn kh«ng?\n\n- Thêi gian online cña ng­¬i hiÖn lµ: <color=blue>"..GioOnline().."<color> giê <color=blue>"..PhutOnline().."<color> phót.",2,1,"",
	"T¹i h¹ ®ång ý/OkOnlineGift",
		END_TALK)
end

function OkOnlineGift()
	if (GetOnlineMin() < 120) then Talk(1,"","Ng­¬i ch­a ®ñ <color=yellow>2 giê online<color> kh«ng thÓ nhËn th­ëng.") return end
	SetOnlineMin(GetOnlineMin()-120)
	local Item = ItemSetAdd(0,5,269,0,0,5,4,0);
	SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
	AddItemID(Item);
	Msg2Player("NhËn <color=yellow>4 Ch÷ L©m<color> thµnh c«ng.");
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
	EVENT = {
	{265,2,"Chµo"},
	{266,2,"Mõng"},
	{267,2,"Ngµy"},
	{268,2,"Vâ "},
	{269,1,"L©m"},
	{270,1,"Quèc Kh¸nh(xanh)"},}
	local tong = getn(EVENT)-1;
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh«ng ®ñ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.." <color> <color=pink>"..EVENT[i][3].." <color>, vui lßng kiÓm tra l¹i.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].."<color>.")
end

function GhepEvent2(num)
	EVENT = {
	{265,2,"Chµo"},
	{266,2,"Mõng"},
	{267,2,"Ngµy"},
	{268,2,"Vâ "},
	{269,2,"L©m"},
	{271,1,"Quèc Kh¸nh(vµng)"},}
	local tong = getn(EVENT)-1;
	local TienYC = num*65000
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	if (GetCash() < TienYC) then Talk(1,"","B¹n kh«ng ®ñ <color=red>"..TienYC.." l­îng <color>, vui lßng kiÓm tra l¹i.") return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh«ng ®ñ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.." <color> <color=pink>"..EVENT[i][3].." <color>, vui lßng kiÓm tra l¹i.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		Pay(TienYC)
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].." <color>.")
end

function GhepEvent3(num)
	EVENT = {
	{265,4,"Chµo"},
	{266,4,"Mõng"},
	{267,4,"Ngµy"},
	{268,4,"Vâ "},
	{269,4,"L©m"},
	{273,1,"Tói PhÈm Mµu"},
	{272,1,"Quèc Kh¸nh(®á)"},}
	local tong = getn(EVENT)-1;
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh«ng ®ñ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.." <color> <color=pink>"..EVENT[i][3].." <color>, vui lßng kiÓm tra l¹i.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].." <color>.")
end

function noinput()
end

function no()
end