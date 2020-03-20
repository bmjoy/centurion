//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <engine.h>

// main function

int main(int numArgs, char *args[]) {

	Engine::Init(args[0]);
	
	if (Engine::myWindow::ShouldClose == false) {
		try {
			return Engine::Launch();
		}
		catch (...) {
			Engine::GameClose();
		}
	}
	
	else {
		return 0;
	}

}
