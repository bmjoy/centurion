#include <global>

using namespace glb;

AudioManager::AudioManager(){}

void AudioManager::MusicPlay(std::string name, bool looping) {
	device = new AudioDevice();
	if (device){
		music = device->CreateMusic();
		if (!music->LoadFromFile(name.c_str())){
			std::cout << "[DEBUG] Impossible to find or parse audio file. No music will be played.\n";
		}
		else{
			std::cout << "[DEBUG] Music file detected. Starting the playing function.\n";
			music->Play();
			MusicLoop(looping);
		}
	}
	else {
		showGameWarning("WARNING_audioDevice");
	}
}

void AudioManager::MusicStop() {
	if (music) {
		if (device){
			delete device;
		}
		music->Stop();
		std::cout << "[DEBUG] Music variable is working properly.\n";
		if (IsMusicPlaying()) {
			std::cout << "[DEBUG] Music is playing!\n";
			music->Stop();
		}
		else {
			std::cout << "[DEBUG] Music is not playing!\n";
		}
	}
}

void AudioManager::MusicPause() {
	if (music) {
		music->Pause();
	}
}

void AudioManager::MusicRewind() {
	if (music) {
		music->Rewind();
	}
}

void AudioManager::MusicLoop(bool loop) {
	music->SetLooping(loop);
}

void AudioManager::MusicVolume(int value) {
	music->SetVolume(value);
}

void AudioManager::Playlist() {

}

bool AudioManager::IsMusicPlaying() {
	return music->IsPlaying();
}

bool AudioManager::IsMusicLooping() {
	if (music) {
		return music->IsLooping();
	}
	else{
		return false;
	}
}

AudioManager::~AudioManager(){
}
