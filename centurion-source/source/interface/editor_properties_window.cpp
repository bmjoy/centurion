#include <interface>
#include <picking>
#include <engine>
#include <surface>
#include <game>

using namespace glb;
using namespace engine;

namespace editor {

	PropertiesWindow::PropertiesWindow() {}
	
	void PropertiesWindow::create() {
		back_image = gui::Image("propertieswindow_back");
		back_image.create("center", settings.GetWindowWidth()/2.f, settings.GetWindowHeight()/2.f, 0, 0, 0);

		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = settings.GetWindowWidth() / 2.f - back_image.getImageSize().x / 2.f;
		startY = settings.GetWindowHeight() / 2.f + back_image.getImageSize().y / 2.f;

		buttons[0] = gui::Image("propertieswindow_buttonleft");
		buttons[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "PropertiesWindow_cancel");
		increasePickingID();

		buttons[1] = gui::Image("propertieswindow_buttonright");
		buttons[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "PropertiesWindow_apply");
		increasePickingID();

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(getTranslation("EDITOR_propertiesCancel"), "tahoma_13px", startX + 63.f, startY - 365.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(getTranslation("EDITOR_propertiesApply"), "tahoma_13px", startX + 492.f, startY - 365.f, "center", "middle", vec4(255.f), "bold");

		textinput_back[0] = gui::Image("propertieswindow_textinput_1");
		textinput_back[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "PropertiesWindow_name");
		increasePickingID();

		textinput_back[1] = gui::Image("propertieswindow_textinput_2");
		textinput_back[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "PropertiesWindow_settl_name");
		increasePickingID();

		text[0] = gui::SimpleText("static");
		text[0].create_static(getTranslation("EDITOR_properties"), "tahoma_15px", startX + 15.f, startY - 25.f, "left", "normal", vec4(255), "bold");
		text[1] = gui::SimpleText("static");
		text[1].create_static(getTranslation("EDITOR_propertiesName"), "tahoma_15px", startX + 15.f, startY - 86.f, "left", "normal", vec4(255), "normal");
		text[2] = gui::SimpleText("static");
		text[2].create_static(getTranslation("EDITOR_propertiesSettlement"), "tahoma_15px", startX + 15.f, startY - 130.f, "left", "normal", vec4(255), "normal");	

		currentMapName = getTranslation("EDITOR_defaultMapName");
		text_input[0].create("", startX + 246.f, startY - 85.f, 30);
		text_input[1].create("", startX + 246.f, startY - 129.f, 30);
		
		text_input[0].active();
		PropertiesWindowIsOpen = false;	
	}

	void PropertiesWindow::render(bool pick) {
		if (!IsWindowOpened && !PropertiesWindowIsOpen && !addingObject){
			if (getBoolean("mouse-right") && (game::buildings.count(rightClickID) > 0)) {
				if (game::buildings[rightClickID].isSelected()){
					type = "building";
					building_pointer = &game::buildings[rightClickID];
					text_input[0].create(building_pointer->get_name(), startX + 246.f, startY - 85.f, 30);
					text_input[1].create(building_pointer->get_settlement_name(), startX + 246.f, startY - 129.f, 30);
					
					PropertiesWindowIsOpen = true;
					IsWindowOpened = true;
					AddObjectWindowIsOpen = false;
				}
 			}
		}

		if (PropertiesWindowIsOpen) {
			if (pick && getBoolean("mouse-left")) {
				buttons[0].render(true);
				buttons[1].render(true);
				textinput_back[0].render(true);
				textinput_back[1].render(true);
				picking();
			}
			if (!pick) {
				back_image.render(false);
				for (int i = 0; i < 2; i++) buttons[i].render(false);
				for (int i = 0; i < 2; i++) buttons_text[i].render_static();
				for (int i = 0; i < 3; i++) text[i].render_static();
				text_input[0].render();
				text_input[1].render();
			}
		}	
	}

	

	PropertiesWindow::~PropertiesWindow() {}
};
