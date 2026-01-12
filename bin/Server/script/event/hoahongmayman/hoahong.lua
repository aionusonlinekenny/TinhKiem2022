Include( "\\script\\header\\taskid.lua" )
function main(id)
	local dwid = GetNpcValue(id);
	if(GetUUID() ~= dwid) then
	Talk(1,"","MÇm hoa hång kh«ng ph¶i cña b¹n!");
	return end
	SetTaskTemp(NPCINDEX, id);
	Say("Ch¨m sãc hoa hång ra hoa cÇn chi phÝ 1 v¹n l­îng.",2,
	"Ch¨m sãc c©y (100000 l­îng)/truongthanh",
	"KÕt thóc ®èi tho¹i/no")
end

function truongthanh()
	if(GetCash() < 100000) then
	Talk(1,"","Xin lçi! B¹n kh«ng ®ñ ng©n l­îng!")
	return end
	Pay(100000);
	DelNpc(GetTaskTemp(NPCINDEX));
	local w,x,y = GetWorldPos();
	local id = AddNpc(1255,1,SubWorldID2Idx(w),x,y,0);
	SetNpcName(id, "Hoa hång may m¾n cña "..GetName());
	SetNpcCurCamp(id, 6);
	SetNpcLifeTime(id,2400);
	SetNpcValue(id, GetUUID());
	SetNpcScript(id, "\\script\\feature\\npcevent\\hoahongchin.lua");
end;

function no()
end;
