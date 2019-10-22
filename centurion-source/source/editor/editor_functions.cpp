#include <editor>
#include <interface>

namespace editor {

	/* extern variables definitions */

	bool IsWindowOpened = false;
	bool OpenMapWindowIsOpen = false;
	bool OpenMapWindowUpdate = false;
	vector<string> availableScenarios;
	bool NewMapWindowIsOpen = true;
	bool NewMapResetText = false;
	string currentMapName;
	bool AddObjectWindowIsOpen = false;
	bool AddObjectWindowUpdateForm1and2 = false;
	bool AddObjectWindowUpdateForm2 = false;

	string EditorObjectStringListForm0[NumberOfObjects] = { "" };
	string EditorObjectStringListForm1[NumberOfObjects] = { "" };
	string EditorObjectStringListForm2[NumberOfObjects] = { "" };

	Editor *EDITOR() { return &myeditor; }

	/* tools */
}