#version 330 core

/*  
    ----- FRAGMENT SHADER -----
    Here we can render colors, textures, lights, shadows ...
    Vertices come all together: here we can do nothing to manipulate
    the position or other properties of EACH vertex.
*/

struct DirectionalLight 
{
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

/* To Rendering */

out vec4 FragColor;

/* From Vertex Shader */

in vec2 FragTex;
in vec2 FragTexGrass;
in vec3 FragNorm;
in float FragCol;
in float terrainType;

/* Uniform Variables Init */

uniform DirectionalLight directionalLight;
uniform int wireframe;

/* Tracing */

uniform int tracing;

/* Textures */

uniform sampler2D grass1;
uniform sampler2D road1;

/* Terrain Weights */

in float grassWeight;
in float roadWeight;
in float rockWeight;

vec4 terraintex;

/* ----- ----- ----- ----- */
/*      MAIN FUNCTION      */
/* ----- ----- ----- ----- */

void main()
{
    vec4 ambientColor = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
    
    float diffuseFactor = max(dot(FragNorm, normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
   
    vec4 terraintex = vec4((texture(grass1, FragTexGrass) * (ambientColor + diffuseColor) * (grassWeight) + texture(road1, FragTexGrass) * (ambientColor + diffuseColor) * (roadWeight)).xyz,1.0f);
    
    if (wireframe==0){
        if (tracing == 1){
            FragColor = vec4(FragCol,FragCol,FragCol,1.0);
        }
        else {
            FragColor = terraintex;
        }
    }
    else {
        if (tracing == 1){
            FragColor = vec4(FragCol,FragCol,FragCol,1.0);
        }
        else {

            FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
        }
    }
}