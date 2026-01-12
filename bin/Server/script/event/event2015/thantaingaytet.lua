Include("\\script\\header\\event_endtime.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");
-----------------EVENT CONFIG-----------------
EVENT_ACTIVE1 = 0
EVENT_ACTIVE2 = 0
END_TALK = "Tho¸t/no";
NHANROI = "Ng­¬i ®· nhËn phÇn th­ëng nµy råi.";
MAX_RESTTIME = 1
----------------------------------------------

function main(NpcIndex)
	if (EVENT_ACTIVE1 == 1) then
	Say2(15676,2,1,"",
	"Ho¹t ®éng Léc Xu©n Nh­ ý/HoatDong1",
	-- "Ho¹t ®éng TrÈy Nªu §Çu N¨m/HoatDong2",
		END_TALK)
	else
	Say2(15676,1,1,"",
		END_TALK)
	end
end

function ghepcapneu(sel)
	local nTaskTime = GetTask(TASK_THOIGIAN5);
	local nCurTime = GetTimeMin();
	if(CheckRoom(2,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 2 « trèng.") return end
	if(nCurTime - nTaskTime < MAX_RESTTIME) then Talk(1,"",15678) return end
	SetTaskTemp(EVENT_TET, SetNumber(GetTaskTemp(EVENT_TET),6,sel));
	Input("GhepCayNeu");
end

function HoatDong1()
	local nTask = GetTask(TASK_MUTTET);
	local TAB_MSG = {
		"<color=red>ThÇn Tµi<color>: ho¹t ®éng <color=blue>Léc Xu©n Nh­ ý <color>diÔn ra tõ ngµy <bclr=blue>15-01-2015 ®Õn 28-02-2015<bclr>, tham gia sù kiÖn ®ång ®¹o sÏ nhËn rÊt nhiÒu kinh nghiÖm vµ vËt phÈm gi¸ trÞ.\n\n<color=yellow>Møt Gõng<color>\t\t\t\t\t\t\t\t"..GetNumber3(nTask,1).."\n<color=cyan>Møt BÝ <color>\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,2).."\n<color=red>Møt Dõa<color>\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,3).."",
		"ChÕ biÕn Møt Gõng/ghepevent",
		"ChÕ biÕn Møt BÝ /ghepevent",
		"ChÕ biÕn Møt Dõa/ghepevent",
		}
	Say2(TAB_MSG[1],4,1,"",
		TAB_MSG[2],
		TAB_MSG[3],
		TAB_MSG[4],
		END_TALK)
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
	{78,"Tói §­êng"},
	{79,"Gõng"},
	{83,"Møt Gõng"}}
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
	{78,"Tói §­êng"},
	{80,"BÝ "},
	{84,"Møt Gõng"}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);KiemTraNL2 = GetItemCount(EVENT[2][1],5)
	NL1YC = num*3; NL2YC = num*2
	TienYC = num*30000
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
	{78,"Tói §­êng"},
	{81,"Dõa"},
	{85,"Møt Dõa"}}
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