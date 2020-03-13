#include <interface>
#include <game/strategy.h>
#include <picking.h>
#include <menu>
#include <engine.h>
#include <translationsTable.h>

using namespace glb;
using namespace glm;

namespace menu {

	GlobalOptions::GlobalOptions() {};

	void GlobalOptions::create() {

		startX = Engine::myWindow::Width / 2.f - 300.f;
		startY = Engine::myWindow::Height / 2.f + 200.f;

		back.create("border-filled", startX, startY, 600.f, 400.f, "top-left", 0);

		vector<string> form_options;
		map<string, int> langMap = TranslationsTable::GetLanguagesMap();
		for (map<string, int>::iterator i = langMap.begin(); i != langMap.end(); i++) {
			form_options.push_back("LANG_" + i->first);
		}
		languages = gui::FormInput(true);
		languages.create(startX + 150.f, startY - 77.f, 150.f, 20.f, form_options, PickingUI::obtainPickingID());
		languages.selectedText = "LANG_"+ Settings::Language;
		PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, "GlobalOptions_formLanguages");

		text[0] = gui::SimpleText("static");
		text[0].create_static(TranslationsTable::GetTranslation("OPTIONS_Title"), "tahoma_15px", startX + 20.f, startY - 40.f, "left", "normal", vec4(255), "bold");

		text[1] = gui::SimpleText("static");
		text[1].create_static(TranslationsTable::GetTranslation("OPTIONS_LanguageText"), "tahoma_15px", startX + 20.f, startY - 93.f, "left", "normal", vec4(255), "normal");

	}

	void GlobalOptions::render(bool pick) {
		if (pick && Engine::Mouse::LeftClick) {
			languages.render(true, vec4());
			picking();
		}
		if (!pick) {
			back.render(vec4(0, 0, 0, 0.5f));
			languages.render(false, vec4(0,0,0,255));
			text[0].render_static();
			text[1].render_static();
		}
	}

	void GlobalOptions::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(Picking::leftClickID_UI);

		if (Picking::leftClickID_UI == 0)
			languages.close();

		if (clickName == "GlobalOptions_formLanguages") {
			int i = languages.get_clicked_option();
			languages.open_close();
			if (i > 0) {
				languages.select_option(i);
				currentLan = languages.selectedText.substr(5);
			}
		}
	}

	GlobalOptions::~GlobalOptions() {};
};