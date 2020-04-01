#version 330 core

in vec2 FragTexGrid;
in vec2 FragTex;
out vec4 FragColor;

uniform sampler2D grid;
uniform sampler2D grid_color;

//uniform int path[1000];
//uniform int path_length;


in vec3 fragPos;

void main()
{
//	int x = int(fragPos.x/20.f);
//	int y = int(fragPos.y/20.f);

	FragColor = texture(grid_color, FragTex);

//	for (int i = 0; i < path_length; i++){
//		
//		if (x == path[i*2] && y == path[i*2+1]){
//			FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
//		}
//
//	}

}