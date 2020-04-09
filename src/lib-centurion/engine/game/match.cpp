#include "match.h"
#include "map.h"
#include <engine.h>

using namespace std;
using namespace glm;

namespace Game
{
	void Match::Tracing(void)
	{
		if (Engine::Mouse::RightClick)
		{
			unsigned char tracingCol[4];
			Game::Map::Render(true);
			glReadPixels((GLint)Engine::Mouse::GetXRightClick(), (GLint)Engine::Mouse::GetYRightClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
			Engine::Mouse::SetMouseZNoise((Game::Mapgen::GetMaxZ() - Game::Mapgen::GetMinZ()) * ((float)tracingCol[0] / 255.0f) + Game::Mapgen::GetMinZ());
		}
	}
};
