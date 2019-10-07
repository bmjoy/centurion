#include "game_functions.h"

using namespace glb;

namespace game {

	SelRectPoints *SelRectCoords() { return &selRectCoords; }

	void picking(std::map<int, Building> *bList, std::map<int, Unit> *uList, glm::mat4 *proj, glm::mat4 *view, int *clickId, bool *blockMinimap) {
		if (!GAME::MENU_IS_ACTIVE){
			for (std::map<int, Building>::iterator bld = (*bList).begin(); bld != (*bList).end(); bld++) {
				bld->second.render(true, 0);
			}
			for (std::map<int, Unit>::iterator u = (*uList).begin(); u != (*uList).end(); u++) {
				u->second.render(*proj, *view, true, 0);
			}
			if (GLB::MOUSE_LEFT) {
				*clickId = get_id();
				if (GAME::MINIMAP_IS_ACTIVE) {
					*blockMinimap = false;
					if (*clickId > 0) {
						*blockMinimap = true;
					}
				}
			}
		}
	}

	void tracing(Surface *s, glm::mat4 *proj, glm::mat4 *view) {
		if (!GAME::MINIMAP_IS_ACTIVE) {
			unsigned char tracingCol[4];
			(*s).render(true);
			glReadPixels((GLint)getParam("mouse-x-leftclick"), (GLint)getParam("mouse-y-leftclick"), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
			GLB::Z_NOISE = (MAP::MAX_Z - MAP::MIN_Z) * ((float)tracingCol[0] / 255.0f) + MAP::MIN_Z;
			game::clearBuffers();
		}
	}

	void clearBuffers() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void goToPosition(std::map<int, Building> *bList, Camera *c, double *lastTime, int *clickId, bool *blockMinimap) {
		if (GAME::MINIMAP_IS_ACTIVE) {
			if (GLB::MOUSE_LEFT && cursorInGameScreen()) {
				cameraToX = getParam("mouse-x-leftclick") / getParam("window-width")*(float)GAME::MAP_WIDTH - getParam("window-width-zoomed") / 2.f;
				cameraToY = getYMinimapCoord(getParam("mouse-y-leftclick")) / getParam("window-height")*(float)GAME::MAP_HEIGHT - getParam("window-height-zoomed") / 2.f;
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
					GAME::MINIMAP_IS_ACTIVE = false;
				}
				GLB::MOUSE_LEFT = false;
			}
		}
	}

	void renderObjects(std::map<int, Building> *bList, std::map<int, Unit> *uList, gui::Rectangle *selRectangle, glm::mat4 *proj, glm::mat4 *view, int *clickId, int *selectedUnits) {
		for (std::map<int, Building>::iterator bld = bList->begin(); bld != bList->end(); bld++) {
			bld->second.render(false, *clickId);
		}
		if (!GAME::MINIMAP_IS_ACTIVE) {
			for (std::map<int, Unit>::iterator u = uList->begin(); u != uList->end(); u++) {
				u->second.render(*proj, *view, false, *clickId);
				(*selectedUnits) += (int)u->second.getSelected();
			}
		}
		game::renderSelRectangle(selRectangle);
	} 


	void renderSelRectangle(gui::Rectangle *selRectangle) {
		if (!GAME::MINIMAP_IS_ACTIVE) {
			if (GLB::MOUSE_LEFT && cursorInGameScreen()) {

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
					selRectangle->render(glm::vec4(255.f), 0, startX, startY, abs(w), abs(h), origin);
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

