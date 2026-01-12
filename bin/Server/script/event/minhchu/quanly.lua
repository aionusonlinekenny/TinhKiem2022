Include("\\script\\library\\worldlibrary.lua");

DETAIL_MANH = {78,79,80,81,82,96};--vi du id 5 loai material
NAME_MANH = {"Kim","Méc","Thñy","Háa","Thæ "};--vi du ten 5 loai material

DETAIL_NAME = {"§é phæ Thanh C©u",
			"§å phæ V©n Léc",
			"§å phæ Th­¬ng Lang",
			"§å phæ HuyÒn Viªn",
			"§å phæ Tö M·ng",
			"§å phæ Kim ¤",
			"§å phæ B¹ch Hæ",
			"§å phæ XÝch L©n",
			"§å phæ Minh Ph­îng"
			}
DETAIL_DOPHO = {35,37,39,41,43,45,47,49,51}
DETAIL_MANHBITHUAT = 77
DETAIL_BITICH = 238		

DELAIL_2LOAIAN = {97,98}
NAME_2LOAIAN = {"Tinh LuyÖn Hoµng Kim Ên (C­êng hãa)",
				"Tinh LuyÖn Hoµng Kim Ên (Nh­îc hãa)"}
				
DETAIL_MANHTHANTHU = {83,84,85,86,87,91,92,93,94,88,89,90}

DETAIL_BUAGIAMDINH = {95}

NAME_BUAGIAMDINH = {"Bïa Gi¸m §Þnh"}

NAME_THANTHU = {"M¶nh Kim Tinh B¹ch Hæ V­¬ng",
				"M¶nh Háa Tinh Kim Hæ V­¬ng",
				"M¶nh Long Tinh H¾c Hæ V­¬ng",
				"M¶nh H·n HuyÕt Long C©u",
				"M¶nh S­ tö",
				"M¶nh D­¬ng Sa",
				"M¶nh Ngù Phong",
				"M¶nh Truy ®iÖn",
				"M¶nh L­u Tinh",
				"M¶nh L¹c ®µ",
				"M¶nh D­¬ng §µ",
				"M¶nh H­¬u ®èm"}
				
function main(NpcIndex)
Say("Ng­êi cÇn ta gióp g× ?",3,
	"N©ng cÊp ®é phæ/nangcapdopho",
	"N©ng cÊp bÝ tÞch/nangcapbitich",
	-- "Gi¸m ®Þnh thó c­ìi/giamdinhthucuoi",
	-- "N©ng cÊp thÇn thó Hoµng Kim/thanthuhoangkim",
	-- "Tinh luyÖn Ên Ngò Hµnh/tinhluyenan",
	-- "LuyÖn c¸c lo¹i Ên/cacloaian",
	"KÕt thóc ®èi tho¹i/no")
end
function thanthuhoangkim()
Say("Ng­êi cÇn ta gióp g×?",3,
	"Hót ch©n khÝ thÇn thó/luyenthugiamdinh",
	"LuyÖn thµnh thÇn thó Hoµng Kim/luyenthanhthk",
	"KÕt thóc ®èi tho¹i/no")
end

function cacloaian()
Say("Ng­¬i cã muèn sö dông Tói May M¾n khi luyÖn ¢n kh«ng?\nTói may m¾n gióp ng­¬i cã thÓ b¶o toµn nguyªn liÖu khi ghÐp thÊt b¹i.",3,
	"Ta rÊt cÇn/tacan",
	"Ta kh«ng cÇn/takhongcan",
	"KÕt thóc ®èi tho¹i/no")
end
function giamdinhthucuoi()
Say("H·y chän lo¹i thó c­ìi cÇn gi¸m ®Þnh.",13,
	"Kim Tinh B¹ch Hæ V­¬ng/giamdinh",
	"Háa Tinh Kim Hæ V­¬ng/giamdinh",
	"Long Tinh H¾c Hæ V­¬ng/giamdinh",
	"H·n HuyÕt Long C©u/giamdinh",
	"S­ tö/giamdinh",
	"D­¬ng Sa/giamdinh",
	"Ngù Phong/giamdinh",
	"Truy ®iÖn/giamdinh",
	"L­u Tinh/giamdinh",
	"L¹c ®µ/giamdinh",
	"D­¬ng §µ/giamdinh",
	"H­¬u ®èm/giamdinh",
	-- "Më trang 2/trang2",
	"KÕt Thóc §èi Tho¹i/no")
end
function trang2()
Say("H·y chän lo¹i thó c­ìi cÇn gi¸m ®Þnh.",5,
	"D­¬ng Sa/giamdinh2",
	"Ngù Phong/giamdinh2",
	"Truy ®iÖn/giamdinh2",
	"L­u Tinh/giamdinh2",
	"KÕt Thóc §èi Tho¹i/no")
end


function tacan()
Say("H·y chän lo¹i Ên ng­¬i cÇn",4,
	"LuyÖn Ên c­êng hãa/achtmm",
	"LuyÖn Ên nh­îc hãa/anhtmm",
	"LuyÖn ngò hµnh Ên/nhatmm",
	"KÕt thóc ®èi tho¹i/no")
end

function takhongcan()
Say("H·y chän lo¹i Ên ng­¬i cÇn",4,
	"LuyÖn Ên c­êng hãa/ach",
	"LuyÖn Ên nh­îc hãa/anh",
	"LuyÖn ngò hµnh Ên/nha",
	"KÕt thóc ®èi tho¹i/no")
end
function tinhluyenan()
Say("H·y chän lo¹i Ên ng­¬i cÇn",3,
	"Tinh luyÖn Ên c­êng hãa/tla",
	"Tinh luyÖn Ên nh­îc hãa/tla",
	"KÕt thóc ®èi tho¹i/no")
end
function tla(sel)
local nSel = sel+1
if(nSel == 1) then
PutItem("Tinh luyÖn Ên C­êng Hãa cÇn\n1.Ên C­êng Hãa(CÊp 5 trë lªn)\n2.2500 v¹n","y/tlach","n/no")
else
PutItem("Tinh luyÖn Ên Nh­îc Hãa cÇn\n1.Ên Nh­îc Hãa(CÊp 5 trë lªn)\n2.2500 v¹n","y/tlanh","n/no")
end
end;



