Include("\\script\\lib\\worldlibrary.lua")
Include("\\script\\header\\taskid.lua")
Include("\\script\\global\\npcchucnang\\lequan.lua")
MAX_PLAYER_PER_IP = 3

-- Define logWrite and LoginLog before main
function logWrite(category, str)
    local gm_Log = "dulieu/player_log/" .. date("%d_%m_%Y") .. ".txt" -- Use game engine's date function
    local fs_log = openfile(gm_Log, "a") -- Use game engine's file I/O
    write(fs_log, str)
    closefile(fs_log)
end

function LoginLog()
    local szAccount = GetAccount()
    local szName = GetName()
    local dwID = GetUUID()
    local nLevel = GetLevel()
    local nFaction = GetFaction()
    local nIP = GetIP()
    local nExtPoint = GetExtPoint()
    local nMoney = GetCash()
    local log = "" ..
        date("%H:%M:%S") .. -- Use game engine's date function
        "\t Tµi kho∂n: " ..
        szAccount ..
        "\t Nh©n vÀt: " ..
        szName ..
        "\t UUID: " ..
        dwID ..
        "\t C p: \t" ..
        nLevel .. "\t M´n ph·i: " .. nFaction .. "\t IP: " .. nIP ..
        "\t Xu: " .. nExtPoint .. "\t TÊng ti“n vπn: \t" .. nMoney .. "\n"
    logWrite("player_log", log)
end

function main()
    if type(GetNumberPlayerByIp) ~= "function" or type(GetIP2) ~= "function" then
        print("Error: GetNumberPlayerByIp or GetIP2 is not defined")
        return
    end

    if (GetNumberPlayerByIp(GetIP2()) > MAX_PLAYER_PER_IP) then
        Talk(1, "ExitGame",
            format("MÁi IP chÿ cho ph–p Æ®ng nhÀp tËi Æa <color=yellow>%d<color> nh©n vÀt, tho∏t game!",
                MAX_PLAYER_PER_IP))
        return
    end

    print("Login => [Account= " ..
        GetAccount() .. "], [Name= " ..
        GetName() .. "], [Level = " ..
        GetLevel() .. "], [UUID= " ..
        GetUUID() .. "]")
    LoginLog() -- Line 59
    RLTask()

    local szLogin = ""
    if (GetSex() == 1) then
        szLogin = "N˜ hi÷p <color=yellow>" .. GetName() .. "<color> Æ∑ online!"
    else
        szLogin = "ßπi hi÷p <color=yellow>" .. GetName() .. "<color> Æ∑ online!"
    end

    Msg2SubWorld(szLogin)

    if (GetTaskS(TASK_NAME) ~= "" and (GetName() ~= GetTaskS(TASK_NAME))) then
        RemoveItem(FindItem(6, 1543), 1) -- di danh phu
    end
    SetTask(TASK_NAME, GetName())
    local nTaskValue = GetTask(TASK_THOIGIAN7)
    if (GetNumber(4, nTaskValue, 2) > 0) then
        ktthoigian()
    end
    if (FindItem(2, 0) <= 0) then
        LockItem(AddItemID(AddItem(0, 2, 0, 0, 0, 5, 0, 0)), -3)
    end
    RestoreRankEx()
    zmd_check_expire_timer()
    dlg_offlineaward()
    local w, x, y = GetWorldPos()
    local nPKValue = GetPK()
    if (w == 208) then
        local nTaskValue = GetTask(TASK_DUNGCHUNG3)
        local nTask = GetNumber(3, nTaskValue, 3)
        if (nTask >= 100) then
            local nTimerID = GetTimerId()
            if (nTimerID ~= 9) then
                SetTask(TASK_DUNGCHUNG3, SetNumber(3, GetTask(TASK_DUNGCHUNG3), 3, 100 + nPKValue))
                SetTimer(2 * 60 * 60 * 18, 9)
            end
            SetCreateTeam(0)
        elseif (w == 242) then
            ForbidUseTownP(1)
        end
    end
end

