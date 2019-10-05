#include "light.h"

Light::Light(){
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity) {
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

void Light::use_light(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
	//ambient
	glUniform3f((GLint)ambientColorLocation, colour.x , colour.y, colour.z);
	glUniform1f((GLint)ambientIntensityLocation, ambientIntensity);

	//diffuse
	glUniform3f((GLint)directionLocation, direction.x, direction.y, direction.z);
	glUniform1f((GLint)diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light(){}
