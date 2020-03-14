//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


#include <engine.h>

// main function

int main(int numArgs, char *args[]) {

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
