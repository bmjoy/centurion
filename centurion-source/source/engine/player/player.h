#pragma once

#include <header.h>

using namespace std;
using namespace glm;

class Player
{
public:
	Player();
	void create(int Id, int Team, string Race, vec3 Col);
	void setStartPoint(vec2 point) { startPoint = point; }
	vec2 getStartPoint() { return startPoint; }
	vec3 *getPlayerColor() { return &color; }
	int getPlayerTeam() { return team; }
	int getPlayerId() { return id; }
	string getPlayerRace() { return startRace; }
	~Player();
private:
	vec3 color;
	int id;
	int team;
	string startRace;
	vec2 startPoint;
};