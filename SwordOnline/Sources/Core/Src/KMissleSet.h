#ifndef KMISSLESET_H
#define KMISSLESET_H
#include "KMissle.h"
#include "KLinkArray.h"

class CORE_API KMissleSet
{
	KLinkArray		m_FreeIdx;				//	可用表
	KLinkArray		m_UseIdx;				//	已用表
public:
	void	Init();
	INT		Activate();
	INT		FindFree();
	INT		Add(INT SubWorldId, INT regionid, INT x, INT y, INT dx = 0, INT dy = 0);
	INT		Add(INT SubWorldId, INT px, INT py);
	INT		CreateMissile(INT nSkillId, INT nMissleId, INT nLauncher, INT nTargetId, INT nSubWorldId, INT nPX, INT nPY, INT nDir);
	void	Remove(INT nIndex);
	void	ClearMissles();
	INT		GetCount();
	void	Draw();
};
extern CORE_API KMissleSet MissleSet;
#endif
