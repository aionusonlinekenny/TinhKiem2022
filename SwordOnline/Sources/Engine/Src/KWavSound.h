//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KWavSound.h
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KWavSound_H
#define KWavSound_H
//---------------------------------------------------------------------------
#define BUFFER_COUNT		3	// sound buffer count
//---------------------------------------------------------------------------
class ENGINE_API KWavSound
{
private:
	LPDIRECTSOUNDBUFFER 	m_Buffer[BUFFER_COUNT];
	int32_t						GetFreeBuffer();
	int32_t						GetPlayBuffer();
public:
	KWavSound();
	~KWavSound();
	BOOL					Load(LPSTR FileName);
	void					Free();
	void					Play(int32_t nPan, int32_t nVol, BOOL bLoop);
	void					Stop();
	void					SetPan(int32_t nPan);
	void					SetVolume(int32_t nVolume);
	BOOL					IsPlaying();
};
//---------------------------------------------------------------------------
#endif