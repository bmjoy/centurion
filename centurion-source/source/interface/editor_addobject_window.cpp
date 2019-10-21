#include <interface>
#include <picking>
#include <engine>
#include <surface>

using namespace glb;
using namespace engine;


namespace editor {

	AddObjectWindow::AddObjectWindow() {}
	
	void AddObjectWindow::create() {
		back_size.x = 790; back_size.y = 260;

		back_image = gui::Image("addobjectwindow_back");
		back_image.create("center", getParam("window-width")/2.f, back_size.y / 2.f, 0, 0, 0);

		object_thumbnail = gui::Image("buildings_rome_rtownhall");
		object_thumbnail.create("center", getParam("window-width") / 2.f, back_size.y / 2.f, 0, 0, 0);

		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = getParam("window-width") / 2.f - back_image.getImageSize().x / 2.f;
		startY = back_size.y / 2.f + back_image.getImageSize().y / 2.f;

		buttons[0] = gui::Image("addobjectwindow_button1");
		buttons[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "AddObjWindow_add");
		increasePickingID();

		buttons[1] = gui::Image("addobjectwindow_button2");
		buttons[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "AddObjWindow_close");
		increasePickingID();

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(getTranslation("EDITOR_newMapButtonClose"), "tahoma_13px", startX + 725.f, startY - 28.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(getTranslation("EDITOR_newMapButtonCreate"), "tahoma_13px", startX + 725.f, startY - 233.f, "center", "middle", vec4(255.f), "bold");

		arrows[0] = gui::Image("addobjectwindow_arrowleft");
		arrows[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "AddObjWindow_leftarrow");
		increasePickingID();

		arrows[1] = gui::Image("addobjectwindow_arrowright");
		arrows[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "AddObjWindow_rightarrow");
		increasePickingID();

		objectForms[0] = gui::FormInput(true);
		pickingIDs[0] = getPickingID();
		objectForms[0].create(startX + 11.f, startY - 11.f, 150.f, 20.f, { "EDITOR_buildings", "EDITOR_units", "EDITOR_decorations" }, pickingIDs[0]);
		addValueToPickingListUI(pickingIDs[0], "AddObjWindow_form0");
		increasePickingID();

		objectForms[1] = gui::FormInput(true);
		pickingIDs[1] = getPickingID();
		objectForms[1].create(startX + 11.f, startY - 11.f - 30.f, 150.f, 20.f, glb::races, pickingIDs[1]);
		addValueToPickingListUI(pickingIDs[1], "AddObjWindow_form1");
		increasePickingID();

		objectForms[2] = gui::FormInput(true);
		pickingIDs[2] = getPickingID();
		objectForms[2].create(startX + 11.f, startY - 11.f - 30.f * 2, 150.f, 20.f, { "" }, pickingIDs[2]);
		addValueToPickingListUI(pickingIDs[2], "AddObjWindow_form2");
		increasePickingID();

		AddObjectWindowUpdateForm2 = true;
	}
	void AddObjectWindow::update() {
		if (AddObjectWindowUpdateForm2) {
			objectForms[2].create(startX + 11.f, startY - 11.f - 30.f * 2, 150.f, 20.f, glb::races, pickingIDs[2]);
		}

		formSelectedTexts[0] = objectForms[0].selectedText.substr(7);
		formSelectedTexts[1] = objectForms[1].selectedText.substr(5);

		cout << formSelectedTexts[0] + "_" + formSelectedTexts[1] << endl;

		AddObjectWindowUpdateForm1and2 = false;
		AddObjectWindowUpdateForm2 = false;
	}
	void AddObjectWindow::render(bool pick) {

		if (AddObjectWindowUpdateForm1and2 || AddObjectWindowUpdateForm2) update();

		if (AddObjectWindowIsOpen) {
			if (pick && getBoolean("mouse-left")) {
				buttons[0].render(true);
				buttons[1].render(true);
				arrows[0].render(true);
				arrows[1].render(true);
				for (int j = 2; j >= 0; j--)
					objectForms[j].render(true, glm::vec4());
				picking();
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
	}
	void AddObjectWindow::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		leftClickID = get_id();
		string clickName = getPickedObjectName(leftClickID);

		if (leftClickID == 0) {
			for (int j = 0; j < 3; j++)
				objectForms[j].close();
		}

		for (int j = 0; j < 3; j++){
			if (clickName == "AddObjWindow_form" + to_string(j)) {
				int i = objectForms[j].get_clicked_option();
				objectForms[j].open_close();
				if (i > 0) objectForms[j].select_option(i);
				for (int k = 0; k < 3; k++)
					if (k != j)
						objectForms[k].close();
			}
		}		
	}

	AddObjectWindow::~AddObjectWindow() {}
};
