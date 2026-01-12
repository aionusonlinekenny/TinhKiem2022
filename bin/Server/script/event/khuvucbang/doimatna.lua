Include("\\script\\library\\worldlibrary.lua");

TIENDOIMNA = 10000000
XUDOIMNA = 1000


function doimatnanew()
	Say("Mêi chän:",4,
		"MÆt n¹ cÊp 1/choncap",
		"MÆt n¹ cÊp 2/choncap",
		"MÆt n¹ cÊp 3/choncap",
		"KÕt thóc ®èi tho¹i/no")
end

function choncap(sel)
nDat = sel+1
if (nDat == 1) then
if GetFactionEx() == "TL" then
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"QuyÒn Ph¸p/ThieuLamTC11","C«n Ph¸p/ThieuLamTC11","§ao Ph¸p/ThieuLamTC11","Tho¸t/no")
elseif GetFactionEx() == "TV" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"Chïy Ph¸p/ThienVuongTC11","Th­¬ng Ph¸p/ThienVuongTC11","§ao Ph¸p/ThienVuongTC11","Tho¸t/no")
elseif GetFactionEx() == "DM" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"Phi §ao/DuongMonTC11","Phi Tiªu/DuongMonTC11","Tô TiÔn/DuongMonTC11","Tho¸t/no")
elseif GetFactionEx() == "ND" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"Ch­ëng Ph¸p/NguDocTC11","§ao Ph¸p/NguDocTC11","Tho¸t/no")
elseif GetFactionEx() == "NM" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KiÕm Ph¸p/NgaMiTC11","Ch­ëng Ph¸p/NgaMiTC11","Tho¸t/no")
elseif GetFactionEx() == "TY" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"§¬n §ao/ThuyYenTC11","Song §ao/ThuyYenTC11","Tho¸t/no")
elseif GetFactionEx() == "CB" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"Ch­ëng Ph¸p/CaiBangTC11","Bæng Ph¸p/CaiBangTC11","Tho¸t/no")
elseif GetFactionEx() == "TN" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"M©u Ph¸p/ThienNhanTC11","§ao Ph¸p/ThienNhanTC11","Tho¸t/no")
elseif GetFactionEx() == "VD" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KiÕm Ph¸p/VoDangTC11","QuyÒn Ph¸p/VoDangTC11","Tho¸t/no")
elseif GetFactionEx() == "CL" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"§ao Ph¸p/ConLonTC11","KiÕm Ph¸p/ConLonTC11","Tho¸t/no")
elseif GetFactionEx() == "HS" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 1 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KhÝ T«ng/HoaSonTC11","KiÕm T«ng/HoaSonTC11","Tho¸t/no")
end
elseif (nDat == 2) then
if GetFactionEx() == "TL" then
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"QuyÒn Ph¸p/ThieuLamTC25","C«n Ph¸p/ThieuLamTC25","§ao Ph¸p/ThieuLamTC25","Tho¸t/no")
elseif GetFactionEx() == "TV" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"Chïy Ph¸p/ThienVuongTC25","Th­¬ng Ph¸p/ThienVuongTC25","§ao Ph¸p/ThienVuongTC25","Tho¸t/no")
elseif GetFactionEx() == "DM" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"Phi §ao/DuongMonTC25","Phi Tiªu/DuongMonTC25","Tô TiÔn/DuongMonTC25","Tho¸t/no")
elseif GetFactionEx() == "ND" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"Ch­ëng Ph¸p/NguDocTC25","§ao Ph¸p/NguDocTC25","Tho¸t/no")
elseif GetFactionEx() == "NM" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KiÕm Ph¸p/NgaMiTC25","Ch­ëng Ph¸p/NgaMiTC25","Tho¸t/no")
elseif GetFactionEx() == "TY" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"§¬n §ao/ThuyYenTC25","Song §ao/ThuyYenTC25","Tho¸t/no")
elseif GetFactionEx() == "CB" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"Ch­ëng Ph¸p/CaiBangTC25","Bæng Ph¸p/CaiBangTC25","Tho¸t/no")
elseif GetFactionEx() == "TN" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"M©u Ph¸p/ThienNhanTC25","§ao Ph¸p/ThienNhanTC25","Tho¸t/no")
elseif GetFactionEx() == "VD" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KiÕm Ph¸p/VoDangTC25","QuyÒn Ph¸p/VoDangTC25","Tho¸t/no")
elseif GetFactionEx() == "CL" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"§ao Ph¸p/ConLonTC25","KiÕm Ph¸p/ConLonTC25","Tho¸t/no")
elseif GetFactionEx() == "HS" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 2 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KhÝ T«ng/HoaSonTC25","KiÕm T«ng/HoaSonTC25","Tho¸t/no")
end
else
if GetFactionEx() == "TL" then
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"QuyÒn Ph¸p/ThieuLamTC35","C«n Ph¸p/ThieuLamTC35","§ao Ph¸p/ThieuLamTC35","Tho¸t/no")
elseif GetFactionEx() == "TV" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"Chïy Ph¸p/ThienVuongTC35","Th­¬ng Ph¸p/ThienVuongTC35","§ao Ph¸p/ThienVuongTC35","Tho¸t/no")
elseif GetFactionEx() == "DM" then        
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",4,"Phi §ao/DuongMonTC35","Phi Tiªu/DuongMonTC35","Tô TiÔn/DuongMonTC35","Tho¸t/no")
elseif GetFactionEx() == "ND" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"Ch­ëng Ph¸p/NguDocTC35","§ao Ph¸p/NguDocTC35","Tho¸t/no")
elseif GetFactionEx() == "NM" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KiÕm Ph¸p/NgaMiTC35","Ch­ëng Ph¸p/NgaMiTC35","Tho¸t/no")
elseif GetFactionEx() == "TY" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"§¬n §ao/ThuyYenTC35","Song §ao/ThuyYenTC35","Tho¸t/no")
elseif GetFactionEx() == "CB" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"Ch­ëng Ph¸p/CaiBangTC35","Bæng Ph¸p/CaiBangTC35","Tho¸t/no")
elseif GetFactionEx() == "TN" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"M©u Ph¸p/ThienNhanTC35","§ao Ph¸p/ThienNhanTC35","Tho¸t/no")
elseif GetFactionEx() == "VD" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KiÕm Ph¸p/VoDangTC35","QuyÒn Ph¸p/VoDangTC35","Tho¸t/no")
elseif GetFactionEx() == "CL" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"§ao Ph¸p/ConLonTC35","KiÕm Ph¸p/ConLonTC35","Tho¸t/no")
elseif GetFactionEx() == "HS" then       
Say("Vui lßng chän MÆt N¹ Hoµng Kim cÊp 3 cÇn ®æi cña ph¸i<color=yellow> "..GetFaction().." <color>",3,"KhÝ T«ng/HoaSonTC35","KiÕm T«ng/HoaSonTC35","Tho¸t/no")
end
end
end

function ThieuLamTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC1","n/no")
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC12","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC13","n/no")	
	end
end

function ThienVuongTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC12","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC13","n/no")	
	end
end

function NgaMiTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NgaMiTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NgaMiTC12","n/no")	
	end
end

function ThuyYenTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThuyYenTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThuyYenTC12","n/no")	
	end
end

function NguDocTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NguDocTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NguDocTC12","n/no")	
	end
end

function DuongMonTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC12","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC13","n/no")
	end
end

function CaiBangTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/CaiBangTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/CaiBangTC12","n/no")	
	end
end

function ThienNhanTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienNhanTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienNhanTC12","n/no")	
	end
end

function VoDangTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/VoDangTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/VoDangTC12","n/no")
	end
end

function ConLonTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ConLonTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ConLonTC12","n/no")	
	end
end

function HoaSonTC11(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/HoaSonTC1","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/HoaSonTC12","n/no")	
	end
end


function ThieuLamTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC2","n/no")
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC22","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC23","n/no")	
	end
end

function ThienVuongTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC22","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC23","n/no")	
	end
end

function NgaMiTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NgaMiTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NgaMiTC22","n/no")	
	end
end

function ThuyYenTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThuyYenTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThuyYenTC22","n/no")	
	end
end

function NguDocTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NguDocTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NguDocTC22","n/no")	
	end
end

function DuongMonTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC22","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC23","n/no")
	end
end

function CaiBangTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/CaiBangTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/CaiBangTC22","n/no")	
	end
end

function ThienNhanTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienNhanTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienNhanTC22","n/no")	
	end
end

function VoDangTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/VoDangTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/VoDangTC22","n/no")
	end
end

function ConLonTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ConLonTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ConLonTC22","n/no")	
	end
end

function HoaSonTC25(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/HoaSonTC2","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/HoaSonTC22","n/no")	
	end
end


function ThieuLamTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC3","n/no")
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC32","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThieuLamTC33","n/no")	
	end
end

function ThienVuongTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC32","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienVuongTC33","n/no")	
	end
end

function NgaMiTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NgaMiTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NgaMiTC32","n/no")	
	end
end

function ThuyYenTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThuyYenTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThuyYenTC32","n/no")	
	end
end

function NguDocTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NguDocTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/NguDocTC32","n/no")	
	end
end

function DuongMonTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC32","n/no")	
	elseif (sel == 2) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/DuongMonTC33","n/no")
	end
end

function CaiBangTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/CaiBangTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/CaiBangTC32","n/no")	
	end
end

function ThienNhanTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienNhanTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ThienNhanTC32","n/no")	
	end
end

function VoDangTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/VoDangTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/VoDangTC32","n/no")
	end
end

function ConLonTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ConLonTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/ConLonTC32","n/no")	
	end
end

function HoaSonTC35(sel)
	if (sel == 0) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/HoaSonTC3","n/no")	
	elseif (sel == 1) then
	PutItem("§æi mÆt n¹ cÇn: \n1.1000 v¹n vµ 1000 xu","y/HoaSonTC32","n/no")	
	end
end



function ThieuLamTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4563-2,0,0,0,2)
	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tTLQ")
	 RemoveItem(nRealIndex,1);	
end
function ThieuLamTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4564-2,0,0,0,2)
	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tTLB")
	 RemoveItem(nRealIndex,1);			
