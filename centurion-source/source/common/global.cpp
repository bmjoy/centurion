#include "global.h"

namespace glb {
    
    using namespace glb;
    
    void setParam(string param, float value){
        params[param] = value;
    }
    float getParam(string param){
        return params[param];
    }
};