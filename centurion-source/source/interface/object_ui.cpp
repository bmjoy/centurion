#include <gui>
#include <interface>

namespace game {
	ObjectUI::ObjectUI() {}

	void ObjectUI::create(string className) {
		objectName = gui::SimpleText("static");
		objectName.create_static(className, "tahoma_15px_bold", 100.f, 100.f, "left", "normal", vec4(255.f));
	}

	void ObjectUI::render() {
		objectName.render_static();
	}

	ObjectUI::~ObjectUI() {}
}
