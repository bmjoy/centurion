#include <ui.h>
#include <file_manager.h>
#include <logger.h>
#include <picking.h>

#include <engine.h>
#include <tinyxml2.h>

using namespace std;
using namespace glm;

gui::Iframe::Iframe()
{
}

gui::Iframe::Iframe(string _name)
{
	name = _name;
}

void gui::Iframe::Clear()
{
	for (int i = 0; i < listOfTextLists.size(); i++) {
		if (listOfTextLists[i] != nullptr) {
			delete listOfTextLists[i];
		}
		listOfTextLists[i] = nullptr;
	}
}

void gui::Iframe::AddTextList(TextList* txtList)
{
	listOfTextLists.push_back(txtList);
}

void gui::Iframe::Create(int xPos, int yPos, int width, int height)
{
	w = width;
	h = height;
	x = xPos;
	y = yPos;

	try
	{

		string path = Folders::INTERFACE_IFRAME + name + ".xml";
		tinyxml2::XMLDocument xmlFile;
		xmlFile.LoadFile(path.c_str());
		
		string back_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("background")->Attribute("image_name");
		string topleft_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("top_left")->Attribute("image_name");
		string topright_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("top_right")->Attribute("image_name");
		string bottomright_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom_right")->Attribute("image_name");
		string bottomleft_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom_left")->Attribute("image_name");
		string right_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("right")->Attribute("image_name");
		string left_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("left")->Attribute("image_name");
		string top_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("top")->Attribute("image_name");
		string bottom_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom")->Attribute("image_name");

		back = gui::Image(back_name);
		back.create("bottom-left", (float)xPos, (float)yPos, (float)w, (float)h, 0);
		
		left = gui::Image(left_name);
		left.create("bottom-left", (float)xPos, (float)yPos, 0.f, (float)h, 0);

		top = gui::Image(top_name);
		top.create("bottom-left", (float)xPos, (float)yPos + (float)h, (float)w, 0.f, 0);

		right = gui::Image(right_name);
		right.create("bottom-left", (float)xPos + (float)w, (float)yPos , 0.f, (float)h, 0);

		bottom = gui::Image(bottom_name);
		bottom.create("bottom-left", (float)xPos, (float)yPos, (float)w, 0.f, 0);

		topleft = gui::Image(topleft_name);
		topleft.create("bottom-left", (float)xPos, (float)yPos + (float)h, 0.f, 0.f, 0);

		topright = gui::Image(topright_name);
		topright.create("bottom-left", (float)xPos + w, (float)yPos + (float)h, 0.f, 0.f, 0);

		bottomright = gui::Image(bottomright_name);
		bottomright.create("bottom-left", (float)xPos + w, (float)yPos, 0.f, 0.f, 0);

		bottomleft = gui::Image(bottomleft_name);
		bottomleft.create("bottom-left", (float)xPos, (float)yPos, 0.f, 0.f, 0);
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the iframe data of \"" + name + "\"", "gui", "Iframe", "Create");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

void gui::Iframe::Render(bool picking)
{
	back.render(false, 0, 0, true);
	left.render(false, 0, 0, true);
	top.render(false, 0, 0, true);
	right.render(false, 0, 0, true);
	bottom.render(false, 0, 0, true);
	topleft.render(false);
	topright.render(false);
	bottomright.render(false);
	bottomleft.render(false);
	   
	RenderImages(picking);
	RenderButtons(picking);
	RenderTexts();
	RenderTextLists(picking);
}

void gui::Iframe::RenderImages(bool picking)
{
	for (int i = 0; i < listOfImages.size(); i++) {
		listOfImages[i].render(picking);
	}
}

void gui::Iframe::RenderButtons(bool picking)
{
	for (int i = 0; i < listOfButtons.size(); i++) {
		listOfButtons[i].render(picking, Picking::leftClickID_UI);
	}
}

void gui::Iframe::RenderTexts()
{
	for (int i = 0; i < listOfTexts.size(); i++) {
		listOfTexts[i].render_static();
	}
}

void gui::Iframe::RenderTextLists(bool picking)
{
	for (int i = 0; i < listOfTextLists.size(); i++) {
		listOfTextLists[i]->Render(picking);
	}
}

gui::Iframe::~Iframe()
{
}