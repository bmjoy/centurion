#ifndef H_ISOUNDBUFFER
#define H_ISOUNDBUFFER

class ISoundBuffer
{
public:
	virtual bool LoadFromFile(const char *szFile) = 0;
	virtual unsigned int GetChannels() = 0;
	virtual unsigned int GetFrames() = 0;
	virtual unsigned int GetSampleRate() = 0;
	virtual unsigned int GetSampleCount() = 0;
	virtual void SetSampleRate(unsigned int rate) = 0;
	virtual void SetChannels(unsigned int channels) = 0;
	virtual void SetFrames(unsigned int frames) = 0;
	virtual void SetSampleCount(unsigned int count) = 0;
};

#endif