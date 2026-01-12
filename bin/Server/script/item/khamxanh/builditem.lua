-- AlexKing <Build>
-- 25.8.2016
Include("\\script\\lib\\lib_vatpham.lua")
Include("\\script\\lib\\lib_nangcap.lua")

NOTNANGCAPLOCKITEM = "Kh«ng thÓ n©ng cÊp vËt phÉm khãa.";
NOTCHINHXAC = "Vui lßng ®Æt vµo chÝnh x¸c nguyªn liÖu.";
HOPTHANHSUCC = "Hîp thµnh thµnh c«ng trang bÞ <color=gold>%s <color>.";
NOTENOUGHMONEY = "Ng­¬i kh«ng ®ñ %d l­îng.";
NOTENOUGHTLUCKY = "Xin ®Æt thªm c¸c lo¹i kho¸ng th¹ch!";
UNSUCCESS = "Hîp thµnh thÊt b¹i, mÊt mét Ýt vËt liÖu vµ ng©n l­îng!";

function main(uParam,nParam)
	if (uParam == 0) or (nParam == 0) then
	return end

	if (uParam > getn(ITEMHOPTHANH)) then
	return end
	
	if (nParam > getn(ITEMHOPTHANH[uParam])) then	-- anti pha
	return end
	--dofile("script\\special\\builditem.lua")
	if (uParam == 1) then
		hopthanh_1(nParam);
	return end
	
	if (uParam == 2) then
		hopthanh_2(nParam);
	return end	
end

function hopthanh_1(nParam)
	local nEnough = 0;
	local nIndex = 0;
	local nCountEquip = 0;
	local nCountVL = 0;	
--	local bBlock,nTime = 0,0;
--	local bBLockItem = 0;
	local nLuck = 0;
	local i,j = 0,0;
	for i=0,5 do
		for j=0,3 do
		nIndex = GetROItem(ROOMG,i,j);	
		if (nIndex > 0) then
			nCountEquip = nCountEquip + 1;
--			bBlock,nTime = GetBindItem(2,nIndex);
--			if bBlock > 0 then bBLockItem = 2 end		
		end
		end
	end
	
	for i=0,3 do
		nIndex = GetPOItem(POSCBI,i);
		if (nIndex > 0) then
		nCountVL = nCountVL + 1;	
		end 
	end	
	
	if (nCountVL > 0) then
	Talk(1,"",NOTCHINHXAC)		
	return end	
	
--	if (bBLockItem ~= 0) then		-- khong nang cap trang bi khoa
--	Talk(1,"",NOTNANGCAPLOCKITEM)		
--	return end			

	if (nCountEquip < ITEMHOPTHANH[1][nParam][8]) then
	Talk(1,"",NOTCHINHXAC)		
	return end
	
	for i = ITEMHOPTHANH[1][nParam][4],ITEMHOPTHANH[1][nParam][5] do
		if GetItemCount(BLUEITEM,0,4,i,0,0,0,ROOMG) >= 1 then
		nEnough = nEnough + 1;
		end		
	end
	
	if (nEnough ~= ITEMHOPTHANH[1][nParam][8]) then	
	Talk(1,"",NOTCHINHXAC)	
	return end
	
	if GetCash() < ITEMHOPTHANH[1][nParam][6] then
	Talk(1,"",format(NOTENOUGHMONEY,ITEMHOPTHANH[1][nParam][6]));
	return end	
	
	if (ITEMHOPTHANH[1][nParam][9] == 1) then
		nLuck = get_lucky();
		
		if (nLuck == 0) then
		Talk(1,"",NOTENOUGHTLUCKY)	
		return end
		
--		nLuck = nLuck - ITEMHOPTHANH[1][nParam][7] - bBLockItem;	-- item khoa' kha~ nang max thap hon
		nLuck = nLuck - ITEMHOPTHANH[1][nParam][7];
		
		if nLuck > 10 then nLuck = 10 end 
		if nLuck < 0 then nLuck = 0 end
		
		for i = ITEMHOPTHANH[1][nParam][4],ITEMHOPTHANH[1][nParam][5] do
			DelItem(BLUEITEM,0,4,i,0,0,0,ROOMG,1);	
		end
		
		local nIndex = ItemSetAdd(GOLDITEM, ITEMHOPTHANH[1][nParam][3], 0, random(nLuck,10))
