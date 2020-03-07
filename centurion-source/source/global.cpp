#include <global>
#include <surface>
#include <math>
#include <json.hpp>
#include <game/strategy.h>
#include <picking>
#include <object/object.h>
#include <menu>
#include <Windows.h>
#include <engine/camera.h>
#include <engine/mouse.h>
#include <engine/window.h>
#include <translationTable-xml.hxx>
#include <interface>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

/* ----- GLOBAL ----- */

using namespace glb;
using namespace std;

using namespace engine;

namespace glb {

	string exe_root = ""; //defined in main.cpp
	mat4 menuProjection;
	mat4 cameraProjection;
	mat4 minimapProjection;

	vector<vec3> colors;
	vector<string> racesNames;
	map<string, Race> RACES;
	map<string, int> availableLanguages;

	int CharCodepointPressed = -1; // for text input
	bool KeyCode[GLFW_KEY_LAST] = { false }; // for text input

	void setErrors(map<string, string> errorsMap) { errors = errorsMap; }
	string getErrorCode(string error) { return errors[error]; }

	void initParams() {
		try {
			//Close the game if it wasn't able to find or process errorCodes.json file

			ifstream errorCodes_path("assets/data/errorCodes.json");
			if (!errorCodes_path.good()) {
				map<string, string> tempMap;
				tempMap["NOT_FOUND"] = "0x00000001";
				translation_table_current["noFile"] = "Unable to find or process ERROR CODES file.\n  Forced application shutdown has started.";
				setErrors(tempMap);
				forceGameClosure("NOT_FOUND", "noFile");
			}
			json errorCodes = json::parse(errorCodes_path);
			map<string, string> errorsMap = errorCodes.get<map<string, string>>();
			setErrors(errorsMap);

			// Read Settings.xml

			Settings::Init();
			Settings::ReadSettings();

			//read_settings();
			read_translation_tables();

			myWindow::Ratio = myWindow::Width / myWindow::Height;
			myWindow::WidthZoomed = myWindow::Width + (Camera::GetCurrentZoom() - 1) * Camera::GetZoomFactor();
			myWindow::HeightZoomed = myWindow::Height + (Camera::GetCurrentZoom() - 1) * Camera::GetZoomFactor() / myWindow::Ratio;
		
			menuProjection = glm::ortho(0.0f, myWindow::Width, 0.0f, myWindow::Height, -100.0f, 100.0f);
			cameraProjection = glm::ortho(0.0f, myWindow::WidthZoomed, 0.0f, myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH);

			ifstream data_path("assets/data/data.json");
			//Close the game if it wasn't able to find or process data.json file
			if (!data_path.good()) {
				forceGameClosure("NOT_FOUND", "ERROR_data");
			}
			json data = json::parse(data_path);

			for (int i = 0; i < data["player_colors"].size(); i++) {
				vec3 color = vec3(data["player_colors"][i]["r"], data["player_colors"][i]["g"], data["player_colors"][i]["b"]);
				glb::colors.push_back(color);
			}

			stbi_flip_vertically_on_write(1);
		}
		catch (...) {
			std::cout << "An error occurred" << std::endl;
		}
	}

	void read_translation_tables() {
		try{
			if (availableLanguages.empty()) {
				int nLanguages = 0;
				vector<string> filesName = get_all_files_names_within_folder("assets/data");
				for (int i = 0; i < filesName.size(); i++) {
					if (filesName[i].substr(0, filesName[i].find('_')) == "translationTable") {
						string lan = filesName[i].substr(filesName[i].find('_'));
						availableLanguages[lan.substr(1,lan.find('.') -1)] = nLanguages;
						nLanguages++;
					}
				}
			}

			string path = "assets/data/translationTable_" + Settings::Language + ".xml";
			auto_ptr<translationTable> tTable = translationTable_(path);
			translationTable::entry_iterator it;
			for (it = tTable->entry().begin(); it != tTable->entry().end(); it++) {
				translation_table_current[it->stringName()] = it->result();
			}
		}
		catch (...) {
			std::cout << "An error occurred" << std::endl;
		}
	}

