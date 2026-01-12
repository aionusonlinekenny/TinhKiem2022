Include("\\script\\feature\\event2015\\thantai.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");

Yr,Mth,Dy,Hr,Mn,Se = GetTime()
TimeEvent = (""..Hr..":"..Mn..":"..Se.." - "..Dy.."/"..Mth.."/"..Yr.."")

function main(nIndex)
	if (EVENT_ACTIVE1 == 2) then
		local Npcid = GetTaskTemp(EVENT_TETNPCDW);
		if(Npcid > 0) then
			local w,x,y;
			Npcid,w,x,y = FindNpc(Npcid);
			if(Npcid < 1) then
			StopTimer();
			SetTaskTemp(EVENT_TET,0);
			SetTaskTemp(EVENT_TETNPCDW, 0);
			SetRes(-1);
			end
		end
		local nTask = GetTaskTemp(EVENT_TET);
		local nBuoc = GetNumber(nTask, 5);
		if(nBuoc == 0) then
		Say(15675,2,
		"§ång ý nhiÖm vô/nhannv",
		"Ta chØ ghÐ qua/no")
		elseif(nBuoc > 4) then
			hoanthanh()
		else
			Talk(1,"","Ng­¬i vÉn ch­a ®i hÕt 4 h­íng cña thµnh. Ta vÉn ®ang quan s¸t ng­¬i ®©y, ®õng cã l­êi nhÐ !")
		end
	else
		Say(15675,1,
		"Ta chØ ghÐ qua/no")
	end
end;

TAB_THANH={
-- {1  ,1599,3216},
{11 ,3127,5063},
{37 ,1744,3066},
-- {78 ,1552,3254},
{80 ,1773,3040}
};

TAB_RANDOM={
{1,2,3,4},
{1,2,4,3},
{1,3,2,4},
{1,3,4,2},
{1,4,2,3},
{1,4,3,2},
{2,1,3,4},
{2,1,4,3},
{2,3,1,4},
{2,3,4,1},
{2,4,1,3},
{2,4,3,1},
{3,1,2,4},
{3,1,4,2},
{3,2,1,4},
{3,2,4,1},
{3,4,1,2},
{3,4,2,1},
{4,1,2,3},
{4,1,3,2},
{4,2,1,3},
{4,2,3,1},
{4,3,1,2},
{4,3,2,1}
};

TAB_HUONG={
"§«ng",
"T©y",
"Nam",
"B¾c"
};

function nhannv()
	if(GetFightState() == 1) then Talk(1,"","Tr¹ng th¸i chiÕn ®Êu kh«ng thÓ nhËn nhiÖm vô.") return end
	if(GetLevel() < 100) then Talk(1,"","H·y cè g¾ng luyÖn tËp ®¹t cÊp 100 råi ®Õn gÆp ta.") return end
	local nTask = GetTask(TASK_RESET3);
	local nLan = GetNumber(nTask, 5);
	if(nLan > 1) then Talk(1,"","Ngµy mai h·y ®Õn ®©y tham gia tiÕp nhÐ b¹n trÎ.") return end
	------------------------------------
	SetTask(TASK_RESET3, SetNumber(nTask,5,nLan+1));
	local nTask = 10000;--o^ [5]=1
	local nRand = random(getn(TAB_RANDOM));
	nTask = SetNumber(nTask,1,TAB_RANDOM[nRand][1]);
	nTask = SetNumber(nTask,2,TAB_RANDOM[nRand][2]);
	nTask = SetNumber(nTask,3,TAB_RANDOM[nRand][3]);
	nTask = SetNumber(nTask,4,TAB_RANDOM[nRand][4]);
	SetTaskTemp(EVENT_TET,nTask);
	local nRandTT = random(getn(TAB_THANH));
	NewWorld(TAB_THANH[nRandTT][1], TAB_THANH[nRandTT][2], TAB_THANH[nRandTT][3]);
	local nId=AddNpcNew(random(727,728),1,TAB_THANH[nRandTT][1],TAB_THANH[nRandTT][2]*32,TAB_THANH[nRandTT][3]*32,"\\script\\event2015\\conlan.lua",0,"L©n S­-"..GetName(),nil,"555",0);
	SetNpcLifeTime(nId, 34560);--32 phut de phong su co'
	SetNpcTG(nId,GetUUID());
	SetTaskTemp(EVENT_TETNPCDW, GetNpcID(nId));
	SetTimer(32400,6);--30 phut nvu that bai
	SetRes(726);
	Msg2Player("B¹n b¾t ®Çu di chuyÓn lÇn l­ît c¸c h­íng<color=yellow> "..TAB_HUONG[TAB_RANDOM[nRand][1]].."-"..TAB_HUONG[TAB_RANDOM[nRand][2]].."-"..TAB_HUONG[TAB_RANDOM[nRand][3]].."-"..TAB_HUONG[TAB_RANDOM[nRand][4]]);
	AddNote("("..TimeEvent..") <color=red>L©n S­ Mõng TÕt<color>: ngµi h·y di chuyÓn l©n theo thø tù cöa thµnh "..TAB_HUONG[TAB_RANDOM[nRand][1]].."-"..TAB_HUONG[TAB_RANDOM[nRand][2]].."-"..TAB_HUONG[TAB_RANDOM[nRand][3]].."-"..TAB_HUONG[TAB_RANDOM[nRand][4]]);
