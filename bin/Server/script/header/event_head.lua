Include("\\script\\library\\worldlibrary.lua")
Include("\\script\\header\\taskid.lua")

EVENT_STARTTIME = 202405010000
EVENT_ENDTIME = 202407312359
EVENT_EXP1 = 1000000
EVENT_EXP2 = 5000000

ENAM = 2024
ETHANG = 7
ENGAY = 31
-------------------- Config Event --------------------
EVENT_ACTIVE = 1
Item1 = 1662 -- danh quai
Item2 = 1665 -- danh quai
Item3 = 1669 -- hoat dong
ItemPK = 1669 -- hoat dong
------------------------------------------------------
RateMap7x = 100 -- Cang cao ti le cang thap
RateMap8x = 90
RateMap9x = 200 -- cang cao ti le cang thap
------------------------------------------------------
SoLuongVT = 5 -- van tieu
SoLuongTBK = 5 -- thien bao kho
SoLuongTK = 50 -- tong kim
SoLuongVA = 5 -- vuot ai
SoLuongPLD = 10 -- phong lang do
SoLuongTT = 5 -- thong thuong
SoLuongVD = 5 -- viem de
SoLuongST = 1 -- sat thu
SoLuongDT = 2 -- da tau
SoLuongNCD = 5 -- da tau
------------------------------------------------------
SoLuongVTTSN = 2 -- van tieu
SoLuongTBKTSN = 2 -- thien bao kho
SoLuongTKTSN = 3 -- tong kim
SoLuongVATSN = 4 -- vuot ai
SoLuongPLDTSN = 2 -- phong lang do
SoLuongTTTSN = 2 -- thong thuong
SoLuongVDTSN = 5 -- viem de
SoLuongSTTSN = 2 -- sat thu
SoLuongDTTSN = 2 -- da tau
SoLuongNCDSN = 5 -- ncd

function IsEventActive()
    local nCurTime = tonumber(GetLocalDate("%Y%m%d%H%M"))
    if (nCurTime >= EVENT_STARTTIME and nCurTime <= EVENT_ENDTIME) then
        return 1
    end
    return 0
end

function DropEventMap7x(NpcIndex)
    if (EVENT_ACTIVE == 1) then
        if (RANDOM(RateMap7x) == 10) then
            local nItemIdx = AddItem(0, 6, RANDOM(Item1, Item2), 0, 0, 0, 0, 0)
            SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
            DropItem(NpcIndex, nItemIdx)
        end
    end
end

function DropEventMap8x(NpcIndex)
    if (EVENT_ACTIVE == 1) then
        if (RANDOM(RateMap8x) == 10) then
            local nItemIdx = AddItem(0, 6, RANDOM(Item1, Item2), 0, 0, 0, 0, 0)
            SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
            DropItem(NpcIndex, nItemIdx)
        end
    end
end

function DropEvent(NpcIndex)
    if (EVENT_ACTIVE == 1) then
        local nRate = RANDOM(RateMap9x)
        if (nRate >= 15 and nRate <= 20) then
            local nItemIdx = AddItem(0, 6, RANDOM(Item1, Item2), 0, 0, 0, 0, 0)
            SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
            DropItem(NpcIndex, nItemIdx)
        end
    end
end

function DropEventTK()
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, ItemPK, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongTK)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end

function DropEventVA()
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, Item3, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongVA)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end

function DropEventST()
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, Item3, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongST)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end

function DropEventDT()
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, Item3, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongDT)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end

function DropEventTBK()
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, Item3, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongTBK)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end

function DropEventVT()
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, Item3, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongVT)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end

function DropEventTT()
    if (EVENT_ACTIVE == 1) then
        local nItemIdx = AddItem(0, 6, Item3, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongTT)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end

function DropEventVD()
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, Item3, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongVD)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end

function DropEventPLD(NpcIndex)
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, ItemPK, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongPLD)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        DropItem(NpcIndex, nItemIdx)
    end
end

function DropEventNCD()
    if EVENT_ACTIVE == 1 then
        local nItemIdx = AddItem(0, 6, Item3, 0, 0, 0, 0)
        SetStackItem(nItemIdx, SoLuongNCD)
        SetExpiredTime(nItemIdx, ENAM, ETHANG, ENGAY, 0, 0, 0)
        AddItemID(nItemIdx)
    end
end
