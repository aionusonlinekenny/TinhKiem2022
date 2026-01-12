Include("\\script\\header\\taskid.lua");
Include("\\script\\header\\factionhead.lua")
Include("\\script\\header\\task_string.lua")
Include("\\script\\misc\\daiyitoushi\\toushi_head.lua");

TB_BASE_STRG = { 35, 20, 25, 30, 20 };
TB_BASE_DEX = { 25, 35, 25, 20, 15 };
TB_BASE_VIT = { 25, 20, 25, 30, 25 };
TB_BASE_ENG = { 15, 25, 25, 20, 40 };

TB_TOUSHI_PLAYERBASE =
{
    [0] =
    {
        ipower     = 35,
        iagility   = 25,
        iouter     = 25,
        iinside    = 15,
        imaxlife   = 204,
        imaxinner  = 16,
        lifeperlvl = 4,
        manaperlvl = 1,
        lifepervit = 8,
        manapereng = 1,
    },
    [1] =
    {
        ipower     = 20,
        iagility   = 35,
        iouter     = 20,
        iinside    = 25,
        imaxlife   = 103,
        imaxinner  = 77,
        lifeperlvl = 3,
        manaperlvl = 2,
        lifepervit = 5,
        manapereng = 3,
    },
    [2] =
    {
        ipower     = 25,
        iagility   = 25,
        iouter     = 25,
        iinside    = 25,
        imaxlife   = 153,
        imaxinner  = 77,
        lifeperlvl = 3,
        manaperlvl = 2,
        lifepervit = 6,
        manapereng = 3,
    },
    [3] =
    {
        ipower     = 30,
        iagility   = 20,
        iouter     = 30,
        iinside    = 20,
        imaxlife   = 213,
        imaxinner  = 41,
        lifeperlvl = 3,
        manaperlvl = 1,
        lifepervit = 7,
        manapereng = 2,
    },
    [4] =
    {
        ipower     = 20,
        iagility   = 15,
        iouter     = 25,
        iinside    = 40,
        imaxlife   = 76,
        imaxinner  = 163,
        lifeperlvl = 1,
        manaperlvl = 3,
        lifepervit = 3,
        manapereng = 4,
    },
}


function dk_chuyenphai(nFactionNumb)
    local nFlag = GetTask(TSK_TOUSHI_FLAG);
    local tbDes = {
        "<npc>: Thiªn h¹ ®¹i lo¹n nh­  ngµy h«m nay, c¸c ğ¹i m«n ph¸i trong vâ l©m ai còng chØ biÕt lo cho c¸i lîi cña m×nh, tïy theo chiÕn sù mµ bÕ quan,  trung nh©n vâ l©m th× bŞ tæn thÊt h¬n qu¸ nöa. VŞ kÕ vâ l©m h­¬ng háa, trïng t©n chÊn h­ng vâ l©m. T«n mÖnh vâ l©m minh chñ, kÓ tõ h«m nay c¸c ğ¹i m«n ph¸i xğa bá c¸c quy tğc, më réng m«n quy, cho phğp c¸c ğÖ tö gia nhËp c¸c m«n ph¸i kh¸c, ğÓ cïng nhau Ên chøng vâ häc, s¸ng lËp ra nh÷ng tuyÖt häc vâ c«ng míi l¹, trung h­ng ng· vâ l©m." };

    if (nFlag == 0) then
        tinsert(tbDes, format("§¸i nghÖ ğÇu s­, chuyÓn m«n ph¸i/toushiApplyEnter(%d)", nFactionNumb));
    elseif (nFlag == 1) then
        tinsert(tbDes, format("§¸i nghÖ ğÇu s­, b¸i nhËp m«n h¹/toushiTransEnter(%d)", nFactionNumb));
    elseif (nFlag == 2 and GetFactionNo() == nFactionNumb) then
        tbDes = {
            "<npc>: NÕu nh­ ng­¬i ğ· muèn gia nhËp bæn ph¶i, sau nµy ph¶i chuyªn t©m mµ khæ luyÖn, sím cğ ngµy ngé ra ğ­îc tuyÖt kü vâ häc" };
    else
        tbDes = { "<npc>: Xem ra, vŞ <sex> nµy mang g¸nh nÆng trªn vai trïng t©n chÊn h­ng vâ l©m." };
    end

    tinsert(tbDes, "KÕt thóc ğèi tho¹i/OnCancel");

    CreateTaskSay(tbDes);
end

function toushiApplyEnter(nFactionNumb)
    local nSex = GetSex();
    local szDes = "";
    local szHierarchTitle = toushiGetHierarchTitle(nFactionNumb);

    if (nSex == 1) then
        szDes =
        "{NÕu nh­ cğ ğ­îc tİn vËt cña bang chñ thiªn v­¬ng bang D­¬ng Anh th× cğ thÓ gia nhËp m«n ph¸i thiªn v­¬ng.}";
    end

    szDes = format(
        "<npc>: Vâ häc trong giíi vâ l©m cña chóng ta b¸c ğ¹i uyªn th©m, lµm s­ phô ta còng muèn më réng m«n quy ğÓ cïng nhau øng chİnh c¸c m«n ph¸i kh¸c, nay ta ğÓ ng­¬i rêi khái  bæn m«n ta còng cğ thÓ thu nhËn %s m«n ph¸i ğÖ tö . %s<enter>, con ğ­êng cÇu mong vâ häc qu¶ kh«ng dÔ dµng chót nµo, lµ mét s­ phô ta còng ph¶i thö th¸ch ng­¬i mét phen. NÕu nh­ ng­¬i cğ thÓ{{ğ¹t ğÕn cÊp 120}} h¬n n÷a cßn ph¶i t×m vÒ ğ©y cho bæn ph¸i tİn vËt ğ· bŞ thÊt l¹c tõ l©u {{ “Tİn VËt M«n Ph¸i”}}, s­ phô  míi cho ng­¬i h¹ s¬n.",
        toushiGetSexName(nSex),
        szDes);


    CreateTaskSay({ szDes,
        format("%s yªn t©m, ğÖ tö sÏ ğem tİn vËt vÒ ğ©y./OnCancel", szHierarchTitle),
        format("%s, Xin mêi xem ğ©y cğ ph¶i lµ tİn vËt m«n ph¸i?/toushiApplyMenPaiXinWu(%d)", szHierarchTitle,
            nFactionNumb),
        "KÕt thóc ğèi tho¹i/OnCancel"
    });
