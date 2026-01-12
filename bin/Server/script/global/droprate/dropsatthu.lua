--Kinnox
Include("\\script\\header\\taskid.lua");
Include("\\script\\header\\event_head.lua");
Include("\\script\\global\\droprate\\DropMain.lua");
Include("\\script\\global\\droprate\\SourceDrop.lua");
Include("\\script\\lib\\worldlibrary.lua");
SATTHU_EXP = 150000;
function DropRate(NpcIndex)
	local nValue  = GetTask(TASK_NVST);
	local nNum    = GetNumber(2, nValue, 1);
	local nGenre  = RANDOM(1, getn(KindItem)); --KindFiveElement[1]
	local nLevel  = RANDOM(1, 2);           -- level trang bi
	local nSeries = GetNpcSer(NpcIndex);
	local nLuck   = GetLucky(0);            --may man cua nguoi choi
	local nSel    = RANDOM(0, 20);          -- ti le roi do
	nDetail       = KindItem[nGenre][1];
	nParti        = RANDOM(KindItem[nGenre][2], KindItem[nGenre][3]);

	if (nNum == GetNpcValue(NpcIndex)) then
		---------------------------------
		dropnum = 8; --so' luong. cuc. do` roi
		--phan rot vat pham trang bi-----------------
		for i = 1, dropnum do
			nGenre   = RANDOM(1, getn(KindItem));
			nLevel   = RANDOM(8, 10);
			nSeries  = GetNpcSer(NpcIndex);
			nLuck    = GetLucky(0) + 10; -- xin hon quai thuong 1 diem may man;
			nNumLine = RANDOM(4, 6); -- so dong cua trang bi xanh
			nDetail  = KindItem[nGenre][1];
			nParti   = RANDOM(KindItem[nGenre][2], KindItem[nGenre][3]);
			CalCulator_Drop(NpcIndex, nGenre, nDetail, nParti, nLevel, nSeries, nLuck, nNumLine);
		end

		nValue = SetNumber(2, nValue, 1, 0);
		SetTask(TASK_NVST, nValue);
		nValue = GetTask(TASK_RESET);
		local nTimes = GetNumber(2, nValue, 4);
		nValue = SetNumber(2, nValue, 4, nTimes + 1);
		SetTask(TASK_RESET, nValue);
		--Add them gi thi bo vao day lun
		AddOwnExp(SATTHU_EXP);
		AddItemIDStack(AddItem(0, 6, 398, 0, 0, nSeries, 0, 0)); --add theo he ngu hanh;
		AddSkillState(541, 1, 0, 54);
		Msg2Player("B¹n ®· thu phôc ®­îc tªn s¸t thñ, hoµn thµnh nhiÖm vô!");

		-- local nXu = RANDOM(5,10)
		-- AddStackItem(nXu,4,417,0,0,5,0,0)
		-- Msg2Player(format("NhËn ®­îc <color=yellow>%d<color> TiÒn ®ång!", nXu));

		--phan tang vat pham trang bi------9 nhiem vu----------	
		if (nTimes == 9) then
			if (CheckRoom(2, 2) == 1) then
				for i = 1, 2 do
					local nNewItem = {};
					nNewItem[i] = AddItemID(AddItem(0, 6, 71, 0, 0, 5, 0, 0)) -- Tien thao lo
					LockItem(nNewItem[i]);
				end
				Msg2Player("B¹n nhËn ®­îc phÇn th­ëng s¸t thñ.");
			end
		end

		---------------------------------------------------------------------------
		----------------------------- DROP MANH HOANG KIM ---------------------------
		---------------------------------------------------------------------------
		if (RANDOM(200) <= 2) then
			--local nSeries = GetNpcSer(NpcIndex);
			--local szNpcName = GetNpcName(NpcIndex)
			--if(nSeries == 0) then
			--	if(szNpcName == "HuyÒn Nan §¹i S­" or szNpcName == "HuyÒn Gi¸c §¹i S­ ") then
			local nIdx = AddItem(2, 0, RANDOM(159, 167), 0, 0, 0, RANDOM(1,10), 0)
			DropItem(NpcIndex, nIdx)
			--DropQuestKey(NpcIndex, RANDOM(927, 942))
			--	end
			--end
		end

		---------------------------------
		--phan rot vat pham dac biet--
		if (nSel < 5) then return end;
		if (nSel == 6) then -- rot bi kip
			DropMenpai(NpcIndex);
		elseif (nSel == 7) then
			DropQuestKey(NpcIndex, (RANDOM(238, 240))); -- thuy tinh;
			--DropQuestKey(NpcIndex, (RANDOM(238, 240))); -- thuy tinh;
			--DropQuestKey(NpcIndex, (RANDOM(238, 240))); -- thuy tinh;
		elseif (nSel == 8) then
			DropQuestKey(NpcIndex, 353);          -- tinh hong bao thach;
		elseif (nSel == 9) then
			DropMagicScript(NpcIndex, (RANDOM(121, 123))); -- phuc duyen;
		else
			-- Khong rot;
		end

		--if(RANDOM(15) >= 10 )then
		DropMagicScript(NpcIndex, 4814);
		--end

		if (RANDOM(1000) == 13) then
			DropMagicScript(NpcIndex, 4813);
		end

		-- for i = 1, 3 do
		-- 	DropQuestKey(NpcIndex, (RANDOM(238, 240))); -- thuy tinh;
		-- end
		--event----------------
		DropEventST()
		-----------------------------
	end
end

function DropMenpai(NpcIndex)
	local nRanMenPai = RANDOM(1, 5);
	local tMenpai = RANDOM(1, getn(TAB_Menpai));
	local nMenPai = RANDOM(1, getn(TAB_Menpai[tMenpai]));
	local findMenPai = TAB_Menpai[tMenpai][nMenPai];
	if (nRanMenPai == 4) then
		DropMagicScript(NpcIndex, findMenPai);
	elseif (nSel == 3) then
		DropMagicScript(NpcIndex, 1314); -- BK 120;
	elseif (nSel == 2) then
		DropMagicScript(NpcIndex, 1313); -- BK 150;
	elseif (nSel == 5) then
		DropMagicScript(NpcIndex, 1315); -- Tien Dong;
	else
		DropMagicScript(NpcIndex, 1126); -- Bi kip 120;
	end
end
