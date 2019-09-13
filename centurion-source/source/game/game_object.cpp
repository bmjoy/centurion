#include "game_object.h"



GObject::GObject()
{
}

void GObject::set_position(glm::vec3 pos) {
	position = pos;
}

void GObject::set_class(std::string className) {
	std::ifstream path("assets/data/classes/" + className + ".oc.json");
	data = json::parse(path);
}

void GObject::set_player(int playerId) {
	player_color = GAME::PLAYERS_COLOR[playerId];
}

void GObject::set_id(int pickingId) {
	picking_id = pickingId;
}

GObject::~GObject()
{
}
