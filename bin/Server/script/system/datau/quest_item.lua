Include("\\script\\global\\npcchucnang\\datau.lua"); 
Include("\\script\\source\\sourcejx49.lua")
--script\global\npcchucnang

-- Da quy / duoc pham
ITEM = {
	{1,2,"ThiÕt La H¸n"},
	{2,28,"Lam Thñy Tinh"},
	{3,29,"Tö Thñy Tinh"},
	{4,30,"Lôc Thñy Tinh"},
	{5,20,"TÝnh Hång B¶o Th¹ch"},
	{6,107,"Phóc duyÖn lé (Trung)"},
	{7,106,"Phóc duyÖn lé (TiÓu)"},
	{8,0,"Vâ L©m MËt TÞch"},
	{9,1,"TÈy Tñy Kinh"},
	{10,108,"Phóc duyÖn lé (§¹i)"},
}

function main()
--	dofile("script/system/datau/quest_item.lua")
	finish_item()	
end

function finish_item()
	duocpham()
end
