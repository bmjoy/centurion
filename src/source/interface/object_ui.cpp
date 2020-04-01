#include <ui.h>
#include <interface>

using namespace std;
using namespace glm;

namespace game {
	ObjectUI::ObjectUI() {}

	void ObjectUI::create(string className) {
		objectName = gui::SimpleText("static");
		objectName.create_static(className, "tahoma_15px_bold", 100.f, -50.f, "left", "normal", vec4(0.f));
	}

	void ObjectUI::render() {
		objectName.render_static();
	}

	ObjectUI::~ObjectUI() {}
}
