#version 330 core

in vec2 FragTex;
out vec4 FragColor;

uniform sampler2D grid;

in vec3 fragPos;

void main()
{
	vec4 color = texture(grid, FragTex);
	
	float value = color.r;
	
	// Passable
	if (value == 0)
	{
		FragColor = vec4(0.f, 1.f, 0.f, 0.25f);
	}
	
	// Not Passable
	else if (value == 1.f/255.f)
	{
		FragColor = vec4(1.f, 0.f, 0.f, 0.25f);
	}
}