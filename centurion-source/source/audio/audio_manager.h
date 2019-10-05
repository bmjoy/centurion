#include "music.h"

class AudioManager
{

public:
	AudioManager();
	void MusicStop();
	void MusicPause();
	void MusicRewind();
	void MusicLoop(bool loop);
	void MusicPlay(std::string name, bool looping = true);
	void MusicVolume(int value);
	void Playlist();
	bool IsMusicPlaying();
	bool IsMusicLooping();
	~AudioManager();

private:
	IMusic *music;
	AudioDevice *device;
};