#ifndef EMPTYCIRCLE_H
#define EMPTYCIRCLE_H

#include "shader.h"

class EmptyCircle : public Shader
{
public:
	EmptyCircle();
	void create();
	void render(CircleData &data);
	~EmptyCircle();
};

static EmptyCircle eCircle;

#endif