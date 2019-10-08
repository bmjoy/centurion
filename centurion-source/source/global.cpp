#include <global.hpp>
#include <surface>
#include <math>

/* ----- GLOBAL ----- */

using namespace std;
using namespace glm;

namespace glb {
	void setErrors(map<string, string> errorsMap) { errors = errorsMap; }
	string getErrorCode(string error) { return errors[error]; }
	void setParam(string param, float value) { params[param] = value; }
	float getParam(string param) { return params[param]; }
	void setBoolean(string param, bool value) { booleans[param] = value; }
	bool getBoolean(string param) { return booleans[param]; }

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
	int get_id() {
		unsigned char data[4];
		glReadPixels((GLint)glb::getParam("mouse-x-leftclick"), (GLint)glb::getParam("mouse-y-leftclick"), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
		int pickedID =
			data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;
		return pickedID;
	}
	vec3 getPickingColorFromID(int pickingID) {
		int r = (pickingID & 0x000000FF) >> 0;
		int g = (pickingID & 0x0000FF00) >> 8;
		int b = (pickingID & 0x00FF0000) >> 16;
		return vec3(r, g, b);
	}
	vec2 getZoomedCoords(float xCoord, float yCoord) {
		float x = xCoord * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position");
		float y = yCoord * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position");
		return vec2(x, y);
	}
	void forceGameClosure(string reason) {
		MessageBox(NULL, reason.c_str(), gameNameLPCSTR, MB_ICONERROR);
		GLB::WINDOW_CLOSE = true;
	}
	void showGameWarning(string reason) {
		MessageBox(NULL, reason.c_str(), gameNameLPCSTR, MB_ICONINFORMATION);
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
        Text()->apply_projection_matrix(GLB::MENU_PROJECTION);
        Cursor()->apply_projection_matrix(GLB::MENU_PROJECTION);

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
        Img()->apply_projection_matrix(GLB::MENU_PROJECTION);
        Img()->apply_view_matrix();
        ECircle()->apply_projection_matrix(GLB::MENU_PROJECTION);
        ECircle()->apply_view_matrix();
        ERectangle()->apply_projection_matrix(GLB::MENU_PROJECTION);
        ERectangle()->apply_view_matrix();
        FRectangle()->apply_projection_matrix(GLB::MENU_PROJECTION);
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
