#version 330 core

in vec2 FragTex;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 color;

void main()
{
    FragColor = vec4(color.xyz, texture(tex, FragTex).a);
}