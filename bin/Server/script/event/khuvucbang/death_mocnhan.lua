--nguoi choi danh quai' cuoi cung` khi chet
Include("\\script\\library\\worldlibrary.lua")
Include("\\script\\header\\taskid.lua")

SATTHU_EXP = 50000000
TASKRESETHTMN = TASK_RESET6	--tim 1 task de reset moi ngay
CELLRESETHTMN = 4	--tim 1 o^ trong' de reset moi ngay
function LastDamage(NpcIndex)
--Add them gi thi bo vao day lun
local nTask = GetTask(TASKRESETHTMN);
local nLan = GetNumber(nTask,CELLRESETHTMN);
if(nLan >= 5) then Msg2Player("H«m nay "..GetName().." ®· hÊp thô tèi ®a 5 méc nh©n,kh«ng thÓ hÊp thô thªm.") return end
AddOwnExp(SATTHU_EXP)
SetTask(TASKRESETHTMN,SetNumber(nTask,CELLRESETHTMN,nLan+1));
Msg2Player("Ng­¬i nhËn ®­îc <color=yellow> 50.000.000 <color> ®iÓm kinh nghiÖm")
end


--khi hoi sinh
function Revive(NpcIndex)
end
--Khi chet
function DeathSelf(NpcIndex)
DelNpc(NpcIndex)
end