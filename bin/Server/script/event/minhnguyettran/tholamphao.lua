---Script by Manh Dat
---Tho Lam Phao
Include("\\script\\header\\event_endtime.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");
END_TALK = "KÕt thóc ®èi tho¹i/no";
MAX_RESTTIME = 10
function GetTimeLoad()
	Yr,Mth,Dy,Hr,Mn,Se = GetTime();
	TimeNow = Mth * 31 * 24 * 60 * 60 + Dy * 24 * 60 * 60 + Hr * 60 * 60 + Mn * 60 + Se;
	return TimeNow;
end
function main(NpcIndex)
	Say2("<color=red>Thî Lµm Ph¸o<color>: sù kiÖn <color=blue>Rén R· Giao Thõa<color> do ta phô tr¸ch, ngµi t×m ta cã viÖc g×?",4,1,"",
	"Ho¹t ®éng Ph¸o Hoa N¨m Míi/PhaoHoa2016",
	"Ta mu«n mua D©y Cét Phong Ph¸o §¹i/daycot",
	END_TALK)
end


function PhaoHoa2016()
	local nTask = GetTask(TASK_PHONGPHAO2017);
	local TAB_MSG = {
		"<color=red>Thî Lµm Ph¸o<color>: ho¹t ®éng <color=blue>Rén R· Giao Thõa<color> diÔn ra tõ ngµy <color=blue>20-1-2017 ®Õn 05-02-2016<color>, tham gia sù kiÖn ®ång ®¹o sÏ nhËn rÊt nhiÒu kinh nghiÖm vµ vËt phÈm gi¸ trÞ.\n\n<color=yellow>Phong ph¸o tiÓu<color>\t\t\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,1).."\n<color=cyan>Phong ph¸o trung<color>\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,2).."\n<color=red>Phong ph¸o ®¹i<color>\t\t\t\t\t\t\t\t\t"..GetNumber3(nTask,3).."",
		"GhÐp phong ph¸o tiÓu/ghep1",
		"GhÐp phong ph¸o trung/ghep2",
		"GhÐp phong ph¸o ®¹i/ghep3",
		}
	Say2(TAB_MSG[1],5,1,"",
		TAB_MSG[2],
		TAB_MSG[3],
		TAB_MSG[4],
		END_TALK)
end

