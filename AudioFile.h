#pragma once

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")


#define TAUDIOFILE_NOSEEK (unsigned long)-1
#define TAUDIOFILE_DATASTART (unsigned long)-2

class CTIAudioFile
{
public:
	CTIAudioFile(){};
	virtual ~CTIAudioFile(){};
	virtual bool Open(const char* filename)=0;
	virtual bool Read(const unsigned long start,void* buffer,unsigned long &readbyte)=0;
	virtual bool Close()=0;
	virtual CTIAudioFile* Clone()=0;
	virtual const char* GetAudioName(){return m_filename;};
	virtual const WAVEFORMATEX* GetFormat()=0;
	virtual const unsigned long GetPCMDataSize()=0;
	virtual const unsigned long GetLength()=0;
protected:
	char* m_filename;
};
