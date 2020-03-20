#ifndef  _UNIT_H
#define _UNIT_H

#include <ui.h>
#include "object.h"
#include <json.hpp>

using namespace std;
using namespace glm;

class Building;
class Player;


class Unit : public GObject
{
public:
	Unit();
	static int GetCounter() { return unitsCounter; }
	static void IncreaseCounter() { unitsCounter++; }
	static void ResetCounter() { unitsCounter = 0; }
	void SetPosition(const float x, const float y);
	void create();
	void render(bool picking, int clickID = 0, bool not_placeable = false) override;
	~Unit();
private:
	UnitData unitData;

	static int unitsCounter;

	/* unit properties & data */
	string currentStateStr;
	json entityData;
	float creationTime;

	/* unit position */
	vec3 position2D, position3D;
	bool clickSelection, rectangleSelection;

	/* unit circle if selected */
	gui::Circle selectionCircle;

	/* unit movement & pathfinding */
	bool is_Moving;
	int pathCount;
	float angle, delta, distance, res_distance;
	vector<ivec2> path;
	vec2 startPoint, endPoint;
	void position_update();
	void walk_behaviour();

	/* debug objects */
	vector<gui::Rectangle> pathQuadsList;
	struct HitBox {
		gui::Rectangle rectangle;
		array<float, 8> coords;
	} hitbox;
	gui::Image circlePos;
};


// UNIT FUNCTIONS --> source/unit/functions.cpp

void updateZ(vec3 &pos2d, vec3 *pos3d);
void updateFrame(float *creationTime, int *frame, int max_frames, int duration);
float getAngle(vector<ivec2> &path, int &n);
float getDistance(vector<ivec2> &path, int &n);
float getResDistance(vector<ivec2> &path, int &n, vec3 &pos2d);
vector<ivec2> getPath(vec2 &start, vec2 &end);

#endif