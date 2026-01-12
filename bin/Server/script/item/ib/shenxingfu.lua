Include("\\script\\header\\revivepos_head.lua");
Include("\\script\\header\\taskid.lua");
Include("\\script\\header\\map_helper.lua");
Include("\\script\\header\\forbidmap.lua");
Include("\\script\\lib\\worldlibrary.lua");
Include("\\script\\header\\npcfile.lua")
Include("\\script\\header\\loidai.lua");
Include("\\script\\header\\factionhead.lua");
SELECT_GO = "ThÇn hµnh phï, ®i ®Õn n¬i ng­¬i muèn."
SELECT_REVIVE = "ThiÕt ®Æt ®iÓm håi sinh cho lÇn sau sö dông thæ ®Þa phï."
SELECT_OUT = "Rêi khái/no"
SUCCESS = "ThiÕt ®Æt ®iÓm håi sinh t¹i %s thµnh c«ng."
PLACE_TOGO = "H·y lùa chän n¬i cÇn ®Õn"

function no()
end;

function main(nIndex)
	--dofile("script/item/ib/shenxingfu.lua")
	if(IsForbidUseTownP() == 1) then
		Msg2Player("B¶n ®å nµy cÊm sö dông ThÇn Hµnh Phï!")
		return
	end
	townportal_l()
end;

function townportal_l()
	Say(10195, 3,
		"ThiÕt ®Æt ®iÓm håi sinh/diemphucsinh",
		"Chän n¬i muèn ®Õn/chondiadiem",
		SELECT_OUT)
end

function chondiadiem()
	Say(SELECT_GO, 7,
		SELECT_OUT,
		--"LÊy täa ®é /laytoado",
		"Thµnh thÞ /thanhthi",
		"Th«n trang/thon",
		"M«n ph¸i/monphai",
		"ChiÕn tr­êng Tèng Kim/tongkim",
		"B¶n ®å luyÖn c«ng/bando"
	)
end;

function laytoado()
	local w, x, y = GetWorldPos()
	Say("M¸p sè <color=Green>" .. w .. "<color> täa ®é <color=Yellow>" .. x .. ", " .. y) -- Hi?n th? t?a d?
end;

function thanhthi()
	local count = 1;
	local CityTab = {};

	for i = 1, getn(THANH_ARRAY) do
		CityTab[count] = THANH_ARRAY[i][2] .. "/chonthanh";
		count = count + 1;
	end
	if (count == 1) then
		return
	end;
	CityTab[count] = SELECT_OUT;
	Say(SELECT_GO, count, CityTab)
end;

function chonthanh(nNo)
	local nSel = nNo + 1;
	local count = 1;
	local CityPosTab = {};
	SetTaskTemp(SEL_MOVEARRAY, nSel);
	for i = 1, getn(THANH_ARRAY[nSel][3]) do
		CityPosTab[count] = THANH_ARRAY[nSel][3][i][2] .. "/posthanh";
		count = count + 1;
	end
	if (count == 1) then
		return
	end
	CityPosTab[count] = SELECT_OUT;
	Say(SELECT_GO, count, CityPosTab);
end;

function posthanh(nNo)
	local nSel = nNo + 1;
	local nMoveAR = GetTaskTemp(SEL_MOVEARRAY);
	local w, x, y = Rev2Pos(THANH_ARRAY[nMoveAR][1], THANH_ARRAY[nMoveAR][3][nSel][1]);
	if (x ~= nil and x > 0) then
		if (NewWorld(w, floor(x / 32), floor(y / 32)) == 1) then
			SetFightState(0)
			Msg2Player("Xin h·y ngåi yªn, chóng ta ®i " .. THANH_ARRAY[nMoveAR][3][nSel][2] .. " nµo!");
		end
	end;
end;

function thon()
	local count = 1;
	local CityTab = {};
	for i = 1, getn(THON_ARRAY) do
		CityTab[count] = THON_ARRAY[i][2] .. "/chonthon";
		count = count + 1;
	end
	if (count == 1) then
		return
	end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_GO, count, CityTab)
end;

function chonthon(nNo)
	local nSel = nNo + 1
	local w, x, y = Rev2Pos(THON_ARRAY[nSel][1], THON_ARRAY[nSel][3]);
	if (x ~= nil and x > 0) then
		if (NewWorld(w, floor(x / 32), floor(y / 32)) == 1) then
			SetFightState(0)
			Msg2Player("Xin h·y ngåi yªn, chóng ta ®i " .. THON_ARRAY[nSel][2] .. " nµo!");
		end
	end;
end;

function monphai()
	--if(1 == 1) then
	--Talk(1,"","Chøc n¨ng t¹m thêi ch­a më!");
	--return end
	local count = 1;
	local CityTab = {};

	for i = 1, getn(MONPHAI_ARRAY) do
		CityTab[count] = MONPHAI_ARRAY[i][2] .. "/chonmonphai";
		count = count + 1;
	end
	if (count == 1) then
		return
	end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_GO, count, CityTab)
