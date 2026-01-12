Include( "\\script\\header\\taskid.lua" )
function main(id)
	local dwid = GetNpcValue(id);
	local szTong = GetTongName()
	local BangID = Name2ID(szTong)
	local nDangCap = GetLevel()
	local TrungSinh = GetTask(TASK_CHUYENSINH);
	if(BangID ~= dwid) then	
	Talk(1,"","Xin lçi! Bµn tiÖc nµy kh«ng thuéc vÒ bang héi cña b¹n")
	return end
	-- if(TrungSinh < 1) then Talk(1,"","Nh©n vËt ch­a chuyÓn sinh kh«ng thÓ tham gia bµn tiÖc") return end
	-- if(TrungSinh > 1) then Talk(1,"","Nh©n vËt chuyÓn sinh 2 kh«ng thÓ tham gia bµn tiÖc") return end
	if(nDangCap > 185) then Talk(1,"","§¨ng cÊp lín h¬n 185 kh«ng thÓ tham gia bµn tiÖc.") return end
	local nTimeid = GetTimerId()
	if(nTimeid > 0) then
		if(nTimeid == 8) then	--task08 timer ban tiec dang chay.
		if(GetTaskTemp(NPCINDEX) ~= 0) then
		Talk(1,"","B¹n ®· tham gia bµn tiÖc. H·y ®Õn gÇn bµn tiÖc mµ b¹n ®· tham gia vµ chê nhËn th­ëng mçi phót")
		return end
		else	-- co timer khac
		Talk(1,"","Xin lçi! B¹n ®ang cã viÖc bËn kh«ng ®­îc tham gia bµn tiÖc nµy")
		return end
	end
	SetTimer(1080, 8)	--timer 08
	SetTaskTemp(NPCINDEX, GetNpcID(id))
	Msg2Player("B¾t ®Çu tham gia bµn tiÖc. H·y lu«n ë gÇn bµn tiÖc ®Ó nhËn th­ëng mçi phót.")
end;

function no()
end;
