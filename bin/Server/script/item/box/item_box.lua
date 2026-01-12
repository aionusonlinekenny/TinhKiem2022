Include("\\script\\global\\droprate\\SourceDrop.lua");

function main(nItemIndex)
    if (CalcFreeItemCellCount() < 10) then
        Say("CÇn tèi thiÓu 10 « trèng, xin h·y s¾p xÕp l¹i hµnh trang tr­íc ®·!");
        return 1;
    end;

    RemoveItem(nItemIndex, 1)
    local nLuck = RANDOM(500)
    if (nLuck >= 495) then
        local nItemIdx = AddGoldItem(RANDOM(1, 146))
        AddCountNews(format(
            "Ng­êi ch¬i <color=yellow>%s<color> më <color=yellow>%s<color> may m¾n nhËn ®­îc <color=yellow>%s<color>!",
            GetName(), GetItemName(nItemIndex), GetItemName(nItemIdx)), 1)
        Msg2SubWorld(format(
            "Ng­êi ch¬i <color=yellow>%s<color> më <color=yellow>%s<color> may m¾n nhËn ®­îc <color=yellow>%s<color>!",
            GetName(), GetItemName(nItemIndex),
            GetItemName(nItemIdx)))
    else
        local nGenre  = RANDOM(1, getn(KindItem)); --KindFiveElement[1]
        local nDetail = KindItem[nGenre][1];
        local nParti  = RANDOM(KindItem[nGenre][2], KindItem[nGenre][3]);
        local nSeries = RANDOM(0, 4);

        AddItemID(AddItem(0, 0, nDetail, nParti, 10, nSeries, 100, 10, RANDOM(9, 10), RANDOM(9, 10), RANDOM(9, 10),
            RANDOM(9, 10),
            RANDOM(9, 10)))
    end
end
