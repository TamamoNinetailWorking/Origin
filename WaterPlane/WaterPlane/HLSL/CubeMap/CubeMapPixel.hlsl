#include "CubeMap.hlsli"

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

float4 main(PS_INPUT input) : SV_Target
{
	return g_Texture.Sample(g_Sampler, input.Tex);
	//return float4(1,0,0,1);
}