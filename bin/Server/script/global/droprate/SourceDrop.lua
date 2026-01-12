--|------------------------------------------------------|
--|FORMULA_LUCKY = nLuckRate quy dinh cot maxx item;   --|
--|Author: Kinnox									   --|	
--|Date: 22/03/2021									   --|	
--|------------------------------------------------------|
Include("\\script\\global\\droprate\\matran\\armor.lua");
Include("\\script\\global\\droprate\\matran\\amulet.lua");
Include("\\script\\global\\droprate\\matran\\belt.lua");
Include("\\script\\global\\droprate\\matran\\boot.lua");
Include("\\script\\global\\droprate\\matran\\helm.lua");
Include("\\script\\global\\droprate\\matran\\cuff.lua");
Include("\\script\\global\\droprate\\matran\\pendant.lua");
Include("\\script\\global\\droprate\\matran\\ring.lua");
Include("\\script\\global\\droprate\\matran\\meeleeweapon.lua");
Include("\\script\\global\\droprate\\matran\\rangrateweapon.lua");
--TAB BI KIP 9x
TAB_Menpai = {
	{ 37, 38, 39 },      -- Thien vuong
	{ 56, 57, 58 },      -- thieu lam
	{ 47, 48, 49 },      -- ngu doc
	{ 27, 28, 45, 46, 53 }, -- duong mon
	{ 42, 43, 59 },      -- nga mi
	{ 40, 41 },          -- thuy yen
	{ 54, 55 },          -- cai bang
	{ 35, 36 },          -- thien nhan
	{ 33, 34 },          -- vo dang
	{ 50, 51, 52 }       -- con lon
}
--Item Blue
KindItem = {
	{ 2, 0, 27 }, -- armor
	{ 4, 0, 1 }, -- amulet
	{ 6, 0, 1 }, -- belt
	{ 5, 0, 3 }, -- boot
	{ 7, 0, 13 }, -- helm
	{ 8, 0, 1 }, -- cuff
	{ 9, 0, 1 }, -- pendant
	{ 3, 0, 0 }, -- ring
	{ 0, 0, 6 }, -- melleweapon
	{ 1, 0, 2 } -- rangeweapon
};

