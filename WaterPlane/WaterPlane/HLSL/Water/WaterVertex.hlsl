#include "Water.hlsli"

VS_OUTPUT main( float3 pos : POSITION,
				float3 normal : NORMAL,
				float2 tex : TEXCOORD)
{
	VS_OUTPUT output;

	output.Pos = mul(float4(pos, 1.0f), world);

	output.WPosVec = output.Pos;
	output.WPosVec.w = 0.0f;

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Tex = tex;

	output.Normal = float4(normal, 1.0f);

	output.RTV_Tex = output.Pos;

	return output;
}