Include("\\script\\library\\worldlibrary.lua")
Include("\\script\\header\\taskid.lua")

SATTHU_EXP = 75000000
function DropRate(NpcIndex)
local nValue = GetTask(TASK_NVBANG);
local nNum = GetNumber2(nValue,1);
local banghoi = GetTongName()
local bangid = GetTongID()
local Dat = GetTempInt("DiemHDBang",bangid)
    if (nNum == GetNpcValue(NpcIndex)) then
	nValue = SetNumber2(nValue,1,0);
    SetTask(TASK_NVBANG,nValue);
	nValue = GetTask(TASK_RESET6);
	local nTimes = GetNumber2(nValue,1);
	nValue = SetNumber2(nValue,1,nTimes+1);
    SetTask(TASK_RESET6,nValue);
	----------------------------------------
	--Add them gi thi bo vao day lun
	SetTempInt("DiemHDBang",bangid,Dat+1);
	SetTask(TASK_TINHHOA, SetNumber(GetTask(TASK_TINHHOA),1)+1);
    AddOwnExp(SATTHU_EXP)
	Msg2Player("Ng≠¨i nhÀn Æ≠Óc <color=yellow> 75.000.000 <color> Æi”m kinh nghi÷m")
	-- Msg2Player("Ng≠¨i Æ∑ lµm r t tËt,gi’t Æ≠Óc ph∂n ÆÂ,bang hÈi <color=yellow>"..banghoi.." <color> cÒa ng≠¨i nhÀn Æ≠Óc 1 Æi”m hoπt ÆÈng bang.")
	-- Msg2Player("Bπn nhÀn Æ≠Óc 1<color=yellow> ßi”m Tinh Hoa <color>")
	-- Msg2SubWorld(""..GetName().." vıa hoµn thµnh <color=orange>Nhi÷m VÙ Bang<color>")

	--------------------------------------
	end
end