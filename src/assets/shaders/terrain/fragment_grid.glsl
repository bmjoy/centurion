#version 330 core

in vec2 FragTex;
out vec4 FragColor;

uniform sampler2D grid;
uniform sampler2D grid_color;

in vec3 fragPos;

void main()
{
	FragColor = texture(grid_color, FragTex);
}