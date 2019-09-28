#include "game_functions.h"

void game::picking(std::map<int, Building> *bList, std::map<int, Unit> *uList, glm::mat4 *proj, glm::mat4 *view, int *clickId, bool *blockMinimap) {
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

void game::tracing(Surface *s, glm::mat4 *proj, glm::mat4 *view) {
	if (!GAME::MINIMAP_IS_ACTIVE) {
		unsigned char tracingCol[4];
		(*s).render(*proj, *view, true);
		glReadPixels(GLB::MOUSE_LEFT_X, GLB::MOUSE_LEFT_Y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
		GLB::Z_NOISE = (MAP::MAX_Z - MAP::MIN_Z) * ((float)tracingCol[0] / 255.0) + MAP::MIN_Z;
		game::clearBuffers();
	}
}

void game::clearBuffers() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void game::goToPosition(std::map<int, Building> *bList, Camera *c, double *lastTime, int *clickId, bool *blockMinimap) {
	if (GAME::MINIMAP_IS_ACTIVE) {
		if (GLB::MOUSE_LEFT && cursorInGameScreen()) {
			cameraToX = GLB::MOUSE_LEFT_X / (float)GLB::WINDOW_WIDTH*(float)GAME::MAP_WIDTH - (float)GLB::WINDOW_WIDTH / 2.0;
			cameraToY = getYMinimapCoord((float)GLB::MOUSE_LEFT_Y) / (float)GLB::WINDOW_HEIGHT*(float)GAME::MAP_HEIGHT - (float)GLB::WINDOW_HEIGHT / 2.0;
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
				cameraToX = (*bList)[*clickId].get_xPos() - GLB::WINDOW_WIDTH_ZOOMED / 2;
				cameraToY = (*bList)[*clickId].get_yPos() - GLB::WINDOW_HEIGHT_ZOOMED / 2;
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

void game::renderObjects(std::map<int, Building> *bList, std::map<int, Unit> *uList, gui::Rectangle *selRectangle, glm::mat4 *proj, glm::mat4 *view, int *clickId, int *selectedUnits) {
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


void game::renderSelRectangle(gui::Rectangle *selRectangle) {
	if (!GAME::MINIMAP_IS_ACTIVE) {
		if (GLB::MOUSE_LEFT && cursorInGameScreen()) {

			float startX = (float)GLB::MOUSE_LEFT_X * GLB::WINDOW_WIDTH_ZOOMED / GLB::WINDOW_WIDTH + cameraLastX;
			float startY = (float)GLB::MOUSE_LEFT_Y * GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT + cameraLastY;
			float lastX = (float)GLB::MOUSE_X * GLB::WINDOW_WIDTH_ZOOMED / GLB::WINDOW_WIDTH + GAME::CAMERA_POS_X;
			float lastY = (float)GLB::MOUSE_Y * GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT + GAME::CAMERA_POS_Y;
			if (GLB::MOUSE_Y < GAME::UI_BOTTOM_HEIGHT) {
				lastY = (float)GAME::UI_BOTTOM_HEIGHT*GLB::WINDOW_HEIGHT_ZOOMED/GLB::WINDOW_HEIGHT + 1.0f + GAME::CAMERA_POS_Y;
			}
			if (GLB::MOUSE_Y > GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT) {
				lastY = GLB::WINDOW_HEIGHT_ZOOMED - (float)GAME::UI_TOP_HEIGHT*GLB::WINDOW_HEIGHT_ZOOMED/GLB::WINDOW_HEIGHT - 1.0f + GAME::CAMERA_POS_Y;
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
				GLB::SEL_RECT_COORDS.minX = std::min(startX, lastX);
				GLB::SEL_RECT_COORDS.maxX = std::max(startX, lastX);
				GLB::SEL_RECT_COORDS.minY = std::min(startY, lastY);
				GLB::SEL_RECT_COORDS.maxY = std::max(startY, lastY);
			}

		}
		else {
			cameraLastX = GAME::CAMERA_POS_X;
			cameraLastY = GAME::CAMERA_POS_Y;
			GLB::SEL_RECT_COORDS.minX = 0;
			GLB::SEL_RECT_COORDS.maxX = 0;
			GLB::SEL_RECT_COORDS.minY = 0;
			GLB::SEL_RECT_COORDS.maxY = 0;
		}
	}
}

