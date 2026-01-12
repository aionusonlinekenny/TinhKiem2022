Include("\\script\\lib\\worldlibrary.lua")
Include("\\script\\serverscript.lua")
Include("\\script\\global\\npcchucnang\\phantang.lua")
Include("\\script\\header\\factionhead.lua")
Include("\\script\\header\\itemblue.lua")
Include("\\script\\header\\dialogsay.lua")
Include("\\script\\header\\boss_daihk.lua")
Include("\\script\\header\\taskid.lua")

LEVEL_ARRAY = {
    {1, "1 cÊp"},
    {2, "2 cÊp"},
    {5, "5 cÊp"},
    {10, "10 cÊp"},
    -- { 20,  "20 cÊp" },
    -- { 50,  "50 cÊp" },
    --{ 100, "100 cÊp" },
    {120, "120 cÊp"}
}

function main(nIndex)
    dofile("script/gmscript.lua")
    if (GetAccount() == "dungnd" or GetAccount() == "frankenstein") then
        SetTaskTemp(DESPLAYERID, nIndex)
        Say(
            "Sö dông tÝnh n¨ng:",
            9,
            "Test/testham",
            "Qu¶n lý server/server_info",
            --
            --"T¸c ®éng lªn môc tiªu/muctieu",
            "Thao t¸c víi chÝnh m×nh/myself",
            "LÊy Item/getitem",
            "NhËn ®å xanh/nhantrangbi",
            --"T¶i l¹i script/reload",
            "Tho¸t/no"
        )
    end
    return 0
end

function muctieu()
    local nIdx = GetTaskTemp(DESPLAYERID)
    local gmIdx = PlayerIndex
    local szStr = ""

    PlayerIndex = nIdx
    w, x, y = GetWorldPos()
    szPlayerName = GetName()
    nPlayerLevel = GetLevel()
    szStr =
        format(
        "Th«ng tin nh©n vËt:<enter>- PlayerIndex: <color=yellow>%d<color><enter>- Tµi kho¶n: <color=yellow>%s<color><enter>- Nh©n vËt: <color=yellow>%s<color><enter>- §¼ng cÊp: <color=yellow>%d<color><enter>- Täa ®é: [<color=green>%s<color> - <color=yellow>%d/%d<color>]",
        nIdx,
        GetAccount(),
        szPlayerName,
        nPlayerLevel,
        GetWorldName(w),
        floor(x / 8),
        floor(y / 16)
    )

    PlayerIndex = gmIdx
    --bat dau thao tac voi muc tieu--------------------
    --RemoveItem(106,1);
    --KillPlayer();
    Say(
        szStr,
        8,
        "Tèng giam vµo ngôc/tonggiam",
        "Lªn cÊp/tar_tangcap",
        "Fix lçi/tar_fix",
        "Hç trî t©n thñ/tar_tanthu",
        "Hç trî lËp bang/tar_lanhdao",
        "Hç trî ®Òn bï sù cè/tar_denbu",
        --"NhËn ®å /nhanitem",
        --"KiÓm täa ®é/toado",
        "Trôc xuÊt/tar_kichout",
        --"Thªm ®iÓm F3-F5/tiemnang",
        --"Kü n¨ng/kynang",
        --"Céng tiÒm n¨ng/cong",
        "Tho¸t/no"
    )
end

function myself() --thao tac voi chinh minh
    Say(
        "Thao t¸c víi chÝnh m×nh:",
        8,
        "Lªn cÊp/tangcap",
        "NhËn KNB/nhan_knb",
        --"NhËn xu /nhanxu",
        --"NhËn TiÒn v¹n/tienvan",
        --"NhËn Lôc Thñy Tinh/nhan_ltt",
        "KiÓm täa ®é /toado",
        "Thªm ®iÓm F3-F5/tiemnang",
        --"Kü n¨ng/kynang",
        --"Céng tiÒm n¨ng/cong",
        "Tho¸t/no"
    )
end

function opentim()
    Input("timten")
end

function nhanxu()
    AddStackItem(999, 4, 417, 0, 0, 5, 0, 0)
    AddStackItem(999, 4, 417, 0, 0, 5, 0, 0)
    AddStackItem(999, 4, 417, 0, 0, 5, 0, 0)
    AddStackItem(999, 4, 417, 0, 0, 5, 0, 0)
    AddStackItem(999, 4, 417, 0, 0, 5, 0, 0)
    AddStackItem(999, 4, 417, 0, 0, 5, 0, 0)
    AddStackItem(999, 4, 417, 0, 0, 5, 0, 0)
end

function tienvan()
    Earn(1000000)
end

function nhan_ltt()
    AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    --AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    --AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    --AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    -- AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    --AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
    --AddStackItem(50, 4, 240, 0, 0, 5, 0, 0)
end