function achtmm()
PutItem("LuyÖn ¢n C­êng Háa cÇn\n1.Ngò S¾c Long Ch©u(Kim,Méc,Thñy,Háa,Thæ)x20\n2.1000 v¹n l­îng\n3.Tui may m¾n: x1\nTØ lÖ thµnh c«ng: 50%\nH¹n sö dông : vÜnh viÔn","y/doianchtmm","n/no")
end
function anhtmm()
PutItem("LuyÖn ¢n C­êng Háa cÇn\n1.Ngò S¾c Long Ch©u(Kim,Méc,Thñy,Háa,Thæ)x20\n2.1000 v¹n l­îng\n3.Tui may m¾n: x1\nTØ lÖ thµnh c«ng: 50%\nH¹n sö dông : vÜnh viÔn","y/doiannhtmm","n/no")
end
function nhatmm()
PutItem("LuyÖn ngò hµnh Ên cÇn\n1.Tinh LuyÖn ¢n C­êng Hãa x2\n2.Tinh LuyÖn ¢n Nh­îc Hãa x2\n3.5000 v¹n\n4.Tui may m¾n: x1\nTØ lÖ thµnh c«ng 50%\nH¹n sö dông : vÜnh viÔn","y/doianhatmm","n/no")
end

function luyenthugiamdinh()
Talk(1,"","TÝnh n¨ng ®ang ®­îc hoµn thiÖn")
-- PutItem("Muèn hót ch©n khÝ thÇn thó ®Ó ®­îc Hoµng Kim ThÇn Long §¬n cÇn \n1.ThÇn thó ®· gi¸m ®Þnh x1\n2.Hoµng Kim ThuËt Long §¬n x1\n3.2000 v¹n","y/hoantatluyen","n/no")
end

function luyenthanhthk()
Talk(1,"","TÝnh n¨ng ®ang ®­îc hoµn thiÖn")
-- PutItem("LuyÖn thÇn thó Hoµng Kim cÇn\n1.Hoµng Kim ThÇn Long §¬n x1\n2.2000 v¹n","y/hoantatluyen","n/no")
end

function ach()
PutItem("LuyÖn ¢n C­êng Háa cÇn\n1.Ngò S¾c Long Ch©u(Kim,Méc,Thñy,Háa,Thæ)x20\n2.1000 v¹n l­îng\nTØ lÖ thµnh c«ng: 50%\nThÊt b¹i tiªu hao: 50% nguyªn liÖu\nH¹n sö dông : vÜnh viÔn","y/doianch","n/no")
end
function anh()
PutItem("LuyÖn ¢n C­êng Háa cÇn\n1.Ngò S¾c Long Ch©u(Kim,Méc,Thñy,Háa,Thæ)x20\n2.1000 v¹n l­îng\nTØ lÖ thµnh c«ng: 50%\nThÊt b¹i tiªu hao: 50% nguyªn liÖu\nH¹n sö dông : vÜnh viÔn","y/doiannh","n/no")
end
function nha()
PutItem("LuyÖn ngò hµnh Ên cÇn\n1.Tinh LuyÖn ¢n C­êng Hãa x2\n2.Tinh LuyÖn ¢n Nh­îc Hãa x2\n3.5000 v¹n\nTØ lÖ thµnh c«ng 50%\nThÊt b¹i tiªu hao: 50% nguyªn liÖu\nH¹n sö dông : vÜnh viÔn","y/doianha","n/no")
end

function nangcapbitich()
PutItem("N©ng cÊp BÝ TÞch L¨ng Ba Vi Bé cÇn:\n1.20 m¶nh BÝ ThuËt\n2.50 Phóc Duyªn <color=green>§¹i<color> \n3.5000 v¹n l­îng.\nTØ lÖ thµnh c«ng: 100%","y/bithuat","n/no")
end

function nangcapdopho()
Say("Ng­êi cÇn ta gióp g× ?",11,
	"N©ng cÊp ®é phæ V©n Léc/nangcapdophosel",
	"N©ng cÊp ®é phæ Th­¬ng Lang/nangcapdophosel",
	"N©ng cÊp ®é phæ HuyÒn Viªn/nangcapdophosel",
	"N©ng cÊp ®é phæ Tö M·ng/nangcapdophosel",
	-- "N©ng cÊp ®é phæ Kim ¤/nangcapdophosel",
	-- "N©ng cÊp ®é phæ B¹ch Hæ/nangcapdophosel",
	-- "N©ng cÊp ®é phæ XÝch L©n/nangcapdophosel",
	-- "N©ng cÊp ®é phæ Minh Ph­îng/nangcapdophosel",
	-- "N©ng cÊp ®é phæ §»ng Long/nangcapdophosel",
	-- "N©ng cÊp ®é phæ S­¬ng Tinh/nangcapdophosel",
	"KÕt thóc ®èi tho¹i/no")
end

