#version 330 core

in vec3 frag_position;
in vec3 normal;
in vec2 text_coor;

uniform float u_time;
uniform vec2 u_mouse;
uniform vec2 u_resolution;

// for texture_manager
//uniform sampler2D texture0;
//uniform sampler2D texture1;

struct Material{
    sampler2D texture0;
    sampler2D specular;
    float shininess;
};

struct LightProperty{
    vec3 ambient_light;
    vec3 diffuse_light;
    vec3 specular_light;

    vec3 light_position;
};

uniform Material material;
uniform LightProperty light;

uniform vec3 camera_position;

float gaussian_mouse(vec2 pos, vec2 mouse, float sigma){
    vec2 distance = pos - mouse;
    return exp( - sigma * (distance.x * distance.x + distance.y * distance.y) );
}

void main()
{
    vec2 norm_xy = gl_FragCoord.xy / u_resolution;
    vec2 norm_mouse = u_mouse / u_resolution;

    vec3 color = texture(material.texture0, text_coor).rgb;
    vec3 specular_color = texture(material.specular, text_coor).rgb;

    color = mix(specular_color, color, step(specular_color.r + specular_color.g +specular_color.b, 0.0));
    
    // lighting part
    vec3 light_dir = normalize(frag_position - light.light_position);

    vec3 ambient = light.ambient_light * color;
    
    vec3 diffuse = light.diffuse_light * color * max(0.0, dot(normal, - light_dir));

    float spe_strength = max( dot(reflect(light_dir, normal), normalize(camera_position - frag_position)), 0.0);
    vec3 specular = light.specular_light * specular_color * pow( spe_strength, material.shininess);

    color =  (ambient + diffuse + specular);

    vec3 mouse_color = vec3(0.0, 0.0, 1.0);
    float mouse_gauss = gaussian_mouse(norm_xy, norm_mouse, 100.0);
    color = mix( color, mouse_color, mouse_gauss);

    gl_FragColor = vec4(color, 1.0);
}