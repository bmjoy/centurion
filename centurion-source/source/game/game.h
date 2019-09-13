#pragma once

#include <global.hpp>
#include <json.hpp>

#include "../engine/camera.h"
#include "../gui/ui_game.h"
#include "../terrain/surface.h"
#include "../primitives/empty_rectangle.h"
#include "../unit/unit.h"
#include "../unit/unit_sprite.h"
#include "../building/building.h"
#include "../engine/cursor_point.h"
#include "../engine/window.h"

class Game
{
public:
	Game();

	void set_window(myWindow *wnd) { window = wnd; }
	void create();
	void run();
	std::map<int, Unit> unitList;

	~Game();

private:

	/* OBJECTS */

	myWindow *window;
	Camera camera;
	Surface surface;
	EmptyRectangle minimapRectangle;
	EmptyRectangle selectionRectangle;
	USprite unit_sprite;
	Unit unit;
	UIGame ui;
	CPoint cursor_point;

	Building b;
	std::map<int, Building> buildingList;

	int objectId = 0;
	int click_id;
	int clickIdList[2] = { 0, 0 };

	void picking();
	bool cursorInGameScreen() { return (GLB::MOUSE_LEFT_Y > GAME::UI_BOTTOM_HEIGHT) && (GLB::MOUSE_LEFT_Y < (GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT)); }

	unsigned char data;

	json settl_data;
	std::string r;
	glm::vec2 origin;

	float threshold;
	float cameraLastX, cameraLastY, camera_to_x, camera_to_y;
	glm::mat4 view, projection, proj2;
	glm::mat4 minimapRectangle_model;

	std::array<float, 8> minimap_rect_coords;
	std::array<float, 8> sel_rect_coords;
	void go_to_position();
	void tracing();
	void clear_buffers();

	void render_selection_rectangle();
	void render_minimap_rectangle();

	int clicks_count;
	bool blockMinimap;
	double lastTime;
	bool doubleClick;
};