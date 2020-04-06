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
	text_input_has_background = false;
}

gui::Iframe::Iframe(string _name)
{
	name = _name;
	backgroundIsCreated = false;
	text_input_has_background = false;
}

void gui::Iframe::Clear()
{
	for (int i = 0; i < listOfTextLists.size(); i++) {
		if (listOfTextLists[i] != nullptr) {
			delete listOfTextLists[i];
		}
		listOfTextLists[i] = nullptr;
	}
	for (int i = 0; i < listOfTextInputs.size(); i++) {
		if (listOfTextInputs[i] != nullptr) {
			delete listOfTextInputs[i];
		}
		listOfTextInputs[i] = nullptr;
	}
	listOfTexts.clear();
	listOfButtons.clear();
	listOfImages.clear();
}

void gui::Iframe::AddButton(const std::wstring &text, const int xBtn, const int yBtn, const std::string &luaCmd)
{
	gui::Button btn = gui::Button();
	int btnId = Picking::UI::ObtainPickingID();
	btn.create(button_img_name, text, x + xBtn, y + yBtn, btnId, button_txt_color, luaCmd);
	listOfButtons.push_back(btn);
}

void gui::Iframe::AddText(const std::wstring & wtext, const int xPos, const int yPos)
{
	gui::SimpleText txt = gui::SimpleText("static");
	txt.create_static(wtext, "tahoma_13px", 1.f * x + xPos, 1.f * y + yPos, "left", "normal", glm::vec4(255.f));
	listOfTexts.push_back(txt);
}

void gui::Iframe::AddTextList(const int textListId, const int xPos, const int yPos, const std::string & luaCmd, const unsigned int maxOptions, const unsigned int borderWidth)
{
	gui::TextList* _list = new gui::TextList();
	int txtListPickingId = Picking::UI::ObtainPickingID();
	_list->Create(textListId, x + xPos, y + yPos, text_list_font, text_list_color, text_list_background, txtListPickingId, luaCmd, maxOptions, borderWidth);
	listOfTextLists.push_back(_list);
}

void gui::Iframe::AddTextInput(const int textInputId, const int xPos, const int yPos, const int width, std::wstring placeholderText)
{
	gui::TextInput* _input = new gui::TextInput();
	_input->Create(textInputId, Picking::UI::ObtainPickingID(), x + xPos, y + yPos, width, text_input_font, text_input_has_background, text_input_background, text_input_border, text_input_fontweight, placeholderText);
	_input->Enable();
	listOfTextInputs.push_back(_input);
}

void gui::Iframe::Create(int xPos, int yPos, int width, int height, std::wstring iframe_title)
{
	w = width;
	h = height;
	x = xPos;
	y = yPos;
	
	iframeTitle = gui::SimpleText("static");
	iframeTitle.create_static(iframe_title, "tahoma_15px", xPos + 10.f, yPos + h - 15.f, "left", "middle", glm::vec4(255.f), "bold");
	
	ReadXml();
}

void gui::Iframe::Create(const std::string & LuaCommand, std::wstring iframe_title)
{
	Hector::ExecuteCommand(LuaCommand);
	Hector::GetIntegerVariable("x", &x);
	Hector::GetIntegerVariable("y", &y);
	Hector::GetIntegerVariable("w", &w);
	Hector::GetIntegerVariable("h", &h);

	iframeTitle = gui::SimpleText("static");
	iframeTitle.create_static(iframe_title, "tahoma_15px", x + 10.f, y + h - 15.f, "left", "middle", glm::vec4(255.f), "bold");

	ReadXml();
}

void gui::Iframe::Render(bool picking)
{
	if (backgroundIsCreated == false) {
		CreateBackgroundImage();
		backgroundIsCreated = true;
	}

	back.render(picking, 0, 0, true);
	left.render(picking, 0, 0, true);
	top.render(picking, 0, 0, true);
	right.render(picking, 0, 0, true);
	bottom.render(picking, 0, 0, true);
	topleft.render(picking);
	topright.render(picking);
	bottomright.render(picking);
	bottomleft.render(picking);

	RenderImages(picking);
	RenderButtons(picking);
	RenderTexts(picking);
	RenderTextLists(picking);
	RenderTextInputs(picking);

	iframeTitle.render_static();
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
		listOfButtons[i].render(picking, Picking::UI::GetLeftClickId());
	}
}

