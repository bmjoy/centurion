#version 330 core

in vec2 FragTex;

uniform sampler2D texture1;
uniform int not_placeable;

void main()
{
    vec4 FragColor = texture(texture1, FragTex);
    if (FragColor.a <= 0.8f) discard; 
    
	if (not_placeable == 1) gl_FragColor = vec4(FragColor.r, 0.f, 0.f, FragColor.a);

    if (not_placeable == 0) gl_FragColor = FragColor;
}
