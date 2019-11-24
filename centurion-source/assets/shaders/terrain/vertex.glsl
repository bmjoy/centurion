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

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in float zNoise;
layout (location = 3) in vec3 norm;
layout (location = 4) in float terrain;

/* Uniform Variables Init */

uniform mat4 projection;
uniform mat4 view;
uniform float scaleTextX;
uniform float scaleTextY ;
uniform int minimap;

/* Tracing */

uniform float minZ;
uniform float maxZ;
uniform int tracing;

/* To Fragment Shader Init */

out vec2 FragTex;
out vec2 FragTexScaled;
out vec3 FragNorm;
out float FragCol;

/* Terrain */
uniform int nTerrains;
out float terrainWeight[20];
out float terrainType;

/* ----- ----- ----- ----- */
/*      MAIN FUNCTION      */
/* ----- ----- ----- ----- */

void main() 
{   
    int terrainID = int(terrain) - 1;
    if (minimap == 0){
        for (int i = 0; i < nTerrains; i++){
            terrainWeight[i] = 0.f;
        }
        terrainWeight[terrainID] = 1.f;
    }
    else {
        terrainType = float(terrainID);
    }
    
    /* RENDERING */
    int offset = 256; 
    vec4 newPos = vec4(pos.x - offset, pos.y + zNoise - offset, pos.y *(-1.f) + zNoise - 1000, 1.0);
    gl_Position = projection * view * newPos;
    
    /* To Fragment Shader */
    
    FragTex = tex;
	FragTexScaled = vec2(tex.x*scaleTextX, tex.y*scaleTextY);
    FragNorm = norm;
    FragCol = (zNoise - minZ) / (maxZ - minZ) ; // in (0-1)
}
