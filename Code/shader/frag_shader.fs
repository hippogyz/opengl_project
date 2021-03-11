#version 330 core

in vec3 frag_position;
in vec3 normal_vector;
in vec2 text_coor;

uniform float u_time;
uniform vec2 u_mouse;
uniform vec2 u_resolution;

// for texture_manager
//uniform sampler2D texture0;
//uniform sampler2D texture1;

struct Material{
    sampler2D default_texture;
    sampler2D specular_texture;
    float ambient;
    float shininess;
};

struct FragMaterial{
    vec4 default_text;
    vec4 specular_text;
    float ambient;
    float shininess;
};

struct DirLight{
    bool is_active;

    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    bool is_active;

    vec3 position;
    vec3 diffuse;
    vec3 specular;

    float decay_0;
    float decay_1;
    float decay_2;
};

struct SpotLight{
    bool is_active;
    
    vec3 position;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;

    float decay_0;
    float decay_1;
    float decay_2;

    float inner_cone;
    float outer_cone;
};

vec3 cal_dirlight( DirLight lig, FragMaterial mat, vec3 norm_vec, vec3 frag_pos, vec3 view_dir );
vec3 cal_pointlight( PointLight lig, FragMaterial mat, vec3 norm_vec, vec3 frag_pos, vec3 view_dir );
vec3 cal_spotlight( SpotLight lig, FragMaterial mat, vec3 norm_vec, vec3 frag_pos, vec3 view_dir );

float gaussian_mouse(vec2 pos, vec2 mouse, float sigma);


uniform Material material;

#define POINT_LIGHT_NUM 4
uniform PointLight point_light[POINT_LIGHT_NUM];
uniform DirLight dir_light;
uniform SpotLight spot_light;

uniform vec3 camera_position;

void main()
{
    vec2 norm_xy = gl_FragCoord.xy / u_resolution;
    vec2 norm_mouse = u_mouse / u_resolution;

    vec4 default_color = texture(material.default_texture, text_coor);
    vec4 specular_color = texture(material.specular_texture, text_coor);
    default_color = mix(specular_color, default_color, 
        step(specular_color.r + specular_color.g +specular_color.b, 0.0));
    
    FragMaterial frag_material = FragMaterial( 
        default_color,
        specular_color,
        material.ambient,
        material.shininess
    );

    // calculate lighting
    vec3 color = frag_material.default_text.rgb * frag_material.ambient;

    vec3 view_direction = frag_position - camera_position;
    for( int i = 0; i < POINT_LIGHT_NUM; ++i )
    {
        color += cal_pointlight(point_light[i], frag_material, 
            normal_vector, frag_position, view_direction);
    }
    color += cal_dirlight(dir_light, frag_material,
            normal_vector, frag_position, view_direction);
    color += cal_spotlight(spot_light, frag_material,
            normal_vector, frag_position, view_direction);
    

    vec3 mouse_color = vec3(0.0, 0.0, 1.0);
    float mouse_gauss = gaussian_mouse(norm_xy, norm_mouse, 100.0);
    color = mix( color, mouse_color, mouse_gauss);

    gl_FragColor = vec4(color, 1.0);
}




///////////////////////////////////

vec3 cal_dirlight( DirLight lig, FragMaterial mat, vec3 norm_vec, vec3 frag_pos, vec3 view_dir ){
    if(!lig.is_active)
        return vec3(0.0);

    vec3 n_dir = normalize(norm_vec);
    vec3 l_dir = normalize(lig.direction);
    vec3 v_dir = normalize(view_dir);

    vec3 dif = lig.diffuse * mat.default_text.rgb * max( dot(n_dir, -l_dir), 0.0 );
    
    float spec_dot = max( dot(reflect(l_dir, n_dir), -v_dir), 0.0);
    vec3 spec = lig.specular * mat.specular_text.rgb * pow(spec_dot, mat.shininess);

    return dif + spec;
}

vec3 cal_pointlight( PointLight lig, FragMaterial mat, vec3 norm_vec, vec3 frag_pos, vec3 view_dir ){
    if(!lig.is_active)
        return vec3(0.0);

    vec3 n_dir = normalize(norm_vec);
    vec3 l_dir = normalize(frag_pos - lig.position);
    vec3 v_dir = normalize(view_dir);

    vec3 dif = lig.diffuse * mat.default_text.rgb * max( dot(n_dir, -l_dir), 0.0 );

    float spec_dot = max( dot(reflect(l_dir, n_dir), -v_dir), 0.0);
    vec3 spec = lig.specular * mat.specular_text.rgb * pow(spec_dot, mat.shininess);

    float dist = distance( lig.position, frag_pos );
    float decay = 1.0 / ( lig.decay_0 + lig.decay_1 * dist + lig.decay_2 * dist*dist);

    return (dif + spec) * decay;
}

vec3 cal_spotlight( SpotLight lig, FragMaterial mat, vec3 norm_vec, vec3 frag_pos, vec3 view_dir ){
    if(!lig.is_active)
        return vec3(0.0);

    vec3 n_dir = normalize(norm_vec);
    vec3 l_dir = normalize(frag_pos - lig.position);
    vec3 v_dir = normalize(view_dir);

    vec3 dif = lig.diffuse * mat.default_text.rgb * max( dot(n_dir, -l_dir), 0.0 );

    float spec_dot = max( dot(reflect(l_dir, n_dir), -v_dir), 0.0);
    vec3 spec = lig.specular * mat.specular_text.rgb * pow(spec_dot, mat.shininess);

    float dist = distance( lig.position, frag_pos );
    float decay = 1.0 / ( lig.decay_0 + lig.decay_1 * dist + lig.decay_2 * dist*dist);

    float spot_cone = dot( l_dir, normalize(lig.direction) );
    float cone_decay = clamp( (spot_cone - lig.outer_cone)/(lig.inner_cone - lig.outer_cone) , 0.0, 1.0 );
    decay *= cone_decay;

    return (dif + spec) * decay;
}

float gaussian_mouse(vec2 pos, vec2 mouse, float sigma){
    vec2 distance = pos - mouse;
    return exp( - sigma * (distance.x * distance.x + distance.y * distance.y) );
}