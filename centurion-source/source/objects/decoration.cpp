#include <object>
#include <engine>
#include <stb_image.h>
#include <player>
#include <pathfinding>
#include <game>
#include <math>
#include <interface>

using namespace std;
using namespace glm;

namespace decoration {

	Decoration::Decoration() {
		type = "decoration";
	}

	void Decoration::create() {
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
		decorData.textureID = obj::DSprite()->getTextureID(className);
	}

	void Decoration::render() {
		decorData.position = vec2(position.x, position.y);

		obj::DSprite()->render(decorData);
	}

	Decoration::~Decoration() {}
};