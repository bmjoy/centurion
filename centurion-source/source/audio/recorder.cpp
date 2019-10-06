#include "recorder.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <algorithm>
#include <iterator>

Recorder::Recorder()
{
	pBuffer = NULL;
	const char *deviceName = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
	pCaptureDevice = alcCaptureOpenDevice(deviceName, 44100, AL_FORMAT_MONO16, 44100);
	bRecording = false;
}

Recorder::~Recorder()
{
	if (pBuffer)
		delete pBuffer;
}

ALCdevice *Recorder::GetDevice()
{
	return pCaptureDevice;
}

ISoundBuffer *Recorder::GetBuffer()
{
	return pBuffer;
}

ISoundBuffer *Recorder::CopyToBuffer()
{
	recorderMutex.lock();
	if (!samples.empty())
	{
		SoundBuffer* buff = new SoundBuffer();
		buff->SetSampleRate(44100);
		buff->WriteData(&samples[0], samples.size());
		buff->SetSampleCount((unsigned int)samples.size());
		samples.resize(0);
		samples.clear();
		recorderMutex.unlock();
		return buff;
	}
	recorderMutex.unlock();
	return NULL;
}

bool Recorder::Ready()
{
	if (!pCaptureDevice)
		return false;
	return true;
}

void Recorder::Start()
{
	if (pBuffer)
		delete pBuffer;
	if (!samples.empty())
	{
		samples.resize(0);
		samples.clear();
	}
	alcCaptureStart(pCaptureDevice);
	bRecording = true;
	recorderThread = new std::thread(Recorder::ProcessRecording, this);

}

void Recorder::Stop()
{
	bRecording = false;
	alcCaptureStop(pCaptureDevice);
	if (!samples.empty())
	{
		pBuffer = (SoundBuffer*)CopyToBuffer();
	}
}

bool Recorder::IsRecording()
{
	return bRecording;
}


void Recorder::ProcessRecording(Recorder *recorder)
{
	while (recorder->IsRecording())
	{
		
		ALCint samplesAvailable;
		alcGetIntegerv(recorder->GetDevice(), ALC_CAPTURE_SAMPLES, 1, &samplesAvailable);
		if (samplesAvailable > 0)
		{
			recorder->recorderMutex.lock();

			short *samples = new short[samplesAvailable];
			alcCaptureSamples(recorder->GetDevice(), &samples[0], samplesAvailable);
			std::copy(&samples[0], &samples[0] + samplesAvailable, std::back_inserter(recorder->GetSamples()));
			delete[] samples;
			recorder->recorderMutex.unlock();
#ifdef LINUX
			usleep(100 * 1000);
#endif
#ifdef WINDOWS
			Sleep(100);
#endif
			
		}
	}
}