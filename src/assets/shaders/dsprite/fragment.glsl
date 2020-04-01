#version 330 core

in vec2 FragTex;

out vec4 FragColor;

uniform sampler2D texture1;
uniform int not_placeable;

void main()
{
    FragColor = texture(texture1, FragTex);
    if (FragColor.a <= 0.5f) discard; 
    
	if (not_placeable == 1) FragColor = vec4(FragColor.r, 0.f, 0.f, FragColor.a);
}
