Include("\\script\\header\\factionhead.lua")
Include("\\script\\misc\\daiyitoushi\\change_faction.lua")

function main(nNpcIndex)
	if (nvthegioi(nNpcIndex) == 1) then
		return
	end;
	local player_Faction = GetFaction();
	if (player_Faction == "") then
		Talk(1, "enroll_select", "Chµo mõng b¹n ®Õn Hoa S¬n ph¸i!")
	elseif (player_Faction == "huashan") then
		if (GetNumber(1, GetTask(TASK_DUNGCHUNG2), 9) == 0) then
			if GetLevel() >= 60 then
				Say("<npc>: §· l©u kh«ng gÆp! Mäi ng­êi ®Òu rÊt mong nhí! §¹i hiÖp d¹o nµy cã kháe kh«ng?", 5,
					"ChuyÓn ph¸i/doiphai", "XuÊt s­ xuèng nói/xuatsu", "T×m hiÓu khu vùc luyÖn c«ng/map_help",
					"T×m hiÓu vâ nghÖ bæn m«n/skill_help", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
			else
				Say("<npc>: §· l©u kh«ng gÆp! Mäi ng­êi ®Òu rÊt mong nhí! §¹i hiÖp d¹o nµy cã kháe kh«ng?", 3,
					"T×m hiÓu khu vùc luyÖn c«ng/map_help", "T×m hiÓu vâ nghÖ bæn m«n/skill_help",
					"Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
			end
		else
			Say("<npc>: §· l©u kh«ng gÆp! Mäi ng­êi ®Òu rÊt mong nhí! §¹i hiÖp d¹o nµy cã kháe kh«ng?", 4,
				"Trïng ph¶n m«n ph¸i/trungphansumon", "T×m hiÓu khu vùc luyÖn c«ng/map_help",
				"T×m hiÓu vâ nghÖ bæn m«n/skill_help", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		end
	else
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Xin chµo ®¹i hiÖp!")
		end
	end
end

function enroll_select()
	if (GetSeries() == 2) and (GetCamp() == 0) then
		if (GetLevel() >= 10) then
			Say("Cã høng thó gia nhËp Vò Hån kh«ng? Mäi ng­êi cã c¬m cïng ¨n, cã r­îu cïng uèng!", 2,
				"Gia NhËp/go", "§Ó ta suy nghÜ kü l¹i xem/thing")
		else
			Talk(1, "", "C¨n b¶n cña ng­¬i cßn kĞm l¾m! H·y ®i luyÖn tËp thªm, bao giê ®Õn cÊp 10 l¹i ®Õn t×m ta!")
		end
	end
end;

function go()
	gianhapmonphai(10)
end;

function thing()
	Talk(1, "", 10452)
end;

function doiphai()
	dk_chuyenphai(10)
end
