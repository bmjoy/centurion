#pragma once

#include <global.hpp>
#include <objects.h>

#include "ui_object.h"
#include "rectangle.h"

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
	gui::Rectangle back;
	std::vector<gui::Rectangle> back_options;
	float width, height;
	std::vector<std::string> form_options;
	
	bool boolOptions;
	bool isOpened;

};

