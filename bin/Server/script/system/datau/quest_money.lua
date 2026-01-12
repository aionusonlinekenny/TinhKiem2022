Include("\\script\\global\\npcchucnang\\datau.lua"); 

--Tien
MONEY = {
	{1,1000,2000},
	{2,1000,2000},
	{3,1000,2000},
	{4,1000,2000},
	{5,1000,10000},
	{6,1000,10000},
	{7,1000,10000},
	{8,1000,10000},
	{9,10000,15000},
	{10,10000,15000},
	{11,10000,15000},
	{12,10000,15000},
	{13,10000,20000},
	{14,10000,20000},
	{15,10000,20000},
	{16,20000,30000},
	{17,20000,30000},
}

function main()
--	Msg2Player("Nhan Tien")
--	dofile("script/system/datau/quest_money.lua")
	finish_money()
end

function finish_money()
	tien()
end