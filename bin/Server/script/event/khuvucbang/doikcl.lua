Include("\\script\\header\\taskid.lua");
Include("\\script\\header\\congthanh.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\feature\\sukiengame\\khuvucbang\\doimatna.lua");
SOLUONGKCLYC = 35000
SOLUONGDHDYCB = 2000
XUCANBBH = 5000
DTHYC = 100
TIENVANYCTH = 1000000
TIENTRUNGTU = 6990000
TIENTRUNGTUKO = 15000000
TIENTRUNGTUBH = 30000000
HDDTYC = 100
MANHGHEPANHM = 98
TIENCANBBH = 200000000
DETAIL_NAMECD = {"HuyÒn Ch©n §¬n",
			     "HuyÕt Ch©n §¬n",
			     "B¹ch Ch©n §¬n"}
				 
DETAIL_VP = {"B¹n ®ång hµnh. Ngo¹i quan quyÓn (trung)",
			 "B¹n ®ång hµnh. Ngo¹i quan quyÓn (cao)",
			 "B¹n ®ång hµnh. Kü n¨ng quyÓn (trung)",
			 "B¹n ®ång hµnh. Kü n¨ng quyÓn (cao)"}
			 
DETAIL_CD = {107,108,109};
function main()
if DUATOP_ACTIVE == 2 then Talk(1,"","TÝnh n¨ng b¶o tr×.") return end
if(GetTongID() == 0) then Talk(1,"","Ph¶i cã bang míi cã thÓ sö dông ®­îc!") return end
local banghoi = GetTongName()
local DatDHDB = GetTempInt("DiemHDBang",GetTongID())
local nTaskValue = GetTask(TASK_TINHHOA,1);
	Say2("Ta ®ang lµ Thèng LÜnh Khu Vùc bang , ng­¬i cÇn g× ë ta? <enter>Bang héi : <color=yellow>"..banghoi.." <color> <enter>§iÓm H§ bang : <color=yellow>"..DatDHDB.." <color> <enter>§iÖm Tinh Hoa : <color=yellow> "..nTaskValue.."",7,1,"",
        -- "§¨ng ký ®¹i chiÕn Boss Bang Héi/doikcl",
		-- "Ta muèn ®æi mÆt n¹ hoµng kim/doimatnanew",
		-- "Ta muèn ®æi méc nh©n lÖnh bµi/doimnlb",
		"Ta muèn më ®¹i tiÖc bang héi/daitiecbanghoi",
		"TÝnh n¨ng trïng tu trang bÞ/trungtutrangbi",
		-- "TÝnh n¨ng ®ång hµnh ®Æc biÖt/tinhnangdonghanh",
		"Xem sè l­îng Khiªu ChiÕn LÖnh/XemSoLuongKCL",
		"KÕt thóc ®èi tho¹i/no")
end

function tinhnangdonghanh()
Say2("H·y lùa chän lo¹i?",3,1,"",
	"LuyÖn ngo¹i quan ®ång hµnh/ngoaiquan",
	"LuyÖn s¸ch kü n¨ng ®ång hµnh/kndh",
	"KÕt thóc ®èi tho¹i/no")
end

function ngoaiquan()
Say2("Chän luyÖn ngo¹i quan?",3,1,"",
	"Ngo¹i quan quyÓn trung cÊp/ngoaiquan2",
	"Ngo¹i quan quyÒn cao cÊp/ngoaiquan2",
	"KÕt thóc ®èi tho¹i/no")
end

function kndh()
Say2("Chän luyÖn s¸ch kü n¨ng?",3,1,"",
	"S¸ch kü n¨ng quyÓn trung cÊp/kndh2",
	"S¸ch kü n¨ng quyÒn cao cÊp/kndh2",
	"KÕt thóc ®èi tho¹i/no")
end

function trungtutrangbi()
Say2("H·y lùa chän lo¹i?",3,1,"",
	"Trïng tu trang bÞ/ratrangbitm1",
	"R· trang bÞ/ratb",
	"KÕt thóc ®èi tho¹i/no")
