#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 FragTex;

uniform mat4 projection;
uniform mat4 view;
uniform float x;
uniform float y;

uniform int w;
uniform int h;

uniform int max_directions;
uniform int max_frames;
uniform int current_frame;
uniform int current_dir;
uniform int yOffset;

void main()
{
    gl_Position = projection * view * vec4(pos.x * w / max_frames + x, pos.y * h / max_directions + y + yOffset, pos.z, 1.0);
	FragTex = vec2(tex.x / float(max_frames) + float(current_frame) / float(max_frames), tex.y / float(max_directions) + float(current_dir) / float(max_directions));
}  