function nhan_knb()
    AddStackItem(50, 4, 343, 0, 0, 5, 0, 0)
end

function timten(name)
    local nOnlinePlayer = GetPlayerCount()
    if (nOnlinePlayer <= 0) then
        Msg2Player("Kh«ng cã ng­êi ch¬i Online!")
        return
    end

    local szStr = ""
    local nTempPlayerIdx = 0
    local gmIdx = PlayerIndex
    for i = 1, nOnlinePlayer do
        PlayerIndex = i

        if (GetAccount() == name or GetName() == name) then
            w, x, y = GetWorldPos()
            szPlayerName = GetName()
            nPlayerLevel = GetLevel()
            szStr =
                format(
                "[<color=yellow>Idx: %d - Acc: %s - Char: %s - Level: %d<color>] - Täa ®é: [<color=green>%s<color> - <color=yellow>%d/%d<color>]",
                i,
                name,
                szPlayerName,
                nPlayerLevel,
                GetWorldName(w),
                floor(x / 8),
                floor(y / 16)
            )
            nTempPlayerIdx = i
            --DelMagic(1060)
            --AddMagic(1060,1)
            break
        end
    end
    PlayerIndex = gmIdx
    SetTaskTemp(DESPLAYERID, nTempPlayerIdx)
    Msg2Player(szStr)
    --Msg2Player(GetTaskTemp(DESPLAYERID))
    muctieu()
end

function reload1()
    Input("reload1")
end

function reload()
    --dofile(filePath);
    dofile("script/item/event/tuiduocpham.lua")
end

function tonggiam()
    local nIdx = GetTaskTemp(DESPLAYERID)
    local gmIdx = PlayerIndex
    PlayerIndex = nIdx --bat dau thao tac voi muc tieu
    if (NewWorld(197, 1590, 3206) == 1) then
        LeaveTeam()
        SetPKMode(0, 0) --phuc hoi pk tu do
        SetFightState(0) --phi chien dau
        SetPunish(0)
        ForbidUseTownP(1)
        SetCurCamp(GetCamp())
    end
    PlayerIndex = gmIdx
end

function tangcap()
    local count = 1
    local StrTab = {}

    for i = 1, getn(LEVEL_ARRAY) do
        StrTab[count] = LEVEL_ARRAY[i][2] .. "/lencap"
        count = count + 1
    end

    if (count == 1) then
        return
    end

    StrTab[count] = "Tho¸t/no"
    Say("Lªn bao nhiªu cÊp?", count, StrTab)
end

function lencap(sel)
    local nSel = sel + 1
    ST_LevelUp(LEVEL_ARRAY[nSel][1])
end

function nhanitem()
    Say("NhËn ®å :", 4, "M¸u mana/mana", "An Bang §Þnh Quèc/nhanab", "VËt phÈm kh¸c/itemkhac", "Tho¸t/no")
end

function nhanab()
    for j = 160, 168 do
        AddItemGold(j, 5)
    end
    AddItem(0, 0, 10, 6, 10, 0, 0, 0) --bontieu
end

function mana()
    while (CheckRoom(1, 1) == 1) do
        AddItem(0, duocpham, 2, 0, 5, 5, 0, 0)
    end
end

function itemkhac()
    Earn(500000)
    AddTool(10)
    --Item xanh---------------------------------
    --local kind,genre,detail,parti,level,series = 0,0,0,0,10,2;
    --local nIndex = ItemSetAdd(kind,genre,detail,parti,level,series,0);
    --local	type1 , value1_1, value1_3,
    --	type2 , value2_1, value2_3,
    --	type3 , value3_1, value3_3,
    --	type4 , value4_1, value4_3,
    --	type5 , value5_1, value5_3,
    --	type6 , value6_1, value6_3,
    --	type7 , value7_1, value7_3,
    --	type8 , value8_1, value8_3
    --=
    --	115 , 30, 0,
    --	123 , 60, 0,
    --	85 , 200, 0,
    --	110 , 40, 0,
    --	126 , 80, 0,
    --	102 , 25, 0,
    --	0 , 0, 0,
    --	0 , 0, 0;
    --ChangeItemMagic(nIndex,
    --				type1 , value1_1, value1_3,
    --				type2 , value2_1, value2_3,
    --				type3 , value3_1, value3_3,
    --				type4 , value4_1, value4_3,
    --				type5 , value5_1, value5_3,
    --				type6 , value6_1, value6_3,
    --				type7 , value7_1, value7_3,
    --				type8 , value8_1, value8_3
    --				);
    --AddItemID(nIndex);
    ----------------------------------------------------------
end

function toado()
    local w, x, y = GetWorldPos()
    Msg2Player("Map sè <color=Green>" .. w .. "<color> täa ®é <color=Yellow>" .. x .. ", " .. y)
    Msg2Player("Map sè <color=Green>" .. w .. "<color> täa ®é <color=Yellow>" .. floor(x / 32) .. ", " .. floor(y / 32))
