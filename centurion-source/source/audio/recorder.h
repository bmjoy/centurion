/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef H_RECORDER
#define H_RECORDER

#include "interfaces/IRecorder.h"
#include "sound_buffer.h"
#include <thread>
#include <mutex>

/// <summary>
/// This class handles audio recording.
/// </summary>
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

	/// <summary>
	/// This function aquires the audio buffer.
	/// </summary>
	virtual ISoundBuffer *GetBuffer();

	/// <summary>
	/// This function copies recorded audio into aquired buffer.
	/// </summary>
	virtual ISoundBuffer *CopyToBuffer();

	/// <summary>
	/// This function starts to recording an audio.
	/// </summary>
	virtual void Start();

	/// <summary>
	/// This function stops audio recording.
	/// </summary>
	virtual void Stop();

	/// <summary>
	/// This boolean function returns if the main recording audio is currently ready to record.
	/// </summary>
	virtual bool Ready();

	/// <summary>
	/// This boolean function returns if an audio is currently recording.
	/// </summary>
	virtual bool IsRecording();

	/// <summary>
	/// This function evaluates and checks the last recorded audio.
	/// </summary>
	static void ProcessRecording(Recorder *);
	
	ALCdevice *GetDevice();
	std::vector<short> &GetSamples()
	{
		return samples;
	}

};
#endif