function giamdinh(sel)
local nSel = sel+1
if(nSel == 1) then
PutItem("N©ng cÊp Kim Tinh B¹ch Hæ V­¬ng cÇn\n1.10 m¶nh Kim Tinh B¹ch Hæ V­¬ng\n\n2.Bïa Gi¸m §Þnh: x1\n3.500 v¹n\nH¹n sö dông: 15 ngµy","y/kimtinhbachhovuong","n/no")
elseif(nSel == 2) then
PutItem("N©ng cÊp Háa Tinh Kim Hæ V­¬ng cÇn\n1.10 m¶nh Háa Tinh Kim Hæ V­¬ng\n\n2.Bïa Gi¸m §Þnh: x1\n3.500 v¹n\nH¹n sö dông: 15 ngµy","y/hoatinhkimhovuong","n/no")
elseif(nSel == 3) then
PutItem("N©ng cÊp Long Tinh H¾c Hæ V­¬ng cÇn\n1.10 m¶nh Long Tinh H¾c Hæ V­¬ng\n\n2.Bïa Gi¸m §Þnh: x1\n3.500 v¹n\nH¹n sö dông: 15 ngµy","y/longtinhhachovuong","n/no")
elseif(nSel == 4) then
PutItem("N©ng cÊp H·n HuyÕt Long C©u cÇn\n1.10 m¶nh H·n HuyÕt Long C©u\n\n2.Bïa Gi¸m §Þnh: x1\n3.500 v¹n\nH¹n sö dông: 15 ngµy","y/hanhuyetlongcau","n/no")
elseif(nSel == 5) then
PutItem("N©ng cÊp S­ Tö cÇn\n1.10 m¶nh S­ Tö\n\n2.Bïa Gi¸m §Þnh: x1\n3. 500 v¹n\nH¹n sö dông: 15 ngµy","y/sututhuong","n/no")
elseif(nSel == 6) then
PutItem("N©ng cÊp D­¬ng Sa cÇn\n1.10 m¶nh D­¬ng Sa\n\n2.Bïa Gi¸m §Þnh: x1\n3. 500 v¹n\nH¹n sö dông: 15 ngµy","y/duongsa","n/no")
elseif(nSel == 7) then
PutItem("N©ng cÊp Ngù Phong cÇn\n1.10 m¶nh Ngù Phong\n\n2.Bïa Gi¸m §Þnh: x1\n3. 500 v¹n\nH¹n sö dông: 15 ngµy","y/nguphong","n/no")
elseif(nSel == 8) then
PutItem("N©ng cÊp Truy §iÖn cÇn\n1.10 m¶nh Truy §iÖn\n\n2.Bïa Gi¸m §Þnh: x1\n3. 500 v¹n\nH¹n sö dông: 15 ngµy","y/truydien","n/no")
elseif(nSel == 9) then
PutItem("N©ng cÊp L­u Tinh cÇn\n1.10 m¶nh L­u Tinh\n\n2.Bïa Gi¸m §Þnh: x1\n3. 500 v¹n\nH¹n sö dông: 15 ngµy","y/luutinh","n/no")
elseif(nSel == 10) then
PutItem("N©ng cÊp L­u Tinh cÇn\n1.10 m¶nh L¹c §µ\n\n2.Bïa Gi¸m §Þnh: x1\n3. 500 v¹n\nH¹n sö dông: 15 ngµy","y/lacda","n/no")
elseif(nSel == 11) then
PutItem("N©ng cÊp L­u Tinh cÇn\n1.10 m¶nh D­¬ng §µ\n\n2.Bïa Gi¸m §Þnh: x1\n3. 500 v¹n\nH¹n sö dông: 15 ngµy","y/duongda","n/no")
elseif(nSel == 12) then
PutItem("N©ng cÊp L­u Tinh cÇn\n1.10 m¶nh H­¬u §èm\n\n2.Bïa Gi¸m §Þnh: x1\n3. 500 v¹n\nH¹n sö dông: 15 ngµy","y/huudom","n/no")
else
end
end;
function giamdinh2(sel)
local nSel = sel+1
if(nSel == 1) then
Talk(1,"","TÝnh n¨ng nµy ch­a ®­îc më,quay l¹i sau")
elseif(nSel == 2) then
Talk(1,"","TÝnh n¨ng nµy ch­a ®­îc më,quay l¹i sau")
elseif(nSel == 3) then
Talk(1,"","TÝnh n¨ng nµy ch­a ®­îc më,quay l¹i sau")
else
Talk(1,"","TÝnh n¨ng nµy ch­a ®­îc më,quay l¹i sau")
end
end;

function nangcapdophosel(sel)
local nSel = sel+1
if(nSel == 1) then
PutItem("N©ng cÊp ®å phæ V©n Léc cÇn\n1.4 ®é phæ Thanh C©u\n2.1 Phóc Duyªn <color=green>§¹i<color> \n3.100 v¹n l­îng\nTØ lÖ thµnh c«ng : 20%\nThÊt b¹i tiªu hao : 1 §PTC","y/nangcapdophovl","n/no")
elseif(nSel == 2) then
PutItem("N©ng cÊp ®å phæ Th­¬ng Lang cÇn\n1.4 ®é phæ V©n Léc\n2.3 Phóc Duyªn <color=green>§¹i<color> \n3.160 v¹n l­îng\nTØ lÖ thµnh c«ng : 30%\nThÊt b¹i tiªu hao : 1 §PVL","y/nangcapdophotl","n/no")
elseif(nSel == 3) then
PutItem("N©ng cÊp ®å phæ HuyÒn Viªn cÇn\n1.4 ®é phæ Th­¬ng Lang\n2.5 Phóc Duyªn <color=green>§¹i<color> \n3.180 v¹n l­îng\nTØ lÖ thµnh c«ng : 30%\nThÊt b¹i tiªu hao : 1 §PTL","y/nangcapdophohv","n/no")
elseif(nSel == 4) then
PutItem("N©ng cÊp ®å phæ Tö M·ng cÇn\n1.4 ®é phæ HuyÒn Viªn\n2.7 Phóc Duyªn <color=green>§¹i<color> \n3.200 v¹n l­îng\nTØ lÖ thµnh c«ng : 30%\nThÊt b¹i tiªu hao : 1 §PHV","y/nangcapdophotm","n/no")
-- elseif(nSel == 5) then
-- PutItem("N©ng cÊp ®å phæ Kim ¤ cÇn\n1.4 ®é phæ Tö M·ng\n2.300 v¹n l­îng\nTØ lÖ thµnh c«ng : 30%\nThÊt b¹i tiªu hao : 1 §PTM","y/nangcapdophoko","n/no")
-- elseif(nSel == 6) then
-- PutItem("N©ng cÊp ®å phæ B¹ch Hæ cÇn\n1.4 ®é phæ §é Phæ Kim ¤\n2.600 v¹n l­îng\nTØ lÖ thµnh c«ng : 33,3%\nThÊt b¹i tiªu hao : 2 DPKO","y/nangcapdophobh","n/no")
-- elseif(nSel == 7) then
-- PutItem("N©ng cÊp ®å phæ XÝch L©n cÇn\n1.4 ®é phæ §é Phæ B¹ch Hæ\n2.1500 v¹n l­îng\nTØ lÖ thµnh c«ng : 33,3%\nThÊt b¹i tiªu hao : 2 DPBH","y/nangcapdophoxl","n/no")
-- elseif(nSel == 7) then
-- Talk(1,"","TÝnh n¨ng nµy ch­a ®­îc më,quay l¹i sau")
-- elseif(nSel == 8) then
-- Talk(1,"","TÝnh n¨ng nµy ch­a ®­îc më,quay l¹i sau")
-- elseif(nSel == 9) then
-- Talk(1,"","TÝnh n¨ng nµy ch­a ®­îc më,quay l¹i sau")
-- elseif(nSel == 10) then
-- Talk(1,"","TÝnh n¨ng nµy ch­a ®­îc më,quay l¹i sau")
end
end;

