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

/* From Vertex Shader */

in vec2 FragTex;
in vec2 FragTexScaled;
in vec3 FragNorm;
in float FragCol;
in float terrainWeight[20];

/* Uniform Variables Init */

uniform int nTerrains;
uniform DirectionalLight directionalLight;
uniform int wireframe;

/* Tracing */

uniform int tracing;

/* Textures */

uniform sampler2D sampleTex[20];

/* ----- ----- ----- ----- */
/*      MAIN FUNCTION      */
/* ----- ----- ----- ----- */

void main()
{   
    vec4 ambientColor = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;    
    float diffuseFactor = max(dot(FragNorm, normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
       
    vec4 terrainTexture = vec4(0);
    
    for (int i = 0; i < nTerrains; i++){
        terrainTexture += texture(sampleTex[i], FragTexScaled) * (ambientColor + diffuseColor) * terrainWeight[i];
    }
   
    terrainTexture = vec4(terrainTexture.xyz, 1.f);
    
    if (wireframe==0){
        if (tracing == 1){
            gl_FragColor = vec4(FragCol,FragCol,FragCol,1.0);
        }
        else {
            gl_FragColor = terrainTexture;
        }
    }
    else {
        if (tracing == 1){
            gl_FragColor = vec4(FragCol,FragCol,FragCol,1.0);
        }
        else {

            gl_FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
        }
    }
}