Include("\\script\\header\\event_head.lua")
Include("\\script\\lib\\award.lua")

tbItem = {
    {tbProp = {4, 240, 0}, szName = "Lôc Thñy Tinh", nRate = 18},
    {tbProp = {4, 238, 0}, szName = "Lam Thñy Tinh", nRate = 16},
    {tbProp = {4, 239, 0}, szName = "Tö Thñy Tinh", nRate = 13},
    {tbProp = {4, 353, 0}, szName = "Tinh Hång B¶o Th¹ch", nRate = 12},
    {tbProp = {6, 1182, 0}, szName = "Tiªn Th¶o Lé §Æc BiÖt", nRate = 11},
    {tbProp = {6, 71, 0}, szName = "Tiªn Th¶o Lé", nRate = 9},
    {tbProp = {6, 23, 0}, szName = "ThiÕt La H¸n", nRate = 7},
    {tbProp = {6, 22, 0}, szName = "TÈy Tñy Kinh", nRate = 5},
    {tbProp = {6, 26, 0}, szName = "Vâ L©m MËt TÞch", nRate = 4},
    --{ tbProp = { 6, 1126, 0 }, szName = "BÝ quyÕt kü n¨ng cÊp 120", nRate = 3 },
    -- { tbProp = { 6, 2433, 0 },      szName = "§¹i Thµnh BÝ KÝp 90",   nRate = 2 },
    {tbProp = {6, 4815, 0}, szName = "B¹ch Kim LÖnh", nRate = 2},
    {tbProp = {6, 2434, 0}, szName = "§¹i Thµnh BÝ KÝp 120", nRate = 1},
    {tbProp = {0, 10, 6, 5, 0, 0}, szName = "B«n Tiªu", nRate = 0.1}
}

function main(nItemIndex)
    if (IsEventActive() ~= 1) then
        Talk(1, "", "Kh«ng trong thêi gian ho¹t ®éng!")
        return 1
    end

    if (CalcFreeItemCellCount() < 10) then
        Say("CÇn tèi thiÓu 10 « trèng, xin h·y s¾p xÕp l¹i hµnh trang tr­íc ®·!")
        return 1
    end
    RemoveItem(nItemIndex, 1)

    AddStackExp(EVENT_EXP1)
    Msg2Player(format("NhËn ®­îc <color=yellow>%d<color> ®iÓm kinh nghiÖm.", EVENT_EXP1))

    GiveRandom(tbItem)
end
