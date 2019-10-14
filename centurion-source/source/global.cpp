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

		setParam("ui-bottom-height", 60.f);
		setParam("ui-top-height", 100.f);

		setBoolean("window-should-close", false);
		setBoolean("wireframe", false);
		setBoolean("mouse-left", false);
		setBoolean("mouse-right", false);
		setBoolean("mouse-release", false);
		setBoolean("mouse-scroll-bool", false);
		setParam("mouse-scroll", 0.f);

		setBoolean("up-key", false);
		setBoolean("down-key", false);
		setBoolean("right-key", false);
		setBoolean("left-key", false);
		setBoolean("esc-key", false);
		setBoolean("ctrl-key", false);


		menuProjection = glm::ortho(0.0f, getParam("window-width"), 0.0f, getParam("window-height"), -100.0f, 100.0f);
		cameraProjection = glm::ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)mapWidth, (float)mapWidth);

		ifstream data_path("assets/data/data.json");
		//Close the game if it wasn't able to find or process data.json file
		if (!data_path.good()) {
			forceGameClosure("NOT_FOUND" , "ERROR_data");
		}
		json data = json::parse(data_path);

		for (int i = 0; i < data["races"].size(); i++) {
			string race = data["races"][i].get<string>();
			glb::races.push_back(getTranslation("RACE_" + race));
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
		ifstream translationTable_path("assets/data/interface/texts/english.json");
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
			ifstream localTranslations_path("assets/data/interface/texts/" + language + ".json");
			if (localTranslations_path.good()) {
				json tJson = json::parse(localTranslations_path);
				map<string, string> localTranslationsMap = tJson.get<map<string, string>>();
				for (map<string, string>::iterator i = localTranslationsMap.begin(); i != localTranslationsMap.end(); i++) {
					replaceTranslation(i->first, i->second);
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
		string eC = getTranslation("WORD_errorCode") == "" ? "Error code" : getTranslation("WORD_errorCode");
		string text = "  " + eC + ": " + getErrorCode(errorCode) + "\n\n  " + getTranslation(errorText);
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
