#pragma once

#include "ui_object.h"
#include "simple_text.h"
#include "image.h"

namespace gui {
	class Button : public UIObject
	{
	public:
		Button();
		void create(std::string imageName, int x, int y);
		void render(bool picking);
		~Button();

	private:
		SimpleText buttonText;
		Image buttonImage;
	};
}

