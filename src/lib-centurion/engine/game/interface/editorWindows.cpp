#include "editorWindows.h"
#include <file_manager.h>
#include <logger.h>
#include <settings.h>
#include <picking.h>
#include <hector-lua.h>

#include <engine.h>
#include <tinyxml2.h>
#include <translationsTable.h>

#include <encoding.hpp>

using namespace std;
using namespace glm;

#pragma region static variables

array<EditorWindows::EditorWindow*, MAX_NUMBER_OF_EDITOR_WINDOWS> EditorWindows::listOfWindows = { nullptr };
bool EditorWindows::isHidden = false;

#pragma endregion

#pragma region EditorWindow class

bool EditorWindows::EditorWindow::IsOpened(void)
{
	return this->isOpened;
}

void EditorWindows::EditorWindow::Open(void)
{
	this->isOpened = true;
	this->opening = true;
}

void EditorWindows::EditorWindow::Toggle(void)
{
	if (this->isOpened == false)
	{
		this->Open();
	}
	else {
		this->Close();
	}
}

void EditorWindows::EditorWindow::Close(void)
{
	this->isOpened = false;
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

void EditorWindows::CloseWindow(const unsigned int id)
{
	if (id < 0 || id > MAX_NUMBER_OF_EDITOR_WINDOWS) return;
	if (listOfWindows[id] == nullptr) return;

	listOfWindows[id]->Close();
}

void EditorWindows::ToggleWindow(const unsigned int id)
{
	if (id < 0 || id > MAX_NUMBER_OF_EDITOR_WINDOWS) return;
	if (listOfWindows[id] == nullptr) return;

	listOfWindows[id]->Toggle();
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

			int id = _it_wind->IntAttribute("id");

			string isOpenedStr = string(_it_wind->Attribute("isOpened"));
			bool isOpened = (isOpenedStr == "true");
			string luaOpeningCMD = string(_it_wind->FirstChildElement("openingScript")->GetText());
			string luaConditionCMD = string(_it_wind->FirstChildElement("conditionScript")->GetText());
			string luaConditionFun = string(_it_wind->FirstChildElement("conditionScript")->Attribute("function"));

			gui::Iframe iframe = gui::Iframe(string(_it_wind->Attribute("iframe")));

			string sizeScript = _it_wind->Attribute("size");
			string positionScript = _it_wind->Attribute("position");
			wstring iframeTitle = TranslationsTable::GetWTranslation(_it_wind->Attribute("name"));


			iframe.Create(sizeScript + positionScript, iframeTitle);

			// buttons
			for (tinyxml2::XMLElement* _it_btn = _it_wind->FirstChildElement("buttonArray")->FirstChildElement(); _it_btn != NULL; _it_btn = _it_btn->NextSiblingElement())
			{
				wstring btnText = TranslationsTable::GetWTranslation(_it_btn->Attribute("text"));
				string btnLuaCmd = _it_btn->FirstChildElement("onclickScript")->GetText();
				int btnX = _it_btn->IntAttribute("xOffset");
				int btnY = _it_btn->IntAttribute("yOffset");
				iframe.AddButton(btnText, btnX, btnY, btnLuaCmd);
			}

			// text lists 
			for (tinyxml2::XMLElement* _it_txtlist = _it_wind->FirstChildElement("textListArray")->FirstChildElement(); _it_txtlist != NULL; _it_txtlist = _it_txtlist->NextSiblingElement())
			{
				int textListID = _it_txtlist->IntAttribute("textListId");
				int xOffset = _it_txtlist->IntAttribute("xOffset");
				int yOffset = _it_txtlist->IntAttribute("yOffset");
				int maxOpt = _it_txtlist->IntAttribute("maxOptions");
				int tlWidth = _it_txtlist->IntAttribute("width");
				string txtListLuaCmd = "";
				if (_it_txtlist->FirstChildElement("onclickScript") != NULL) {
					txtListLuaCmd = _it_txtlist->FirstChildElement("onclickScript")->GetText();
				}
				iframe.AddTextList(textListID, xOffset, yOffset, txtListLuaCmd, maxOpt, tlWidth);
			}

			// text inputs
			for (tinyxml2::XMLElement* _it_txtinput = _it_wind->FirstChildElement("textInputArray")->FirstChildElement(); _it_txtinput != NULL; _it_txtinput = _it_txtinput->NextSiblingElement())
			{
				int textInputId = _it_txtinput->IntAttribute("textInputId");
				int xOffset = _it_txtinput->IntAttribute("xOffset");
				int yOffset = _it_txtinput->IntAttribute("yOffset");
				int tiWidth = _it_txtinput->IntAttribute("width");
				wstring placeholder = TranslationsTable::GetWTranslation(_it_txtinput->Attribute("placeholder"));
				iframe.AddTextInput(textInputId, xOffset, yOffset, tiWidth, placeholder);
			}

			// simple texts
			for (tinyxml2::XMLElement* _it_txt = _it_wind->FirstChildElement("simpleTextArray")->FirstChildElement(); _it_txt != NULL; _it_txt = _it_txt->NextSiblingElement())
			{
				int xOffset = _it_txt->IntAttribute("xOffset");
				int yOffset = _it_txt->IntAttribute("yOffset");
				std::wstring wtext = TranslationsTable::GetWTranslation(_it_txt->Attribute("name"));
				iframe.AddText(wtext, xOffset, yOffset);
			}

			eWind->Create(luaOpeningCMD, luaConditionCMD, luaConditionFun, iframe);
			if (isOpened) eWind->Open();
			
			AddWindow(id, eWind);
		}
		isHidden = false;
	}
	catch (const std::exception&)
	{
		Engine::GameClose();
	}
}

void EditorWindows::Render(const bool picking)
{
	if (isHidden) return;

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

bool EditorWindows::AnyWindowIsOpened(void)
{
	for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++)
	{
		if (listOfWindows[i] != nullptr)
		{
			if (listOfWindows[i]->IsOpened()) return true;
		}
	}
	return false;
}

void EditorWindows::CloseEveryWindow(void)
{
	for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++)
	{
		if (listOfWindows[i] != nullptr)
		{
			listOfWindows[i]->Close();
		}
	}
}

void EditorWindows::AddWindow(const unsigned int id, EditorWindow * win)
{
	EditorWindows::listOfWindows[id] = win;
}

EditorWindows::~EditorWindows(void)
{
}

