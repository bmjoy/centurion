#version 330 core

out vec4 FragColor;

uniform vec4 color;
uniform vec4 picking_color;
uniform int picking;

void main()
{
    
    if (picking == 1){
        FragColor = picking_color;
    }
    else {
        FragColor = color;
    }
}