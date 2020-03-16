#include <classes/habitable.h>
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

void Habitable::StartGoldProduction(void)
{
	this->bCanProduceGold = true;
}

void Habitable::StopGoldProduction(void)
{
	this->bCanProduceGold = true;
}

void Habitable::StartFoodProduction(void)
{
	this->bCanProduceFood = true;
}

void Habitable::StopFoodProduction(void)
{
	this->bCanProduceFood = false;
}
