#pragma once

#include <headers>

namespace glb {

    using namespace std;
    
    static map<string, float> params;
    void setParam(string param, float value);
    float getParam(string param);

	static map<string, string> errors;
	void setErrors(map<string, string> errorsMap);
	string getErrorCode(string error);
	
	/* reading/saving map data */
	void readIndicesData(unsigned int *indices, string path);
	void readVerticesData(float *vertices, string path);
	void readVerticesPosData(int *verticesPos, string path);
};