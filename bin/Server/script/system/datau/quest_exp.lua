--Author: kakashi
--Date: 07/11/12
Include("\\script\\global\\npcchucnang\\datau.lua"); 

--Kinh nghiem
EXP = {
	{1,10000,30000},
	{2,10000,40000},
	{3,20000,50000},
	{4,20000,60000},
	{5,30000,70000},
	{6,40000,80000},
	{7,50000,90000},
	{8,50000,100000},
	{9,60000,110000},
	{10,70000,120000},
	{11,80000,130000},
	{12,80000,140000},
	{13,90000,150000},
	{14,90000,160000},
	{15,100000,170000},
	{16,100000,180000},
	{17,100000,180000},
}

function main()
	-- Msg2Player("Nhan EXP")
	-- dofile("script/system/datau/quest_exp.lua")
	finish_exp()
end

function finish_exp()
	kinhnghiem()
end