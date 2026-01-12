Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");
BOSS_NVBANG={
	{"Ph¶n §« Thõa V©n",342,39625,76988},
	{"Ph¶n §å Long §Ønh",342,42422,78196},
	{"Ph¶n §å DiÖp VÊn",342,39942,96050},
	{"Ph¶n §å Tµ Long",342,50600,80184},
	{"Ph¶n §å TiÓu Linh",342,47377,85432},
	{"Ph¶n §å TuyÕt V©n",342,47936,91192},
	{"Ph¶n §å Hµng Long ",342,49282,97216},
	{"Ph¶n §å Minh Trung",342,44918,95162},
	{"PhÈn §å Nh©n Trung",342,41702,99526},
	{"Ph¶n §å H¹ TuyÕt",342,38408,98206},
	{"Ph¶n §å NhËt H¶o",342,39698,92024},
	{"Ph¶n §å TuÖ Trung",342,38067,86074},
	{"Ph¶n §å Tµ KhiÕt",342,39954,78582},
	{"Ph¶n §å YÕn Thanh",342,42877,77912},
	{"Ph¶n §å Th«ng Thiªn",342,47361,77140},
	{"Ph¶n §å Hµ H­¬ng",342,49707,80936},
	{"Ph¶n §å L­u Xu©n",342,47818,86446},
	{"Ph¶n §å Thanh TuyÖt",342,48677,92244}
	};

MAX_PERDAY = 10
function main()
if DUATOP_ACTIVE == 2 then Talk(1,"","TÝnh n¨ng b¶o tr×.") return end
if(GetTongID() == 0) then Talk(1,"","Ph¶i cã bang míi cã thÓ sö dông ®­îc!") return end
	local banghoi = GetTongName()
	local DatDHDB = GetTempInt("DiemHDBang",GetTongID())
	local nTaskValue = GetTask(TASK_TINHHOA,1);
	local nDone = GetNumber2(GetTask(TASK_RESET6),1);
	local bHave = GetNumber2(GetTask(TASK_NVBANG),1)
	if(bHave > 0) then
	Say2("Nh÷ng tªn ph¶n ®å cña bang héi vÉn ®ang lÈn trèn vµ ung dung sèng t¹i Vi S¬n §¶o,ng­¬i mau lªn ®ã thanh to¸n h¾n. <enter>Bang héi : <color=yellow>"..banghoi.." <color> <enter>§iÓm H§ bang : <color=yellow>"..DatDHDB.." <color> <enter>NhiÖm vô ®· lµm trong ngµy : <color=yellow>"..nDone.."<color> <enter>§iÖm Tinh Hoa : <color=yellow> "..nTaskValue.."",3,1,"",
	"Hñy bá nhiÖm bang/huynhiemvubang",
	"Di chuyÓn ®Õn khu vùc nhiÖm vô/dichuyenkvnv",
	"§ãng/no")
	return end
	Say2("Nh÷ng tªn ph¶n ®å cña bang héi vÉn ®ang lÈn trèn vµ ung dung sèng t¹i Vi S¬n §¶o,ng­¬i mau lªn ®ã thanh to¸n h¾n. <enter>Bang héi : <color=yellow>"..banghoi.." <color> <enter>§iÓm H§ bang : <color=yellow>"..DatDHDB.." <color> <enter>NhiÖm vô ®· lµm trong ngµy : <color=yellow>"..nDone.."<color> <enter>§iÖm Tinh Hoa : <color=yellow> "..nTaskValue.."",4,1,"",
    "NhËn nhiÖm vô bang/nhiemvunhan",
	"KiÕm tra t×nh h×nh nhiÖm vô/kiemtranv",
	"Di chuyÓn ®Õn khu vùc nhiÖm vô/dichuyenkvnv",
	"§ãng/no")
end

function kiemtranv()
local banghoi = GetTongName()
local DatDHDB = GetTempInt("DiemHDBang",GetTongID())
local nDone = GetNumber2(GetTask(TASK_RESET6),1);
Talk(1,"","Bang héi : <color=yellow>"..banghoi.." <color> <enter>§iÓm H§ bang : <color=yellow>"..DatDHDB.." <color> <enter>NhiÖm vô ®· lµm trong ngµy : <color=yellow>"..nDone.." ")
end

