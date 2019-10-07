#pragma once

#include <global.hpp>
#include <mapgen>
#include <objects>

class Mouse
{
public:
	Mouse();
	void render();
	void mouse_control(int lastX, int lastY);
	~Mouse();

private:
	float znoise;
	float yzoomed;

	glm::vec3 position;
	std::string currentState;	
};

