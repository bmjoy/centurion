#include <game>
#include <engine>
#include <player>
#include <surface>
#include <picking>
#include "../interface/game_ui.h"

using namespace glb;
using namespace std;
using namespace glm;
using namespace unit;
using namespace building;

namespace game {

	Game *GAME() { return &mygame; }
	void init() { *GAME() = Game(); }

	bool gameMenuStatus = false;
	bool gameMinimapStatus = false;
	bool gameGridStatus = false;
	int mapWidth = 30000;
	int mapHeight = 20000;
	float cameraMovespeed = 10.f;
	int playersNumber = 1;
	int maxPlayersNumber = 10;
	int currentZoomCamera = 8;
	float zoomCameraFactor = 100.f;
	float townhallRadius = 1875.f;
	

	SelRectPoints *SelRectCoords() { return &selRectCoords; }

	void picking(map<int, Building> *bList, map<int, Unit> *uList, mat4 *proj, mat4 *view, int *clickId, bool *blockMinimap) {
		if (!gameMenuStatus && getBoolean("mouse-left")){
			for (map<int, Building>::iterator bld = (*bList).begin(); bld != (*bList).end(); bld++) {
				bld->second.render(true, 0);
			}
			for (map<int, Unit>::iterator u = (*uList).begin(); u != (*uList).end(); u++) {
				u->second.render(*proj, *view, true, 0);
			}
			if (getBoolean("mouse-left")) {
				*clickId = get_id();
				if (gameMinimapStatus) {
					*blockMinimap = false;
					if (*clickId > 0) {
						*blockMinimap = true;
					}
				}
			}
		}
	}

	void tracing(Surface *s, mat4 *proj, mat4 *view) {
		if (!gameMinimapStatus) {
			unsigned char tracingCol[4];
			(*s).render(true);
			glReadPixels((GLint)getParam("mouse-x-leftclick"), (GLint)getParam("mouse-y-leftclick"), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
			mapgen::mouseZNoise = (mapgen::maxZ - mapgen::minZ) * ((float)tracingCol[0] / 255.0f) + mapgen::minZ;
			clearBuffers();
		}
	}

	void clearBuffers() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void goToPosition(map<int, Building> *bList, Camera *c, double *lastTime, int *clickId, bool *blockMinimap) {
		if (gameMinimapStatus) {
			if (getBoolean("mouse-left") && cursorInGameScreen()) {
				cameraToX = getParam("mouse-x-leftclick") / getParam("window-width")*(float)mapWidth - getParam("window-width-zoomed") / 2.f;
				cameraToY = getYMinimapCoord(getParam("mouse-y-leftclick")) / getParam("window-height")*(float)mapHeight - getParam("window-height-zoomed") / 2.f;
				/* Double Click detection */
				// if you are clicking on a townhall you have to double click 
				// to move the camera there and quit minimap
				if (glfwGetTime() - *lastTime > 1.0f) { clickCount = 0; }
				if (clickCount == 0) {
					clickCount++;
					clickIdList[0] = *clickId;
					*lastTime = glfwGetTime();
					doubleClick = false;
				}
				else if (clickCount == 1) {
					if (glfwGetTime() - *lastTime < 1.0f) {
						clickIdList[1] = *clickId;
						if (clickIdList[1] == clickIdList[0]) {
							doubleClick = true;
						}
						else {
							clickIdList[0] = *clickId;
						}
					}
				}
				if (doubleClick && *clickId > 0) {
					cameraToX = (*bList)[*clickId].get_xPos() - getParam("window-width-zoomed") / 2.f;
					cameraToY = (*bList)[*clickId].get_yPos() - getParam("window-height-zoomed") / 2.f;
					*blockMinimap = false;
				}
				//------------------------------------------------
				if (! (*blockMinimap)) {

					(*c).go_to_pos(cameraToX, cameraToY);
					gameMinimapStatus = false;
				}
			}
		}
	}

	void renderObjects(map<int, Building> *bList, map<int, Unit> *uList, gui::Rectangle *selRectangle, mat4 *proj, mat4 *view, int *clickId, int *selectedUnits) {
		for (map<int, Building>::iterator bld = bList->begin(); bld != bList->end(); bld++) {
			bld->second.render(false, *clickId);
		}
		if (!gameMinimapStatus) {
			for (map<int, Unit>::iterator u = uList->begin(); u != uList->end(); u++) {
				u->second.render(*proj, *view, false, *clickId);
				(*selectedUnits) += (int)u->second.getSelected();
			}
		}
		renderSelRectangle(selRectangle);
	} 


	void renderSelRectangle(gui::Rectangle *selRectangle) {
		if (!gameMinimapStatus) {
			if (getBoolean("mouse-left") && cursorInGameScreen()) {

				float startX = getParam("mouse-x-leftclick") * getParam("window-width-zoomed") / getParam("window-width") + cameraLastX;
				float startY = getParam("mouse-y-leftclick") * getParam("window-height-zoomed") / getParam("window-height") + cameraLastY;
				float lastX = getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position");
				float lastY = getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position");
				if (getParam("mouse-y-position") < getParam("ui-bottom-height")) {
					lastY = getParam("ui-bottom-height")*getParam("window-height-zoomed") / getParam("window-height") + 1.0f + getParam("camera-y-position");
				}
				if (getParam("mouse-y-position") > getParam("window-height") - getParam("ui-top-height")) {
					lastY = getParam("window-height-zoomed") - getParam("ui-top-height")*getParam("window-height-zoomed") / getParam("window-height") - 1.0f + getParam("camera-y-position");
				}

				float w = (lastX - startX);
				float h = (lastY - startY);

				int origin = 0;
				if (w > 0 && h > 0) origin = 0; // bottom-left
				if (w > 0 && h < 0) origin = 1; // top-left
				if (w < 0 && h > 0) origin = 4; // bottom-right
				if (w < 0 && h < 0) origin = 3; // top-right

				if (abs(w) > 2 && abs(h) > 2) {
					selRectangle->render(vec4(255.f), 0, startX, startY, abs(w), abs(h), origin);
					(*SelRectCoords()).minX = std::min(startX, lastX);
					(*SelRectCoords()).maxX = std::max(startX, lastX);
					(*SelRectCoords()).minY = std::min(startY, lastY);
					(*SelRectCoords()).maxY = std::max(startY, lastY);
				}

			}
			else {
				cameraLastX = getParam("camera-x-position");
				cameraLastY = getParam("camera-y-position");
				(*SelRectCoords()).minX = 0;
				(*SelRectCoords()).maxX = 0;
				(*SelRectCoords()).minY = 0;
				(*SelRectCoords()).maxY = 0;
			}
		}
	}
}

