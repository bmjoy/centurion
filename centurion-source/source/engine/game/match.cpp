#include "match.h"
#include <engine.h>
#include <mapgen/mapgen.h>

Match::Match(void) {}

void Match::Tracing(void)
{
	if (Engine::Mouse::RightClick) 
	{
		unsigned char tracingCol[4];
		Game::Map::Render(true);
		glReadPixels((GLint)Engine::Mouse::GetXRightClick(), (GLint)Engine::Mouse::GetYRightClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
		mapgen::mouseZNoise = (mapgen::maxZ - mapgen::minZ) * ((float)tracingCol[0] / 255.0f) + mapgen::minZ;
	}
}
Match::~Match(void) {}