end;

function hoanthanh()
	local nNpcdw = GetTaskTemp(EVENT_TETNPCDW);
	local nNpcIdx = FindNearNpc(nNpcdw);
	-- if(nNpcIdx < 1) then
	-- Talk(1,"","L©n s­ cña ng­¬i bÞ thÊt l¹c ë ®©u råi? Mau t×m l¹i ®©y!")
	-- return end
	nNpcIdx,w,x,y = FindNpc(nNpcdw);
	if(nNpcIdx > 0) then
		DelNpc(nNpcIdx)
	end
	StopTimer();
	SetTaskTemp(EVENT_TET,0);
	SetTaskTemp(EVENT_TETNPCDW, 0);
	SetRes(-1);
	-------------------------------------
	AddOwnExp(20000000)
	if (RANDOM(300) == 248) then VPNgauNhien() else ThongBaoEvent() end
	-------------------------------------
	Talk(1,"","Tèt l¾m b¹n trÎ ! Ng­¬i ®· gãp phÇn t¹o nªn mïa xu©n cho nh©n gian råi ®Êy!")
	AddNote("("..TimeEvent..") <color=red>L©n S­ Mõng TÕt<color>: ngµi hoµn thµnh nhiÖm vô nhËn ®­îc phÇn th­ëng xøng ®¸ng.")
end;

function VPNgauNhien()
	if RANDOM(3) >= 1 then ThongBaoEvent() return end
	a = random(1,1)
	AddMat(MATERIAL[a][1])
	logThuongMuaLan(""..GetAccount().."\t\t\t"..GetName().."\t\t\t"..GOLD[a][2].."")
	Msg2SubWorld("Chóc mõng<color=yellow> "..GetName().." <color>tham gia ho¹t ®éng móa L©n S­ Mõng TÕt ë L©m An nhËn ®­îc <color=cyan>trang bÞ "..GOLD[a][2].."")
end

MATERIAL = {
	{37,"§å phæ V©n Léc"},
	{39,"§å phæ Th­¬ng Lang"},
	{41,"§å phæ HuyÒn Viªn"},
	{43,"§å phæ Tö M·ng"},
	{45,"§å phæ Kim ¤"},
	{47,"§å phæ B¹ch Hæ"},
	{49,"§å phæ XÝch L©n"},
	{51,"§å phæ Minh Ph­îng"},
}

function ThongBaoEvent()
	Msg2SubWorld("Chóc mõng<color=yellow> "..GetName().." <color>tham gia ho¹t ®éng móa L©n S­ Mõng TÕt nhËn ®­îc rÊt nhiÒu ®iÓm kinh nghiÖm.")
end

function logThuongMuaLan(str)
local gm_Log = "dulieu/SuKien/MuaLanMungTet.txt"
local fs_log = openfile(gm_Log, "a");
write(fs_log, date("%H:%M:%S_%d-%m-%y").."\t"..str.."\n");
closefile(fs_log);
end

function no()
end;