function nangcapdophovl()
	if(GetCash() < 1000000) then
		Msg2Player("CÇn 100 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat,TabMat2 = {},{};--mang~ nguyen lieu
	local TabMatC,TabMatC2 = 0,0;--so luong id cua mang~
	local nMatcount,nMatcount2 = 0,0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_DOPHO[1]) then	--do pho
				TabMat[TabMatC+1] = nIndex;
				TabMatC = TabMatC+1;
				nMatcount = nMatcount + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 2 and detail == 2) then	--phuc duyen dai
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount ~= 4) then	Talk(1,"","CÇn bá vµo 4 §å phæ <color=yellow>"..DETAIL_NAME[1].."") return end
	if(nMatcount2 ~= 1) then Talk(1,"","CÇn bá vµo 1 <color=yellow>Phóc Duyªn §¹i") return end
	Pay(1000000)
	if(random(100) > 80) then	--nang cap thanh cong
		AddItem(0,3,DETAIL_DOPHO[2],0,0,5,0,0)	--add 1 cai do pho moi
		--xoa cac nguyen lieu
		for i=1,TabMatC do RemoveItem(TabMat[i],1) end
		for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_NAME[2].."")
		logSuKien("NpcMinhChu","DO PHO VL - "..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong]")
	else	--nang cap that bai, xoa' 1 cai
		for i=1,TabMatC do RemoveItem(TabMat[i],1) end
		for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
		AddItem(0,3,DETAIL_DOPHO[1],0,0,5,3,0)	--add 1 cai do pho moi
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 1 <color=yellow>"..DETAIL_NAME[1].."")
		logSuKien("NpcMinhChu","DO PHO VL - "..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai]")
	end
end;

function nangcapdophotl()
	if(GetCash() < 1600000) then
		Msg2Player("CÇn 160 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat,TabMat2 = {},{};--mang~ nguyen lieu
	local TabMatC,TabMatC2 = 0,0;--so luong id cua mang~
	local nMatcount,nMatcount2 = 0,0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_DOPHO[2]) then	--do pho
				TabMat[TabMatC+1] = nIndex;
				TabMatC = TabMatC+1;
				nMatcount = nMatcount + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 2 and detail == 2) then	--phuc duyen dai
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount ~= 4) then	Talk(1,"","CÇn bá vµo 4 §å phæ <color=yellow>"..DETAIL_NAME[2].."") return end
	if(nMatcount2 ~= 3) then Talk(1,"","CÇn bá vµo 3 <color=yellow>Phóc Duyªn §¹i") return end
	Pay(1600000)
	if(random(100) > 70) then	--nang cap thanh cong
		AddItem(0,3,DETAIL_DOPHO[3],0,0,5,0,0)	--add 1 cai do pho moi
		--xoa cac nguyen lieu
		for i=1,TabMatC do RemoveItem(TabMat[i],1) end
		for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_NAME[3].."")
		logSuKien("NpcMinhChu","DO PHO TL - "..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong]")
	else	--nang cap that bai, xoa' 1 cai
		for i=1,TabMatC do RemoveItem(TabMat[i],1) end
		for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
		AddItem(0,3,DETAIL_DOPHO[2],0,0,5,3,0)	--add 1 cai do pho moi
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 1 <color=yellow>"..DETAIL_NAME[2].."")
		logSuKien("NpcMinhChu","DO PHO TL - "..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai]")
	end
end;

function nangcapdophohv()
	if(GetCash() < 1800000) then
		Msg2Player("CÇn 180 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat,TabMat2 = {},{};--mang~ nguyen lieu
	local TabMatC,TabMatC2 = 0,0;--so luong id cua mang~
	local nMatcount,nMatcount2 = 0,0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_DOPHO[3]) then	--do pho
				TabMat[TabMatC+1] = nIndex;
				TabMatC = TabMatC+1;
				nMatcount = nMatcount + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 2 and detail == 2) then	--phuc duyen dai
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount ~= 4) then	Talk(1,"","CÇn bá vµo 4 §å phæ <color=yellow>"..DETAIL_NAME[3].."") return end
	if(nMatcount2 ~= 5) then Talk(1,"","CÇn bá vµo 5 <color=yellow>Phóc Duyªn §¹i") return end
	Pay(1800000)
	if(random(100) > 70) then	--nang cap thanh cong
		AddItem(0,3,DETAIL_DOPHO[4],0,0,5,0,0)	--add 1 cai do pho moi
		--xoa cac nguyen lieu
		for i=1,TabMatC do RemoveItem(TabMat[i],1) end
		for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_NAME[4].."")
		logSuKien("NpcMinhChu","DO PHO HV - "..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong]")
	else	--nang cap that bai, xoa' 1 cai
		for i=1,TabMatC do RemoveItem(TabMat[i],1) end
		for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
		AddItem(0,3,DETAIL_DOPHO[3],0,0,5,3,0)	--add 1 cai do pho moi
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 1 <color=yellow>"..DETAIL_NAME[3].."")
		logSuKien("NpcMinhChu","DO PHO HV - "..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai]")
	end
end;

function nangcapdophotm()
	if(GetCash() < 2000000) then
		Msg2Player("CÇn 200 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat,TabMat2 = {},{};--mang~ nguyen lieu
	local TabMatC,TabMatC2 = 0,0;--so luong id cua mang~
	local nMatcount,nMatcount2 = 0,0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_DOPHO[4]) then	--do pho
				TabMat[TabMatC+1] = nIndex;
				TabMatC = TabMatC+1;
				nMatcount = nMatcount + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 2 and detail == 2) then	--phuc duyen dai
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount ~= 4) then	Talk(1,"","CÇn bá vµo 4 §å phæ <color=yellow>"..DETAIL_NAME[4].."") return end
	if(nMatcount2 ~= 7) then Talk(1,"","CÇn bá vµo 7 <color=yellow>Phóc Duyªn §¹i") return end
	Pay(2000000)
	if(random(100) > 70) then	--nang cap thanh cong
		AddItem(0,3,DETAIL_DOPHO[5],0,0,5,0,0)	--add 1 cai do pho moi
		--xoa cac nguyen lieu
		for i=1,TabMatC do RemoveItem(TabMat[i],1) end
		for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_NAME[5].."")
		logSuKien("NpcMinhChu","DO PHO TM - "..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong]")
	else	--nang cap that bai, xoa' 1 cai
		for i=1,TabMatC do RemoveItem(TabMat[i],1) end
		for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
		AddItem(0,3,DETAIL_DOPHO[4],0,0,5,3,0)	--add 1 cai do pho moi
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 1 <color=yellow>"..DETAIL_NAME[4].."")
		logSuKien("NpcMinhChu","DO PHO TM - "..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai]")
	end
