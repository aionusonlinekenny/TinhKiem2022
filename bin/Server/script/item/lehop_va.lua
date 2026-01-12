tbItem = {
    { "Lôc Thñy Tinh",          4, 240 },
    { "Tinh Hång B¶o Th¹ch",    4, 353 },
    { "LÖnh bµi Phong L¨ng §é", 4, 489 },
    { "Tiªn Th¶o Lé",           6, 71 },
    { "Tiªn Th¶o Lé §Æc BiÖt",  6, 1182 },
    { "ThiÕt La H¸n",           6, 23 },
    { "Vâ L©m MËt TÞch",        6, 26 },
    { "TÈy Tñy Kinh",           6, 22 },
}

function main(nItemIdx)
    local nIndex = RANDOM(1, 8)
    if (CheckRoom(2, 2) == 0) then
        Talk(1, "", 12266)
        return 0
    end

    LockItem(AddItemEx(tbItem[nIndex][2], tbItem[nIndex][3], 0, 0, 0, 0, 0), -2)
    Msg2Player(format("NhËn ®­îc 1 %s", tbItem[nIndex][1]))
    RemoveItem(nItemIdx,1);
    return 1
end
