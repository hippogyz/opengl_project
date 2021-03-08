#version 330 core

in vec3 frag_position;
in vec3 normal;
in vec2 text_coor;

uniform float u_time;
uniform vec2 u_mouse;
uniform vec2 u_resolution;

struct LightingMaterial{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
};

uniform LightingMaterial material;

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 camera_position;

uniform sampler2D texture0;
//uniform sampler2D texture1;

float gaussian_mouse(vec2 pos, vec2 mouse, float sigma){
    vec2 distance = pos - mouse;
    return exp( - sigma * (distance.x * distance.x + distance.y * distance.y) );
}

void main()
{
    vec2 norm_xy = gl_FragCoord.xy / u_resolution;
    vec2 norm_mouse = u_mouse / u_resolution;

    vec3 color = texture(texture0, text_coor).xyz;
    
    // lighting part
    vec3 light_dir = normalize(frag_position - light_position);

    vec3 ambient = light_color * material.ambient_color;
    
    vec3 diffuse = light_color * material.diffuse_color * max(0.0, dot(normal, - light_dir));

    float spe_strength = max( dot(reflect(light_dir, normal), normalize(camera_position - frag_position)), 0.0);
    vec3 specular = light_color * material.specular_color * pow( spe_strength, material.shininess);

    color =  (ambient + diffuse + specular);

    vec3 mouse_color = vec3(0.0, 0.0, 1.0);
    float mouse_gauss = gaussian_mouse(norm_xy, norm_mouse, 100.0);
    color = mix( color, mouse_color, mouse_gauss);

    gl_FragColor = vec4(color, 1.0);
}