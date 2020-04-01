#ifndef H_IAUDIODEVICE
#define H_IAUDIODEVICE

#include "IMusic.h"
#include "ISound.h"
#include "IAudioContext.h"
#include "IListener.h"
#include "IRecorder.h"

class IAudioDevice
{
public:
	virtual IAudioContext *GetContext() = 0;
	virtual IMusic *CreateMusic() = 0;
	virtual ISound *CreateSound() = 0;
	virtual IListener* GetListener() = 0;
	virtual IRecorder *CreateRecorder() = 0;
	virtual ISoundBuffer *CreateBuffer() = 0;

	virtual bool Ready() = 0;
	virtual bool IsError() = 0;
};

#endif