end;

function nangcapdophoko()
	if(GetCash() < 3000000) then
		Msg2Player("CÇn 300 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat = {};--mang~ nguyen lieu
	local TabMatC = 0;--so luong id cua mang~
	local nMatcount = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_DOPHO[5]) then	--do pho
				TabMat[TabMatC+1] = nIndex;
				TabMatC = TabMatC+1;
				nMatcount = nMatcount + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount ~= 4) then	--khong dung so luong
		Msg2Player("CÇn bá ®óng 4 c¸i <color=yellow>"..DETAIL_NAME[5].." ")
	return end
	Pay(3000000)
	if(random(3) == 1) then	--nang cap thanh cong
		AddItem(0,3,DETAIL_DOPHO[6],0,0,5,0,0)	--add 1 cai do pho moi
		--xoa cac nguyen lieu
		for i=1,TabMatC do
			RemoveItem(TabMat[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_NAME[6].." .")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\tDoi thanh cong DP Kim O")
	else	--nang cap that bai, xoa' 1 cai
		for i=1,TabMatC do
			RemoveItem(TabMat[i],1)
		end
		AddItem(0,3,DETAIL_DOPHO[5],0,0,5,2,0) --add 3 cai do pho moi
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 2 <color=yellow>"..DETAIL_NAME[5].." ")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] DP Kim O")
	end
end;


function nangcapdophobh()
	if(GetCash() < 6000000) then
		Msg2Player("CÇn 600 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat = {};--mang~ nguyen lieu
	local TabMatC = 0;--so luong id cua mang~
	local nMatcount = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_DOPHO[6]) then	--do pho
				TabMat[TabMatC+1] = nIndex;
				TabMatC = TabMatC+1;
				nMatcount = nMatcount + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount ~= 4) then	--khong dung so luong
		Msg2Player("CÇn bá ®óng 4 c¸i <color=yellow>"..DETAIL_NAME[6].." ")
	return end
	Pay(6000000)
	if(random(3) == 2) then	--nang cap thanh cong
		AddItem(0,3,DETAIL_DOPHO[7],0,0,5,0,0)	--add 1 cai do pho moi
		--xoa cac nguyen lieu
		for i=1,TabMatC do
			RemoveItem(TabMat[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_NAME[7].." .")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\tDoi thanh cong DP Bach Ho")
	else	--nang cap that bai, xoa' 1 cai
		for i=1,TabMatC do
			RemoveItem(TabMat[i],1)
		end
		AddItem(0,3,DETAIL_DOPHO[6],0,0,5,2,0) --add 3 cai do pho moi
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 2 <color=yellow>"..DETAIL_NAME[6].." ")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] DP Bach Ho")
	end
end;

function nangcapdophoxl()
	if(GetCash() < 15000000) then
		Msg2Player("CÇn 1500 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat = {};--mang~ nguyen lieu
	local TabMatC = 0;--so luong id cua mang~
	local nMatcount = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_DOPHO[7]) then	--do pho
				TabMat[TabMatC+1] = nIndex;
				TabMatC = TabMatC+1;
				nMatcount = nMatcount + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount ~= 4) then	--khong dung so luong
		Msg2Player("CÇn bá ®óng 4 c¸i <color=yellow>"..DETAIL_NAME[7].." ")
	return end
	Pay(15000000)
	if(random(3) == 2) then	--nang cap thanh cong
		AddItem(0,3,DETAIL_DOPHO[8],0,0,5,0,0)	--add 1 cai do pho moi
		--xoa cac nguyen lieu
		for i=1,TabMatC do
			RemoveItem(TabMat[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..DETAIL_NAME[8].." .")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\tDoi thanh cong DP Xich Lan")
	else	--nang cap that bai, xoa' 1 cai
		for i=1,TabMatC do
			RemoveItem(TabMat[i],1)
		end
		AddItem(0,3,DETAIL_DOPHO[7],0,0,5,2,0) --add 3 cai do pho moi
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 2 <color=yellow>"..DETAIL_NAME[8].." ")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] DP Xich Lan")
	end
end;

function bithuat()
	if(GetCash() < 50000000) then Msg2Player("CÇn 5000 v¹n l­îng ®Ó n©ng cÊp.") return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat,TabMat2 = {},{};--mang~ nguyen lieu
	local TabMatC,TabMatC2 = 0,0;--so luong id cua mang~
	local nMatcount,nMatcount2 = 0,0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHBITHUAT) then	--manh bi thuat
				TabMat[TabMatC+1] = nIndex;
				TabMatC = TabMatC+1;
				nMatcount = nMatcount + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 2 and detail == 2) then	--phuc duyen dai
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount ~= 20) then Talk(1,"","CÇn bá vµo 20 <color=yellow>M¶nh bÝ thuËt Hoa S¬n") return end
	if(nMatcount2 ~= 50) then Talk(1,"","CÇn bá vµo 50 <color=yellow>Phóc Duyªn §¹i") return end
	Pay(50000000)
	AddItem(0,5,DETAIL_BITICH,0,0,5,0,0)	--add bi tich
	--xoa cac nguyen lieu
	for i=1,TabMatC do RemoveItem(TabMat[i],1) end
	for i=1,TabMatC2 do RemoveItem(TabMat2[i],1) end
	Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>BÝ TÞch L¨ng Ba Vi Bé")
	logSuKien("NpcMinhChu-BiTich","DOI BI TICH - "..GetName().."\t\t\t"..GetAccount().."")
end;


