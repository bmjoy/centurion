#include <player>

Player::Player(){
	id = 0;
	color = glm::vec4(0.f);
	team = 0;
}

void Player::create(int Id, int Team, std::string Race, glm::vec3 Col) {
	id = Id;
	team = Team;
	startRace = Race;
	color = Col;
}

Player::~Player(){}
