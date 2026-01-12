Include("\\script\\lib\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");

function main()
	local nTask = GetNumber(1, GetTask(TASK_NVDANHVONG), 6);
	if (nTask == 1 and GetItemCount(0, 4, 181) < 1) then
		Say(11255, 2, "Mua/muabong", "T¸n gÉu/tangau")
		return
	end
	tangau()
end

function muabong()
	if (GetCash() < 200) then
		Talk(1, "", 11261)
	else
		AddItemID(AddItem(0, 4, 181, 0, 0, 5, 0, 0))
		Pay(200)
		Msg2Player("Mua ®­îc 1 ®«i b«ng tay.")
		Talk(1, "", 11260)
	end
end

function tangau()
	Talk(3, "morongruong", 11257, 11258, 11259)
end

function morongruong()
	if (GetExpandBox() == 0) then
		Say(11256, 2, "Mua r­¬ng më réng/muaruong", "Kh«ng mua/no")
	elseif (GetExpandBox() == 1) then
		Say(11256, 2, "Mua r­¬ng më réng 2 (3 KNB)/muaruong2", "Kh«ng mua/no")
	elseif (GetExpandBox() == 2) then
		Say(11256, 2, "Mua r­¬ng më réng 3 (4 KNB)/muaruong3", "Kh«ng mua/no")
	elseif (GetExpandBox() == 3) then
		Say(11256, 2, "Mua r­¬ng më réng 4 (5 KNB)/muaruong4", "Kh«ng mua/no")
	elseif (GetExpandBox() == 4) then
		Say(11256, 2, "Mua r­¬ng më réng 5 (6 KNB)/muaruong5", "Kh«ng mua/no")
	else
		if (GetSavePw() == "") then
			Say(11256, 2, "T¹o m· khãa b¶o vÖ/taoma", "KÕt thóc ®èi tho¹i/no")
		else
			Say(11256, 2, "§æi m· khãa b¶o vÖ/doima", "KÕt thóc ®èi tho¹i/no")
		end;
	end
end

function taoma()
	Input("taoma2")
end;

function taoma2(x)
	SetSavePw(x);
end;

function doima()
	Msg2Player("<sex> h·y nhËp lÇn l­ît mËt khÈu cò vµ mËt khÈu míi ph©n biÖt b»ng kı tù <color=yellow><SPACE> <color>");
	Input("doima2");
end;

function doima2(x)
	if (GetSavePw() == GetParam(x, 1, " ")) then
		SetSavePw(GetParam(x, 2, " "));
	end
end;

function muaruong()
	if (GetItemCount(0, 4, 343) >= 2) then --2 hoang kim bao
		DelItem(2, 0, 4, 343)
		SetExpandBox(1)
		OpenBox(1)
		Talk(1, "morongruong", 11263)
	else
		Talk(1, "", 11261)
	end
end

function muaruong2()
	if (GetItemCount(0, 4, 343) >= 3) then --3 hoang kim bao
		DelItem(3, 0, 4, 343)
		SetExpandBox(2)
		OpenBox(1)
		Talk(1, "", 11263)
	else
		Talk(1, "", "Ng­¬i kh«ng mang ®ñ 3 KNB råi!")
	end
end

function muaruong3()
	if (GetItemCount(0, 4, 343) >= 4) then --2 hoang kim bao
		DelItem(4, 0, 4, 343)
		SetExpandBox(3)
		OpenBox(1)
		Talk(1, "", 11263)
	else
		Talk(1, "", "Ng­¬i kh«ng mang ®ñ 4 KNB råi!")
	end
end

function muaruong4()
	if (GetItemCount(0, 4, 343) >= 5) then --2 hoang kim bao
		DelItem(5, 0, 4, 343)
		SetExpandBox(4)
		OpenBox(1)
		Talk(1, "", 11263)
	else
		Talk(1, "", "Ng­¬i kh«ng mang ®ñ 5 KNB råi!")
	end
end

function muaruong5()
	if (GetItemCount(0, 4, 343) >= 6) then --2 hoang kim bao
		DelItem(6, 0, 4, 343)
		SetExpandBox(5)
		OpenBox(1)
		Talk(1, "", 11263)
	else
		Talk(1, "", "Ng­¬i kh«ng mang ®ñ 6 KNB råi!")
	end
end

function no()
end
