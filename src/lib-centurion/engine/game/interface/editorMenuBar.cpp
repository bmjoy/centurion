#include "editorMenuBar.h"

#include <file_manager.h>
#include <logger.h>
#include <settings.h>
#include <engine.h>
#include <peripherals/mywindow.h>
#include <picking.h>
#include <translationsTable.h>
#include <tinyxml2.h>

using namespace std;
using namespace glm;

#pragma region Static variables

array<EditorMenuBar::EditorMenu*, MAX_NUMBER_OF_EDITOR_MENUS> EditorMenuBar::listOfMenus = { nullptr };
gui::Rectangle EditorMenuBar::topBar;
vec4 EditorMenuBar::color;
float EditorMenuBar::height;
float EditorMenuBar::width;
bool EditorMenuBar::isHidden = false;
unsigned int EditorMenuBar::minPickingId = 0, EditorMenuBar::maxPickingId = 0;
string EditorMenuBar::font = "tahoma_13px";

#pragma endregion


#pragma region EditorMenu class

bool EditorMenuBar::EditorMenu::IsOpened(void)
{
	if (this != nullptr) {
		return EditorMenu::isOpened;
	}
	return false;
}

void EditorMenuBar::EditorMenu::Open(void)
{
	EditorMenu::isOpened = true;
}

void EditorMenuBar::EditorMenu::Close(void)
{

	EditorMenu::isOpened = false;
}

void EditorMenuBar::EditorMenu::Create(gui::Rectangle _titleBack, gui::SimpleText _titleText, vector<gui::Rectangle> _optionsBack, vector<gui::SimpleText> _optionsText)
{
	titleBack = _titleBack;
	titleText = _titleText;
	optionsBack = _optionsBack;
	optionsText = _optionsText;
}

void EditorMenuBar::EditorMenu::Render(const bool picking, const vec4 &color)
{
	titleBack.render(color, picking, Picking::leftClickID_UI);
	if (picking == false) {
		titleText.render_static();
	}

	if (isOpened == false) return;

	for (int i = 0; i < optionsBack.size(); i++) {
		optionsBack[i].render(color, picking, Picking::leftClickID_UI);
		if (picking == false) {
			optionsText[i].render_static();
		}
	}
}

#pragma endregion


void EditorMenuBar::ToggleEditorMenu(const unsigned int id)
{
	if (Engine::Mouse::LeftClick) {
		if (listOfMenus[id]->IsOpened()) {
			listOfMenus[id]->Close();
		}
		else {
			listOfMenus[id]->Open();
			for (int i = 0; i < MAX_NUMBER_OF_EDITOR_MENUS; i++) {
				if (i == id) continue;
				if (listOfMenus[i] == nullptr) continue;
				listOfMenus[i]->Close();
			}
		}
		Engine::Mouse::LeftClick = false;
	}
}

