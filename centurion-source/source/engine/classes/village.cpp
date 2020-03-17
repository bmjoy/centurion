#include <classes/village.h>
#include <engine.h>
#include <stb_image.h>
#include <player/player.h>
#include <pathfinding/pathfinding.h>
#include <game/strategy.h>
#include <maths.h>
#include <interface>
#include <building_sprite.h>
#include <logger.h>

using namespace std;
using namespace glm;

VillageHall::VillageHall()
{
	this->IAmACentralBuilding();
	this->IAmAVillageHall();
	this->StartFoodProduction();
}
