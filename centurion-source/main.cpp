//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <global>
#include <engine.h>

// main function

int main(int numArgs, char *args[]) {

	glb::initParams();
	Engine::Init();
	
	if (Engine::myWindow::ShouldClose == false) {
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
