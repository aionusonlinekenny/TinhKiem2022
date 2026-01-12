#ifndef __KMISSLEMAGICATTRIBSDATA_H__
#define __KMISSLEMAGICATTRIBSDATA_H__

class KMissleMagicAttribsData
{
	INT			nRef;
public:
	KMagicAttrib* m_pStateMagicAttribs;
	INT m_nStateMagicAttribsNum;

	KMagicAttrib* m_pDamageMagicAttribs;
	INT m_nDamageMagicAttribsNum;

	KMagicAttrib* m_pImmediateAttribs;
	INT m_nImmediateMagicAttribsNum;
	/*
	#ifdef _DEBUG
	static	INT m_nTotalRef;
	static	INT m_nTotalCount;
	#endif
	*/
private:
	void Remove()
	{
		//	if (m_pStateMagicAttribs != NULL)		delete m_pStateMagicAttribs; 
		if (m_pDamageMagicAttribs != NULL)		delete[]m_pDamageMagicAttribs;
	};

public:

	KMissleMagicAttribsData() {
		nRef = m_nStateMagicAttribsNum = m_nDamageMagicAttribsNum = m_nImmediateMagicAttribsNum = 0;
		m_pImmediateAttribs = m_pStateMagicAttribs = m_pDamageMagicAttribs = NULL;
	};

	INT AddRef() {
		/*
		#ifdef _DEBUG
		++m_nTotalRef;
		++nRef;
		if (nRef == 1)
		{
		//		m_nTotalCount += m_nStateMagicAttribsNum;
		m_nTotalCount += m_nDamageMagicAttribsNum;
		}
		return nRef;
		#else

		  return ++nRef;
		  #endif
			*/
		return ++nRef;

	};

	INT GetRef() { return nRef; };

	INT DelRef()
	{
		/*
		#ifdef _DEBUG
		--m_nTotalRef;
		#endif
			*/
		return --nRef;
	};

	~KMissleMagicAttribsData()
	{
		Remove();
	};

};

#endif //__KMISSLEMAGICATTRIBSDATA_H__
