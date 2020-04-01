/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef H_LISTENER
#define H_LISTENER

#include "interfaces/IListener.h"

/// <summary>
/// This class handles the sound and music listener in a 3D sound environment. It should to coincide with main camera.
/// </summary>
class Listener : public IListener
{
public:
	Listener();
	~Listener();

	/// <summary>
	/// This function sets the listener location (3D sound environment) to a specified position.
	/// </summary>
	/// <param name="x">Game world X coordinate.</param>
	/// <param name="y">Game world Y coordinate.</param>
	/// <param name="z">Game world Z coordinate.</param>
	virtual void SetLocation(float x, float y, float z);

	/// <summary>
	/// This function sets the listener orientation (3D sound environment) relative to the game world.
	/// </summary>
	/// <param name="*orientation">An array of six elements used to turn x, y and z against their relative 2D-axis.
	/// E.g. float listenerOrientation[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };</param>
	virtual void SetOrientation(float *orientation);

	/// <summary>
	/// This function sets the listener velocity (3D sound environment).
	/// </summary>
	/// <param name="x">Game world X coordinate.</param>
	/// <param name="y">Game world Y coordinate.</param>
	/// <param name="z">Game world Z coordinate.</param>
	virtual void SetVelocity(float x, float y, float z);
};
#endif