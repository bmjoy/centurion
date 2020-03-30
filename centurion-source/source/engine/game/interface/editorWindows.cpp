#include "editorWindows.h"
#include <file_manager.h>
#include <logger.h>
#include <settings.h>
#include <picking.h>
#include <hector-lua.h>

#include <engine.h>
#include <tinyxml2.h>

#pragma region static variables

array<EditorWindows::EditorWindow*, MAX_NUMBER_OF_EDITOR_WINDOWS> EditorWindows::listOfWindows = { nullptr };

#pragma endregion

#pragma region EditorWindow class

bool EditorWindows::EditorWindow::IsOpened(void)
{
	return EditorWindow::isOpened;
}

void EditorWindows::EditorWindow::Open(void)
{
	EditorWindow::isOpened = true;
	EditorWindow::opening = true;
}

void EditorWindows::EditorWindow::Close(void)
{
	EditorWindow::isOpened = false;
}

void EditorWindows::EditorWindow::Clear(void)
{
	iframe.Clear();
}

void EditorWindows::EditorWindow::Create(string _luaOpeningScript, string _luaConditionScript, string _luaConditionFunction, gui::Iframe _iframe)
{
	luaOpeningScript = _luaOpeningScript;
	luaConditionScript = _luaConditionScript;
	luaConditionFunction = _luaConditionFunction;
	iframe = _iframe;
	opening = false;
	isOpened = false;

	Hector::ExecuteCommand(luaConditionScript);
}

void EditorWindows::EditorWindow::Render(const bool picking)
{
	if (isOpened == false) {

		bool conditionResult = false;
		Hector::ExecuteBooleanMethod(luaConditionFunction, &conditionResult);
		if (conditionResult) {
			this->Open();
		}
	}

	if (opening) {
		Hector::ExecuteCommand(luaOpeningScript);
		opening = false;
	}

	if (isOpened) {
		iframe.Render(picking);
	}
}

#pragma endregion

void EditorWindows::OpenWindow(const unsigned int id)
{
	if (id < 0 || id > MAX_NUMBER_OF_EDITOR_WINDOWS) return;
	if (listOfWindows[id] == nullptr) return;

	listOfWindows[id]->Open();
}

void EditorWindows::Create(void)
{
	try
	{
		string path = Folders::INTERFACE_EDITOR + "editorWindows.xml";
		tinyxml2::XMLDocument xmlFile;
		xmlFile.LoadFile(path.c_str());


		for (tinyxml2::XMLElement* _it_wind = xmlFile.FirstChildElement("editorWindows")->FirstChildElement(); _it_wind != NULL; _it_wind = _it_wind->NextSiblingElement())
		{
			EditorWindow* eWind = new EditorWindow();

			int id = stoi(string(_it_wind->Attribute("id")));

			string isOpenedStr = string(_it_wind->Attribute("isOpened"));
			bool isOpened = (isOpenedStr == "true");
			string luaOpeningCMD = string(_it_wind->FirstChildElement("openingScript")->GetText());
			string luaConditionCMD = string(_it_wind->FirstChildElement("conditionScript")->GetText());
			string luaConditionFun = string(_it_wind->FirstChildElement("conditionScript")->Attribute("function"));

			gui::Iframe iframe = gui::Iframe(string(_it_wind->Attribute("iframe")));

			string sizeScript = string(_it_wind->Attribute("size"));
			string positionScript = string(_it_wind->Attribute("position"));

			Hector::ExecuteCommand(sizeScript);
			Hector::ExecuteCommand(positionScript);
			int x = 0, y = 0, w = 0, h = 0;
			Hector::GetIntegerVariable("x", &x);
			Hector::GetIntegerVariable("y", &y);
			Hector::GetIntegerVariable("width", &w);
			Hector::GetIntegerVariable("height", &h);

			iframe.Create(x, y, w, h);

			// text lists 
			for (tinyxml2::XMLElement* _it_txtlist = _it_wind->FirstChildElement("textListArray")->FirstChildElement(); _it_txtlist != NULL; _it_txtlist = _it_txtlist->NextSiblingElement())
			{
				gui::TextList* _list = new gui::TextList();
				int textListID = stoi(_it_txtlist->Attribute("textListId"));
				_list->Create(
					textListID,
					x + stoi(_it_txtlist->Attribute("xOffset")),
					y + stoi(_it_txtlist->Attribute("yOffset")),
					string(_it_txtlist->FirstChildElement("text")->Attribute("font")),
					vec4(stoi(_it_txtlist->FirstChildElement("text")->Attribute("r")),
						stoi(_it_txtlist->FirstChildElement("text")->Attribute("g")),
						stoi(_it_txtlist->FirstChildElement("text")->Attribute("b")),
						255.f),
					vec4(stoi(_it_txtlist->FirstChildElement("text_background")->Attribute("r")),
						stoi(_it_txtlist->FirstChildElement("text_background")->Attribute("g")),
						stoi(_it_txtlist->FirstChildElement("text_background")->Attribute("b")),
						255.f),
					PickingUI::ObtainPickingID()
				);
				gui::TextList::AddTextListToArray(textListID, _list);
				iframe.AddTextList(_list);
			}

			// buttons
			for (tinyxml2::XMLElement* _it_btn = _it_wind->FirstChildElement("buttonArray")->FirstChildElement(); _it_btn != NULL; _it_btn = _it_btn->NextSiblingElement())
			{
				gui::Button btn = gui::Button();
				btn.create(
					string(_it_btn->Attribute("image_name")),
					string(_it_btn->Attribute("name")),
					x + stoi(_it_btn->Attribute("xOffset")),
					y + stoi(_it_btn->Attribute("yOffset")),
					PickingUI::ObtainPickingID(),
					vec4(0, 0, 0, 255),
					string(_it_btn->Attribute("onclick"))
				);

				iframe.AddButton(btn);
			}

			eWind->Create(luaOpeningCMD, luaConditionCMD, luaConditionFun, iframe);
			if (isOpened) eWind->Open();

			AddWindow(id, eWind);
		}
	}
	catch (const std::exception&)
	{
		Engine::GameClose();
	}
}

void EditorWindows::Render(const bool picking)
{
	for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++) {
		if (listOfWindows[i] != nullptr) {
			listOfWindows[i]->Render(picking);
		}
	}
}


void EditorWindows::Clear(void)
{
	for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++)
	{
		if (listOfWindows[i] != nullptr)
		{
			listOfWindows[i]->Clear();
			delete listOfWindows[i];
		}
		listOfWindows[i] = nullptr;
	}
}

EditorWindows::~EditorWindows(void)
{
}

void EditorWindows::AddWindow(const unsigned int id, EditorWindow * win)
{
	EditorWindows::listOfWindows[id] = win;
}
