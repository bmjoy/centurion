#include <classes/object.h>
#include <player/player.h>
#include <game/strategy.h>
#include <pathfinding/pathfinding.h>

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

float GObject::GetRadius(void)
{
	return this->radius;
}

void GObject::SetRadius(const float par_radius)
{
	this->radius = par_radius;
}

string GObject::GetClassName(void)
{
	return this->className;
}

void GObject::SetClassName(string parClassName)
{
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

unsigned short int GObject::GetRace(void)
{
	return this->race;
}

string GObject::GetRaceName(void)
{
	return this->raceName;
}

void GObject::SetRaceName(const string par_raceName)
{
	this->raceName = par_raceName;
}

void GObject::SetRace(const unsigned short int par_race)
{
	this->race = par_race;
}

unsigned short int GObject::GetSight(void)
{
	return this->sight;
}

void GObject::SetSight(const unsigned short int par_sight)
{
	this-> sight = par_sight;
}

bool GObject::CanBePositionedIntoWater(void)
{
	return this->isWaterObject;
}

void GObject::AllowPositioningIntoWater(void)
{
	this->isWaterObject = true;
}
void GObject::DenyPositioningIntoWater(void)
{
	this->isWaterObject = false;
}

bool GObject::IsBuilding(void)
{
	return (this->type == "building");
}

bool GObject::IsUnit(void)
{
	return this->type == "unit";
}

bool GObject::IsDecoration(void)
{
	return (this->type == "decoration");
}

Building* GObject::AsBuilding()
{ 
	return (Building*)this; 
}
Unit* GObject::AsUnit()
{ 
	return (Unit*)this; 
}
Decoration* GObject::AsDecoration()
{ 
	return (Decoration*)this;
}

void GObject::update_pass(void) 
{
	astar::updatePassMatrix(this->pass_grid, this->position);
}

void GObject::clear_pass(void) 
{
	astar::clearPassMatrix(this->pass_grid, this->position);
}

void GObject::SetPosition(const vec3 pos)
{
	this->position = pos;
}

vec3 GObject::GetPosition(void)
{
	return this->position;
}

int GObject::get_xPos(void)
{ 
	return (int)position.x; 
}
int GObject::get_yPos(void)
{ 
	return (int)position.y; 
}

GObject::~GObject(void) 
{
	//TODO
}
