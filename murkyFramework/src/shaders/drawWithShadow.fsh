#version 400 core

in  vec3 colour;
in  vec2 textCoords;
in  vec3 worldPos;

out vec4 colour_out;
uniform sampler2D text;
uniform sampler2D depth;

uniform mat4 lightProjectionMat;

void main()
{
	vec4 p2;  
	p2 = lightProjectionMat*vec4(worldPos, 1.0f); 	
	float lightDepth = textureProj(depth, vec3(p2.x, p2.y, p2.z )).x;//divde x, y by z
	/*
	lightDepth += textureProj(depth, vec3(p2.x+0.01f, p2.y, p2.z )).x;
	lightDepth += textureProj(depth, vec3(p2.x-0.01f, p2.y, p2.z )).x;
	lightDepth += textureProj(depth, vec3(p2.x, p2.y+0.01f, p2.z )).x;
	lightDepth += textureProj(depth, vec3(p2.x, p2.y-0.01f, p2.z )).x;
	lightDepth *= (1.0f/5.0f);
	*/

	// convert windows coords for Z into real Z
	float n = 1.0f;
	float f = 30.0f;
	float ptDepth = (1.0f/p2.z)*(-2.0f*f*n)/(f-n) +(f+n)/(f-n);
	
	//normalise -1 <-> 1  to 0 <-> 1
	ptDepth = ptDepth/2.0f +0.5f;	

	if(ptDepth>lightDepth+0.001f)
	{// in shadow
		colour_out = vec4(0.3,0.3,0.3,0);		
	}
	else
	{// in light	
		colour_out = vec4(0.9,0.9,0.9,0) +0.01f*texture(text, textCoords);			
	}
}
	