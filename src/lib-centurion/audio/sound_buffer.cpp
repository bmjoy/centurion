#include "sound_buffer.h"
#include "audio_device.h"
#include <stdio.h>

SoundBuffer::SoundBuffer(){
	alGenBuffers(1, m_buffer);
	m_iChannels = 0;
	m_iSampleRate = 0;
	m_iFrames = 0;
	m_iSampleCount = 0;
}

bool SoundBuffer::LoadFromFile(const char *szFile){
	SF_INFO soundInfo;
	soundInfo.format = 0;
	SNDFILE * sound = sf_open(szFile, SFM_READ, &soundInfo);
	if (!sound){
		return false;
	}
	SetChannels(soundInfo.channels);
	SetSampleRate(soundInfo.samplerate);
	SetFrames((unsigned int)soundInfo.frames);
	SetSampleCount((unsigned int)static_cast<std::size_t>(soundInfo.frames) * soundInfo.channels);
	short *samples = new short[m_iSampleCount + 1];
	int readBytes = (int)sf_read_short(sound, &samples[0], m_iSampleCount);
	if (readBytes == 0){
		sf_close(sound);
		return false;
	}
	WriteData(&samples[0], m_iSampleCount);
	delete[] samples;
	sf_close(sound);
	return true;
}

unsigned int SoundBuffer::GetChannels(){
	return m_iChannels;
}

unsigned int SoundBuffer::GetFrames(){
	return m_iFrames;
}

unsigned int SoundBuffer::GetSampleRate(){
	return m_iSampleRate;
}

unsigned int SoundBuffer::GetSampleCount(){
	return m_iSampleCount;
}

void SoundBuffer::WriteData(void *data, size_t size){
	alBufferData(m_buffer[0], m_iSoundFormat, data, (ALsizei)size * sizeof(short), m_iSampleRate);
}

void SoundBuffer::SetSampleRate(unsigned int rate){
	m_iSampleRate = rate;
}

void SoundBuffer::SetChannels(unsigned int channels){
	m_iChannels = channels;
	if (m_iChannels == 1)
		m_iSoundFormat = AL_FORMAT_MONO16;
	if (m_iChannels == 2)
		m_iSoundFormat = AL_FORMAT_STEREO16;
}

void SoundBuffer::SetFrames(unsigned int frames){
	m_iFrames = frames;
}

void SoundBuffer::SetSampleCount(unsigned int count){
	m_iSampleCount = count;
}

SoundBuffer::~SoundBuffer() {
	alDeleteBuffers(1, m_buffer);
}