--		SetBindItem(2,nIndex,bBLockItem,0);
		SetPItemID(nIndex,POSCBI,0);	
		Pay(ITEMHOPTHANH[1][nParam][6]);
		Talk(1,"",format(HOPTHANHSUCC,ITEMHOPTHANH[1][nParam][2]));
	return end
	
	if (ITEMHOPTHANH[1][nParam][9] == 2) then
		local nLuckP = random(1,10);
		
		if (nLuckP < 7) then
			for i = 1,3 do
				local nRand = random(ITEMHOPTHANH[1][nParam][4],ITEMHOPTHANH[1][nParam][5]);
				if GetItemCount(BLUEITEM,0,4,nRand,0,0,0,ROOMG) >= 1 then
				DelItem(BLUEITEM,0,4,nRand,0,0,0,ROOMG,1);
				end
			end
			Pay(ITEMHOPTHANH[1][nParam][6]/5);	
			Talk(1,"",UNSUCCESS);
		return end
		
		nLuck = random(1,10);
		
		for i = ITEMHOPTHANH[1][nParam][4],ITEMHOPTHANH[1][nParam][5] do
			DelItem(BLUEITEM,0,4,i,0,0,0,ROOMG,1);	
		end
		
		local nIndex = ItemSetAdd(GOLDITEM, ITEMHOPTHANH[1][nParam][3], nLuck, random(nLuck-1,nLuck))
--		SetBindItem(2,nIndex,bBLockItem,0);
		SetPItemID(nIndex,POSCBI,0);	
		Pay(ITEMHOPTHANH[1][nParam][6]);
		Talk(1,"",format(HOPTHANHSUCC,ITEMHOPTHANH[1][nParam][2]));		
	return end
	
	if (ITEMHOPTHANH[1][nParam][9] == 3) then
		local nLuckP = random(1,10);
		
		if (nLuckP < 8) then
			for i = 1,5 do
				local nRand = random(ITEMHOPTHANH[1][nParam][4],ITEMHOPTHANH[1][nParam][5]);
				if GetItemCount(BLUEITEM,0,4,nRand,0,0,0,ROOMG) >= 1 then
				DelItem(BLUEITEM,0,4,nRand,0,0,0,ROOMG,1);
				end
			end
			Pay(ITEMHOPTHANH[1][nParam][6]/5);	
			Talk(1,"",UNSUCCESS);
		return end
			
		for i = ITEMHOPTHANH[1][nParam][4],ITEMHOPTHANH[1][nParam][5] do
			DelItem(BLUEITEM,0,4,i,0,0,0,ROOMG,1);	
		end
		
		local nIndex = ItemSetAdd(GOLDITEM, ITEMHOPTHANH[1][nParam][3], 0, 0)
--		SetBindItem(2,nIndex,bBLockItem,0);
		SetPItemID(nIndex,POSCBI,0);	
		Pay(ITEMHOPTHANH[1][nParam][6]);
		Talk(1,"",format(HOPTHANHSUCC,ITEMHOPTHANH[1][nParam][2]));		
	return end	
	
	for i = ITEMHOPTHANH[1][nParam][4],ITEMHOPTHANH[1][nParam][5] do
		DelItem(BLUEITEM,0,4,i,0,0,0,ROOMG,1);	
	end
		
	local nIndex = ItemSetAdd(GOLDITEM, ITEMHOPTHANH[1][nParam][3], 0, 0)
--	SetBindItem(2,nIndex,bBLockItem,0);
	SetPItemID(nIndex,POSCBI,0);	
	Pay(ITEMHOPTHANH[1][nParam][6]);
	Talk(1,"",format(HOPTHANHSUCC,ITEMHOPTHANH[1][nParam][2]));		
end;

function hopthanh_2(nParam)
	local nCountEquip = 0;
	local nIndex = 0;
