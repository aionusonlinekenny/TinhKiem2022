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
	AddTool(88)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>B› k›p 150.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tBK 150")
	elseif Dat == 2 then
	Dumenongua()
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>Ng˘a cao c p.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tNgua Cao Cap")
	elseif Dat == 3 then
	AddMat(47)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>ß‰ phÊ Bπch HÊ.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tDo Pho Bach Ho")
	elseif Dat == 4 then
	AddMat(48)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>Bπch HÊ Thπch.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tBach Ho Thach")	
	elseif Dat == 5 then
	AddMat(49)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>ß‰ phÊ X›ch L©n.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tDo Pho Xich Lan")	
	elseif Dat == 6 then
	AddMat(50)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ <color=yellow>X›ch L©n Thπch.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tX›ch L©n Thπch")
	elseif Dat == 7 then
	AddMat(random(66,68),10)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ 10 <color=yellow>ThŒ bµi trang bﬁ.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tThe Bai Trang Bi")
	elseif Dat == 8 then
	AddMat(random(63,65),10)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ 10 <color=yellow>ThŒ bµi trang bﬁ.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tThe Bai Trang Bi")
	elseif Dat == 9 then
	AddMat(random(60,62),10)
		Msg2Player("Bπn nhÀn Æ≠Óc 20.000.000 Æi”m kinh nghi÷m vµ 10 <color=yellow>ThŒ bµi trang bﬁ.")
	logPhanThuonghh(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tThe Bai Trang Bi")
	end
end
function Dumenongua()
		n = RANDOM(6)
		if (n == 2) then
			a = random(1,2)
			local Ngua = ItemSetAdd(0,0,10,NGUA[a][1],10,0,0)
			AddItemID(Ngua)
			Msg2Player("Bπn nhÀn Æ≠Óc <color=yellow>"..NGUA[a][2].." (v‹nh vi‘n)")
			logPhanThuong(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tEvent: "..GetNumber3(GetTask(TASK_HOAHONG),3).."\t\t"..NGUA[a][2].." (vinh vien)")
		else
			a = random(1,9)
			local Ngua = ItemSetAdd(0,0,10,NGUA[a][1],10,0,0)
				if a > 5 then
				SetItemRich(Ngua,6000)
				SetItemTS(Ngua,1)
				elseif a > 2 then
				SetItemRich(Ngua,3000)
				SetItemTS(Ngua,1)
				end
			SetItemDate(Ngua,43200)
			AddItemID(Ngua)
			Msg2Player("Bπn nhÀn Æ≠Óc <color=yellow>"..NGUA[a][2].."")
			logPhanThuong(""..GetAccount().."\t\t"..GetName().."\t\tXu: "..GetCoin(1).."\t\tEvent: "..GetNumber3(GetTask(TASK_HOAHONG),3).."\t\t"..NGUA[a][2].."")
		end
	end
NGUA = {
	{9,"X›ch Long C©u"},
	{13,"Si™u Quang"},
	{15,"H·a Tinh Kim HÊ V≠¨ng"},
	{16,"Kim Tinh Bπch HÊ V≠¨ng"},
	{17,"Long Tinh Hæc HÊ V≠¨ng"},
	{22,"S≠ tˆ "},
	{23,"Lπc Æµ "},
	{24,"D≠¨ng ßµ "},
	{25,"H≠¨u ÆËm"},
}

function no()
end;
function logPhanThuonghh(str)
local gm_Log = "dulieu/SuKien/HoaHong.txt"
local fs_log = openfile(gm_Log, "a");
write(fs_log, date("%H:%M:%S_%d-%m-%y").."\t"..str.."\n");
closefile(fs_log);
end