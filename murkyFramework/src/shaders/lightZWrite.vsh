#version 400 core
layout(location = 0) in vec3 position;

uniform mat4 projectionMatrix3;

out vec3 colour;
//out vec3 worldPos;

void main()
{	
	gl_Position = projectionMatrix3*vec4(position, 1.0);		
}