function doianch()
	if(GetCash() < 10000000) then
		Msg2Player("CÇn 1000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMat2 = {};--mang~ nguyen lieu
	local TabMat3 = {};--mang~ nguyen lieu
	local TabMat4 = {};--mang~ nguyen lieu
	local TabMat5 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local TabMatC2 = 0;--so luong id cua mang~
	local TabMatC3 = 0;--so luong id cua mang~
	local TabMatC4 = 0;--so luong id cua mang~
	local TabMatC5 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	local nMatcount2 = 0;--so luong xep chong`
	local nMatcount3 = 0;--so luong xep chong`
	local nMatcount4 = 0;--so luong xep chong`
	local nMatcount5 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_MANH[1]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[2]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[3]) then	--manh 1
				TabMat3[TabMatC3+1] = nIndex;
				TabMatC3 = TabMatC3+1;
				nMatcount3 = nMatcount3 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[4]) then	--manh 1
				TabMat4[TabMatC4+1] = nIndex;
				TabMatC4 = TabMatC4+1;
				nMatcount4 = nMatcount4 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[5]) then	--manh 1
				TabMat5[TabMatC5+1] = nIndex;
				TabMatC5 = TabMatC5+1;
				nMatcount5 = nMatcount5 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[1].."")
	return end
	if(nMatcount2 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[2].."")
	return end
	if(nMatcount3 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[3].."")
	return end
	if(nMatcount4 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[4].."")
	return end
	if(nMatcount5 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[5].."")
	return end
	Pay(10000000)
	if(random(10) < 4) then	--nang cap thanh cong
		local id= ItemSetAdd(2,0,3206-2,0,0,0,0)	--add 1 cai an moi id vi du 9999
		AddItemID(id)
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		for i=1,TabMatC3 do
			RemoveItem(TabMat3[i],1)
		end
		for i=1,TabMatC4 do
			RemoveItem(TabMat4[i],1)
		end
		for i=1,TabMatC5 do
			RemoveItem(TabMat5[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>Ên C­êng Hãa")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng Ên <color=yellow>Ên C­êng Hãa")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] ACH")
	else	--nang cap that bai, xoa' 1 nua~ so luong
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		for i=1,TabMatC3 do
			RemoveItem(TabMat3[i],1)
		end
		for i=1,TabMatC4 do
			RemoveItem(TabMat4[i],1)
		end
		for i=1,TabMatC5 do
			RemoveItem(TabMat5[i],1)
		end
		--add lai 1 nua~ so luong
		for i=1,10 do
		AddItem(0,3,DETAIL_MANH[i],0,0,5,5,0)
		end
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 50% sè nguyªn liÖu")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] ACH")
	end
end;
function doiannh()
	if(GetCash() < 10000000) then
		Msg2Player("CÇn 1000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMat2 = {};--mang~ nguyen lieu
	local TabMat3 = {};--mang~ nguyen lieu
	local TabMat4 = {};--mang~ nguyen lieu
	local TabMat5 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local TabMatC2 = 0;--so luong id cua mang~
	local TabMatC3 = 0;--so luong id cua mang~
	local TabMatC4 = 0;--so luong id cua mang~
	local TabMatC5 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	local nMatcount2 = 0;--so luong xep chong`
	local nMatcount3 = 0;--so luong xep chong`
	local nMatcount4 = 0;--so luong xep chong`
	local nMatcount5 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_MANH[1]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[2]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[3]) then	--manh 1
				TabMat3[TabMatC3+1] = nIndex;
				TabMatC3 = TabMatC3+1;
				nMatcount3 = nMatcount3 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[4]) then	--manh 1
				TabMat4[TabMatC4+1] = nIndex;
				TabMatC4 = TabMatC4+1;
				nMatcount4 = nMatcount4 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[5]) then	--manh 1
				TabMat5[TabMatC5+1] = nIndex;
				TabMatC5 = TabMatC5+1;
				nMatcount5 = nMatcount5 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[1].."")
	return end
	if(nMatcount2 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[2].."")
	return end
	if(nMatcount3 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[3].."")
	return end
	if(nMatcount4 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[4].."")
	return end
	if(nMatcount5 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[5].."")
	return end
	Pay(10000000)
	if(random(10) < 4) then	--nang cap thanh cong
		local id= ItemSetAdd(2,0,3216-2,0,0,0,0)	--add 1 cai an moi id vi du 9999
		AddItemID(id)
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		for i=1,TabMatC3 do
			RemoveItem(TabMat3[i],1)
		end
		for i=1,TabMatC4 do
			RemoveItem(TabMat4[i],1)
		end
		for i=1,TabMatC5 do
			RemoveItem(TabMat5[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>Ên Nh­îc Hãa")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng Ên <color=yellow>Ên Nh­îc Hãa")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] ANH")
	else	--nang cap that bai, xoa' 1 nua~ so luong
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		for i=1,TabMatC3 do
			RemoveItem(TabMat3[i],1)
		end
		for i=1,TabMatC4 do
			RemoveItem(TabMat4[i],1)
		end
		for i=1,TabMatC5 do
			RemoveItem(TabMat5[i],1)
		end
		--add lai 1 nua~ so luong
		for i=1,10 do
		AddItem(0,3,DETAIL_MANH[i],0,0,5,5,0)
		end
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 50% sè nguyªn liÖu")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] ANH")
	end
end;
function doianha()
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
				if(kind == 0 and genre == 3 and detail == DELAIL_2LOAIAN[1]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DELAIL_2LOAIAN[2]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 2) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 2 c¸i <color=yellow>"..NAME_2LOAIAN[1].."")
	return end
	if(nMatcount2 ~= 2) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 2 c¸i <color=yellow>"..NAME_2LOAIAN[2].."")
	return end
	Pay(50000000)
	if(random(10) < 5) then	--nang cap thanh cong
		local id= ItemSetAdd(2,0,3226-2,0,0,0,0)	--add 1 cai an moi id vi du 9999
		AddItemID(id)
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>Ngò Hµnh Ên")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng Ên <color=yellow>Ngò Hµnh Ên")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] NHA")
	else	--nang cap that bai, xoa' 1 nua~ so luong
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		--add lai 1 nua~ so luong
		AddItem(0,3,DELAIL_2LOAIAN[1],0,0,5,5,0)
		AddItem(0,3,DELAIL_2LOAIAN[2],0,0,5,5,0)
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i mÊt 50% sè nguyªn liÖu")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] NHA")
	end
