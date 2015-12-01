struct VertexInputType
{
    float3 position : POSITION;
    float3 color : COLOR0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;    
};

cbuffer constantProjMat : register( b0 )
{
    matrix projectionMatrix;
};

PixelInputType mainvs(VertexInputType input)
{
   PixelInputType output;
   
    output.position = mul( projectionMatrix, float4(input.position, 1.f) );	
    output.color	= float4(input.color, 0.f);        
    return output;
}