function ghep1()
	local TimeGhep = GetTaskTemp(TEMP_TIMEEVENT)
	if (TimeGhep > GetTimeLoad()) then Talk(1,"","Vui lßng chê thªm <color=yellow>"..(TimeGhep-GetTimeLoad()).." gi©y<color> n÷a h·y ®æi tiÕp.") return end
	if(CheckRoom(2,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 2 « trèng.") return end
	Input("GhepEvent1");
end

function ghep2()
	local TimeGhep = GetTaskTemp(TEMP_TIMEEVENT)
	if (TimeGhep > GetTimeLoad()) then Talk(1,"","Vui lßng chê thªm <color=yellow>"..(TimeGhep-GetTimeLoad()).." gi©y<color> n÷a h·y ®æi tiÕp.") return end
	if(CheckRoom(2,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 2 « trèng.") return end
	Input("GhepEvent2");
end

function ghep3()
	local TimeGhep = GetTaskTemp(TEMP_TIMEEVENT)
	if (TimeGhep > GetTimeLoad()) then Talk(1,"","Vui lßng chê thªm <color=yellow>"..(TimeGhep-GetTimeLoad()).." gi©y<color> n÷a h·y ®æi tiÕp.") return end
	if(CheckRoom(2,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 2 « trèng.") return end
	Input("GhepEvent3");
end

function GhepEvent1(num)
	EVENT = {
	{137,"Ph¸o tiÓu"},
	{140,"Phong ph¸o tiÓu"}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);
	PhaoTieu = num*5;
	TienYC = num*20000
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yelow>50 vËt phÈm<color> mµ th«i.") return end
	if (KiemTraNL1 < PhaoTieu) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL1.."/"..PhaoTieu.."<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (GetCash() < TienYC) then Talk(1,"","B¹n kh«ng ®ñ <color=red>"..TienYC.." l­îng<color>, vui lßng kiÓm tra l¹i.") return end
		if(CheckRoom(1,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 1 « trèng.") return end
		Pay(TienYC)
		DelItem(EVENT[1][1],5,PhaoTieu)
		local Item = ItemSetAdd(0,5,EVENT[2][1],0,0,5,num,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		AddItemID(Item);
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[2][2].."<color>.")
end

function GhepEvent2(num)
	EVENT = {
	{137,"Ph¸o tiÓu"},
	{138,"Ph¸o trung"},
	{141,"Phong ph¸o trung"}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);
	KiemTraNL2 = GetItemCount(EVENT[2][1],5)
	PhaoTieu = num*5;
	PhaoTrung = num*5
	TienYC = num*30000
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yelow>50 vËt phÈm<color> mµ th«i.") return end
	if (KiemTraNL1 < PhaoTieu) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL1.."/"..PhaoTieu.."<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL2 < PhaoTrung) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL2.."/"..PhaoTrung.."<color> <color=pink>"..EVENT[2][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (GetCash() < TienYC) then Talk(1,"","B¹n kh«ng ®ñ <color=red>"..TienYC.." l­îng<color>, vui lßng kiÓm tra l¹i.") return end	
	if(CheckRoom(1,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 1 « trèng.") return end
		Pay(TienYC)
		DelItem(EVENT[1][1],5,PhaoTieu)
		DelItem(EVENT[2][1],5,PhaoTrung)
		local Item = ItemSetAdd(0,5,EVENT[3][1],0,0,5,num,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		AddItemID(Item);
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);	
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[3][2].."<color>.")
end

function GhepEvent3(num)
	EVENT = {
	{137,"Ph¸o tiÓu"},
	{138,"Ph¸o trung"},
	{139,"Ph¸o ®¹i"},
	{143,"D©y cét phong ph¸o ®¹i"},
	{142,"Phong ph¸o ®¹i"}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);
	KiemTraNL2 = GetItemCount(EVENT[2][1],5);
	KiemTraNL3 = GetItemCount(EVENT[3][1],5);
	KiemTraNL4 = GetItemCount(EVENT[4][1],5);
	PhaoTieu = num*5;
	PhaoTrung = num*5;
	PhaoDai = num*5;
	TienYC = num*40000
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yelow>50 vËt phÈm<color> mµ th«i.") return end
	if (KiemTraNL1 < PhaoTieu) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL1.."/"..PhaoTieu.."<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL2 < PhaoTrung) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL2.."/"..PhaoTrung.."<color> <color=pink>"..EVENT[2][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL3 < PhaoDai) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL3.."/"..PhaoDai.."<color> <color=pink>"..EVENT[3][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL4 < num) then Talk(1,"","Kh«ng ®ñ nguyªn liÖu, b¹n ®ang cã <color=red>"..KiemTraNL4.."/"..num.."<color> <color=pink>"..EVENT[4][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (GetCash() < TienYC) then Talk(1,"","B¹n kh«ng ®ñ <color=red>"..TienYC.." l­îng<color>, vui lßng kiÓm tra l¹i.") return end	
	if(CheckRoom(1,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 1 « trèng.") return end
		Pay(TienYC)
		DelItem(EVENT[1][1],5,PhaoTieu)
		DelItem(EVENT[2][1],5,PhaoTrung)
		DelItem(EVENT[3][1],5,PhaiDai)
		DelItem(EVENT[4][1],5,num)
		local Item = ItemSetAdd(0,5,EVENT[5][1],0,0,5,num,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		AddItemID(Item);
		SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);
		Msg2Player("B¹n ®æi thµnh c«ng "..num.." <color=yellow>"..EVENT[5][2].."<color>.")
end

function daycot()
	local TimeGhep = GetTaskTemp(TEMP_TIMEEVENT)
	if(CheckRoom(1,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 1 « trèng.") return end
	if (TimeGhep > GetTimeLoad()) then Talk(1,"","Vui lßng chê thªm <color=yellow>"..(TimeGhep-GetTimeLoad()).." gi©y<color> n÷a h·y mua tiÕp.") return end
	Input("MuaDayCot");
end

function MuaDayCot(num)
	XuYC = num*12
	if (num < 1 or num > 50) then Talk(1,"","Mçi lÇn ®æi chØ ®­îc <color=yelow>50 D©y Cét Phong Phao<color> mµ th«i.") return end
	if (GetCoin(1) < XuYC) then Talk(1,"","B¹n kh«ng ®ñ <color=red>"..XuYC.." xu<color>, vui lßng kiÓm tra l¹i.") return end
	PayCoin(XuYC)
	local Item = ItemSetAdd(0,5,143,0,0,5,num,0);
	SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
	AddItemID(Item);
	Msg2Player("B¹n ®· mua "..num.." <color=yellow>D©y Cét Phong Ph¸o §¹i<color>.")
	SetTaskTemp(TEMP_TIMEEVENT,GetTimeLoad()+MAX_RESTTIME);	
	logMuaDayCot("TK: "..GetAccount().."\t\tName: "..GetName().."\t\tLevel: "..GetLevel().."\t\tXu truoc: "..(GetCoin(0)+XuYC).."\t\tXu sau: "..GetCoin(0).."\t\t\tMua "..num.." day cot")
end

function noinput()
end

function no()
end