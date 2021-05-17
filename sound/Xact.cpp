#include "Xact.h"

XactSound XactSoundObject;
IXACT3Engine *SoundEngine;

bool SetupXACT(char *waveBank, char *soundBank)
{
	ZeroMemory(&XactSoundObject,sizeof(XactSound));
	if(FAILED(CoInitializeEx(NULL,COINIT_MULTITHREADED)))
	{
		return false;
	}
	if(FAILED(XACT3CreateEngine(0,&SoundEngine)))
	{
		return false;
	}
	if(&SoundEngine == NULL)
	{
		return false;
	}
	XACT_RUNTIME_PARAMETERS xparams = {0};
	xparams.lookAheadTime = 250;
	if(FAILED(SoundEngine->Initialize(&xparams)))
	{
		return false;
	}
	if(!LoadWaveBank(waveBank))
	{
		return false;
	}
	if(!LoadSoundBank(soundBank))
	{
		return false;
	}
	return true;

}

bool LoadWaveBank(char *fileName)
{
	HANDLE file = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(file == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	DWORD fileSize = GetFileSize(file,NULL);
	if(fileSize == -1)
	{
		CloseHandle(file);
		return false;
	}
	HANDLE mapFile = CreateFileMapping(file,NULL,PAGE_READONLY,0,fileSize,NULL);
	if(!mapFile)
	{
		CloseHandle(file);
		return false;
	}
	void *ptr = MapViewOfFile(mapFile,FILE_MAP_READ,0,0,0);
	if(!ptr)
	{
		CloseHandle(mapFile);
		CloseHandle(file);
		return false;
	}
	XactSoundObject.WaveBankData = ptr;
	if(FAILED(SoundEngine->CreateInMemoryWaveBank(XactSoundObject.WaveBankData,fileSize,0,0,&XactSoundObject.WaveBank)))
	{
		CloseHandle(mapFile);
		CloseHandle(file);
		return false;
	}
	CloseHandle(mapFile);
	CloseHandle(file);
	return true;
}
bool LoadSoundBank(char *fileName)
{
	HANDLE file = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(file == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	DWORD fileSize = GetFileSize(file,NULL);
	if(fileSize == -1)
	{
		CloseHandle(file);
		return false;
	}
	XactSoundObject.SoundBankData = new BYTE[fileSize];
	DWORD bytesRead;
	if(ReadFile(file,XactSoundObject.SoundBankData,fileSize,&bytesRead,NULL) != 0 )
	{
		SoundEngine->CreateSoundBank(XactSoundObject.SoundBankData,fileSize,0,0,&XactSoundObject.SoundBank);
	}
	CloseHandle(file);
	return true;
}
void CloseAllSounds()
{
	if(SoundEngine)
	{
		SoundEngine->ShutDown();
		SoundEngine->Release();
	}
	if(XactSoundObject.SoundBankData)
	{
		delete [] XactSoundObject.SoundBankData;
	}
	if(XactSoundObject.WaveBankData)
	{
		UnmapViewOfFile(XactSoundObject.WaveBankData);
		CoUninitialize();
	}
}


void PlaySound(XACTINDEX SoundName,XactSound XactClass)
{
	/*if(SoundName == 0)
{*/
	XactClass.SoundBank->Play(SoundName,0,0,NULL);
//}
//else
//{
//	MessageBox(NULL,"Cannot play sound","Error",MB_OK);
//	//Error handling
//	
//}
}

void StopSound(XACTINDEX SoundName,XactSound XactClass)
{
	/*if(SoundName > 0)
	{*/
		XactClass.SoundBank->Stop(SoundName,0);
	//}
	//else
	//{
	//	//MessageBox(NULL,"Cannot Stop sound","Error",MB_OK);
	//}
}

//Save a pointer to the location in the music where you want to pick back up on.

//void PauseSound(XACTINDEX SoundName,XactSoundClass XactClass)
//{
//	if(SoundName > 0)
//	{
//		XactClass.ProjectSoundBank-
//	}
//}
//void PauseBGM(XACTINDEX SoundName,XactSoundClass XactClass)
//{
//
//}
void PlayBGM(XACTINDEX SoundName,XactSound XactClass)
{
		XactClass.SoundBank->Play(SoundName,::XACT_FLAG_BACKGROUND_MUSIC,0,NULL);
}

void StopBGM(XACTINDEX SoundName,XactSound XactClass)
{
	XactClass.SoundBank->Stop(SoundName,0);
}


