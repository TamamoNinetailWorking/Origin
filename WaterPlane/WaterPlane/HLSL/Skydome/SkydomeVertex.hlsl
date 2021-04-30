#include "Skydome.hlsli"

SkydomeVS_OUTPUT main( 
	float3 pos : POSITION ,
	float3 normal : NORMAL, 
	float2 tex : TEXCOORD)
{
	SkydomeVS_OUTPUT output;
	
	output.Pos = mul(float4(pos,1.0f), world);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Tex = tex;

	return output;
}