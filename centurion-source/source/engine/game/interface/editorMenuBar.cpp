#include "editorMenuBar.h"
#include "editorMenuBar-xml.hxx"

#include <file_manager.h>
#include <logger.h>
#include <settings.h>
#include <engine.h>
#include <picking.h>
#include <translationsTable.h>

#pragma region Static variables

array<EditorMenuBar::EditorMenu*, MAX_NUMBER_OF_EDITOR_MENUS> EditorMenuBar::listOfMenus = { nullptr };
gui::Rectangle EditorMenuBar::topBar;
vec4 EditorMenuBar::color;
float EditorMenuBar::height;
float EditorMenuBar::width;
unsigned int EditorMenuBar::minPickingId = 0, EditorMenuBar::maxPickingId = 0;
string EditorMenuBar::font = "tahoma_13px";

#pragma endregion


#pragma region EditorMenu class

void EditorMenuBar::EditorMenu::Create(gui::Rectangle _titleBack, gui::SimpleText _titleText, vector<gui::Rectangle> _optionsBack, vector<gui::SimpleText> _optionsText)
{
	titleBack = _titleBack;
	titleText = _titleText;
	optionsBack = _optionsBack;
	optionsText = _optionsText;
}

void EditorMenuBar::EditorMenu::Render(bool picking, vec4 &color)
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


void EditorMenuBar::ToggleEditorMenu(int id)
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

void EditorMenuBar::Create()
{
	try
	{
		xml_schema::properties props;
		props.no_namespace_schema_location(Folders::XML_SCHEMAS + "editorMenuBar.xsd");
		auto_ptr<c_editorMenuBar> menuXML = c_editorMenuBar_(Folders::INTERFACE_EDITOR + "editorMenuBar.xml", 0, props);

		/* bar properties */

		font = string(menuXML->font());
		color = vec4(menuXML->c_background().r(), menuXML->c_background().g(), menuXML->c_background().b(), 255.f);
		width = Engine::myWindow::Width;
		height = float(menuXML->height());

		topBar = gui::Rectangle();
		topBar.create("filled", 0.0f, (float)-height, Engine::myWindow::Width, (float)height, "bottom-left", 0);

		/* menus properties */

		c_editorMenuBar::c_editorMenu_iterator _it_menu;
		float titlePosX = 0.f;
		float titlePosY = Engine::myWindow::Height - height;
		 
		minPickingId = PickingUI::getLastID();

		for (_it_menu = menuXML->c_editorMenu().begin(); _it_menu != menuXML->c_editorMenu().end(); _it_menu++) {

			// ------ menu title
			EditorMenu* _menu = new EditorMenu();
			int _menu_id = _it_menu->id();
			string title = TranslationsTable::GetTranslation(string(_it_menu->name()));
			string luaCmd = string(_it_menu->onclick());
			float titleWidth = std::max(int(gui::SimpleText::CalculateTextWidth(title, font)) + 20, 60);

			gui::Rectangle titleBack = gui::Rectangle();
			titleBack.create("filled", titlePosX, titlePosY, titleWidth, height, "bottom-left", PickingUI::obtainPickingID(), luaCmd);
			gui::SimpleText titleText = gui::SimpleText("static");
			titleText.create_static(title, font, titlePosX + 0.5f * titleWidth, titlePosY + 0.5f * height, "center", "middle", vec4(255.f), "normal");
			
			// ------ menu options

			c_editorMenu::command_iterator _it_cmd;
			float maxOptionWordSize = 0;
			
			// calculate the submenu width
			for (_it_cmd = _it_menu->command().begin(); _it_cmd != _it_menu->command().end(); _it_cmd++) {
				string optionstring = title + "_" + string(_it_cmd->name());
				maxOptionWordSize = std::max(maxOptionWordSize, gui::SimpleText::CalculateTextWidth(optionstring, font));
			}
			float optionsWidth = maxOptionWordSize + 50;
			float offSetX = 20.f;

			// create vectors
			vector<gui::Rectangle> optionsBack = vector<gui::Rectangle>();
			vector<gui::SimpleText> optionsText = vector<gui::SimpleText>();
			for (_it_cmd = _it_menu->command().begin(); _it_cmd != _it_menu->command().end(); _it_cmd++) {
				string optionstring = TranslationsTable::GetTranslation(string(_it_menu->name()) + "_" + string(_it_cmd->name()));
				string optionLuaCmd = string(_it_cmd->onclick());
				int j = int(_it_cmd->id());

				gui::Rectangle optionback = gui::Rectangle();
				optionback.create("filled", titlePosX, titlePosY - height * (j + 1), optionsWidth, height, "bottom-left", PickingUI::obtainPickingID(), optionLuaCmd);

				gui::SimpleText optiontext = gui::SimpleText("static");
				optiontext.create_static(optionstring, font, titlePosX + offSetX, titlePosY - height * (j + 1) + height * 0.5f, "left", "middle", vec4(255.f), "normal");

				optionsBack.push_back(optionback);
				optionsText.push_back(optiontext);
			}

			_menu->Create(titleBack, titleText, optionsBack, optionsText);

			AddMenu(_menu_id, _menu);
			titlePosX += titleWidth;
		}
		maxPickingId = PickingUI::getLastID() + 1;

	}
	catch (const xml_schema::exception & e) {
		std::cout << e << std::endl;
		throw;
	}
	catch (const std::exception&)
	{
		throw;
	}
}

void EditorMenuBar::Render(bool picking)
{
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

