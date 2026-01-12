Include("\\script\\lib\\worldlibrary.lua")
Include("\\script\\header\\tongkim.lua");
Include("\\script\\header\\taskid.lua");
Include("\\script\\header\\event_head.lua");
Include("\\script\\header\\phonglangdo.lua");

function OnTimer()
	StopTimer();
end;

function OnMissionTimer(nIndex)
	StopMissionTimer(nIndex, 3);
	if (SubWorld == SubWorldID2Idx(380)) then
		local nCount = GetMSPlayerCount(1);
		local nTongAcc, nKimAcc, nPlayerCamp = 0, 0, 0;
		local i;
		for i = 1, nCount do
			if (GetMSIdxGroup(1, i) == 0) then
				nTongAcc = nTongAcc + GetPMParam(1, i, 2);
			else
				nKimAcc = nKimAcc + GetPMParam(1, i, 2);
			end
		end
		if (nTongAcc > nKimAcc) then
			nPlayerCamp = 1;
		elseif (nTongAcc < nKimAcc) then
			nPlayerCamp = 2;
		else
			nPlayerCamp = 0;
		end

		--Msg2SubWorld("<color=cyan>Th≠Îng trÀn TËng Kim nµy:")
		-- local szPlayerName;
		-- local nTotalRank = 10

		-- if nTotalRank > nCount then
		-- 	nTotalRank = nCount
		-- end

		-- for i = 1, nTotalRank do
		-- 	szPlayerName = GetMSLadder(1, i)
		-- 	for j = 1, nCount do
		-- 		PlayerIndex = MSDIdx2PIdx(1, j);
		-- 		local nRank = i

		-- 		-----------------------------
		-- 		if (GetName() == szPlayerName and GetPMParam(1, i, 0) == 1) then
		-- 			AddOwnExp(100000000)
		-- 			if (nRank == 1) then
		-- 				AddItemEx(6, 1182, 0, 0, 5, 0, 0)
		-- 				AddItemEx(6, 1182, 0, 0, 5, 0, 0)
		-- 				AddItemEx(6, 1182, 0, 0, 5, 0, 0)
		-- 				-----------------------------
		-- 				-- Msg2SubWorld("<color=yellow>Hπng " ..
		-- 				-- 	nRank ..
		-- 				-- 	":<color=red> " ..
		-- 				-- 	GetName() .. " <color> nhÀn 3 TTL ßB & <color=yellow>100 tri÷u<color> Æi”m kinh nghi÷m!")
		-- 				-----------------------------
		-- 			else
		-- 				AddItemEx(6, 1182, 0, 0, 5, 0, 0)
		-- 				-----------------------------
		-- 				-- Msg2SubWorld("<color=yellow>Hπng " ..
		-- 				-- 	nRank ..
		-- 				-- 	":<color=red> " ..
		-- 				-- 	GetName() .. " <color> nhÀn 1 TTL ßB & <color=yellow>100 tri÷u<color> Æi”m kinh nghi÷m")
		-- 				-----------------------------
		-- 			end
		-- 		end
		-- 	end
		-- end

		for i = 1, nCount do
			if (GetPMParam(1, i, 0) == 1) then --dang online thi tinh diem
				PlayerIndex = MSDIdx2PIdx(1, i);
				SetPKMode(0, 0);      --phuc hoi pk tu do
				SetFightState(0);     --phi chien dau
				SetPunish(0);
				ForbidUseTownP(0);
				ForbidParty(0)
				RestoreRankEx();
				local nTotalAccum = GetPMParam(1, i, 2);
				local nCamp = GetCurCamp();
				if (nPlayerCamp == nCamp) then --nam trong phe chien thang, thuong them tich luy
					local nAccum = TICHLUY_THUONG * GetPMParam(1, i, 15);
					nTotalAccum = nTotalAccum + nAccum;
					Talk(1, "",
						"Xin chÛc mıng! Bπn nhÀn Æ≠Óc ph«n th≠Îng <color=yellow>" .. nAccum .. "<color> Æi”m t›ch lÚy");
					AddItemID(AddItem(0, 6, 11, 0, 0, 5, 0, 0)) --phao bong

					if(nTotalAccum >= 3000) then
						AddStackItem(100, 6, 4814, 0, 0, 0, 0, 0)
						Msg2Player("NhÀn Æ≠Óc <color=yellow>100 L‘ HÈp Trang Bﬁ<color>");
						AddItemEx(6, 1182, 0, 0, 5, 0, 0)
						AddStackExp(50000000)
					end
				else
					if(nTotalAccum >= 1500) then
						AddStackItem(50, 6, 4814, 0, 0, 0, 0, 0)
						Msg2Player("NhÀn Æ≠Óc <color=yellow>50 L‘ HÈp Trang Bﬁ<color>");
						AddItemEx(6, 1182, 0, 0, 5, 0, 0)
						AddStackExp(10000000)
					end
				end
				if (nCamp == 1) then --tong
					AddValue(4, nTotalAccum);
				else     --kim
					AddValue(5, nTotalAccum);
				end
				AddAccum(nTotalAccum);
				if (nCamp == 1) then --tong
					NewWorld(325, RANDOM(1537, 1548), RANDOM(3160, 3195));
					SetRevPos(325, 1)
				else --kim
					NewWorld(325, RANDOM(1566, 1588), RANDOM(3079, 3096));
					SetRevPos(325, 2)
				end
				SetDeathScript("") --set script khi chet
				SetReviveNow(0);
				SetCurCamp(GetCamp()); --phuc hoi phe ban dau
				--event----------------
				DropEventTK()
				--end------------------
			end
		end
		if (nPlayerCamp == 1) then
			AddNews(format("ßπi chi’n TËng Kim Æ∑ k’t thÛc. T›ch lÚy TËng %d:%d Kim. Phe TËng giµnh Æ≠Óc thæng lÓi!",
				nTongAcc, nKimAcc));
		elseif (nPlayerCamp == 2) then
			AddNews(format("ßπi chi’n TËng Kim Æ∑ k’t thÛc. T›ch lÚy TËng %d:%d Kim. Phe Kim giµnh Æ≠Óc thæng lÓi!",
				nTongAcc, nKimAcc));
		else
			AddNews(format("ßπi chi’n TËng Kim Æ∑ k’t thÛc. T›ch lÚy TËng %d:%d Kim. TrÀn nµy Æ∑ hﬂa!", nTongAcc, nKimAcc));
		end
		for i = 0, 21 do
			SetMission(i, 0);
		end
		CloseMission(1); --dong mission
	else
		for i = 1, getn(MAP_PHONGLANGDO) do
			if (SubWorld == SubWorldID2Idx(MAP_PHONGLANGDO[i])) then
				ontime_phonglangdo(nIndex)
				return
			end
		end
	end
end;

function ontime_phonglangdo(nIndex)
	StopMissionTimer(nIndex, 3);
	local nCount = GetMSPlayerCount(1);
	for i = 1, nCount do
		if (GetPMParam(1, i, 0) == 1) then --dang online
			PlayerIndex = MSDIdx2PIdx(1, i);
			SetDeathScript("")       --set script khi chet
			SetLogoutRV(0);
			SetPKMode(0, 0);         --phuc hoi pk tu do
			SetPunish(0);
			NewWorld(336, RANDOM(1333, 1349), RANDOM(2850, 2861));
			SetTask(TASK_DUNGCHUNG, SetNumber(1, GetTask(TASK_DUNGCHUNG), 2, 1));
		end
	end
	CloseMission(1); --dong mission
end;