end

function trungtuz()
Say2("B¹n chän trïng tu trang bÞ",3,1,"",
	"Kim ¤/trungtutbz",
	"B¹ch Hæ/trungtutbz",
	"KÕt thóc ®èi tho¹i/no")
end;

function ratb()
Say2("B¹n chän r· trang bÞ",4,1,"",
	"V©n Léc/ratrangbiz",
	"Th­¬ng Lang/ratrangbiz",
	"HuyÒn Viªn/ratrangbiz",
	"KÕt thóc ®èi tho¹i/no")
end;

function doimnlb()
	Say2("Muèn ®æi LÖnh Bµi Méc Nh©n ng­¬i cÇn ph¶i cã <enter>TiÒn v¹n : 100 v¹n <enter>§iÓm tinh hoa : 100.",2,1,"",
	"Ta muèn ®æi/okdoidth",
	"KÕt thóc ®èi tho¹i/no")
end;

function daitiecbanghoi()
	if(GetMasterName() ~= GetName()) then
	 Talk(1,"","ChØ cã bang chñ míi cã thÓ tham kh¶o tÝnh n¨ng nµy.")
	return end
	Say2("Bang chñ muèn më 1 ®¹i tiÕc bang héi th× cÇn ph¶i cã 100 ®iÓm ho¹t ®éng bang,kh«ng thÓ céng dån 2 bµn tiÖc 1 lóc,bang chñ cã muèn më tiÖc kh«ng?",2,1,"",
	"Ta muèn/daitiec",
	"KÕt thóc ®èi tho¹i/no")
end

function doikcl()
	-- Talk(1,"","TÝnh n¨ng b¶o tr×")
	Talk(1,"okdoikcl","Muèn ®æi LÖnh Bµi Boss Bang Héi ng­¬i cÇn ph¶i cã <enter>§iÓm ho¹t ®éng bang : 2000 <enter>Khiªu chiÕn lÖnh : 35000 <enter>TiÒn v¹n cÇn : 10.000 v¹n l­îng.")
end;

function okdoikcl1()
Talk(1,"","TÝnh n¨ng ®ang trong giai ®o¹n hoµn thiÖn,sÏ ra m¾t trong thêi gian sím nhÊt")
end

function ratrangbiz(sel)
local nSel = sel+1
if(nSel == 1) then
PutItem("B¹n chän r· trang bÞ V©n Léc,sau khi r· nhËn l¹i ®­îc ®é phæ V©n Léc,V©n Léc Th¹ch x3\nPhÝ 1000 v¹n l­îng","y/ratbvanloc","n/no")
elseif(nSel == 2) then
PutItem("B¹n chän r· trang bÞ Th­¬ng Lang,sau khi r· nhËn l¹i ®­îc ®é phæ Th­¬ng Lang,Th­¬ng Lang Th¹ch x3,®é phæ V©n Léc,V©n Léc Th¹ch x3\nPhÝ 1000 v¹n l­îng","y/ratbthuonglang","n/no")
else
PutItem("B¹n chän r· trang bÞ HuyÒn Viªn,sau khi r· nhËn l¹i ®­îc §é Phæ HuyÒn Viªn,HuyÒn Viªn LÖnh x3\nPhÝ 1999 v¹n l­îng","y/ratrangbitm1","n/no")
end
end;
-- if(nSel == 1) then
-- PutItem("B¹n chän r· trang bÞ HuyÒn Viªn,sau khi r· nhËn l¹i ®­îc ®é phæ HuyÒn Viªn: \n1.699 v¹n l­îng","y/ratrangbitm1","n/no")
-- elseif(nSel == 2) then
-- PutItem("B¹n chän r· trang bÞ Tö M·ng,sau khi r· nhËn l¹i ®­îc ®é phæ Tö M·ng: \n1.2000 v¹n l­îng.","y/ratrangbitm1","n/no")
-- else
-- PutItem("B¹n chän r· trang bÞ Kim ¤,sau khi r· nhËn l¹i ®­îc §é Phæ Kim ¤,2 §é Phæ Tö M·ng, 2 Kim ¤ Th¹ch,3 Tö M·ng Th¹ch,1 §é Phæ HuyÒn Viªn: \n1.2000 v¹n l­îng.","y/ratrangbitm1","n/no")
-- end
function ratrangbitm1()
Talk(1,"","TÝnh n¨ng ®ang hoµn thiÖn")
end


