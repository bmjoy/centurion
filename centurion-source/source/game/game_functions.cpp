#include "game_functions.h"

void game::picking(std::map<int, Building> *bList, std::map<int, Unit> *uList, glm::mat4 *proj, glm::mat4 *view, int *clickId, bool *blockMinimap) {
	for (std::map<int, Building>::iterator bld = (*bList).begin(); bld != (*bList).end(); bld++) {
		bld->second.render(*proj, *view, true, 0);
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

void game::renderObjects(std::map<int, Building> *bList, std::map<int, Unit> *uList, glm::mat4 *proj, glm::mat4 *view, int *clickId, int *selectedUnits) {
	if (!GAME::GRID_IS_ACTIVE) {
		for (std::map<int, Building>::iterator bld = (*bList).begin(); bld != (*bList).end(); bld++) {
			bld->second.render(*proj, *view, false, *clickId);
		}
		if (!GAME::MINIMAP_IS_ACTIVE) {
			for (std::map<int, Unit>::iterator u = (*uList).begin(); u != (*uList).end(); u++) {
				u->second.render(*proj, *view, false, *clickId);
				(*selectedUnits) += (int)u->second.getSelected();
			}
		}
	}
}

void game::renderSelRectangle(EmptyRectangle *rect, std::array<float, 8> *coords, glm::mat4 *view, float *cameraLastX, float *cameraLastY) {
	if (!GAME::MINIMAP_IS_ACTIVE) {
		if (GLB::MOUSE_LEFT && cursorInGameScreen()) {
			(*coords)[0] = GLB::MOUSE_LEFT_X * (float)GLB::WINDOW_WIDTH_ZOOMED / (float)GLB::WINDOW_WIDTH + *cameraLastX;
			(*coords)[1] = GLB::MOUSE_LEFT_Y * (float)GLB::WINDOW_HEIGHT_ZOOMED / (float)GLB::WINDOW_HEIGHT + *cameraLastY;
			(*coords)[2] = GLB::MOUSE_LEFT_X * (float)GLB::WINDOW_WIDTH_ZOOMED / (float)GLB::WINDOW_WIDTH + *cameraLastX;
			(*coords)[3] = GLB::MOUSE_Y * (float)GLB::WINDOW_HEIGHT_ZOOMED / (float)GLB::WINDOW_HEIGHT + GAME::CAMERA_POS_Y;
			(*coords)[4] = GLB::MOUSE_X * (float)GLB::WINDOW_WIDTH_ZOOMED / (float)GLB::WINDOW_WIDTH + GAME::CAMERA_POS_X;
			(*coords)[5] = GLB::MOUSE_Y * (float)GLB::WINDOW_HEIGHT_ZOOMED / (float)GLB::WINDOW_HEIGHT + GAME::CAMERA_POS_Y;
			(*coords)[6] = GLB::MOUSE_X * (float)GLB::WINDOW_WIDTH_ZOOMED / (float)GLB::WINDOW_WIDTH + GAME::CAMERA_POS_X;
			(*coords)[7] = GLB::MOUSE_LEFT_Y * (float)GLB::WINDOW_HEIGHT_ZOOMED / (float)GLB::WINDOW_HEIGHT + *cameraLastY;
			if (GLB::MOUSE_Y < GAME::UI_BOTTOM_HEIGHT) {
				(*coords)[3] = GAME::UI_BOTTOM_HEIGHT + 1.0f + GAME::CAMERA_POS_Y;
				(*coords)[5] = GAME::UI_BOTTOM_HEIGHT + 1.0f + GAME::CAMERA_POS_Y;
			}
			if (GLB::MOUSE_Y > GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT) {
				(*coords)[3] = GLB::WINDOW_HEIGHT_ZOOMED - GAME::UI_TOP_HEIGHT - 1.0f + GAME::CAMERA_POS_Y;
				(*coords)[5] = GLB::WINDOW_HEIGHT_ZOOMED - GAME::UI_TOP_HEIGHT - 1.0f + GAME::CAMERA_POS_Y;
			}
			GLB::SEL_RECT_COORDS.minX = std::min((*coords)[0], (*coords)[4]);
			GLB::SEL_RECT_COORDS.maxX = std::max((*coords)[0], (*coords)[4]);
			GLB::SEL_RECT_COORDS.minY = std::min((*coords)[1], (*coords)[3]);
			GLB::SEL_RECT_COORDS.maxY = std::max((*coords)[1], (*coords)[3]);
			(*rect).apply_projection_matrix(GLB::CAMERA_PROJECTION);
			(*rect).create(*coords);
			(*rect).render(*view, glm::mat4(1.0f));
		}
		else {
			*cameraLastX = GAME::CAMERA_POS_X;
			*cameraLastY = GAME::CAMERA_POS_Y;
		}
	}
}

void game::renderMapRectangle(EmptyRectangle *rect, std::array<float, 8> *coords) {
	if (GAME::MINIMAP_IS_ACTIVE) {
		/* minimap rectangle coordinates */
		(*coords)[0] = 0.0f;	(*coords)[1] = GLB::WINDOW_HEIGHT_ZOOMED - GAME::UI_TOP_HEIGHT;
		(*coords)[2] = 0.0f;	(*coords)[3] = GAME::UI_BOTTOM_HEIGHT;
		(*coords)[4] = GLB::WINDOW_WIDTH_ZOOMED; (*coords)[5] = GAME::UI_BOTTOM_HEIGHT;
		(*coords)[6] = GLB::WINDOW_WIDTH_ZOOMED; (*coords)[7] = GLB::WINDOW_HEIGHT_ZOOMED - GAME::UI_TOP_HEIGHT;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(GAME::CAMERA_POS_X, GAME::CAMERA_POS_Y, 0.0));
		(*rect).create(*coords);
		(*rect).render(glm::mat4(1.0f), model);
	}
}
