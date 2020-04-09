/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef GL_LIGHT_H
#define GL_LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

/// <summary>
/// This class is used to generate light for the terrain
/// </summary>
class glLight
{
public:
	/// <summary>
	/// Constructors
	/// </summary>
	glLight();
	glLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,  //ambient
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity); //diffuse

	/// <summary>
	/// This method sends the inputs to the GPU
	/// </summary>
	void use_light(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, //ambient
		GLfloat diffuseIntensityLocation, GLfloat directionLocation); //diffuse

	/// <summary>
	/// Destructor
	/// </summary>
	~glLight();
private:
	// ambient light
	glm::vec3 colour;
	GLfloat ambientIntensity;

	// diffuse light
	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

#endif