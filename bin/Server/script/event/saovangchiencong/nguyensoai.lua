Include("\\script\\header\\event_endtime.lua");
Include("\\script\\header\\taskid.lua");

-----------------EVENT CONFIG-----------------
EVENT_NPCACTIVE = 1		-- on/off event
END_TALK = "K’t thÛc ÆËi thoπi/no";
MAX_RESTTIME = 10		-- thoi gian gian cach
ITEM_NAM = 2017
ITEM_THANG = 5
ITEM_NGAY = 25
EVENT_NAME = "<color=black><bclr=red>Sao Vµng Chi’n C´ng<bclr><color>"
EVENT_TIME = "<color=black><bclr=yellow>27-04-2017 - 20-05-2017<bclr><color>"
----------------------------------------------

function GetTimeLoad()
	Yr,Mth,Dy,Hr,Mn,Se = GetTime();
	TimeNow = Mth * 31 * 24 * 60 * 60 + Dy * 24 * 60 * 60 + Hr * 60 * 60 + Mn * 60 + Se;
	return TimeNow;
end

function main(NpcIndex)
	local nTask = GetTask(TASK_EVENTGOLDKEN);
	if (EVENT_NPCACTIVE == 1) then
	Say2("Tı "..EVENT_TIME..", khi mang nguy™n li÷u Æ’n Æ©y ta sœ giÛp ng≠¨i t◊m hi”u t∏c dÙng ti“m »n cÒa "..EVENT_NAME..".\n\n<color=yellow>Huy Hi÷u Sao Vµng<color>\t\t\t\t\t"..GetNumber3(nTask,1).."\n<color=red>Huy Hi÷u Sao ß·<color>\t\t\t\t\t\t"..GetNumber3(nTask,2).."\n<color=orange>Huy Hi÷u ßπi So∏i<color>\t\t\t\t"..GetNumber3(nTask,3),5,1,"",
	"Huy Hi÷u Sao Vµng/ghepevent",
	"Huy Hi÷u Sao ß·/ghepevent",
	"Huy Hi÷u ßπi So∏i/ghepevent",
	"NhÀn Huy Hi÷u 4 Sao/OnlineGift",
		END_TALK)
	elseif (EVENT_NPCACTIVE == 2) then
	Say2("Hi÷n tπi hoπt ÆÈng "..EVENT_NAME.." Æ∑ k’t thÛc.",1,1,"",
		END_TALK)
	else
	Say2("Hoπt ÆÈng "..EVENT_NAME.." sœ bæt Æ«u tı ngµy "..EVENT_TIME..".",1,1,"",
		END_TALK)
	end
end

function OnlineGift()
	if (GetLevel() < 100) then Talk(1,"","Nh©n vÀt d≠Ìi c p 100 kh´ng th” tham gia.") return end
	Say2("MÁi 2 giÍ online sœ nhÀn Æ≠Óc 2 Chocolate, ng≠¨i c„ muËn nhÀn kh´ng?\n\n- ThÍi gian online cÒa ng≠¨i hi÷n lµ: <color=blue>"..GioOnline().."<color> giÍ <color=blue>"..PhutOnline().."<color> phÛt.",2,1,"",
	"Tπi hπ ÆÂng ˝/OkOnlineGift",
		END_TALK)
end

function OkOnlineGift()
	if (GetOnlineMin() < 120) then Talk(1,"","Ng≠¨i ch≠a ÆÒ <color=yellow>2 giÍ online<color> kh´ng th” nhÀn th≠Îng.") return end
	SetOnlineMin(GetOnlineMin()-120)
	local Item = ItemSetAdd(0,5,248,0,0,5,4,0);
	SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
	AddItemID(Item);
	Msg2Player("NhÀn <color=yellow>4 Huy Hi÷u 4 Sao<color> thµnh c´ng.");
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
	if (TimeGhep > GetTimeLoad()) then Talk(1,"","Vui lﬂng chÍ th™m <color=yellow>"..(TimeGhep-GetTimeLoad()).." gi©y<color> n˜a h∑y ÆÊi ti’p.") return end
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
	{245,2,"Huy Hi÷u 1 Sao"},
	{246,2,"Huy Hi÷u 2 Sao"},
	{247,2,"Huy Hi÷u 3 Sao"},
	{248,1,"Huy Hi÷u 4 Sao"},
	{251,1,"Huy Hi÷u Sao Vµng"},}
	local tong = getn(EVENT)-1;
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh´ng ÆÒ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.." <color> <color=pink>"..EVENT[i][3].." <color>, vui lﬂng ki”m tra lπi.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("Bπn ÆÊi thµnh c´ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].."<color>.")
end

function GhepEvent2(num)
	EVENT = {	-- 2 nguyen lieu
	{245,2,"Huy Hi÷u 1 Sao"},
	{246,2,"Huy Hi÷u 2 Sao"},
	{247,2,"Huy Hi÷u 3 Sao"},
	{248,2,"Huy Hi÷u 4 Sao"},
	{252,1,"Huy Hi÷u Sao ß·"},}
	local tong = getn(EVENT)-1;
	local TienYC = num*30000
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	if (GetCash() < TienYC) then Talk(1,"","Bπn kh´ng ÆÒ <color=red>"..TienYC.." l≠Óng <color>, vui lﬂng ki”m tra lπi.") return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh´ng ÆÒ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.." <color> <color=pink>"..EVENT[i][3].." <color>, vui lﬂng ki”m tra lπi.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		Pay(TienYC)
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("Bπn ÆÊi thµnh c´ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].." <color>.")
end

function GhepEvent3(num)
	EVENT = {	-- 4 nguyen lieu
	{245,4,"Huy Hi÷u 1 Sao"},
	{246,4,"Huy Hi÷u 2 Sao"},
	{247,4,"Huy Hi÷u 3 Sao"},
	{248,4,"Huy Hi÷u 4 Sao"},
	{249,1,"Huy Hi÷u Chi’n C´ng"},
	{253,1,"Huy Hi÷u ßπi So∏i"},}
	local tong = getn(EVENT)-1;
	if (num < 1 or num > 50) then Talk(1,"",15685) return end
	for i=1,tong do
		local sl = EVENT[i][2]*num;
		if (GetItemCount(EVENT[i][1],5) < sl) then
		Talk(1,"","Kh´ng ÆÒ <color=red>"..GetItemCount(EVENT[i][1],5).."/"..sl.." <color> <color=pink>"..EVENT[i][3].." <color>, vui lﬂng ki”m tra lπi.")
		return end
	end
		local Item = ItemSetAdd(0,5,EVENT[getn(EVENT)][1],0,0,5,num,0);
		SetItemDate(Item,ITEM_NAM,ITEM_THANG,ITEM_NGAY,0,0);
		AddItemID(Item);
		for i=1,tong do DelItem(EVENT[i][1],5,EVENT[i][2]*num) end
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("Bπn ÆÊi thµnh c´ng "..num.." <color=yellow>"..EVENT[getn(EVENT)][3].." <color>.")
end

function noinput()
end

function no()
end