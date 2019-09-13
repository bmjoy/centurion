#include <iostream>
#include <irrKlang/irrKlang.h>

using namespace irrklang;

class Music {
public:
	Music();
	ISoundEngine* play(const char* path, bool loop);
	void stop(ISoundEngine* engine)	{ engine->stopAllSounds(); }
	void pause(ISoundEngine* engine, bool pause) { engine->setAllSoundsPaused(pause); }
	void destroy(ISoundEngine* engine) { engine->drop(); }
	~Music();
};