function trungtutbz(sel)
local nSel = sel+1
if(nSel == 1) then
PutItem("Ng­¬i cÇn chuÈn bÞ: \n1.1500 v¹n l­îng","y/trungtuko","n/no")
else
PutItem("Ng­¬i cÇn chuÈn bÞ: \n1.3000 v¹n l­îng.","y/trungtubh","n/no")
end
end;

function ngoaiquan2(sel)
local nSel = sel+1
if(nSel == 1) then
PutItem("B¹n chän Ngo¹i Quan Trung CÊp: \n1.2000 v¹n l­îng\n2.300 HuyÒn Ch©n §¬n","y/ngoaiquantc","n/no")
else
PutItem("B¹n chän Ngo¹i Quan Cao CÊp: \n1.5000 v¹n l­îng\n2.900 HuyÒn Ch©n §¬n","y/ngoaiquancc","n/no")
end
end;

function kndh2(sel)
local nSel = sel+1
if(nSel == 1) then
PutItem("B¹n chän S¸ch Kü N¨ng Trung CÊp: \n1.2000 v¹n l­îng\n2.150 HuyÕt Ch©n §¬n\n3.150 B¹ch Ch©n §¬n","y/kndhtc","n/no")
else
PutItem("B¹n chän S¸ch Kü N¨ng Cao CÊp: \n1.5000 v¹n l­îng\n2.450 HuyÕt Ch©n §¬n\n3.450 B¹ch Ch©n §¬n","y/kndhcc","n/no")
end
end;

function XemSoLuongKCL()
	local banghoi = GetTongName()
	local Dat = GetTempInt("KCLBang",GetTongID())
	if Dat == 0 then Talk(1,"","Bang héi<color=yellow> "..banghoi.." <color>ch­a gãp Khiªu ChiÕn LÖnh.") return end
	if GetName() ~= GetMasterName() then Talk(1,"","Chøc vô cña ng­¬i qu¸ thÊp ta kh«ng thÓ tiÕt lé thiªn c¬.") return end
	Talk(1,"","Sè l­îng Khiªu ChiÕn LÖnh cña bang héi<color=yellow> "..banghoi.." <color>hiÖn t¹i lµ: <color=red>"..Dat.." c¸i")
end

