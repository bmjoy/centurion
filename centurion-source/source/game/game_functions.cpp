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
using namespace engine;
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
	bool selRectangleIsActive = false;

	map<int, Building> buildings = { };
	map<int, Unit> units = { };
	vector<Unit> selectedUnits = { };

	vector<Player> playersList;

	SelRectPoints *SelRectCoords() { return &selRectCoords; }

	void picking(int *clickId, bool *blockMinimap) {
		if (!gameMenuStatus && getBoolean("mouse-left") && !selRectangleIsActive){
			for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
				bld->second.render(true, 0);
			}
			for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
				u->second.render(true, 0);
			}	
			*clickId = get_id();
			if (gameMinimapStatus) {					
				*blockMinimap = false;
				if (*clickId > 0) {
					*blockMinimap = true;
				}
			}
		}
	}

	void tracing(Surface *s) {
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

	void goToPosition(double *lastTime, int clickId, bool *blockMinimap) {
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
					clickIdList[0] = clickId;
					*lastTime = glfwGetTime();
					doubleClick = false;
				}
				else if (clickCount == 1) {
					if (glfwGetTime() - *lastTime < 1.0f) {
						clickIdList[1] = clickId;
						if (clickIdList[1] == clickIdList[0]) {
							doubleClick = true;
						}
						else {
							clickIdList[0] = clickId;
						}
					}
				}
				if (doubleClick && clickId > 0) {
					cameraToX = buildings[clickId].get_xPos() - getParam("window-width-zoomed") / 2.f;
					cameraToY = buildings[clickId].get_yPos() - getParam("window-height-zoomed") / 2.f;
					*blockMinimap = false;
				}
				//------------------------------------------------
				if (! (*blockMinimap)) {
					CAMERA()->go_to_pos(cameraToX, cameraToY);
					gameMinimapStatus = false;
					setBoolean("mouse-left", false);
					setBoolean("mouse-left-pressed", false);
				}
			}
		}
	}

	void renderObjects(int clickId) {
		for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
			bld->second.render(false, clickId);
		}
		if (!gameMinimapStatus) {
			for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
				u->second.render(false, clickId);
				//if (u->second.isSelected())	  selectedUnits.push_back(u->second);
			}
		}
		renderSelRectangle();
	} 


	void renderSelRectangle() {
		if (!gameMinimapStatus) {
			if (getBoolean("mouse-left-pressed")) {
				if (!selRectangleIsActive){
					cout << "DEBUG: Selection rectangle enabled.\n";
					(*SelRectCoords()).startX = getParam("mouse-x-leftclick") * getParam("window-width-zoomed") / getParam("window-width") + cameraLastX;
					(*SelRectCoords()).startY = getParam("mouse-y-leftclick") * getParam("window-height-zoomed") / getParam("window-height") + cameraLastY;
				}
				(*SelRectCoords()).lastX = getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position");
				(*SelRectCoords()).lastY = getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position");
				if (getParam("mouse-y-position") < getParam("ui-bottom-height")) {
					(*SelRectCoords()).lastY = getParam("ui-bottom-height")*getParam("window-height-zoomed") / getParam("window-height") + 1.0f + getParam("camera-y-position");
				}
				if (getParam("mouse-y-position") > getParam("window-height") - getParam("ui-top-height")) {
					(*SelRectCoords()).lastY = getParam("window-height-zoomed") - getParam("ui-top-height")*getParam("window-height-zoomed") / getParam("window-height") - 1.0f + getParam("camera-y-position");
				}

				float w = ((*SelRectCoords()).lastX - (*SelRectCoords()).startX);
				float h = ((*SelRectCoords()).lastY - (*SelRectCoords()).startY);

				int origin = 0;
				if (w > 0 && h > 0) origin = 0; // bottom-left
				if (w > 0 && h < 0) origin = 1; // top-left
				if (w < 0 && h > 0) origin = 4; // bottom-right
				if (w < 0 && h < 0) origin = 3; // top-right

				if (abs(w) > 1 && abs(h) > 1){
					GAME()->selRectangle.render(vec4(255.f), 0, (*SelRectCoords()).startX, (*SelRectCoords()).startY, abs(w), abs(h), origin);
					(*SelRectCoords()).minX = std::min((*SelRectCoords()).startX, (*SelRectCoords()).lastX);
					(*SelRectCoords()).maxX = std::max((*SelRectCoords()).startX, (*SelRectCoords()).lastX);
					(*SelRectCoords()).minY = std::min((*SelRectCoords()).startY, (*SelRectCoords()).lastY);
					(*SelRectCoords()).maxY = std::max((*SelRectCoords()).startY, (*SelRectCoords()).lastY);
				}
				else {
					(*SelRectCoords()).minX = -0.1f;
					(*SelRectCoords()).maxX = -0.1f;
					(*SelRectCoords()).minY = -0.1f;
					(*SelRectCoords()).maxY = -0.1f;
				}
				
				selRectangleIsActive = true;
			}
			else {
				if (selRectangleIsActive) cout << "DEBUG: Selection rectangle disabled.\n";
				cameraLastX = getParam("camera-x-position");
				cameraLastY = getParam("camera-y-position");
				(*SelRectCoords()).startX = -0.1f;
				(*SelRectCoords()).startY = -0.1f;
				(*SelRectCoords()).lastX = -0.1f;
				(*SelRectCoords()).lastY = -0.1f;
				(*SelRectCoords()).minX = -0.1f;
				(*SelRectCoords()).maxX = -0.1f;
				(*SelRectCoords()).minY = -0.1f;
				(*SelRectCoords()).maxY = -0.1f;
				selRectangleIsActive = false;
			}
		}
	}

	void setMinimapProjection() {
		float bottom = (-1.f)*(mapHeight * getParam("ui-bottom-height") / (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height")));
		float top = mapHeight + mapHeight * getParam("ui-top-height") / (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height"));
		float left = 0.f;
		float right = (float)mapWidth;
		glb::minimapProjection = ortho(left, right, bottom, top, -right, right);
	}
}

