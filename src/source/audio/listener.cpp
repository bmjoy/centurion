#include "listener.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

Listener::Listener()
{
}

void Listener::SetLocation(float x, float y, float z){
	alListener3f(AL_POSITION, x, y, z);

}

void Listener::SetOrientation(float *orientation){
	alListenerfv(AL_ORIENTATION, orientation);
}

void Listener::SetVelocity(float x, float y, float z){
	alListener3f(AL_VELOCITY, x, y, z);
}

Listener::~Listener()
{
}