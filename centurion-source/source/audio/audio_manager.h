#include "music.h"
#include "objects.h"

class AudioManager
{

public:
	AudioManager();
	void MusicStop();
	void MusicPause();
	void MusicRewind();
	void MusicLoop(bool loop);
	void MusicPlay(std::string name);
	bool IsMusicPlaying();
	bool IsMusicLooping();
	~AudioManager();

private:
	IMusic *music;
};