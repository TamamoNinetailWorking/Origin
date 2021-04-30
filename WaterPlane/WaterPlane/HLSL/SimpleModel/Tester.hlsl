#include "SimpleModel.hlsli"


PPLVS_OUTPUT main(
	float4 pos : POSITION,
	float4 normal : NORMAL,
	float2 tex : TEXCOORD)
{
	float4 out_pos;
	PPLVS_OUTPUT outer;
	out_pos = mul(pos, world);
	outer.WPos = out_pos;//World Position

	out_pos = mul(out_pos, View);
	out_pos = mul(out_pos, Projection);

	outer.Pos = out_pos;
	outer.Tex = tex;

	normal = float4(normal.xyz, 0.0f);
	float4 N = mul(normal, world);
	outer.WNormal = normalize(N);
	return outer;
}