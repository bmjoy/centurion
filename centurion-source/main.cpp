//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <global>
#include <engine>
//#include <lmx/lmxparse.h>

// main function 
int main(int numArgs, char *args[]) {

	//glb::exe_root = "";
	//std::cout << "[DEBUG] Centurion.exe started at the following root: " << glb::exe_root << std::endl;

	glb::initParams();
	if (!glb::getBoolean("window-should-close")) {
		try {
			return engine::ENGINE()->launch();
		}
		catch (...) {

		}
	}
	else {
		return 0;
	}
}
