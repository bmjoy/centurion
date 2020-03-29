#include "audio_manager.h"

#include <logger.h>

AudioManager::AudioManager(){}

void AudioManager::MusicPlay(string name, bool looping) {
	device = new AudioDevice();
	if (device){
		music = device->CreateMusic();
		if (!music->LoadFromFile(name.c_str())){
			Logger::Warn("Impossible to find or parse audio file. No music will be played.");
		}
		else{
			Logger::Info("Music file detected. Starting the playing function.");
			music->Play();
			MusicLoop(looping);
		}
	}
	else {
		//showGameWarning("WARNING_audioDevice");
	}
}

void AudioManager::MusicStop() {
	if (music) {
		if (device){
			delete device;
		}
		music->Stop();
		Logger::Info("Music variable is working properly.");
		if (IsMusicPlaying()) {
			Logger::Info("Music is playing!");
			music->Stop();
		}
		else {
			Logger::Info("Music is not playing.");
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
