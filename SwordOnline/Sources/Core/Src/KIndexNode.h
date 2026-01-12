#ifndef KIndexNodeH
#define	KIndexNodeH

class KIndexNode : public KNode
{
public:
	INT		m_nIndex;
	INT		m_Ref;
public:
	KIndexNode() { m_nIndex = NULL; m_Ref = 0; };
	void	AddRef() { m_Ref++; };
	void	Release() {
		_ASSERT(m_Ref > 0);
		if (m_Ref > 0)
			m_Ref--;
	};
};

#endif