-- Other functions remain unchanged
function zmd_check_expire_timer()
    local nExpireTime = GetEquipItemEx()
    if (nExpireTime ~= 0) then
        if (GetCurServerSec() - nExpireTime <= 86400) then
            if (GetNumber(1, GetTask(TASK_DUNGCHUNG3), 1) == 1) then
                SetTask(TASK_DUNGCHUNG3, SetNumber(1, GetTask(TASK_DUNGCHUNG3), 1, 0))
                Talk(1, "", "TÛi tˆ m…u cÒa Æπi hi÷p sœ h’t hπn trong vﬂng h´m nay")
                Msg2Player("TÛi tˆ m…u cÒa Æπi hi÷p sœ h’t hπn trong vﬂng h´m nay")
            end
        end
    else
        if (GetNumber(1, GetTask(TASK_DUNGCHUNG3), 1) == 1) then
            SetTask(TASK_DUNGCHUNG3, SetNumber(1, GetTask(TASK_DUNGCHUNG3), 1, 0))
            Msg2Player("TÛi tˆ m…u cÒa Æπi hi÷p Æ∑ h’t hπn")
        end
    end
end

function dlg_offlineaward()
    local nLvSec = GetTask(TASK_LVGAMESEC)
    local nLgSec = GetCurServerSec()

    if (nLvSec == nil or nLgSec == nil) then
        return
    end

    if (nLgSec < nLvSec) then
        nLgSec = nLvSec
    end
    if (nLvSec == 0) then
        nLvSec = nLgSec
    end
    if (nLgSec - nLvSec <= 0) then
        return
    end
    local nOffSec = floor((nLgSec - nLvSec) / 60)
    CountExp(nOffSec, GetLevel(), GetTranslife())
    local nCurExp = GetTask(TASK_LVGAMEEXP)
    Say("LINK:<nil>", 0,
        format(
            "RÍi mπng l«n tr≠Ìc: <color=red>%s<color><enter>Hi÷n tπi Æ®ng nhÀp vµo: <color=red>%s<color>\nThÍi gian rÍi mπng: <color=red>%d<color> phÛt\nMi‘n ph› nhÀn Æ≠Óc Æi”m kinh nghi÷m <color=red>%d<color> (ChÛ ˝: Chuy”n b∂n ÆÂ sœ bﬁ m t)",
            GetTimeDate("%Y/%m/%d %H:%M:%S", nLvSec),
            GetTimeDate("%Y/%m/%d %H:%M:%S", nLgSec),
            nOffSec,
            nCurExp), 2,
        "Mi‘n ph› nhÀn ph«n th≠Îng kinh nghi÷m/action_takefreeexp",
        "Tho∏t ra/Cancel")
end

function action_takefreeexp()
    if (GetLevel() < 50 and GetTranslife() <= 0) then
        Talk(1, "", "Kh´ng phÔ hÓp vÌi Æi÷u ki÷n nhÀn th≠Îng")
        return
    end
    local nCurExp = GetTask(TASK_LVGAMEEXP)
    if (nCurExp <= 0) then
        Talk(1, "", "ß∑ sˆ dÙng h’t thÍi gian rÍi mπng.")
        return
    end

    SetTask(TASK_LVGAMEEXP, 0)
    AddStackExp(nCurExp)
end

function CountExp(itr, level, tran_count)
    local LIST_EXP = { 15000, 30000, 40000, 40000, 60000, 72000, 86400, 103680, 103680, 103680, 103680, 103680, 103680, 103680, 103680 }
    if (itr <= 0) then
        return 0
    end
    local index = 1
    if (level >= 151) then
        index = tran_count + 3
    elseif (level >= 131) then
        index = 2
    elseif (level >= 50) then
        index = 1
    else
        return 0
    end
    SetTask(TASK_LVGAMEEXP, GetTask(TASK_LVGAMEEXP) + LIST_EXP[index] * itr)
end

function RLTask()
    local Yr, Mth, Dy, Hr, Mn, Se, nDw = GetTime()
    if (Dy ~= GetNumber(2, GetTask(TASK_THOIGIAN6), 1)) then
        SetTask(TASK_THOIGIAN6, SetNumber(2, GetTask(TASK_THOIGIAN6), 1, Dy))
        SetTask(TASK_RESET, 0)
        SetTask(TASK_RESET2, 0)
        SetTask(TASK_RESET3, 0)
        SetTask(TASK_RESET4, 0)
        SetTask(TASK_RESET5, 0)
        SetTask(TASK_DATCUOC4, 0)
        SetTask(TASK_DATCUOC5, 0)
        SetTask(TASK_DATCUOC6, 0)

        local nCurSvDw = GetNumber(1, GetTask(TASK_RESETWEEK), 9)
        if (nCurSvDw ~= nDw and nDw == 1) then -- thu2
            SetTask(TASK_RESETWEEK, 0)
            SetTask(TASK_RESETWEEK, SetNumber(1, GetTask(TASK_RESETWEEK), 9, nDw))
        end
    end
end

function ExitGame()
    OfflineLive()
end