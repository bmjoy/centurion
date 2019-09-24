#pragma once

#include <global.hpp>

#include <objects.h>

#include "../primitives/empty_rectangle.h"
#include "../primitives/filled_rectangle.h"
#include "ui_object.h"

class FormInput : public UIObject
{
public:
	FormInput();
	FormInput(bool options);
	void create(float w, float h, std::vector<std::string> options);
	void render(bool picking = false, glm::vec4 border_color = glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
	void open_close();
	void close();
	void select_option(int i);
	std::string selectedText;
	~FormInput();

private:
	EmptyRectangle border;
	EmptyRectangle optionsBorder;
	FilledRectangle back;
	//CBitmapFont text;
	
	float width, height;
	std::array<float, 8> coords;
	std::vector<std::string> form_options;
	glm::vec4 back_color;
	
	bool boolOptions;
	bool isOpened;

};

