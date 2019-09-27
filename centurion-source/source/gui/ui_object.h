#pragma once

#include <global.hpp>

class UIObject
{
public:
	UIObject();
	void set_position(glm::vec2 pos);
	void set_img_path(std::string path);
	void set_text(std::string txt);
	void set_id(int pickingId);
	void set_color(glm::vec4 col);
	int picking_id;
	~UIObject();

protected:
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 pickingColor;
	std::string img_path;
	std::string text;
};

