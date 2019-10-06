#ifndef H_AUDIOCONTEXT
#define H_AUIDOCONTEXT

#include "interfaces/IAudioContext.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

class AudioContext : public IAudioContext
{
	ALCcontext *pContext;
public:
	AudioContext(ALCdevice* device);
	~AudioContext();
	virtual bool Ensure();
};

#endif