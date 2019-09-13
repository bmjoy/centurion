#pragma once

#include <global.hpp>
#include "ui_object.h"

#include "../primitives/image.h"

class DivImage : public UIObject
{
public:
	DivImage();
	void create(std::string origin, glm::ivec2 image_size = glm::ivec2(0,0));
	void render(bool picking);
	~DivImage();
private:
	Image img;
};