void EditorMenuBar::Create(void)
{
	try
	{
		string path = Folders::INTERFACE_EDITOR + "editorMenuBar.xml";
		tinyxml2::XMLDocument xmlFile;
		xmlFile.LoadFile(path.c_str());

		/* bar properties */

		font = string(xmlFile.FirstChildElement("editorMenuBar")->Attribute("font"));

		tinyxml2::XMLElement *_background = xmlFile.FirstChildElement("editorMenuBar")->FirstChildElement("background");
		color = vec4(stoi(_background->Attribute("r")), stoi(_background->Attribute("g")), stoi(_background->Attribute("b")), 255.f);

		width = Engine::myWindow::Width;

		string height_str = string(xmlFile.FirstChildElement("editorMenuBar")->Attribute("height"));
		height = stof(height_str);

		topBar = gui::Rectangle();
		topBar.create("filled", 0.0f, (float)-height, Engine::myWindow::Width, (float)height, "bottom-left", 0);

		/* menus properties */

		float titlePosX = 0.f;
		float titlePosY = Engine::myWindow::Height - height;

		minPickingId = PickingUI::GetLastPickingID();

		tinyxml2::XMLElement *levelElement = xmlFile.FirstChildElement("editorMenuBar")->FirstChildElement("editorMenuArray");
		for (tinyxml2::XMLElement* _it_menu = levelElement->FirstChildElement(); _it_menu != NULL; _it_menu = _it_menu->NextSiblingElement())
		{
			// ------ menu title
			EditorMenu* _menu = new EditorMenu();
			int _menu_id = stoi(_it_menu->Attribute("id"));
			string title = TranslationsTable::GetTranslation(string(_it_menu->Attribute("name")));
			string luaCmd = string(_it_menu->Attribute("onclick"));
			float titleWidth = std::max(gui::SimpleText::CalculateTextWidth(title, font) + 20.f, 60.f);

			gui::Rectangle titleBack = gui::Rectangle();
			titleBack.create("filled", titlePosX, titlePosY, titleWidth, height, "bottom-left", PickingUI::ObtainPickingID(), luaCmd);
			gui::SimpleText titleText = gui::SimpleText("static");
			titleText.create_static(title, font, titlePosX + 0.5f * titleWidth, titlePosY + 0.5f * height, "center", "middle", vec4(255.f), "normal");

			// ------ menu options

			float maxOptionWordSize = 0;

			// calculate the submenu width
			for (tinyxml2::XMLElement* _it_cmd = _it_menu->FirstChildElement(); _it_cmd != NULL; _it_cmd = _it_cmd->NextSiblingElement())
			{
				string optionstring = title + "_" + string(_it_cmd->Attribute("name"));
				maxOptionWordSize = std::max(maxOptionWordSize, gui::SimpleText::CalculateTextWidth(optionstring, font));
			}
			float optionsWidth = maxOptionWordSize + 50;
			float offSetX = 20.f;

			// create vectors
			vector<gui::Rectangle> optionsBack = vector<gui::Rectangle>();
			vector<gui::SimpleText> optionsText = vector<gui::SimpleText>();
			for (tinyxml2::XMLElement* _it_cmd = _it_menu->FirstChildElement(); _it_cmd != NULL; _it_cmd = _it_cmd->NextSiblingElement())
			{
				string optionstring = TranslationsTable::GetTranslation(string(_it_menu->Attribute("name")) + "_" + string(_it_cmd->Attribute("name")));
				string optionLuaCmd = string(_it_cmd->FirstChildElement("onclickScript")->GetText());
				int j = stoi(_it_cmd->Attribute("id"));

				gui::Rectangle optionback = gui::Rectangle();
				optionback.create("filled", titlePosX, titlePosY - height * (j + 1), optionsWidth, height, "bottom-left", PickingUI::ObtainPickingID(), optionLuaCmd);

				gui::SimpleText optiontext = gui::SimpleText("static");
				optiontext.create_static(optionstring, font, titlePosX + offSetX, titlePosY - height * (j + 1) + height * 0.5f, "left", "middle", vec4(255.f), "normal");

				optionsBack.push_back(optionback);
				optionsText.push_back(optiontext);
			}

			_menu->Create(titleBack, titleText, optionsBack, optionsText);

			AddMenu(_menu_id, _menu);
			titlePosX += titleWidth;
		}
		maxPickingId = PickingUI::GetLastPickingID() + 1;
		Engine::myWindow::TopBarHeight = height;
	}
	catch (const std::exception&)
	{
		Engine::GameClose();
	}
}

void EditorMenuBar::Render(const bool picking)
{
	if (isHidden) return;

	topBar.render(color, picking);
	for (int i = 0; i < MAX_NUMBER_OF_EDITOR_MENUS; i++) {
		if (listOfMenus[i] != nullptr) {
			if (picking == false) {
				if (Engine::Mouse::LeftClick && Engine::Mouse::GetYPosition() < Engine::myWindow::Height - height) {
					if (Picking::leftClickID_UI > minPickingId || Picking::leftClickID_UI < maxPickingId) {
						listOfMenus[i]->Close();
					}
				}
			}
			listOfMenus[i]->Render(picking, color);
		}
	}
}

void EditorMenuBar::Hide(void)
{
	isHidden = true;
	Engine::myWindow::TopBarHeight = 0;
}

void EditorMenuBar::Show(void)
{
	isHidden = false;
	Engine::myWindow::TopBarHeight = height;
}

bool EditorMenuBar::IsOpened(void)
{
	for (auto i : EditorMenuBar::listOfMenus)
	{
		if (i->IsOpened()) return true;
	}
	return false;
}

void EditorMenuBar::AddMenu(const unsigned int id, EditorMenu * menu)
{
	EditorMenuBar::listOfMenus[id] = menu;
}

