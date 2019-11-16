#version 330 core

in vec2 FragTex;

uniform sampler2D texture1;

void main()
{
    vec4 FragColor = texture(texture1, FragTex);
    //if (FragColor.a <= 0.1f) discard; 
    
    gl_FragColor = FragColor;
}
