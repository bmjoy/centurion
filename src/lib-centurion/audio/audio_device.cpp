#include "audio_device.h"
#include "audio_context.h"
#include "music.h"
#include "sound.h"
#include "recorder.h"

AudioDevice::AudioDevice(){
	pDevice = alcOpenDevice(NULL);
	pContext = new AudioContext(pDevice);
	pContext->Ensure();
	pListener = new Listener();
	float listenerOrientation[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	pListener->SetLocation(0, 0, 0);
	pListener->SetOrientation(listenerOrientation);
	pListener->SetVelocity(0, 0, 0);
}

bool AudioDevice::Ready(){
	if (!pDevice || !pContext || !pContext->Ensure())
		return false;
	return true;
}

IMusic *AudioDevice::CreateMusic(){
	return new Music();
}

ISound *AudioDevice::CreateSound(){
	return new Sound();
}

IAudioContext *AudioDevice::GetContext(){
	return pContext;
}

bool AudioDevice::IsError(){
	ALenum error = alGetError();
	if (error != AL_NO_ERROR){
		return true;
	}
	return false;
}

IListener* AudioDevice::GetListener(){
	return pListener;
}

IRecorder *AudioDevice::CreateRecorder(){
	return new Recorder();
}

ISoundBuffer *AudioDevice::CreateBuffer(){
	return new SoundBuffer();
}

AudioDevice::~AudioDevice() {
	delete pListener;
	delete pContext;
	alcCloseDevice(pDevice);
}