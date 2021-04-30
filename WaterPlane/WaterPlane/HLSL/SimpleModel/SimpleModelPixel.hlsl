#include "SimpleModel.hlsli"

Texture2D g_Texture : register(t0);
Texture2D g_Toon : register(t1);
SamplerState g_Sampler: register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return g_Texture.Sample(g_Sampler,input.Tex) * input.Diffuse + input.Specular;
	//return input.Specular;
	//return input.Diffuse;
}

float4 Line() :SV_TARGET
{
	return float4(1,0,1,1);
}

float4 sub(VS_OUTPUT input) : SV_TARGET
{
	return input.Diffuse + input.Specular;
}


float4 pplmain(PPLVS_OUTPUT input):SV_TARGET
{
	float4 N = normalize(input.WNormal);
	float4 L = normalize(Light - input.WPos);
	float4 V = normalize(CameraPos - input.WPos);
	float4 H = normalize(L + V);

	float Dot = dot(L,N);
	Dot = Dot * 0.5 + 0.5;
	Dot = Dot * Dot;
	//float4 Diffuse = mat_diffuse * diffuse * max(0, dot(L, input.WNormal));
	//float4 Diffuse = mat_diffuse * diffuse * Dot;
	float4 Diffuse = mat_diffuse * diffuse;
	float4 Ambient = mat_ambient * ambient;
	float s = pow(max(0, dot(N, H)), 50);
	float4 Specular = mat_specular * specular * s;
	
	//float2 toonuv = float2(Dot, Dot);
	//float4 toon = g_Toon.Sample(g_Sampler, toonuv);

	//float4 col = float4(1,1,1,1);
	//if (input.WPos.y >= WaterHeight.y)
	//{
	//	input.Color.a = 0;
	//}
	
	float4 dif_amb = Diffuse + Ambient;
	float4 texColor = g_Texture.Sample(g_Sampler, input.Tex) * dif_amb + Specular;
	//texColor *= toon;
	return texColor;
	//return Specular;
}

float4 pplsub(PPLVS_OUTPUT input) :SV_TARGET
{
	float4 N = normalize(input.WNormal);
	float4 L = normalize(Light - input.WPos);
	float4 V = normalize(CameraPos - input.WPos);
	float4 H = normalize(L + V);

	float Dot = dot(L,N);
	Dot = Dot * 0.5 + 0.5;
	Dot = Dot * Dot;
	//float4 Diffuse = mat_diffuse * diffuse * max(0, dot(L, input.WNormal));
	//float4 Diffuse = mat_diffuse * diffuse * Dot;
	float4 Diffuse = mat_diffuse * diffuse;
	float4 Ambient = mat_ambient * ambient;
	float s = pow(max(0, dot(N, H)), 50);
	float4 Specular = mat_specular * specular * s;

	float2 toonuv = float2(Dot, Dot);
	float4 toon = g_Toon.Sample(g_Sampler, toonuv);

	//float4 col = float4(1,1,1,1);
	//if (input.WPos.y >= WaterHeight.y)
	//{
	//	input.Color.a = 0;
	//}

	float4 dif_amb = Diffuse + Ambient;
	float4 texColor = dif_amb + Specular;
	texColor *= toon;
	return texColor;
	//return Specular;
}

float4 CullingPPLMain(PPLVS_OUTPUT input) :SV_TARGET
{
	float4 L = normalize(Light - input.WPos);
	float4 V = normalize(CameraPos - input.WPos);
	float4 H = normalize(L + V);

	float Dot = dot(L,input.WNormal);
	Dot = Dot * 0.5 + 0.5;
	Dot = Dot * Dot;
	//float4 Diffuse = mat_diffuse * diffuse * max(0, dot(L, input.WNormal));
	float4 Diffuse = mat_diffuse * diffuse * Dot;
	float4 Ambient = mat_ambient * ambient;
	float4 Specular = mat_specular * specular * pow(max(0, dot(input.WNormal, H)), 10);

	float4 dif_amb = Diffuse + Ambient;
	float4 texColor = g_Texture.Sample(g_Sampler, input.Tex) * dif_amb + Specular;
	//texColor.a = input.Color.a;
	return texColor * input.Color;
	//return Specular;
}