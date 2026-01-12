Include("\\script\\header\\factionhead.lua")
Include("\\script\\misc\\daiyitoushi\\change_faction.lua")

function main(nNpcIndex)
	if (nvthegioi(nNpcIndex) == 1) then
		return
	end;
	local player_Faction = GetFaction();
	if (player_Faction == "") then
		Talk(1, "enroll_select",
			"Thóy Yªn m«n chóng ta tuy toµn lµ n÷, nh­ng vâ nghÖ lÊy 'Khinh, Kho¸i, Kú, Mü' mµ næi tiÕng giang hå. Tû muéi tÒ t©m!  Trong giang hå kh«ng ai d¸m coi khinh")
	elseif (player_Faction == "cuiyan") then
		if (GetNumber(1, GetTask(TASK_DUNGCHUNG2), 9) == 0) then
			if GetLevel() >= 60 then
				Say("<npc>: Tû tû t¹i sao xuÊt s­ vËy!  Cø ë ®©y tù t¹i ch¼ng ph¶i vui vÎ l¾m sao!?", 5,
					"ChuyÓn ph¸i/doiphai", "XuÊt s­ xuèng nói/xuatsu", "T×m hiÓu khu vùc luyÖn c«ng/map_help",
					"T×m hiÓu vâ nghÖ bæn m«n/skill_help", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
			else
				Say("<npc>: Tû tû t¹i sao xuÊt s­ vËy!  Cø ë ®©y tù t¹i ch¼ng ph¶i vui vÎ l¾m sao!?", 3,
					"T×m hiÓu khu vùc luyÖn c«ng/map_help", "T×m hiÓu vâ nghÖ bæn m«n/skill_help",
					"Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
			end
		else
			Say("<npc>: Tû tû t¹i sao xuÊt s­ vËy!  Cø ë ®©y tù t¹i ch¼ng ph¶i vui vÎ l¾m sao!?", 4,
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
	elseif (player_Faction == "emei") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Xinh ®Ñp nh­ hoa t­¬i mµ h»ng ngµy cø ®èi diÖn víi Thanh §¨ng Cæ PhËt!  Ta kh«ng thİch c¸c ng­¬i ë ®iÓm nµy! ")
		end
	elseif (player_Faction == "tangmen") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Hai ph¸i chóng ta liªn minh l¹i, kh«ng cÇn ph¶i ph©n tranh n÷a! Ch¼ng ph¶i lµ hû sù ­?")
		end
	elseif (player_Faction == "wudu") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Ta ghĞt nhÊt lµ c¸i bän len lĞn h¹ ®éc h¹i ng­êi!  Êy!  Kh«ng ph¶i ta nãi ng­¬i! ")
		end
	elseif (player_Faction == "tianwang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"D­¬ng Hå cña Thiªn V­¬ng bang c¸c ng­¬i cßn kĞm xa tû tû cña ta, thiªn h¹ nam nh©n qu¶ nhiªn ch¼ng ra g× hÕt! ")
		end
	elseif (player_Faction == "shaolin") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Hßa th­îng ®Çu träc sao l¹i ®Õn n¬i cña c¸c n÷ nhi thÕ nµy?! ")
		end
	elseif (player_Faction == "wudang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Quı m«n lÊy hiÖp nghÜa lµm träng, nh­ng kh«ng biÕt §¬n T­ Nam cã biÕt hæ thÑn hay kh«ng mµ cø b¸m theo c¸c s­ tû xinh ®Ñp cña chóng ta")
		end
	elseif (player_Faction == "kunlun") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "C«n L«n ph¸i ph¸t triÓn thÇn tèc! TiÓu n÷ rÊt ng­ìng mé! ")
		end
	elseif (player_Faction == "tianren") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Ai lµm hoµng ®Õ th× còng mÆc!  Nh­ng c¸c ng­¬i tµn s¸t sinh linh th× bæn c« n­¬ng kh«ng bá qua! ")
		end
	elseif (player_Faction == "gaibang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(2, "", "§¹i hiÖp!  §õng qua ®©y! ", "¸! Bä chĞt!!! ")
		end
	end
end

function enroll_select()
	if (GetSeries() == 2) and (GetCamp() == 0) then
		if (GetLevel() >= 10) then
			Say(10188, 2, "Gia nhËp Thóy Yªn/go", "§Ó ta suy nghÜ kü l¹i xem/thing")
		else
			Talk(1, "", "C¨n b¶n cña ng­¬i cßn kĞm l¾m! H·y ®i luyÖn tËp thªm, bao giê ®Õn cÊp 10 l¹i ®Õn t×m ta!")
		end
	end
end;

function go()
	gianhapmonphai(5)
end;

function thing()
	Talk(1, "", 10187)
end;

function doiphai()
	dk_chuyenphai(5)
end