function okdoikcl()
if (GetCash() < TIENCANBBH) then Talk(1,"","B¹n cÇn 20.000 v¹n ®Ó cã thÓ ®æi lªnh bµi khiªu chiÕn Boss Bang Héi") return end
if (GetCash() < XUCANBBH) then Talk(1,"","B¹n cÇn 5000 xu ®Ó cã thÓ ®æi lªnh bµi khiªu chiÕn Boss Bang Héi") return end

	local banghoi = GetTongName()
	local bangid = GetTongID()
	local Dat = GetTempInt("KCLBang",bangid)
	local DatHDB = GetTempInt("DiemHDBang",bangid)
	if(GetMasterName() ~= GetName()) then
	 Talk(1,"","ChØ cã bang chñ míi cã thÓ ®æi.")
	 return end
	if(GetTempInt("KCLBang",GetTongID()) < SOLUONGKCLYC) then
	 Talk(1,"","Bang héi cña ng­¬i kh«ng ®ñ "..SOLUONGKCLYC.." Khiªu ChiÕn LÖnh,kh«ng thÓ ®æi.")
	return end	
	if(GetTempInt("DiemHDBang",GetTongID()) < SOLUONGDHDYCB) then
	 Talk(1,"","Bang héi cña ng­¬i kh«ng ®ñ "..SOLUONGDHDYCB.." §iÓm Ho¹t §éng Bang,kh«ng thÓ ®æi.")
	return end	
	
	----Them vao day
	SetTempInt("KCLBang",bangid,Dat-SOLUONGKCLYC);
	SetTempInt("DiemHDBang",bangid,DatHDB-SOLUONGDHDYCB);
	Pay(TIENCANBBH)
	PayCoin(XUCANBBH)
	local TCMLB = ItemSetAdd(0,3,111,0,0,5,0,0);
	AddItemID(TCMLB);	
	Msg2SubWorld("<color=orange>Giang hå loan tin bang chñ <color=yellow>"..GetName().."<color> ®· ®o¹t lÊy ®­îc <color=red>LÖnh Bµi Boss Bang Héi<color> vÒ cho bang <color=yellow>"..banghoi.." ")
	AddNews2("<color=orange>Giang hå loan tin bang chñ <color=yellow>"..GetName().."<color> ®· ®o¹t lÊy ®­îc <color=red>LÖnh Bµi Boss Bang Héi<color> vÒ cho bang <color=yellow>"..banghoi.." ")
	logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tBoss Bang\t\t"..Dat..","..DatHDB.."")
end

function okdoidth()
	local nTaskValue = GetTask(TASK_TINHHOA,1);
	if (GetCash() < TIENVANYCTH) then
	 Talk(1,"","TiÒn v¹n cña ng­¬i kh«ng ®ñ "..TIENVANYCTH..",kh«ng thÓ ®æi.")
	return end
	if(nTaskValue < DTHYC) then
	 Talk(1,"","§iÓm tinh hoa cña ng­¬i kh«ng ®ñ "..DTHYC..",kh«ng thÓ ®æi.")
	return end	
	local MNLB = ItemSetAdd(0,2,151,0,0,5,0,0);
	SetItemDate(MNLB,10080);
	AddItemID(MNLB);	
	----Them vao day
	SetTask(TASK_TINHHOA, SetNumber(GetTask(TASK_TINHHOA),1)-DTHYC);
	Msg2Player("B¹n nhËn ®­îc 1 <color=yellow> LÖnh Bµi Méc Nh©n")
	logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tMoc Nhan\t\t"..nTaskValue.."")
end


function daitiec()
	local bangid = GetTongID()
	local DatHDB = GetTempInt("DiemHDBang",bangid)
	if (GetCash() < TIENVANYCTH) then
	 Talk(1,"","TiÒn v¹n cña ng­¬i kh«ng ®ñ "..TIENVANYCTH..",kh«ng thÓ ®æi.")
	return end
	if(GetTempInt("DiemHDBang",bangid) < HDDTYC) then
	 Talk(1,"","§iÓm ho¹t ®éng bang cña ng­¬i kh«ng ®ñ "..HDDTYC..",kh«ng thÓ ®æi.")
	return end	
	local THETIEC = ItemSetAdd(0,2,150,0,0,5,0,0);
	SetItemDate(THETIEC,1080);
	AddItemID(THETIEC);	
	----Them vao day
	SetTempInt("DiemHDBang",bangid,DatHDB-HDDTYC);
	Msg2Player("B¹n nhËn ®­îc 1 <color=yellow> ThÎ ®Æt tiÖc")
	logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDai Tiec\t\t"..DatHDB.."")
end


