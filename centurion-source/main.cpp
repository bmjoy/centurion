//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <global>
#include <engine/engine.h>
#include <engine/window.h>

using namespace engine;

// main function 
int main(int numArgs, char *args[]) {

	glb::initParams();
	Engine::Init();
	if (myWindow::ShouldClose == false) {
		try {
			return Engine::launch();
		}
		catch (...) {

		}
	}
	else {
		return 0;
	}
}
