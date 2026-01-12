--Author: Rim
--Date: 6-2-2021
NEED_ROOM_EMPTY = "Hµnh trang ph¶i cã ®ñ 2x4 « trèng.";
SAME_5HANH = "Trang bÞ ®· cã ngò hµnh thÝch hîp. Xin lùa chän ngò hµnh kh¸c.";
LIMITED_LEVEL = "Trang bÞ ®· ®¹t ®¼ng cÊp cao nhÊt, kh«ng thÓ n©ng cÊp.";
NEEDMONEY = 12000;

-----------------------------------------------------|
-----PHAN TRAM THANH CONG----------------------------|
PHANTRAM_NGUHANH = 75;       --|
PHANTRAM_UPLEVEL = 75;       --|
PHANTRAM_CHANGEOPTION = 100; --|
MIN_LUCK = 55
level_option_vip = 7;      --|
--tang cang cao thi cang de thanh cong(toi da 99%);--|
-----------------------------------------------------|
POSTB = 12; --Defaul;

function main()
	--dofile("script\\item\\khamxanh\\tremble.lua")
	excute_action();
end

function excute_action()
	if (GetCash() < NEEDMONEY) then
		Msg2Player("B¹n kh«ng ®ñ 1 v¹n 2000 l­îng.")
		return
	end

	nItemGenre, nDetail, nParticur, nLevel, nSeries = 0;
	nLucky = GetLucky(0);
	nItem = GetPOItem(POSTB, 0); -- o cua trang bi;
	nItemGenre, nDetail, nParticur, nLevel, nSeries, nLuck, nStackNum = FindItemEx(nItem);

	nKim = GetPOItem(POSTB, 3); -- lam tt
	if (nKim > 0) then       --doi sang he Kim
		if (nSeries == 0) then
			Msg2Player(SAME_5HANH);
			return
		end
		nSeriesNew = 0;
		Covert_NguHanh(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky, nSeriesNew);
		RemoveItem(nKim, 1);
		Pay(NEEDMONEY);
		return
	end

	nMoc = GetPOItem(POSTB, 4);
	if (nMoc > 0) then --doi sang he Moc
		if (nSeries == 1) then
			Msg2Player(SAME_5HANH);
			return
		end
		nSeriesNew = 1;
		Covert_NguHanh(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky, nSeriesNew);
		RemoveItem(nMoc, 1);
		Pay(NEEDMONEY);
		return
	end

	nThuy = GetPOItem(POSTB, 5);
	if (nThuy > 0) then --doi sang he Thuy
		if (nSeries == 2) then
			Msg2Player(SAME_5HANH);
			return
		end
		nSeriesNew = 2;
		Covert_NguHanh(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky, nSeriesNew);
		RemoveItem(nThuy, 1);
		Pay(NEEDMONEY);
		return
	end

	nHoa = GetPOItem(POSTB, 6);
	if (nHoa > 0) then --doi sang he Hoa
		if (nSeries == 3) then
			Msg2Player(SAME_5HANH);
			return
		end
		nSeriesNew = 3;
		Covert_NguHanh(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky, nSeriesNew);
		RemoveItem(nHoa, 1);
		Pay(NEEDMONEY);
		return
	end

	nTho = GetPOItem(POSTB, 7);
	if (nTho > 0) then --doi sang he Tho
		if (nSeries == 4) then
			Msg2Player(SAME_5HANH);
			return
		end
		nSeriesNew = 4;
		Covert_NguHanh(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky, nSeriesNew);
		RemoveItem(nTho, 1);
		Pay(NEEDMONEY);
		return
	end

	nTuTT = GetPOItem(POSTB, 1);
	if (nTuTT > 0) then --nang cap trang bi ( tu thuy tinh );
		if (nLevel >= 10) then
			Msg2Player(LIMITED_LEVEL);
			return
		end
		RemoveItem(nTuTT, 1);
		UpLevel_Item(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky);
		Pay(NEEDMONEY);
		return
	end

	nLucTT = GetPOItem(POSTB, 2);
	if (nLucTT > 0) then --thay doi thuoc tinh trang bi ( luc thuy tinh );
		RemoveItem(nLucTT, 1);
		changeOption_Item(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky);
		Pay(NEEDMONEY);
		return
	end
end;

