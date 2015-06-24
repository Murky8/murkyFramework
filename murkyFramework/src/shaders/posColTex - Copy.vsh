#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour_in;
layout(location = 2) in vec2 textCoordsIn;

uniform mat4 projectionMatrix2;

out vec3 colour;
out vec2 textCoords;


void main()
{
	colour  = colour_in;	
	textCoords = textCoordsIn;	
	gl_Position = projectionMatrix2*vec4(position, 1.0);		
}