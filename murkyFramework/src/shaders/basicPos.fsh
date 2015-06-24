	#version 400 core

	in  vec3 colourx;
	//in  vec2 textCoords;
	//uniform sampler2D Tex1;
	out vec4 outputColor;

	void main()
	{
		//outputColor = vec4(colourx, 0.5)*texture( Tex1, textCoords );	
		outputColor = vec4(colourx, 1.0f);	
	}