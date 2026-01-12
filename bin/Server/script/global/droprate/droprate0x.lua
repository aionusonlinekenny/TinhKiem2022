--Kinnox
Include("\\script\\global\\droprate\\DropMain.lua")
Include("\\script\\global\\droprate\\SourceDrop.lua")
Include("\\script\\lib\\worldlibrary.lua")
function DropRate(NpcIndex)
	local btype = GetNpcBoss(NpcIndex)
	local MONEY_0x = (RANDOM(30, 70)) * MONEY
	local nGenre = RANDOM(1, getn(KindItem)) --KindFiveElement[1]
	local nLevel = RANDOM(1, 2) -- level trang bi
	local nSeries = GetNpcSer(NpcIndex)
	local nLuck = GetLucky(0) + 50 --may man cua nguoi choi
	local nSel = RANDOM(0, 15) -- ti le roi do
	nDetail = KindItem[nGenre][1]
	nParti = RANDOM(KindItem[nGenre][2], KindItem[nGenre][3])
	----------------For blue boss-----------------------------------------------------------
	if (btype == 1) then --boss xanh
		dropnum = 5 --so' luong. cuc. do` roi
		for i = 1, dropnum do
			nSel = RANDOM(0, 20)
			if (nSel < 5) then
				DropMoney(NpcIndex, MONEYdrop)
			elseif (nSel == 8) then
				nItemIdx = AddItem(0, 6, 212, 0, 0, 5, 0, 0) -- mat chi
				SetObjPickExecute(DropItem(NpcIndex, nItemIdx), 1)
			elseif (nSel > 5) then
				nGenre = RANDOM(1, getn(KindItem))
				nLevel = RANDOM(1, 2)
				nSeries = GetNpcSer(NpcIndex)
				nLuck = GetLucky(0) + 50 -- xin hon quai thuong 1 diem may man;
				nNumLine = RANDOM(1, 2) -- so dong cua trang bi xanh
				nDetail = KindItem[nGenre][1]
				nParti = RANDOM(KindItem[nGenre][2], KindItem[nGenre][3])
				-- Msg2Player(""..nGenre.." - "..nDetail.." - "..nParti.." - "..nLevel.." - "..nSeries.." - "..nLuck.." - "..nNumLine.."  ");
				CalCulator_Drop(NpcIndex, nGenre, nDetail, nParti, nLevel, nSeries, nLuck, nNumLine)
			end
		end
		return
	end
	-----------------------------------------------------------------------------------------
	if (nSel > 6) then --khong rot
		return
	end
	-----------------------------------------------------------------------------------------
	if (nSel < 2) then
		DropMoney(NpcIndex, MONEYdrop)
	elseif (nSel == 5) then
		if (RANDOM(2) == 1) then
			nItemIdx = AddItem(0, 6, RANDOMC(205, 391), 0, 0, 5, 0, 0) -- than bi lo chi,thit tuoi
			SetObjPickExecute(DropItem(NpcIndex, nItemIdx), 1)
		end
	elseif (nSel == 4) then
		nRanItem = RANDOM(1, 30)
		if (nRanItem == 5) then
			nNumLine = RANDOM(1, 2) -- so dong cua trang bi xanh
			CalCulator_Drop(NpcIndex, nGenre, nDetail, nParti, nLevel, nSeries, nLuck, nNumLine)
		end
	end
end

function DropMenpai(NpcIndex)
	local nRanMenPai = RANDOM(1, 5)
	local tMenpai = RANDOM(1, getn(TAB_Menpai))
	local nMenPai = RANDOM(1, getn(TAB_Menpai[tMenpai]))
	local findMenPai = TAB_Menpai[tMenpai][nMenPai]
	if (nRanMenPai == 4) then
		DropMagicScript(NpcIndex, findMenPai)
	elseif (nRanMenPai == 5) then
		DropMagicScript(NpcIndex, 1126) -- Bi kip 120;
	else
		-- Khong rot;
	end
end
