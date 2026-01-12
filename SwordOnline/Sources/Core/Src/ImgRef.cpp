#include "KCore.h"
#include "../../Represent/iRepresent/iRepresentShell.h"
#include "KSubWorldSet.h"

iRepresentShell* g_pRepresent = 0;

UINT	l_Time = 0;

UINT IR_GetCurrentTime()
{
	return l_Time;
}

//--------------------------------------------------------------------------
//	功能：更新图形换帧计算用时钟
//--------------------------------------------------------------------------
void IR_UpdateTime()
{
	l_Time = timeGetTime();
}

//--------------------------------------------------------------------------
//	功能：换帧计算
//--------------------------------------------------------------------------
void IR_NextFrame(INT& nFrame, INT nTotalFrame, UINT UINTerval, UINT& uFlipTime)
{
	if (nTotalFrame > 1 && UINTerval)
	{
		while ((l_Time - uFlipTime) >= UINTerval)
		{
			uFlipTime += UINTerval;
			if ((++nFrame) >= nTotalFrame)
			{
				uFlipTime = l_Time;
				nFrame = 0;
				break;
			}
		}
	}
}
