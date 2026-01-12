NPCLIFE = 800000000
NPCEXP = 5000000

function LastDamage(NpcIndex)
	Msg2SubWorld("§¹i hiÖp <color=green>" ..
		GetName() .. "<color> ®· tiªu diÖt thµnh c«ng <color=yellow>" .. GetNpcName(NpcIndex));
	--AddOwnExp(NPCEXP)
end;

--khi hoi sinh
function Revive(NpcIndex)
	--set skill theo ngu hanh, ap dung cho quai cap cao (co skill)------------
	local Series = GetNpcSer(NpcIndex)
	local nSTVL, nDoc, nBang, nHoa, nLoi = 0, 0, 0, 0, 0;
	if (Series == 0) then --KIM
		SetNpcSkill(NpcIndex, 1058, 20, 1);
		SetNpcSkill(NpcIndex, 1059, 20, 2);
		SetNpcSkill(NpcIndex, 1060, 20, 3);
		SetNpcSkill(NpcIndex, 1055, 20, 4);
		nSTVL = 1000;
	elseif (Series == 1) then --MOC
		SetNpcSkill(NpcIndex, 1066, 20, 1);
		SetNpcSkill(NpcIndex, 1067, 20, 2);
		SetNpcSkill(NpcIndex, 1069, 20, 3);
		SetNpcSkill(NpcIndex, 1070, 20, 4);
		nDoc = 150;        --doc sat khong giong sat thuong khac, = sat thuong khac chia 5, vi no co rut' doc
	elseif (Series == 2) then --THUY
		SetNpcSkill(NpcIndex, 1061, 20, 1);
		SetNpcSkill(NpcIndex, 1062, 20, 2);
		SetNpcSkill(NpcIndex, 1063, 20, 3);
		SetNpcSkill(NpcIndex, 1065, 20, 4);
		nBang = 1000;      --neu quai he thuy thi cho bang sat
	elseif (Series == 3) then --HOA
		SetNpcSkill(NpcIndex, 1073, 20, 1);
		SetNpcSkill(NpcIndex, 1074, 20, 2);
		SetNpcSkill(NpcIndex, 1075, 20, 3);
		SetNpcSkill(NpcIndex, 1076, 20, 4);
		nHoa = 1000;       --quai he hoa thi cho hoa sat,tuy theo quai manh hay yeu ma` cho so nay, vi du 8x thi cho 75 chan han ok
	elseif (Series == 4) then --THO
		SetNpcSkill(NpcIndex, 1078, 20, 1);
		SetNpcSkill(NpcIndex, 1079, 20, 2);
		SetNpcSkill(NpcIndex, 1080, 20, 3);
		SetNpcSkill(NpcIndex, 1081, 20, 4);
		nLoi = 1000;
	else --KHONG CO HE, truong hop nay la add sai hay sao do
		SetNpcSkill(NpcIndex, 876, 20, 1);
		SetNpcSkill(NpcIndex, 933, 20, 2);
		SetNpcSkill(NpcIndex, 931, 20, 3);
		SetNpcSkill(NpcIndex, 930, 20, 4);
		nSTVL = 1000;
	end
	print(format("%s - HP: %d", GetNpcName(NpcIndex), GetNpcLife(NpcIndex)))
	SetNpcDmgEx(NpcIndex, nSTVL, nDoc, nBang, nHoa, nLoi, 0); --sat thuong noi cong
	SetNpcDmgEx(NpcIndex, nSTVL, nDoc, nBang, nHoa, nLoi, 1); --sat thuong ngoai cong
	SetNpcExp(NpcIndex, NPCEXP);
	SetNpcResist(NpcIndex, 95, 95, 95, 95, 95, 1);
	--SetNpcLife(NpcIndex, NPCLIFE);
	SetNpcReplenish(NpcIndex, 50000); --phuc hoi sinh luc
	SetNpcHitRecover(NpcIndex, 30); --cao nhat la 100(ko giat) --tuy cam hung
	SetNpcSpeed(NpcIndex, 30);     --toc do di chuyen tang len
end;

--Khi chet
function DeathSelf(NpcIndex)
	DelNpc(NpcIndex)
end;
