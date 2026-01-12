--Script by Manh Dat
--Xa phu Minh Nguyet


function main(NpcIndex)
	Say("Ng­êi muèn ®i ®©u?",2,
	"Quay vÒ khu vùc bang héi/kvbh",
	"Kh«ng cÇn ®©u/out")
end;

function kvbh()
	if(GetTongID() == 0) then
		Talk(1,"","Ph¶i cã bang míi v« ®­îc khu vùc nµy!")
	return end
	SetFightState(0)
	NewWorld(594, 1703,3297)	--vao map a toa do xy
end;

function out()
end;