function Covert_NguHanh(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky, nSeriesNew)
	tylephantram(PHANTRAM_NGUHANH); --ty le;
	RemoveItem(nItem, 1);
	-- local FronOP1                      = {};
	-- local FronOP2                      = {};
	-- local BackOp1                      = {};
	-- local BackOp2                      = {};
	-- local value1_3,
	-- value2_3,
	-- value3_3,
	-- value4_3,
	-- value5_3,
	-- value6_3,
	-- value7_3,
	-- value8_3;
	-- FronOP1[1], FronOP2[1], value1_3, --1
	-- FronOP1[2], FronOP2[2], value2_3,
	-- FronOP1[3], FronOP2[3], value3_3, --3
	-- FronOP1[4], FronOP2[4], value4_3,
	-- FronOP1[5], FronOP2[5], value5_3, --5
	-- FronOP1[6], FronOP2[6], value6_3,
	-- FronOP1[7], FronOP2[7], value7_3,
	-- FronOP1[8], FronOP2[8], value8_3
	--                                    = GetMagicAttrib(nItem);

	-- nIndex                             = AddItem(0, 0, nDetail, nParticur, nLevel, nSeriesNew, 0, level_option_vip);
	-- local value1_3_n,
	-- value2_3_n,
	-- value3_3_n,
	-- value4_3_n,
	-- value5_3_n,
	-- value6_3_n,
	-- value7_3_n,
	-- value8_3_n;
	-- BackOp1[1], BackOp2[1], value1_3_n,
	-- BackOp1[2], BackOp2[2], value2_3_n, --2
	-- BackOp1[3], BackOp2[3], value3_3_n,
	-- BackOp1[4], BackOp2[4], value4_3_n, --4
	-- BackOp1[5], BackOp2[5], value5_3_n,
	-- BackOp1[6], BackOp2[6], value6_3_n, --6
	-- BackOp1[7], BackOp2[7], value7_3_n,
	-- BackOp1[8], BackOp2[8], value8_3_n
	--                                    = GetMagicAttrib(nIndex);

	-- SetMagicAttrib(nIndex,
	-- 	FronOP1[1], FronOP2[1], value1_3, --1
	-- 	BackOp1[2], BackOp2[2], value2_3_n, --2
	-- 	FronOP1[3], FronOP2[3], value3_3, --3
	-- 	BackOp1[4], BackOp2[4], value4_3_n, --4
	-- 	FronOP1[5], FronOP2[5], value5_3, --5
	-- 	BackOp1[6], BackOp2[6], value6_3_n, --6
	-- 	BackOp1[7], BackOp2[7], value7_3_n,
	-- 	BackOp1[8], BackOp2[8], value8_3_n
	-- );
	--AddItemID(nIndex);
	level_option_vip = RANDOM(8, 10); --|
	SERVER_LUCKY = RANDOM(55, 100)

	AddItemEx(0, nDetail, nParticur, nLevel, nSeriesNew, nLucky, RANDOM(level_option_vip, 10));
	Msg2Player("<color=yellow>§«i ngò hµnh trang bÞ thµnh c«ng");
end

function UpLevel_Item(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky)
	if (tylephantram(PHANTRAM_UPLEVEL) == 1) then --ty le;
		RemoveItem(nItem, 1);
		local MA               = {};
		local VA               = {};
		local value1_3,
		value2_3,
		value3_3,
		value4_3,
		value5_3,
		value6_3,
		value7_3,
		value8_3;
		MA[1], VA[1], value1_3,
		MA[2], VA[2], value2_3,
		MA[3], VA[3], value3_3,
		MA[4], VA[4], value4_3,
		MA[5], VA[5], value5_3,
		MA[6], VA[6], value6_3,
		MA[7], VA[7], value7_3,
		MA[8], VA[8], value8_3
		                       = GetMagicAttrib(nItem);

		nIndex                 = AddItem(0, 0, nDetail, nParticur, nLevel + 1, nSeries, 0);

		SetMagicAttrib(nIndex,
			MA[1], VA[1], value1_3,
			MA[2], VA[2], value2_3,
			MA[3], VA[3], value3_3,
			MA[4], VA[4], value4_3,
			MA[5], VA[5], value5_3,
			MA[6], VA[6], value6_3,
			MA[7], VA[7], value7_3,
			MA[8], VA[8], value8_3
		);
		AddItemID(nIndex);
		Msg2Player("<color=yellow>N©ng cÊp trang bÞ thµnh c«ng");
	end
end

function changeOption_Item(nItem, nItemGenre, nDetail, nParticur, nLevel, nSeries, nLucky)
	if (tylephantram(PHANTRAM_CHANGEOPTION) == 1) then
		RemoveItem(nItem, 1);
		nIndex = AddItemEx(0, nDetail, nParticur, nLevel, nSeries, nLucky, RANDOM(level_option_vip, 10));
		--AddItemID(nIndex);
		Msg2Player("<color=yellow>§«i thuéc tÝnh trang bÞ thµnh c«ng");
	end
end

function tylephantram(nPhantram)
	local nRan = RANDOM(1, 100);
	--Msg2Player(nRan)
	if (nRan > nPhantram) then
		Msg2Player("ThÊt b¹i, xin thö l¹i! Tæn thÊt 1 thñy tinh. Tû lÖ thµnh c«ng lµ <color=yellow> " ..
			nPhantram .. "% <color>");
		return 0
	else
		return 1
	end
end
