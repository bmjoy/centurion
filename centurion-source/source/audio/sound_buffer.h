/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef H_SOUNDBUFFER
#define H_SOUNDBUFFER

#include "sndfile.h"
#include "interfaces/ISoundBuffer.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vector>

/// <summary>
/// This class is used to create and handle sound buffers.
/// </summary>
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

	/// <summary>
	/// This function loads a music file into a precise sound buffer.
	/// </summary>
	/// <param name="*szFile">Sound file. It supports only referenced constant char variables.</param>
	virtual bool LoadFromFile(const char *szFile);

	/// <summary>
	/// This function returns the number of channels of a sound (1 = mono; 2 = stereo).
	/// </summary>
	virtual unsigned int GetChannels();
	virtual unsigned int GetFrames();

	/// <summary>
	/// This function returns the sample rate of a sound (value expressed in Hz).
	/// </summary>
	virtual unsigned int GetSampleRate();
	virtual unsigned int GetSampleCount();

	/// <summary>
	/// This function sets the sound Sample Rate (usually 44100 Hz).
	/// </summary>
	/// <param name="rate">Sample rate value. It supports only non-negative integer values.</param>
	virtual void SetSampleRate(unsigned int rate);

	/// <summary>
	/// This function sets the number of channels of a sound (1 = mono; 2 = stereo).
	/// </summary>
	/// <param name="channels">Number of channels. It supports only non-negative integer values.</param>
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