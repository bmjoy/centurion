#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 FragTex;

uniform mat4 projection;
uniform mat4 view;

uniform float x;
uniform float y;
uniform float z;
uniform float w;
uniform float h;
uniform int picking;
uniform int isLayerColor;
uniform int isBorder;

void main()
{
    float z_offset = z;
    if (picking == 1) z_offset = z - 3.f;
    if (isBorder == 1) z_offset = z - 2.f;
    if (isLayerColor == 1) z_offset = z - 1.f;
    

    gl_Position = projection * view * vec4(pos.x*w + x, pos.y*h + y, pos.z + y + z_offset, 1.0);
	FragTex = vec2(tex.x, tex.y);
}  









