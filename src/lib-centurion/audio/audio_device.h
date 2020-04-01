/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef H_AUDIODEVICE
#define H_AUDIODEVICE

#include "interfaces/IAudioDevice.h"
#include "interfaces/IAudioContext.h"
#include "interfaces/IMusic.h"
#include "interfaces/ISound.h"
#include "listener.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

/// <summary>
/// This class is used to communicate with default audio peripheral.
/// </summary>
class AudioDevice : public IAudioDevice
{
	ALCdevice *pDevice;
	IAudioContext *pContext;
	Listener *pListener;
public:
	AudioDevice();
	~AudioDevice();
	virtual bool Ready();
	
	/// <summary>
	/// This function aquires the audio context.
	/// </summary>
	virtual IAudioContext *GetContext();

	/// <summary>
	/// This function is used to create a music class instance.
	/// </summary>
	virtual IMusic *CreateMusic();

	/// <summary>
	/// This function is used to create a sound class instance.
	/// </summary>
	virtual ISound *CreateSound();

	/// <summary>
	/// This function aquires the audio listener.
	/// </summary>
	virtual IListener* GetListener();

	/// <summary>
	/// This function is used to create a sound recorder class instance.
	/// </summary>
	virtual IRecorder *CreateRecorder();

	/// <summary>
	/// This function is used to create a sound buffer class instance.
	/// </summary>
	virtual ISoundBuffer *CreateBuffer();

	/// <summary>
	/// This function returns the audio device reference.
	/// </summary>
	ALCdevice *GetDevice()
	{
		return pDevice;
	}
	
	/// <summary>
	/// This boolean function returns if there is any OpenAL error.
	/// </summary>
	virtual bool IsError();
};

#endif