function trungtutm()
if (GetCash() < TIENTRUNGTU) then 
Talk(1,"","B¹n kh«ng cã ®ñ 500 v¹n ®Ó trïng tu trang bÞ Tö M·ng") 
return end
 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
 local count = 0;
 for i=0,5 do
  for j=0,3 do
  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
  if (nIndex > 0) then
   count = count + 1;
   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
    = nIndex,kind,genre,detail,parti,level,series,row;
  end
  end
 end
 if(count ~= 1) then
 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
 return end
 if(Rkind ~= 2) then
 Msg2Player("ChØ ®æi ®å Hoµng Kim, kh«ng ®æi ®å th­êng")
 return end
 if (Rrow < 1826 or Rrow > 2055) and (Rrow < 4714 or Rrow > 4733) then
 Msg2Player("ChØ cã thÓ bá trang bÞ tö m·ng vµo ®©y")
 return end
 local HoaSon = GetItemHS(nRealIndex);
 Pay(TIENTRUNGTU)
 Item = ItemSetAdd(Rkind,0,Rrow-2,0,0,0,random(1,10))

 SetItemRich(Item, 1000)
 AddItemID(Item); 
 Msg2Player("B¹n ®· trïng tu trang bÞ Tö M·ng <color=yellow> Thµnh C«ng")
 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tTrung tu Tu Mang\t\t"..Rrow.."")
 RemoveItem(nRealIndex,1);
end

function trungtuko()
if (GetCash() < TIENTRUNGTUKO) then Talk(1,"","B¹n kh«ng cã ®ñ 1500 v¹n ®Ó trïng tu trang bÞ Kim ¤") return end
 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
 local count = 0;
 for i=0,5 do
  for j=0,3 do
  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
  if (nIndex > 0) then
   count = count + 1;
   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
    = nIndex,kind,genre,detail,parti,level,series,row;
  end
  end
 end
 if(count ~= 1) then
 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
 return end
 if(Rkind ~= 2) then
 Msg2Player("ChØ ®æi ®å Hoµng Kim, kh«ng ®æi ®å th­êng")
 return end
 if(Rrow < 2056 or Rrow > 2285) and (Rrow < 4734 or Rrow > 4753) then
 Msg2Player("ChØ cã thÓ bá trang bÞ kim « vµo ®©y")
 return end
 local HoaSon = GetItemHS(nRealIndex);
 Pay(TIENTRUNGTUKO)
 Item = ItemSetAdd(Rkind,0,Rrow-2,0,0,0,random(1,20))
 if (HoaSon > 0) then
 SetItemHS(nNewItem, 1)
 end
 SetItemRich(Item, 3000)
 AddItemID(Item); 
 Msg2Player("B¹n ®· trïng tu trang bÞ Kim ¤ <color=yellow> Thµnh C«ng")
 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tTrung tu Kim O\t\t"..Rrow.."")
 RemoveItem(nRealIndex,1);
end

function trungtubh()
if (GetCash() < TIENTRUNGTUBH) then Talk(1,"","B¹n kh«ng cã ®ñ 3000 v¹n ®Ó trïng tu trang bÞ Kim ¤") return end
 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
 local count = 0;
 for i=0,5 do
  for j=0,3 do
  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
  if (nIndex > 0) then
   count = count + 1;
   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
    = nIndex,kind,genre,detail,parti,level,series,row;
  end
  end
 end
 if(count ~= 1) then
 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
 return end
 if(Rkind ~= 2) then
 Msg2Player("ChØ ®æi ®å Hoµng Kim, kh«ng ®æi ®å th­êng")
 return end
 if(Rrow < 2286 or Rrow > 2515) and (Rrow < 4754 or Rrow > 4773) then
 Msg2Player("ChØ cã thÓ bá trang bÞ b¹ch hæ vµo ®©y")
 return end
 local HoaSon = GetItemHS(nRealIndex);
 Pay(TIENTRUNGTUBH)
 Item = ItemSetAdd(Rkind,0,Rrow-2,0,0,0,random(1,20))
 if (HoaSon > 0) then
 SetItemHS(nNewItem, 1)
 end
 SetItemRich(Item, 5000)
 AddItemID(Item); 
 Msg2SubWorld(""..GetName().." ®· trïng tu trang bÞ B¹ch Hæ <color=yellow> Thµnh C«ng")
 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tTrung tu Bach Ho\t\t"..Rrow.."")
 RemoveItem(nRealIndex,1);
