#include <object/object.h>
#include <player>
#include <game/strategy.h>
#include <pathfinding>

using namespace std;
using namespace glm;


GObject::GObject()
{
	selected = false;
}

unsigned short int GObject::GetPlayer(void)
{
	return this->playerID;
}

bool GObject::IsSelected(void)
{
	return this->selected;
}

void GObject::Select(bool par_selected)
{
	this->selected = par_selected;
}

void GObject::SetPlayer(const unsigned short int par_playerID)
{
	this->player = &playersList[par_playerID];
}

unsigned int GObject::GetPickingID(void)
{
	return this->pickingID;
}

void GObject::SetPickingID(const unsigned int par_pickingID)
{
	this->pickingID = par_pickingID;
	int r = (par_pickingID & 0x000000FF) >> 0;
	int g = (par_pickingID & 0x0000FF00) >> 8;
	int b = (par_pickingID & 0x00FF0000) >> 16;
	pickingColor = vec3(r / 255.f, g / 255.f, b / 255.f);
}

string GObject::GetClassName(void)
{
	return this->className;
}

void GObject::SetClassName(string parClassName)
{
	ifstream path("assets/data/classes/" + parClassName + ".oc.json");
	data = json::parse(path);
	this->className = parClassName;
}

string GObject::GetType(void)
{
	return this->type;
}

void GObject::SetType(const string par_type)
{
	this->type = par_type;
}

string GObject::GetName(void)
{
	return this->name;
}

void GObject::SetName(const string par_name)
{
	this->name = par_name;
}
//---

void GObject::set_position(vec3 pos)
{
	position = pos;
}

void GObject::update_pass() {
	astar::updatePassMatrix(pass_grid, position);
}
void GObject::clear_pass() {
	astar::clearPassMatrix(pass_grid, position);
}
GObject::~GObject() {}

/*
GObject::GObject()
{
	selected = false;
}
void GObject::set_position(vec3 pos) 
{
	position = pos;
}
void GObject::set_class(string class_name) 
{
	ifstream path("assets/data/classes/" + class_name + ".oc.json");
	data = json::parse(path);
	className = class_name;
}
void GObject::SetPlayer(int player_id) {
	player = &playersList[player_id];
}
void GObject::SetPickingID(int pickingId) {
	picking_id = pickingId;
	int r = (pickingId & 0x000000FF) >> 0;
	int g = (pickingId & 0x0000FF00) >> 8;
	int b = (pickingId & 0x00FF0000) >> 16;
	pickingColor = vec3(r / 255.f, g / 255.f, b / 255.f);
}
void GObject::update_pass() {
	astar::updatePassMatrix(pass_grid, position);
}
void GObject::clear_pass() {
	astar::clearPassMatrix(pass_grid, position);
}
GObject::~GObject(){}
*/

