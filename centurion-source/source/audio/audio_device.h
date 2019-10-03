#ifndef H_AUDIODEVICE
#define H_AUDIODEVICE

#include "interfaces/IAudioDevice.h"
#include "interfaces/IAudioContext.h"
#include "interfaces/IMusic.h"
#include "interfaces/ISound.h"
#include "listener.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

class AudioDevice : public IAudioDevice
{
	ALCdevice *pDevice;
	IAudioContext *pContext;
	Listener *pListener;
public:
	AudioDevice();
	~AudioDevice();
	virtual bool Ready();
	
	virtual IAudioContext *GetContext();
	virtual IMusic *CreateMusic();
	virtual ISound *CreateSound();
	virtual IListener* GetListener();
	virtual IRecorder *CreateRecorder();
	virtual ISoundBuffer *CreateBuffer();
	ALCdevice *GetDevice()
	{
		return pDevice;
	}
	
	virtual bool IsError();
};

#endif