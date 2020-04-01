#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 FragTex;

uniform mat4 projection;
uniform float x;
uniform float y;

void main()
{
    gl_Position = projection * vec4(pos.x + x, pos.y + y, pos.z, 1.0);
	FragTex = tex;
}  