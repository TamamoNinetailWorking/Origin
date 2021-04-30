#include "../Common.hlsli"

cbuffer SimpleModel_Buffer : register(b1)
{
	matrix world;
	float4 mat_diffuse;
	float4 mat_ambient;
	float4 mat_specular;
	float4 mat_emissive;
}

cbuffer ColorBuffer : register(b2)
{
	float4 diffuse;
	float4 ambient;
	float4 specular;
	float4 emissive;
	float4 WaterHeight;
}

cbuffer light_buffer : register(b3)
{
	float4 Light;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Diffuse : COLOR0;
	float4 Specular : COLOR1;
	float2 Tex : TEXCOORD0;
};

struct PPLVS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 WPos : POSITION;
	float4 WNormal : NORMAL;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD0;
};