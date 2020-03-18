#include "editorWindows.h"
#include "editorWindows-xml.hxx"

#include <file_manager.h>
#include <logger.h>
#include <settings.h>
#include <picking.h>
#include <hector-lua.h>

#pragma region static variables

array<EditorWindows::EditorWindow*, MAX_NUMBER_OF_EDITOR_WINDOWS> EditorWindows::listOfWindows = { nullptr };

#pragma endregion

#pragma region EditorWindow class

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

void EditorWindows::EditorWindow::Render(bool picking)
{
	if (isOpened == false) {
		
		bool conditionResult = Hector::ExecuteBooleanMethod(luaConditionFunction);
		if (conditionResult) {
			this->Open();
		}
	}

	if (opening) {
		Hector::ExecuteCommand(luaOpeningScript);
		opening = false;
	}

	if (isOpened) {
		iframe.Render();
	}
}

#pragma endregion

void EditorWindows::OpenWindow(int id)
{
	if (id < 0 || id > MAX_NUMBER_OF_EDITOR_WINDOWS) return;
	if (listOfWindows[id] == nullptr) return;

	listOfWindows[id]->Open();
}

void EditorWindows::Create()
{
	try
	{
		xml_schema::properties props;
		props.no_namespace_schema_location(Folders::XML_SCHEMAS + "editorWindows.xsd");
		auto_ptr<c_editorWindows> winXml = c_editorWindows_(Folders::INTERFACE_EDITOR + "editorWindows.xml", 0, props);

		c_editorWindows::c_editorWindow_iterator _it_wind;
		for (_it_wind = winXml->c_editorWindow().begin(); _it_wind != winXml->c_editorWindow().end(); _it_wind++) {

			EditorWindow* eWind = new EditorWindow();

			int id = int(_it_wind->id());
			string luaOpeningCMD = string(_it_wind->openingScript());
			string luaConditionCMD = string(_it_wind->conditionScript());
			string luaConditionFun = string(_it_wind->conditionScript().function());

			gui::Iframe iframe = gui::Iframe(string(_it_wind->iframe()));
			iframe.Create((int)_it_wind->x(), (int)_it_wind->y(), (int)_it_wind->width(), (int)_it_wind->height());

			eWind->Create(luaOpeningCMD, luaConditionCMD, luaConditionFun, iframe);
			AddWindow(id, eWind);
		}
	}
	catch (const xml_schema::exception & e) {
		std::cout << e << std::endl;
		throw;
	}
	catch (const std::exception&)
	{

	}
}

void EditorWindows::Render(bool picking)
{
	for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++) {
		if (listOfWindows[i] != nullptr) {
			listOfWindows[i]->Render(picking);
		}
	}
}