--Dong hien.
KindOption_Show = {
	--level: 			1			2			3			4			5			6			7			8			9			10
	--	ID option - nMin,nMax
	{ { 85 },  { { 1, 50 }, { 20, 70 }, { 30, 90 }, { 40, 120 }, { 50, 150 }, { 60, 170 }, { 70, 190 }, { 70, 200 }, { 70, 200 }, { 70, 200 } } }, --1-- sinh luc
	{ { 89 },  { { 1, 50 }, { 20, 70 }, { 30, 90 }, { 40, 120 }, { 50, 150 }, { 60, 170 }, { 70, 190 }, { 70, 200 }, { 70, 200 }, { 70, 200 } } }, --2-- noi luc
	{ { 93 },  { { 1, 50 }, { 20, 70 }, { 30, 90 }, { 40, 120 }, { 50, 150 }, { 60, 170 }, { 70, 190 }, { 70, 200 }, { 70, 200 }, { 70, 200 } } }, --3-- the luc
	{ { 96 },  { { 1, 3 }, { 1, 3 }, { 1, 5 }, { 1, 5 }, { 1, 5 }, { 1, 6 }, { 1, 7 }, { 1, 9 }, { 1, 9 }, { 1, 9 } } },                        --4-- phuc hoi moi giay nua diem the luc
	{ { 92 },  { { 1, 2 }, { 1, 2 }, { 1, 2 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 } } },                        --5-- phuc hoi moi giay nua diem noi luc
	{ { 88 },  { { 1, 2 }, { 1, 2 }, { 1, 3 }, { 1, 6 }, { 1, 6 }, { 1, 6 }, { 1, 6 }, { 1, 6 }, { 1, 6 }, { 1, 6 } } },                        --6-- phuc hoi moi giay nua diem sinh luc
	{ { 43 },  { { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 } } },                        --7-- khong the pha huy
	{ { 117 }, { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 10, 15 }, { 10, 16 }, { 10, 17 }, { 10, 20 }, { 10, 20 }, { 10, 20 } } },          --8-- phan don can chien (diem)
	{ { 113 }, { { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 30 }, { 20, 30 }, { 20, 40 }, { 20, 40 }, { 20, 40 }, { 20, 40 }, { 20, 40 } } },    --9-- thoi gian phuc hoi (danh cho ao) --OPTION DAC BIET
	{ { 114 }, { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 16 }, { 1, 17 }, { 5, 20 }, { 6, 20 }, { 7, 20 } } },                --10-- khang tat ca
	{ { 111 }, { { 10, 20 }, { 10, 20 }, { 10, 27 }, { 10, 30 }, { 10, 33 }, { 10, 37 }, { 15, 40 }, { 15, 40 }, { 17, 40 }, { 17, 40 } } },    --11-- toc do di chuyen --OPTION DAC BIET
	{ { 115 }, { { 10, 20 }, { 10, 20 }, { 10, 20 }, { 10, 20 }, { 10, 20 }, { 10, 20 }, { 10, 30 }, { 10, 30 }, { 10, 30 }, { 10, 30 } } },    --12-- toc do danh--OPTION DAC BIET
	{ { 136 }, { { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 5 }, { 1, 5 }, { 1, 6 }, { 1, 7 }, { 1, 10 }, { 1, 10 }, { 1, 10 } } },                     --13-- hut sinh luc--OPTION DAC BIET
	{ { 137 }, { { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 5 }, { 1, 5 }, { 1, 6 }, { 1, 7 }, { 1, 10 }, { 1, 10 }, { 1, 10 } } },                     --14-- hut noi luc--OPTION DAC BIET
	{ { 126 }, { { 20, 20 }, { 20, 30 }, { 20, 30 }, { 20, 40 }, { 20, 50 }, { 20, 60 }, { 30, 90 }, { 30, 100 }, { 50, 100 }, { 50, 100 } } }, --15-- sat thuong vat ly ngoai cong ( %) --OPTION DAC BIET
	{ { 166 }, { { 1, 50 }, { 20, 70 }, { 30, 90 }, { 30, 120 }, { 35, 150 }, { 30, 170 }, { 40, 190 }, { 50, 200 }, { 60, 200 }, { 60, 200 } } }, --16-- do chinh sat
	{ { 58 },  { { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 } } },                        --17-- bo qua ne tranh
};

