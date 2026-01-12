function main(sel)
if ( GetFightState() == 1 ) then	-- 玩家处于非战斗状态，即在城内
SetPos(1631,3183)		-- 设置走出Trap点，目的点在城外	
SetFightState(0)		-- 转换为战斗状态
else			       		-- 玩家处于战斗状态，即在城外
end;
end;