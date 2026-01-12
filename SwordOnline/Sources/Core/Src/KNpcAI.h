#ifndef	KNpcAIH
#define	KNpcAIH

#include "KMath.h"

class	KNpcAI
{
public:
	INT				m_nIndex;
	BOOL			m_bActivate;
	INT				m_IgnoreCornerLag; // ne' goc lag auto
private:
public:
	KNpcAI();
	void				Activate(INT nIndex);
	void				Enable() { m_bActivate = TRUE; }
	void				Disable() { m_bActivate = FALSE; }
private:
	INT				GetNearestNpc(INT nRelation);
	INT				GetNpcNumber(INT nRelation);
	void				ProcessPlayer();

	/*	void			ProcessAIType1();
		void			ProcessAIType2();
		void			ProcessAIType3();
		void			ProcessAIType4();
		void			ProcessAIType5();
		void			ProcessAIType6();
		void			ProcessAIType7();
		void			ProcessAIType8();
		void			ProcessAIType9();
		void			ProcessAIType10();*/
	void			ProcessAIType01();		// 普通主动类1
	void			ProcessAIType02();		// 普通主动类2
	void			ProcessAIType03();		// 普通主动类3
	void			ProcessAIType04();		// 普通被动类1
	void			ProcessAIType05();		// 普通被动类2
	void			ProcessAIType06();		// 普通被动类3
	void			FollowAttack(INT nIdx);
	BOOL			InEyeshot(INT nIdx);
	void			CommonAction();
	BOOL			KeepActiveRange();
	void			KeepAttackRange(INT nEnemy, INT nRange);
	void			Flee(INT nIdx);
	BOOL			CheckNpc(INT nIndex);
#ifndef _SERVER
	void			FollowPeople(INT nIdx);
	void			FollowObject(INT nIdx);
#else
	void			FindPathNpc();
#endif
	friend class KNpc;
#ifndef _SERVER
	// 装饰性质NPC运动函数系列 
	// flying add these on Jun.4.2003
	// 所有装饰性NPC运动处理入口，由Activate(INT)内部调用
	INT				ProcessShowNpc();
	INT  			ShowNpcType11();
	INT				ShowNpcType12();
	INT				ShowNpcType13();
	INT				ShowNpcType14();
	INT				ShowNpcType15();
	INT				ShowNpcType16();
	INT				ShowNpcType17();
	INT             GetNpcMoveOffset(INT nDir, INT nDistance, INT* pnX, INT* pnY);

	// 判断是否超出范围
	//BOOL KeepActiveShowRange();
	// 16/17 AiMode NPC的逃逸动作
	INT				DoShowFlee(INT nIdx);

	// 判断是否这个帧内可以给该NPC下指令
	BOOL			CanShowNpc();
#endif
	// flying add the function to get nearest player.
	INT			IsPlayerCome();
};

#if !defined _SERVER
inline INT KNpcAI::GetNpcMoveOffset(INT nDir, INT nDistance, INT* pnX, INT* pnY)
{
	_ASSERT(pnX);
	_ASSERT(pnY);

	*pnX = -nDistance * g_DirSin(nDir, 64);
	*pnY = -nDistance * g_DirCos(nDir, 64);

	return true;
}

inline BOOL KNpcAI::CanShowNpc()
{
	BOOL bResult = TRUE;
	if (Npc[m_nIndex].m_AiParam[5] < Npc[m_nIndex].m_AiParam[4])
		bResult = FALSE;
	Npc[m_nIndex].m_AiParam[5]++;
	return bResult;
}
#endif

extern KNpcAI NpcAI;
#endif
