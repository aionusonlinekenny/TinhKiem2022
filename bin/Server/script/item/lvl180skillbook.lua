Include("\\script\\header\\factionhead.lua")


function main(sel)
	if GetFactionNo() == -1 then
		Msg2Player("<color=yellow>Ch­a gia nhËp m«n ph¸i, kh«ng thÓ häc kü n¨ng nµy!<color>")
		return 1
	end

	if GetLevel() < 180 then
		Msg2Player("<color=yellow>§¼ng cÊp ch­a ®ñ 180, kh«ng thÓ häc kü n¨ng nµy!<color>")
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
	local nList = SKILL180_ARRAY[nFaction + 1]
	for i = 1, getn(nList) do
		if (nList[i][3] > 0) then
			--Msg2Player(nList[i][6] .. "-" .. nList[i][1] .. "-" .. nList[i][2])
			if (GetLevel() < 180) then
				Msg2Player("B¹n cÇm lÊy <color=yellow>Bİ kİp Kü n¨ng cÊp 180<color> nghiªn cøu c¶ nöa ngµy, kÕt qu¶ kh«ng lÜnh ngé ®­îc g×. ")
				return 1
			end
			if (HaveMagic(nList[i][2]) <=0) then
				AddMagic(nList[i][2], 1)
				Msg2Player("§· häc ®­îc kü n¨ng <color=yellow>" .. nList[i][1] .. "<color>. ")
			else
				Msg2Player("B¹n ®· cÇm <color=yellow>Bİ kİp Kü n¨ng cÊp 180<color> nghiÒn tíi ngÉm lui, nh­ng còng kh«ng häc ®­îc g× trong ®ã ")
                return 1
			end
		end
	end

	return 0
end
