#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 FragTex;

uniform mat4 projection;
uniform mat4 model;

uniform float current_x;
uniform float current_y;
uniform float max_x;
uniform float max_y;

void main()
{
    gl_Position = projection * model * vec4(pos.xy, 0.0, 1.0);
	FragTex = vec2(tex.x / max_x + current_x / max_x, tex.y / max_y + current_y / max_y);
}  