end;

function chonmonphai(nNo)
	local nSel = nNo + 1;
	local w, x, y = Rev2Pos(MONPHAI_ARRAY[nSel][1], MONPHAI_ARRAY[nSel][3]);
	if (x ~= nil and x > 0) then
		Say("M¸p sè <color=Green>" .. nSel .. "<color> täa ®é <color=Yellow>" .. x .. ", " .. y) -- Hi?n th? t?a d?
		if (NewWorld(w, floor(x / 32), floor(y / 32)) == 1) then
			SetFightState(0)
		end
	end;

	--NewWorld(131,50,164);
end;

function tongkim()
	Say(PLACE_TOGO, 3,
		SELECT_OUT,
		"§iÓm b¸o danh bªn Tèng/ditong",
		"§iÓm b¸o danh bªn Kim/dikim"
	)
end;

function ditong()
	if (NewWorld(325, 1539, 3186) == 1) then
		SetFightState(0)
	end
end;

function dikim()
	if (NewWorld(325, 1581, 3093) == 1) then
		SetFightState(0)
	end
end;

function bando()
	if (GetLevel() < 90) then
		Msg2Player("B¹n ch­a ®ñ ®¼ng cÊp ®i ®Õn n¬i luyÖn c«ng cÊp cao");
		return
	end;
	local count = 1;
	local CityTab = {};

	for i = 1, getn(PLACES_ARRAY) do
		CityTab[count] = GetWayPointName(PLACES_ARRAY[i]) .. "/chonbando";
		count = count + 1;
	end
	if (count == 1) then
		return
	end
	CityTab[count] = SELECT_OUT;
	Say(PLACE_TOGO, count, CityTab)
end;

function chonbando(nNo)
	local nSel = nNo + 1;
	local w, x, y = GetWayPointPos(PLACES_ARRAY[nSel]);
	if (NewWorld(w, x, y) == 1) then
		SetFightState(1)
	end;
end;

function diemphucsinh()
	Say(SELECT_REVIVE, 3,
		SELECT_OUT,
		"Thµnh thÞ /psthanhthi",
		"Th«n trang/psthon",
		"M«n ph¸i/psmonphai"
	)
end;

function psthanhthi()
	local count = 1;
	local CityTab = {};

	for i = 1, getn(THANH_ARRAY) do
		CityTab[count] = THANH_ARRAY[i][2] .. "/pschonthanh";
		count = count + 1;
	end
	if (count == 1) then
		return
	end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_REVIVE, count, CityTab)
end;

function pschonthanh(nNo)
	local nSel = nNo + 1;
	local count = 1;
	local CityPosTab = {};
	SetTaskTemp(SEL_MOVEARRAY, nSel);
	for i = 1, getn(THANH_ARRAY[nSel][3]) do
		CityPosTab[count] = THANH_ARRAY[nSel][3][i][2] .. "/psposthanh";
		count = count + 1;
	end
	if (count == 1) then
		return
	end
	CityPosTab[count] = SELECT_OUT;
	Say(SELECT_REVIVE, count, CityPosTab)
end;

function psposthanh(nNo)
	local nSel = nNo + 1;
	local nMoveAR = GetTaskTemp(SEL_MOVEARRAY);
	SetRevPos(THANH_ARRAY[nMoveAR][1], THANH_ARRAY[nMoveAR][3][nSel][1]);
	Msg2Player(format(SUCCESS, THANH_ARRAY[nMoveAR][3][nSel][2]))
end;

function psthon()
	local count = 1;
	local CityTab = {};
	for i = 1, getn(THON_ARRAY) do
		CityTab[count] = THON_ARRAY[i][2] .. "/pschonthon";
		count = count + 1;
	end
	if (count == 1) then
		return
	end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_REVIVE, count, CityTab)
end;

function pschonthon(nNo)
	local nSel = nNo + 1;
	SetRevPos(THON_ARRAY[nSel][1], THON_ARRAY[nSel][3]);
	Msg2Player(format(SUCCESS, THON_ARRAY[nSel][2]))
end;

function psmonphai()
	if (1 == 1) then
		Talk(1, "", "Chøc n¨ng t¹m thêi ch­a më!");
		return
	end
	local count = 1;
	local CityTab = {};

	for i = 1, getn(MONPHAI_ARRAY) do
		CityTab[count] = MONPHAI_ARRAY[i][2] .. "/pschonmonphai";
		count = count + 1;
	end
	if (count == 1) then
		return
	end
	CityTab[count] = SELECT_OUT;
	Say(SELECT_REVIVE, count, CityTab)
end;

function pschonmonphai(nNo)
	local nSel = nNo + 1;
	SetRevPos(MONPHAI_ARRAY[nSel][1], MONPHAI_ARRAY[nSel][3]);
	Talk(1, "", format(SUCCESS, MONPHAI_ARRAY[nSel][2]))
end;
