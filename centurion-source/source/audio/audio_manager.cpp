#include "audio_manager.h"
#pragma warning(disable: 4996)

AudioManager::AudioManager()
{
}

void AudioManager::MusicPlay(std::string name) {
	AudioDevice *device = new AudioDevice();

	if(device){
		music = device->CreateMusic();
		if (!music->LoadFromFile(name.c_str())){
			MessageBox(NULL, "Error code 0x00000002\n\nThe game is unable to find or process SETTLEMENTS file.\nForced application shutdown has started.", "Centurion", MB_ICONERROR);
		}
		else{
			std::cout << "DEBUG: Music file detected. Starting the playing function.\n";
			music->Play();
		}
	}
	else {
		MessageBox(NULL, "Error code 0x00000002\n\nThe game is unable to find or process SETTLEMENTS file.\nForced application shutdown has started.", "Centurion", MB_ICONERROR);
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