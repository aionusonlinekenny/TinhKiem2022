Include("\\script\\header\\event_endtime.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");
-----------------EVENT CONFIG-----------------
EVENT_ACTIVE1 = 1
ACTIVE_MOCEVENT = 1
END_TALK = "Tho¸t/no";
NHANROI = "Ng­¬i ®· nhËn phÇn th­ëng nµy råi.";
MAX_RESTTIME = 1
NOT_LEVEL 	= "B¹n cÇn ph¶i <color=yellow>®¹t %d cÊp<color> ®Ó nhËn th­ëng!"
NHANROI 	= "B¹n ®· nhËn phÇn th­ëng nµy råi."
NHAN_OK		= "B¹n ®· nhËn th­ëng <color=yellow>cÊp %d<color>."
CHUA_RUONG	= "B¹n ph¶i chõa <color=yellow>%d hµng « trèng<color> trong hµnh trang ®Ó nhËn th­ëng!"
NOT_OK		= "B¹n ph¶i vµo ph¸i míi cã thÓ nhËn th­ëng."
----------------------------------------------

function main(NpcIndex)
	if (EVENT_ACTIVE1 == 1) then
	Say2("Ng­¬i cã muèn tham gia Event Quèc Kh¸nh Tö §µn Méc?",3,1,"",
	"Ho¹t §éng Tö §µn Méc/batdau",
	"TÝch Lòy NhËn Tinh ThiÕt TuyÕn/onlttt",
		END_TALK)
	else
	Say2("Sù kiÖn ®· kÕt thóc!",2,1,"",
		END_TALK)
	end
end

function onlttt()
	Say2("Mçi 2 giê online sÏ nhËn ®­îc 6 Tinh ThiÕt TuyÕn, ng­¬i cã muèn nhËn kh«ng?\n\n- Thêi gian online cña ng­¬i lµ: <color=blue>"..GioOnline().."<color> giê <color=blue>"..PhutOnline().."<color> phót",3,1,"",
	"T¹i h¹ ®ång ý/okeonlttt",
		END_TALK)
end

function okeonlttt()
	if GetOnlineMin() < 120 then Talk(1,"","Ng­¬i ch­a ®ñ <color=yellow>2 giê online<color> kh«ng thÓ nhËn th­ëng.") return end
	SetOnlineMin(GetOnlineMin()-60)
	AddEventItem(127,6)
	Msg2Player("<color=yellow>B¹n ®· nhËn ®­îc 6 <color=red>Tinh ThiÕt TuyÕn.");
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

function batdau()
	local nTask = GetTask(TASK_TUDANMOC);
	local TAB_MSG = {
		"<color=red>§¹i S­ L·o L·o<color>: ho¹t ®éng <color=blue>Tö §µn Méc <color> diÔn ra tõ ngµy <color=blue>03-09-2015 ®Õn 03-10-2015<color>, tham gia sù kiÖn ®ång ®¹o sÏ nhËn rÊt nhiÒu kinh nghiÖm vµ vËt phÈm gi¸ trÞ.\n\n<color=yellow>Méc Tr­¬ng <color>\t\t\t\t\t\t\t\t"..GetNumber3(nTask,2).."\n<color=cyan>§éc HuyÒn CÇm <color>\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,3).."",
		-- "§æi Méc Tr­¬ng/ghepevent",
		"§æi §éc HuyÒn CÇm/ghepevent",
		}
	Say2(TAB_MSG[1],3,1,"",
		TAB_MSG[2],
		-- TAB_MSG[3],
		-- TAB_MSG[4],
		END_TALK)
end

function ghepevent()
	-- local n = sel+1
	local nTaskTime = GetTask(TASK_THOIGIAN5);
	local nCurTime = GetTimeMin();
	if(CheckRoom(2,1) == 0) then Talk(1,"",15680) return end
	if(nCurTime - nTaskTime < MAX_RESTTIME) then Talk(1,"",15678) return end
	-- if (n == 1) then
	-- Input("GhepEvent1");
	-- elseif (n == 2) then
	Input("GhepEvent2");
end

function GhepEvent1(num)
	EVENT = {
	{125,"Tö §µn Méc  "},
	{126,"Tinh ThiÕt Táa  "},
	{127,"Tinh ThiÕt TuyÕn  "},
	{128,"Méc T­¬ng  "}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);KiemTraNL2 = GetItemCount(EVENT[2][1],5);KiemTraNL3 = GetItemCount(EVENT[3][1],5)
	NL1YC = num*4; NL2YC = num*3; NL3YC = num*2
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yellow>50 vËt phÈm<color> mµ th«i.") return end
	if (KiemTraNL1 < NL1YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL1.."/"..NL1YC.."<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL2 < NL2YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL2.."/"..NL2YC.."<color> <color=pink>"..EVENT[2][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL3 < NL2YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL3.."/"..NL3YC.."<color> <color=pink>"..EVENT[3][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if(CheckRoom(1,1) == 0) then Talk(1,"",15679) return end
		local Item = ItemSetAdd(0,5,EVENT[4][1],0,0,5,num,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		AddItemID(Item);
		DelItem(EVENT[1][1],5,NL1YC)
		DelItem(EVENT[2][1],5,NL2YC)
		DelItem(EVENT[3][1],5,NL3YC)
		local nCurTime = GetTimeMin();
		SetTask(TASK_THOIGIAN5,nCurTime);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[4][2].."<color>.")
end

function GhepEvent2(num)
	EVENT = {
	{125,"Tö §µn Méc  "},
	{127,"Tinh ThiÕt TuyÕn  "},
	{126,"Tinh ThiÕt Táa  "},
	{124,"L¹n Ng©n Cø  "},
	{129,"§éc HuyÒn CÇm  "}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);KiemTraNL2 = GetItemCount(EVENT[2][1],5);KiemTraNL3 = GetItemCount(EVENT[3][1],5);KiemTraNL4 = GetItemCount(EVENT[4][1],5)
	NL1YC = num*6; NL2YC = num*6; NL3YC = num*1; NL4YC = num*1
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yellow>50 vËt phÈm<color> mµ th«i.") return end
	if (KiemTraNL1 < NL1YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL1.."/"..NL1YC.."<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL2 < NL2YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL2.."/"..NL2YC.."<color> <color=pink>"..EVENT[2][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL3 < NL3YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL3.."/"..NL3YC.."<color> <color=pink>"..EVENT[3][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL4 < NL4YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL4.."/"..NL4YC.."<color> <color=pink>"..EVENT[4][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if(CheckRoom(1,1) == 0) then Talk(1,"",15679) return end
		local Item = ItemSetAdd(0,5,EVENT[5][1],0,0,5,num,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		AddItemID(Item);
		DelItem(EVENT[1][1],5,NL1YC)
		DelItem(EVENT[2][1],5,NL2YC)
		DelItem(EVENT[3][1],5,NL3YC)
		DelItem(EVENT[4][1],5,NL4YC)
		local nCurTime = GetTimeMin();
		SetTask(TASK_THOIGIAN5,nCurTime);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[5][2].."<color>.")
end

function noinput()
end

function no()
end