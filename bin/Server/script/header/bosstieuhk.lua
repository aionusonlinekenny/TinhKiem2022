--script add boss
BOSSTIEU_ARRAY_HK = {
	{ "DiÖu Nh­",          513, 2, { 1, 6 } },
	{ "LiÔu Thanh Thanh",  523, 1, { 2, 5 } },
	{ "Tr­¬ng T«ng ChÝnh", 511, 4, { 3, 4 } }
};

POSITION_ARRAY_HK = {
	{ 78,  42878, 106214 },
	{ 37,  45312, 113152 },
	{ 1,   53760, 88576 },
	{ 125, 53680, 98949 }, --luutien
	{ 123, 51478, 102982 }, --laoho
	{ 9,   73241, 184375 } --truonggiang
};

COMMON_INFO = "Cã ng­êi nh×n thÊy <color=Yellow>%s<color> xuÊt hiÖn t¹i %s"

function releasebosstieu()
	local nNpcIndex = 0;
	local nPos = 1;
	for i = 1, getn(BOSSTIEU_ARRAY_HK) do
		nMap = RANDOM(1, 2);
		nPos = BOSSTIEU_ARRAY_HK[i][4][nMap];
		nNpcIndex = AddNpcNew(BOSSTIEU_ARRAY_HK[i][2], 100, POSITION_ARRAY_HK[nPos][1],
			POSITION_ARRAY_HK[nPos][2], POSITION_ARRAY_HK[nPos][3], DEATH_BOSSTIEU,
			5, nil, 1, BOSSTIEU_ARRAY_HK[i][3], 300000, 28000000, 5000, 8000, nil, nil, nil, nil, nil, 80, 2,
			DROP_BOSSTIEU);
		if (nNpcIndex > 0) then
			SetNpcTimeout(nNpcIndex, 32400);
			Msg2SubWorld(format(COMMON_INFO, BOSSTIEU_ARRAY_HK[i][1],
				GetWorldName(POSITION_ARRAY_HK[nPos][1]),
				floor(POSITION_ARRAY_HK[nPos][2] / 256),
				floor(POSITION_ARRAY_HK[nPos][3] / 512)));

			print(format(COMMON_INFO, BOSSTIEU_ARRAY_HK[i][1],
				GetWorldName(POSITION_ARRAY_HK[nPos][1]),
				floor(POSITION_ARRAY_HK[nPos][2] / 256),
				floor(POSITION_ARRAY_HK[nPos][3] / 512)))
		end
	end
end;
