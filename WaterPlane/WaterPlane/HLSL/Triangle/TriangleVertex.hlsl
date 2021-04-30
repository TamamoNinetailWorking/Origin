#include "Triangle.hlsli"

float4 main(float3 pos : POSITION) : SV_POSITION
{
	float4 out_pos;

	out_pos = mul(float4(pos,1.0f),World);
	out_pos = mul(out_pos, View);
	out_pos = mul(out_pos, Projection);

	return out_pos;
}