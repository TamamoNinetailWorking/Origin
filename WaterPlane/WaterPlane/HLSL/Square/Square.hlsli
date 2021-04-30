#include "../Common.hlsli"

cbuffer square_world : register(b1)
{
	matrix world;
	float4 diffuse;
};

cbuffer temp_buffer : register(b2)
{
	float Time;
	float Speed;
	float Power;
	float Distortion;
	float RefractiveRatio;
	float3 padding;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Normal : TEXCOORD3;
	float4 RTV_Tex : TEXCOORD1;
	float4 WPosVec : TEXCOORD2;
	float2 Tex : TEXCOORD0;
};