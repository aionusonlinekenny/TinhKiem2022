function GiveRandom(tbItem)
    if tbItem == nil then
        return 0
    end
    local rtotal = 100000
    local rcur = RANDOM(1, rtotal);
    local rstep = 0;

    --Msg2Player(rcur)
    for i = 1, getn(tbItem) do
        rstep = rstep + floor(tbItem[i].nRate * rtotal / 100);
        --Msg2Player(rstep)
        if (rcur <= rstep) then
            tbProp = tbItem[i].tbProp
            local nItemGenre = tbProp[1]
            local nItemDetail = tbProp[2]
            local nItemPartial = tbProp[3] or -1
            local nItemSeries = tbProp[4] or -1
            local nItemLevel = tbProp[5] or -1
            local nItemLuck = tbProp[6] or -1
            local nItemLuckLevel = tbProp[7] or -1

            local nItemIndex = AddItemEx(nItemGenre, nItemDetail, nItemPartial, nItemSeries, nItemLevel, nItemLuck,
                nItemLuckLevel)

            local szItemName = tbItem[i].szName or GetItemName(nItemIndex)
            Msg2Player(format("NhËn ®­îc <color=yellow>1 %s<color>.", szItemName))

            break
        end
    end

    return 1
end
