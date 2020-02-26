#include <global>
#include <surface>
#include <math>
#include <json.hpp>
#include <game>
#include <picking>
#include <object>
#include <menu>
#include "../lmx/xml-settings.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

/* ----- GLOBAL ----- */

using namespace glb;
using namespace std;
using namespace game;

namespace glb {

	string exe_root = ""; //defined in main.cpp
	string language = "english"; //Default value
    mat4 menuProjection;
	mat4 cameraProjection;
	mat4 minimapProjection;

	GLFWwindow *MainWindow;

	vector<vec3> colors;
	vector<string> racesNames;
	map<string, Race> RACES;
	map<string, int> availableLanguages;

	int CharCodepointPressed = -1; // for text input
	bool KeyCode[GLFW_KEY_LAST] = { false }; // for text input

	void setErrors(map<string, string> errorsMap) { errors = errorsMap; }
	string getErrorCode(string error) { return errors[error]; }
	void setParam(string param, float value) { params[param] = value; }
	float getParam(string param) { return params[param]; }
	void setBoolean(string param, bool value) { booleans[param] = value; }
	bool getBoolean(string param) { return booleans[param]; }

	void initParams() {
		//Close the game if it wasn't able to find or process errorCodes.json file
		ifstream errorCodes_path("assets/data/errorCodes.json");
		if (!errorCodes_path.good()) {
			map<string, string> tempMap;
			tempMap["NOT_FOUND"] = "0x00000001";
			translation_table_current["noFile"] = "Unable to find or process ERROR CODES file.\n  Forced application shutdown has started.";
			setErrors(tempMap);
			forceGameClosure("NOT_FOUND" , "noFile");
		}
		json errorCodes = json::parse(errorCodes_path);
		map<string, string> errorsMap = errorCodes.get<map<string, string>>();
		setErrors(errorsMap);

		lmx::elmx_error xmlReadError;
		string settingsPath = exe_root + "\\settings.xml";
		c_settings settings = c_settings(settingsPath.c_str(), &xmlReadError);

		if (xmlReadError != lmx::ELMX_OK)
			cout << xmlReadError << endl;

		read_settings();

		getParam("Ciao");

		read_translation_tables();
		
		setParam("window-ratio", getParam("window-width") / getParam("window-height"));
		setParam("window-width-zoomed", getParam("window-width") + (currentZoomCamera - 1) * zoomCameraFactor);
		setParam("window-height-zoomed", getParam("window-height") + (currentZoomCamera - 1) * zoomCameraFactor / getParam("window-ratio"));
		setBoolean("window-should-close", false);
		setBoolean("wireframe", false);
		setBoolean("mouse-left", false);
		setBoolean("mouse-left-pressed", false);
		setBoolean("mouse-right", false);
		setBoolean("mouse-release", false);
		setBoolean("mouse-scroll-bool", false);
		setParam("mouse-scroll", 0.f);

		menuProjection = glm::ortho(0.0f, getParam("window-width"), 0.0f, getParam("window-height"), -100.0f, 100.0f);
		cameraProjection = glm::ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)mapWidth, (float)mapWidth);

		ifstream data_path("assets/data/data.json");
		//Close the game if it wasn't able to find or process data.json file
		if (!data_path.good()) {
			forceGameClosure("NOT_FOUND" , "ERROR_data");
		}
		json data = json::parse(data_path);

		for (int i = 0; i < data["player_colors"].size(); i++) {
			vec3 color = vec3(data["player_colors"][i]["r"], data["player_colors"][i]["g"], data["player_colors"][i]["b"]);
			glb::colors.push_back(color);
		}

		stbi_flip_vertically_on_write(1);
	}

	void read_settings() {
		ifstream fin("settings");

		// default values
		setParam("window-width", 1366.f);
		setParam("window-height", 768.f);
		setParam("camera-movespeed", 10.f);
		setParam("camera-max-zoom", 20.f);
		language = "english";
		setBoolean("debug", false);
		setBoolean("full-screen", false);

		if (!fin.good()) {
			save_settings();
		}
		else {
			string line, value;
			while (getline(fin, line)) {
				if (line[0] == '[') continue;
				string values[2];
				stringstream s(line);
				int i = 0;
				while (getline(s, values[i], '=')) i++;
				if (values[0] == "window-width") setParam(values[0], stof(values[1]));
				if (values[0] == "window-height") setParam(values[0], stof(values[1]));
				if (values[0] == "camera-movespeed") setParam(values[0], stof(values[1]));
				if (values[0] == "camera-max-zoom") setParam(values[0], stof(values[1]));
				if (values[0] == "language") language = values[1];
				if (values[0] == "debug") setBoolean("debug", (bool)stoi(values[1]));
				if (values[0] == "full-screen") setBoolean("full-screen", (bool)stoi(values[1]));
			}
		}
		cout << "[DEBUG] Current language: " << language << "\n";
	}

	void save_settings() {
		ofstream fout("settings");
		if (fout.is_open()) {
			fout << "[Settings]\n\n";
			fout << "window-width=" << getParam("window-width") << "\n";
			fout << "window-height=" << getParam("window-height") << "\n";
			fout << "camera-movespeed=" << getParam("camera-movespeed") << "\n";
			fout << "camera-max-zoom=" << getParam("camera-max-zoom") << "\n";
			fout << "language=" << language << "\n";
			fout << "debug=" << getBoolean("debug") << "\n";
			fout << "full-screen=" << getBoolean("full-screen") << "\n";
		}
		fout.close();
	}

	void saveLog() {
		ofstream logFile("logs/global.log");		
		if (logFile.is_open()){
			logFile << "PARAMETERS\n";
			for (map<string, float>::iterator x = params.begin(); x != params.end(); x++) {
				logFile << "\t" << x->first << " = " << x->second << "\n";
			}
			logFile << "\nBOOLEANS\n";
			for (map<string, bool>::iterator x = booleans.begin(); x != booleans.end(); x++) {
				logFile << "\t" << x->first << " = " << x->second << "\n";
			}
			logFile << "\nERROR CODE\n";
			for (map<string, string>::iterator x = errors.begin(); x != errors.end(); x++) {
				logFile << "\t" << x->first << " = " << x->second << "\n";
			}
		}
		logFile.close();
	}

	void read_translation_tables() {
		ifstream fin("assets/data/translations - table.tsv");
		if (!fin.good()) {
			translation_table_current["temp"] = "Unable to find or process TRANSLATIONS file.\n  Forced application shutdown has started.";
			forceGameClosure("NOT_FOUND", translation_table_current["temp"]);
		}
		else {
			string line, value;
			int currentlang = 0;
			int nLanguages = 0;
			int row = 0;
			while (getline(fin, line)) {
				if (row == 0) { // first row
					stringstream s(line);
					while (getline(s, value, '\t')) {
						if (value == language) currentlang = nLanguages;
						if (nLanguages != 0) availableLanguages[value] = nLanguages - 1;
						nLanguages++;
					}
				}
				if (row > 0) {
					vector<string> values(nLanguages, "");
					stringstream s(line);
					int i = 0;
					while (getline(s, value, '\t')) {
						values[i] = value;
						i++;
					}
					string key = values[0];
					string eng = values[1];
					string current = values[currentlang];
					(current != "") ? translation_table_current[key] = current : translation_table_current[key] = eng;
				}
				row++;
			}
		}
	}

	void changeLanguage(string lan) {
		language = lan;
		read_translation_tables();
		menu::MENU()->update();
		cout << "DEBUG : Language changed to " + lan << endl;
		cout << "DEBUG : Settings file updated!" << endl;
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
		// create a folder which will contain all scenario files
		string path = "scenarios/" + name;
		_mkdir(path.c_str());

		// save heights 
		ofstream heightsFile(path+"/heights");
		if (heightsFile.is_open()) {
			for (int i = 0; i < mapgen::nVertices * 4; i += 4)
				if (i == 0) {
					heightsFile << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
				}
				else {
					heightsFile << "," << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
				}
		}
		heightsFile.close();

		// save texture type
		ofstream textureFile(path + "/texture");
		if (textureFile.is_open()) {
			for (int i = 0; i < mapgen::nVertices; i ++)
				if (i == 0) {
					textureFile << mapgen::MapTextures()[i];
				}
				else {
					textureFile << "," << mapgen::MapTextures()[i];
				}
		}
		textureFile.close();

		// save objects
		ofstream objectsFile(path + "/objects.tsv");
		if (objectsFile.is_open()) {
			objectsFile << "type\tclass\tname\tsettlement\tplayerID\tx\ty\txoffset\tyoffset" << "\n";
			for (map<int, Building>::iterator bld = game::buildings.begin(); bld != game::buildings.end(); bld++) {
				objectsFile << bld->second.get_type() << "\t";
				objectsFile << bld->second.get_class() << "\t";
				objectsFile << bld->second.get_name() << "\t";
				objectsFile << bld->second.get_settlement_name() << "\t";
				objectsFile << bld->second.get_playerID() << "\t";
				objectsFile << bld->second.get_position().x << "\t";
				objectsFile << bld->second.get_position().y << "\t";
				if (bld->second.is_independent()) {
					objectsFile << 0 << "\t";
					objectsFile << 0;
				}
				else {
					objectsFile << bld->second.get_position().x - bld->second.get_settlement_building()->get_position().x << "\t";
					objectsFile << bld->second.get_position().y - bld->second.get_settlement_building()->get_position().y;
				}
				objectsFile << "\n";
			}
			/*for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
				u->second.render(true, 0);
			}*/
			for (map<int, Decoration>::iterator dec = game::decorations.begin(); dec != game::decorations.end(); dec++) {
				objectsFile << dec->second.get_type() << "\t";
				objectsFile << dec->second.get_class() << "\t";
				objectsFile << "N/A" << "\t";
				objectsFile << dec->second.get_settlement_name() << "\t";
				objectsFile << 0 << "\t";
				objectsFile << dec->second.get_position().x << "\t";
				objectsFile << dec->second.get_position().y << "\t";
				if (dec->second.get_settlement_name() != "N/A") {
					objectsFile << dec->second.get_position().x - dec->second.get_settlement_building()->get_position().x << "\t";
					objectsFile << dec->second.get_position().y - dec->second.get_settlement_building()->get_position().y << "\n";
				}
				else {
					objectsFile << 0 << "\t";
					objectsFile << 0 << "\n";
				}
			}
		}
		objectsFile.close();

		cout << "[DEBUG] The map is saved with the following name: " + name << endl;
	}
	void openScenario(string name) {
		// read heights
		{
			fstream fin;
			fin.open("scenarios/" + name + "/heights");
			string line, number;
			getline(fin, line);
			stringstream s(line);
			int i = 0;
			while (getline(s, number, ',')) {
				mapgen::MapHeights()[i] = stof(number);
				i++;
			}
		}
		// read texture
		{
			fstream fin;
			fin.open("scenarios/" + name + "/texture");
			string line, number;
			getline(fin, line);
			stringstream s(line);
			int i = 0;
			while (getline(s, number, ',')) {
				mapgen::MapTextures()[i] = stof(number);
				i++;
			}
		}
		// read objects
		{
			fstream fin;
			fin.open("scenarios/" + name + "/objects.tsv");
			string line, value;
			int row = 0;
			while (getline(fin, line)){
				if (row > 0){
					string objectsInfo[9];
					stringstream s(line);
					int i = 0;
					while (getline(s, value, '\t')) {
						objectsInfo[i] = value;
						i++;
					}
					string type = objectsInfo[0];
					string className = objectsInfo[1];
					string name = objectsInfo[2];
					string settl_name = objectsInfo[3];
					int playerID = stoi(objectsInfo[4]);
					float xPos = stof(objectsInfo[5]);
					float yPos = stof(objectsInfo[6]);
					float xOffset = stof(objectsInfo[7]);
					float yOffset = stof(objectsInfo[8]);
					if (type == "building") {
						building::Building b = building::Building();
						b.set_class(className);
						b.set_player(playerID);
						b.set_position(vec3(xPos, yPos, 0.f));
						b.set_id(getPickingID());
						b.set_settlement_name(settl_name);
						b.create(name);
						game::buildings[getPickingID()] = b;
						if (game::buildings[getPickingID()].is_independent()) {
							game::independent_buildings[getPickingID()] = &game::buildings[getPickingID()];
						}
						increasePickingID();
					}
					if (type == "decoration") {
						decoration::Decoration d = decoration::Decoration();
						d.set_class(className);
						d.set_player(0);
						d.set_position(vec3(xPos, yPos, 0.f));
						d.set_id(getPickingID());
						d.create();
						game::decorations[getPickingID()] = d;
						increasePickingID();
					}
				}
				row++;
			}
			/* set central building for every dependent building */
			for (map<int, Building>::iterator bld = game::buildings.begin(); bld != game::buildings.end(); bld++) {
				int ID = bld->first;
				if (!bld->second.is_independent()) {
					for (map<int, Building*>::iterator settl = game::independent_buildings.begin(); settl != game::independent_buildings.end(); settl++) {
						int settl_ID = settl->first;
						if (settl->second->get_settlement_name() == bld->second.get_settlement_name()) {
							game::buildings[ID].set_settlement_building(game::independent_buildings[settl_ID]);
							break;
						}
					}
				}
			}
		}
	}
	void takeScreenshot() {
		char filename[50];
		static char basename[30];
		time_t t = time(NULL);
		strftime(basename, 30, "%Y%m%d_%H%M%S.png", localtime(&t));
		strcpy(filename, "screenshots/");
		strcat(filename, basename);

		int w = (int)getParam("window-width");
		int h = (int)getParam("window-height");
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
	vector<string> get_all_folders_names_within_folder(string folder){
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
		return getParam("window-height") * (x - getParam("ui-bottom-height")) / (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height"));
	}

	bool cursorInGameScreen() {
		return (getParam("mouse-y-leftclick") > getParam("ui-bottom-height")) && (getParam("mouse-y-leftclick") < (glb::getParam("window-height") - getParam("ui-top-height")));
	}

	void clearAndSwapBuffers(GLFWwindow *window) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	vec2 getZoomedCoords(float xCoord, float yCoord) {
		float x = xCoord * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position");
		float y = yCoord * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position");
		return vec2(x, y);
	}

	void forceGameClosure(string errorCode, string errorText) {
		string eC = (getTranslation("WORD_errorCode") == "") ? "Error code" : getTranslation("WORD_errorCode");
		string text = "  " + eC + ": " + getErrorCode(errorCode) + "\n\n  " + getTranslation(errorText);
		if (language == "arabic") text = "  " + getErrorCode(errorCode)+ ": " + eC + "\n\n  " + getTranslation(errorText);
		const int wideLength = sizeof(text.c_str())*128;
		WCHAR wstr[wideLength];
		MultiByteToWideChar(CP_UTF8, 0, text.c_str(), wideLength, wstr, wideLength);
		MessageBoxW(NULL, wstr, gameNameLPCWSTR, MB_ICONERROR);
		setBoolean("window-should-close", true);
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

	Race::Race(){
	}

	Race::~Race(){
	}
};

/* ----- OBJECTS ----- */

namespace obj {

    AudioManager *Audio() { return &audioM; }
    BitmapFont *Text(){ return &txt; }
    BuildingSprite *BSprite(){ return &bsprite; }
    DecorationSprite *DSprite(){ return &dsprite; }
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
