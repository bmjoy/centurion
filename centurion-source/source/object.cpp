#include <object>
#include <player>

using namespace std;
using namespace glm;

GObject::GObject(){
	selected = false;
}
void GObject::set_position(vec3 pos) {
	position = pos;
}
void GObject::set_class(string class_name) {
	ifstream path("assets/data/classes/" + class_name + ".oc.json");
	data = json::parse(path);
	className = class_name;
}
void GObject::set_player(Player *p) {
	player = p;
}
void GObject::set_id(int pickingId) {
	picking_id = pickingId;
	int r = (pickingId & 0x000000FF) >> 0;
	int g = (pickingId & 0x0000FF00) >> 8;
	int b = (pickingId & 0x00FF0000) >> 16;
	pickingColor = vec3(r / 255.f, g / 255.f, b / 255.f);
}
GObject::~GObject(){}
