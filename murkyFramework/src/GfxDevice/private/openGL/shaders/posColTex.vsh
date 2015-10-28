#version 400 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_col;
layout(location = 2) in vec2 in_textCoords;
out vec3 colour;
out vec2 textCoords;
uniform mat4 projMatrix;

void main()
{
	colour		= in_col;	
	textCoords	= in_textCoords;	
	gl_Position = projMatrix * vec4(position, 1.0);		
}