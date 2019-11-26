#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 FragTex;

uniform mat4 projection;
uniform mat4 view;
uniform float x;
uniform float y;
uniform float w;
uniform float h;
uniform float ratioX;
uniform float ratioY;
uniform int origin;
uniform int repeat;

float x1, y1;

void main()
{   
    if (origin == 0){ // bottom-left
        x1 = pos.x * w + x;
        y1 = pos.y * h + y;
    }

    if (origin == 1){ // center
        x1 = pos.x * w + x - w / 2.f;
        y1 = pos.y * h + y - h / 2.f;
    }
    
    if (origin == 2){ // top-left
        x1 = pos.x * w + x;
        y1 = pos.y * h + y - h;
    }
    
    gl_Position = projection * view * vec4(x1, y1, pos.z, 1.0);
	
	FragTex = tex;
	if (repeat == 1){
		FragTex = vec2(tex.x * ratioX, tex.y * ratioY);
	}
}  