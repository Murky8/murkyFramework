	#version 400 core

	in  vec3 colourx;
	in  vec2 textCoords;
	uniform sampler2D Tex1;
	out vec4 outputColour;
	
	void main()
	{
		float txc = texture(Tex1, textCoords).r;	
		

		//txc += texture(Tex1, vec2(textCoords.x+0.01f, textCoords.y)).r;	
		//txc += texture(Tex1, vec2(textCoords.x-0.01f, textCoords.y)).r;	
		//txc += texture(Tex1, vec2(textCoords.x, textCoords.y+0.01f)).r;	
		//txc += texture(Tex1, vec2(textCoords.x, textCoords.y-0.01f)).r;	
		//txc *=(1.0f/5.0f);
			
		outputColour = txc;			
	}

		
