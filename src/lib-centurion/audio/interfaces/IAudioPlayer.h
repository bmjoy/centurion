#ifndef H_IAUDIOPLAYER
#define H_IAUDIOPLAYER

#include "ISoundBuffer.h"

class IAudioPlayer
{
public:

	virtual void SetBuffer(ISoundBuffer *buffer) = 0;
	virtual ISoundBuffer *GetBuffer() = 0;
	virtual bool LoadFromFile(const char *szFile) = 0;

	virtual void SeekOffset(float offset) = 0;
	virtual float GetDuration() = 0;
	virtual void SeekSecond(float second) = 0;
	virtual float GetPosition() = 0;

	virtual void Play() = 0;
	virtual void Stop() = 0;
	virtual void Pause() = 0;
	virtual void Rewind() = 0;

	virtual bool IsPlaying() = 0;
	virtual bool IsPaused() = 0;

	virtual void SetLooping(bool state) = 0;
	virtual bool IsLooping() = 0;

	virtual void SetPitch(float value) = 0;
	virtual void SetVolume(int value) = 0;
};
#endif