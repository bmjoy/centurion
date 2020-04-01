#include <ui.h>
#include <file_manager.h>
#include <logger.h>
#include <picking.h>

#include <engine.h>
#include <tinyxml2.h>
#include <hector-lua.h>

using namespace std;
using namespace glm;

gui::Iframe::Iframe()
{
	backgroundIsCreated = false;
}

gui::Iframe::Iframe(string _name)
{
	name = _name;
	backgroundIsCreated = false;
}

void gui::Iframe::AddButton(const std::string &text, const int xBtn, const int yBtn, const std::string &luaCmd)
{
	gui::Button btn = gui::Button();
	int btnId = PickingUI::ObtainPickingID();
	btn.create(button_img_name, text, x + xBtn, y + yBtn, btnId, button_txt_color, luaCmd);
	listOfButtons.push_back(btn);
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

void gui::Iframe::AddTextList(const int textListId, const int xPos, const int yPos)
{
	gui::TextList* _list = new gui::TextList();
	int txtListPickingId = PickingUI::ObtainPickingID();
	_list->Create(textListId, x + xPos, y + yPos, text_list_font, text_list_color, text_list_background, txtListPickingId);
	listOfTextLists.push_back(_list);
}

void gui::Iframe::Create(int xPos, int yPos, int width, int height)
{
	w = width;
	h = height;
	x = xPos;
	y = yPos;

	ReadXml();
}

void gui::Iframe::Create(const std::string & LuaCommand)
{
	Hector::ExecuteCommand(LuaCommand);
	Hector::GetIntegerVariable("x", &x);
	Hector::GetIntegerVariable("y", &y);
	Hector::GetIntegerVariable("w", &w);
	Hector::GetIntegerVariable("h", &h);

	ReadXml();
}

void gui::Iframe::Render(bool picking)
{
	if (backgroundIsCreated == false) {
		CreateBackgroundImage();
		backgroundIsCreated = true;
	}

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

void gui::Iframe::ReadXml()
{
	try
	{
		string path = Folders::INTERFACE_IFRAME + name + ".xml";
		tinyxml2::XMLDocument xmlFile;
		xmlFile.LoadFile(path.c_str());

		back_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("background")->Attribute("image_name");
		topleft_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("top_left")->Attribute("image_name");
		topright_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("top_right")->Attribute("image_name");
		bottomright_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom_right")->Attribute("image_name");
		bottomleft_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom_left")->Attribute("image_name");
		right_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("right")->Attribute("image_name");
		left_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("left")->Attribute("image_name");
		top_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("top")->Attribute("image_name");
		bottom_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom")->Attribute("image_name");

		// button data

		if (xmlFile.FirstChildElement("iframe")->FirstChildElement("button") != NULL)
		{
			int r = 0, g = 0, b = 0;
			button_img_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->Attribute("image_name");
			try {
				r = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->FirstChildElement("textColor")->Attribute("r")));
				g = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->FirstChildElement("textColor")->Attribute("g")));
				b = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->FirstChildElement("textColor")->Attribute("b")));
			}
			catch (...) {
				Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the textColor tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
				Logger::Warn(lmsg);
			}
			button_txt_color = vec4(r, g, b, 255);
			button_font = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->FirstChildElement("font")->Attribute("name");
		}

		// textlist data

		if (xmlFile.FirstChildElement("iframe")->FirstChildElement("textList") != NULL)
		{
			int r = 0, g = 0, b = 0;
			button_img_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->Attribute("image_name");

			try {
				r = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("background")->Attribute("r")));
				g = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("background")->Attribute("g")));
				b = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("background")->Attribute("b")));
			}
			catch (...) {
				Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the background tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
				Logger::Warn(lmsg);
			}
			text_list_background = vec4(r, g, b, 255);

			r = 0; g = 0; b = 0;
			try {
				r = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("textColor")->Attribute("r")));
				g = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("textColor")->Attribute("g")));
				b = stoi(string(xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("textColor")->Attribute("b")));
			}
			catch (...) {
				Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the background tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
				Logger::Warn(lmsg);
			}
			text_list_color = vec4(r, g, b, 255);

			text_list_font = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("font")->Attribute("name");
		}
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the iframe data of \"" + name + "\"", "gui", "Iframe", "Create");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

void gui::Iframe::CreateBackgroundImage()
{
	back = gui::Image(back_name);
	back.create("bottom-left", (float)x, (float)y, (float)w, (float)h, 0);

	left = gui::Image(left_name);
	left.create("bottom-left", (float)x, (float)y, 0.f, (float)h, 0);

	top = gui::Image(top_name);
	top.create("bottom-left", (float)x, (float)y + (float)h, (float)w, 0.f, 0);

	right = gui::Image(right_name);
	right.create("bottom-left", (float)x + (float)w, (float)y, 0.f, (float)h, 0);

	bottom = gui::Image(bottom_name);
	bottom.create("bottom-left", (float)x, (float)y, (float)w, 0.f, 0);

	topleft = gui::Image(topleft_name);
	topleft.create("bottom-left", (float)x, (float)y + (float)h, 0.f, 0.f, 0);

	topright = gui::Image(topright_name);
	topright.create("bottom-left", (float)x + w, (float)y + (float)h, 0.f, 0.f, 0);

	bottomright = gui::Image(bottomright_name);
	bottomright.create("bottom-left", (float)x + w, (float)y, 0.f, 0.f, 0);

	bottomleft = gui::Image(bottomleft_name);
	bottomleft.create("bottom-left", (float)x, (float)y, 0.f, 0.f, 0);
}

gui::Iframe::~Iframe()
{
}