end

function toushiApplyMenPaiXinWu()
    local bFlag = toushiCheckApplyState();
    if (bFlag == TOUSHI_SUCCEED) then
        GiveItemUI("Tr×nh tİn vËt m«n ph¸i", "Bá tİn vËt m«n ph¸i vµo « trèng bªn d­íi.", "toushiApplyShowXinWu",
            "OnCancel", 1);
    else
        CreateTaskSay({ TB_TOUSHI_ERR_MSG[bFlag], "KÕt thóc ğèi tho¹i/OnCancel" });
    end
end

function toushiApplyShowXinWu()
    local nCount, nFIndex, nFGenre, nFDetail = 0, 0, 0, 0;
    for i = 0, 5 do
        for j = 0, 3 do
            nFIndex, nFGenre, nFDetail = FindItemEx(14, i, j);
            --Msg2Player(format("Idx: %d - G: %d - D: %d", nFIndex, nFGenre, nFDetail))
            if (nFIndex > 0 and nFGenre == 6 and nFDetail == 1679) then --trang bi, khoa vinh vien
                if (RemoveItem(nFIndex, 1) == 1) then
                    nCount = nCount + 1;
                    --Msg2Player("Giao Tİn vËt m«n ph¸i thµnh c«ng!")
                end
            end
        end
    end

    if (nCount > 0) then
        SetTask(TSK_TOUSHI_FLAG, 1);
        CreateTaskSay({
            "<npc>: §óng vËt nµy råi, ta phª chuÈn cho ng­¬i h¹ s¬n, ta mong r»ng ng­¬i sím cğ ngµy ngé ra ğ­îc tuyÖt kü vâ häc ğÓ quang ğ¹i ng· vâ l©m.",
            "§Ö tö nhÊt ğŞnh sÏ nghe lêi d¹y cña s­ phô/OnCancel" });
    else
        Talk(1, "", "Ng­¬i ğ­a g× cho ta vËy?");
    end
end

function toushiGetSexName(nSex)
    if (nSex == 0) then
        return "Nam";
    else
        return "N÷ ";
    end
end

function toushiGetHierarchTitle(nFactionNumb)
    if (nFactionNumb == 7 or nFactionNumb == 3) then           -- Îå¶ğ½Ì£¬ÌìÈÌ½Ì Îª½ÌÖ÷
        return TB_TOUSHI_HIERARCH[2];
    elseif (nFactionNumb == 1 or TB_TOUSHI_HIERARCH == 6) then -- ÌìÍõ°ï£¬Ø¤°ï Îª°ïÖ÷
        return TB_TOUSHI_HIERARCH[3];
    else                                                       -- ÆäËû ÎªÕÆğÅ
        return TB_TOUSHI_HIERARCH[1];
    end
end

function toushiCheckApplyState()
    if (GetLevel() < 120) then
        return TOUSHI_LVLERR;
    end

    if (GetFactionNo() == -1) then
        return TOUSHI_NOFACTERR;
    end

    return TOUSHI_SUCCEED;
end

function toushiCheckTransFactState(nFactionNumb, nStage)
    local bFlag       = GetTask(TSK_TOUSHI_FLAG);        -- »ñµğ×ªÍ¶Ê¦ğÅµÄ±ê¼Ç
    local nMyFacNumb  = GetFactionNo();                  -- »ñµğÔ­ğÅÅÉID
    local nSex        = GetSex();                        -- ½ÇÉ«ğÔ±ğ
    local nLastChange = GetTask(TSK_LAST_CHANGE_FACTION) -- ÉÏ´Î×ªÍ¶Ê¦ğÅ±ê¼Ç
    local nCurTime    = GetCurServerSec();

    if (bFlag == 0) then
        return TOUSHI_FLAGERR;
    end

    if (bFlag >= 2) then
        return TOUSHI_UNKNOWN;
    end

    if (nMyFacNumb == -1) then
        return TOUSHI_NOFACTERR;
    end
    --Bá giíi h¹n thêi gian chuyÓn m«n ph¸i - Modified By DinhHQ - 20110813
    -- if ((nCurTime - nLastChange) < TOUSHI_COLDDOWN_TIME) then
    --     return TOUSHI_TIMEERR;
    -- end

    if ((nSex == 0 and nFactionNumb == 4) or (nSex == 1 and nFactionNumb == 0)) then
        return TOUSHI_MALEERR;
    end

    if (nMyFacNumb == nFactionNumb) then
        return TOUSHI_EQFACTERR;
    end

    if (GetLevel() < 120) then
        return TOUSHI_LVLERR;
    end

    if (nStage == 2) then
        -- if (check_zhuansheng_league(LG_WLLSLEAGUE) == 1) then
        --     return TOUSHI_LGERR;
        -- end

        if (CheckEquip() > 0) then
            return TOUSHI_EQUIPERR;
        end
    end

    if (GetTask(TSK_TOUSHI_COUNT) == 0 and CalcFreeItemCellCount() < 2) then
        return TOUSHI_NOFREEBAG;
    end

    return TOUSHI_SUCCEED;
