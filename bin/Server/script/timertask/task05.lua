Include("\\script\\lib\\worldlibrary.lua")
Include("\\script\\header\\loidai.lua");
function OnTimer()
	local world, x, y = GetWorldPos()
		
	if(world == 950) then
		--NewWorld(78, 1569, 3234)
		local nXu = CalcEquiproomItemCount(4, 417, 1, -1)
		if(nXu < 1000) then
			Msg2Player("§· hÕt thêi gian train!")
			NewWorld(78, 1569, 3234)
			Talk(1,"","Ng­¬i kh«ng mang ®ñ tiÒn råi, cÇn ph¶i cã <color=yellow>1000 TiÒn ®ång<color> ®Ó cã thÓ tu luyÖn 1 giê!")
			StopTimer();
			--return
		else
			ConsumeEquiproomItem(1000, 4, 417, 1, -1)
			SetTimer(60*60*18,5);
			Msg2Player("HÖ thèng tù ®éng gia h¹n thêi gian tu luyÖn ¸c Lang Cèc thªm 1 giê!")
		end
	end
end;

function OnMissionTimer(nIndex)
	StopMissionTimer(nIndex, 5);
end;
