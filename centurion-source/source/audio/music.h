#ifndef H_MUSIC
#define H_MUSIC

#include "audio_player.h"
#include "interfaces/IMusic.h"

class Music : virtual public AudioPlayer, public IMusic
{

public:
	Music();
	~Music();
};

#endif