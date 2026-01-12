--nguoi choi danh quai' cuoi cung` khi chet
Include("\\script\\header\\taskid.lua");
Include("\\script\\header\\event_head.lua");
Include("\\script\\global\\droprate\\SourceDrop.lua");

THANHTHI_RETURN = {
	{ 1,   1510, 3188 },
	{ 11,  3214, 4986 },
	{ 162, 1587, 3228 },
	{ 37,  1638, 3051 },
	{ 78,  1514, 3216 },
	{ 80,  1697, 2950 },
	{ 176, 1372, 2996 }
};

function LastDamage(NpcIndex)
	--tinh kinh nghiem cho toan doi
	local nOldPlayer = PlayerIndex; --nguoi giet quai
	local nCount = GetMSPlayerCount(1);
	local szName = GetMissionS(0);

	--x - floor(x/y)
	if (szName ~= " ") then
		local nRestTime = GetMSRestTime(1, 2);
		local nRemainSec = 30 * 60 - floor(nRestTime / 18);
		local nMin = floor(nRemainSec / 60);
		nRemainSec = nRemainSec - nMin * 60;
		AddCountNews(
			format("§éi ngò %s ®· hoµn thµnh nhiÖm vô ®óng thêi gian, sö dông: %d phót %d gi©y", szName, nMin, nRemainSec),
			3);
		PlayerIndex = FindPlayer(szName);
		if (PlayerIndex > 0) then
			SetValue(10, nRestTime / 18);
		end;
		PlayerIndex = nOldPlayer;
	end

	for i = 1, nCount do
		if (GetPMParam(1, i, 0) == 1) then --dang online thi tinh diem
			PlayerIndex = MSDIdx2PIdx(1, i);
			if (PlayerIndex == nOldPlayer) then
				AddOwnExp(16000000)
			else
				AddOwnExp(floor(8000000 * 50 / 100))
			end
			SetCurCamp(GetCamp())
			SetFightState(0)
			SetDeathScript("")
			SetPunish(0)
			local nTask = GetNumber(1, GetTask(TASK_NVST), 9);
			NewWorld(THANHTHI_RETURN[nTask][1],
				THANHTHI_RETURN[nTask][2], THANHTHI_RETURN[nTask][3])
			--AddItem
			AddItemID(AddItem(0, 6, 1401, 0, 0, 5, 0, 0));
			if (CheckRoom(1, 1) == 1) then
				local TTL = AddItem(0, 6, 71, 0, 0, 5, 0)
				LockItem(TTL)
				AddItemID(TTL)
			end

			--------Add Event Item ----------
			DropEventVA()
			---------------------------------

			if (RANDOM(50) <= 5) then
				DropMagicScript(NpcIndex, 397);
			end
			--------add xu----------
			local nNewItem = AddStackItem(2, 6, 4814, 0, 0, 0, 0, 0) --tien dong 50 cai
			--LockItem(nNewItem, -2);
			Msg2Player("NhËn ®­îc <color=yellow>2 LÔ Hép Trang BÞ<color>");
			------------------------
		end
	end

	SetMission(0, 0);
	SetMission(1, 0);
	CloseMission(1)
end

--khi hoi sinh
function Revive(NpcIndex)
	SetNpcSkill(NpcIndex, 165, 20, 1)
	SetNpcSkill(NpcIndex, 165, 20, 2)
	SetNpcSkill(NpcIndex, 165, 20, 3)
	SetNpcSkill(NpcIndex, 165, 20, 4)

	SetNpcDmgEx(NpcIndex, 0, 0, 0, 0, 50, 0);
end

--Khi chet
function DeathSelf(NpcIndex)
end;