function dichuyenkvnv()
	local nDangCap = GetLevel()
	local TrungSinh = GetTask(TASK_CHUYENSINH);
	-- if(TrungSinh < 1) then Talk(1,"","Nh©n vËt ch­a chuyÓn sinh kh«ng thÓ tham gia") return end
	-- if(TrungSinh > 1) then Talk(1,"","Nh÷ng chuyÖn nµy h·y nh­êng l¹i cho nh÷ng huynh ®µi cßn Ýt kinh nghiÖm xö lý,cao thñ trong bang kh«ng cÇn ra tay") return end
	-- if(nDangCap > 155) then Talk(1,"","Nh÷ng chuyÖn nµy h·y nh­êng l¹i cho nh÷ng huynh ®µi cßn Ýt kinh nghiÖm xö lý,cao thñ trong bang kh«ng cÇn ra tay.") return end
	NewWorld(342,1179,2429)
	SetFightState(1)
end
function no()
end

function nhiemvunhan()
	local nValue = GetTask(TASK_NVBANG);
	local nDone = GetNumber2(GetTask(TASK_RESET6),1);
	local nDangCap = GetLevel()
	local TrungSinh = GetTask(TASK_CHUYENSINH);
	-- if(TrungSinh < 1) then Talk(1,"","Nh©n vËt ch­a chuyÓn sinh kh«ng thÓ tham gia") return end
	-- if(TrungSinh > 1) then Talk(1,"","Nh÷ng chuyÖn nµy h·y nh­êng l¹i cho nh÷ng huynh ®µi cßn Ýt kinh nghiÖm xö lý,cao thñ trong bang kh«ng cÇn ra tay") return end
	-- if(nDangCap > 155) then Talk(1,"","Nh÷ng chuyÖn nµy h·y nh­êng l¹i cho nh÷ng huynh ®µi cßn Ýt kinh nghiÖm xö lý,cao thñ trong bang kh«ng cÇn ra tay.") return end
	if(nDone >= MAX_PERDAY) then
		Talk(1,"","Ngµy h«m nay b¹n ®· hoµn thµnh ®ñ nhiÖm vô bang héi,ngµy mai cã thÓ quay l¹i lµm tiÕp.")
	return end
	local nNum = RANDOM(1,getn(BOSS_NVBANG));
	SetTask(TASK_NVBANG, SetNumber2(nValue,1,nNum));
	NewWorld(342,1179,2429)
	SetFightState(1)
	local str = format("%s ®ang ë %s %d/%d. Bµn tay h¾n nhuém ®Çy m¸u t­¬i. Ta kh«ng tin lµ ng­¬i giÕt ®­îc h¾n.", BOSS_NVBANG[nNum][1], GetSubWorldName(BOSS_NVBANG[nNum][2]), floor(BOSS_NVBANG[nNum][3]/256), floor(BOSS_NVBANG[nNum][4]/512));
	Talk(1,"",str);
	str = format("L·nh Nguyªn So¸i: giÕt %s %s %d/%d.", BOSS_NVBANG[nNum][1], GetSubWorldName(BOSS_NVBANG[nNum][2]), floor(BOSS_NVBANG[nNum][3]/256), floor(BOSS_NVBANG[nNum][4]/512));
	AddNote(str);
end;

function huynhiemvubang()
	Say2("Nh÷ng tªn ph¶n ®å cña bang héi vÉn ®ang lÈn trèn vµ ung dung sèng t¹i Vi S¬n §¶o,ng­¬i mau lªn ®ã thanh to¸n h¾n.", 2,1,"",
	"Ta muèn hñy./xacnhanhuynv",
	"§Ó ta suy nghÜ l¹i./no")
end;

function xacnhanhuynv()
	local nValue = GetTask(TASK_NVBANG);
	SetTask(TASK_NVBANG, SetNumber2(nValue,1,0));
	Msg2Player("Ng­¬i kh«ng ®ñ søc råi ,hÑn gÆp l¹i.")
end;
