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
		back_image.create("center", getParam("window-width")/2.f, getParam("window-height")/2.f, 0, 0, 0);

		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = getParam("window-width") / 2.f - back_image.getImageSize().x / 2.f;
		startY = getParam("window-height") / 2.f + back_image.getImageSize().y / 2.f;

		buttons[0] = gui::Image("propertieswindow_buttonleft");
		buttons[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "PropertiesWindow_abort");
		increasePickingID();

		buttons[1] = gui::Image("propertieswindow_buttonright");
		buttons[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "PropertiesWindow_apply");
		increasePickingID();

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(getTranslation("EDITOR_propertiesAbort"), "tahoma_13px", startX + 63.f, startY - 365.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(getTranslation("EDITOR_propertiesApply"), "tahoma_13px", startX + 492.f, startY - 365.f, "center", "middle", vec4(255.f), "bold");

		textinput_back = gui::Image("propertieswindow_textinput");
		textinput_back.create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "PropertiesWindow_textclick");
		increasePickingID();

		text[0] = gui::SimpleText("static");
		text[0].create_static(getTranslation("EDITOR_properties"), "tahoma_15px", startX + 15.f, startY - 25.f, "left", "normal", vec4(255), "bold");
		text[1] = gui::SimpleText("static");
		text[1].create_static(getTranslation("EDITOR_propertiesName"), "tahoma_15px", startX + 15.f, startY - 86.f, "left", "normal", vec4(255), "normal");

		currentMapName = getTranslation("EDITOR_defaultMapName");
		text_input.create("", startX + 246.f, startY - 85.f, 30);
		
		text_input.active();
		PropertiesWindowIsOpen = false;	
	}

	void PropertiesWindow::render(bool pick) {
		if (!IsWindowOpened && !PropertiesWindowIsOpen){
			if (getBoolean("mouse-right") && (game::buildings.count(rightClickID) > 0)) {
				if (game::buildings[rightClickID].isSelected()){
					type = "building";
					building_pointer = &game::buildings[rightClickID];
					text_input.create(building_pointer->get_name(), startX + 246.f, startY - 85.f, 30);
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
				textinput_back.render(true);
				picking();
			}
			if (!pick) {
				back_image.render(false);
				buttons[0].render(false);
				buttons[1].render(false);
				buttons_text[0].render_static();
				buttons_text[1].render_static();				
				text[0].render_static();
				text[1].render_static();
				text_input.render();
			}
		}	
	}

	

	PropertiesWindow::~PropertiesWindow() {}
};
