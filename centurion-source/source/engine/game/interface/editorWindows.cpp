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

void EditorWindows::EditorWindow::Clear()
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
		iframe.Render(picking);
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
			bool isOpened = bool(_it_wind->isOpened());
			string luaOpeningCMD = string(_it_wind->openingScript());
			string luaConditionCMD = string(_it_wind->conditionScript());
			string luaConditionFun = string(_it_wind->conditionScript().function());

			gui::Iframe iframe = gui::Iframe(string(_it_wind->iframe()));
			iframe.Create((int)_it_wind->x(), (int)_it_wind->y(), (int)_it_wind->width(), (int)_it_wind->height());

			// text lists 

			c_editorWindow::textList_iterator _it_txtlist;
			for (_it_txtlist = _it_wind->textList().begin(); _it_txtlist != _it_wind->textList().end(); _it_txtlist++) {

				gui::TextList* _list = new gui::TextList();
				_list->Create(
					int(_it_txtlist->textListId()),
					int(_it_wind->x() + _it_txtlist->xOffset()),
					int(_it_wind->y() + _it_txtlist->yOffset()),
					string(_it_txtlist->text().font()),
					vec4(_it_txtlist->text().r(), _it_txtlist->text().g(), _it_txtlist->text().b(), 255.f),
					vec4(_it_txtlist->text_background().r(), _it_txtlist->text_background().g(), _it_txtlist->text_background().b(), 255.f),
					PickingUI::obtainPickingID()
				);

				gui::TextList::AddTextListToArray(int(_it_txtlist->textListId()), _list);
				iframe.AddTextList(_list);



			}

			c_editorWindow::button_iterator _it_btn;
			for (_it_btn = _it_wind->button().begin(); _it_btn != _it_wind->button().end(); _it_btn++) {

				gui::Button btn = gui::Button();
				btn.create(
					string(_it_btn->image_name()),
					string(_it_btn->name()),
					int(_it_wind->x() + _it_btn->xOffset()),
					int(_it_wind->y() + _it_btn->yOffset()),
					PickingUI::obtainPickingID(),
					vec4(0, 0, 0, 255),
					string(_it_btn->onclick())
				);

				iframe.AddButton(btn);
			}

			eWind->Create(luaOpeningCMD, luaConditionCMD, luaConditionFun, iframe);
			if (isOpened) eWind->Open();

			AddWindow(id, eWind);
		}
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

void EditorWindows::Render(bool picking)
{
	for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++) {
		if (listOfWindows[i] != nullptr) {
			listOfWindows[i]->Render(picking);
		}
	}
}


void EditorWindows::Clear()
{
	for (int i = 0; i < MAX_NUMBER_OF_EDITOR_WINDOWS; i++) {
		if (listOfWindows[i] != nullptr) {
			listOfWindows[i]->Clear();
			delete listOfWindows[i];
		}
		listOfWindows[i] = nullptr;
	}
}

EditorWindows::~EditorWindows()
{
}