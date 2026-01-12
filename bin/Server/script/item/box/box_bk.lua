Include("\\script\\lib\\worldlibrary.lua");
Include("\\script\\global\\droprate\\SourceDrop.lua");

function main(nItemIndex)
    if (CalcFreeItemCellCount() < 10) then
        Say("CÇn tèi thiÓu 10 « trèng, xin h·y s¾p xÕp l¹i hµnh trang tr­íc ®·!");
        return 1;
    end;

    RemoveItem(nItemIndex, 1);
    local nItemIdx = AddPlatinaItem(RANDOM(1, 140), 10);
    local szMesg = "";

    szMesg = format(
        "Ng­êi ch¬i <color=yellow>%s<color> më <color=yellow>%s<color> may m¾n nhËn ®­îc <color=yellow>%s<color>!",
        GetName(), GetItemName(nItemIndex), GetItemName(nItemIdx))
    AddCountNews(szMesg, 1)
    Msg2SubWorld(szMesg)
    ExchangeLog(szMesg)
    
    return 0
end

function ExchangeLog(szMsg)
    local szAccount = GetAccount()
    local szName = GetName()
    local dwID = GetUUID()
    local nLevel = GetLevel()
    local nFaction = GetFaction()
    local nIP = GetIP()
    local log = "" ..
        date("%H:%M:%S") ..
        "\t Tµi kho¶n: " ..
        szAccount ..
        "\t Nh©n vËt: " ..
        szName ..
        "\t UUID: " ..
        dwID ..
        "\t CÊp: \t" ..
        nLevel .. "\t M«n ph¸i: " .. nFaction .. "\t IP: " .. nIP .. "\t" .. szMsg ..
        "\n"
    logWrite("exchange_item", log)
end
