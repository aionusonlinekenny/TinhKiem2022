-- script by ManhDat
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");

function main(NpcIndex)
	if(GetNpcValue(NpcIndex) == GetUUID()) then
	local DangCapCay = GetTaskTemp(CAPCAY);
	SetTaskTemp(NPCINDEX,NpcIndex)
		if DangCapCay == 1 then
		Say("Chµo "..GetName().." h·y trang trÝ thªm 5 <color=blue>T¸o §á<color> cho c©y th«ng, "..TinhThoiGian(NpcIndex)..".",2,
			"G¾n thªm t¸o ®á/ChamSoc",
			"Rêi khái/no")
		elseif DangCapCay == 2 then
		Say("Chµo "..GetName().." h·y trang trÝ thªm <color=blue>Ng«i Sao ¦íng Väng<color> cho c©y th«ng, "..TinhThoiGian(NpcIndex)..".",2,
			"G¾n thªm ®Ìn ng«i sao/ChamSoc",
			"Rêi khái/no")
		elseif DangCapCay == 3 then
		Say("Chµo "..GetName().." h·y trang trÝ thªm <color=blue>KÑo ChiÕc GËy<color> cho c©y th«ng, "..TinhThoiGian(NpcIndex)..".",2,
			"G¾n thªm kÑo chiÕc gËy/ChamSoc",
			"Rêi khái/no")
		elseif DangCapCay == 4 then
		Say("Chµo "..GetName().." h·y trang trÝ thªm <color=blue>Hép Quµ Gi¸ng Sinh<color> cho c©y th«ng, "..TinhThoiGian(NpcIndex)..".",2,
			"G¾n thªm Hép Quµ Gi¸ng Sinh/ChamSoc",
			"Rêi khái/no")
		elseif DangCapCay == 5 then
		Say("Xin chóc mõng "..GetName().." c©y th«ng cña b¹n ®· trang trÝ ®Çy ®ñ , "..TinhThoiGian(NpcIndex).." .",2,
			"NhËn th­ëng Noel 2015/ChamSoc",
			"Rêi khái/no")
		else
		Talk(1,"","Xin chóc mõng c©y th«ng ®· ®· trang trÝ ®Çy ®ñ , cßn <color=yellow>"..floor(GetNpcLifeTime(NpcIndex)/18).." gi©y<color> n÷a c©y sÏ tù biÕn mÊt.")
		end
	else
		Say("§©y kh«ng ph¶i c©y cña ng­¬i trång,kh«ng thÓ trang trÝ, h·y mau rêi khái ®©y.",2,
			"Rêi khái/no")
	end
end

function TinhThoiGian(NpcIndex)
	Giay = floor(GetNpcLifeTime(NpcIndex)/18)
	if Giay > 30 then
	return "cßn <color=yellow>"..(Giay-30).." gi©y<color> n÷a h·y trang trÝ"
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
	if floor(GetNpcLifeTime(GetTaskTemp(NPCINDEX))/18) > 30 then Talk(1,"","HiÖn t¹i ch­a tíi thêi gian trang trÝ vui lßng chê.") return end
	if DangCapCay == 1 then
	if GetItemCount(69,5) < 5 then Talk(1,"","B¹n kh«ng mang theo 5 T¸o §á kh«ng thÓ trang trÝ.") return end
	DelNpc(IDCay)
	CallNpc(1491, 1, W, floor(X/32), floor(Y/32), "C©y Th«ng Noel T¸o §á", 120);
	SetTaskTemp(CAPCAY,2)
	-- SetTimer(10*18,8)
	SetTaskTemp(CAYTHONGNOEL,12)
	DelItem(69,5,5)
	elseif DangCapCay == 2 then
	if GetItemCount(71,5) < 5 then Talk(1,"","B¹n kh«ng mang theo 5 Ng«i Sao ¦íng Väng kh«ng thÓ trang trÝ.") return end
	DelNpc(IDCay)
	CallNpc(1492, 1, W, floor(X/32), floor(Y/32), "C©y Th«ng Noel Ng«i Sao", 120);
	SetTaskTemp(CAPCAY,3)
	-- SetTimer(10*18,8)
	SetTaskTemp(CAYTHONGNOEL,12)
	DelItem(71,5,5)
	elseif DangCapCay == 3 then
	if GetItemCount(70,5) < 5 then Talk(1,"","B¹n kh«ng mang theo 5 KÑo ChiÕc GËy kh«ng thÓ trang trÝ.") return end
	DelNpc(IDCay)
	CallNpc(1493, 1, W, floor(X/32), floor(Y/32), "C©y Th«ng Noel ChiÕc KÑo", 90);
	SetTaskTemp(CAPCAY,4)
	-- SetTimer(10*18,8)
	SetTaskTemp(CAYTHONGNOEL,9)
	DelItem(70,5,5)
	elseif DangCapCay == 4 then
	if GetItemCount(75,5) < 5 then Talk(1,"","B¹n kh«ng mang theo Hép Quµ Gi¸ng Sinh kh«ng thÓ trõ s©u.") return end
	DelNpc(IDCay)
	CallNpc(1494, 1, W, floor(X/32), floor(Y/32), "C©y Th«ng Noel Hoµn H¶o", 90);
	SetTaskTemp(CAPCAY,5)
	-- SetTimer(10*18,8)
	SetTaskTemp(CAYTHONGNOEL,9)
	DelItem(75,5,5)
	elseif DangCapCay == 5 then
	DelNpc(IDCay)
	CallNpc(1494, 1, W, floor(X/32), floor(Y/32), "C©y Th«ng Noel Hoµn H¶o", 90);
	SetTaskTemp(CAPCAY,0)
	-- SetTimer(10*18,8)
	SetTaskTemp(CAYTHONGNOEL,9)
	NhanThuongNgauNhien()
end
end

function NhanThuongNgauNhien()
logMiniEvent("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\t[Level: "..GetLevel().."]\t\t[Trang Tri Thong Thanh Cong]")
Msg2SubWorld("<color=yellow>Chóc mõng ng­êi ch¬i<color> <color=Wood>"..GetName().."<color> <color=yellow>®· trång 1 c©y th«ng trªn ®Ønh Hoa S¬n.<color>")
Talk(1,"","Hoµn thµnh Event nhËn th­ëng")
end

function logMiniEvent(str)
local gm_Log = "dulieu/SuKien/logMiniEvent.txt"
local fs_log = openfile(gm_Log, "a");
write(fs_log, date("%H:%M:%S_%d-%m-%y").."\t"..str.."\n");
closefile(fs_log);
end



function CallNpc(NpcId, NpcLevel, W, X, Y, Name)
	local playername = GetName().."-";
	nNpcIdx = AddNpcNew(NpcId,NpcLevel,W,X*32,Y*32,"\\script\\feature\\trongcay\\caythongnoel.lua",6,playername..Name)
	SetNpcValue(nNpcIdx,GetUUID())
	SetNpcLifeTime(nNpcIdx, 120*18);
end

function no()
end