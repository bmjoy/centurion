/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "music.h"
#include <string>

/// <summary>
/// This class is used to handle audio in game (SDT and SFX).
/// </summary>
class AudioManager {
public:
	AudioManager();

	#pragma region Music section
	/// <summary>
	/// This function stops the execution of a music.
	/// </summary>
	void MusicStop();

	/// <summary>
	/// This function pauses the current music.
	/// </summary>
	void MusicPause();

	/// <summary>
	/// This function rewinds the current music from its starts.
	/// </summary>
	void MusicRewind();

	/// <summary>
	/// This function activate or deactivate music looping.
	/// </summary>
	/// <param name="loop">Looping value. It supports only booleans.</param>
	void MusicLoop(bool loop);

	/// <summary>
	/// This function plays a specific filename music.
	/// </summary>
	/// <param name="name">Name of the music file. It supports only strings.</param>
	/// <param name="loop">Looping value. It supports only booleans.</param>
	void MusicPlay(std::string name, bool looping = true);

	/// <summary>
	/// This function sets the global volume of music.
	/// </summary>
	/// <param name="value">Global volume value, with a range 0 - 100. Supports only integers.</param>
	void MusicVolume(int value);

	/// <summary>
	/// This boolean function returns if a music is currently playing.
	/// </summary>
	bool IsMusicPlaying();

	/// <summary>
	/// This boolean function returns if a music is currently pausing.
	/// </summary>
	bool IsMusicPaused();

	/// <summary>
	/// This boolean function returns if a music is currently looping.
	/// </summary>
	bool IsMusicLooping();
	#pragma endregion

	#pragma region SFX section
	/// <summary>
	/// This function stops the execution of a sound.
	/// </summary>
	void SoundStop();

	/// <summary>
	/// This function pauses the current sound.
	/// </summary>
	void SoundPause();

	/// <summary>
	/// This function rewinds the current sound from its starts.
	/// </summary>
	void SoundRewind();

	/// <summary>
	/// This function activate or deactivate sound looping.
	/// </summary>
	/// <param name="loop">Looping value. It supports only booleans.</param>
	void SoundLoop(bool loop);

	/// <summary>
	/// This function plays a specific filename sound.
	/// </summary>
	/// <param name="name">Name of the sound file. It supports only strings.</param>
	/// <param name="loop">Looping value. It supports only booleans.</param>
	void SoundPlay(std::string name, bool looping = true);

	/// <summary>
	/// This function sets the global volume of sounds.
	/// </summary>
	/// <param name="value">Global volume value, with a range 0 - 100. Supports only integers.</param>
	void SoundVolume(int value);

	/// <summary>
	/// This boolean function returns if a sound is currently playing.
	/// </summary>
	bool IsSoundPlaying();

	/// <summary>
	/// This boolean function returns if a sound is currently pausing.
	/// </summary>
	bool IsSoundPaused();

	/// <summary>
	/// This boolean function returns if a sound is currently looping.
	/// </summary>
	bool IsSoundLooping();
	#pragma endregion

	~AudioManager();
private:
	IMusic *music;
	ISound *sound;
	AudioDevice *device;
};

static AudioManager audioM;

#endif