void gui::Iframe::RenderTexts(bool picking)
{
	if (picking == true) return;

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

void gui::Iframe::RenderTextInputs(bool picking)
{
	for (auto ti : listOfTextInputs)
	{
		ti->Render(picking);
	}
}

void gui::Iframe::ReadXml()
{
	try
	{
		string path = Folders::INTERFACE_IFRAME + name + ".xml";
		tinyxml2::XMLDocument xmlFile;
		xmlFile.LoadFile(path.c_str());

		back_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("background")->Attribute("image_name");
		topleft_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("top_left")->Attribute("image_name");
		topright_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("top_right")->Attribute("image_name");
		bottomright_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom_right")->Attribute("image_name");
		bottomleft_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom_left")->Attribute("image_name");
		right_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("right")->Attribute("image_name");
		left_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("left")->Attribute("image_name");
		top_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("top")->Attribute("image_name");
		bottom_name = xmlFile.FirstChildElement("iframe")->FirstChildElement("bottom")->Attribute("image_name");

		// button data

		if (xmlFile.FirstChildElement("iframe")->FirstChildElement("button") != NULL)
		{
			int r = 0, g = 0, b = 0;
			button_img_name = (string)xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->Attribute("image_name");
			try {
				r = xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->FirstChildElement("textColor")->IntAttribute("r");
				g = xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->FirstChildElement("textColor")->IntAttribute("g");
				b = xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->FirstChildElement("textColor")->IntAttribute("b");
			}
			catch (...) {
				Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the textColor tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
				Logger::Warn(lmsg);
			}
			button_txt_color = vec4(r, g, b, 255);
			button_font = xmlFile.FirstChildElement("iframe")->FirstChildElement("button")->FirstChildElement("font")->Attribute("name");
		}

		// textlist data

		if (xmlFile.FirstChildElement("iframe")->FirstChildElement("textList") != NULL)
		{
			int r = 0, g = 0, b = 0;
			try {
				r = xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("background")->IntAttribute("r");
				g = xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("background")->IntAttribute("g");
				b = xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("background")->IntAttribute("b");
			}
			catch (...) {
				Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the textList::background tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
				Logger::Warn(lmsg);
			}
			text_list_background = vec4(r, g, b, 255);

			r = 0; g = 0; b = 0;
			try {
				r = xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("textColor")->IntAttribute("r");
				g = xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("textColor")->IntAttribute("g");
				b = xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("textColor")->IntAttribute("b");
			}
			catch (...) {
				Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the textList::textColor tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
				Logger::Warn(lmsg);
			}
			text_list_color = vec4(r, g, b, 255);

			text_list_font = xmlFile.FirstChildElement("iframe")->FirstChildElement("textList")->FirstChildElement("font")->Attribute("name");
		}

		// text input data 
		if (xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput") != NULL)
		{
			int r = 0, g = 0, b = 0;

			if (xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("background") != NULL) {
				try {
					r = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("background")->IntAttribute("r");
					g = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("background")->IntAttribute("g");
					b = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("background")->IntAttribute("b");
					text_input_has_background = true;
				}
				catch (...) {
					Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the textInput::background tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
					Logger::Warn(lmsg);
					text_input_has_background = false;
				}
			}
			else {
				text_input_has_background = false;
			}
			text_input_background = vec4(r, g, b, 255);

			r = 0; g = 0; b = 0;
			try {
				r = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("textColor")->IntAttribute("r");
				g = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("textColor")->IntAttribute("g");
				b = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("textColor")->IntAttribute("b");
			}
			catch (...) {
				Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the textInput::textColor tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
				Logger::Warn(lmsg);
			}
			text_input_color = vec4(r, g, b, 255);

			r = 0; g = 0; b = 0;
			if (xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("border") != NULL)
			{
				try {
					r = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("border")->IntAttribute("r");
					g = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("border")->IntAttribute("g");
					b = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("border")->IntAttribute("b");
				}
				catch (...) {
					Logger::LogMessage lmsg = Logger::LogMessage("An error occurred parsing the textInput::border tag of the following iframe: " + name + ".xml", "Warn", "gui", "Iframe", "Create");
					Logger::Warn(lmsg);
				}
			}
			text_input_border = vec4(r, g, b, 255);

			text_input_font = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("font")->Attribute("name");
			text_input_fontweight = xmlFile.FirstChildElement("iframe")->FirstChildElement("textInput")->FirstChildElement("font")->Attribute("weight");
		}
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the iframe data of \"" + name + "\"", "Error", "gui", "Iframe", "Create");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

void gui::Iframe::CreateBackgroundImage()
{
	int commonPickingId = Picking::UI::ObtainPickingID();

	back = gui::Image(back_name);
	back.create("bottom-left", (float)x, (float)y, (float)w, (float)h, commonPickingId);

	left = gui::Image(left_name);
	left.create("bottom-left", (float)x, (float)y, 0.f, (float)h, commonPickingId);

	top = gui::Image(top_name);
	top.create("bottom-left", (float)x, (float)y + (float)h, (float)w, 0.f, commonPickingId);

	right = gui::Image(right_name);
	right.create("bottom-left", (float)x + (float)w, (float)y, 0.f, (float)h, commonPickingId);

	bottom = gui::Image(bottom_name);
	bottom.create("bottom-left", (float)x, (float)y, (float)w, 0.f, commonPickingId);

	topleft = gui::Image(topleft_name);
	topleft.create("bottom-left", (float)x, (float)y + (float)h, 0.f, 0.f, commonPickingId);

	topright = gui::Image(topright_name);
	topright.create("bottom-left", (float)x + w, (float)y + (float)h, 0.f, 0.f, commonPickingId);

	bottomright = gui::Image(bottomright_name);
	bottomright.create("bottom-left", (float)x + w, (float)y, 0.f, 0.f, commonPickingId);

	bottomleft = gui::Image(bottomleft_name);
	bottomleft.create("bottom-left", (float)x, (float)y, 0.f, 0.f, commonPickingId);
}

gui::Iframe::~Iframe()
{
}
