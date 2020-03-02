#include <interface>
#include <game>
#include <picking>
#include <menu>
#include <engine/mouse.h>
#include <engine/window.h>

using namespace glb;
using namespace glm;

namespace menu {

	GlobalOptions::GlobalOptions() {};

	void GlobalOptions::create() {

		startX = engine::myWindow::Width / 2.f - 300.f;
		startY = engine::myWindow::Height / 2.f + 200.f;

		back.create("border-filled", startX, startY, 600.f, 400.f, "top-left", 0);

		vector<string> form_options;
		for (map<string,int>::iterator i = availableLanguages.begin(); i != availableLanguages.end(); i++)
			form_options.push_back("LANG_" + i->first);
		languages = gui::FormInput(true);
		languages.create(startX + 150.f, startY - 77.f, 150.f, 20.f, form_options, getPickingID());
		languages.selectedText = "LANG_"+ Settings::Language;
		addValueToPickingListUI(getPickingID(), "GlobalOptions_formLanguages");
		increasePickingID();

		text[0] = gui::SimpleText("static");
		text[0].create_static(getTranslation("OPTIONS_Title"), "tahoma_15px", startX + 20.f, startY - 40.f, "left", "normal", vec4(255), "bold");

		text[1] = gui::SimpleText("static");
		text[1].create_static(getTranslation("OPTIONS_LanguageText"), "tahoma_15px", startX + 20.f, startY - 93.f, "left", "normal", vec4(255), "normal");

	}

	void GlobalOptions::render(bool pick) {
		if (pick && engine::Mouse::LeftClick) {
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
		GLint mouseX = (GLint)engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)engine::Mouse::GetYPosition();
		leftClickID_UI = get_id();
		string clickName = getPickedObjectName(leftClickID_UI);

		if (leftClickID_UI == 0)
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