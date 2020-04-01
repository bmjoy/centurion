#version 330 core

in vec2 FragTex;
vec4 FragColor;

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
    
    FragColor = texture(texture1, FragTex);
    if (FragColor.a <= 0.1f) discard; 
    
    if (pass == 1) gl_FragColor = FragColor;
    
    /* Picking */    
    
    if (picking == 1) gl_FragColor = FragColor.a * picking_color;
    
    else {
        
        /* Camera Rendering */
        
        if (minimap == 0){

            if (selection == 1) gl_FragColor = FragColor + vec4(0.2f, 0.2f, 0.2f, 0.0f);     
           
            if (selection == 0) gl_FragColor = FragColor;
               
            if (not_placeable == 1) gl_FragColor = vec4(FragColor.r, 0.f, 0.f, FragColor.a);
            
        }
        
        /* Minimap Rendering */ 
        
        else {
        
            if (selection == 1 && isLayerColor == 1) gl_FragColor = FragColor.a * vec4(1.0f, 1.0f, 1.0f, 1.0f);
                
            if (selection == 0 && isLayerColor == 1) gl_FragColor = FragColor.a * vec4(player_color, 1.0f);
           
            if (isLayerColor == 0) gl_FragColor = FragColor;   
        
        }
        
    }
}