end

function ThieuLamTC13()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4565-2,0,0,0,2)
	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tTLD")
	 RemoveItem(nRealIndex,1);			
end

function ThienVuongTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4567-2,0,0,0,2)
	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tTVC")
	 RemoveItem(nRealIndex,1);			
end
function ThienVuongTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4568-2,0,0,0,2)
	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tTVT")
	 RemoveItem(nRealIndex,1);			
end

function ThienVuongTC13()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4566-2,0,0,0,2)
	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tTVD")
	 RemoveItem(nRealIndex,1);			
end

function NgaMiTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4569-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NMK")
	 RemoveItem(nRealIndex,1);			
end
function NgaMiTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4570-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NMC")
	 RemoveItem(nRealIndex,1);			
end

function ThuyYenTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4571-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TYD")
	 RemoveItem(nRealIndex,1);			
end

function ThuyYenTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4572-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TYSD")
	 RemoveItem(nRealIndex,1);			
end

function NguDocTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4573-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tNDC")
	 RemoveItem(nRealIndex,1);			
end

function NguDocTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4574-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tNDD")
	 RemoveItem(nRealIndex,1);	
end

function DuongMonTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4575-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tDMPD")
	 RemoveItem(nRealIndex,1);			
end
function DuongMonTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4577-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tDMPT")
	 RemoveItem(nRealIndex,1);			
end
function DuongMonTC13()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4576-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\tDMTT")
	 RemoveItem(nRealIndex,1);			
end

function CaiBangTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4578-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CBR")
	 RemoveItem(nRealIndex,1);			
end
function CaiBangTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4579-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CBR")
	 RemoveItem(nRealIndex,1);			
end

function ThienNhanTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4580-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TNK")
	 RemoveItem(nRealIndex,1);			
end
function ThienNhanTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4581-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TND")
	 RemoveItem(nRealIndex,1);			
end

function VoDangTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4583-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t VDK")
	 RemoveItem(nRealIndex,1);			
end
function VoDangTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4582-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t VDQ")
	 RemoveItem(nRealIndex,1);			
end

function ConLonTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4584-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CLD")
	 RemoveItem(nRealIndex,1);			
end
function ConLonTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,4585-2,0,0,0,2)

	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CLK")
	 RemoveItem(nRealIndex,1);			
end

function HoaSonTC1()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,5318-2,0,0,0,2)
	 SetItemHS(Item, 1)
	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t HSKHI")
	 RemoveItem(nRealIndex,1);			
