Include("\\script\\header\\event_endtime.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");
-----------------EVENT CONFIG-----------------
EVENT_ACTIVE1 = 1
END_TALK = "Tho¸t/no";
NHANROI = "Ng­¬i ®· nhËn phÇn th­ëng nµy råi.";
EVENT_NAME = "<color=black><bclr=yellow>N÷ Nhi H­¬ng - Hoa Hång May M¾n <bclr><color>"
EVENT_TIME = "<color=black><bclr=red>03-03-2015 - 23-03-2015<bclr><color>"
MAX_RESTTIME = 1
----------------------------------------------

function main(NpcIndex)
local nTask = GetTask(TASK_HOAHONG);
	if (EVENT_ACTIVE1 == 1) then
	Say2("Tõ "..EVENT_TIME..", ta lµ ng­êi qu¶n lý ho¹t ®éng "..EVENT_NAME..", cÇn g× cø ®Õn gÆp ta.\n\n\n<color=red>Vßng Hoa Vµng<color>\t\t\t\t\t\t\t\t"..GetNumber3(nTask,1).."/300\n<color=green>Vßng Hoa Hång<color>\t\t\t\t\t\t\t\t"..GetNumber3(nTask,2).."/400\n<color=cyan>Vßng Hoa Xanh<color>\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,3).."/500",4,1,"",	
	"GhÐp Vßng Hoa Vµng/ghepevent",
	"GhÐp Vßng Hoa Hång/ghepevent",
	"GhÐp Vßng Hoa Xanh/ghepevent",
	END_TALK)
	else
	Say2("Tõ "..EVENT_TIME..", ta lµ ng­êi qu¶n lý ho¹t ®éng "..EVENT_NAME..", cÇn g× cø ®Õn gÆp ta.\n\n\n<color=red>Vßng Hoa Vµng<color>\t\t\t\t\t\t\t\t"..GetNumber3(nTask,1).."/300\n<color=green>Vßng Hoa Hång<color>\t\t\t\t\t\t\t\t"..GetNumber3(nTask,2).."/400\n<color=cyan>Vßng Hoa Xanh<color>\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,3).."/500",1,1,"",		
	END_TALK)
	end
end


function ghepevent(sel)
	local n = sel+1
	local nTaskTime = GetTask(TASK_THOIGIAN5);
	local nCurTime = GetTimeMin();
	if(CheckRoom(2,1) == 0) then Talk(1,"",15680) return end
	if(nCurTime - nTaskTime < MAX_RESTTIME) then Talk(1,"",15678) return end
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
	{91,"Hoa cóc vµng "},
	{92,"Hoa cóc tÝm "},
	{96,"Vßng Hoa Vµng "}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);KiemTraNL2 = GetItemCount(EVENT[2][1],5)
	NL1YC = num*5; NL2YC = num*3
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yellow>50 vËt phÈm<color> mµ th«i.") return end
	if (KiemTraNL1 < NL1YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL1.."/"..NL1YC.."<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL2 < NL2YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL2.."/"..NL2YC.."<color> <color=pink>"..EVENT[2][2].."<color>, vui lßng kiÓm tra l¹i.") return end
		if(CheckRoom(1,1) == 0) then Talk(1,"",15679) return end
		local Item = ItemSetAdd(0,5,EVENT[3][1],0,0,5,num,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		AddItemID(Item);
		DelItem(EVENT[1][1],5,NL1YC)
		DelItem(EVENT[2][1],5,NL2YC)
		local nCurTime = GetTimeMin();
		SetTask(TASK_THOIGIAN5,nCurTime);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[3][2].."<color>.")
end

function GhepEvent2(num)
	EVENT = {
	{91,"Hoa cóc vµng "},
	{93,"Hoa cóc tr¾ng "},
	{97,"Vßng Hoa Hång "}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);KiemTraNL2 = GetItemCount(EVENT[2][1],5)
	NL1YC = num*3; NL2YC = num*2
	TienYC = num*50000
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yellow>50 vËt phÈm<color> mµ th«i.") return end
	if (KiemTraNL1 < NL1YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL1.."/"..NL1YC.."<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL2 < NL2YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL2.."/"..NL2YC.."<color> <color=pink>"..EVENT[2][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (GetCash() < TienYC) then Talk(1,"","B¹n kh«ng ®ñ <color=red>"..TienYC.." l­îng<color>, vui lßng kiÓm tra l¹i.") return end
		if(CheckRoom(1,1) == 0) then Talk(1,"",15679) return end
		local Item = ItemSetAdd(0,5,EVENT[3][1],0,0,5,num,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		AddItemID(Item);
		Pay(TienYC)
		DelItem(EVENT[1][1],5,NL1YC)
		DelItem(EVENT[2][1],5,NL2YC)
		local nCurTime = GetTimeMin();
		SetTask(TASK_THOIGIAN5,nCurTime);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[3][2].."<color>.")
end

function GhepEvent3(num)
	EVENT = {
	{91,"Hoa cóc vµng "},
	{94,"Hoa hång "},
	{98,"Vßng Hoa Xanh "}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);KiemTraNL2 = GetItemCount(EVENT[2][1],5)
	NL1YC = num*5
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yellow>50 vËt phÈm<color> mµ th«i.") return end
	if (KiemTraNL1 < NL1YC) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL1.."/"..NL1YC.."<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL2 < num) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL2.."/"..num.."<color> <color=pink>"..EVENT[2][2].."<color>, vui lßng kiÓm tra l¹i.") return end
		if(CheckRoom(1,1) == 0) then Talk(1,"",15679) return end
		local Item = ItemSetAdd(0,5,EVENT[3][1],0,0,5,num,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		AddItemID(Item);
		DelItem(EVENT[1][1],5,NL1YC)
		DelItem(EVENT[2][1],5,num)
		local nCurTime = GetTimeMin();
		SetTask(TASK_THOIGIAN5,nCurTime);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[3][2].."<color>.")
end

function noinput()
end

function no()
end