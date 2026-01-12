Include( "\\script\\feature\\npcevent\\batnhahead.lua" )
function main(id)
	local dwid = GetNpcValue(id);
	if(GetUUID() == dwid) then
	local nPassTime = 2700 - GetNpcLifeTime(id);
	if(nPassTime > 2160) then	--qua 2 phut
	SetTaskTemp(NPCINDEX, id);
	Say("§· ®Õn giai ®o¹n ch¨m sãc <color=green>thø 4<color>. H·y t­íi n­íc cho c©y!",2,
	"T­íi n­íc (cÇn 1 thïng n­íc)/tuoinuoc",
	"Tho¸t/no")
	else
	Talk(1,"","H·y chê ®îi! Sau "..(floor((2160-nPassTime)/18)+1).." gi©y n÷a míi cã thÓ t­íi n­íc <color=green>lÇn 4")
	end
	else	--cuop
		if(GetFightState() ~= 1) then
		Msg2Player("Kh«ng thÓ c­íp c©y B¸t Nh· trong tr¹ng th¸i phi chiÕn ®Êu!")
		return end
		if(GetLevel() < 120) then
		Talk(1,"","§¼ng cÊp tõ 120 trë lªn míi cã thÓ c­íp c©y B¸t Nh·!")
		return end
		if(GetFactionNo() < 0) then
		Talk(1,"","B¹n ph¶i gia nhËp m«n ph¸i míi cã thÓ c­íp c©y B¸t Nh·!")
		return end
		local nPlayer = FindPlayer(dwid);
		local tempid = PlayerIndex;
		if(nPlayer > 0) then
		PlayerIndex = nPlayer
		local nNpcIdx = GetPlayerNpcIdx()
		PlayerIndex = tempid
		nNpcIdx = FindNearNpc(GetNpcID(nNpcIdx))
		if(nNpcIdx > 0) then
		Msg2Player("C©y B¸t Nh· thuéc së h÷u cña ng­êi kh¸c. Muèn ®o¹t lÊy b¹n ph¶i h¹ ®­îc ng­êi ®ã.")
		return end
		end
		local z,w,x,y = FindNpc(GetNpcID(id));
		DelNpc(id);
		local newid = AddNpc(1532,1,SubWorldID2Idx(w),x,y,0);
		SetNpcName(newid, "B¸t Nh·-"..GetName());
		SetNpcCurCamp(newid, 6);
		SetNpcLifeTime(newid,2700);--thoi gian cay mat > 2 phut
		SetNpcValue(newid, GetUUID());
		SetNpcScript(newid, "\\script\\feature\\npcevent\\caybatnha4.lua");
		Msg2Player("B¹n ®· ®o¹t ®­îc 1 c©y B¸t Nh·. H·y tiÕp tôc ch¨m sãc t­íi n­íc.")
	end
end

function tuoinuoc()
	if(GetFightState() ~= 1) then
	Msg2Player("B¹n ®ang trong tr¹ng th¸i phi chiÕn ®Êu! Kh«ng thÓ t­íi n­íc!")
	return end
	if(GetItemCount(THUNGNUOC_DETAIL,5) < 1) then
	Talk(1,"","B¹n cÇn cã 1 Thïng n­íc ®Ó t­íi c©y!")
	return end
	
	local dwNID = GetNpcID(GetTaskTemp(NPCINDEX));
	if(FindNearNpc(dwNID) == 0) then
	return end
	DelItem(THUNGNUOC_DETAIL,5,1);
	AddOwnExp(BATNHA_EXP4);
	Msg2Player("C©y ®· ph¸t triÓn ®Õn mét giai ®o¹n míi. Xin chóc mõng!")
	local nNpcIdx,w,x,y = FindNpc(dwNID);
	if(nNpcIdx > 0) then
	DelNpc(nNpcIdx);
	local id = AddNpc(1533,1,SubWorldID2Idx(w),x,y,0);
	SetNpcName(id, "B¸t Nh·-"..GetName());
	SetNpcCurCamp(id, 6);
	SetNpcLifeTime(id,2700);--thoi gian cay mat > 2 phut
	SetNpcValue(id, GetUUID());
	SetNpcScript(id, "\\script\\feature\\npcevent\\caybatnha5.lua");
	else return end
	--cac thanh vien khac trong nhom' neu co'
	local tempid = PlayerIndex;
	local nTeamId = GetTeam();
	for i=0,7 do
	local nPIndex = GetTeamMem(nTeamId, i );
		if(nPIndex > 0 and nPIndex ~= tempid) then
			PlayerIndex = nPIndex;
			nNpcIdx = GetPlayerNpcIdx()
			PlayerIndex = tempid
			nNpcIdx = FindNearNpc(GetNpcID(nNpcIdx)) --dung gan` cay nhan 1/2 kn
			if(nNpcIdx > 0) then
			PlayerIndex = nPIndex;
			AddOwnExp(floor(BATNHA_EXP4/2));
			Msg2Player("B¹n nhËn ®­îc "..floor(BATNHA_EXP4/2).." ®iÓm kinh nghiÖm tõ c©y B¸t Nh·.")
			end
		end
	end
end;

function no()
end;
