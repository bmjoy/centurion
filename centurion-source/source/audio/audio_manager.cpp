#include "audio_manager.h"

AudioManager::AudioManager()
{
}

void AudioManager::MusicPlay(std::string name) {
	AudioDevice *device = new AudioDevice();

	if(device){
		music = device->CreateMusic();
		if (!music->LoadFromFile(name.c_str())){
			std::cout << "DEBUG: Impossible to find or parse audio file. No music will be played.\n";
		}
		else{
			std::cout << "DEBUG: Music file detected. Starting the playing function.\n";
			music->Play();
		}
	}
	else {
		showGameWarning("Error code 0x00000002\n\nUnable to find (or communicate with) the audio device.\nNo sound will be played as long as the error persists.");
	}
}

void AudioManager::MusicStop() {
	if(music){
		music->Stop();
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

void AudioManager::MusicLoop(bool loop=false) {
	music->SetLooping(loop);
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

AudioManager::~AudioManager()
{
}