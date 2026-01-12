--nguoi choi danh quai' cuoi cung` khi chet
function LastDamage(NpcIndex)
end


--khi hoi sinh
function Revive(NpcIndex)
	local Series = GetNpcSer(NpcIndex)
	local nSTVL, nDoc, nBang, nHoa, nLoi = 0,0,0,0,0;
	if(Series==0) then		--KIM
		SetNpcSkill(NpcIndex, 318, 30, 1);--ngu hanh tuong khac 50
		SetNpcSkill(NpcIndex, 321, 30, 2);
		SetNpcSkill(NpcIndex, 322, 30, 3);
		SetNpcSkill(NpcIndex, 325, 30, 4);
		nSTVL = 1200;
	elseif(Series==1) then	--MOC
		SetNpcSkill(NpcIndex, 339, 30, 1);
		SetNpcSkill(NpcIndex, 302, 30, 2);
		SetNpcSkill(NpcIndex, 353, 30, 3);
		SetNpcSkill(NpcIndex, 355, 30, 4);
		nDoc = 350;--doc sat khong giong sat thuong khac, = sat thuong khac chia 5, vi no co rut' doc
	elseif(Series==2) then	--THUY
		SetNpcSkill(NpcIndex, 380, 30, 1);
		SetNpcSkill(NpcIndex, 336, 30, 2);
		SetNpcSkill(NpcIndex, 337, 30, 3);
		SetNpcSkill(NpcIndex, 380, 30, 4);
		nBang = 1200; --neu quai he thuy thi cho bang sat 
	elseif(Series==3) then	--HOA
		SetNpcSkill(NpcIndex, 357, 30, 1);
		SetNpcSkill(NpcIndex, 359, 30, 2);
		SetNpcSkill(NpcIndex, 361, 30, 3);
		SetNpcSkill(NpcIndex, 362, 30, 4);
		nHoa = 1200;--quai he hoa thi cho hoa sat,tuy theo quai manh hay yeu ma` cho so nay, vi du 8x thi cho 75 chan han ok
	elseif(Series==4) then	--THO
		SetNpcSkill(NpcIndex, 365, 30, 1);
		SetNpcSkill(NpcIndex, 372, 30, 2);
		SetNpcSkill(NpcIndex, 375, 30, 3);
		SetNpcSkill(NpcIndex, 368, 30, 4);
		nLoi = 1200;
	else					--KHONG CO HE, truong hop nay la add sai hay sao do
		SetNpcSkill(NpcIndex, 424, 50, 1);
		SetNpcSkill(NpcIndex, 424, 50, 2);
		SetNpcSkill(NpcIndex, 424, 50, 3);
		SetNpcSkill(NpcIndex, 424, 50, 4);
		nSTVL = 360;
	end
		SetNpcDmgEx(NpcIndex,nSTVL, nDoc, nBang, nHoa, nLoi,0);--sat thuong noi cong
end
--Khi chet
function DeathSelf(NpcIndex)
end