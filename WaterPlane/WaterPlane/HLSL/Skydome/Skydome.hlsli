#include "../Common.hlsli"

cbuffer Skydome_Buffer : register(b1)
{
	matrix world;
}

struct SkydomeVS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};