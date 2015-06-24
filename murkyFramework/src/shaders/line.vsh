#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour_in;

uniform mat4 projectionMatrix;

out vec3 colour;

void main()
{
  colour  = colour_in;	
  gl_Position = projectionMatrix*vec4(position, 1.0);		
}