#include "player.h"

using namespace std;
using namespace glm;

Player::Player(void)
{
	id = 0;
	color = glm::vec4(0.f);
	team = 0;
}

void Player::Create(const unsigned int Id, const unsigned int Team, const std::string Race, const glm::vec3 Col) 
{
	id = Id;
	team = Team;
	startRace = Race;
	color = Col;
}

void Player::SetStartPoint(const vec2 point)
{
	this->startPoint = point;
}

vec2 Player::GetStartPoint(void)
{
	return this->startPoint;
}

vec3 * Player::GetPlayerColor(void)
{
	return &(this->color);
}

unsigned int Player::GetPlayerTeam(void)
{
	return this->team;
}

unsigned int Player::GetPlayerId(void)
{
	return this->id;
}

string Player::GetPlayerRace(void)
{
	return this->startRace;
}

Player::~Player(void){}
