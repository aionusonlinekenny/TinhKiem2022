function OnDeath(nDesNpcIndex)
	local nPlayerDataIdx = PIdx2MSDIdx(1, PlayerIndex);
	if (nPlayerDataIdx > 0) then
		SetPMParam(1, nPlayerDataIdx, 0, 0) --setoffline
		DelMSPlayer(1)
	end
	
	SetPunish(0)
	RestoreCamp()
	SetLogoutRV(0);
	ForbidUseTownP(0);
	SetDeathScript("")

	local szName = GetName()
	Msg2MSAll(1, format("%s tö vong.", szName)); --thong bao roi khoi~
end;
