#pragma  once 

class KPartnerSkill:public ISkill
{
private:
	unsigned long  m_ulNpcTemplateId;

private:
	unsigned long GetPartnerLevel();

public:
	INT				GetSkillId();
	const char *	GetSkillName();
	INT				GetSkillStyle();
	void			LoadSkillLevelData(unsigned long  ulLevel, INT nParam);
	BOOL			Cast(INT nLauncher,  INT nParam1, INT nParam2);
	BOOL			InitSkill();
	static void		GetDesc(unsigned long ulSkillId, unsigned long ulCurLevel, char * pszMsg, INT nOwnerIndex,  bool bNextLevelDesc);
}