	void saveLog() {
		ofstream logFile("logs/Params.xml");
		if (logFile.is_open()) {
			logFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<params>\n" <<
				"\t<camera>\n" <<
				"\t\t<xPosition>" << Camera::GetXPosition() << "</xPosition>\n" <<
				"\t\t<yPosition>" << Camera::GetYPosition() << "</yPosition>\n" <<
				"\t</camera>\n" <<
				"\t<mouse>\n" <<
				"\t\t<scrollValue>" << Mouse::ScrollValue << "</scrollValue>\n" <<
				"\t\t<xLeftClick>" << Mouse::GetXLeftClick() << "</xLeftClick>\n" <<
				"\t\t<xPosition>" << Mouse::GetXPosition() << "</xPosition>\n" <<
				"\t\t<xRightClick>" << Mouse::GetXRightClick() << "</xRightClick>\n" <<
				"\t\t<y2dPosition>" << Mouse::GetY2DPosition() << "</y2dPosition>\n" <<
				"\t\t<y2dRightClick>" << Mouse::GetY2DRightClick() << "</y2dRightClick>\n" <<
				"\t\t<yLeftClick>" << Mouse::GetYLeftClick() << "</yLeftClick>\n" <<
				"\t\t<yPosition>" << Mouse::GetYPosition() << "</yPosition>\n" <<
				"\t\t<yRightClick>" << Mouse::GetYRightClick() << "</yRightClick>\n" <<
				"\t\t<leftClick>" << Mouse::LeftClick << "</leftClick>\n" <<
				"\t\t<leftHold>" << Mouse::LeftHold << "</leftHold>\n" <<
				"\t\t<release>" << Mouse::Release << "</release>\n" <<
				"\t\t<rightClick>" << Mouse::RightClick << "</rightClick>\n" <<
				"\t\t<scrollBool>" << Mouse::ScrollBool << "</scrollBool>\n" <<
				"\t</mouse>\n" <<
				"\t<window>\n" <<
				"\t\t<heightZoomed>" << myWindow::HeightZoomed << "</heightZoomed>\n" <<
				"\t\t<ratio>" << myWindow::Ratio << "</ratio>\n" <<
				"\t\t<widthZoomed>" << myWindow::WidthZoomed << "</widthZoomed>\n" <<
				"\t\t<shouldClose>" << myWindow::ShouldClose << "</shouldClose>\n" <<
				"\t\t<bottomBarHeight>" << myWindow::BottomBarHeight << "</bottomBarHeight>\n" <<
				"\t\t<topBarHeight>" << myWindow::TopBarHeight << "</topBarHeight>\n" <<
				"\t</window>\n" <<
				"</params>";
		}
		logFile.close();
	}

	void changeLanguage(string lan) {
		Settings::Language = lan;
		read_translation_tables();
		menu::MENU()->update();
		cout << "DEBUG : Language changed to " + lan << endl;
		cout << "DEBUG : Settings file updated!" << endl;
	}

	void setMinimapProjection() {
		float bottom = (-1.f) * (MEDIUM_MAP_HEIGHT * myWindow::BottomBarHeight / (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight));
		float top = MEDIUM_MAP_HEIGHT + MEDIUM_MAP_HEIGHT * myWindow::TopBarHeight / (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight);
		float left = 0.f;
		float right = (float)MEDIUM_MAP_WIDTH;
		glb::minimapProjection = ortho(left, right, bottom, top, -right, right);
	}

	void readIndicesData(unsigned int *indices, string path) {
		fstream fin;
		fin.open(path);
		string line, number;
		getline(fin, line);
		stringstream s(line);
		int i = 0;
		while (getline(s, number, ',')) {
			indices[i] = (unsigned int)stoi(number);
			i++;
		}
	}

	void readVerticesData(float *vertices, string path) {
		fstream fin;
		fin.open(path);
		string line, number;
		getline(fin, line);
		stringstream s(line);
		int i = 0;
		while (getline(s, number, ',')) {
			vertices[i] = stof(number);
			i++;
		}
	}

