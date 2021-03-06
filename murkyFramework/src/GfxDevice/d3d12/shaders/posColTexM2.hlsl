//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color	: COLOR;
    float2 texcoord	: TEXCOORD;
};

Texture2D		g_texture : register(t0);
SamplerState	g_sampler : register(s0);

cbuffer cb0
{
    row_major float4x4 g_mWorldViewProj;
    row_major float4x4 g_mWorldViewProj2;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
    PSInput result;

    //output.pos = mul(float4(position, 1.0), g_mWorldViewProj);
    //result.position	= position;
    result.position = mul(float4(position), g_mWorldViewProj2);
    result.color	= color;
    result.texcoord	= texcoord;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    //return input.color;
    return g_texture.Sample(g_sampler, input.texcoord);
}
