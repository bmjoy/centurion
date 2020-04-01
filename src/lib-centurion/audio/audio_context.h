/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef H_AUDIOCONTEXT
#define H_AUIDOCONTEXT

#include "interfaces/IAudioContext.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

/// <summary>
/// This class creates an audio context (required to combine and to make listener, periphericals and all sounds communicate with each other).
/// </summary>
class AudioContext : public IAudioContext
{
	ALCcontext *pContext;
public:
	AudioContext(ALCdevice* device);
	~AudioContext();

	/// <summary>
	/// This boolean function ensures that the audio context is properly initialized and ready to use.
	/// </summary>
	virtual bool Ensure();
};

#endif