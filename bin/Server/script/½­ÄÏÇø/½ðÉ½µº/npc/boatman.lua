-- Î÷É½Óì´¬·ò

function main()
	Say("Ng­¬i muèn ngåi thuyÒn ®Õn D­¬ng Ch©u µh? 5000 l­îng!", 2, "§­îc th«i! §i nµo!/toyangzhou", "Kh«ng ngåi thuyÒn ®©u/no");
end

function toyangzhou()
	if( GetCash() >= 5000 ) then
		NewWorld( 80,1369,3369 );
		DisabledUseTownP(0)	-- ²»ÏŞÖÆÆäÀë¿ªÎ÷É½Óì·µ»ØÑïÖİºóÊ¹ÓÃ»Ø³Ç·û
		SetFightState(1);
		ForbidChangePK(0);
		Pay( 5000 );
	else
		Talk( 1,"","Kh«ng tiÒn kh«ng thÓ lªn thuyÒn!");
	end
end

function no()
end