Include( "\\script\\header\\taskid.lua" )
Include("\\script\\library\\worldlibrary.lua");
function main(id)
	local dwid = GetNpcValue(id);
	if(GetUUID() ~= dwid) then
	Talk(1,"","Hoa hÂng t·a h≠¨ng th¨m ngµo ngπt!");
	return end
	SetTaskTemp(NPCINDEX, id);
	Say("Hoa hÂng Æ∑ ra hoa, bπn muËn nhÀn th≠Îng kh´ng?",2,
	"NhÀn th≠Îng may mæn/nhanthuong",
	"Kh´ng/no")
end

function nhanthuong()
	SetNpcValue(GetTaskTemp(NPCINDEX), 0);
	--thuong----------
	if (CheckRoom(2,3) == 0) then Talk(1,"","Vui lﬂng chıa ›t nh t 2x3 ´ trËng.") return end
	AddOwnExp(20000000); -- 20 trieu exp
	PhanThuong83()
end;

function PhanThuong83()
	Dat = random(9)
	if Dat == 1 then
	AddEventItem(16)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>An Bang Ng‰c bÈi b∂o r≠¨ng.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tAn Bang Ngoc Boi Ruong")
	elseif Dat == 2 then
	AddEventItem(17)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>An Bang Th≠Óng giÌi b∂o r≠¨ng.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tAn Bang Thuong Gioi Ruong")
	elseif Dat == 3 then
	AddEventItem(18)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>An Bang Hπ giÌi b∂o r≠¨ng.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tAn Bang Ha Gioi Ruong")
	elseif Dat == 4 then
	AddEventItem(20)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>ßﬁnh QuËc § Sa b∂o r≠¨ng.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tDinh Quoc O Sa Ruong")	
	elseif Dat == 5 then
	AddEventItem(21)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>ßﬁnh QuËc Thanh Sa b∂o r≠¨ng.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tDinh Quoc Thanh Sa Ruong")	
	elseif Dat == 6 then
	AddEventItem(77)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>TÛi thÛ c≠Ïi.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tTui Thu Cuoi")
	elseif Dat == 7 then
	AddMat(random(41,45),10)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ 10 <color=yellow>ThŒ bµi trang bﬁ.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tThe Bai Trang Bi")
	elseif Dat == 8 then
	AddEventItem(79)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>HÂng ∂nh L‘ Bao.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tHong Anh Le Bao")
	elseif Dat == 9 then
	AddMat(random(46,49),10)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ 10 <color=yellow>ThŒ bµi trang bﬁ.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tThe Bai Trang Bi")
	end
end
function no()
end;
function logPhanThuonghh(str)
local gm_Log = "dulieu/SuKien/HoaHong.txt"
local fs_log = openfile(gm_Log, "a");
write(fs_log, date("%H:%M:%S_%d-%m-%y").."\t"..str.."\n");
closefile(fs_log);
end