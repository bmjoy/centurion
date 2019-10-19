#include <interface>
#include <picking>
#include <engine>

using namespace glb;
using namespace engine;

namespace editor {

	EditorMenu::EditorMenu(){
        barHeight = 30;
        barColor = glm::vec4(60.f, 68.f, 104.f, 255.f);
        titlesList = { "File", "Edit", "Tools" };
        menuIsOpened = false;
		maxPickingID = 0;
		minPickingID = 0;
    }

    void EditorMenu::create() {
		minPickingID = getPickingID();

        topBar = gui::Rectangle();
        topBar.create("filled", 0.0f, (float)-barHeight, getParam("window-width"), (float)barHeight, "bottom-left", 0);

        titles["File"].title = "File";
        titles["File"].options = { "New", "Open", "Save", "Exit" };

        titles["Edit"].title = "Edit";
        titles["Edit"].options = { "Edit1", "Edit2", "Edit3" };

		titles["Tools"].title = "Tools";
		titles["Tools"].options = { "Tools1", "Tools2", "Tools3" };

        for (int i = 0; i < titles.size(); i++) {
            std::string s = titlesList[i];
            /* title */
            titles[s].pickingID = getPickingID();
            titles[s].pickingColor = glm::vec4(getPickingColorFromID(titles[s].pickingID), 255.f);
            pickingList[titles[s].title] = titles[s].pickingID;

            titles[s].color = barColor;
            titles[s].hoverColor = glm::vec4(40.f, 46.f, 70.f, 255.f);

            titles[s].titleWidth = 75;
            titles[s].titleHeight = barHeight;
            titles[s].titlePosition = glm::ivec2(titles[s].titleWidth * i, getParam("window-height") - titles[s].titleHeight);
            titles[s].titleBack = gui::Rectangle();
            
            titles[s].titleBack = gui::Rectangle();
            titles[s].titleBack.create("filled", (float)titles[s].titlePosition.x, (float)titles[s].titlePosition.y, (float)titles[s].titleWidth, (float)titles[s].titleHeight, "bottom-left", titles[s].pickingID);
            titles[s].titleText = gui::SimpleText("static");
            titles[s].titleText.create_static(getTranslation("EDITOR_" + titles[s].title), "tahoma_13px", titles[s].titlePosition.x + 0.5f * titles[s].titleWidth, titles[s].titlePosition.y + 0.5f * titles[s].titleHeight, "center", "middle", glm::vec4(255.f), "bold");
            

            /* options */
            titles[s].optionsWidth = 100;
            titles[s].optionsHeight = barHeight;
            titles[s].optionsOffsetX = 20;
            for (int j = 0; j < titles[s].options.size(); j++) {
                titles[s].optionsPosition.push_back(glm::ivec2(titles[s].titleWidth * i, getParam("window-height") - titles[s].optionsHeight * (j + 2)));

                gui::SimpleText tempText = gui::SimpleText("static");
                tempText.create_static(getTranslation("EDITOR_" + titles[s].title + titles[s].options[j]), "tahoma_13px", titles[s].optionsPosition[j].x + 0.5f * titles[s].optionsWidth - titles[s].optionsOffsetX, titles[s].optionsPosition[j].y + 0.5f * titles[s].optionsHeight, "left", "middle", glm::vec4(255.f), "bold");
                titles[s].optionsText.push_back(tempText);

                gui::Rectangle tempRect = gui::Rectangle();
                tempRect.create("filled", (float)titles[s].optionsPosition[j].x, (float)titles[s].optionsPosition[j].y, (float)titles[s].optionsWidth, (float)titles[s].optionsHeight, "bottom-left", titles[s].pickingID);
                titles[s].optionsBack.push_back(tempRect);
            }

            titles[s].isOpened = false;

			increasePickingID();
        }
		maxPickingID = getPickingID();
    }

    void EditorMenu::render(bool pick) {
        
        /* picking rendering */
        if (pick && getBoolean("mouse-left")){
            for (int i = 0; i < titles.size(); i++) {
                std::string s = titlesList[i];
				titles[s].titleBack.render(titles[s].pickingColor);
                if (titles[s].isOpened) {
                    for (int j = 0; j < titles[s].options.size(); j++) {
                        titles[s].optionsBack[j].render(titles[s].pickingColor);
                    }
                }
            }

            picking(); // --> source/picking/editormenu_picking.cpp
        }

        /* normal rendering */
        else {
            topBar.render(barColor);
            for (int i = 0; i < titles.size(); i++) {
                std::string s = titlesList[i];
                if (isHover(titles[s].titlePosition, titles[s].titleWidth, titles[s].titleHeight)) {
					if (!IsWindowOpened) { titles[s].titleBack.render(titles[s].hoverColor); }
                    if (menuIsOpened) {
                        titles[s].isOpened = true;
                        for (int j = 0; j < titles.size(); j++) {
                            std::string s1 = titlesList[j];
                            if (s1 != titles[s].title) {
                                titles[s1].isOpened = false;
                            }
                        }
                    }
                }
                else {
                    titles[s].titleBack.render(titles[s].color);
                }
            
                titles[s].titleText.render_static();

                if (titles[s].isOpened) {
                    for (int j = 0; j < titles[s].options.size(); j++) {
                        if (isHover(titles[s].optionsPosition[j], titles[s].optionsWidth, titles[s].optionsHeight)) {
                            titles[s].optionsBack[j].render(titles[s].hoverColor);
                        }
                        else {
                            titles[s].optionsBack[j].render(titles[s].color);
                        }
                        titles[s].optionsText[j].render_static();
                    }
                }
            }
        }
    }

	EditorMenu::~EditorMenu(){
    }
};
