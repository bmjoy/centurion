#ifndef H_IRECORDER
#define H_IRECORDER

#include "ISoundBuffer.h"

class IRecorder
{
public:
	virtual ISoundBuffer *GetBuffer() = 0;
	virtual ISoundBuffer *CopyToBuffer() = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual bool Ready() = 0;
	virtual bool IsRecording() = 0;
};

#endif