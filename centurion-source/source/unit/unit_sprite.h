#pragma once

#include <global.hpp>
#include <json.hpp>
#include <shader.h>

class USprite : public Shader
{
public:
	USprite(); 
	USprite(int shaderID);
	void create(json eData, glm::vec3 playerColor);
	void render(glm::mat4 modelMat, std::string state);
	void set_frame(int f, int mf);
	void set_direction(int d);
	//void set_znoise(float z);
	~USprite();

private:
	int r, g, b;
	float w, h;
	std::string state;
	std::map<std::string, int> textureIdMap;
	json entityData;
	glm::vec3 player_color;
};

