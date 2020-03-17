#include <classes/shipyard.h>
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

Shipyard::Shipyard()
{
	this->IAmACentralBuilding();
	this->IAmAShipyard();
}
