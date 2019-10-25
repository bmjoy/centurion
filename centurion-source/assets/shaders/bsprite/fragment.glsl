#version 330 core

in vec2 FragTex;
out vec4 FragColor;

uniform sampler2D texture1;
uniform int isLayerColor;
uniform int picking;
uniform int selection;
uniform int minimap;
uniform int pass;
uniform int not_placeable;
uniform vec3 player_color;
uniform vec4 picking_color;

void main()
{
    /* Pass */
    
    if (pass == 1){
        FragColor = texture(texture1, FragTex);
    }
    
    /* Picking */
    
    if (picking == 1){
        FragColor = texture(texture1, FragTex).a * picking_color;
    }
    
    else {
        
        /* Camera Rendering */
        
        if (minimap == 0){
            
            if (selection == 1) {
                FragColor = texture(texture1, FragTex) + vec4(0.2f, 0.2f, 0.2f, 0.0f);
            }
            else {
                FragColor = texture(texture1, FragTex);
            }
            
            if (not_placeable == 1) {
                FragColor = vec4(texture(texture1, FragTex).r, 0.f, 0.f, texture(texture1, FragTex).a);
            }
            
        }
        
        /* Minimap Rendering */ 
        
        else {
            if (selection == 1 && isLayerColor == 1) {
                FragColor = texture(texture1, FragTex).a * vec4(1.0f, 1.0f, 1.0f, 1.0f);
            }
            if (selection == 0 && isLayerColor == 1){
                FragColor = texture(texture1, FragTex).a * vec4(player_color, 1.0f);
            }
            if (isLayerColor == 0){
                FragColor = texture(texture1, FragTex);
            }
        }
        
    }
}
