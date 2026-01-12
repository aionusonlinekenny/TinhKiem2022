Include("\\script\\header\\factionhead.lua")
Include("\\script\\misc\\daiyitoushi\\change_faction.lua")

function main(nNpcIndex)
	if (nvthegioi(nNpcIndex) == 1) then
		return
	end;
	local player_Faction = GetFaction();
	if (player_Faction == "") then
		Talk(1, "enroll_select",
			"Gi¸o chñ cña bæn gi¸o lµ Hoµn Nhan Hïng LiÖt. Danh chÊn thiªn h¹! Hoµng ®Õ Kim quèc còng ph¶i nh­êng 3 phÇn. Cao thñ cña bæn gi¸o nhiÒu v« sè! Nh©n tµi ngäa hæ tµng long")
	elseif (player_Faction == "tianren") then
		if (GetNumber(1, GetTask(TASK_DUNGCHUNG2), 9) == 0) then
			if GetLevel() >= 60 then
				Say(
					"<npc>: Nghe nãi ng­¬i sau khi xuÊt s­ ®· lËp nªn nhiÒu c«ng danh! Qu¶ kh«ng hæ danh lµ ®Ö tö cña bæn ph¸i",
					5, "ChuyÓn ph¸i/doiphai", "XuÊt s­ xuèng nói/xuatsu", "T×m hiÓu khu vùc luyÖn c«ng/map_help",
					"T×m hiÓu vâ nghÖ bæn m«n/skill_help", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
			else
				Say(
					"<npc>: Nghe nãi ng­¬i sau khi xuÊt s­ ®· lËp nªn nhiÒu c«ng danh! Qu¶ kh«ng hæ danh lµ ®Ö tö cña bæn ph¸i",
					3, "T×m hiÓu khu vùc luyÖn c«ng/map_help", "T×m hiÓu vâ nghÖ bæn m«n/skill_help",
					"Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
			end
		else
			Say(
				"<npc>: Nghe nãi ng­¬i sau khi xuÊt s­ ®· lËp nªn nhiÒu c«ng danh! Qu¶ kh«ng hæ danh lµ ®Ö tö cña bæn ph¸i",
				4, "Trïng ph¶n m«n ph¸i/trungphansumon", "T×m hiÓu khu vùc luyÖn c«ng/map_help",
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
				"NÕu Nga My ph¸i chŞu gia nhËp bæn gi¸o, vËy th× bän ta suèt ngµy ®Òu ®­îc ng¾m nh×n ng­êi ®Ñp! ¤i! Sung s­íng chÕt ®i th«i!")
		end
	elseif (player_Faction == "cuiyan") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Sím ®· nghe con g¸i ph­¬ng Nam s¾c ®Ñp nghiªng thµnh, h«m nay ®­îc gÆp ®©y, qu¶ lµ tiÕng ®ån kh«ng ngoa")
		end
	elseif (player_Faction == "tangmen") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Gi¸o chñ cña bæn gi¸o lu«n cã lßng mong mái quı gi¸o cïng gia nhËp, vinh hoa phó quı, cao quan bæng léc, h­ëng c¶ ®êi kh«ng hÕt!")
		end
	elseif (player_Faction == "wudu") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Thiªn NhÉn vµ Ngò §«c gi¸o mµ hîp thµnh Nam c«ng B¾c kİch, Trung Nguyªn sím muén còng r¬i vµo tay chóng ta")
		end
	elseif (player_Faction == "tianwang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Kh«ng ai cã thÓ ng¨n c¶n ®­îc sù nghiÖp hïng b¸ Trung nguyªn cña Thiªn NhÉn gi¸o")
		end
	elseif (player_Faction == "shaolin") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Cao thñ ThiÕu L©m ph¸i cña c¸c ng­¬i ®· s¾p quy tiªn hÕt råi!Nh©n tµi trÎ tuæi l¹i ch¼ng cã mÊy ng­êi! Xem ra ThiÕu L©m còng ®· s¾p ®Õn ngµy tËn nghiÖp! Ha! Ha! Ha!")
		end
	elseif (player_Faction == "wudang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"Ta ®· nghe Néi c«ng, kiÕm thuËt cña Vâ §ang lõng danh Trung nguyªn! Cã c¬ héi nhÊt ®Şnh ph¶i thØnh gi¸o!")
		end
	elseif (player_Faction == "kunlun") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "",
				"C«n L«n ph¸i c¸c ng­¬i hïng cø T©y Vùc,cã d· t©m dßm ngã Trung nguyªn ®· l©u. SÏ cã  mét ngµy bæn gi¸o vµ c¸c ng­¬i ph©n chia cao thÊp t¹i Trung Nguyªn")
		end
	elseif (player_Faction == "gaibang") then
		if (GetTask(TSK_TOUSHI_FLAG) == 1) then
			Say("<npc>: §¸i nghÖ ®Çu s­, chuyÓn ®æi m«n ph¸i", 2,
				"B¸i nhËp m«n h¹/doiphai", "Xin chuyÓn dïm lêi hái th¨m s­ phô /no")
		else
			Talk(1, "", "Tªn ¨n mµy nµy muèn trµ trén vµo ®©y lµm gian tÕ ­?")
		end
	end
end

function enroll_select()
	if (GetSeries() == 3) and (GetCamp() == 0) then
		if (GetLevel() >= 10) then
			Say("Cã muèn gia nhËp bæn gi¸o kh«ng?", 2, "Gia nhËp Thiªn NhÉn gi¸o/go", "Kh«ng gia nhËp/thing")
		else
			Talk(1, "", "C¨n b¶n cña ng­¬i cßn kĞm l¾m! H·y ®i luyÖn tËp thªm, bao giê ®Õn cÊp 10 l¹i ®Õn t×m ta!")
		end
	end
end;

function go()
	gianhapmonphai(7)
end;

function thing()
	Talk(1, "", 10424)
end;

function doiphai()
	dk_chuyenphai(7)
end
