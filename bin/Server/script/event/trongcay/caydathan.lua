--||Author:	Min khung
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");

function main(NpcIndex)
	-- dofile("script/feature/trongcay/caydathan.lua")
	if(GetNpcValue(NpcIndex) == GetUUID()) then
	local DangCapCay = GetTaskTemp(CAPCAY);
	SetTaskTemp(NPCINDEX,NpcIndex)
		if DangCapCay == 1 then
		Say("Giai ®o¹n nµy lµ t­íi c©y yªu cÇu ph¶i cã <color=blue>Thïng N­íc<color>, "..TinhThoiGian(NpcIndex)..".",2,
			"B¾t ®Çu t­íi c©y/ChamSoc",
			"Rêi khái/no")
		elseif DangCapCay == 2 then
		Say("Giai ®o¹n nµy lµ bãn ph©n yªu cÇu ph¶i cã <color=blue>Tói Ph©n Bãn<color>, "..TinhThoiGian(NpcIndex)..".",2,
			"B¾t ®Çu bãn ph©n/ChamSoc",
			"Rêi khái/no")
		elseif DangCapCay == 3 then
		Say("Giai ®o¹n cuèi cïng lµ b¾t s©u yªu cÇu ph¶i cã <color=blue>Thuèc Trõ S©u<color>, "..TinhThoiGian(NpcIndex)..".",2,
			"B¾t ®Çu diÖt s©u/ChamSoc",
			"Rêi khái/no")
		else
		Talk(1,"","Xin chóc mõng c©y ®a ®· lín kh«ng cÇn ch¨m sãc thªm, cßn <color=yellow>"..floor(GetNpcLifeTime(NpcIndex)/18).." gi©y<color> n÷a c©y sÏ tù biÕn mÊt.")
		end
	else
		Say("§©y kh«ng ph¶i c©y cña ng­¬i trång, h·y mau rêi khái ®©y.",2,
			"Rêi khái/no")
	end
end

function TinhThoiGian(NpcIndex)
	Giay = floor(GetNpcLifeTime(NpcIndex)/18)
	if Giay > 30 then
	return "cßn <color=yellow>"..(Giay-30).." gi©y<color> n÷a h·y ch¨m sãc c©y"
	else
	return "cßn <color=yellow>"..Giay.." gi©y<color> n÷a c©y sÏ biÕn mÊt"
	end
end

function ChamSoc()
	local W,X,Y = GetWorldPos();
	local MapPlayer = GetTaskTemp(SAVEMAP);
	local ToaDoX = GetTaskTemp(SAVEXPOS);
	local ToaDoY = GetTaskTemp(SAVEYPOS);
	local IDCay = GetTaskTemp(NPCINDEX);
	local DangCapCay = GetTaskTemp(CAPCAY);
	if MapPlayer == 0 then DelNpc(IDCay) return end
	if floor(GetNpcLifeTime(GetTaskTemp(NPCINDEX))/18) > 30 then Talk(1,"","HiÖn t¹i ch­a tíi thêi gian ch¨m sãc vui lßng chê.") return end
	if DangCapCay == 1 then
	if GetItemCount(43,5) < 1 then Talk(1,"","B¹n kh«ng mang theo Thïng N­íc kh«ng thÓ t­íi c©y.") return end
	DelNpc(IDCay)
	CallNpc(1452, 2, MapPlayer, ToaDoX, ToaDoY, "C©y ®a (bãn)", 120);
	SetTaskTemp(CAPCAY,2)
	SetTimer(10*18,8)
	SetTaskTemp(CAYHOAHONG,12)
	DelItem(43,5)
	elseif DangCapCay == 2 then
	if GetItemCount(42,5) < 1 then Talk(1,"","B¹n kh«ng mang theo Tói Ph©n Bãn kh«ng thÓ bãn ph©n.") return end
	DelNpc(IDCay)
	CallNpc(1452, 3, MapPlayer, ToaDoX, ToaDoY, "C©y ®a (s©u)", 120);
	SetTaskTemp(CAPCAY,3)
	SetTimer(10*18,8)
	SetTaskTemp(CAYHOAHONG,12)
	DelItem(42,5)
	elseif DangCapCay == 3 then
	if GetItemCount(44,5) < 1 then Talk(1,"","B¹n kh«ng mang theo Thuèc Trõ S©u kh«ng thÓ trõ s©u.") return end
	DelNpc(IDCay)
	CallNpc(1452, 4, MapPlayer, ToaDoX, ToaDoY, "C©y ®a thÇn kú", 90);
	SetTaskTemp(CAPCAY,4)
	SetTimer(10*18,8)
	SetTaskTemp(CAYHOAHONG,9)
	DelItem(44,5)
	end
end

function CallNpc(NpcId, NpcLevel, W, X, Y, Name, Giay)
	local playername = GetName().."-";
	nNpcIdx = AddNpcNew(NpcId,NpcLevel,W,X*32,Y*32,"\\script\\feature\\trongcay\\caydathan.lua",6,playername..Name)
	SetNpcValue(nNpcIdx,GetUUID())
	SetNpcLifeTime(nNpcIdx, Giay*18);
end

function no()
end