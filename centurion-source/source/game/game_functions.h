#pragma once

#include <global.hpp>
#include <objects.h>

#include "../terrain/surface.h"
#include "../building/building.h"
#include "../unit/unit.h"
#include "../engine/camera.h"

namespace game {
	static int clickCount = 0;
	static float cameraToX = 0.f, cameraToY = 0.f;
	static bool doubleClick = false;
	static int clickIdList[2] = { 0, 0 };

	void picking(std::map<int, Building> *bList, std::map<int, Unit> *uList, glm::mat4 *proj, glm::mat4 *view, int *clickId, bool *blockMinimap);
	void tracing(Surface *s, glm::mat4 *proj, glm::mat4 *view);
	void clearBuffers();
	void goToPosition(std::map<int, Building> *bList, Camera *c, double *lastTime, int *clickId, bool *blockMinimap);
	void renderObjects(std::map<int, Building> *bList, std::map<int, Unit> *uList, glm::mat4 *proj, glm::mat4 *view, int *clickId, int *selectedUnits);
	void renderSelRectangle(std::array<float, 8> *coords, glm::mat4 *view, float *cameraLastX, float *cameraLastY);
	void renderMapRectangle(std::array<float, 8> *coords);
}