end

function toushiTransEnter(nFactionNumb)
    local bFlag           = toushiCheckTransFactState(nFactionNumb, 1);
    local nSex            = GetSex();
    local szHierarchTitle = toushiGetHierarchTitle(nFactionNumb);

    if (bFlag == TOUSHI_SUCCEED) then
        local szDes =
        "<npc> VŞ <sex> nµy, ng­¬i ğ· t×m ğ­îc ğ­êng ğÕn chç cña ta ğ©y, ta buéc ng­¬i ph¶i quªn ği nh÷ng tuyÖt häc mµ ng­¬i ğ· l·nh ngé ğ­îc tõ s­ phô cña ng­¬i. {{ ng­¬i ph¶i biÕt r»ng ğ· lµ gia nhËp bæn m«n, th× nh÷ng vâ c«ng ğ· häc tr­íc ğ©y kh«ng thÓ sö dông, ğ­¬ng nhiªn ta sÏ truyÒn thô vâ c«ng cña bæn ph¸i cho ng­¬i, ğ­a ng­¬i gia nhËp bæn ph¸i, ta tin t­ëng r»ng sÏ kh«ng tæn thÊt thÕ lùc.}} ng­¬i ğ· nghÜ kü ch­a ğğ, qu¶ thùc lµ ng­¬i muèn gia nhËp bæn ph¸i hay kh«ng?";

        if (nFactionNumb == 1 and nSex == 1) then
            szDes =
            "<npc> VŞ <sex> nµy, ng­¬i ğ· t×m ğ­îc ğ­êng ğÕn chç cña ta ğ©y, ta buéc ng­¬i ph¶i quªn ği nh÷ng tuyÖt häc mµ ng­¬i ğ· l·nh ngé ğ­îc tõ s­ phô cña ng­¬i. Cğ ğiÒu bæn ph¸i ta vèn dÜ kh«ng thu nhËn n÷ ğÖ tö, lµ v× sî r»ng n÷ ğÖ tö sÏ kh«ng lÜnh ngé ğ­îc c«ng ph¸p thuÇn d­¬ng cña bæn ph¸i. M·i cho ğÕn nöa n¨m tr­íc ğ©y bæn bang chñ v« t×nh ğ¹t ğ­îc mét ThiÕp Cæ Ph­¬ng, míi t×m ğ­îc c«ng ph¸p chÕ gi¶i. ChØ lµ bİ d­îc ë trong ph­¬ng thuèc nµy còng kh«ng lÊy g× æn ğŞnh cho lğm, cÇn ph¶i cğ bæn bang chñ truyÒn c«ng ğ· th«ng kinh m¹ch. NÕu nh­ ng­¬i cğ thÓ t×m ğ­îc {{ Tİn vËt cña bæn bang chñ }}, míi cğ thÓ gia nhËp bæn bang. Ngoµi ra {{ ng­¬i còng ph¶i biÕt r»ng ğ· lµ gia nhËp bæn m«n, th× nh÷ng vâ c«ng ğ· häc tr­íc ğ©y kh«ng thÓ sö dông, ğ­¬ng nhiªn ta sÏ truyÒn thô vâ c«ng cña bæn ph¸i cho ng­¬i, ğ­a ng­¬i gia nhËp bæn ph¸i, ta tin t­ëng r»ng sÏ kh«ng tæn thÊt thÕ lùc.}} ng­¬i ğ· nghÜ kü ch­a ğğ, qu¶ thùc lµ ng­¬i muèn gia nhËp bæn ph¸i hay kh«ng?";
        end

        CreateTaskSay({ szDes,
            format("%s, ta ğ· nghÜ kü råi, muèn gia nhËp m«n ph¸i cña bæn bang./toushiTransSureTodo(%d)", szHierarchTitle,
                nFactionNumb),
            format("%s, ğÓ ta suy nghÜ thªm/OnCancel", szHierarchTitle),
        });
    else
        CreateTaskSay({ TB_TOUSHI_ERR_MSG[bFlag], "KÕt thóc ğèi tho¹i/OnCancel" });
    end
end

function toushiTransSureTodo(nFactionNumb)
    local tbDescrib = { "<npc> Ng­¬i cÇn ph¶i lùa chän con ğ­êng chñ c«ng:" };

    for k, v in TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].tbSkillID[90] do
        if (type(v) == "table") then
            tbDescrib[getn(tbDescrib) + 1] = format("%s/toushiTransSure(%d, %d)", v[3], nFactionNumb, k);
        end
    end

    tbDescrib[getn(tbDescrib) + 1] = "Ta cÇn ph¶i suy nghÜ thªm/OnCancel";

    CreateTaskSay(tbDescrib);
end

function toushiTransSure(nFactionNumb, nMagicIdx)
    local bFlag = toushiCheckTransFactState(nFactionNumb, 2);
    local nSex = GetSex();
    local szLog = "";

    if (bFlag == TOUSHI_SUCCEED) then
        if (nFactionNumb == 1 and nSex == 1) then
            local ndnumbeibao = GetItemCount(0, 6, 1680)

            if (ndnumbeibao >= 1) then
                DelItem(1, 0, 6, 1680)
            else
                CreateTaskSay({ TB_TOUSHI_ERR_MSG[TOUSHI_TWFEMALEERR], "KÕt thóc ğèi tho¹i/OnCancel" });
                return TOUSHI_TWFEMALEERR;
            end

            szLog = "Giao tİn vËt cña D­¬ng Anh thµnh c«ng";
        end

        local bTrans = toushiDoTransPlayer(GetFactionNo(), nFactionNumb, nMagicIdx, szLog); -- ´¦Àí½ÇÉ«Ïà¹ØÊôğÔ

        -- ÉèÖğÀäÈ´Ê±¼ä
        local nCurTime = GetCurServerSec();
        SetTask(TSK_LAST_CHANGE_FACTION, nCurTime);
    else
        CreateTaskSay({ TB_TOUSHI_ERR_MSG[bFlag], "KÕt thóc ğèi tho¹i/OnCancel" });
    end
