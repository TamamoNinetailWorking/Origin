#include "Water.hlsli"

Texture2D g_Texture : register(t0);
Texture2D g_NormalTexture : register(t1);
SamplerState g_Sampler: register(s0);

float wave(float2 uv, float2 emitter, float speed, float phase)
{
	float dst = distance(uv, emitter);
	float temp = 0.5 + 0.5 * sin(dst * phase - Time * speed);
	return pow(temp, 2.0f);
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float2 position = input.Tex;

	float w = wave(position, float2(0.6, 0.11), Speed, 20);
	w += wave(position, float2(0.9, 0.6), Speed, 90);
	w += wave(position, float2(0.1, 0.84), Speed, 150.0);
	w += wave(position, float2(0.32, 0.81), Speed, 150.0);
	w += wave(position, float2(0.16, 0.211), Speed, 150.0);
	w += wave(position, float2(0.39, 0.46), Speed, 150.0);
	w += wave(position, float2(0.51, 0.484), Speed, 150.0);
	w += wave(position, float2(0.732, 0.91), Speed, 150.0);
	w *= 0.116 * Power;

	float2 tex = input.Tex;
	tex += w;

	float3 norm = normalize(g_NormalTexture.Sample(g_Sampler, tex).rgb * 2.0f - 1.0f);
	float2 uv = input.Tex + float2(norm.r,norm.b) * Distortion;
	uv.y = uv.y * (-1) + 1;

	float4 w_color = g_Texture.Sample(g_Sampler, uv);
	return saturate(w_color);
}