#ifndef H_SOUNDBUFFER
#define H_SOUNDBUFFER

#include "sndfile.h"
#include "interfaces/ISoundBuffer.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vector>

class SoundBuffer : public ISoundBuffer
{
	ALuint m_buffer[1];
	unsigned int m_iChannels;
	unsigned int m_iFrames;
	unsigned int m_iSampleRate;
	unsigned int m_iSampleCount;
	unsigned int m_iSoundFormat;
public:
	SoundBuffer();
	~SoundBuffer();
	virtual bool LoadFromFile(const char *szFile);
	virtual unsigned int GetChannels();
	virtual unsigned int GetFrames();
	virtual unsigned int GetSampleRate();
	virtual unsigned int GetSampleCount();
	virtual void SetSampleRate(unsigned int rate);
	virtual void SetChannels(unsigned int channels);
	virtual void SetFrames(unsigned int frames);
	virtual void SetSampleCount(unsigned int count);
	ALuint GetBufferID()
	{
		return m_buffer[0];
	}
	void WriteData(void *data, size_t size);

};

#endif