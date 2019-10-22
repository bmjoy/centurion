#include <global>
#include <surface>
#include <math>
#include <json.hpp>
#include <game>

/* ----- GLOBAL ----- */

using namespace glb;
using namespace std;
using namespace game;

namespace glb {

	string language = "english"; //Default value
    mat4 menuProjection;
	mat4 cameraProjection;
	mat4 minimapProjection;

	GLFWwindow *MainWindow;

	vector<vec3> colors;
	vector<string> races;

	int CharCodepointPressed = -1; // for text input
	bool KeyCode[GLFW_KEY_LAST] = { false }; // for text input

	void setErrors(map<string, string> errorsMap) { errors = errorsMap; }
	string getErrorCode(string error) { return errors[error]; }
	void setParam(string param, float value) { params[param] = value; }
	float getParam(string param) { return params[param]; }
	void setBoolean(string param, bool value) { booleans[param] = value; }
	bool getBoolean(string param) { return booleans[param]; }
	void setTranslations(map<string, string> translationsMap) { translations = translationsMap; }
	void replaceTranslation(string code, string value) { translations[code] = value; }
	string getTranslation(string code) { return translations[code]; }

	void initParams() {
		//Close the game if it wasn't able to find or process errorCodes.json file
		ifstream errorCodes_path("assets/data/errorCodes.json");
		if (!errorCodes_path.good()) {
			map<string, string> tempMap;
			tempMap["NOT_FOUND"] = "0x00000001";
			tempMap["noFile"] = "Unable to find or process ERROR CODES file.\n  Forced application shutdown has started.";
			setErrors(tempMap);
			setTranslations(tempMap);
			forceGameClosure("NOT_FOUND" , "noFile");
		}
		json errorCodes = json::parse(errorCodes_path);
		map<string, string> errorsMap = errorCodes.get<map<string, string>>();
		setErrors(errorsMap);

		ifstream settings_path("settings.json");
		if (!settings_path.good()) {
			forceGameClosure("NOT_FOUND" , "ERROR_settings");
		}
		json settings = json::parse(settings_path);

		language = settings["language"].get<string>();
		cout << "DEBUG: Current language: " << language << "\n";
		translateTexts();
		setBoolean("debug", (bool)settings["debug"].get<int>());
		setParam("window-width", (float)settings["window_width"]);
		setParam("window-height", (float)settings["window_height"]);
		setParam("window-ratio", getParam("window-width") / getParam("window-height"));
		setParam("window-width-zoomed", getParam("window-width") + (currentZoomCamera - 1) * zoomCameraFactor);
		setParam("window-height-zoomed", getParam("window-height") + (currentZoomCamera - 1) * zoomCameraFactor / getParam("window-ratio"));

		setBoolean("window-should-close", false);
		setBoolean("wireframe", false);
		setBoolean("mouse-left", false);
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

		for (int i = 0; i < data["races"].size(); i++) {
			glb::races.push_back(data["races"][i].get<string>());
		}

		for (int i = 0; i < data["player_colors"].size(); i++) {
			vec3 color = vec3(data["player_colors"][i]["r"], data["player_colors"][i]["g"], data["player_colors"][i]["b"]);
			glb::colors.push_back(color);
		}
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

	void translateTexts() {
		ifstream translationTable_path("assets/translations/english.json");
		if (translationTable_path.good()) {
			json tJson = json::parse(translationTable_path);
			map<string, string> translationsMap = tJson.get<map<string, string>>();
			setTranslations(translationsMap);
		}
		else {
			map<string, string> tempMap;
			tempMap["noFile"] = "Unable to find or process ENGLISH language file.\n  Forced application shutdown has started.";
			setTranslations(tempMap);
			forceGameClosure("NOT_FOUND", "noFile");
		}
		if (language != "english") {
			ifstream localTranslations_path("assets/translations/" + language + ".json");
			if (localTranslations_path.good()) {
				json tJson = json::parse(localTranslations_path);
				map<string, string> localTranslationsMap = tJson.get<map<string, string>>();
				for (map<string, string>::iterator i = localTranslationsMap.begin(); i != localTranslationsMap.end(); i++) {
					if (i->second.size() > 0){
						replaceTranslation(i->first, i->second);
					}
				}
			}
		}
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
		cout << "DEBUG: The map is saved with the following name: " + name << endl;
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
	}
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	string ReadFile(const char* fileLocation) {
		string content;
		ifstream fileStream(fileLocation, ios::in);
		if (!fileStream.is_open()) {
			printf("DEBUG: Failed to read %s! File doesn't exist.", fileLocation);
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
};

/* ----- OBJECTS ----- */

namespace obj {

    AudioManager *Audio() { return &audioM; }
    BitmapFont *Text(){ return &txt; }
    BuildingSprite *BSprite(){ return &bsprite; }
    CursorImage *Cursor() { return &cursor; }
    EmptyCircle *ECircle() { return &eCircle; }
    EmptyRectangle *ERectangle() { return &eRect; }
    FilledRectangle *FRectangle() { return &fRect; }
    ImageSprite *Img() { return &img; }
    UnitSprite *USprite() { return &usprite; }
    Terrain *MapTerrain() { return &terrain; }
    Grid *MapGrid() { return &grid; }
    
    void init() {
        //*Audio() = AudioManager();
        *Text() = BitmapFont();
        *BSprite() = BuildingSprite();
        *Cursor() = CursorImage();
        *ECircle() = EmptyCircle();
        *ERectangle() = EmptyRectangle();
        *FRectangle() = FilledRectangle();
        *Img() = ImageSprite();
        *USprite() = UnitSprite();
        *MapTerrain() = Terrain();
        *MapGrid() = Grid();
    }
    void compile() {
        Text()->compile();
        BSprite()->compile();
        ECircle()->compile();
        ERectangle()->compile();
        FRectangle()->compile();
        Cursor()->compile();
        Img()->compile();
        USprite()->compile();
        MapTerrain()->compile();
        MapGrid()->compile();
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
        Img()->create();
        USprite()->create();

        //----

        MapTerrain()->create();
        MapGrid()->create();
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
    }
    void applyGameMatrices(mat4 *proj, mat4 *view) {
        BSprite()->apply_projection_matrix(*proj);
        BSprite()->apply_view_matrix(*view);
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
