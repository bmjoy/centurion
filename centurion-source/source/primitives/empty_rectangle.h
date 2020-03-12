#ifndef EMPTYRECTANGLE_H
#define EMPTYRECTANGLE_H

#include "shader.h"

class EmptyRectangle : public Shader
{
public:
	EmptyRectangle();
	void create();
	void render(RectangleData &data);
	~EmptyRectangle();
private:
	float vertices[24];
};

static EmptyRectangle eRect;

#endif