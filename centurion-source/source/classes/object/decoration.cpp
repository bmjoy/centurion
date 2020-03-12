#include <object/decoration.h>
#include <engine.h>
#include <stb_image.h>
#include <player>
#include <pathfinding>
#include <game/strategy.h>
#include <maths.h>
#include <interface>

#include <primitives.h>
#include <decoration_sprite.h>

using namespace std;
using namespace glm;

Decoration::Decoration() {
	type = "decoration";
	settlementName = "N/A";
	settlID = 0;
}

void Decoration::prepare() {
	string pass_path = data["pass_path"].get<string>();
	pass_grid = astar::readPassMatrix(pass_path, className);

	int w, h, nrChannels;

	ifstream path_ent(data["ent_path"].get<string>());
	if (!path_ent.good()) {
		//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
	}
	json ent_data = json::parse(path_ent);
	string texturePath = ent_data["path"].get<string>() + ent_data["sprite"].get<string>();

	unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
	stbi_image_free(texture);


	decorData.width = w;
	decorData.height = h;
	decorData.className = className;
	decorData.textureID = DSprite()->getTextureID(className);
}

void Decoration::create() {

	string pass_path = data["pass_path"].get<string>();
	if (pass_grid.size() == 0) pass_grid = astar::readPassMatrix(pass_path, className);
	update_pass();

	int w, h, nrChannels;

	ifstream path_ent(data["ent_path"].get<string>());
	if (!path_ent.good()) {
		//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
	}
	json ent_data = json::parse(path_ent);
	string texturePath = ent_data["path"].get<string>() + ent_data["sprite"].get<string>();

	unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
	stbi_image_free(texture);

	decorData.width = w;
	decorData.height = h;
	decorData.className = className;
	decorData.textureID = DSprite()->getTextureID(className);

	settlementName = "N/A";
	checkSettlement();
	oldX = position.x;
	oldY = position.y;
}

void Decoration::checkSettlement() {
	vector<Building*> listOfIndipBuildings = Game::GetListOfIndipendentBuildings();
	for (int i = 0; i < listOfIndipBuildings.size(); i++) {
		Building* bld = listOfIndipBuildings[i];
		string settlName = bld->get_settlement_name();
		float settlX = bld->get_position().x;
		float settlY = bld->get_position().y;

		if (math::euclidean_distance(position.x, position.y, settlX, settlY) <= TOWNHALL_RADIUS) {
			settlementName = settlName;
			settlID = bld->get_id();
			independent = bld;
			oldSettlX = settlX;
			oldSettlY = settlY;
			break;
		}
		else {
			settlID = 0;
			settlementName = "N/A";
		}
	}
}

bool Decoration::is_placeable() {
	return astar::checkAvailability(pass_grid, position);
}

void Decoration::render(bool picking, int clickID, bool not_placeable)  {

	if (picking) return;

	if (Engine::getEnvironment() == "editor") {
		if (Game::IsGameObjectNotNull(settlID)) {
			if (oldX != position.x || oldY != position.y || oldSettlX != Game::GetGameObjectPtrById(settlID)->get_position().x || oldSettlY != Game::GetGameObjectPtrById(settlID)->get_position().y) {
				checkSettlement();
			}
		}
	}

	decorData.position = vec2(position.x, position.y);
	DSprite()->render(decorData, not_placeable);
}

Decoration::~Decoration() {}