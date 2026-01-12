--Author: TamLTM
--Date: 09/30/2020
-- Script Uy Thac
Include("\\script\\global\\sourceatn.lua")
Include("\\script\\global\\tasklist.lua")

function main()
	--dofile("script/system/offline.lua")
	--Talk(1, "", "ñy th¸c offline ch­a ho¹t ®éng...")
	utoffline()
end

function utoffline()
	if GetTask(TaskUyThacOffline) == 0 then
		Say(
			"<color=red>HÖ Thèng<color>: thêi gian B¹ch CÇu Hoµn cña b¹n cßn <color=red><color=blue>" ..
			giobch() .. "<color> giê <color=blue>" .. phutbch() .. "<color> phót<color>.", 2,
			"B¾t ®Çu ñy th¸c rêi m¹ng/startut",
			"Tho¸t/no")
	else
		Talk(1, "", "Ng­¬i ®· kÝch ho¹t ñy th¸c, h·y mau tho¸t nh©n vËt.")
	end
end

function no()
end

function giobch()
	if GetTask(TaskBCH) < 60 then
		return 0
	else
		gio = floor((GetTask(TaskBCH) / 60), 2)
		return gio
	end
end

function phutbch()
	if GetTask(TaskBCH) < 60 then
		return GetTask(TaskBCH)
	else
		gio = floor((GetTask(TaskBCH) / 60), 2)
		phut = GetTask(TaskBCH) - gio * 60
		return phut
	end
end

function startut()
	if GetTask(TaskBCH) >= 1 then
		ngay = tonumber(date("%d"))
		thang = tonumber(date("%m"))
		nam = tonumber(date("%y"))
		gio = tonumber(date("%H"))
		phut = tonumber(date("%M"))
		SetTask(TaskUyThacOffline2, thang)
		trunggian = (ngay * 24 * 60) + (gio * 60) + phut
		SetTask(TaskUyThacOffline, trunggian)
		Msg2Player("B¹n ®· ñy th¸c vµo lóc: " ..
		gio .. " giê " .. phut .. " phót, ngµy " .. ngay .. "/" .. thang .. "/" .. nam ..
		", b©y giê b¹n cã thÓ rêi m¹ng.")
	else
		Talk(1, "", "B¹n ®· <color=red>hÕt B¹ch CÇu Hoµn<color>, vui lßng kiÓm tra vµ quay l¹i sau.")
	end
end

function stoput()
	ngay = tonumber(date("%d"))
	thang = tonumber(date("%m"))
	gio = tonumber(date("%H"))
	phut = tonumber(date("%M"))
	songay = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
	if thang == GetTask(TaskUyThacOffline2) then
		hientai = (ngay * 24 * 60) + (gio * 60) + phut
		sophutdaut = (hientai - GetTask(TaskUyThacOffline))
		if GetTask(TaskBCH) >= sophutdaut then
			SetTask(TaskUyThacOffline1, GetTask(TaskUyThacOffline1) + sophutdaut)
			SetTask(TaskUyThacOffline, 0)
			SetTask(TaskBCH, GetTask(TaskBCH) - sophutdaut)
		else
			SetTask(TaskUyThacOffline1, GetTask(TaskUyThacOffline1) + GetTask(TaskBCH))
			SetTask(TaskUyThacOffline, 0)
			SetTask(TaskBCH, 0)
		end
	elseif (thang == (GetTask(TaskUyThacOffline2) + 1)) or (thang == (GetTask(TaskUyThacOffline2) - 11)) then
		thangtruoc = (songay[thang] * 24 * 60) - GetTask(TaskUyThacOffline)
		thangnay = (ngay * 24 * 60) + (gio * 60) + phut
		hientai = thangnay + thangtruoc
		if GetTask(TaskBCH) >= hientai then
			SetTask(TaskUyThacOffline1, GetTask(TaskUyThacOffline1) + hientai)
			SetTask(TaskUyThacOffline, 0)
			SetTask(TaskBCH, GetTask(TaskBCH) - hientai)
		else
			SetTask(TaskUyThacOffline1, GetTask(TaskUyThacOffline1) + GetTask(TaskBCH))
			SetTask(TaskUyThacOffline, 0)
			SetTask(TaskBCH, 0)
		end
	else
		SetTask(TaskUyThacOffline, 0)
		SetTask(TaskUyThacOffline1, 0)
		Talk(1, "",
			"V× lý do b¹n kh«ng vµo game h¬n <color=yellow>1 th¸ng<color> nªn kinh nghiÖm tÝch lòy ®­îc trë vÒ 0.")
	end
end

function expuythac()
	if (GetLevel() > 70) and (GetLevel() <= 90) then
		return 1000
	elseif (GetLevel() > 90) and (GetLevel() <= 110) then
		return 1500
	elseif (GetLevel() > 110) and (GetLevel() <= 130) then
		return 3000
	elseif (GetLevel() > 130) and (GetLevel() <= 150) then
		return 5000
	elseif (GetLevel() > 150) and (GetLevel() <= 170) then
		return 7500
	elseif (GetLevel() > 170) and (GetLevel() <= 190) then
		return 10000
	elseif (GetLevel() > 190) and (GetLevel() <= 195) then
		return 13000
	elseif (GetLevel() > 195) and (GetLevel() <= 200) then
		return 17000
	end
end

function nhanexputoff()
	soexpnd = expuythac() * GetTask(TaskUyThacOffline1)
	if GetTask(TaskUyThacOffline1) >= 15 then
		AddOwnExp(soexpnd)
		SetTask(TaskUyThacOffline1, 0)
	else
		Talk(1, "", "Thêi gian ñy th¸c trªn <color=red>15 phót<color> míi ®­îc nhËn kinh nghiÖm.")
	end
end
