#include "debug_ui.h"

using namespace glb;

DebugUI::DebugUI()
{
	staticTextList = {
		"frames/s: ",
		"ms/frame: ",
		"y-window: ",
		"x-window: ",
		"y-map: ",
		"x-map: ",
		"selected units: ",
		"z-normal: ",
		"y-normal: ",
		"x-normal: "
	};
	dynamicTextList = { "0" };

	startX = 14.f;
	startY = GAME::UI_BOTTOM_HEIGHT + 12.f;
	deltaY = 15.f;
}

void DebugUI::create() {

	dynamicText = gui::SimpleText("dynamic");

	debugText = gui::SimpleText("static");
	debugText.create_static("Centurion - DEBUG MODE", "inconsolata_8", startX, startY + 177, "left", "normal", glm::vec4(255,255,0,255));

	for (int i = 0; i < staticTextList.size(); i++) {
		gui::SimpleText tempText = gui::SimpleText("static");
		tempText.create_static(staticTextList[i], "inconsolata_8", startX, startY + deltaY * i, "left", "normal", glm::vec4(255.f));
		staticText.push_back(tempText);
	}

	back = gui::Rectangle();
	back.create("border-filled", 10.f, GAME::UI_BOTTOM_HEIGHT + 10.f, 186, 200.f, "bottom-left", 0);
	
	
}

void DebugUI::render(int fps, int mpfs, int selUnits) {
	std::stringstream streamx, streamy, streamz;
	float x, y, xNorm = 0.f, yNorm = 0.f, zNorm = 0.f;
	bool cursorinGame = (GLB::MOUSE_Y > GAME::UI_BOTTOM_HEIGHT) && (GLB::MOUSE_Y < (getParam("window-height") - GAME::UI_TOP_HEIGHT) && GLB::MOUSE_X > 0 && GLB::MOUSE_X < getParam("window-width"));


	if (cursorinGame) {
		x = (GLB::MOUSE_X * getParam("window-width-zoomed") / getParam("window-width") + GAME::CAMERA_POS_X);
		y = (GLB::MOUSE_Y * getParam("window-height-zoomed") / getParam("window-height") + GAME::CAMERA_POS_Y);
		float x1 = round(x / (float)mapgen::grid_size);
		float y1 = round(y / (float)mapgen::grid_size);
		int j = (int)(y1 * GAME::MAP_WIDTH / mapgen::grid_size + x1);

		xNorm = mapgen::MapVertices()[mapgen::VerticesPos()[j] * 10 + 6];
		yNorm = mapgen::MapVertices()[mapgen::VerticesPos()[j] * 10 + 7];
		zNorm = mapgen::MapVertices()[mapgen::VerticesPos()[j] * 10 + 8];
	}
	


	back.render(glm::vec4(0.f, 0.f, 0.f, 0.5f));

	debugText.render_static();

	dynamicTextList[0] = std::to_string(fps);
	dynamicTextList[1] = std::to_string(mpfs);
	dynamicTextList[2] = std::to_string(GLB::MOUSE_Y);
	dynamicTextList[3] = std::to_string(GLB::MOUSE_X);
	dynamicTextList[4] = std::to_string((int)getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).y);
	dynamicTextList[5] = std::to_string((int)getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).x);
	dynamicTextList[6] = std::to_string(selUnits);
	
	
	streamx << std::fixed << std::setprecision(2) << zNorm;
	dynamicTextList[7] = streamx.str();

	streamy << std::fixed << std::setprecision(2) << yNorm;
	dynamicTextList[8] = streamy.str();

	streamz << std::fixed << std::setprecision(2) << xNorm;
	dynamicTextList[9] = streamz.str();

	!GLB::GAME ? n = 2 : n = (int)dynamicTextList.size();

	for (int i = 0; i < n; i++) {
		staticText[i].render_static();
		dynamicText.render_dynamic(dynamicTextList[i], "inconsolata_8", startX + 135.f, startY + deltaY * i, glm::vec4(255.f), "left", "normal");
	}
}

DebugUI::~DebugUI()
{
}
