#include <classes/decoration.h>
#include <engine.h>
#include <stb_image.h>
#include <player/player.h>
#include <pathfinding/pathfinding.h>
#include <game/strategy.h>

#include <decoration_sprite.h>

using namespace std;
using namespace glm;

Decoration::Decoration() 
{
	this->bIsBeingMoved = false;
	this->bIsPlaceable = true;
	this->SetType("cpp_decorationclass");
	settlementName = "N/A";
	settlID = 0;
	this->SetPosition(vec3(Engine::Mouse::GetXMapCoordinate(), Engine::Mouse::GetYMapCoordinate(), 10.f));
}

void Decoration::checkSettlement() {
	/*vector<Building*> listOfIndipBuildings = Game::GetListOfIndipendentBuildings();
	for (int i = 0; i < listOfIndipBuildings.size(); i++) {
		Building* bld = listOfIndipBuildings[i];

		string settlName = bld->GetSettlement()->GetSettlementName();
		float settlX = bld->GetPosition().x;
		float settlY = bld->GetPosition().y;

		if (math::euclidean_distance(this->GetPosition().x, this->GetPosition().y, settlX, settlY) <= TOWNHALL_RADIUS) {
			settlementName = settlName;
			settlID = bld->GetPickingID();
			independent = bld;
			oldSettlX = settlX;
			oldSettlY = settlY;
			break;
		}
		else {
			settlID = 0;
			settlementName = "N/A";
		}
	}*/
}

void Decoration::CheckIfPlaceable(void)
{
	bIsPlaceable = true;
	vec3 var_position = this->GetPosition();
	std::vector<std::vector<unsigned int>> passGrid = this->GetPass();
	bIsPlaceable = astar::CheckObjectPassAvailability(passGrid, var_position);
}

void Decoration::Render(const bool picking, const unsigned int clickID)
{
	if (picking) return;

	this->CheckIfPlaceable();

	if (Engine::GetEnvironment() == EDITOR_ENV) {
		if (Game::IsGameObjectNotNull(settlID)) {
			if (oldX != this->GetPosition().x || oldY != this->GetPosition().y || oldSettlX != GObject::GetObjectByID(settlID)->GetPosition().x || oldSettlY != GObject::GetObjectByID(settlID)->GetPosition().y) {
				checkSettlement();
			}
		}
	}
	DSprite()->Render(spriteData, this->GetPosition().x, this->GetPosition().y, bIsPlaceable);
}

Decoration::~Decoration() {}