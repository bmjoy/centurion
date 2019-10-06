#ifndef H_RECORDER
#define H_RECORDER

#include "interfaces/IRecorder.h"
#include "sound_buffer.h"
#include <thread>
#include <mutex>

class Recorder : public IRecorder
{
	SoundBuffer *pBuffer;
	ALCdevice *pCaptureDevice;
	bool bRecording;
	std::vector<short> samples;
	std::thread *recorderThread;
	
public:
	std::mutex recorderMutex;
	Recorder();
	~Recorder();
	virtual ISoundBuffer *GetBuffer();
	virtual ISoundBuffer *CopyToBuffer();
	virtual void Start();
	virtual void Stop();
	virtual bool Ready();
	virtual bool IsRecording();

	static void ProcessRecording(Recorder *);
	
	ALCdevice *GetDevice();
	std::vector<short> &GetSamples()
	{
		return samples;
	}

};
#endif