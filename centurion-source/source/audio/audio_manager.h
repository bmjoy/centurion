#include "music.h"

class AudioManager
{

public:
	AudioManager();
	void MusicStop();
	void MusicPause();
	void MusicRewind();
	void MusicLoop(bool loop);
	void MusicPlay(std::string name);
	void MusicVolume(int value);
	void Playlist();
	bool IsMusicPlaying();
	bool IsMusicLooping();
	~AudioManager();

private:
	IMusic *music;
};