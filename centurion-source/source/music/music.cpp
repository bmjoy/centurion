#include "music.h"

Music::Music() {
}

ISoundEngine* Music::play(const char* path, bool loop){
	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine)
		return NULL;
	engine->play2D(path, loop);
	return engine;
}

Music::~Music() {
}