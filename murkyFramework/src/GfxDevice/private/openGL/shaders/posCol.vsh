#version 400 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_col;

uniform mat4 projectionMatrix;
out vec3 colour;

void main()
{
	colour = in_col;
	gl_Position = projectionMatrix*vec4(in_pos, 1.0);
}
