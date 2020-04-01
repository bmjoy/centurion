#include "audio_player.h"
#include <logger.h>

AudioPlayer::AudioPlayer(){
	pBuffer = NULL;
	bLooping = false;
	alGenSources(1, m_source);
	alSource3f(m_source[0], AL_POSITION, 0, 0, 0);
	alSource3f(m_source[0], AL_VELOCITY, 0, 0, 0);
	alSource3f(m_source[0], AL_DIRECTION, 0, 0, 0);
	this->SetVolume(100);
	this->SetPitch(1);
}

bool AudioPlayer::LoadFromFile(const char *szFile){
	if (pBuffer)
		delete pBuffer;
	SoundBuffer *Buffer = new SoundBuffer();
	bool result = Buffer->LoadFromFile(szFile);
	this->SetBuffer(Buffer);
	return result;
}

AudioPlayer::~AudioPlayer(){
	if (pBuffer)
		delete pBuffer;
	alDeleteSources(1, m_source);
}

void AudioPlayer::SetBuffer(ISoundBuffer *buffer){
	pBuffer = (SoundBuffer*)buffer;
	alSourcei(m_source[0], AL_BUFFER, pBuffer->GetBufferID());
}

ISoundBuffer *AudioPlayer::GetBuffer(){
	return (ISoundBuffer*)pBuffer;
}

void AudioPlayer::SeekOffset(float offset){
	if (offset < 0 || offset > 1)
		return;

	ALint tot = 0;
	alGetBufferi(pBuffer->GetBufferID(), AL_SIZE, &tot);
	alSourcei(m_source[0], AL_BYTE_OFFSET, (ALint)offset * tot);

}

void AudioPlayer::SeekSecond(float second){
	SeekOffset(second / GetBufferLengthInSeconds());
}

float AudioPlayer::GetBufferLengthInSeconds(){
	ALint size, bits, freq;

	alGetBufferi(pBuffer->GetBufferID(), AL_SIZE, &size);
	alGetBufferi(pBuffer->GetBufferID(), AL_BITS, &bits);
	alGetBufferi(pBuffer->GetBufferID(), AL_FREQUENCY, &freq);
	if (alGetError() != AL_NO_ERROR)
		return -1.0f;
	else
		return (ALfloat)((ALuint)size / pBuffer->GetChannels() / (bits / 8)) / (ALfloat)freq;
}

float AudioPlayer::GetDuration(){
	return static_cast<float>(pBuffer->GetSampleCount()) / pBuffer->GetSampleRate() / pBuffer->GetChannels();
}

float AudioPlayer::GetPosition(){
	ALint offset = 0;
	alGetSourcei(m_source[0], AL_BYTE_OFFSET, &offset);
	ALint tot = 0;
	alGetBufferi(pBuffer->GetBufferID(), AL_SIZE, &tot);
	return (GetBufferLengthInSeconds() * offset) / tot;
}

void AudioPlayer::Play(){
	alSourcePlay(m_source[0]);
}

void AudioPlayer::Rewind(){
	alSourceRewind(m_source[0]);
}

void AudioPlayer::Stop(){
	alSourceStop(m_source[0]);
	Logger::Info("Music stopped!");
}

void AudioPlayer::Pause(){
	alSourcePause(m_source[0]);
}

bool AudioPlayer::IsPlaying(){
	int val = 0;
	alGetSourcei(m_source[0], AL_SOURCE_STATE, &val);
	if (val == AL_PLAYING)
		return true;
	return false;
}

bool AudioPlayer::IsPaused(){
	int val = 0;
	alGetSourcei(m_source[0], AL_SOURCE_STATE, &val);
	if (val == AL_PAUSED)
		return true;
	return false;
}


void AudioPlayer::SetLooping(bool state){
	alSourcei(m_source[0], AL_LOOPING, state);
	bLooping = state;
}

bool AudioPlayer::IsLooping(){
	return bLooping;
}

void AudioPlayer::SetPitch(float value){
	alSourcef(m_source[0], AL_PITCH, value);
}

void AudioPlayer::SetVolume(int value){
	alSourcef(m_source[0], AL_GAIN, value/100.f);
}