end

function tiemnang()
    AddProp(500)
    AddMagicPoint(200)
end

function kynang()
    Say(
        "LÊy kü n¨ng:",
        12,
        "ThiÕu L©m/gmselskill",
        "Thiªn v­¬ng/gmselskill",
        "§­êng m«n/gmselskill",
        "Ngò ®éc/gmselskill",
        "Nga My/gmselskill",
        "Thóy Yªn/gmselskill",
        "C¸i Bang/gmselskill",
        "Thiªn NhÉn/gmselskill",
        "Vâ §ang/gmselskill",
        "C«n L«n/gmselskill",
        "Tho¸t/no"
    )
end

function gmselskill(sel)
    ClearSkill() --xoa skill cu~
    AddMagic(53, 1) --cong kich vat ly
    AddMagic(1, 1) --cong kich vat ly
    AddMagic(2, 1) --cong kich vat ly
    --bat dau addskill phai'
    local nSel = sel + 1
    addskill_thuong(sel + 1)
    for i = 1, getn(SKILL90_ARRAY[nSel]) do
        if (SKILL90_ARRAY[nSel][i][3] > 0) then
            AddMagic(SKILL90_ARRAY[nSel][i][2], 20)
        else
            AddMagic(SKILL90_ARRAY[nSel][i][2], 0)
        end
    end
    AddMagic(SKILL120_ARRAY[nSel][2], 20)
end

function Show()
    for i = 1, GetPlayerCount() do
        gmidx = PlayerIndex
        PlayerIndex = i
        TarName = GetName()
        PlayerIndex = gmidx
        Msg2Player("ID <color=green>" .. i .. "<color> tªn lµ:<color=yellow> " .. TarName .. "")
    end
end

function noinput()
end

function no()
end

function getitem()
    Input("layitem")
end

function layitem(szStr)
    local szICode = Split(szStr, ",")
    local n = getn(szICode)
    --Msg2Player(n)
    if (n ~= 1) then
        if (n < 6) then
            Talk(1, "", format("Sai Item code. ThiÕu tr­êng d÷ liÖu. NhËp l¹i: <color=yellow>%s<color>", n))
            return
        end
        if (n > 8) then
            Talk(1, "", format("Sai Item code. V­ît qu¸ tr­êng d÷ liÖu. NhËp l¹i: <color=yellow>%s<color>", n))
            return
        end
        local itclass = tonumber(szICode[1])
        local itgender = tonumber(szICode[2])
        local itdetai = tonumber(szICode[3])
        local itpartial = tonumber(szICode[4])
        local itlv = tonumber(szICode[5])
        local itseri = tonumber(szICode[6])
        local itluck = tonumber(szICode[7])
        local itmagic = tonumber(szICode[8])
        if (itlv < 0) then
            Talk(1, "", format("CÊp ®é mÆn ®å kh«ng ®­îc thÊp. NhËp l¹i: <color=yellow>%s<color>", itlv))
            return
        end
        if (itlv > 10) then
            Talk(1, "", format("ChØ hæ trî cÊp 10. NhËp l¹i:<color=yellow>%s<color>", itlv))
            return
        end
        if (itmagic == nil) then
            itmagic = 0
        end
        AddItemEx(itclass, itgender, itdetai, itpartial, itlv, itseri, itluck, itmagic)
    else
        local EquipId = tonumber(szICode[1])
        AddItemID(AddItem(2, 0, EquipId, 0, 0, 0, 0, 0))
    end
end

function testham()
    --Msg2Player(CountPlayerInSubWorldByIP(380, GetIP2()))
    --AddItemEx(0,0,0,10,0,RANDOM(55,100),RANDOM(7,10))
    --toado()
    --ForbidUseTownP(0);
    --AddItemEx(6, 1182, 0, 0, 5, 0, 0)
    --AddItemEx(0, 10, 29, 10, 0, 0, 0)
    --AddPlatinaItem(101, 10)
    -- AddPlatinaItem(94, 10)
    -- AddPlatinaItem(94, 10)
    -- AddPlatinaItem(94, 10)
    -- AddPlatinaItem(94, 10)
    -- AddPlatinaItem(98, 10)
    -- AddPlatinaItem(99, 10)
    -- AddPlatinaItem(100, 10)
    -- AddPlatinaItem(130, 10)
    --LeaveTong()
    -- if(tonumber(GetLocalDate("%w"))==6) then-- thu 7 hang tuan
    -- 	Msg2Player("Thu 7")
    -- end;
    --releasebossdai()
    --SetTranslife(11)
    -- for i = 1, 3 do
    --     AddItemEx(6, 2433, 0, 0, 5, 0, 0)
    --     AddItemEx(6, 1440, 0, 0, 5, 0, 0)
    --     AddStackItem(100, 6, 4814, 0, 0, 5, 0, 0)
    -- end
    --Msg2Player(GetFactionNo())
    --ST_LevelUp(80)
    --denbu_server()
    -- SetCamp(4);
	-- SetCurCamp(4);
    AddMagic(1368,20)
