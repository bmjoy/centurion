#include <editor>

namespace editor {

	/* extern variables definitions */

	bool IsWindowOpened = false;
	bool OpenMapWindowIsOpen = false;
	bool OpenMapWindowUpdate = false;
	vector<string> availableScenarios;
	bool NewMapWindowIsOpen = true;
	bool NewMapResetText = false;
	string currentMapName;

	Editor *EDITOR() { return &myeditor; }

	/* tools */
}