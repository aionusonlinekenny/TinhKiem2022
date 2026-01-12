//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcFindPath.h
// Date:	2002.01.06
// Code:	边城浪子
// Desc:	Obj Class
//---------------------------------------------------------------------------

#ifndef KNPCFINDPATH_H
#define KNPCFINDPATH_H

class KNpcFindPath
{
public:
	INT				m_NpcIdx;			// 这个寻路是属于哪个 npc 的
	INT				m_nDestX;			// 目标点坐标 x
	INT				m_nDestY;			// 目标点坐标 y
	INT				m_nFindTimer;		// 找路，一段时间以后没找到就不找了
	INT				m_nMaxTimeLong;		// 找多长时间的路
	INT				m_nFindState;		// 当前是否处于找路状态
	INT				m_nPathSide;		// 往哪个方向找
	INT				m_nFindTimes;		// 朝一个目标点找路的次数
public:
	KNpcFindPath();
	//	功能：初始化	nNpc :这个寻路是属于哪个 npc 的
	void				Init(INT nNpc);
	//	功能：传入当前坐标、方向、目标点坐标、速度，寻路找到下一步应该走的方向
	//	返回值；如果返回false：不能走了;true ，找到一个方向，方向值放在pnGetDir (按64方向)
	INT					GetDir(INT nXpos, INT nYpos, INT nDir, INT nDestX, INT nDestY, INT nMoveSpeed, INT* pnGetDir);
	// 64 方向转换为 8 方向
	INT				Dir64To8(INT nDir);
	// 8 方向转换为 64 方向
	INT				Dir8To64(INT nDir);
	// 判断两点间的直线距离是否大于或等于给定距离
	BOOL				CheckDistance(INT x1, INT y1, INT x2, INT y2, INT nDistance);
	// 判断某个点是否是障碍
	INT				CheckBarrier(INT nChangeX, INT nChangeY);

	INT					CheckBarrierMin(INT nChangeX, INT nChangeY);
};
#endif
