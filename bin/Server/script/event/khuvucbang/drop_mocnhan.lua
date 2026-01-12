Include("\\script\\library\\worldlibrary.lua")
Include("\\script\\header\\taskid.lua")


function DropRate(NpcIndex)
local banghoi = GetTongName()
local bangid = GetTongID()
local Dat = GetTempInt("DiemHDBang",bangid)
----------------------------------------
SetTempInt("DiemHDBang",bangid,Dat+1);
SetTask(TASK_TINHHOA, SetNumber(GetTask(TASK_TINHHOA),1)+1);
local num = random(1,3)
local num2 = random(2,4)
local num3 = random(2,4)
local num4 = random(108,109)
for i=1,num do DropNpcItem(NpcIndex, 0, 3,110,0,0,5,0,0); end --bao ruong
for i=1,num2 do DropNpcItem(NpcIndex, 0, 3,107,0,0,5,0,0); end --bao ruong
for i=1,num3 do DropNpcItem(NpcIndex, 0, 3,num4,0,0,5,0,0); end --bao ruong

Msg2Player("Bπn nhÀn Æ≠Óc 1 <color=yellow> ßi”m Bang HÈi <color>.")
Msg2Player("Bπn nhÀn Æ≠Óc 1<color=yellow> ßi”m Tinh Hoa <color>")
--------------------------------------
end