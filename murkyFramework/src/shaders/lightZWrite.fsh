#version 400 core

//in  vec3 worldPos;
out vec4 color_out;

void main()
{	
	float i =  gl_FragCoord.z;
	if(i<0.01)
		i=1.0f;
	else
		if(i>0.99)
		i=0.0f;

	color_out = vec4( i, i, i, 1.0f);  
}