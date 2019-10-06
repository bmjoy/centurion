#ifndef H_ISOUND
#define H_ISOUND

#include "IAudioPlayer.h"

class ISound : virtual public IAudioPlayer
{
public:
	virtual void SetLocation(float x, float y, float z) = 0;
	virtual void SetDirection(float x, float y, float z) = 0;
	virtual void SetVelocity(float x, float y, float z) = 0;
};

#endif