end;
function doianchtmm()
	if(GetCash() < 10000000) then
		Msg2Player("CÇn 1000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMat2 = {};--mang~ nguyen lieu
	local TabMat3 = {};--mang~ nguyen lieu
	local TabMat4 = {};--mang~ nguyen lieu
	local TabMat5 = {};--mang~ nguyen lieu
	local TabMat6 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local TabMatC2 = 0;--so luong id cua mang~
	local TabMatC3 = 0;--so luong id cua mang~
	local TabMatC4 = 0;--so luong id cua mang~
	local TabMatC5 = 0;--so luong id cua mang~
	local TabMatC6 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	local nMatcount2 = 0;--so luong xep chong`
	local nMatcount3 = 0;--so luong xep chong`
	local nMatcount4 = 0;--so luong xep chong`
	local nMatcount5 = 0;--so luong xep chong`
	local nMatcount6 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_MANH[1]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[2]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[3]) then	--manh 1
				TabMat3[TabMatC3+1] = nIndex;
				TabMatC3 = TabMatC3+1;
				nMatcount3 = nMatcount3 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[4]) then	--manh 1
				TabMat4[TabMatC4+1] = nIndex;
				TabMatC4 = TabMatC4+1;
				nMatcount4 = nMatcount4 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[5]) then	--manh 1
				TabMat5[TabMatC5+1] = nIndex;
				TabMatC5 = TabMatC5+1;
				nMatcount5 = nMatcount5 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[6]) then	--tui may man
				TabMat6[TabMatC6+1] = nIndex;
				TabMatC6 = TabMatC6+1;
				nMatcount6 = nMatcount6 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[1].."")
	return end
	if(nMatcount2 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[2].."")
	return end
	if(nMatcount3 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[3].."")
	return end
	if(nMatcount4 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[4].."")
	return end
	if(nMatcount5 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[5].."")
	return end
	if(nMatcount6 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>Tói May m¾n")
	return end
	Pay(10000000)
	if(random(10) < 4) then	--nang cap thanh cong
		local id= ItemSetAdd(2,0,3206-2,0,0,0,0)	--add 1 cai an moi id vi du 9999
		AddItemID(id)
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		for i=1,TabMatC3 do
			RemoveItem(TabMat3[i],1)
		end
		for i=1,TabMatC4 do
			RemoveItem(TabMat4[i],1)
		end
		for i=1,TabMatC5 do
			RemoveItem(TabMat5[i],1)
		end
		for i=1,TabMatC6 do
			RemoveItem(TabMat6[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>Ên C­êng Hãa")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng Ên <color=yellow>Ên C­êng Hãa")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] ACH")
	else	--nang cap that bai, xoa' 1 nua~ so luong
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i , do ng­¬i sö dông Tói May M¾n nªn b¶o toµn ®­îc nguyªn liÖu")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] ACH")
	end
end;
function doiannhtmm()
	if(GetCash() < 10000000) then
		Msg2Player("CÇn 1000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMat2 = {};--mang~ nguyen lieu
	local TabMat3 = {};--mang~ nguyen lieu
	local TabMat4 = {};--mang~ nguyen lieu
	local TabMat5 = {};--mang~ nguyen lieu
	local TabMat6 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local TabMatC2 = 0;--so luong id cua mang~
	local TabMatC3 = 0;--so luong id cua mang~
	local TabMatC4 = 0;--so luong id cua mang~
	local TabMatC5 = 0;--so luong id cua mang~
	local TabMatC6 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	local nMatcount2 = 0;--so luong xep chong`
	local nMatcount3 = 0;--so luong xep chong`
	local nMatcount4 = 0;--so luong xep chong`
	local nMatcount5 = 0;--so luong xep chong`
	local nMatcount6 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DETAIL_MANH[1]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[2]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[3]) then	--manh 1
				TabMat3[TabMatC3+1] = nIndex;
				TabMatC3 = TabMatC3+1;
				nMatcount3 = nMatcount3 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[4]) then	--manh 1
				TabMat4[TabMatC4+1] = nIndex;
				TabMatC4 = TabMatC4+1;
				nMatcount4 = nMatcount4 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[5]) then	--manh 1
				TabMat5[TabMatC5+1] = nIndex;
				TabMatC5 = TabMatC5+1;
				nMatcount5 = nMatcount5 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[6]) then	--tui may man
				TabMat6[TabMatC6+1] = nIndex;
				TabMatC6 = TabMatC6+1;
				nMatcount6 = nMatcount6 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	if(nMatcount1 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[1].."")
	return end
	if(nMatcount2 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[2].."")
	return end
	if(nMatcount3 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[3].."")
	return end
	if(nMatcount4 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[4].."")
	return end
	if(nMatcount5 ~= 20) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 20 c¸i <color=yellow>"..NAME_MANH[5].."")
	return end
	if(nMatcount6 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>Tói May m¾n")
	return end
	Pay(10000000)
	if(random(10) < 4) then	--nang cap thanh cong
		local id= ItemSetAdd(2,0,3216-2,0,0,0,0)	--add 1 cai an moi id vi du 9999
		AddItemID(id)
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		for i=1,TabMatC3 do
			RemoveItem(TabMat3[i],1)
		end
		for i=1,TabMatC4 do
			RemoveItem(TabMat4[i],1)
		end
		for i=1,TabMatC5 do
			RemoveItem(TabMat5[i],1)
		end
		for i=1,TabMatC6 do
			RemoveItem(TabMat6[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>Ên Nh­îc Hãa")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng Ên <color=yellow>Ên Nh­îc Hãa")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] ANH")
	else	--nang cap that bai, xoa' 1 nua~ so luong
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i , do ng­¬i sö dông Tói May M¾n nªn b¶o toµn ®­îc nguyªn liÖu")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] ANH")
	end
