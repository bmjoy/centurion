#include <interface>
#include <picking>
#include <engine>
#include <surface>

using namespace glb;
using namespace engine;

namespace editor {

	NewMapWindow::NewMapWindow() {}
	
	void NewMapWindow::create() {
		text_input.create();
	}

	void NewMapWindow::render() {
		text_input.render();
	}

	NewMapWindow::~NewMapWindow() {}
};
