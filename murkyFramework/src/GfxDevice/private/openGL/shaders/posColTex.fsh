	#version 400 core
    
	in vec3 colour;
    in vec2 textCoords;
    uniform sampler2D textureSamplerID;
    out vec4 frag_colour;
     
    void main ()
    {
      frag_colour = vec4 (colour, 1.0)*texture( textureSamplerID, textCoords );
    }