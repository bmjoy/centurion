#include "match.h"
#include <engine/mouse.h>
#include <surface>

using namespace engine;

Match::Match(){}

void Match::Tracing(){
	if (Mouse::RightClick) {
		unsigned char tracingCol[4];
		Game::Surface::Render(true);
		glReadPixels((GLint)Mouse::GetXRightClick(), (GLint)Mouse::GetYRightClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
		mapgen::mouseZNoise = (mapgen::maxZ - mapgen::minZ) * ((float)tracingCol[0] / 255.0f) + mapgen::minZ;
	}
}
Match::~Match(){}