end

function toushiDoTransPlayer(nMyFacNumb, nFactionNumb, nMagicIdx, szLog)
    LeaveTeam(); -- ÏÈ×öÀë¶Ó²Ù×÷

    local nLevel        = GetLevel();
    local tbOldSkill    = TB_DAIYITOUSHI_FACTS[nMyFacNumb + 1].tbSkillID;
    local tbNewSkill    = TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].tbSkillID;
    local tb90SkillExp  = {};
    local tb120SkillExp = {};
    local tb150SkillExp = {};

    zhuansheng_clear_skill(0, 0)       -- µ÷Óğ×ªÉú Ï´¼¼ÄÜµğº¯Êı

    for i = 1, getn(tbOldSkill[90]) do -- ±£ÁôÔ­¼¼ÄÜÊìÁ·¶È
        if (type(tbOldSkill[90][i]) == "table") then
            local nMagicLvl, nMagicExp, nMagicExpPre = toushiGetMagicProp(tbOldSkill[90][i][1]);

            if (nMagicLvl == 0) then
                nMagicLvl = 1;
            end

            if (tb90SkillExp.nShuliandu == nil) then
                tb90SkillExp.nShuliandu = {};
            end
            if (nMagicLvl > 0) then
                tb90SkillExp.nShuliandu[getn(tb90SkillExp.nShuliandu) + 1] = {
                    ["nMagicLvl"] = nMagicLvl,
                    ["nMagicExpPre"] = nMagicExpPre
                };
            end
        else
            if (HaveMagic(tbOldSkill[90][i]) >= 0) then
                tb90SkillExp.nJinengdian = 1;
            end
        end
    end

    --sort(tb90SkillExp.nShuliandu, toushiSortSkillsExp);									-- ¶ÔÊìÁ·¶ÈµÈ¼¶ğ­Ñé×öÅÅğò£¬¸øÖ÷¹ğÂ·Ïß¼Ó×î¸ßÊìÁ·¶È

    if (HaveMagic(tbOldSkill[120][1][1]) > 0) then -- ±£Áô120¼¶¼¼ÄÜÊìÁ·¶È
        local nMagicLvl, nMagicExp, nMagicExpPre = toushiGetMagicProp(tbOldSkill[120][1][1]);

        if (nMagicLvl > 0) then
            tb120SkillExp = { ["nMagicLvl"] = nMagicLvl, ["nMagicExpPre"] = nMagicExpPre };
        end
    end

    for i = 1, getn(tbOldSkill[150]) do
        if (type(tbOldSkill[150][i]) == "table") then
            local nMagicLvl, nMagicExp, nMagicExpPre = toushiGetMagicProp(tbOldSkill[150][i][1]);
            --Msg2Player("1-"..nMagicLvl)
            -- if (nMagicLvl == 0) then
            --     nMagicLvl = 1;
            -- end
            if (tb150SkillExp.nShuliandu == nil) then
                tb150SkillExp.nShuliandu = {};
            end
            if (nMagicLvl > 0) then
                tb150SkillExp.nShuliandu[getn(tb150SkillExp.nShuliandu) + 1] = {
                    ["nMagicLvl"] = nMagicLvl,
                    ["nMagicExpPre"] = nMagicExpPre
                };
            end
        else
            if (HaveMagic(tbOldSkill[150][i]) >= 0) then
                tb150SkillExp.nJinengdian = 1;
            end
        end
    end

    --sort(tb150SkillExp.nShuliandu, toushiSortSkillsExp);									-- ¶ÔÊìÁ·¶ÈµÈ¼¶ğ­Ñé×öÅÅğò£¬¸øÖ÷¹ğÂ·Ïß¼Ó×î¸ßÊìÁ·¶È
    for i = 1, getn(tb150SkillExp.nShuliandu) do
        print(tb150SkillExp.nShuliandu[i].nMagicLvl, tb150SkillExp.nShuliandu[i].nMagicExpPre)
    end

    for nSkillLvl, tbLvlSkill in tbOldSkill do -- Éğ³ığÉğÅÅÉ¼¼ÄÜ
        for i = 1, getn(tbLvlSkill) do
            if (type(tbLvlSkill[i]) == "table") then
                DelMagic(tbLvlSkill[i][1]);
            else
                DelMagic(tbLvlSkill[i]);
            end
        end
    end

    --for nSkillLvl, tbLvlSkill in tbNewSkill do							-- Ñ§Ï°ğÂğÅÅÉ¼¼ÄÜ
    for nSkillLvl = 10, 150, 10 do -- Ñ§Ï°ğÂğÅÅÉ¼¼ÄÜ
        local tbLvlSkill = tbNewSkill[nSkillLvl];
        if (tbLvlSkill) then
            if (nSkillLvl == 150) then
                toushiAdd150Skill(nMyFacNumb, nFactionNumb, tbOldSkill, tb150SkillExp, tbLvlSkill, nMagicIdx);
            elseif (nSkillLvl == 120) then                                                                   -- Èç¹ûÊÇ120¼¶¼¼ÄÜ,Ö±½ÓÉèÖğÎªğÉ120¼¶¼¼ÄÜµÈ¼¶ºÍÊìÁ·¶È
                toushiAdd120Skill(tb120SkillExp, tbLvlSkill[1][1]);
            elseif (nSkillLvl == 90) then                                                                    -- Èç¹ûÊÇ90¼¶¼¼ÄÜ
                toushiAdd90Skill(nMyFacNumb, nFactionNumb, tbOldSkill, tb90SkillExp, tbLvlSkill, nMagicIdx); --
            else                                                                                             -- Èç¹ûÊÇ¼Óµğ¼¼ÄÜ
                for i = 1, getn(tbLvlSkill) do
                    AddMagic(tbLvlSkill[i]);
                end
            end
        end
    end


    local nOldSeries = GetSeries();
    local nNewSeries = TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nSeries;

    toushiAddWuXingSkill(nOldSeries, nNewSeries); -- Èç¹ûÓğÎåğğ¼¼ÄÜ£¬ÄÇğ´¸ü»»ÎªğÂÎåğğ

    zhuansheng_clear_prop(0, 0, nNewSeries)       -- µ÷Óğ×ªÉú Ï´Ç±ÄÜµğº¯Êı,ÒÔğÂÎåğğÏ´µğ,¸÷Îåğğ»ù´¡Ç±ÄÜµğ²»Ò»Ñù

    SetSeries(nNewSeries);                        -- ×ª»»½ÇÉ«ÎåğğÊôğÔ

    --toushiResetMaxLife(nNewSeries, nLevel);                             -- ½ÇÉ«¶ÔÓ¦ÎåğğµÈ¼¶ µÄ»ù´¡ÑªÁ¿
    --toushiResetMaxMana(nNewSeries, nLevel);                             -- ½ÇÉ«¶ÔÓ¦ÎåğğµÈ¼¶ µÄ»ù´¡ÄÚÁ¦
    SetTask(TASK_DAOTAYTUY, 1); --

    --SetLastFactionNumber(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nFacNumb); -- ½ÇÉ«ğÅÅÉID

    SetTask(TB_DAIYITOUSHI_FACTS[nMyFacNumb + 1].nTaskId_Fact, 0); -- Çå½ÇÉ«Ô­ğÅÅÉÈÎÎñ±äÁ¿

    if (GetFaction() ~= "Míi nhËp giang hå ") then
        SetFaction(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].szFacName);                        -- ½ÇÉ«ğÅÅÉğû³Æ

        SetCamp(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nCamp);                               -- ¶ÔÓ¦ğÅÅÉÕóÓª
        SetCurCamp(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nCamp);                            -- ¶ÔÓ¦ğÅÅÉÕóÓª

        SetTask(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nTaskId_Fact, TOUSHI_FACTION_HUISHI); -- ğÅÅÉÈÎÎñ±äÁ¿,»ØÊ¦
    else
        SetTask(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nTaskId_Fact, TOUSHI_FACTION_CHUSHI); -- ğÅÅÉÈÎÎñ±äÁ¿,³öÊ¦
    end

    SetRank(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nRankId); -- ½ÇÉ«Í·ÏÎ

    SetTask(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nTaskId_137,
        TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].nValue_137); -- ğÅÅÉÈÎÎñ±äÁ¿,³öÊ¦

    --SetTask(144, 0);                                       --ğâ·ÑÏ´µğ
    SetTask(TSK_TOUSHI_FLAG, 0);                              -- Çå´øÒÕÍ¶Ê¦³É¹¦±ê¼Ç
    SetTask(TSK_TOUSHI_COUNT, GetTask(TSK_TOUSHI_COUNT) + 1); -- ÉèÖğ´øÒÕÍ¶Ê¦³É¹¦´ÎÊı

    --if (GetByte(GetTask(TSK_ZHENPAILINGDAN_USECNT), 3) ~= 1) then
    --SetItemBindState(AddItem(unpack(TB_TOUSHI_ZHENPAILINGDAN)), -2);			-- ÔùËÍµÄÎïÆ·ÎªÓÀğğ°ó¶¨
    --SetItemBindState(AddItem(unpack(TB_TOUSHI_ZHENPAILINGYAO)), -2);
    --	SetTask(TSK_ZHENPAILINGDAN_USECNT, SetByte(GetTask(TSK_ZHENPAILINGDAN_USECNT), 3, 1))
    --	szLog = szLog..", thu ğ­îc trÊn ph¸i linh ğ¬n vµ trÊn ph¸i linh d­îc";
    --end


    CreateTaskSay({
        "<npc>NÕu nh­ ng­¬i ğ· muèn gia nhËp bæn ph¶i, sau nµy ph¶i chuyªn t©m mµ khæ luyÖn, sím cğ ngµy ngé ra ğ­îc tuyÖt kü vâ häc. §©y lµ hai viªn linh d­îc cña bæn ph¸i, cğ thÓ gióp ng­¬i n©ng cao c«ng lùc, h·y nhËn lÊy mµ sö dông.",
        "KÕt thóc ğèi tho¹i/OnCancel" });

    Msg2Faction(format(TB_DAIYITOUSHI_FACTS[nFactionNumb + 1].szMsg, GetName()));

    -- WriteLog(format("[%s]\t%s\tName:%s\tAccount:%s\tLevel:%d,OldFactionId:%d,NewFactionId:%d,MainMagic:%d,%s",
    -- 			"§¸i nghÖ ğÇu s­",
    -- 			GetLocalDate("%Y-%m-%d %X"),
    -- 			GetName(), GetAccount(),
    -- 			GetLevel(), nMyFacNumb, nFactionNumb,nMagicIdx,
    -- 			"Xö lı chuyÓn m«n ph¸i thµnh c«ng"..szLog));
    KickOutSelf();
