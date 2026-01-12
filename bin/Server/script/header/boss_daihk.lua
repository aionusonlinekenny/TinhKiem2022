--script add boss
BOSSTIEU_ARRAY = {
    { "V­¬ng T¸ ",           739,  95, 341, 0, { 1, 6 } },
    { "HuyÒn Gi¸c §¹i S­ ",  740,  95, 322, 0, { 1, 6 } },
    { "§­êng BÊt NhiÔm",     741,  95, 336, 1, { 1, 6 } },
    { "B¹ch Doanh Doanh",    742,  95, 336, 1, { 1, 6 } },
    { "Thanh TuyÖt S­ Th¸i", 743,  95, 341, 2, { 1, 6 } },
    { "Yªn HiÓu Tr¸i",       744,  95, 336, 2, { 1, 6 } },
    { "Hµ Nh©n Ng· ",        745,  95, 321, 3, { 1, 6 } },
    { "§¬n T­ Nam",          746,  95, 341, 4, { 1, 6 } },
    { "TuyÒn C¬ Tö ",        747,  95, 340, 4, { 1, 6 } },
    { "Hµn M«ng",            748,  95, 342, 1, { 1, 6 } },
    { "§oan Méc DuÖ ",       565,  95, 227, 3, { 1, 6 } },
    { "Lam Y Y",             582,  95, 181, 1, { 1, 6 } },
    { "Chung Linh Tó ",      567,  95, 181, 2, { 1, 6 } },
    { "M¹nh Th­¬ng L­¬ng",   583,  95, 341, 3, { 1, 6 } },
    { "HuyÒn Nan §¹i S­",    1365, 95, 342, 0, { 1, 6 } },
    { "Thanh Liªn Tö",       1368, 95, 875, 4, { 1, 6 } },
    { "§­êng Phi YÕn",       1366, 95, 342, 1, { 1, 6 } },
    { "Tõ §¹i Nh¹c",         1367, 95, 342, 4, { 1, 6 } },
};

POSITION_ARRAY = {
    { 80,  { { 1833, 3367 }, { 1511, 3239 }, { 1582, 2902 }, { 1987, 2975 } }, "D­¬ng Ch©u" },
    { 11,  { { 2909, 5236 }, { 2883, 4835 }, { 3317, 5280 }, { 3306, 4860 } }, "Thµnh §«" },
    { 162, { { 1336, 3205 }, { 1426, 3401 }, { 1723, 3489 }, { 1668, 2981 } }, "§¹i Lý" },
    { 1,   { { 1424, 3044 }, { 1783, 3183 }, { 1724, 3406 }, { 1497, 3383 } }, "Ph­îng T­êng" },
    { 78,  { { 1753, 3121 }, { 1670, 3462 }, { 1369, 3411 }, { 1379, 3141 } }, "T­¬ng D­¬ng" },
    { 37,  { { 1770, 3403 }, { 1520, 3281 }, { 1535, 2902 }, { 2017, 2679 } }, "BiÖn Kinh" },
    { 176, { { 1162, 2781 }, { 1832, 3342 }, { 1247, 3354 }, { 1754, 2805 } }, "L©m An" },
};

COMMON_INFO = "Cã ng­êi nh×n thÊy <color=Yellow>%s<color> xuÊt hiÖn t¹i %s (%d/%d)"

function releasebossdai()
    local nNpcIndex = 0;
    local nPos = 1;
    local nMap = 1;
    local nBoss = RANDOM(3, getn(BOSSTIEU_ARRAY))
    for i = 1, nBoss do
        nMap = RANDOM(1, 7);
        nPos = RANDOM(1, 4);

        nNpcIndex = AddNpcNew(BOSSTIEU_ARRAY[i][2], BOSSTIEU_ARRAY[i][3], POSITION_ARRAY[nMap][1],
            POSITION_ARRAY[nMap][2][nPos][1] * 32, POSITION_ARRAY[nMap][2][nPos][2] * 32, DEATH_BOSSDAI,
            5, BOSSTIEU_ARRAY[i][1], 1, BOSSTIEU_ARRAY[i][5], 300000, 800000000, 5000, 8000, nil, nil, nil, nil, nil, 80,
            2,
            DROP_BOSSDAI);
        if (nNpcIndex > 0) then
            SetNpcTimeout(nNpcIndex, 32400);
            Msg2SubWorld(format(COMMON_INFO, BOSSTIEU_ARRAY[i][1],
                GetWorldName(POSITION_ARRAY[nMap][1]),
                floor(POSITION_ARRAY[nMap][2][nPos][1] / 8),
                floor(POSITION_ARRAY[nMap][2][nPos][2] / 16)));
            print(format(COMMON_INFO, BOSSTIEU_ARRAY[i][1],
                GetWorldName(POSITION_ARRAY[nMap][1]),
                floor(POSITION_ARRAY[nMap][2][nPos][1] / 8),
                floor(POSITION_ARRAY[nMap][2][nPos][2] / 16)))
        end
    end
end;