end

function ratbvanloc()
if (GetCash() < 10000000) then 
Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó r· trang bÞ V©n L«c") 
return end
 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
 local count = 0;
 for i=0,5 do
  for j=0,3 do
  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
  if (nIndex > 0) then
   count = count + 1;
   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
    = nIndex,kind,genre,detail,parti,level,series,row;
  end
  end
 end
 if(count ~= 1) then
 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
 return end
 local bLock,nMin = GetItemLock(nRealIndex);
 if(bLock == 2) then
 Msg2Player("VËt phÈm khãa vÜnh viÔn kh«ng thÓ thùc hiÖn.")
 return end
 if(Rkind ~= 2) then
 Msg2Player("ChØ ®æi ®å Hoµng Kim, kh«ng ®æi ®å th­êng")
 return end
 if (Rrow < 1136 or Rrow > 1365) and (Rrow < 4654 or Rrow > 4673) then
 Msg2Player("ChØ cã thÓ bá trang bÞ tö m·ng vµo ®©y")
 return end
 Pay(10000000)
 AddItem(0,3,37,0,0,5,0,0)
 AddItem(0,3,38,0,0,5,3,0)
 Msg2SubWorld(""..GetName().." R· thµnh c«ng b¹n nhËn ®­îc §é Phæ <color=yellow> V©n Léc")
 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tRa Van Loc")
 RemoveItem(nRealIndex,1);
end

function ratbthuonglang()
if (GetCash() < 10000000) then 
Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó r· trang bÞ Th­¬ng Lang") 
return end
 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
 local count = 0;
 for i=0,5 do
  for j=0,3 do
  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
  if (nIndex > 0) then
   count = count + 1;
   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
    = nIndex,kind,genre,detail,parti,level,series,row;
  end
  end
 end
 if(count ~= 1) then
 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
 return end
 local bLock,nMin = GetItemLock(nRealIndex);
 if(bLock == 2) then
 Msg2Player("VËt phÈm khãa vÜnh viÔn kh«ng thÓ thùc hiÖn.")
 return end
 if(Rkind ~= 2) then
 Msg2Player("ChØ ®æi ®å Hoµng Kim, kh«ng ®æi ®å th­êng")
 return end
 if (Rrow < 1366 or Rrow > 1595) and (Rrow < 4674 or Rrow > 4693) then
 Msg2Player("ChØ cã thÓ bá trang bÞ tö m·ng vµo ®©y")
 return end
 Pay(10000000)
 AddItem(0,3,37,0,0,5,0,0)
 AddItem(0,3,39,0,0,5,0,0)
 AddItem(0,3,38,0,0,5,3,0)
 AddItem(0,3,40,0,0,5,3,0)
 Msg2SubWorld(""..GetName().." R· thµnh c«ng b¹n nhËn ®­îc §é Phæ <color=yellow> Th­¬ng Lang")
 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tRa Thuong Lang")
 RemoveItem(nRealIndex,1);
end

function ratrangbihv()
if (GetCash() < TIENTRUNGTU) then 
Talk(1,"","B¹n kh«ng cã ®ñ 699 v¹n ®Ó r· trang bÞ HuyÒn Viªn") 
return end
 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
 local count = 0;
 for i=0,5 do
  for j=0,3 do
  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
  if (nIndex > 0) then
   count = count + 1;
   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
    = nIndex,kind,genre,detail,parti,level,series,row;
  end
  end
 end
 if(count ~= 1) then
 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
 return end
 local bLock,nMin = GetItemLock(nRealIndex);
 if(bLock == 2) then
 Msg2Player("VËt phÈm khãa vÜnh viÔn kh«ng thÓ thùc hiÖn.")
 return end
 if(Rkind ~= 2) then
 Msg2Player("ChØ ®æi ®å Hoµng Kim, kh«ng ®æi ®å th­êng")
 return end
 if (Rrow < 1596 or Rrow > 1825) and (Rrow < 4694 or Rrow > 4713) then
 Msg2Player("ChØ cã thÓ bá trang bÞ tö m·ng vµo ®©y")
 return end
 Pay(TIENTRUNGTU)
 AddItem(0,3,41,0,0,5,0,0)
 Msg2SubWorld(""..GetName().." R· thµnh c«ng b¹n nhËn ®­îc §é Phæ <color=yellow> HuyÒn Viªn")
 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tRa Huyen Vien")
 RemoveItem(nRealIndex,1);
