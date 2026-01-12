function main(sel)
    if (GetTongID() == 0) then
        Talk(1, "", "Kh«ng cã bang héi kh«ng thÓ ®i vµo khu vùc nµy")
        return
    end
    if (GetFightState() == 0) then -- Íæ¼Ò´¦ÓÚ·ÇÕ½¶·×´Ì¬£¬¼´ÔÚ³ÇÄÚ
        SetPos(1645, 3165)       -- ÉèÖÃ×ß³öTrapµã£¬Ä¿µÄµãÔÚ³ÇÍâ	
        SetFightState(1)         -- ×ª»»ÎªÕ½¶·×´Ì¬
    else                         -- Íæ¼Ò´¦ÓÚÕ½¶·×´Ì¬£¬¼´ÔÚ³ÇÍâ
    end;
end;