end

function toushiAddWuXingSkill(nOldSeries, nNewSeries)
    if (nOldSeries ~= nNewSeries) then
        local nMagicLvl1 = HaveMagic(TB_WUXING_SKILL_ID[nOldSeries + 1].nMagicId1);
        local nMagicLvl2 = HaveMagic(TB_WUXING_SKILL_ID[nOldSeries + 1].nMagicId2);
        if (nMagicLvl1 ~= -1) then
            DelMagic(TB_WUXING_SKILL_ID[nOldSeries + 1].nMagicId1);
            AddMagic(TB_WUXING_SKILL_ID[nNewSeries + 1].nMagicId1);
        elseif (nMagicLvl2 >= 1) then
            local nMagicExp2 = GetSkillExp(TB_WUXING_SKILL_ID[nOldSeries + 1].nMagicId2);
            DelMagic(TB_WUXING_SKILL_ID[nOldSeries + 1].nMagicId2);
            AddMagic(TB_WUXING_SKILL_ID[nNewSeries + 1].nMagicId2, nMagicLvl2);
            IncSkillExp(TB_WUXING_SKILL_ID[nNewSeries + 1].nMagicId2, nMagicExp2, 1);
        end
    end
end

function toushiAdd120Skill(tbOldSkill, nNewSkill)
    if (tbOldSkill.nMagicLvl and tbOldSkill.nMagicLvl > 0) then
        SetTask(LV120_SKILL_ID, nNewSkill);
        AddMagic(nNewSkill, tbOldSkill.nMagicLvl);
        IncSkillExp(nNewSkill, tbOldSkill.nMagicExpPre, 1, 1);
    end
