Include("\\script\\lib\\worldlibrary.lua")

--hoangnhk
FUNC_MAIN = "chaohang"
function main(NpcIndex)
	local w, x, y = GetWorldPos();
	if (w == 1) then --phuong tuong
		Talk(1, FUNC_MAIN, 12235)
		return
	end
	if (w == 11) then --thanh do
		Talk(1, FUNC_MAIN, 12235)
		return
	end
	if (w == 162) then --dai ly
		Talk(1, FUNC_MAIN, 12235)
		return
	end
	if (w == 78) then --tuong duong
		Talk(1, FUNC_MAIN, RANDOM(15267, 15268))
		return
	end
	if (w == 37) then --bien kinh
		Talk(1, FUNC_MAIN, 14792)
		return
	end
	if (w == 80) then --duong chau
		Talk(1, FUNC_MAIN, 15453)
		return
	end
	if (w == 176) then --lam an
		Talk(1, FUNC_MAIN, 10831)
		return
	end
	Talk(1, FUNC_MAIN, 12235)
end;

function chaohang()
	local MUA_BAOLIXI = "N¬i nµy cã bao l× x× kh«ng?/xemhang"
	local OK_TRADE = "§æi Kim Nguyªn B¶o sang tiÒn ®ång?/tiendong";
	local MUA_KNB = "Mua Kim Nguyªn B¶o (1000 v¹n)/van2knb";
	local OK_SELL = "§æi Kim Nguyªn B¶o sang tiÒn v¹n?/knb2van";
	local OK_EXCHANGE = "§æi Kim Nguyªn B¶o sang ngµy ch¬i?/knb2day";
	local NOT_TRADE = "Nh©n tiÖn ghÐ qua th«i/no";
	if (GetSex() == 0) then
		Say(10085, 5,
			MUA_BAOLIXI,
			MUA_KNB,
			OK_SELL,
			OK_EXCHANGE,
			NOT_TRADE)
	else
		Say(10086, 5,
			MUA_BAOLIXI,
			MUA_KNB,
			OK_SELL,
			OK_EXCHANGE,
			NOT_TRADE)
	end;
end;

function xemhang()
	Say(GetSex() == 0 and 10085 or 10086, 3,
		"Ta muèn ®æi TiÓu Hång Bao L× X×/doibaotieu",
		"Ta muèn ®æi §¹i Hång Bao L× X×/doibaodai",
		"Ta kh«ng cÇn/no")
end;

function doibaotieu()
	if (GetCash() < 10000) then
		Talk(1, "", "Ta cÇn 10000 l­îng ®Ó gãi bao l× x× ");
		return
	end;
	Pay(10000);
	local nNewItem = AddItemID(AddItem(0, 6, 14, 0, 0, 5, 0, 0));
	Talk(1, "", format("%s nhËn ®­îc mét %s ", GetSex() == 0 and "§¹i HiÖp" or "N÷ HiÖp", GetNameItem(nNewItem)));
end;

function doibaodai()
	if (GetCash() < 100000) then
		Talk(1, "", "Ta cÇn 100000 l­îng ®Ó gãi bao l× x× ");
		return
	end;
	Pay(100000);
	local nNewItem = AddItemID(AddItem(0, 6, 13, 0, 0, 5, 0, 0));
	Talk(1, "", format("%s nhËn ®­îc mét %s ", GetSex() == 0 and "§¹i HiÖp" or "N÷ HiÖp", GetNameItem(nNewItem)));
end;

function tiendong()
	local nKNB = GetItemCount(0, 4, 343);
	if (nKNB < 1) then
		Talk(1, "", 15290)
		return
	end
	--local nNewItem = AddItemID(AddItem(0, 4, 417, 0, 0, 5, 0, 0)) --tien dong 10 cai
	--SetStackItem(nNewItem, 10);
	AddStackItem(100, 4, 417, 0, 0, 5, 0, 0)
	DelItem(1, 0, 4, 343)
	Msg2Player("B¹n ®· ®æi ®­îc 100 tiÒn ®ång.")
end;

function knb2van()
	local nKNB = GetItemCount(0, 4, 343);
	if (nKNB < 1) then
		Talk(1, "", 15290)
		return
	end

	if (DelItem(1, 0, 4, 343) ~= 0) then
		Earn(8000000)
		Msg2Player("B¹n ®· ®æi ®­îc 800 v¹n ng©n l­îng.")
	end
end;

function knb2day()
	local nKNB = GetItemCount(0, 4, 343);
	if (nKNB < 1) then
		Talk(1, "", 15290)
		return
	end

	if (ExchangeExtDay(7) == 1 and DelItem(1, 0, 4, 343) ~= 0) then
		Msg2Player("B¹n ®· ®æi KNB thµnh 7 ngµy ch¬i, tho¸t game ®Ó kiÓm tra!")
		ExchangeLog()
	else
		Msg2Player("Cã lçi khi chuyÓn ®æi ngµy ch¬i! Vui lßng b¸o admin!")
	end
end

function van2knb()
	local nCash = GetCash();
	if (nCash < 10000000) then
		Talk(1, "", "Ng­¬i kh«ng mang ®ñ 1000 v¹n l­îng råi, vÒ ®i!")
		return
	end
	Pay(10000000)
	AddItemEx(4, 343, 0, 0, 5, 0, 0)
	Msg2Player("B¹n ®· ®æi ®­îc 1 Kim Nguyªn B¶o.")
end;

function no()
end;

function ExchangeLog()
	local szAccount = GetAccount()
	local szName = GetName()
	local dwID = GetUUID()
	local nLevel = GetLevel()
	local nFaction = GetFaction()
	local nIP = GetIP()
	local log = "" ..
		date("%H:%M:%S") ..
		"\t Tµi kho¶n: " ..
		szAccount ..
		"\t Nh©n vËt: " ..
		szName ..
		"\t UUID: " ..
		dwID ..
		"\t CÊp: \t" ..
		nLevel .. "\t M«n ph¸i: " .. nFaction .. "\t IP: " .. nIP ..
		"\t ChuyÓn ®æi KNB thµnh 7 ngµy ch¬i thµnh c«ng!\n"
	logWrite("exchange_log", log)
end