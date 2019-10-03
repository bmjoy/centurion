#include "audio_context.h"
#include <stdio.h>

AudioContext::AudioContext(ALCdevice* device){
	pContext = alcCreateContext(device, NULL);
}

bool AudioContext::Ensure(){
	if (!pContext)
		return false;
	return alcMakeContextCurrent(pContext);
}

AudioContext::~AudioContext()
{
	alcDestroyContext(pContext);
}