#version 330 core

uniform vec4 color;

varying vec2 val;
uniform float w;
uniform float borderWidth;

void main()
{
    float R = 1.0; 
    float R2 = (w-borderWidth)/w;
    float dist = sqrt(dot(val,val)); 
    if (dist >= R || dist <= R2) {
        discard;
    }
    float sm = smoothstep(R,R-0.01,dist);
    float sm2 = smoothstep(R2,R2+0.01,dist);
    float alpha = sm*sm2*color.a;
    gl_FragColor = vec4(color.rgb, alpha);
}