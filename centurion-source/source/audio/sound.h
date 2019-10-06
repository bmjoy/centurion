#ifndef H_SOUND
#define H_SOUND

#include "audio_player.h"

class Sound : virtual public AudioPlayer, public ISound
{
public:
	Sound();
	~Sound();

	virtual void SetLocation(float x, float y, float z);
	virtual void SetDirection(float x, float y, float z);
	virtual void SetVelocity(float x, float y, float z);
};

#endif