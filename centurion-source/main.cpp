#include <global>
#include <engine>

int main() {

	glb::initParams();
	return engine::ENGINE()->launch();
}