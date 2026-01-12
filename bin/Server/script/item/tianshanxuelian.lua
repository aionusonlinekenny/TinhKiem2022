function main(nItemIndex)
    local nPLev = GetLevel()
    if nPLev > 120 or nPLev < 1 then
        Say("ChØ cã nh©n vËt cÊp ®é tõ 1 ®Õn 120 (bao gåm cÊp 1 vµ 120) míi cã thÓ sö dông.", 0)
        return 0
    end

    RemoveItem(nItemIndex, 1);
    --AddStackExp(1433042000)
    ST_LevelUp(120 - nPLev)
    nPLev = GetLevel()
    Msg2Player(format("Chóc mõng b¹n, ®¼ng cÊp hiÖn t¹i cña b¹n lµ %d.", nPLev))
end
