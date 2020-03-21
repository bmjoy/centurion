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

void main()
{
    gl_Position = projection * view * vec4(pos.x * w + x, pos.y * h + y, z - y, 1);
	FragTex = vec2(tex.x, tex.y);
}  









