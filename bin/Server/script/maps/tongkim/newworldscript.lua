--Include("\\script\\maps\\newworldscript_default.lua")

function OnNewWorld(szParam)
    ForbidUseTownP(1)
    ForbidParty(1)
end

function OnLeaveWorld(szParam)
    ForbidUseTownP(0)
    ForbidParty(0)
end
