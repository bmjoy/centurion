#version 330 core

/*  
    ----- VERTEX SHADER -----
    Here we can work on the position of EACH single vertex;
    Moreover, it's possible to prepare stuff for the fragment shader
    which is compiled AFTER the vertex shader.
    For example, we prepare normals and texture coordinates.
    the "layout( location = .)" variables come directly from the .cpp code
    during mesh creation (look for glBindVertexArray and glVertexAttribPointer)
*/

layout (location = 0) in vec4 pos;  // {[pos.x,pos.y,pos.z] pos.w}  the .w is the zNoise
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in float terrain;

vec4 newPos;

/* Noise stuff */

float zNoise;
float zScale;
float xyScale;
float zOffset;

/* Uniform Variables Init */

uniform mat4 projection;
uniform mat4 view;
uniform float scaleTextX;
uniform float scaleTextY ;

/* Tracing */

uniform float minZ;
uniform float maxZ;
uniform int tracing;

/* To Fragment Shader Init */

out vec2 FragTex;
out vec2 FragTexGrass;
out vec3 FragNorm;
out float FragCol;

/* Terrain */

out float grassW, roadW;

/* ----- ----- ----- ----- */

// this function calculate the value of the equation
// of the ellipse (x-cx)^2 / r^2 + ((1.5))*(y-cy)^2 / r^2 = 1  
// 1.5 --> because the plane is inclined 45 degrees and then scaled x2 on y
// and we want a circular form on the map for each townhall

float distEllipse(float x, float y, float cx, float cy, float r){
    return ((x-cx)*(x-cx)/(r*r) + 1.5*(y-cy)*(y-cy)/(r*r));
}

/* ----- ----- ----- ----- */
/*      MAIN FUNCTION      */
/* ----- ----- ----- ----- */

void main() 
{   
   
    /* RENDERING */
    
    newPos = vec4(pos.x, pos.y, pos.z, 1.0);
    gl_Position = projection * view * newPos;
    
    /* To Fragment Shader */
    
    FragTex = tex;
	FragTexGrass = vec2(tex.x*scaleTextX, tex.y*scaleTextY);
    FragNorm = norm;
    FragCol = (pos.w - minZ) / (maxZ - minZ) ; // in (0-1)
    
    if(terrain == 1.f){ // grass
        grassW = 1.f;
        roadW = 0.f;
    }
    
    if(terrain == 2.f){ // road
        grassW = 0.f;
        roadW = 1.f;
    }
}