end
function HoaSonTC12()
	if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
	if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
	 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
	 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
	 local count = 0;
	 for i=0,5 do
	  for j=0,3 do
	  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
	  if (nIndex > 0) then
	   count = count + 1;
	   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
		= nIndex,kind,genre,detail,parti,level,series,row;
	  end
	  end
	 end
	 if(count ~= 1) then
	 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
	 return end
	 if(Rkind ~= 2) then
	 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
	 return end
	 if(Rrow < 4563 or Rrow > 4585) and (Rrow < 5317 or Rrow > 5318) then
	 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
	 return end
	 local HoaSon = GetItemHS(nRealIndex);
	 Pay(TIENDOIMNA)
	 PayCoin(XUDOIMNA)
	 Item = ItemSetAdd(Rkind,0,5317-2,0,0,0,2)
	 SetItemHS(Item, 1)
	 SetItemDate(Item, 86400)
	 SetItemRich(Item, 3000)
	 AddItemID(Item); 
	 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
	 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t HSKIEM")
	 RemoveItem(nRealIndex,1);			
end


function ThieuLamTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4586-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TLQ2")
		 RemoveItem(nRealIndex,1);		
end
function ThieuLamTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4587-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TLB2")
		 RemoveItem(nRealIndex,1);			
end
function ThieuLamTC23()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4588-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TLD2")
		 RemoveItem(nRealIndex,1);			
end

function ThienVuongTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4590-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TVC2")
		 RemoveItem(nRealIndex,1);			
end
function ThienVuongTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4591-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TVT2")
		 RemoveItem(nRealIndex,1);			
end
function ThienVuongTC23()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4589-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TVD2")
		 RemoveItem(nRealIndex,1);			
end

function NgaMiTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4592-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NMK2")
		 RemoveItem(nRealIndex,1);			
end
function NgaMiTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4593-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NMC2")
		 RemoveItem(nRealIndex,1);		
end

function ThuyYenTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4594-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TYD2")
		 RemoveItem(nRealIndex,1);			
end
function ThuyYenTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4595-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TYSD2")
		 RemoveItem(nRealIndex,1);		
end

function NguDocTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4596-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NDC2")
		 RemoveItem(nRealIndex,1);			
end
function NguDocTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4597-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NDD2")
		 RemoveItem(nRealIndex,1);			
end

function DuongMonTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4598-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t DMPD2")
		 RemoveItem(nRealIndex,1);			
end
function DuongMonTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4600-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t DMPT2")
		 RemoveItem(nRealIndex,1);			
end
function DuongMonTC23()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4599-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t DMTT2")
		 RemoveItem(nRealIndex,1);			
end

function CaiBangTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4601-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CBR2")
		 RemoveItem(nRealIndex,1);			
end
function CaiBangTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4602-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CBB2")
		 RemoveItem(nRealIndex,1);			
end

function ThienNhanTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4603-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TNK2")
		 RemoveItem(nRealIndex,1);			
end
function ThienNhanTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4604-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TND2")
		 RemoveItem(nRealIndex,1);			
end

function VoDangTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4606-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t VDK2")
		 RemoveItem(nRealIndex,1);			
end
function VoDangTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4605-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t VDQ2")
		 RemoveItem(nRealIndex,1);			
end

function ConLonTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4607-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CLD2")
		 RemoveItem(nRealIndex,1);			
end
function ConLonTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4608-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CLK2")
		 RemoveItem(nRealIndex,1);			
end

function HoaSonTC2()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,5320-2,0,0,0,2)
		 SetItemHS(Item, 1)
		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t HSKHI2")
		 RemoveItem(nRealIndex,1);			
end

function HoaSonTC22()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4586 or Rrow > 4608) and (Rrow < 5319 or Rrow > 5320) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,5319-2,0,0,0,2)
		 SetItemHS(Item, 1)
		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t HSKIEM2")
		 RemoveItem(nRealIndex,1);			
end



function ThieuLamTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4609-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TLQ 3")
		 RemoveItem(nRealIndex,1);		
end
function ThieuLamTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4610-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TLB 3")
		 RemoveItem(nRealIndex,1);			
end
function ThieuLamTC33()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4611-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TLD 3")
		 RemoveItem(nRealIndex,1);			
end

function ThienVuongTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4613-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TVC 3")
		 RemoveItem(nRealIndex,1);			
end
function ThienVuongTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4614-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TVT 3")
		 RemoveItem(nRealIndex,1);			
