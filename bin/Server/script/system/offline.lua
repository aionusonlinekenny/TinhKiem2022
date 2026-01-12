--script uy thac
--author: Ken Nguyen
Include("\\script\\header\\forbidmap.lua");
Include("\\script\\header\\taskid.lua");
function main()
	Say(
		"<color=red>HÖ Thèng<color>: sau khi chän ñy th¸c ng­êi ch¬i sÏ tù ®éng rêi m¹ng, hÖ thèng tù ®éng sÏ céng cho b¹n mét Ýt kinh nghiÖm ®· uû th¸c.\n\n- Thêi gian B¹ch CÇu Hoµn cßn: <color=blue>" ..
		Gio() .. "<color> giê <color=blue>" .. Phut() .. "<color> phót.", 2,
		"B¾t ®Çu ñy th¸c rêi m¹ng/CheckUyThac",
		"TiÕp tôc trß ch¬i/no")
end;

function Gio()
	if GetTask(TASK_OFFLINEMIN) < 60 or GetTask(TASK_OFFLINEMIN) == nil then
		return 0
	else
		gio = floor((GetTask(TASK_OFFLINEMIN) / 60), 2)
		return gio
	end
end

function Phut()
	if (GetTask(TASK_OFFLINEMIN) == nil) then
		return 0
	elseif GetTask(TASK_OFFLINEMIN) < 60 then
		return GetTask(TASK_OFFLINEMIN)
	else
		gio = floor((GetTask(TASK_OFFLINEMIN) / 60), 2)
		phut = GetTask(TASK_OFFLINEMIN) - gio * 60
		return phut
	end
end

function CheckUyThac()
	if (GetCamp() == 0) then
		Talk(1, "", "B¹n ph¶i vµo ph¸i míi cã thÓ ñy th¸c.")
		return
	end
	if (GetFightState() ~= 0) then
		Talk(1, "", "B¹n chØ cã thÓ ñy th¸c ë nh÷ng n¬i phi chiÕn ®Êu nh­ trong thµnh thÞ, th«n.")
		return
	end
	if (CheckMapForTP() == 1) then
		Talk(1, "", "N¬i ®©y kh«ng thÝch hîp cho viÖc ñy th¸c rêi m¹ng!")
		return
	end;
	local nTask = GetTask(TASK_OFFLINEMIN);
	if nTask >= 1 then
		UyThacBCH()
	else
		if GetLevel() < 90 then
			Talk(1, "", "§¼ng cÊp trªn 90 míi cã thÓ ñy th¸c miÔn phÝ.")
			return
		end
		Say(
			"<color=red>HÖ Thèng<color>: hiÖn t¹i ng­¬i ®· hÕt thêi gian B¹ch CÇu Hoµn, b¾t ®Çu sö dông chøc n¨ng ñy th¸c miÔn phÝ (tèi ®a rêi m¹ng 12 giê).",
			2,
			"ñy th¸c miÔn phÝ/UyThacFree",
			"Tho¸t/no")
	end
end

function UyThacBCH()
	local nTask = GetTask(TASK_OFFLINEMIN);
	if (nTask < 1) then
		Talk(1, "", "Thêi gian B¹ch CÇu Hoµn cña b¹n ®· hÕt, vui lßng kiÓm tra l¹i.")
		return
	end;
	SetTask(TASK_LVGAMESEC, GetCurServerSec())
	OfflineLive(); --ham he thong'
end;

function UyThacFree()
	SetTask(TASK_LINHTINH2, SetNumber(GetTask(TASK_LINHTINH2), 2, 1)); -- trang thai mien phi
	--SetTask(TASK_OFFLINEMIN, GetTask(TASK_OFFLINEMIN) + 144400)
	SetTask(TASK_OFFLINERATE, 1)
	SetTask(TASK_LVGAMESEC, GetCurServerSec())
	OfflineLive(); --ham he thong'
end

function no()
end;
