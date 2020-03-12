#ifndef LIGHT_H
#define LIGHT_H

#include "header_primitives.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,  //ambient
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity); //diffuse

	void use_light(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,  //ambient
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);    //diffuse
	~Light();
private:
	// ambient light
	vec3 colour;
	GLfloat ambientIntensity;

	// diffuse light
	vec3 direction;
	GLfloat diffuseIntensity;
};

#endif