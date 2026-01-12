Include("\\script\\header\\event_head.lua");

function main(nItemIndex)
    if (IsEventActive() ~= 1) then
        Talk(1, "", "Kh«ng trong thêi gian ho¹t ®éng!");
        return 1
    end

    if (CalcFreeItemCellCount() < 10) then
        Say("CÇn tèi thiÓu 10 « trèng, xin h·y s¾p xÕp l¹i hµnh trang tr­íc ®·!");
        return 1;
    end;
    RemoveItem(nItemIndex, 1)
    local nIdx = RANDOM(1662, 1665)

    local nItemIdx = AddItem(0, 6, nIdx, 0, 0, 0, 0);
    SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0);
    AddItemID(nItemIdx);
    Msg2Player("NhËn ®­îc 1 c¸i " .. GetItemName(nItemIdx) .. ".")
end
