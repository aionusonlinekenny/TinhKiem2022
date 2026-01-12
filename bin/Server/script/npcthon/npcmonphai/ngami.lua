Include("\\script\\header\\factionhead.lua")
Include("\\script\\misc\\daiyitoushi\\change_faction.lua")

function main(nNpcIndex)
	if (nvthegioi(nNpcIndex) == 1) then
		return
	end;
	local player_Faction = GetFaction();
	if (player_Faction == "") then
		Talk(1, "enroll_select",
			"Trong thiªn h¹ c¸c m«n ph¸i do n÷ nhi s¸ng lËp th× bæn m«n ®øng ®Çu!  §Ö tö bæn m«n kh«ng nh÷ng vâ nghÖ cao c­êng, cÇm kú thi häa ®Òu tinh th«ng!  Ai nãi giang hå lµ thÕ giíi cña nam nh©n th× cø nh×n bæn ph¸i mµ lµm g­¬ng")
	elseif (player_Faction == "emei") then
		if (GetNumber(1, GetTask(TASK_DUNGCHUNG2), 9) == 0) then
			if GetLevel() >= 60 then
				Say("<npc>: TiÓu s­ muéi l¹i xuèng nói ch¬i µ? Cã cÇn ta ®­a vÒ nói kh«ng?", 5,
					"ChuyÓn ph¸i/doiphai", "XuÊt s­ xuèng nói/xuatsu", "T×m hiÓu khu vùc luyÖn c«ng/map_help",
					"T×m hiÓu vâ nghÖ bæn m«n/skill_help", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
			else
				Say("<npc>: TiÓu s­ muéi l¹i xuèng nói ch¬i µ? Cã cÇn ta ®­a vÒ nói kh«ng?", 3,
					"T×m hiÓu khu vùc luyÖn c«ng/map_help", "T×m hiÓu vâ nghÖ bæn m«n/skill_help",
					"Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
			end
		else
			Say("<npc>: TiÓu s­ muéi l¹i xuèng nói ch¬i µ? Cã cÇn ta ®­a vÒ nói kh«ng?", 4,
				"Trïng ph¶n m«n ph¸i/trungphansumon", "T×m hiÓu khu vùc luyÖn c«ng/map_help",
				"T×m hiÓu vâ nghÖ bæn m«n/skill_help", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		end
	elseif (player_Faction == "huashan") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Xin chµo ®¹i hiÖp!")
		end
	elseif (player_Faction == "vuhontang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Xin chµo ®¹i hiÖp!")
		end
	elseif (player_Faction == "xiaoyao") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Xin chµo ®¹i hiÖp!")
		end
	elseif (player_Faction == "cuiyan") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Quı ph¸i danh chÊn V©n Nam. Tr­íc giê ch­a cïng bæn ph¸i qua l¹i. Kh«ng biÕt cã g× chØ gi¸o! ")
		end
	elseif (player_Faction == "tangmen") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Ng­¬i lµ ng­êi cña Thôc Nam §­êng M«n? Bæn m«n lµ danh m«n ch¸nh ph¸i, tr­íc giê kh«ng qua l¹i víi nh÷ng ng­êi cã t­ t­ëng kh«ng chİnh thèng")
		end
	elseif (player_Faction == "wudu") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Bän tµ ma ngo¹i ®¹o c¸c ng­¬i!  Ai ai còng muèn tiªu diÖt! ")
		end
	elseif (player_Faction == "tianwang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Quı bang bang chñ D­¬ng Anh lµ mét bËc n÷ trung hµo kiÖt!  BÇn ni tõ l©u ng­ìng mé!  Mong sÏ cã ngµy diÖn kiÕn! ")
		end
	elseif (player_Faction == "shaolin") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"A di ®µ phËt!  Th× ra lµ hßa th­îng  ThiÕu l©m, kh«ng biÕt HuyÒn Nh©n ®¹i s­ gÇn ®©y cã kháe kh«ng?")
		end
	elseif (player_Faction == "wudang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Vâ ®ang hiÖp nghÜa, danh chÊn thiªn h¹. §¹o NhÊt Ch©n Nh©n cña quı ph¸i vµ ch­ëng m«n cña bæn ph¸i giao t×nh th©m s©u. Hai ph¸i chóng ta cã thÓ nãi lµ  anh em mét nhµ ")
		end
	elseif (player_Faction == "kunlun") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "TÖ ph¸i vµ quı ph¸i ch­a hÒ qua l¹i, kh«ng biÕt cã g× chØ gi¸o!?")
		end
	elseif (player_Faction == "tianren") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"C¸c ng­¬i phô thuéc Kim quèc, lu«n dßm ngã giang s¬n  §¹i tèng ta. Vâ l©m chİnh ph¸i chóng ta vµ c¸c ng­¬i thÒ kh«ng ®éi trêi chung! ")
		end
	elseif (player_Faction == "gaibang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Quı bang ng­êi ®«ng thÕ m¹nh, hµo khİ xung thiªn, bæn gi¸o rÊt vui ®­îc kÕt giao víi nh÷ng b»ng h÷u nh­ vËy! ")
		end
	end
end

function enroll_select()
	if(GetSex() == 0)then
		Talk(1, "", "Ng­¬i lµ nam nh©n d¸m mß ®Õn ®©y, muèn chÕt µ!")
		return
	end
	if (GetSeries() == 2) and (GetCamp() == 0) then
		if (GetLevel() >= 10) then
			Say("Gia nhËp Nga My ph¸i, chóng ta sÏ lµ tû muéi 1 nhµ!  Häa phóc cïng h­ëng! ", 2,
				"Gia nhËp Nga Mi ph¸i/go", "§Ó ta suy nghÜ kü l¹i xem/thing")
		else
			Say(
				"Tr­íc tiªn muéi h·y luyÖn tËp l¹i c¨n b¶n, ®¹t ®Õn <color=Red>cÊp 10<color> chóng ta sÏ lµ tû muéi 1 nhµ! ",
				0)
		end
	end
end;

function go()
	gianhapmonphai(4)
end;

function thing()
	Talk(1, "", 10213)
end;

function doiphai()
	dk_chuyenphai(4)
end