end;
function doianhatmm()
	if(GetCash() < 50000000) then
		Msg2Player("CÇn 5000 v¹n l­îng ®Ó n©ng cÊp.")
	return end
	local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	local TabMat1 = {};--mang~ nguyen lieu
	local TabMat2 = {};--mang~ nguyen lieu
	local TabMat3 = {};--mang~ nguyen lieu
	local TabMatC1 = 0;--so luong id cua mang~
	local TabMatC2 = 0;--so luong id cua mang~
	local TabMatC3 = 0;--so luong id cua mang~
	local nMatcount1 = 0;--so luong xep chong`
	local nMatcount2 = 0;--so luong xep chong`
	local nMatcount3 = 0;--so luong xep chong`
	for i=0,5 do
		for j=0,3 do
			nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
			if (nIndex > 0) then
				if(kind == 0 and genre == 3 and detail == DELAIL_2LOAIAN[1]) then	--manh 1
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DELAIL_2LOAIAN[2]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_MANH[6]) then	--tui may man
				TabMat3[TabMatC3+1] = nIndex;
				TabMatC3 = TabMatC3+1;
				nMatcount3 = nMatcount3 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 2) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 2 c¸i <color=yellow>"..NAME_2LOAIAN[1].."")
	return end
	if(nMatcount2 ~= 2) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 2 c¸i <color=yellow>"..NAME_2LOAIAN[2].."")
	return end
	if(nMatcount3 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>Tói May M¾n")
	return end
	Pay(30000000)
	if(random(10) < 5) then	--nang cap thanh cong
		local id= ItemSetAdd(2,0,3226-2,0,0,0,0)	--add 1 cai an moi id vi du 9999
		AddItemID(id)
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		for i=1,TabMatC3 do
			RemoveItem(TabMat3[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>Ngò Hµnh Ên")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng Ên <color=yellow>Ngò Hµnh Ên")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] NHA")
	else	--nang cap that bai, xoa' 1 nua~ so luong
		for i=1,TabMatC3 do
			RemoveItem(TabMat3[i],1)
		end
		Msg2Player("RÊt tiÕc b¹n n©ng cÊp thÊt b¹i , do ng­¬i sö dông Tói May M¾n nªn b¶o toµn ®­îc nguyªn liÖu vµ 40% phÝ Ðp.")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[That bai] NHA")
	end
end;

function kimtinhbachhovuong()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[1]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[1].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,16,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- kim tinh bach ho vuong
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[1].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[1].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[1].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function hoatinhkimhovuong()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[2]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[2].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,15,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- long tinh hac ho vuong
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[2].."")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[2].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[2].." (15 ngµy)")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function longtinhhachovuong()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[3]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[3].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,17,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- long tinh hac ho vuong
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[3].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[3].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[3].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function hanhuyetlongcau()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[4]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[4].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,18,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[4].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[4].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[4].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function sututhuong()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[5]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[5].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,22,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[5].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[5].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[5].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function duongsa()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[6]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[6].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,26,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[6].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[6].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[6].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function nguphong()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[7]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[7].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,27,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[7].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[7].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[7].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function truydien()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[8]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[8].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,28,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[8].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[8].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[8].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function luutinh()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[9]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[9].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,29,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[9].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[9].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[9].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function lacda()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[10]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[10].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,23,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[10].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[10].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[10].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function duongda()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[11]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[11].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,24,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[11].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[11].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[11].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;
function huudom()
	if(GetCash() < 5000000) then
		Msg2Player("CÇn 500 v¹n l­îng ®Ó n©ng cÊp.")
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
				if(kind == 0 and genre == 3 and detail == DETAIL_MANHTHANTHU[12]) then	-- manh thu
				TabMat1[TabMatC1+1] = nIndex;
				TabMatC1 = TabMatC1+1;
				nMatcount1 = nMatcount1 + GetItemStack(nIndex);
				elseif(kind == 0 and genre == 3 and detail == DETAIL_BUAGIAMDINH[1]) then	--manh 1
				TabMat2[TabMatC2+1] = nIndex;
				TabMatC2 = TabMatC2+1;
				nMatcount2 = nMatcount2 + GetItemStack(nIndex);
				else	--khong phai~ nguyen lieu tren, thi cancel
					Msg2Player("Vui lßng kh«ng bá vËt phÈm l¹ vµo")
				return end
			end
		end
	end
	
	if(nMatcount1 ~= 10) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 10 c¸i <color=yellow>"..NAME_THANTHU[12].."")
	return end
	if(nMatcount2 ~= 1) then	--khong dung so luong
		Msg2Player("CÇn bá vµo ®óng 1 c¸i <color=yellow>"..NAME_BUAGIAMDINH[1].."")
	return end
	Pay(5000000)
	if(random(1) == 1) then	--nang cap thanh cong
	local Ngua=ItemSetAdd(0,0,10,25,10,0,0)
	SetItemDate(Ngua,16200)
	AddItemID(Ngua)	-- han huyet long cau
		--xoa cac nguyen lieu
		for i=1,TabMatC1 do
			RemoveItem(TabMat1[i],1)
		end
		for i=1,TabMatC2 do
			RemoveItem(TabMat2[i],1)
		end
		Msg2Player("Chóc mõng b¹n n©ng cÊp thµnh c«ng, nhËn ®­îc 1 <color=yellow>"..NAME_THANTHU[12].." (15 ngµy)")
		Msg2SubWorld("Chóc mõng <color=yellow>"..GetName().."<color> luyÖn thµnh c«ng <color=yellow>"..NAME_THANTHU[12].." (15 ngµy)")
		logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\t[Thanh cong] "..NAME_THANTHU[12].."")
	else	--nang cap that bai, xoa' 1 nua~ so luong
	end
end;




function tlach()
if(GetCash() < 25000000) then
	Msg2Player("CÇn 2500 v¹n l­îng ®Ó tinh luyÖn.")
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
 Msg2Player("Kh«ng ®­îc bá vËt phÈm l¹ vµo ®©y!")
 return end
 if(Rrow < 3210 or Rrow > 3215) then
	Msg2Player("ChØ cã thÓ bá vµo ®©y Hoµng Kim Ên (C­êng Hãa) cÊp 5 trë lªn.")
return end	
 if(GetItemDate(nRealIndex) > 1) then
 Msg2Player("Trang bÞ cã thêi h¹n kh«ng thÓ bá vµo r·!")
 return end
 Pay(25000000)
 local id= ItemSetAdd(0,3,DELAIL_2LOAIAN[1],0,0,5,0,0)
 AddItemID(id)
 RemoveItem(nRealIndex,1);
Msg2Player("B¹n ®· tinh luyÖn thµnh c«ng <color=yellow>"..NAME_2LOAIAN[1].."")
logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\tTinh Luyen An Cuong Hoa")
end

function tlanh()
if(GetCash() < 25000000) then
	Msg2Player("CÇn 2500 v¹n l­îng ®Ó tinh luyÖn.")
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
 Msg2Player("Kh«ng ®­îc bá vËt phÈm l¹ vµo ®©y!")
 return end
 if(Rrow < 3220 or Rrow > 3225) then
	Msg2Player("ChØ cã thÓ bá vµo ®©y Hoµng Kim Ên (Nh­îc Hãa) cÊp 5 trë lªn.")
return end	
 if(GetItemDate(nRealIndex) > 1) then
 Msg2Player("Trang bÞ cã thêi h¹n kh«ng thÓ bá vµo r·!")
 return end
 Pay(25000000)
 local id= ItemSetAdd(0,3,DELAIL_2LOAIAN[2],0,0,5,0,0)
 AddItemID(id)
 RemoveItem(nRealIndex,1);
Msg2Player("B¹n ®· tinh luyÖn thµnh c«ng <color=yellow>"..NAME_2LOAIAN[2].."")
logMinhChu(""..GetName().."\t\t\t"..GetAccount().."\t\t\tTinh Luyen An Nhuoc Hoa")
end

function hoantatluyen()
Talk(1,"","TÝnh n¨ng ®ang ®­îc hoµn thiÖn")
end


function no()
end
