function OnDeath(nDesNpcIndex)
	local nPlayerDataIdx = PIdx2MSDIdx(1, PlayerIndex);
	--Msg2Player(nPlayerDataIdx)
	if (nPlayerDataIdx > 0) then
		SetPMParam(1, nPlayerDataIdx, 0, 0) --setoffline

		SetPunish(0)
		SetPKMode(0, 0)
		SetLogoutRV(0);
		ForbidUseTownP(0);
		SetDeathScript("")

		local szName = GetName();
		Msg2MSAll(1, format("%s kh«ng may tö vong.", szName)); --thong bao roi khoi~
		DelMSPlayer(1)
	end
end;
