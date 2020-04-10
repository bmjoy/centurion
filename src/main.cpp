// to hide the cmd:
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <engine.h>

// main function

int main(int numArgs, char *args[]) {

	if (Engine::Initialize(args[0]))
	{
		return Engine::Launch();
	}
	else
	{
		return 1;
	}
	
}
