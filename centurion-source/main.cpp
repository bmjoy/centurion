#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <global>
#include <engine>

int main() {

	glb::initParams();
	if (!glb::getBoolean("window-should-close")) {
		return engine::ENGINE()->launch();
	}
	else {
		return 0;
	}
}