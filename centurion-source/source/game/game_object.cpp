#include "game_object.h"



GObject::GObject()
{
	selected = false;
}

void GObject::set_position(glm::vec3 pos) {
	position = pos;
}

void GObject::set_class(std::string class_name) {
	std::ifstream path("assets/data/classes/" + class_name + ".oc.json");
	data = json::parse(path);
	className = class_name;
}

void GObject::set_player(Player *p) {
	player = p;
}

void GObject::set_id(int pickingId) {
	picking_id = pickingId;
}

GObject::~GObject()
{
}
