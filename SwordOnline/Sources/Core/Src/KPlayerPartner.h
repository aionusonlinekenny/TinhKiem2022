#pragma  once
#ifdef _PARTNER
enum ePartnerAction
{
	partner_action_attacksomeone,
	partner_action_assistsomeone,
	partner_action_killself,
	partner_action_followsomeone,
	partner_action_gotowhere,
	partner_action_autoai,
	partner_action_setcommonai,
};

enum ePartnerNotify
{
	partner_notify_havekillsomeone,
	partner_notify_ownerdead,
	partner_notify_
};
struct TPartnerParam
{
	INT nParam1;
	INT nParam2;
	INT nParam3;
};

typedef struct 
{
	
}TMpsPos;

typedef struct 
{
	
	
}TMapPos;

enum ePartnerType
{
	//召唤类
	partnercount,
};

typedef struct 
{
	unsigned long m_ulTimeOutVanish;//是否有存活时间，如果时间到了就消亡.0表示没有， 大于0表示存活时间
	BOOL m_bDeathVanish;//是否死亡之后就消亡.
	BOOL m_bDeathRevive;//是否死亡后能重生
	unsigned long m_ulOwnerNpcIndex;
}
tPartnerProp;

struct TPartnerParam
{
	INT nNpcIndex;
	ePartnerType eType;
};

class KPlayerPartner
{
private://成员变量
	ePartnerType m_ePartnerType;
	unsigned long m_ulNpcIndex;
	unsigned long m_ulCurLifeTime;
	tPartnerProp m_Prop;

private://成员函数
	void SetType(ePartnerType eType)
	{
		m_ePartnerType = eType;
	};
	void SetNpcIndex(unsigned long ulNpcIndex)
	{
		m_ulNpcIndex = ulNpcIndex;
	};
	KNpc * GetNpc() const
	{
		if (m_ulNpcIndex != 0)
		{
			if (Npc[m_ulNpcIndex].m_Index > 0)
				return &Npc[m_ulNpcIndex];
		}
		return NULL;
	}
	ePartnerType GetType() const 
	{
		return m_ePartnerType;
	};
	
	INT SendCommand(ePartnerType eType, TPartnerParam *pParam);
	BOOL InitData();
//---------------------------------------------------------------	
public://成员变量
	

public://成员函数
	KPlayerPartner();
	BOOL Create();
	
	BOOL Init(INT nNpcIndex, tPartnerProp Prop);
	BOOL Init(INT nNpcTemplateId, INT nLevel, INT nSubWorld, INT nRegion, INT nMapX, INT nMapY, INT nOffX, INT nOffY , tPartnerProp Prop);
	BOOL Init(INT nNpcTemplateId, INT nLevel, INT nSubWorld, INT nMpsX, INT nMpsY, tPartnerProp Prop);
	
	INT	 Activate();
};

class KPartner
{
public:
	virtual INT		SendCommand(ePartnerType eType, TPartnerParam *pParam);
	virtual	INT		SendNotify();
	unsigned long GetIndex();
	BOOL	Create();

};

/*

class KNpcPartnerControl 
{
	
};

class 
{
	virtual void * GetData();
	virtual void * ChangeData();
};
*/
#endif