--Dong An.
KindOption_Hide = {
	--level: 			1			2			3			4			5			6			7			8			9			10
	--	ID option - nMin,nMax
	{ { 134 }, { { 1, 5 }, { 1, 5 }, { 1, 5 }, { 1, 5 }, { 1, 5 }, { 1, 9 }, { 1, 12 }, { 5, 15 }, { 6, 15 }, { 7, 15 } } },                    --1-- chuyen hoa sat thuong thanh noi luc
	{ { 101 }, { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 18 }, { 1, 20 }, { 5, 25 }, { 6, 25 }, { 7, 25 } } },                --2-- khang doc
	{ { 103 }, { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 17 }, { 1, 25 }, { 5, 30 }, { 6, 30 }, { 7, 30 } } },                --3-- khang loi
	{ { 102 }, { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 17 }, { 1, 20 }, { 5, 25 }, { 6, 25 }, { 7, 25 } } },                --4-- khang hoa
	{ { 110 }, { { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 30 }, { 20, 40 }, { 20, 40 }, { 20, 40 }, { 20, 40 } } },    --5-- thoi gian choang ---- OPTION DAC BIET
	{ { 99 },  { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 20 }, { 1, 20 }, { 5, 20 }, { 6, 20 }, { 7, 20 } } },                --6-- diem sinh khi
	{ { 104 }, { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 18 }, { 1, 20 }, { 5, 25 }, { 6, 25 }, { 7, 25 } } },                --7-- phong thu vat ly
	{ { 106 }, { { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 30 }, { 20, 30 }, { 20, 40 }, { 20, 40 }, { 20, 40 } } },    --8-- thoi gian lam cham---- OPTION DAC BIET
	{ { 98 },  { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 20 }, { 1, 20 }, { 5, 20 }, { 6, 20 }, { 7, 20 } } },                --9-- diem than phap
	{ { 108 }, { { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 20 }, { 20, 30 }, { 20, 30 }, { 20, 40 }, { 20, 40 }, { 20, 40 } } },    --10-- thoi gian trung doc ---- OPTION DAC BIET
	{ { 105 }, { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 17 }, { 1, 20 }, { 5, 25 }, { 6, 25 }, { 7, 25 } } },                --11-- khang bang
	{ { 97 },  { { 1, 5 }, { 1, 5 }, { 1, 10 }, { 1, 10 }, { 1, 15 }, { 1, 18 }, { 1, 19 }, { 5, 20 }, { 6, 20 }, { 7, 20 } } },                --12-- suc manh
	{ { 135 }, { { 1, 3 }, { 1, 3 }, { 1, 5 }, { 1, 5 }, { 1, 5 }, { 1, 6 }, { 1, 7 }, { 1, 9 }, { 1, 10 }, { 1, 10 } } },                      --13-- may man ---- OPTION DAC BIET
	{ { 121 }, { { 5, 10 }, { 5, 20 }, { 10, 20 }, { 10, 30 }, { 10, 40 }, { 10, 50 }, { 10, 50 }, { 15, 50 }, { 15, 50 }, { 15, 50 } } },      --14-- sat thuong vat ly ngoai cong ( diem) --OPTION DAC BIET
	{ { 140 }, { { 1, 20 }, { 20, 50 }, { 20, 80 }, { 20, 100 }, { 20, 130 }, { 20, 150 }, { 40, 170 }, { 50, 180 }, { 80, 200 }, { 90, 200 } } }, --15-- sat thuong vat ly noi cong ( diem) --OPTION DAC BIET
	{ { 125 }, { { 5, 10 }, { 5, 20 }, { 10, 20 }, { 10, 23 }, { 10, 33 }, { 10, 40 }, { 10, 50 }, { 15, 50 }, { 15, 50 }, { 15, 50 } } },      --16-- doc sat ngoai cong ( diem) --OPTION DAC BIET
	{ { 144 }, { { 5, 10 }, { 5, 20 }, { 10, 20 }, { 10, 23 }, { 10, 33 }, { 10, 40 }, { 10, 50 }, { 15, 50 }, { 15, 50 }, { 15, 50 } } },      --17-- doc sat noi cong ( diem) --OPTION DAC BIET
	{ { 123 }, { { 5, 10 }, { 5, 20 }, { 10, 20 }, { 10, 30 }, { 10, 40 }, { 10, 50 }, { 10, 50 }, { 30, 80 }, { 30, 90 }, { 30, 100 } } },     --18-- bang sat ngoai cong ( diem) --OPTION DAC BIET
	{ { 141 }, { { 1, 20 }, { 20, 50 }, { 20, 80 }, { 20, 100 }, { 20, 130 }, { 20, 150 }, { 40, 170 }, { 50, 180 }, { 80, 200 }, { 90, 200 } } }, --19-- bang sat noi cong ( diem) --OPTION DAC BIET
	{ { 122 }, { { 20, 20 }, { 20, 30 }, { 20, 30 }, { 20, 40 }, { 20, 50 }, { 20, 60 }, { 40, 90 }, { 50, 100 }, { 50, 100 }, { 50, 100 } } }, --20-- hoa sat ngoai cong ( diem) --OPTION DAC BIET
	{ { 142 }, { { 1, 20 }, { 20, 50 }, { 20, 80 }, { 20, 100 }, { 20, 130 }, { 20, 150 }, { 40, 170 }, { 50, 180 }, { 80, 200 }, { 90, 200 } } }, --21-- hoa sat noi cong noi cong ( diem) --OPTION DAC BIET
	{ { 124 }, { { 20, 20 }, { 20, 30 }, { 20, 30 }, { 20, 40 }, { 20, 50 }, { 20, 60 }, { 40, 90 }, { 50, 100 }, { 50, 100 }, { 50, 100 } } }, --22-- loi sat ngoai cong ( diem) --OPTION DAC BIET
	{ { 143 }, { { 1, 20 }, { 20, 50 }, { 20, 80 }, { 20, 100 }, { 20, 130 }, { 20, 150 }, { 40, 170 }, { 50, 180 }, { 80, 200 }, { 90, 200 } } }, --23-- loi sat noi cong noi cong ( diem) --OPTION DAC BIET
};