end

function ratrangbiko()
if (GetCash() < TIENTRUNGTUKO) then 
Talk(1,"","B¹n kh«ng cã ®ñ 2000 v¹n ®Ó trïng r· trang bÞ KIm ¤") 
return end
 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
 local count = 0;
 for i=0,5 do
  for j=0,3 do
  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
  if (nIndex > 0) then
   count = count + 1;
   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
    = nIndex,kind,genre,detail,parti,level,series,row;
  end
  end
 end
 if(count ~= 1) then
 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
 return end
 if(Rkind ~= 2) then
 Msg2Player("ChØ ®æi ®å Hoµng Kim, kh«ng ®æi ®å th­êng")
 return end
 if(Rrow < 2056 or Rrow > 2285) and (Rrow < 4734 or Rrow > 4753) then
 Msg2Player("ChØ cã thÓ bá trang bÞ Kim ¤ vµo ®©y")
 return end
 Pay(TIENTRUNGTUKO)
 AddItem(0,3,45,0,0,5,0,0)
 Msg2Player("R· thµnh c«ng b¹n nhËn ®­îc §é Phæ <color=yellow> Kim ¤")
 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tRa Kim O")
 RemoveItem(nRealIndex,1);
end


function ratrangbibh()
if (GetCash() < TIENTRUNGTUKO) then 
Talk(1,"","B¹n kh«ng cã ®ñ 2000 v¹n ®Ó trïng r· trang bÞ B¹ch Hæ") 
return end
 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
 local count = 0;
 for i=0,5 do
  for j=0,3 do
  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
  if (nIndex > 0) then
   count = count + 1;
   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
    = nIndex,kind,genre,detail,parti,level,series,row;
  end
  end
 end
 if(count ~= 1) then
 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
 return end
 if(Rkind ~= 2) then
 Msg2Player("ChØ ®æi ®å Hoµng Kim, kh«ng ®æi ®å th­êng")
 return end
 if(Rrow < 2286 or Rrow > 2515) and (Rrow < 4754 or Rrow > 4773) then
 Msg2Player("ChØ cã thÓ bá trang bÞ B¹ch Hæ vµo ®©y")
 return end
 Pay(TIENTRUNGTUKO)
 AddItem(0,3,47,0,0,5,0,0)
 AddItem(0,3,45,0,0,5,2,0)
 local BHTHACH = ItemSetAdd(0,3,48,0,0,5,2,0);LockItem(BHTHACH);AddItemID(BHTHACH)
 local DPKO = ItemSetAdd(0,3,45,0,0,5,0,0);LockItem(DPKO);AddItemID(DPKO)
 local KOTHACH = ItemSetAdd(0,3,46,0,0,5,4,0);LockItem(KOTHACH);AddItemID(KOTHACH)
  local DPTM = ItemSetAdd(0,3,43,0,0,5,2,0);LockItem(DPTM);AddItemID(DPTM)
 Msg2Player("R· thµnh c«ng b¹n nhËn ®­îc<color=yellow> §é Phæ B¹ch Hæ,3 §é Phæ Kim ¤, 2 B¹ch Hæ Th¹ch,4 Kim ¤ Th¹ch,2 §é Phæ Tö M·ng")
 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tRa BH")
 RemoveItem(nRealIndex,1);
