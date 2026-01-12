#ifndef KOptionH
#define	KOptionH

enum
{
	LowPlayer,
	LowNpc,
	LowMissle,
};

enum
{
	ManTypeNameIdx = 94,
	WomanTypeNameIdx = 472,
};

class KOption
{
private:
	INT		m_nSndVolume;
	INT		m_nMusicVolume;
	INT		m_nGamma;
	BOOL	m_bLow[LowMissle + 1];
public:
	INT		GetSndVolume() { return m_nSndVolume; }
	void	SetSndVolume(INT nSndVolume);
	INT		GetMusicVolume() { return m_nMusicVolume; }
	void	SetMusicVolume(INT nMusicVolume);
	INT		GetGamma() { return m_nGamma; };
	void	SetGamma(INT nGamma);
	void	SetLow(INT n, BOOL bLow)
	{
		if (n >= LowPlayer && n <= LowMissle)
			m_bLow[n] = bLow;
	};
	BOOL	GetLow(INT n) { return m_bLow[n]; };
};

extern KOption Option;
#endif