KindFiveElement = {

	-------------SHOW OPTION-----HIDE OPTION
	---ARMOR---
	{ { { 1, 2, 3, 6, 7, 8, 9 }, { 1, 2 } },
		{ { 1, 2, 3, 6, 7, 8, 9 }, { 1, 3 } },
		{ { 1, 2, 3, 6, 7, 8, 9 }, { 1, 4, 5, 6 } },
		{ { 1, 2, 3, 6, 7, 8, 9 }, { 7, 8, 9 } },
		{ { 1, 2, 3, 6, 7, 8, 9 }, { 1, 11, 10 } },
	},
	--AMULET---
	{ { { 1, 2, 3, 4, 10 }, { 2, 12 } },
		{ { 1, 2, 3, 4, 10 }, { 1, 3, 13 } },
		{ { 1, 2, 3, 4, 10 }, { 1, 4, 5, 6 } },
		{ { 1, 2, 3, 4, 10 }, { 8, 9 } },
		{ { 1, 2, 3, 4, 10 }, { 1, 11, 10 } }
	},
	---BELT---	
	{ { { 1, 2, 3, 6, 7 }, { 1, 2 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 3 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 4, 5, 6 } },
		{ { 1, 2, 3, 6, 7 }, { 7, 8, 9 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 11, 10 } },
	},
	---BOOT---	
	{ { { 1, 2, 3, 6, 7, 11 }, { 1, 2 } },
		{ { 1, 2, 3, 6, 7, 11 }, { 1, 3 } },
		{ { 1, 2, 3, 6, 7, 11 }, { 1, 4, 5, 6 } },
		{ { 1, 2, 3, 6, 7, 11 }, { 7, 8, 9 } },
		{ { 1, 2, 3, 6, 7, 11 }, { 1, 11, 10 } },
	},
	---HELM---	
	{ { { 1, 2, 3, 6, 7 }, { 1, 2 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 3 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 4, 5, 6 } },
		{ { 1, 2, 3, 6, 7 }, { 7, 8, 9 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 11, 10 } },
	},
	---CUFF---	
	{ { { 1, 2, 3, 6, 7 }, { 1, 2 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 3 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 4, 5, 6 } },
		{ { 1, 2, 3, 6, 7 }, { 7, 8, 9 } },
		{ { 1, 2, 3, 6, 7 }, { 1, 11, 10 } },
	},
	--PENDANT---
	{ { { 1, 2, 3, 5 }, { 2, 12 } },
		{ { 1, 2, 3, 5 }, { 1, 3, 13 } },
		{ { 1, 2, 3, 5 }, { 1, 4, 5, 6 } },
		{ { 1, 2, 3, 5 }, { 8, 9 } },
		{ { 1, 2, 3, 5 }, { 1, 11, 10 } }
	},
	--RING---
	{ { { 1, 2, 3, 4 }, { 2, 12 } },
		{ { 1, 2, 3, 4 }, { 1, 3, 13 } },
		{ { 1, 2, 3, 4 }, { 1, 4, 5, 6 } },
		{ { 1, 2, 3, 4 }, { 8, 9 } },
		{ { 1, 2, 3, 4 }, { 1, 11, 10 } }
	},
	--MELEEWEAPON---
	{ { { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 2, 14, 15 } },
		{ { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 3, 16, 17 } },
		{ { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 4, 5, 18, 19 } },
		{ { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 8, 20, 21 } },
		{ { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 10, 11, 22, 23 } }
	},
	--RANGRATEWEAPON---
	{ { { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 2, 14, 15 } },
		{ { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 3, 16, 17 } },
		{ { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 4, 5, 18, 19 } },
		{ { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 8, 20, 21 } },
		{ { 1, 2, 3, 12, 13, 14, 15, 16, 17 }, { 1, 10, 11, 22, 23 } }
	},
}