--	local bBlock,nTime = 0,0;
--	local bBLockItem = 0;	
	local dohkid,hoangthach,phulieu = 0;
	local Rkind,Rgen,Rdetail,Rparti,Rlevel,Rseries,Rline,Rnum;	
	local Rmalv1,Rmalv2,Rmalv3,Rmalv4,Rmalv5,Rmalv6,Rmalv7,Rmalv8;
	local nPhuMM = 0;
	local nLucky = 6;
	
	if GetCash() < ITEMHOPTHANH[2][nParam][8] then
	Talk(1,"",format(NOTENOUGHMONEY,ITEMHOPTHANH[2][nParam][8]));
	return end	
	
	dohkid = GetPOItem(POSCBI,0);
	if (dohkid > 0) then
		Rkind,Rgen,Rdetail,Rparti,Rlevel,Rseries,Rline = GetCBItem(dohkid);
		if (Rkind == GOLDITEM) and (Rline == ITEMHOPTHANH[2][nParam][9]) then
			nCountEquip = nCountEquip + 1;
		end
	end
	
	hoangthach = GetPOItem(POSCBI,1); 
	if (hoangthach > 0) then
		Rkind,Rgen,Rdetail,Rparti,Rlevel,Rseries,Rline = GetCBItem(hoangthach);
		Rnum = GetStackItem(hoangthach);
		if (Rkind == BLUEITEM) and (Rdetail == ITEMHOPTHANH[2][nParam][5]) and (Rnum >= ITEMHOPTHANH[2][nParam][7]) then
			nCountEquip = nCountEquip + 1;
		end
	end	
	
	phulieu = GetPOItem(POSCBI,2);
	if (phulieu > 0) then
		Rkind,Rgen,Rdetail,Rparti,Rlevel,Rseries,Rline = GetCBItem(phulieu);
		Rnum = GetStackItem(phulieu);	
		if (Rkind == BLUEITEM) and (Rdetail == ITEMHOPTHANH[2][nParam][4]) and (Rnum >= ITEMHOPTHANH[2][nParam][6]) then
			nCountEquip = nCountEquip + 1;
		end
	end	
	
	if (nCountEquip ~= ITEMHOPTHANH[2][nParam][10]) then
	Talk(1,"",NOTCHINHXAC)		
	return end
	
	nPhuMM = GetPOItem(POSCBI,3);
	if (nPhuMM > 0) then
		Rkind,Rgen,Rdetail,Rparti,Rlevel,Rseries,Rline = GetCBItem(nPhuMM);
		if (Rdetail == 7) then
		nLucky = nLucky - 2;
		elseif (Rdetail == 8) then
		nLucky = nLucky - 5;
		end
		RemoveItem(nPhuMM,1);
	end		
	
--	for i=0,3 do
--		nIndex = GetPOItem(POSCBI,i);
--		bBlock,nTime = GetBindItem(2,nIndex);
--		if bBlock > 0 then bBLockItem = 2 end		
--	end	
	
	if (random(10) > nLucky) then	
		RemoveItem(hoangthach,ITEMHOPTHANH[2][nParam][7]);	
		RemoveItem(phulieu,ITEMHOPTHANH[2][nParam][6]);
		
		Rmalv1,Rmalv2,Rmalv3,Rmalv4,Rmalv5,Rmalv6,Rmalv7,Rmalv8 = GetOTLVItem(dohkid);
		RemoveItem(dohkid,1);	
		local nIndex = ItemSetAdd(GOLDITEM, ITEMHOPTHANH[2][nParam][3], 0, Rmalv1,Rmalv2,Rmalv3,Rmalv4,Rmalv5,Rmalv6,Rmalv7,Rmalv8);
		SetPItemID(nIndex,POSCBI,0);	
		Pay(ITEMHOPTHANH[2][nParam][8]);
		Talk(1,"",format(HOPTHANHSUCC,ITEMHOPTHANH[2][nParam][2]));
	else
		RemoveItem(hoangthach,ITEMHOPTHANH[2][nParam][7]/2);	
		RemoveItem(phulieu,ITEMHOPTHANH[2][nParam][6]/2);
		Msg2Player("Hîp thµnh thÊt b¹i, mÊt mét nöa nguyªn liÖu!");	
	end	
end