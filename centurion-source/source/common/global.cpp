#include "global.h"

namespace glb {
    
    using namespace glb;
    
    void setParam(string param, float value){
        params[param] = value;
    }
    float getParam(string param){
        return params[param];
    }
	void setErrors(map<string, string> errorsMap) {
		errors = errorsMap;
	}
	string getErrorCode(string error) {
		return errors[error];
	}
	void readIndicesData(unsigned int *indices, string path) {
		std::fstream fin;
		fin.open(path);
		std::string line, number;
		std::getline(fin, line);
		std::stringstream s(line);
		int i = 0;
		while (std::getline(s, number, ',')) {
			indices[i] = (unsigned int)std::stoi(number);
			i++;
		}
	}
	void readVerticesData(float *vertices, string path) {
		std::fstream fin;
		fin.open(path);
		std::string line, number;
		std::getline(fin, line);
		std::stringstream s(line);
		int i = 0;
		while (std::getline(s, number, ',')) {
			vertices[i] = std::stof(number);
			i++;
		}
	}
	void readVerticesPosData(int *verticesPos, string path) {
		std::fstream fin;
		fin.open(path);
		std::string line, number;
		std::getline(fin, line);
		std::stringstream s(line);
		int i = 0;
		while (std::getline(s, number, ',')) {
			verticesPos[i] = std::stoi(number);
			i++;
		}
	}
};