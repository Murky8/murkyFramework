struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelInputType mainvs(VertexInputType input)
{
   PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
	// Calculate the position of the vertex against the world, view, and projection matrices.
    //output.position = mul(input.position, worldMatrix);
    //output.position = mul(output.position, viewMatrix);

    input.position.w = 1.0f;
    output.position = input.position;	
	output.tex		= input.tex;
    
    return output;
}