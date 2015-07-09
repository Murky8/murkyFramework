struct VertexInputType
{
    float3 position : POSITION;
    float3 color : COLOR0;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 tex : TEXCOORD0;
};

PixelInputType mainvs(VertexInputType input)
{
   PixelInputType output;
        
    output.position = float4(input.position, 1.f);	
    output.color	= float4(input.color, 0.f);
    output.tex		= input.tex;
    
    return output;
}