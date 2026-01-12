#ifndef SKILL_H
#define SKILL_H

#include "SkillDef.h"


class ISkill
{
public:
    virtual ~ISkill() {};
	virtual INT				GetSkillId() = 0;
	virtual const char *	GetSkillName() = 0;
	virtual INT				GetSkillStyle() = 0;
	virtual void			LoadSkillLevelData(ULONG  ulLevel, INT nParam) = 0;
	virtual BOOL			CanCastSkill  (INT nLauncher, INT&nParam1, INT&nParam2)  const = 0;
	virtual NPCATTRIB		GetSkillCostType() const= 0;
	virtual INT				GetSkillCost(void *) const = 0;
	virtual BOOL			IsTargetOnly() const = 0;
	virtual BOOL			IsTargetEnemy() const = 0;
	virtual BOOL			IsTargetAlly() const = 0;
	virtual BOOL			IsTargetObj() const = 0;
	virtual BOOL			IsTargetNoNpc() const = 0;
	virtual BOOL			IsTargetSelf() const {return FALSE;};
	virtual BOOL			IsAura() const {return FALSE;	};
	virtual BOOL			IsPhysical() const {return FALSE;};
	virtual BOOL			IsExp() {return FALSE;};
	virtual BOOL			IsBase() {return FALSE;};
	virtual INT				GetAttackRadius() const = 0;
	virtual ULONG	GetSkillLevelUpScriptId() const {return 0;} ;
	virtual	INT				GetDelayPerCast(BOOL bRideHorse) const {return 0;};
	virtual INT 			GetSkillSeries() const {return 0;} ;
#ifndef _SERVER
	virtual void			DrawSkillIcon(INT x, INT y, INT Width, INT Height) = 0;
#endif
};

#endif
