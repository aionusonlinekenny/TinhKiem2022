---Script by Manh Dat
---Tho Lam Phao
Include("\\script\\header\\event_head.lua")
Include("\\script\\library\\worldlibrary.lua")
Include("\\script\\header\\taskid.lua")
END_TALK = "K’t thÛc ÆËi thoπi/no"
MAX_RESTTIME = 1
EVENT_YEAR = 2024
EVENT_MONTH = 7
EVENT_DAY = 31

function main(NpcIndex)
	if (IsEventActive() == 1) then
		Say(
			"<color=red>B’p Lˆa Ch∏y LÌn<color>: s˘ ki÷n <color=blue>B∏nh Ch≠ng Ngµy T’t<color> do ta phÙ tr∏ch, ngµi t◊m ta c„ vi÷c g◊?",
			2,
			"Hoπt ÆÈng G„i b∏nh ch≠ng/BanhChung",
			END_TALK
		)
	end
end

function BanhChung()
	local nTaskTH = GetTask(TASK_BANHCHUNGTH)
	local nTaskHH = GetTask(TASK_BANHCHUNGHH)
	local TAB_MSG = {
		"<color=red>B’p Lˆa Ch∏y LÌn<color>: hoπt ÆÈng <color=blue>B∏nh Ch≠ng Ngµy T’t<color> di‘n ra tı ngµy <color=blue>10-03-2024 Æ’n 30-04-2024<color>, tham gia s˘ ki÷n ÆÂng Æπo sœ nhÀn r t nhi“u kinh nghi÷m vµ vÀt ph»m gi∏ trﬁ.\n\n<color=cyan>B∏nh ch≠ng th≠Óng hπng<color>\t\t\t\t\t\t\t\t\t" ..
			nTaskTH .. "\n<color=red>B∏nh ch≠ng h∂o hπng<color>\t\t\t\t\t\t\t\t\t" .. nTaskHH .. "",
		"N u b∏nh ch≠ng th≠Óng hπng/ghep2",
		"N u b∏nh ch≠ng h∂o hπng/ghep3"
	}
	Say(
		TAB_MSG[1],
		3,
		TAB_MSG[2],
		--TAB_MSG[3],
		END_TALK
	)
end

function ghep2()
	local nTaskTime = GetTask(TASK_THOIGIAN5)
	local nCurTime = GetTimeMin()
	if (CheckRoom(2, 1) == 0) then
		Talk(1, "", "Vui lﬂng chıa ›t nh t 2 ´ trËng.")
		return
	end
	-- if (nCurTime - nTaskTime < MAX_RESTTIME) then
	-- 	Talk(1, "", "ThÍi gian lµm b∏nh c∏ch nhau <color=yellow>" .. MAX_RESTTIME .. " phÛt<color>.")
	-- 	return
	-- end
	Input("GhepEvent2", 500)
end

function ghep3()
	local nTaskTime = GetTask(TASK_THOIGIAN5)
	local nCurTime = GetTimeMin()
	if (CheckRoom(2, 1) == 0) then
		Talk(1, "", "Vui lﬂng chıa ›t nh t 2 ´ trËng.")
		return
	end
	-- if (nCurTime - nTaskTime < MAX_RESTTIME) then
	-- 	Talk(1, "", "ThÍi gian lµm b∏nh c∏ch nhau <color=yellow>" .. MAX_RESTTIME .. " phÛt<color>.")
	-- 	return
	-- end
	Input("GhepEvent3", 500)
end

