Include("\\script\\header\\event_head.lua");
Include("\\script\\global\\droprate\\SourceDrop.lua");

function DropRate(NpcIndex)
	local nBoss = GetNpcBoss(NpcIndex);
	if (nBoss == 0) then
		if (random(1, 10) < 6) then
			return
		end
		DropItem(NpcIndex, AddItem(0, 1, 2, 0, 5, 5, 0, 0))
	else --boss
		--DropItem(NpcIndex, AddItem(0, 6, 3361, 0, 0, 5, 0, 0)) --bao ruong
		--DropItem(NpcIndex, AddItem(0, 6, 3361, 0, 0, 5, 0, 0)) --bao ruong
		AddOwnExp(50000000)
		--------add xu----------
		local nNewItem = AddStackItem(5, 6, 4814, 0, 0, 0, 0, 0) 
		--LockItem(nNewItem, -2);
		Msg2Player("NhËn ®­îc <color=yellow>5 LÔ Hép Trang BÞ<color>");
		------------------------
		if (RANDOM(50) <= 5) then
			DropMagicScript(NpcIndex, 397);
		end
		
		DropEventPLD(NpcIndex)
	end
end
