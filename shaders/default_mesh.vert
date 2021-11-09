#version 450

#extension GL_KHR_vulkan_glsl : enable

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;

layout (location = 0) out vec3 frag_color;

void main()
{
	gl_Position = vec4(in_position, 1.0f);

	frag_color = in_color;
}