end

function toushiAdd90Skill(nMyFacNumb, nFactionNumb, tbOldSkill, tbOldSkillExp, tbNewSkill, nMagicIdx)
    local nOldNumb   = nMyFacNumb + 1;
    local nNewNumb   = nFactionNumb + 1;
    local tbAddSkill = {};


    if (GetTask(TB_DAIYITOUSHI_FACTS[nOldNumb].nTaskId_90Skill) == TOUSHI_90SKILL_TASK) then
        -- Èç¹û90¼¶¼¼ÄÜÈÎÎñ±äÁ¿ÎªÍê³ÉµÄ´¦Àí
        SetTask(TB_DAIYITOUSHI_FACTS[nNewNumb].nTaskId_90Skill, TOUSHI_90SKILL_TASK); -- ğÂğÅÅÉ¼¼ÄÜÈÎÎñÉèÖğÎªÍê³É

        local nCount = getn(tbOldSkillExp.nShuliandu);
        local nAddCnt = 0;

        if (nCount > 0) then
            nAddCnt       = nAddCnt + 1;
            tbAddSkill[1] = { tbNewSkill[nMagicIdx][1], tbOldSkillExp.nShuliandu[nAddCnt].nMagicLvl, tbOldSkillExp
                .nShuliandu[nAddCnt].nMagicExpPre }; -- ÏÈ½«Ö÷¹ğÂ·Ïß¼¼ÄÜ¼Ó¼ÇÂ¼
        else
            tbAddSkill[1] = { tbNewSkill[90][nMagicIdx], 1, 0 };
        end

        for k = 1, getn(tbNewSkill) do
            if (type(tbNewSkill[k]) == "table") then                   -- Èç¹ûÊÇÊìÁ·¶È¼¼ÄÜ
                if (tbNewSkill[k][1] ~= tbNewSkill[nMagicIdx][1]) then -- °´Ë³ğò¸øÓëÊìÁ·¶È
                    -- Èç¹ûğÉğÅÅÉ¼¼ÄÜÊìÁ·¶È²»¹»°²ÅÅ
                    if (nCount > nAddCnt) then                         -- Ôò¸øÓë¼¼ÄÜÎªÒ»¼¶£¬ÊìÁ·¶ÈÎª0
                        nAddCnt = nAddCnt + 1;
                        tbAddSkill[getn(tbAddSkill) + 1] = { tbNewSkill[k][1], tbOldSkillExp.nShuliandu[nAddCnt]
                            .nMagicLvl, tbOldSkillExp.nShuliandu[nAddCnt].nMagicExpPre };
                    else
                        tbAddSkill[getn(tbAddSkill) + 1] = { tbNewSkill[k][1], 1, 0 };
                    end
                end
            else                                                  -- Èç¹ûÊÇ¼Óµğ¼¼ÄÜ£¬Ôò¼ÇÂ¼Òª¼Ó
                tbAddSkill[getn(tbAddSkill) + 1] = tbNewSkill[k]; -- ÔÚ¸øÓëµÄÊ±ºòÖ±½Ó¸ø0¼¶
            end
        end
    else
        local tbOtherMagic  = { tbShuliandu = {}, tbJinengdian = {} };
        local nCount        = getn(tbOldSkillExp.nShuliandu);
        local nAddCnt       = 0;
        local nAddJinengCnt = 0;

        if (nCount > 0) then
            nAddCnt = nAddCnt + 1;
            tbAddSkill[1] = { tbNewSkill[nMagicIdx][1], tbOldSkillExp.nShuliandu[1].nMagicLvl, tbOldSkillExp.nShuliandu
                [1].nMagicExpPre }; -- ÏÈ½«Ö÷¹ğÂ·Ïß¼¼ÄÜ¼Ó¼ÇÂ¼
        else
            tbOtherMagic.tbShuliandu[getn(tbOtherMagic.tbShuliandu) + 1] = { tbNewSkill[nMagicIdx][1], 1, 0 };
        end

        for k = 1, getn(tbNewSkill) do
            if (type(tbNewSkill[k]) == "table") then
                if (tbNewSkill[k][1] ~= tbNewSkill[nMagicIdx][1]) then
                    if (nCount > nAddCnt) then
                        nAddCnt = nAddCnt + 1;
                        tbAddSkill[getn(tbAddSkill) + 1] = { tbNewSkill[k][1], tbOldSkillExp.nShuliandu[nAddCnt]
                            .nMagicLvl, tbOldSkillExp.nShuliandu[nAddCnt].nMagicExpPre };
                    else
                        tbOtherMagic.tbShuliandu[getn(tbOtherMagic.tbShuliandu) + 1] = { tbNewSkill[k][1], 1, 0 };
                    end
                end
            else
                if (tbOldSkillExp.nJinengdian) then
                    tbAddSkill[getn(tbAddSkill) + 1] = tbNewSkill[k];
                    nAddJinengCnt                    = nAddJinengCnt + 1;
                else
                    tbOtherMagic.tbJinengdian[getn(tbOtherMagic.tbJinengdian) + 1] = tbNewSkill[k];
                end
            end
        end

        if (getn(tbOtherMagic.tbJinengdian) > 0) then
            for j = 1, getn(tbOtherMagic.tbJinengdian) do
                if (nCount - nAddCnt <= 0) then
                    break
                end

                nAddCnt                          = nAddCnt + 1;
                tbAddSkill[getn(tbAddSkill) + 1] = tbOtherMagic.tbJinengdian[j];
            end
        end

        if (getn(tbOtherMagic.tbShuliandu) > 0) then
            for j = 1, getn(tbOtherMagic.tbShuliandu) do
                if (not tbOldSkillExp.nJinengdian or tbOldSkillExp.nJinengdian - nAddJinengCnt <= 0) then
                    break
                end
                nAddJinengCnt                    = nAddJinengCnt + 1;
                tbAddSkill[getn(tbAddSkill) + 1] = tbOtherMagic.tbShuliandu[j];
            end
        end
    end

    SetTask(TB_DAIYITOUSHI_FACTS[nOldNumb].nTaskId_90Skill, 0); -- È¡ÏûÔ­Ö°Òµ90¼¶¼¼ÄÜÈÎÎñ

    for k = 1, getn(tbAddSkill) do
        if (type(tbAddSkill[k]) == "table") then
            AddMagic(tbAddSkill[k][1], tbAddSkill[k][2]);
            IncSkillExp(tbAddSkill[k][1], tbAddSkill[k][3], 1, 1);
        else
            AddMagic(tbAddSkill[k]);
        end
    end
