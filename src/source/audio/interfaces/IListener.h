#ifndef H_ILISTENER
#define H_ILISTENER

class IListener
{
public:
	virtual void SetLocation(float x, float y, float z) = 0;
	virtual void SetOrientation(float *orientation) = 0;
	virtual void SetVelocity(float x, float y, float z) = 0;
};

#endif