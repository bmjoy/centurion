#include <classes/decoration.h>
#include <engine.h>
#include <stb_image.h>
#include <player/player.h>
#include <pathfinding/pathfinding.h>
#include <game/strategy.h>
#include <maths.h>

#include <decoration_sprite.h>

using namespace std;
using namespace glm;

Decoration::Decoration() 
{
	this->SetType("cpp_decorationclass");
	settlementName = "N/A";
	settlID = 0;
}

void Decoration::prepare() {
	//string pass_path = data["pass_path"].get<string>();
	//string str_className = this->GetClassName();
	//pass_grid = astar::readPassMatrix(pass_path, str_className);

	//int w, h, nrChannels;

	//ifstream path_ent(data["ent_path"].get<string>());
	//if (!path_ent.good()) {
	//	//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
	//}
	//json ent_data = json::parse(path_ent);
	//string texturePath = ent_data["path"].get<string>() + ent_data["sprite"].get<string>();

	//unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
	//stbi_image_free(texture);


	//decorData.width = w;
	//decorData.height = h;
	//decorData.className = this->GetClassName();
	//decorData.textureID = DSprite()->getTextureID(this->GetClassName());
}

void Decoration::create() {

	//string pass_path = data["pass_path"].get<string>();
	//string str_className = this->GetClassName();
	//if (pass_grid.size() == 0) pass_grid = astar::readPassMatrix(pass_path, str_className);
	//update_pass();

	//int w, h, nrChannels;

	//ifstream path_ent(data["ent_path"].get<string>());
	//if (!path_ent.good()) {
	//	//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
	//}
	//json ent_data = json::parse(path_ent);
	//string texturePath = ent_data["path"].get<string>() + ent_data["sprite"].get<string>();

	//unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
	//stbi_image_free(texture);

	//decorData.width = w;
	//decorData.height = h;
	//decorData.className = this->GetClassName();
	//decorData.textureID = DSprite()->getTextureID(this->GetClassName());

	//settlementName = "N/A";
	//checkSettlement();
	//oldX = this->GetPosition().x;
	//oldY = this->GetPosition().y;
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

bool Decoration::is_placeable()
{
	vec3 var_position = this->GetPosition();
	return astar::checkAvailability(pass_grid, var_position);
}

void Decoration::Render(const bool picking, const unsigned int clickID, const bool not_placeable)
{
	if (picking) return;

	if (Engine::GetEnvironment() == EDITOR_ENV) {
		if (Game::IsGameObjectNotNull(settlID)) {
			if (oldX != this->GetPosition().x || oldY != this->GetPosition().y || oldSettlX != GObject::GetObjectByID(settlID)->GetPosition().x || oldSettlY != GObject::GetObjectByID(settlID)->GetPosition().y) {
				checkSettlement();
			}
		}
	}
	DSprite()->Render(spriteData, this->GetPosition().x, this->GetPosition().y, not_placeable);
}

Decoration::~Decoration() {}