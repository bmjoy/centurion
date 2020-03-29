#include "audio_manager.h"

#include <logger.h>

AudioManager::AudioManager(){}

#pragma region Music section
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
			Logger::Info("No music is playing.");
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
	if (value > 100) {
		music->SetVolume(100);
	}
	if (value < 0) {
		music->SetVolume(0);
	}
	else {
		music->SetVolume(value);
	}
}

bool AudioManager::IsMusicPlaying() {
	return music->IsPlaying();
}

bool AudioManager::IsMusicPaused() {
	return music->IsPaused();
}

bool AudioManager::IsMusicLooping() {
	if (music) {
		return music->IsLooping();
	}
	else{
		return false;
	}
}
#pragma endregion

#pragma region SFX section
void AudioManager::SoundPlay(string name, bool looping) {
	device = new AudioDevice();
	if (device) {
		sound = device->CreateSound();
		if (!sound->LoadFromFile(name.c_str())) {
			Logger::Warn("Impossible to find or parse audio file. No sound will be played.");
		}
		else {
			Logger::Info("Sound file detected. Starting the playing function.");
			sound->Play();
			SoundLoop(looping);
		}
	}
	else {
	}
}

void AudioManager::SoundStop() {
	if (sound) {
		if (device) {
			delete device;
		}
		sound->Stop();
		Logger::Info("Sound variable is working properly.");
		if (IsSoundPlaying()) {
			Logger::Info("Music is playing!");
			sound->Stop();
		}
		else {
			Logger::Info("No sound is playing.");
		}
	}
}

void AudioManager::SoundPause() {
	if (sound) {
		sound->Pause();
	}
}

void AudioManager::SoundRewind() {
	if (sound) {
		sound->Rewind();
	}
}

void AudioManager::SoundLoop(bool loop) {
	sound->SetLooping(loop);
}

void AudioManager::SoundVolume(int value) {
	if (value > 100) {
		sound->SetVolume(100);
	}
	if (value < 0) {
		sound->SetVolume(0);
	}
	else {
		sound->SetVolume(value);
	}
}

bool AudioManager::IsSoundPlaying() {
	return sound->IsPlaying();
}

bool AudioManager::IsSoundPaused() {
	return sound->IsPaused();
}

bool AudioManager::IsSoundLooping() {
	if (sound) {
		return sound->IsLooping();
	}
	else {
		return false;
	}
}
#pragma endregion

AudioManager::~AudioManager(){
}
