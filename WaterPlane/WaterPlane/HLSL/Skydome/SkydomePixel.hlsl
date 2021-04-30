#include "Skydome.hlsli"

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

float4 main(SkydomeVS_OUTPUT input) : SV_TARGET
{
	return g_Texture.Sample(g_Sampler,input.Tex);
}