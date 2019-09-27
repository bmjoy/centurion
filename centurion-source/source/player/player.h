#pragma once

#include <global.hpp>

class Player
{
public:
	Player();
	void create(int Id, int Team, std::string Race, glm::vec3 Col);
	void setStartPoint(glm::vec2 point) { startPoint = point; }
	glm::vec2 getStartPoint() { return startPoint; }
	glm::vec3 *getPlayerColor() { return &color; }
	int getPlayerTeam() { return team; }
	int getPlayerId() { return id; }
	std::string getPlayerRace() { return startRace; }
	
	~Player();
private:
	glm::vec3 color;
	int id;
	int team;
	std::string startRace;
	glm::vec2 startPoint;
};