end

function toushiAdd150Skill(nMyFacNumb, nFactionNumb, tbOldSkill, tbOldSkillExp, tbNewSkill, nMagicIdx)
    local nOldNumb       = nMyFacNumb + 1;
    local nNewNumb       = nFactionNumb + 1;
    local tbAddSkill     = {};
    local tbFactStepList = {
        [5] = 6,
        [2] = 7,
        [4] = 7,
        [3] = 5,
        [6] = 9,
        [7] = 9,
        [0] = 6,
        [8] = 5,
        [1] = 4,
        [9] = 5,
    }

    --if (mod(floor(GetTask(TSK_LV150_SKILL) / 100), 100) >= TOUSHI_150SKILL_TASKSTATE) then --fix
    -- Èç¹û150¼¶¼¼ÄÜÈÎÎñ±äÁ¿ÎªÍê³ÉµÄ´¦Àí
    --¸Ä±ä150ÈÎÎñÍê³ÉµÄÈÎÎñ±äÁ¿£¬ğÅÅÉÖ§ÏßµÄğŞ¸Ä
    --SetTask(TSK_LV150_SKILL, TOUSHI_150SKILL_TASKSTATE*100+tbFactStepList[nFactionNumb]) -- fix

    local nCount         = getn(tbOldSkillExp.nShuliandu);
    local nAddCnt        = 0;

    if (nCount > 0) then
        nAddCnt       = nAddCnt + 1;
        tbAddSkill[1] = { tbNewSkill[nMagicIdx][1], tbOldSkillExp.nShuliandu[nAddCnt].nMagicLvl, tbOldSkillExp
            .nShuliandu[nAddCnt].nMagicExpPre }; -- ÏÈ½«Ö÷¹ğÂ·Ïß¼¼ÄÜ¼Ó¼ÇÂ¼
    else
        tbAddSkill[1] = { tbNewSkill[nMagicIdx][1], 1, 0 };
    end
    print(150, tbAddSkill[1][1], tbAddSkill[1][2], tbAddSkill[1][3])

    for k = 1, getn(tbNewSkill) do
        if (type(tbNewSkill[k]) == "table") then                   -- Èç¹ûÊÇÊìÁ·¶È¼¼ÄÜ
            if (tbNewSkill[k][1] ~= tbNewSkill[nMagicIdx][1]) then -- °´Ë³ğò¸øÓëÊìÁ·¶È
                -- Èç¹ûğÉğÅÅÉ¼¼ÄÜÊìÁ·¶È²»¹»°²ÅÅ
                if (nCount > nAddCnt) then                         -- Ôò¸øÓë¼¼ÄÜÎªÒ»¼¶£¬ÊìÁ·¶ÈÎª0
                    nAddCnt = nAddCnt + 1;
                    tbAddSkill[getn(tbAddSkill) + 1] = { tbNewSkill[k][1], tbOldSkillExp.nShuliandu[nAddCnt].nMagicLvl,
                        tbOldSkillExp.nShuliandu[nAddCnt].nMagicExpPre };
                    print(tbNewSkill[k][1], tbOldSkillExp.nShuliandu[nAddCnt].nMagicLvl,
                        tbOldSkillExp.nShuliandu[nAddCnt].nMagicExpPre)
                else
                    tbAddSkill[getn(tbAddSkill) + 1] = { tbNewSkill[k][1], 1, 0 };
                    print(tbNewSkill[k][1], 1, 0)
                end
            end
        else                                                  -- Èç¹ûÊÇ¼Óµğ¼¼ÄÜ£¬Ôò¼ÇÂ¼Òª¼Ó
            tbAddSkill[getn(tbAddSkill) + 1] = tbNewSkill[k]; -- ÔÚ¸øÓëµÄÊ±ºòÖ±½Ó¸ø0¼¶
        end
    end
    --	else
    --		return
    --	end

    for k = 1, getn(tbAddSkill) do
        if (type(tbAddSkill[k]) == "table") then
            AddMagic(tbAddSkill[k][1], tbAddSkill[k][2]);
            IncSkillExp(tbAddSkill[k][1], tbAddSkill[k][3], 1, 1);
        else
            AddMagic(tbAddSkill[k]);
        end
    end
