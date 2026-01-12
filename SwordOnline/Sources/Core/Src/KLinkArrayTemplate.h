#ifndef KLINKARRAYTEMPLATE_H
#define KLINKARRAYTEMPLATE_H

template <class T, size_t ulSize>
class KLinkArrayTemplate
{
private:
	size_t			m_ulTotalSize;
public:
	size_t			m_ulFreeSize;
	T m_Data[ulSize];
	KLinkArray		m_FreeIdx;							//	可用表

	KLinkArray		m_UseIdx;

	T* Add()
	{
		ULONG ulIndex = 0;
		FindFree(ulIndex);
		if (!ulIndex)
			return NULL;

		m_ulFreeSize--;

		m_FreeIdx.Remove(ulIndex);
		m_UseIdx.Insert(ulIndex);
		return &m_Data[ulIndex];
	}

	ULONG Add(T* pT)
	{
		ULONG ulIndex = 0;
		FindFree(ulIndex);
		if (!ulIndex)
			return 0;
		m_Data[ulIndex] = *pT;
		m_ulFreeSize--;

		m_FreeIdx.Remove(ulIndex);
		m_UseIdx.Insert(ulIndex);
		return ulIndex;
	}

	ULONG Remove(T* pSame)
	{
		ULONG ulSameIndex = FindSame(pSame);
		if (!ulSameIndex)
			return 0;
		return Remove(ulSameIndex);
	}

	ULONG Remove(ULONG ulIndex)
	{
		if (ulIndex > m_ulTotalSize)
			return 0;
		m_UseIdx.Remove(ulIndex);
		m_FreeIdx.Insert(ulIndex);
		m_ulFreeSize++;
		return ulIndex;
	}

	void Clear()
	{
		while (1)
		{
			INT nIdx = m_UseIdx.GetNext(0);
			if (nIdx)
			{
				m_UseIdx.Remove(nIdx);
				m_FreeIdx.Insert(nIdx);
			}
			else
			{
				break;
			}
		}
		m_ulFreeSize = ulSize - 1;
	}

	KLinkArrayTemplate()
	{
		m_ulTotalSize = m_ulFreeSize = ulSize - 1;//由于index0是无效的，所以实际数量要减一个
		m_FreeIdx.Init(ulSize);
		m_UseIdx.Init(ulSize);
		for (INT i = ulSize - 1; i > 0; i--)
		{
			m_FreeIdx.Insert(i);
		}
		Clear();
	}

	T* FindFree(ULONG& ulIndex)
	{
		ulIndex = m_FreeIdx.GetNext(0);
		if (!ulIndex)
			return NULL;
		return &m_Data[ulIndex];
	};

	T* GetData(ULONG ulIndex)
	{
		if (ulIndex > m_ulTotalSize || ulIndex == 0)
			return NULL;
		return &m_Data[ulIndex];
	}

	T* GetData(T* pT)
	{

		ULONG ulIndex = FindSame(pT);
		if (!ulIndex)
			return  NULL;
		return &m_Data[ulIndex];

	}

	ULONG FindSame(ULONG ulKey)
	{

		INT nIdx = 0;
		while (1)
		{
			nIdx = m_UseIdx.GetNext(nIdx);
			if (!nIdx)
				break;

			if (ulKey == m_Data[nIdx].m_dwKey)
				return nIdx;
		}
	}

	ULONG GetFreeCount() const
	{
		return m_ulFreeSize;
	};
	ULONG GetUsedCount() const
	{
		_ASSERT(m_ulTotalSize >= m_ulFreeSize);
		return m_ulTotalSize - m_ulFreeSize;
	};
	ULONG GetTotalCount() const
	{
		return m_ulTotalSize;
	};

	virtual	ULONG	FindSame(T* pSame) = 0;
};

#endif
