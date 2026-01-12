--Kinnox
Include("\\script\\global\\droprate\\DropMain.lua");
Include("\\script\\global\\droprate\\SourceDrop.lua");
Include("\\script\\lib\\worldlibrary.lua");
Include("\\script\\header\\event_head.lua");
function DropRate(NpcIndex)
	Msg2SubWorld("<color=green> §¹i hiÖp " .. GetName() .. " ®· tiªu diÖt thµnh c«ng 1 boss tiÓu hoµng kim");
	local MONEYdrop = (random(5000, 7500)) * MONEY;
	local EXP       = 3000000;
	nDropNum        = 8; -- so mon do roi ra;

	for i = 1, nDropNum do
		local nGenre  = random(1, getn(KindItem)); --KindFiveElement[1]
		local nLevel  = random(9, 10);       -- level trang bi
		local nSeries = GetNpcSer(NpcIndex);
		local nLuck   = GetLucky(0) + 10;    --may man cua nguoi choi +4 diem may man;
		nNumLine      = 6;                   -- so dong cua trang bi xanh
		nDetail       = KindItem[nGenre][1];
		nParti        = random(KindItem[nGenre][2], KindItem[nGenre][3]);
		CalCulator_Drop(NpcIndex, nGenre, nDetail, nParti, nLevel, nSeries, nLuck, nNumLine);
	end

	---------------------------------------------------------------------------
	----------------------------- DROP MANH HOANG KIM ---------------------------
	---------------------------------------------------------------------------
	if (RANDOM(1000) <= 10) then
		--local nSeries = GetNpcSer(NpcIndex);
		--local szNpcName = GetNpcName(NpcIndex)
		--if(nSeries == 0) then
		--	if(szNpcName == "HuyÒn Nan §¹i S­" or szNpcName == "HuyÒn Gi¸c §¹i S­ ") then
		--DropQuestKey(NpcIndex, RANDOM(567, 942))
		local nIdx = AddItem(2, 0, RANDOM(1, 147) - 1, 0, 0, 0, 10, 0)
		DropItem(NpcIndex, nIdx)
		--	end
		--end
	end

	---------------------------------------------------------------------------
	----------------------------- DROP SPECIAL ITEM ---------------------------
	---------------------------------------------------------------------------
	for i = 1, random(2, 4) do -- 2->4 mon do
		DropSpecialItem(NpcIndex);
	end
	---------------------------------------------------------------------------
	----------------------------- DROP EVENT  ---------------------------------
	---------------------------------------------------------------------------
	DropEvent(NpcIndex)
end;

function DropSpecialItem(NpcIndex)
	local nSel = RANDOM(1, 15);                  -- ti le roi do
	if (nSel == 1) then
		DropMagicScript(NpcIndex, 26);           -- vo lam mat tich;
	elseif (nSel == 2) then
		DropMagicScript(NpcIndex, 22);           -- Tay tuy kinh;
	elseif (nSel == 3) then
		DropMagicScript(NpcIndex, 71);           -- Tien thao lo;
	elseif (nSel == 4) then
		DropMagicScript(NpcIndex, 72);           -- Que Hoa Tuu ;
	elseif (nSel == 5) then
		DropMagicScript(NpcIndex, 73);           -- Bach Qua Lo ;
	elseif (nSel == 6) then
		DropMagicScript(NpcIndex, 1182);         -- Tien thao lo dac biet ;
	elseif (nSel == 7) then
		DropQuestKey(NpcIndex, (RANDOM(238, 240))); -- thuy tinh;
	elseif (nSel == 8) then
		DropQuestKey(NpcIndex, 353);             -- tinh hong bao thach;
	elseif (nSel == 9) then
		DropMagicScript(NpcIndex, (RANDOM(121, 123))); -- Phuc duyen;
	elseif (nSel == 10) then
		DropMagicScript(NpcIndex, 1314);         -- BK 120;
	elseif (nSel == 11) then
		DropMagicScript(NpcIndex, 1313);         -- BK 150;
	else
		DropMagicScript(NpcIndex, 1315);         -- Tien Dong;
	end
end
