#ifndef Xact_h_
#define Xact_h_

#include <xact3.h>

struct XactSound
{
	IXACT3WaveBank* WaveBank;
	IXACT3SoundBank* SoundBank;
	void *WaveBankData;
	void *SoundBankData;
};

extern  XactSound XactSoundObject;
extern IXACT3Engine *SoundEngine;
extern bool SetupXACT(char *WaveBank, char *SoundBank);
extern bool LoadWaveBank( char *fileName);
extern bool LoadSoundBank(char *fileName);
extern void CloseAllSounds();
extern void PlaySound(XACTINDEX SoundName,XactSound XactClass);
extern void StopSound(XACTINDEX SoundName,XactSound XactClass);
extern void PauseSound(XACTINDEX SoundName,XactSound XactClass);
extern void PlayBGM(XACTINDEX SoundName,XactSound XactClass);
extern void PauseBGM(XACTINDEX SoundName,XactSound XactClass);
extern void StopBGM(XACTINDEX SoundName,XactSound XactClass);

#endif
