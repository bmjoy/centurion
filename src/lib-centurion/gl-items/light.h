/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "header_primitives.h"

/// <summary>
/// This class is used to generate light for the terrain
/// </summary>
class Light
{
public:
	/// <summary>
	/// Constructors
	/// </summary>
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,  //ambient
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity); //diffuse

	/// <summary>
	/// This method sends the inputs to the GPU
	/// </summary>
	void use_light(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, //ambient
		GLfloat diffuseIntensityLocation, GLfloat directionLocation); //diffuse

	/// <summary>
	/// Destructor
	/// </summary>
	~Light();
private:
	// ambient light
	glm::vec3 colour;
	GLfloat ambientIntensity;

	// diffuse light
	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

#endif