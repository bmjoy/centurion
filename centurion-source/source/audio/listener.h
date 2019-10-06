#ifndef H_LISTENER
#define H_LISTENER

#include "interfaces/IListener.h"

class Listener : public IListener
{
public:
	Listener();
	~Listener();
	virtual void SetLocation(float x, float y, float z);
	virtual void SetOrientation(float *orientation);
	virtual void SetVelocity(float x, float y, float z);
};
#endif