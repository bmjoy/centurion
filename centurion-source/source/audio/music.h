/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef H_MUSIC
#define H_MUSIC

#include "audio_player.h"
#include "interfaces/IMusic.h"

/// <summary>
/// This class is used to separate SFXs from OSTs (music handling).
/// </summary>
class Music : virtual public AudioPlayer, public IMusic
{

public:
	Music();
	~Music();
};

#endif