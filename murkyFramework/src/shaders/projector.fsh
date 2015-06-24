#version 400 core

in  vec3 colour;
in  vec3 worldPos;
in  vec2 textCoords;

out vec4 color_out;
uniform sampler2D Tex;
uniform mat4 lightProjMatrix;

void main()
{
	vec4 p2;  
	p2 = lightProjMatrix*vec4(worldPos, 1.0f); 	
	color_out = textureProj(Tex, vec3(p2.x, p2.y, p2.z ));	//divde x, y by z
}
	