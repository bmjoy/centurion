/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef H_SOUND
#define H_SOUND

#include "audio_player.h"

/// <summary>
/// This class creates an instance of a sound (SFX).
/// </summary>
class Sound : virtual public AudioPlayer, public ISound
{
public:
	Sound();
	~Sound();

	/// <summary>
	/// This function sets the sound location (3D sound environment) to a specified position.
	/// </summary>
	/// <param name="x">Game world X coordinate.</param>
	/// <param name="y">Game world Y coordinate.</param>
	/// <param name="z">Game world Z coordinate.</param>
	virtual void SetLocation(float x, float y, float z);

	/// <summary>
	/// This function sets the sound direction (3D sound environment) relative to the camera.
	/// </summary>
	/// <param name="x">Game world X coordinate.</param>
	/// <param name="y">Game world Y coordinate.</param>
	/// <param name="z">Game world Z coordinate.</param>
	virtual void SetDirection(float x, float y, float z);

	/// <summary>
	/// This function sets the sound velocity (3D sound environment) for a non static location sound.
	/// </summary>
	/// <param name="x">Game world X coordinate.</param>
	/// <param name="y">Game world Y coordinate.</param>
	/// <param name="z">Game world Z coordinate.</param>
	virtual void SetVelocity(float x, float y, float z);
};

#endif