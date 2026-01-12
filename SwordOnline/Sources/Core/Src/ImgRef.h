#pragma once

extern struct iRepresentShell* g_pRepresent;

void		 IR_UpdateTime();
UINT IR_GetCurrentTime();
void		 IR_NextFrame(INT& nFrame, INT nTotalFrame, UINT UINTerval, UINT& uFlipTime);