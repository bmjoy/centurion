#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 FragTex;
out vec3 WorldPos;
out vec3 FragNorm;

vec2 pos_xy = vec2(pos.x, pos.y);
vec4 nPos;
float znoise;

uniform mat4 terrainProjection;
uniform mat4 terrainView;
uniform mat4 terrainModel;
uniform float scaleTextX;
uniform float scaleTextY ;


float hash(float n) {
    return fract(sin(n) * 753.5453123);
}

float snoise(vec3 x)
{
	vec3 p = floor(x);
	vec3 f = fract(x);
	f = f * f * (3.0f - (2.0f * f));

	float n = p.x + p.y * 157.0f + 113.0f * p.z;
	return mix(mix(mix(hash(n + 0.0f), hash(n + 1.0f), f.x),
		mix(hash(n + 157.0f), hash(n + 158.0f), f.x), f.y),
		mix(mix(hash(n + 113.0f), hash(n + 114.0f), f.x),
			mix(hash(n + 270.0f), hash(n + 271.0f), f.x), f.y), f.z);
}

float noise(vec3 position, int octaves, float frequency, float persistence) {
	float total = 0.0f;
	float maxAmplitude = 0.0f;
	float amplitude = 1.0f;
	for (int i = 0; i < octaves; i++) {
		total += snoise(position * frequency) * amplitude;
		frequency *= 2.0f;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}
	return total / maxAmplitude;
}

void main() 
{   
    float height = noise(pos.xyz, 2, 5.0, 4.0);
    
    
    // Noise to Z
    //float scale = 4.0;
    float heightScale = 0.1;
    
    //vec2 st = vec2(pos_xy.x*scale, pos_xy.y*scale);
    //znoise = (snoise(st)*0.5+0.5)*heightScale;
    nPos = vec4(pos_xy.x, pos_xy.y, height*heightScale, 1.0);
    
    gl_Position = terrainProjection * terrainView * terrainModel * nPos;
    
    // variables to fragment 
	FragTex = vec2(tex.x*scaleTextX, tex.y*scaleTextY);
    
    FragNorm = mat3(transpose(inverse(terrainModel))) * calculateNormal(nPos.xyz);
}
