#include <interface>
#include <picking>
#include <surface>
#include <game/editor.h>
#include <engine/window.h>
#include <engine/mouse.h>

using namespace glb;

namespace editor {

	AddObjectWindow::AddObjectWindow() {}
	
	void AddObjectWindow::create() {
		back_size.x = 790; back_size.y = 260;

		back_image = gui::Image("addobjectwindow_back");
		back_image.create("center", engine::myWindow::Width/2.f, back_size.y / 2.f, 0, 0, getPickingID_UI());
		increasePickingID_UI();


		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = engine::myWindow::Width / 2.f - back_image.getImageSize().x / 2.f;
		startY = back_size.y / 2.f + back_image.getImageSize().y / 2.f;

		buttons[0] = gui::Image("addobjectwindow_button1");
		buttons[0].create("top-left", startX, startY, 0, 0, getPickingID_UI());
		addValueToPickingListUI(getPickingID_UI(), "AddObjWindow_add");
		increasePickingID_UI();

		buttons[1] = gui::Image("addobjectwindow_button2");
		buttons[1].create("top-left", startX, startY, 0, 0, getPickingID_UI());
		addValueToPickingListUI(getPickingID_UI(), "AddObjWindow_close");
		increasePickingID_UI();

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(getTranslation("EDITOR_addObjectButtonAdd"), "tahoma_13px", startX + 725.f, startY - 28.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(getTranslation("EDITOR_newMapButtonClose"), "tahoma_13px", startX + 725.f, startY - 233.f, "center", "middle", vec4(255.f), "bold");

		arrows[0] = gui::Image("addobjectwindow_arrowleft");
		arrows[0].create("top-left", startX, startY, 0, 0, getPickingID_UI());
		addValueToPickingListUI(getPickingID_UI(), "AddObjWindow_leftarrow");
		increasePickingID_UI();

		arrows[1] = gui::Image("addobjectwindow_arrowright");
		arrows[1].create("top-left", startX, startY, 0, 0, getPickingID_UI());
		addValueToPickingListUI(getPickingID_UI(), "AddObjWindow_rightarrow");
		increasePickingID_UI();

		objectForms[0] = gui::FormInput(true);
		pickingIDs[0] = getPickingID_UI();
		objectForms[0].create(startX + 11.f, startY - 11.f, 150.f, 20.f, { "EDITOR_buildings", "EDITOR_decorations" }, pickingIDs[0]);
		addValueToPickingListUI(pickingIDs[0], "AddObjWindow_form0");
		increasePickingID_UI();

		objectForms[1] = gui::FormInput(true);
		pickingIDs[1] = getPickingID_UI();
		vector<string> form1Options = EditorAddObjectBuildingOptions;
		sort(form1Options.begin(), form1Options.end());
		form1Options.erase(unique(form1Options.begin(), form1Options.end()), form1Options.end());
		objectForms[1].create(startX + 11.f, startY - 11.f - 30.f, 150.f, 20.f, form1Options, pickingIDs[1]);
		addValueToPickingListUI(pickingIDs[1], "AddObjWindow_form1");
		increasePickingID_UI();

		objectForms[2] = gui::FormInput(true);
		pickingIDs[2] = getPickingID_UI();
		objectForms[2].create(startX + 11.f, startY - 11.f - 30.f * 2, 150.f, 20.f, { "" }, pickingIDs[2]);
		addValueToPickingListUI(pickingIDs[2], "AddObjWindow_form2");
		increasePickingID_UI();

		AddObjectWindowUpdateForm2 = true;
		addingObject = false;
	}

	void AddObjectWindow::update() {
		formSelectedTexts[0] = objectForms[0].selectedText.substr(7); // buildings/units/decoration

		if (AddObjectWindowUpdateForm1and2) {
			vector<string> form1Options;
			for (int i = 0; i < NumberOfObjects; i++) {
				if ((EditorObjectStringListForm0[i] == formSelectedTexts[0])){
					if (formSelectedTexts[0] == "buildings") form1Options = EditorAddObjectBuildingOptions;
					if (formSelectedTexts[0] == "units") form1Options = EditorAddObjectUnitOptions;
					if (formSelectedTexts[0] == "decorations") form1Options = EditorAddObjectDecorationOptions;
				}
			}
			if (form1Options.size() > 0){
				sort(form1Options.begin(), form1Options.end());
				form1Options.erase(unique(form1Options.begin(), form1Options.end()), form1Options.end());
			}
			else form1Options = { "" };

			objectForms[1].create(startX + 11.f, startY - 11.f - 30.f, 150.f, 20.f, form1Options, pickingIDs[1]);
			objectForms[1].selectedText = form1Options[0];
			AddObjectWindowUpdateForm1and2 = false;
			AddObjectWindowUpdateForm2 = true;
		}

		formSelectedTexts[1] = objectForms[1].selectedText.substr(20); // rome,egypt,...

		if (AddObjectWindowUpdateForm2) {
			vector<string> form2Options;
			for (int i = 0; i < NumberOfObjects; i++) {
				if ((EditorObjectStringListForm0[i] == formSelectedTexts[0]) && (EditorObjectStringListForm1[i] == formSelectedTexts[1]))
					form2Options.push_back("WORD_" + EditorObjectStringListForm2[i]);
			}
			if (form2Options.size() == 0)
				form2Options = { "" };
			objectForms[2].create(startX + 11.f, startY - 11.f - 30.f * 2, 150.f, 20.f, form2Options, pickingIDs[2]);
			AddObjectWindowUpdateForm2 = false;
		}

		formSelectedTexts[2] = objectForms[2].selectedText.substr(5); // rtownhall,...

		selectedObject = formSelectedTexts[0] + "_" + formSelectedTexts[1] + "_" + formSelectedTexts[2];
		object_thumbnail = gui::Image(selectedObject);
		object_thumbnail.create("center", engine::myWindow::Width / 2.f, back_size.y / 2.f, 0, 0, 0);

		prepareObject(formSelectedTexts[0], formSelectedTexts[2]);
	}

	void AddObjectWindow::render(bool pick) {

		if (AddObjectWindowUpdateForm1and2 || AddObjectWindowUpdateForm2) update();

		if (AddObjectWindowIsOpen) {

			if (pick && engine::Mouse::LeftClick && !addingObject) {
				back_image.render(true);
				buttons[0].render(true);
				buttons[1].render(true);
				arrows[0].render(true);
				arrows[1].render(true);
				for (int j = 2; j >= 0; j--)
					objectForms[j].render(true, glm::vec4());
				picking();
			}

			if (addingObject) {
				insertingObject(formSelectedTexts[0], formSelectedTexts[2]);

				if (engine::Mouse::LeftClick) {
					addObject(formSelectedTexts[0]);
				}
			}

			if (!pick) {
				back_image.render(false);
				object_thumbnail.render(false);
				buttons[0].render(false);
				buttons[1].render(false);
				arrows[0].render(false);
				arrows[1].render(false);
				for (int j = 2; j >= 0; j--)
					objectForms[j].render(false, vec4(60.f, 68.f, 104.f, 255.f));
				buttons_text[0].render_static();
				buttons_text[1].render_static();				
			}
		}	

		if (engine::Mouse::RightClick) addingObject = false;
	}

	AddObjectWindow::~AddObjectWindow() {}
};
