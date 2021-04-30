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

// ���͐���_
struct VS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : WORLDPOS;
	// TODO: ���̃X�^�b�t�̕ύX/�ǉ�
	float3 Normal : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

// �o�͐���_
struct HS_CONTROL_POINT_OUTPUT
{
	float3 pos : WORLDPOS;
	float3 normal : TEXCOORD0;
	float2 tex : TEXCOORD1;
};

// �o�̓p�b�`�萔�f�[�^�B
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4]			: SV_TessFactor; // ���Ƃ��΁A�N���h �h���C���� [4] �ɂȂ�܂�
	float InsideTessFactor[2]			: SV_InsideTessFactor; // ���Ƃ��΁A�N���h �h���C���� Inside[2] �ɂȂ�܂�
															   // TODO: ���̃X�^�b�t�̕ύX/�ǉ�
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