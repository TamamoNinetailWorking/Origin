#include "Wave.hlsli"

Texture2D g_Texture : register(t0);
Texture2D g_NormalTexture : register(t1);
TextureCube g_Reflect : register(t2);
Texture2D g_TestTexture : register(t3);
SamplerState g_Sampler: register(s0);

float wave(float2 uv, float2 emitter, float speed, float phase)
{
	float dst = distance(uv, emitter);
	float temp = 0.5 + 0.5 * sin(dst * phase - Time * speed);
	return pow(temp, 2.0f);
}

float specular(float4 N, float4 H)
{
	return pow(max(0, dot(N, H)), 1.0f);
}

float4 directionalLight(float4 N, float4 L, float4 EyeVector)
{
	float D = saturate(dot(N, L));
	float4 reflect = normalize(2 * D * N - EyeVector);
	return pow(saturate(dot(reflect, EyeVector)), 50);
}

float4 main(DS_OUTPUT input) : SV_TARGET
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

	float4 N = normalize(input.Normal);
	float4 L = normalize(float4(LightDir - input.WPos.xyz,0.0f));
	float4 V = normalize(CameraPos - input.WPos);
	float4 H = normalize(L + V);

	float s = specular(N, H);
	float4 Specular = float4(1, 1, 1, 1) * specular(N,H);

	float4 w_color = g_Texture.Sample(g_Sampler, uv);
	//return saturate(w_color * diffuse + Specular);


	float3 Tex;
	float3 EyeRay = -V;
	float3 ReflectRay = reflect(EyeRay, N);
	Tex = N;
	//Tex.x = ReflectRay.x * 0.5f + 0.5f;
	//Tex.y = ReflectRay.y * 0.5f + 0.5f;
	//Tex.z = ReflectRay.z * 0.5f + 0.5f;
	float4 color = g_Reflect.Sample(g_Sampler, Tex);
	float4 light = directionalLight(N, float4(LightDir, 0.0f), -V);
	//float4 light = Specular;
	color = saturate(color * diffuse + light);
	//color.a = 0.4f;
	color = w_color * 0.2f + color * 0.8f;
	color.a = 0.6f;
	return color;
	//return Specular;
	//return N * 0.5f + 0.5f;
}