end

function ngoaiquantc()
	if(GetCash() < 20000000) then
		Msg2Player("CÇn 2000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_CD[1]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	if(nMatcount1 ~= 300) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 300 c¸i <color=yellow>"..DETAIL_NAMECD[1].."")
	return end
	Pay(20000000)
	local NQTC = ItemSetAdd(0,2,156,0,0,5,0,0);
	-- SetItemDate(NQTC,1080);
	AddItemID(NQTC);
	--xoa cac nguyen lieu
	for i=1,TabMatC1 do
		RemoveItem(TabMat1[i],1)
	end
	Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_VP[1].."")
	logBangHoi(""..GetName().."\t\t\t"..GetAccount().."\t\t\t"..DETAIL_VP[1].."")
end;

function ngoaiquancc()
	if(GetCash() < 50000000) then
		Msg2Player("CÇn 5000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_CD[1]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	if(nMatcount1 ~= 900) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 900 c¸i <color=yellow>"..DETAIL_NAMECD[1].."")
	return end
	Pay(50000000)
	local NQCC = ItemSetAdd(0,2,157,0,0,5,0,0);
	-- SetItemDate(NQCC,1080);
	AddItemID(NQCC);
	--xoa cac nguyen lieu
	for i=1,TabMatC1 do
		RemoveItem(TabMat1[i],1)
	end
	Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_VP[2].."")
	logBangHoi(""..GetName().."\t\t\t"..GetAccount().."\t\t\t"..DETAIL_VP[2].."")
end;

function kndhtc()
	if(GetCash() < 20000000) then
		Msg2Player("CÇn 2000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMat2 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local TabMatC2 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	local nMatcount2 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_CD[2]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_CD[3]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	if(nMatcount1 ~= 150) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 150 c¸i <color=yellow>"..DETAIL_NAMECD[2].."")
	return end
	if(nMatcount2 ~= 150) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 150 c¸i <color=yellow>"..DETAIL_NAMECD[3].."")
	return end
	Pay(20000000)
	local NQTC = ItemSetAdd(0,2,153,0,0,5,0,0);
	-- SetItemDate(NQTC,1080);
	AddItemID(NQTC);
	--xoa cac nguyen lieu
	for i=1,TabMatC1 do
		RemoveItem(TabMat1[i],1)
	end
	for i=1,TabMatC2 do
		RemoveItem(TabMat2[i],1)
	end
	Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_VP[3].."")
	logBangHoi(""..GetName().."\t\t\t"..GetAccount().."\t\t\t"..DETAIL_VP[3].."")
end;

function kndhcc()
	if(GetCash() < 50000000) then
		Msg2Player("CÇn 5000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMat2 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local TabMatC2 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	local nMatcount2 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_CD[2]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_CD[3]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	if(nMatcount1 ~= 450) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 450 c¸i <color=yellow>"..DETAIL_NAMECD[2].."")
	return end
	if(nMatcount2 ~= 450) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 450 c¸i <color=yellow>"..DETAIL_NAMECD[3].."")
	return end
	Pay(50000000)
	local NQTC = ItemSetAdd(0,2,154,0,0,5,0,0);
	-- SetItemDate(NQTC,1080);
	AddItemID(NQTC);
	--xoa cac nguyen lieu
	for i=1,TabMatC1 do
		RemoveItem(TabMat1[i],1)
	end
	for i=1,TabMatC2 do
		RemoveItem(TabMat2[i],1)
	end
	Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_VP[4].."")
	logBangHoi(""..GetName().."\t\t\t"..GetAccount().."\t\t\t"..DETAIL_VP[4].."")
end;



function logBangHoi(str)
local gm_Log = "dulieu/SuKien/BangHoi.txt"
local fs_log = openfile(gm_Log, "a");
write(fs_log, date("%H:%M:%S_%d-%m-%y").."\t"..str.."\n");
closefile(fs_log);
end

function no()
end