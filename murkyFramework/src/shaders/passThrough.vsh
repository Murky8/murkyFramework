#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec2 textCoordsIn;
out vec3 colourx;
out vec2 textCoords;

void main()
{
colourx = colour;	
textCoords = textCoordsIn;	
gl_Position = vec4(position, 1.0);		
}