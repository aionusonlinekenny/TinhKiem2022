Include("\\script\\global\\npcchucnang\\datau.lua"); 

-- Diem danh vong / phuc duyen
POINT = {
	{1,5,10},
	{2,5,15},
	{3,5,20},
	{4,5,30},
	{5,5,35},
	{6,5,40},
	{7,5,45},
	{8,5,50},
	{9,10,60},
	{10,15,70},
	{11,20,70},
	{12,25,75},
	{13,30,75},
	{14,35,80},
	{15,40,80},
	{16,50,100},
	{17,50,100},
}

function main()
	--dofile("script/system/datau/quest_point.lua")
	finish_point()

end

function finish_point()
	idx = random(1,2)
	
	if idx == 1 then
		i = random(1,17)
		b = random(POINT[i][2],POINT[i][3])
		AddRepute(b)
		Talk(1,"","<color=green>D· TÈu: <color> <color=green> Hoµn thµnh nhiÖm vô, nhËn ®­îc "..b.." ®iÓm Danh väng")
	elseif idx == 2 then
		i = random(1,17)
		b = random(POINT[i][2],POINT[i][3])
		AddFuYuan(b)
		Talk(1,"","<color=green>D· TÈu: <color> <color=yellow> Hoµn thµnh nhiÖm vô, nhËn ®­îc "..b.." ®iÓm Phóc duyªn")
	end
	
	ClearTask()
end