end

function toushiGetMagicProp(nMagicId)
    --Msg2Player(nMagicId)
    local nMagicLvl = HaveMagic(nMagicId);

    --Msg2Player(nMagicLvl)
    if (nMagicLvl == -1) then
        return 0, 0, 0;
    end

    if (nMagicLvl <= 0) then
        nMagicLvl = 1;
    end

    -- if (nMagicId > 1054 and nMagicLvl > 29) then
    -- 	nMagicLvl = 29;
    -- end
    if (nMagicLvl > 20) then
        nMagicLvl = 20;
    end

    local nMagicExp    = GetSkillExp(nMagicId);
    local nMagicExpPre = GetSkillExp(nMagicId);

    return nMagicLvl, nMagicExp, nMagicExpPre;
end

function zhuansheng_clear_skill(nlevel, npoint)
    local nskill1 = HaveMagic(210);    --??ÁôÇáš?
    local nskill2 = HaveMagic(400);    --??Áô?°??¸???????
    local nallskill = RollbackSkill(); -- Ç??ıÍ?????ÄÜ??????????ÄÜ??
    if (nskill1 ~= -1) then            --????Çáš?
        nallskill = nallskill - nskill1;
        AddMagic(210, nskill1);
    end

    if (nskill2 ~= -1) then --?????°??¸???????
        nallskill = nallskill - nskill2;
        AddMagic(400, nskill2);
    end
    --nallskill = nallskill - (nlevel -1)	--×?ÉúÖ????ôÉı????¸?Óë?Ä??ÄÜ??
    AddMagicPoint(npoint + nallskill);
end

function toushiSortSkillsExp(tb1, tb2)
    if (tb1.nMagicLvl ~= tb2.nMagicLvl) then
        return tb1.nMagicLvl > tb2.nMagicLvl;
    else
        return tb1.nMagicExpPre > tb2.nMagicExpPre;
    end
end

function zhuansheng_clear_prop(nlevel, npoint, nseries)
    if (not nseries) then
        nseries = GetSeries();
    end

    nseries = nseries + 1;
    local Utask88 = GetTask(88) -- ?????Ö?äÇ?ÄÜÖ?Ö??¨task(88)?Ç?ÎÎ?Ö?Ö??Ó?????ÄÁ?Á???Éí?¨???Š
    AddProp(100)                -- Î??Ü?â??Ó?Î´?Ö?äÇ?ÄÜ???ÉšŠ??¸´?Ä???ËÇé?ö??Ôİ???°????¸?Ë?100??

    AddStrg(TB_BASE_STRG[nseries] - GetStrg(1) + GetByte(Utask88, 1));
    AddDex(TB_BASE_DEX[nseries] - GetDex(1) + GetByte(Utask88, 2));
    AddVit(TB_BASE_VIT[nseries] - GetVit(1) + GetByte(Utask88, 3));
    AddEng(TB_BASE_ENG[nseries] - GetEng(1) + GetByte(Utask88, 4));
    local nallprop = GetProp();
    --nallprop = nallprop - (nlevel - 1) * 5;	--×?ÉúÖ????ôÉı????¸?Óë?ÄÇ?ÄÜ??
    AddProp(npoint - 100);
end

function toushiResetMaxLife(nSeries, nLevel, nExtProp)
    if (TB_TOUSHI_PLAYERBASE[nSeries] and nLevel > 1) then
        local tb = TB_TOUSHI_PLAYERBASE[nSeries];
        local nBaseLife = (nLevel - 1) * tb.lifeperlvl + tb.imaxlife;


        NPCINFO_SetMaxLife(nBaseLife);
        return nBaseLife;
    end
end

function toushiResetMaxMana(nSeries, nLevel)
    if (TB_TOUSHI_PLAYERBASE[nSeries] and nLevel > 1) then
        local tb = TB_TOUSHI_PLAYERBASE[nSeries];
        local nBaseMana = (nLevel - 1) * tb.manaperlvl + tb.imaxinner;


        NPCINFO_SetMaxMana(nBaseMana);
        return nBaseMana;
    end
end

function CheckEquip()
    local nCount = 0;
    for i = 0, 10 do
        local nItemIdx = GetPOItem(2, i)
        if (nItemIdx > 0) then
            nCount = nCount + 1;
        end
    end

    return nCount;
end

function OnCancel()
end
