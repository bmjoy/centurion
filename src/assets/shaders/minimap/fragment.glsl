#version 330 core

in vec2 FragTex;
out vec4 FragColor;

uniform sampler2D texture1;

void main(){
	FragColor = texture(texture1, FragTex);
	//FragColor = vec4(1);
}