function GhepEvent2(num)
	local nNumber = tonumber(num)
	if (nNumber < 1) then
		Talk(1, "", "MÁi l«n lµm tËi thi”u <color=yelow>1 vÀt ph»m<color>.")
		return
	end

	if (nNumber > 500) then
		Talk(1, "", "MÁi l«n chÿ lµm tËi Æa <color=yelow>500 vÀt ph»m<color> mµ th´i.")
		return
	end

	EVENT = {
		{1662, "L∏ b∏nh"},
		{1663, "Gπo n’p"},
		{1664, "ßÀu xanh"},
		{1665, "Thﬁt heo"},
		{1669, "B› quy’t lµm b∏nh ch≠ng th≠Óng hπng"},
		{1671, "B∏nh ch≠ng th≠Óng hπng"}
	}

	KiemTraNL1 = GetItemCount(0, 6, EVENT[1][1])
	KiemTraNL2 = GetItemCount(0, 6, EVENT[2][1])
	KiemTraNL3 = GetItemCount(0, 6, EVENT[3][1])
	KiemTraNL4 = GetItemCount(0, 6, EVENT[4][1])
	KiemTraNL5 = GetItemCount(0, 6, EVENT[5][1])
	LaBanh = nNumber * 1
	GaoNep = nNumber * 1
	DauXanh = nNumber * 1
	ThitHeo = nNumber * 1
	BiQuyet = nNumber * 1
	TienYC = nNumber * 20000

	if (KiemTraNL1 <= 0 or KiemTraNL1 < LaBanh) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL1 .. "/" .. LaBanh .. "<color> <color=pink>" .. EVENT[1][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (KiemTraNL2 <= 0 or KiemTraNL2 < GaoNep) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL2 .. "/" .. GaoNep .. "<color> <color=pink>" .. EVENT[2][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (KiemTraNL3 <= 0 or KiemTraNL3 < DauXanh) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL3 .. "/" .. DauXanh .. "<color> <color=pink>" .. EVENT[3][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (KiemTraNL4 <= 0 or KiemTraNL4 < ThitHeo) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL4 .. "/" .. ThitHeo .. "<color> <color=pink>" .. EVENT[4][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (KiemTraNL5 <= 0 or KiemTraNL5 < BiQuyet) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL5 .. "/" .. BiQuyet .. "<color> <color=pink>" .. EVENT[5][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (GetCash() < TienYC) then
		Talk(1, "", "Bπn kh´ng ÆÒ <color=red>" .. TienYC .. " l≠Óng<color>, vui lﬂng ki”m tra lπi.")
		return
	end
	if (CheckRoom(1, 1) == 0) then
		Talk(1, "", "Vui lﬂng chıa ›t nh t 1 ´ trËng.")
		return
	end
	
	--AddItemID(Item);
	DelItem(LaBanh, 0, 6, EVENT[1][1])
	DelItem(GaoNep, 0, 6, EVENT[2][1])
	DelItem(DauXanh, 0, 6, EVENT[3][1])
	DelItem(ThitHeo, 0, 6, EVENT[4][1])
	DelItem(BiQuyet, 0, 6, EVENT[5][1])
	Pay(TienYC)

	local Item = AddStackItem(nNumber, 6, EVENT[6][1], 0, 0, 0, 0)
	SetExpiredTime(Item, EVENT_YEAR, EVENT_MONTH, EVENT_DAY, 23, 59)

	local nCurTime = GetTimeMin()
	SetTask(TASK_THOIGIAN5, nCurTime)
	Msg2Player("Bπn ÆÊi thµnh c´ng " .. nNumber .. " <color=yellow>" .. EVENT[6][2] .. "<color>.")
end

function GhepEvent3(num)
	local nNumber = tonumber(num)
	if (nNumber < 1) then
		Talk(1, "", "MÁi l«n lµm tËi thi”u <color=yelow>1 vÀt ph»m<color>.")
		return
	end

	if (nNumber > 500) then
		Talk(1, "", "MÁi l«n chÿ lµm tËi Æa <color=yelow>500 vÀt ph»m<color> mµ th´i.")
		return
	end
	EVENT = {
		{1662, "L∏ b∏nh"},
		{1663, "Gπo n’p"},
		{1664, "ßÀu xanh"},
		{1665, "Thﬁt heo"},
		{1670, "B› quy’t lµm b∏nh ch≠ng h∂o hπng"},
		{1672, "B∏nh ch≠ng h∂o hπng"}
	}
	KiemTraNL1 = GetItemCount(0, 6, EVENT[1][1])
	KiemTraNL2 = GetItemCount(0, 6, EVENT[2][1])
	KiemTraNL3 = GetItemCount(0, 6, EVENT[3][1])
	KiemTraNL4 = GetItemCount(0, 6, EVENT[4][1])
	KiemTraNL5 = GetItemCount(0, 6, EVENT[5][1])

	LaBanh = nNumber * 2
	GaoNep = nNumber * 2
	DauXanh = nNumber * 2
	ThitHeo = nNumber * 2
	BiQuyet = nNumber * 1
	TienYC = nNumber * 50000

	if (KiemTraNL1 <= 0 or KiemTraNL1 < LaBanh) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL1 .. "/" .. LaBanh .. "<color> <color=pink>" .. EVENT[1][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (KiemTraNL2 <= 0 or KiemTraNL2 < GaoNep) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL2 .. "/" .. GaoNep .. "<color> <color=pink>" .. EVENT[2][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (KiemTraNL3 <= 0 or KiemTraNL3 < DauXanh) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL3 .. "/" .. DauXanh .. "<color> <color=pink>" .. EVENT[3][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (KiemTraNL4 <= 0 or KiemTraNL4 < ThitHeo) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL4 .. "/" .. ThitHeo .. "<color> <color=pink>" .. EVENT[4][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end
	if (KiemTraNL5 <= 0 or KiemTraNL5 < BiQuyet) then
		Talk(
			1,
			"",
			"Kh´ng ÆÒ nguy™n li÷u, bπn Æang c„ <color=red>" ..
				KiemTraNL5 .. "/" .. BiQuyet .. "<color> <color=pink>" .. EVENT[5][2] .. "<color>, vui lﬂng ki”m tra lπi."
		)
		return
	end

	if (GetCash() < TienYC) then
		Talk(1, "", "Bπn kh´ng ÆÒ <color=red>" .. TienYC .. " l≠Óng<color>, vui lﬂng ki”m tra lπi.")
		return
	end

	if (CheckRoom(1, 1) == 0) then
		Talk(1, "", "Vui lﬂng chıa ›t nh t 1 ´ trËng.")
		return
	end
	
	--AddItemID(Item);
	DelItem(LaBanh, 0, 6, EVENT[1][1])
	DelItem(GaoNep, 0, 6, EVENT[2][1])
	DelItem(DauXanh, 0, 6, EVENT[3][1])
	DelItem(ThitHeo, 0, 6, EVENT[4][1])
	DelItem(BiQuyet, 0, 6, EVENT[5][1])
	Pay(TienYC)

	local Item = AddStackItem(nNumber, 6, EVENT[6][1], 0, 0, 0, 0)
	SetExpiredTime(Item, EVENT_YEAR, EVENT_MONTH, EVENT_DAY, 23, 59)

	local nCurTime = GetTimeMin()
	SetTask(TASK_THOIGIAN5, nCurTime)
	Msg2Player("Bπn ÆÊi thµnh c´ng " .. nNumber .. " <color=yellow>" .. EVENT[6][2] .. "<color>.")
end

function noinput()
end

function no()
end