end
function ThienVuongTC33()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4612-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TVD 3")
		 RemoveItem(nRealIndex,1);			
end

function NgaMiTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4615-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NMK 3")
		 RemoveItem(nRealIndex,1);			
end
function NgaMiTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4616-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NMC 3")
		 RemoveItem(nRealIndex,1);			
end

function ThuyYenTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4617-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TYD 3")
		 RemoveItem(nRealIndex,1);			
end
function ThuyYenTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4618-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TYSD 3")
		 RemoveItem(nRealIndex,1);			
end

function NguDocTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4619-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NDC 3")
		 RemoveItem(nRealIndex,1);			
end
function NguDocTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4620-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t NDD 3")
		 RemoveItem(nRealIndex,1);			
end

function DuongMonTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4621-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t DMPD 3")
		 RemoveItem(nRealIndex,1);			
end
function DuongMonTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4623-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t DMPT 3")
		 RemoveItem(nRealIndex,1);			
end
function DuongMonTC33()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4622-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t DMTT 3")
		 RemoveItem(nRealIndex,1);			
end

function CaiBangTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4624-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CBR 3")
		 RemoveItem(nRealIndex,1);			
end
function CaiBangTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4625-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CBB 3")
		 RemoveItem(nRealIndex,1);			
end

function ThienNhanTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4626-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TNK 3")
		 RemoveItem(nRealIndex,1);			
end
function ThienNhanTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4627-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t TND 3")
		 RemoveItem(nRealIndex,1);			
end

function VoDangTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4629-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t VDK 3")
		 RemoveItem(nRealIndex,1);			
end
function VoDangTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4628-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t VDQ 3")
		 RemoveItem(nRealIndex,1);			
end

function ConLonTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4630-2,0,0,0,2)

		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CLD 3")
		 RemoveItem(nRealIndex,1);			
end
function ConLonTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,4631-2,0,0,0,2)
		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t CLK 3")
		 RemoveItem(nRealIndex,1);			
end

function HoaSonTC3()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,5322-2,0,0,0,2)
		 SetItemHS(Item, 1)
		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t HSKHI 3")
		 RemoveItem(nRealIndex,1);			
end

function HoaSonTC32()
		if (GetCash() < TIENDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 v¹n ®Ó ®æi mÆt n¹") return end
		if (GetCoin() < XUDOIMNA) then Talk(1,"","B¹n kh«ng cã ®ñ 1000 xu ®Ó ®æi mÆt n¹") return end
		 local i,j, nIndex,kind,genre,detail,parti,level,series,row;
		 local nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow;
		 local count = 0;
		 for i=0,5 do
		  for j=0,3 do
		  nIndex,kind,genre,detail,parti,level,series,row = GetItemParam(10,i,j);
		  if (nIndex > 0) then
		   count = count + 1;
		   nRealIndex,Rkind,Rgenre,Rdetail,Rparti,Rlevel,Rseries,Rrow
			= nIndex,kind,genre,detail,parti,level,series,row;
		  end
		  end
		 end
		 if(count ~= 1) then
		 Msg2Player("Trèng hoÆc bá nhiÒu mãn qu¸!")
		 return end
		 if(Rkind ~= 2) then
		 Msg2Player("ChØ ®æi mÆt n¹, kh«ng ®æi ®å th­êng")
		 return end
		 if(Rrow < 4609 or Rrow > 4631) and (Rrow < 5321 or Rrow > 5322) then
		 Msg2Player("ChØ cã thÓ bá trang bÞ MÆt N¹ Hoµng Kim vµo ®©y")
		 return end
		 local HoaSon = GetItemHS(nRealIndex);
		 Pay(TIENDOIMNA)
		 PayCoin(XUDOIMNA)
		 Item = ItemSetAdd(Rkind,0,5321-2,0,0,0,2)
		 SetItemHS(Item, 1)
		 SetItemDate(Item, 86400)
		 SetItemRich(Item, 3000)
		 AddItemID(Item); 
		 Msg2Player("B¹n ®· ®æi mÆt n¹ thµnh c«ng <color=yellow> Thµnh C«ng")
		 logBangHoi("["..GetName().."]\t\t[TK: "..GetAccount().."]\t\t[IP: "..GetIP().."]\t\tDoi Mat Na\t\t HSKIEM 3")
		 RemoveItem(nRealIndex,1);			
end

