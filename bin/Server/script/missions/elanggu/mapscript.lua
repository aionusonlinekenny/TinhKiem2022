--Include("\\script\\maps\\newworldscript_default.lua")

function OnNewWorld(szParam)
	local nXu = GetItemCount(0, 4, 417, -1, -1)
	if (nXu < 1000) then
		NewWorld(78, 1569, 3234)
		SetFightState(0)
		Talk(1, "",
			"Ng≠¨i kh´ng mang ÆÒ ti“n rÂi, c«n ph∂i c„ <color=yellow>1000 Ti“n ÆÂng<color> Æ” c„ th” tu luy÷n 1 giÍ!")
		--return
	else
		ConsumeItem(3, 1000, 4, 417, -1, -1)
		SetTimer(60 * 60 * 18, 5);

		SetFightState(1)
		SetPKMode(0)
		ForbidEnmity(1);

		ForbidUseTownP(1)
		ForbidChangePK(1)
		SetCurCamp(1);
	end

	--OnNewWorldDefault(szParam)
end

function OnLeaveWorld(szParam)
	SetFightState(0)
	ForbidEnmity(0);
	SetPKMode(0)
	ForbidUseTownP(0)
	ForbidChangePK(0)

	camp = GetCamp()
	SetCurCamp(camp)
	StopTimer(5)
	--OnLeaveWorldDefault(szParam)
end
