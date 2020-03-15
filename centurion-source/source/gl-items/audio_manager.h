#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "../audio/music.h"
#include "header_primitives.h"

class AudioManager {
public:
	AudioManager();
	void MusicStop();
	void MusicPause();
	void MusicRewind();
	void MusicLoop(bool loop);
	void MusicPlay(string name, bool looping = true);
	void MusicVolume(int value);
	void Playlist();
	bool IsMusicPlaying();
	bool IsMusicLooping();
	~AudioManager();
private:
	IMusic *music;
	AudioDevice *device;
};

static AudioManager audioM;

#endif
