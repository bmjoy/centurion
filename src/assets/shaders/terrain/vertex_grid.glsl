#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 FragTex;
//out vec2 FragTexGrid;

uniform float gridX;
uniform float gridY;

uniform mat4 projection;
uniform mat4 view;

out vec3 fragPos;

void main()
{
    gl_Position = projection * view * vec4(pos.x, pos.y, pos.z, 1.0);
	FragTex = vec2(tex.x, 1 - tex.y);
	fragPos = pos;
}  