#version 330 core

in vec3 frag_position;
in vec3 normal_vector;
in vec2 text_coor;

uniform float u_time;
uniform vec2 u_mouse;
uniform vec2 u_resolution;

float gaussian_mouse(vec2 pos, vec2 mouse, float sigma);

void main()
{
    vec2 norm_xy = gl_FragCoord.xy / u_resolution;
    vec2 norm_mouse = u_mouse / u_resolution;

    vec3 color =  vec3(1.0,1.0,1.0);

    vec3 mouse_color = vec3(0.0, 0.0, 1.0);
    float mouse_gauss = gaussian_mouse(norm_xy, norm_mouse, 100.0);
    color = mix( color, mouse_color, mouse_gauss);

    gl_FragColor = vec4(color, 1.0);
}


float gaussian_mouse(vec2 pos, vec2 mouse, float sigma){
    vec2 distance = pos - mouse;
    return exp( - sigma * (distance.x * distance.x + distance.y * distance.y) );
}