end

function server_info()
    local szMsg =
        format(
        "Xin chµo %s!<enter>Thêi gian: <color=red>%s<color><enter>Sè ng­êi ch¬i ®ang online: <color=yellow>%d<color>",
        GetName(),
        GetLocalDate("%Y-%m-%d %H:%M"),
        GetPlayerCount()
    )
    Say(szMsg, 3, "Danh s¸ch Online/Show", "T×m nh©n vËt/opentim", "Tho¸t/no")
end

function no()
end

function tar_kichout()
    local nIdx = GetTaskTemp(DESPLAYERID)
    local gmIdx = PlayerIndex

    PlayerIndex = nIdx
    KickOutSelf()
    PlayerIndex = gmIdx
end

function tar_tangcap()
    local count = 1
    local StrTab = {}

    for i = 1, getn(LEVEL_ARRAY) do
        StrTab[count] = LEVEL_ARRAY[i][2] .. "/tar_lencap"
        count = count + 1
    end

    if (count == 1) then
        return
    end

    StrTab[count] = "Tho¸t/no"
    Say("Lªn bao nhiªu cÊp?", count, StrTab)
end

function tar_lencap(sel)
    local nIdx = GetTaskTemp(DESPLAYERID)
    local gmIdx = PlayerIndex

    PlayerIndex = nIdx
    local nSel = sel + 1
    ST_LevelUp(LEVEL_ARRAY[nSel][1])
    Msg2Player(format("<color=yellow>GM<color> ®· tÆng cho b¹n <color=green>%d<color> cÊp!", LEVEL_ARRAY[nSel][1]))
    PlayerIndex = gmIdx
end

function tar_lanhdao()
    local nIdx = GetTaskTemp(DESPLAYERID)
    local gmIdx = PlayerIndex

    PlayerIndex = nIdx
    AddRepute(300 - GetRepute())
    AddFuYuan(500 - GetFuYuan())
    SetLeadLevel(30)
    AddItemEx(4, 195, 0, 0, 5, 0, 0)
    PlayerIndex = gmIdx
end

function tar_fix()
    local nIdx = GetTaskTemp(DESPLAYERID)
    local gmIdx = PlayerIndex

    PlayerIndex = nIdx
    --DelMagic(1057)
    --AddMagic(1057, 1)
    --SetTask(TASK_DATAU_NUM, 849);
    --SetSex(1)
    --SetSavePw("")
    PlayerIndex = gmIdx
end

function tar_tanthu()
    local nIdx = GetTaskTemp(DESPLAYERID)
    local gmIdx = PlayerIndex

    PlayerIndex = nIdx
    for i = 1, 3 do
        AddItemEx(6, 2433, 0, 0, 5, 0, 0)
        AddItemEx(6, 1440, 0, 0, 5, 0, 0)
        AddStackItem(100, 6, 4814, 0, 0, 5, 0, 0)
    end
    PlayerIndex = gmIdx
    Msg2Player("Done!")
end

function tar_denbu()
    local nIdx = GetTaskTemp(DESPLAYERID)
    local gmIdx = PlayerIndex

    PlayerIndex = nIdx
        LockItem(AddStackItem(200, 6, 4814, 0, 0, 5, 0, 0),-2)
        LockItem(AddStackItem(5, 6, 1182, 0, 0, 5, 0, 0), -2)
        ST_LevelUp(5)
        Msg2Player("NhËn ®­îc hç trî ®Òn bï lçi m¸y chñ!")
        SetTask(83,1)
    PlayerIndex = gmIdx
    Msg2Player("Done!")
end


function denbu_server()
    for i = 1, GetPlayerCount() do
        gmidx = PlayerIndex
        local TarName = "§· nhËn"
        PlayerIndex = i
        if(GetTask(83) ~= 1) then
            TarName = GetName()
            -- LockItem(AddStackItem(200, 6, 4814, 0, 0, 5, 0, 0),-2)
            -- LockItem(AddStackItem(5, 6, 1182, 0, 0, 5, 0, 0), -2)
            -- ST_LevelUp(5)
            -- Msg2Player("NhËn ®­îc hç trî ®Òn bï lçi m¸y chñ!")
            -- SetTask(83,1)
        end
        PlayerIndex = gmidx
        Msg2Player("ID <color=green>" .. i .. "<color> tªn lµ:<color=yellow> " .. TarName .. "")
    end
end