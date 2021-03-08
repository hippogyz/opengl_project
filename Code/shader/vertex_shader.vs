#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm_vec;
layout (location = 2) in vec2 texture_uv;

out vec3 frag_position;
out vec3 normal;
out vec2 text_coor;

uniform mat4 model;
uniform mat4 norm_mat;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0);
    frag_position = vec3( model * vec4(pos, 1.0) );
    normal = normalize(mat3(norm_mat) * norm_vec);
    text_coor = texture_uv;
}