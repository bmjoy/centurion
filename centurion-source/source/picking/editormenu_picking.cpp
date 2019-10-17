#include <interface>
#include <picking>
#include <engine>

using namespace glb;
using namespace engine;

namespace editor {

    void EditorMenu::picking() {

        GLint mouseX = (GLint)getParam("mouse-x-position");
        GLint mouseY = (GLint)getParam("mouse-y-position");
		int clickId = get_id();
        int pos = (int)((mouseY - getParam("window-height"))*(-1)) / titles["File"].titleHeight;

        //---------------------
        //   RESET 
        //---------------------

        if (clickId < minPickingID || clickId > maxPickingID) {
            for (int i = 0; i < titles.size(); i++) {
                std::string s = titlesList[i];
                titles[s].isOpened = false;
                menuIsOpened = false;
            }
        }

        //---------------------
        //    FILE
        //---------------------

        if (pickingList["File"] == clickId) {
            if (pos == 0) {
                titles["File"].isOpened = !titles["File"].isOpened;
                menuIsOpened = !menuIsOpened;
                for (int j = 0; j < titles.size(); j++) {
                    std::string s = titlesList[j];
                    if (s != "File") titles[s].isOpened = false;
                }
            }
            //---------------------
            //    SAVE

            if (pos == 1) { // save

				saveCurrentScenario("editor_map");

				for (int i = 0; i < titles.size(); i++) {
					std::string s = titlesList[i];
					titles[s].isOpened = false;
				}
				menuIsOpened = false;
            }
            //---------------------
            //    EXIT

            if (pos == 2) { // exit				
				ENGINE()->Reset();
				for (int i = 0; i < titles.size(); i++) {
					std::string s = titlesList[i];
					titles[s].isOpened = false;					
				}
				menuIsOpened = false;
            }					
        }

        //---------------------
        //    EDIT
        //---------------------

        if (pickingList["Edit"] == clickId) {
            titles["Edit"].isOpened = !titles["Edit"].isOpened;
            menuIsOpened = !menuIsOpened;
            for (int j = 0; j < titles.size(); j++) {
                std::string s = titlesList[j];
                if (s != "Edit") titles[s].isOpened = false;
            }
        }

		//---------------------
		//    TOOLS
		//---------------------

		if (pickingList["Tools"] == clickId) {
			titles["Tools"].isOpened = !titles["Tools"].isOpened;
			menuIsOpened = !menuIsOpened;
			for (int j = 0; j < titles.size(); j++) {
				std::string s = titlesList[j];
				if (s != "Tools") titles[s].isOpened = false;
			}
		}

        //---------------------
        //---------------------
    }

    bool EditorMenu::isHover(glm::vec2 pos, int w, int h) {

        if ((getParam("mouse-x-position") > pos.x) && (getParam("mouse-x-position") < pos.x + w) && (getParam("mouse-y-position") > pos.y) && (getParam("mouse-y-position") < pos.y + h)) {
            return true;
        }
        else {
            return false;
        }
    }
};
