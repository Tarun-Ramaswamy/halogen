#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;

layout (location = 0) out vec3 out_frag_color;



void main()
{
    gl_Position = vec4(in_position.xyz, 1.0f);
    out_frag_color = in_color;
}
