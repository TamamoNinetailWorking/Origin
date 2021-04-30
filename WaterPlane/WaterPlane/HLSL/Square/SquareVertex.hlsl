#include "Square.hlsli"

VS_OUTPUT main( float3 pos : POSITION ,
				float3 normal : NORMAL,
				float2 tex : TEXCOORD)
{
	VS_OUTPUT output;

	output.Pos = mul( float4(pos , 1.0f), world);
	float4 WorldPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Tex = tex;

	output.Normal = float4(normal,1.0f);

	output.RTV_Tex = output.Pos;

	output.WPosVec = WorldPos;
	output.WPosVec.w = 0.0f;

	/*output.RTV_Tex = output.Pos.xy + 1.0f;
	output.RTV_Tex = saturate(output.RTV_Tex / 2.0f);
	output.RTV_Tex.y = (output.RTV_Tex.y - 1.0f) * -1.0f;*/

	return output;
}

VS_OUTPUT Test(float2 pos : POSITION, float2 tex : TEXCOORD)
{
	VS_OUTPUT output;
	output.Pos = mul(float4(pos, 1.0f, 1.0f), Projection);
	output.Tex = tex;
	return output;
}