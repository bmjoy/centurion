#ifndef H_AUDIOPLAYER
#define H_AUDIOPLAYER

#include <cstdlib>
#include <global.hpp>
#include <iostream>
#include <windows.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "audio_device.h"
#include "sound_buffer.h"
#include "interfaces/IAudioPlayer.h"

class AudioPlayer : virtual public IAudioPlayer
{
	SoundBuffer *pBuffer;
	ALuint m_source[1];
	int playerState;
	bool bLooping;
public:
	AudioPlayer();
	~AudioPlayer();
	virtual void SetBuffer(ISoundBuffer *buffer);
	virtual ISoundBuffer *GetBuffer();
	virtual bool LoadFromFile(const char *szFile);


	virtual void SeekOffset(float offset);
	virtual void SeekSecond(float second);
	virtual float GetDuration();
	virtual float GetPosition();

	virtual void Play();
	virtual void Stop();
	virtual void Pause();
	virtual void Rewind();

	bool IsPlaying();
	virtual bool IsPaused();

	virtual void SetLooping(bool state);
	virtual bool IsLooping();

	virtual void SetPitch(float value);
	virtual void SetVolume(int value);

	ALuint GetSourceID()
	{
		return m_source[0];
	}


	float GetBufferLengthInSeconds();
};

#endif