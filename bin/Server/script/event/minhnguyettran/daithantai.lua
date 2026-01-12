---Script by Manh Dat
---Tho Lam Phao
Include("\\script\\header\\event_endtime.lua");
Include("\\script\\library\\worldlibrary.lua");
Include("\\script\\header\\taskid.lua");
END_TALK = "KÕt thóc ®èi tho¹i/no";
NHANROI = "Ng­¬i ®· ®æi phÇn th­ëng nµy råi.";
MAX_RESTTIME = 1

function main(NpcIndex)
	Say2("<color=red>§¹i ThÇn Tµi<color>: sù kiÖn <color=blue>Phóc Léc Thä <color> do ta phô tr¸ch, ngµi t×m ta cã viÖc g×?",4,1,"",
	"TÆng ThÇn Tµi Ch÷ Phóc Léc Thä/PhucLocTho",
	END_TALK)
end


function PhucLocTho()
	EVENT = {
	{108,"Ch÷ Phóc "},
	{109,"Ch÷ Léc "},
	{110,"Ch÷ Thä "},
	{90,"Ph¸o Hoa Ngµy TÕt"}}
	KiemTraNL1 = GetItemCount(EVENT[1][1],5);
	KiemTraNL2 = GetItemCount(EVENT[2][1],5);
	KiemTraNL3 = GetItemCount(EVENT[3][1],5);
	if (KiemTraNL1 < 1) then Talk(1,"","Kh«ng cã ch÷ Phóc, b¹n ®ang cã <color=red>"..KiemTraNL1.."/<color> <color=pink>"..EVENT[1][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL2 < 1) then Talk(1,"","Kh«ng cã ch÷ Léc, b¹n ®ang cã <color=red>"..KiemTraNL2.."/<color> <color=pink>"..EVENT[2][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if (KiemTraNL3 < 1) then Talk(1,"","Kh«ng cã ch÷ Thä, b¹n ®ang cã <color=red>"..KiemTraNL3.."/<color> <color=pink>"..EVENT[3][2].."<color>, vui lßng kiÓm tra l¹i.") return end
	if(CheckRoom(1,1) == 0) then Talk(1,"","Vui lßng chõa Ýt nhÊt 1 « trèng.") return end
		local Item = ItemSetAdd(0,5,EVENT[4][1],0,0,5,1,0);
		SetItemDate(Item,ENAM,ETHANG,ENGAY,0,0);
		LockItem(Item);
		AddItemID(Item);
		DelItem(EVENT[1][1],5,1)
		DelItem(EVENT[2][1],5,1)
		DelItem(EVENT[3][1],5,1)
		Msg2Player("B¹n ®æi thµnh c«ng 1 <color=yellow>"..EVENT[4][2].."<color>.")
end

function noinput()
end

function no()
end