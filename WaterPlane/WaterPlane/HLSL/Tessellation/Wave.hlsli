#include "../Common.hlsli"

cbuffer water_world : register(b1)
{
	matrix world;
	float4 diffuse;
}

cbuffer dist_buffer : register(b2)
{
	float Time;
	float Speed;
	float Power;
	float Distortion;
	float RefractiveRatio;
	float3 padding;
}

cbuffer tess_buffer : register(b3)
{
	float TessFactor : packoffset(c0.x);
	float InsideTessFactor : packoffset(c0.y);
}

cbuffer wave_buffer : register(b4)
{
	float3 WaveCenter;
	float wavebufferpadding;

	float Amplitude;
	float WaveLength;
	float WaveCycle;
	float WaveTime;
	float WaveEdge;

	float3 wavebufferpadding2;
}

cbuffer light_buffer : register(b5)
{
	float3 LightDir;
	float lightpadding;
}

//const double PI = 3.1415926535897932384f;

#define PI (3.1415926535897932384f)

struct VS_OUTPUT
{
	float4 Pos: SV_POSITION;
	float4 Normal : TEXCOORD0;
	float4 RTV_Tex : TEXCOORD1;
	float4 WPosVec : TEXCOORD2;
	float2 Tex : TEXCOORD3;
};

// 入力制御点
struct VS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : WORLDPOS;
	// TODO: 他のスタッフの変更/追加
	float3 Normal : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

// 出力制御点
struct HS_CONTROL_POINT_OUTPUT
{
	float3 pos : WORLDPOS;
	float3 normal : TEXCOORD0;
	float2 tex : TEXCOORD1;
};

// 出力パッチ定数データ。
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4]			: SV_TessFactor; // たとえば、クワド ドメインの [4] になります
	float InsideTessFactor[2]			: SV_InsideTessFactor; // たとえば、クワド ドメインの Inside[2] になります
															   // TODO: 他のスタッフの変更/追加
};

#define NUM_CONTROL_POINTS (4)

//DomainShader Output
struct DS_OUTPUT
{
	float4 Pos  : SV_POSITION;
	float4 WPos : TEXCOORD0;
	float4 Normal : TEXCOORD1;
	float2 Tex : TEXCOORD2;
	//nointerpolation float4 WPos : TEXCOORD2;
	//nointerpolation float2 MapUV : TEXCOORD3;
};