#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 FragTex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform int max_directions;
uniform int max_frames;
uniform int current_frame;
uniform int current_dir;
uniform int yOffset;
//uniform float znoise;

float x, y;

void main()
{
    gl_Position = projection * view * model * vec4(pos.x, pos.y + yOffset, pos.z, 1.0);
	FragTex = vec2(tex.x / float(max_frames) + float(current_frame) / float(max_frames), tex.y / float(max_directions) + float(current_dir) / float(max_directions));
}  