	void readVerticesPosData(int *verticesPos, string path) {
		fstream fin;
		fin.open(path);
		string line, number;
		getline(fin, line);
		stringstream s(line);
		int i = 0;
		while (getline(s, number, ',')) {
			verticesPos[i] = stoi(number);
			i++;
		}
	}
	void saveCurrentScenario(string name) {
	//	
	//	
	//	// create a folder which will contain all scenario files
	//	string path = "scenarios/" + name;
	//	_mkdir(path.c_str());

	//	// save heights 
	//	ofstream heightsFile(path + "/heights");
	//	if (heightsFile.is_open()) {
	//		for (int i = 0; i < mapgen::nVertices * 4; i += 4)
	//			if (i == 0) {
	//				heightsFile << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
	//			}
	//			else {
	//				heightsFile << "," << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
	//			}
	//	}
	//	heightsFile.close();

	//	// save texture type
	//	ofstream textureFile(path + "/texture");
	//	if (textureFile.is_open()) {
	//		for (int i = 0; i < mapgen::nVertices; i++)
	//			if (i == 0) {
	//				textureFile << mapgen::MapTextures()[i];
	//			}
	//			else {
	//				textureFile << "," << mapgen::MapTextures()[i];
	//			}
	//	}
	//	textureFile.close();

	//	// save objects
	//	ofstream objectsFile(path + "/objects.tsv");
	//	if (objectsFile.is_open()) {
	//		objectsFile << "type\tclass\tname\tsettlement\tplayerID\tx\ty\txoffset\tyoffset" << "\n";
	//		for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
	//			objectsFile << bld->second.get_type() << "\t";
	//			objectsFile << bld->second.get_class() << "\t";
	//			objectsFile << bld->second.get_name() << "\t";
	//			objectsFile << bld->second.get_settlement_name() << "\t";
	//			objectsFile << bld->second.get_playerID() << "\t";
	//			objectsFile << bld->second.get_position().x << "\t";
	//			objectsFile << bld->second.get_position().y << "\t";
	//			if (bld->second.is_independent()) {
	//				objectsFile << 0 << "\t";
	//				objectsFile << 0;
	//			}
	//			else {
	//				objectsFile << bld->second.get_position().x - bld->second.get_settlement_building()->get_position().x << "\t";
	//				objectsFile << bld->second.get_position().y - bld->second.get_settlement_building()->get_position().y;
	//			}
	//			objectsFile << "\n";
	//		}
	//		/*for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
	//			u->second.render(true, 0);
	//		}*/
	//		for (map<int, Decoration>::iterator dec = decorations.begin(); dec != decorations.end(); dec++) {
	//			objectsFile << dec->second.get_type() << "\t";
	//			objectsFile << dec->second.get_class() << "\t";
	//			objectsFile << "N/A" << "\t";
	//			objectsFile << dec->second.get_settlement_name() << "\t";
	//			objectsFile << 0 << "\t";
	//			objectsFile << dec->second.get_position().x << "\t";
	//			objectsFile << dec->second.get_position().y << "\t";
	//			if (dec->second.get_settlement_name() != "N/A") {
	//				objectsFile << dec->second.get_position().x - dec->second.get_settlement_building()->get_position().x << "\t";
	//				objectsFile << dec->second.get_position().y - dec->second.get_settlement_building()->get_position().y << "\n";
	//			}
	//			else {
	//				objectsFile << 0 << "\t";
	//				objectsFile << 0 << "\n";
	//			}
	//		}
	//	}
	//	objectsFile.close();

	//	cout << "[DEBUG] The map is saved with the following name: " + name << endl;
	//
	}
	void openScenario(string name) {
		//// read heights
		//{
		//	fstream fin;
		//	fin.open("scenarios/" + name + "/heights");
		//	string line, number;
		//	getline(fin, line);
		//	stringstream s(line);
		//	int i = 0;
		//	while (getline(s, number, ',')) {
		//		mapgen::MapHeights()[i] = stof(number);
		//		i++;
		//	}
		//}
		//// read texture
		//{
		//	fstream fin;
		//	fin.open("scenarios/" + name + "/texture");
		//	string line, number;
		//	getline(fin, line);
		//	stringstream s(line);
		//	int i = 0;
		//	while (getline(s, number, ',')) {
		//		mapgen::MapTextures()[i] = stof(number);
		//		i++;
		//	}
		//}
		//// read objects
		//{
		//	fstream fin;
		//	fin.open("scenarios/" + name + "/objects.tsv");
		//	string line, value;
		//	int row = 0;
		//	while (getline(fin, line)) {
		//		if (row > 0) {
		//			string objectsInfo[9];
		//			stringstream s(line);
		//			int i = 0;
		//			while (getline(s, value, '\t')) {
		//				objectsInfo[i] = value;
		//				i++;
		//			}
		//			string type = objectsInfo[0];
		//			string className = objectsInfo[1];
		//			string name = objectsInfo[2];
		//			string settl_name = objectsInfo[3];
		//			int playerID = stoi(objectsInfo[4]);
		//			float xPos = stof(objectsInfo[5]);
		//			float yPos = stof(objectsInfo[6]);
		//			float xOffset = stof(objectsInfo[7]);
		//			float yOffset = stof(objectsInfo[8]);
		//			if (type == "building") {
		//				Building b = Building();
		//				b.set_class(className);
		//				b.set_player(playerID);
		//				b.set_position(vec3(xPos, yPos, 0.f));
		//				b.set_id(getPickingID());
		//				b.set_settlement_name(settl_name);
		//				b.create(name);
		//				buildings[getPickingID()] = b;
		//				if (buildings[getPickingID()].is_independent()) {
		//					independent_buildings[getPickingID()] = &buildings[getPickingID()];
		//				}
		//				increasePickingID();
		//			}
		//			if (type == "decoration") {
		//				Decoration d = Decoration();
		//				d.set_class(className);
		//				d.set_player(0);
		//				d.set_position(vec3(xPos, yPos, 0.f));
		//				d.set_id(getPickingID());
		//				d.create();
		//				decorations[getPickingID()] = d;
		//				increasePickingID();
		//			}
		//		}
		//		row++;
		//	}
		//	/* set central building for every dependent building */
		//	for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
		//		int ID = bld->first;
		//		if (!bld->second.is_independent()) {
		//			for (map<int, Building*>::iterator settl = independent_buildings.begin(); settl != independent_buildings.end(); settl++) {
		//				int settl_ID = settl->first;
		//				if (settl->second->get_settlement_name() == bld->second.get_settlement_name()) {
		//					buildings[ID].set_settlement_building(independent_buildings[settl_ID]);
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}
	}
	void takeScreenshot() {
		char filename[50];
		static char basename[30];
		time_t t = time(NULL);
		strftime(basename, 30, "%Y%m%d_%H%M%S.png", localtime(&t));
		strcpy(filename, "screenshots/");
		strcat(filename, basename);

		int w = (int)myWindow::Width;
		int h = (int)myWindow::Height;
		unsigned char* imageData = new unsigned char[int(w * h * 3)];
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		int saved = stbi_write_png(filename, w, h, 3, imageData, 0);
		free(imageData);
	}
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	string ReadFile(const char* fileLocation) {
		string content;
		ifstream fileStream(fileLocation, ios::in);
		if (!fileStream.is_open()) {
			printf("[DEBUG] Failed to read %s! File doesn't exist.", fileLocation);
			return "";
		}
		string line = "";
		while (!fileStream.eof()) {
			getline(fileStream, line);
			content.append(line + "\n");
		}
		fileStream.close();
		return content;
	}
	vector<string> get_all_files_names_within_folder(string folder) {
		vector<string> names;
		string search_path = folder + "/*.*";
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					names.push_back(fd.cFileName);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		return names;
	}
	vector<string> get_all_folders_names_within_folder(string folder) {
		vector<string> names;
		WIN32_FIND_DATA findfiledata;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		char fullpath[MAX_PATH];
		GetFullPathName(folder.c_str(), MAX_PATH, fullpath, 0);
		string fp(fullpath);

		hFind = FindFirstFile((LPCSTR)(fp + "\\*").c_str(), &findfiledata);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if ((findfiledata.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY
					&& (findfiledata.cFileName[0] != '.'))
				{
					names.push_back(findfiledata.cFileName);
				}
			} while (FindNextFile(hFind, &findfiledata) != 0);
		}
		return names;
	}

	string getTranslation(string code) {
		if (translation_table_current.count(code) > 0) {
			return translation_table_current[code];
		}
		else {
			return code;
		}
	}

	vector<file_info> get_all_files_names_within_subfolders(string const &folder_name, string const &file_extension) {
		HANDLE finder;          // for FindFirstFile
		WIN32_FIND_DATA file;   // data about current file.
		priority_queue<string, vector<string>,
			greater<string> > dirs;
		dirs.push(folder_name); // start with passed directory 
		vector<file_info> output;
		do {
			string path = dirs.top();// retrieve directory to search
			dirs.pop();
			if (path[path.size() - 1] != '\\')  // normalize the name.
				path += "\\";
			string const fmask = "*";
			string mask = path + fmask;    // create mask for searching
			// First search for files:
			if (INVALID_HANDLE_VALUE == (finder = FindFirstFile(mask.c_str(), &file)))
				continue;
			do {
				if (!(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					file_info fileInfo;
					fileInfo.name = file.cFileName;
					fileInfo.path = path + file.cFileName;
					if (file_extension == "") {
						fileInfo.name = fileInfo.name.substr(0, fileInfo.name.find_last_of("."));
						output.push_back(fileInfo);
					}
					else {
						string fullname = file.cFileName;
						string fileExt = fullname.substr(fullname.find_last_of(".") + 1);
						if (fileExt == file_extension) {
							fileInfo.name = fileInfo.name.substr(0, fileInfo.name.find_last_of("."));
							output.push_back(fileInfo);
						}
					}
				}
			} while (FindNextFile(finder, &file));
			FindClose(finder);
			// Then search for subdirectories:
			if (INVALID_HANDLE_VALUE == (finder = FindFirstFile((path + "*").c_str(), &file)))
				continue;
			do {
				if ((file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (file.cFileName[0] != '.'))
					dirs.push(path + file.cFileName);
			} while (FindNextFile(finder, &file));
			FindClose(finder);
		} while (!dirs.empty());
		return output;
	}

	float getYMinimapCoord(float x) {
		return myWindow::Height * (x - myWindow::BottomBarHeight) / (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight);
	}

	bool cursorInGameScreen() {
		return (Mouse::GetYLeftClick() > myWindow::BottomBarHeight) && (Mouse::GetYLeftClick() < (myWindow::Height - myWindow::TopBarHeight));
	}

	void clearAndSwapBuffers(GLFWwindow *window) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	vec2 getZoomedCoords(float xCoord, float yCoord) {
		float x = xCoord * myWindow::WidthZoomed / myWindow::Width + Camera::GetXPosition();
		float y = yCoord * myWindow::HeightZoomed / myWindow::Height + Camera::GetYPosition();
		return vec2(x, y);
	}

	void forceGameClosure(string errorCode, string errorText) {
		string eC = (getTranslation("WORD_errorCode") == "") ? "Error code" : getTranslation("WORD_errorCode");
		string text = "  " + eC + ": " + getErrorCode(errorCode) + "\n\n  " + getTranslation(errorText);
		if (Settings::Language == "arabic") text = "  " + getErrorCode(errorCode) + ": " + eC + "\n\n  " + getTranslation(errorText);
		const int wideLength = sizeof(text.c_str()) * 128;
		WCHAR wstr[wideLength];
		MultiByteToWideChar(CP_UTF8, 0, text.c_str(), wideLength, wstr, wideLength);
		MessageBoxW(NULL, wstr, gameNameLPCWSTR, MB_ICONERROR);
		myWindow::ShouldClose = true;
	}

	void showGameWarning(string warningText) {
		string text = "  " + getTranslation(warningText);
		const int wideLength = sizeof(text.c_str()) * 128;
		WCHAR wstr[wideLength];
		MultiByteToWideChar(CP_UTF8, 0, text.c_str(), wideLength, wstr, wideLength);
		MessageBoxW(NULL, wstr, gameNameLPCWSTR, MB_ICONINFORMATION);
	}

	bool folderExists(string folderPath) {
		struct stat info;
		if (stat(folderPath.c_str(), &info) == 0)
			return true;
		return false;
	}

	Race::Race() {
	}

	Race::~Race() {
	}
};

/* ----- OBJECTS ----- */

namespace obj {

	AudioManager *Audio() { return &audioM; }
	BitmapFont *Text() { return &txt; }
	BuildingSprite *BSprite() { return &bsprite; }
	DecorationSprite *DSprite() { return &dsprite; }
	CursorImage *Cursor() { return &cursor; }
	EmptyCircle *ECircle() { return &eCircle; }
	EmptyRectangle *ERectangle() { return &eRect; }
	FilledRectangle *FRectangle() { return &fRect; }
	ImageSprite *Img() { return &img; }
	UnitSprite *USprite() { return &usprite; }
	Terrain *MapTerrain() { return &terrain; }
	Grid *MapGrid() { return &grid; }
	MinimapRectangle *MMRectangle() { return &mmRect; }

	void init() {
		//*Audio() = AudioManager();
		*Text() = BitmapFont();
		*BSprite() = BuildingSprite();
		*DSprite() = DecorationSprite();
		*Cursor() = CursorImage();
		*ECircle() = EmptyCircle();
		*ERectangle() = EmptyRectangle();
		*FRectangle() = FilledRectangle();
		*Img() = ImageSprite();
		*USprite() = UnitSprite();
		*MapTerrain() = Terrain();
		*MapGrid() = Grid();
		*MMRectangle() = MinimapRectangle();
	}
	void compile() {
		Text()->compile();
		BSprite()->compile();
		DSprite()->compile();
		ECircle()->compile();
		ERectangle()->compile();
		FRectangle()->compile();
		Cursor()->compile();
		Img()->compile();
		USprite()->compile();
		MapTerrain()->compile();
		MapGrid()->compile();
		MMRectangle()->compile();
	}
	void create() {
		Text()->apply_projection_matrix(glb::menuProjection);
		Cursor()->apply_projection_matrix(glb::menuProjection);

		//----

		Text()->create();
		Cursor()->create();
		ECircle()->create();
		ERectangle()->create();
		FRectangle()->create();
		BSprite()->create();
		DSprite()->create();
		Img()->create();
		USprite()->create();

		//----

		MapTerrain()->create();
		MapGrid()->create();
		MMRectangle()->create();
	}

	void applyMenuMatrices() {
		Img()->apply_projection_matrix(glb::menuProjection);
		Img()->apply_view_matrix();
		ECircle()->apply_projection_matrix(glb::menuProjection);
		ECircle()->apply_view_matrix();
		ERectangle()->apply_projection_matrix(glb::menuProjection);
		ERectangle()->apply_view_matrix();
		FRectangle()->apply_projection_matrix(glb::menuProjection);
		FRectangle()->apply_view_matrix();
		MMRectangle()->apply_projection_matrix(glb::menuProjection);
		MMRectangle()->apply_view_matrix();
	}
	void applyGameMatrices(mat4 *proj, mat4 *view) {
		BSprite()->apply_projection_matrix(*proj);
		BSprite()->apply_view_matrix(*view);
		DSprite()->apply_projection_matrix(*proj);
		DSprite()->apply_view_matrix(*view);
		USprite()->apply_projection_matrix(*proj);
		USprite()->apply_view_matrix(*view);
		Img()->apply_projection_matrix(*proj);
		Img()->apply_view_matrix(*view);
		ECircle()->apply_projection_matrix(*proj);
		ECircle()->apply_view_matrix(*view);
		ERectangle()->apply_projection_matrix(*proj);
		ERectangle()->apply_view_matrix(*view);
		FRectangle()->apply_projection_matrix(*proj);
		FRectangle()->apply_view_matrix(*view);
		MapTerrain()->apply_projection_matrix(*proj);
		MapTerrain()->apply_view_matrix(*view);
		MapGrid()->apply_projection_matrix(*proj);
		MapGrid()->apply_view_matrix(*view);
	}
};
