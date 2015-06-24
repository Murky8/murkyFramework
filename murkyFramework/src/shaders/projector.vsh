#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour_in;

uniform mat4 projectionMatrix2;

out vec3 colour;
out vec3 worldPos;

void main()
{
  colour  = colour_in;	
  worldPos = position;
  gl_Position = projectionMatrix2*vec4(position, 1.0);		
}