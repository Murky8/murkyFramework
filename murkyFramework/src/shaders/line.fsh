#version 400 core

in  vec3 colour;
out vec4 color_out;

void main()
{
  color_out = vec4(colour, 1.0f);  
}