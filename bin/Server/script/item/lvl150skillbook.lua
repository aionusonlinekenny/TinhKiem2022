LV150SKILL_LIST = {
	[1] = {
		{1055, 1, 0, 150, "Kü n¨ng cÊp 150", "§¹i Thõa Nh­ Lai Chó "},
		{1056, 1, 0, 150, "Kü n¨ng cÊp 150", "Vi §µ HiÕn Xö"},
		{1057, 1, 0, 150, "Kü n¨ng cÊp 150", "Tam Giíi Quy ThiÒn"}
	},
	[2] = {
		{1058, 1, 1, 150, "Kü n¨ng cÊp 150", "Hµo Hïng Tr¶m"},
		{1059, 1, 1, 150, "Kü n¨ng cÊp 150", "Tung Hoµnh B¸t Hoang"},
		{1060, 1, 1, 150, "Kü n¨ng cÊp 150", "B¸ V­¬ng T¹m Kim"}
	},
	[3] = {
		{1069, 1, 2, 150, "Kü n¨ng cÊp 150", "V« ¶nh Xuyªn"},
		{1070, 1, 2, 150, "Kü n¨ng cÊp 150", "ThiÕt Liªn Tø S¸t"},
		{1071, 1, 2, 150, "Kü n¨ng cÊp 150", "Cµn Kh«n NhÊt TrÞch"},
		{1110, 0, 2, 150, "Kü n¨ng cÊp 150", "TÝch LÞch Lo¹n Hoµn KÝch"}
	},
	[4] = {
		{1066, 1, 3, 150, "Kü n¨ng cÊp 150", "H×nh Tiªu Cèt LËp"},
		{1067, 1, 3, 150, "Kü n¨ng cÊp 150", "U Hån PhÖ ¶nh"}
	},
	[5] = {
		{1061, 1, 4, 150, "Kü n¨ng cÊp 150", "KiÕm Hoa V·n Tinh"},
		{1062, 1, 4, 150, "Kü n¨ng cÊp 150", "B¨ng Vò L¹c Tinh"},
		{1114, 0, 4, 150, "Kü n¨ng cÊp 150", "Ngäc TuyÒn T©m Kinh"}
	},
	[6] = {
		{1063, 1, 5, 150, "Kü n¨ng cÊp 150", "B¨ng T­íc Ho¹t Kú"},
		{1065, 1, 5, 150, "Kü n¨ng cÊp 150", "Thñy Anh Man Tó"}
	},
	[7] = {
		{1073, 1, 6, 150, "Kü n¨ng cÊp 150", "Thêi Thõa Lôc Long"},
		{1074, 1, 6, 150, "Kü n¨ng cÊp 150", "Bæng Huýnh L­îc §Þa"}
	},
	[8] = {
		{1075, 1, 7, 150, "Kü n¨ng cÊp 150", "Giang H¶i Né Lan"},
		{1076, 1, 7, 150, "Kü n¨ng cÊp 150", "TËt Háa LiÖu Nguyªn"}
	},
	[9] = {
		{1078, 1, 8, 150, "Kü n¨ng cÊp 150", "T¹o Hãa Th¸i Thanh"},
		{1079, 1, 8, 150, "Kü n¨ng cÊp 150", "KiÕm Thïy Tinh Hµ"}
	},
	[10] = {
		{1080, 1, 9, 150, "Kü n¨ng cÊp 150", "Cöu Thiªn C­¬ng Phong"},
		{1081, 1, 9, 150, "Kü n¨ng cÊp 150", "Thiªn L«i ChÊn Nh¹c"}
	},
	[11] = {
		{1369, 1, 10, 150, "Kü n¨ng cÊp 150", "Cöu kiÕm hîp nhÊt"},
		{1384, 1, 10, 150, "Kü n¨ng cÊp 150", "ThÇn Quang Toµn NhiÔu"}
	},
	[12] = {
		{1985, 1, 11, 150, "Kü n¨ng cÊp 150", "Trõ Gian DiÖt NÞnh"},
		{1969, 1, 11, 150, "Kü n¨ng cÊp 150", "Huy S­ DiÖt Lç"}
	},
	[13] = {
		{2142, 1, 12, 150, "Kü n¨ng cÊp 150", "Mai Hoa Tam Léng"},
		{2129, 1, 12, 150, "Kü n¨ng cÊp 150", "ThËp Bé NhÊt S¸t"}
	}
}

--Include("\\script\\item\\skillbook.lua")

function main(sel)
	if GetFactionNo() == -1 then
		Msg2Player("<color=yellow>Ch­a gia nhËp m«n ph¸i, kh«ng thÓ häc kü n¨ng nµy!<color>")
		return 1
	end

	if GetLevel() < 150 then
		Msg2Player("<color=yellow>§¼ng cÊp ch­a ®ñ 150, kh«ng thÓ häc kü n¨ng nµy!<color>")
		return 1
	end

	if (LearnSkillByBook() == 0) then
		RemoveItem(sel, 1)
		return 0
	end
	return 1
end

function LearnSkillByBook()
	local nFaction = GetFactionNo()
	local nList = LV150SKILL_LIST[nFaction + 1]
	for i = 1, getn(nList) do
		if (nFaction == nList[i][3]) then
			--Msg2Player(nList[i][6] .. "-" .. nList[i][1] .. "-" .. nList[i][2])
			if (GetLevel() < nList[i][4]) then
				Msg2Player("B¹n cÇm lÊy " .. nList[i][5] .. " nghiªn cøu c¶ nöa ngµy, kÕt qu¶ lÜnh ngé rÊt Ýt. ")
				return 1
			end
			if (HaveMagic(nList[i][1]) < nList[i][2]) then
				AddMagic(nList[i][1], nList[i][2])
				Msg2Player("§· häc ®­îc kü n¨ng <color=yellow>" .. nList[i][6] .. "<color>. ")
			else
				Msg2Player("B¹n ®· cÇm " .. nList[i][5] .. " nghiÒn tíi ngÉm lui, nh­ng còng kh«ng häc ®­îc g× trong ®ã ")
			end
		end
	end

	return 0
end
