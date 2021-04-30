cbuffer CubeMapCameraBuffer : register(b0)
{
	matrix View[6];
	matrix Projection;
}

cbuffer CubeMapWorldBuffer : register(b1)
{
	matrix World;
}

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct GS_INPUT
{
	float4 Pos: SV_POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	uint RenderTargetIndex : SV_RenderTargetArrayIndex;
};