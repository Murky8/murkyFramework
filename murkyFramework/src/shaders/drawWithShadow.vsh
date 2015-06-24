#version 400 core
layout(location = 0) in vec3 position_in;
layout(location = 1) in vec3 colour_in;
layout(location = 2) in vec2 textCoords_in;

out vec3 colour;
out vec2 textCoords;
out vec3 worldPos;

uniform mat4 projectionMat4;

void main()
{
  colour  = colour_in;	
  worldPos = position_in;
  textCoords = textCoords_in;
  gl_Position = projectionMat4*vec4(position_in, 1.0);		
}

//worldPos = position;