function caltest(NpcIndex)
	-- nGenre	= 2;
	-- nLevel 	= 10;
	-- nSeries	= 1;
	-- nLuck	= 70;-- xin hon quai thuong 1 diem may man;
	-- nNumLine  = 6; -- so dong cua trang bi xanh
	-- nDetail =KindItem[nGenre][1];
	-- nParti  =RANDOM(KindItem[nGenre][2],KindItem[nGenre][3]);
	-- CalCulator_Drop(NpcIndex,nGenre,nDetail,nParti,nLevel,nSeries,nLuck,nNumLine);
end

MESERROR = "Trang bÞ kh«ng tån t¹i!!! vui lßng b¸o admin"
function CalCulator_Drop(NpcIndex, nGenre, nDetail, nParti, nLevel, nSeries, nLuck, nNumLine)
	-- Msg2Player("DEBUG:"..nGenre.."-"..nDetail.."-"..nParti.."-"..nLevel.."-"..nSeries.." -"..nLuck.."")
	local RateServer = 100 -- ha cang thap ty le ra do cang cao ;
	local Option1, Option2, Option3, Option4, Option5, Option6, Option7, Option8 = 0, 0, 0, 0, 0, 0, 0, 0;
	local nIndex = 0;

	--------------------------------------------------------------------------
	--nNumLine quy dinh so dong trang bi
	--------------------------------------------------------------------------
	--------------------------------------------------------------------------
	--Tinh toan Dong 1 -> 6 (max 8) - code hien tai cho phep max 6
	--So dong mang gia tri dua vao may man cua nhan vat
	--------------------------------------------------------------------------
	---------------------------------------------------------
	--Tinh toan trang bi
	--Cap do1: 8->20 =[4,5]
	--Cap do2: 21->37 =[5,6]
	--Cap do3: 38->47 =[6,7]
	--Cap do4: 48->60 =[7,8]
	--Cap do5: >60 =[8,10]
	--nLuckRate == muc do vip cua item;
	local nLuckRate = 0;
	local maxlev = 0;
	local minLev = 0;

	--nLuck = RANDOM(70, 100);
	if (nLuck >= 8) and (nLuck <= 20) then
		nLuckRate = RANDOM(4, 5);
		minLev = 2
		maxlev = 5
	elseif (nLuck >= 21) and (nLuck <= 37) then
		nLuckRate = RANDOM(5, 6);
		minLev = 3
		maxlev = 6
	elseif (nLuck >= 38) and (nLuck <= 59) then
		nLuckRate = RANDOM(6, 7);
		minLev = 4
		maxlev = 7
	elseif (nLuck >= 60) and (nLuck < 65) then
		nLuckRate = RANDOM(7, 8);
		minLev = 5
		maxlev = 8
	elseif (nLuck >= 65) then
		nLuckRate = RANDOM(8, 10);
		minLev = 7
		maxlev = 10
	else
		nLuckRate = RANDOM(1, 3);
		minLev = 1
		maxlev = 3
	end

	--nIndex = AddItem(0, 0, nDetail, nParti, nLevel, nSeries, nLuckRate);
	nIndex = AddItem(0, 0, nDetail, nParti, nLevel, nSeries, nLuck, nLuckRate, RANDOM(minLev, maxlev),
		RANDOM(minLev, maxlev), RANDOM(minLev, maxlev), RANDOM(minLev, maxlev), RANDOM(minLev, maxlev))

	DropItem(NpcIndex, nIndex);
	-- AddItemID(tab[1]);
	tab = {};
end

function DropMagicScript(NpcIndex, nMagicId)
	nIndex = AddItem(0, 6, nMagicId, 0, 0, 0, 0, 0);

	DropItem(NpcIndex, nIndex);
end

function DropQuestKey(NpcIndex, nQuestKeyId)
	nIndex = AddItem(0, 4, nQuestKeyId, 0, 0, 0, 0, 0);

	DropItem(NpcIndex, nIndex);
end
