#include "CubeMap.hlsli"

GS_INPUT main( VS_INPUT input )
{
	GS_INPUT output = (GS_INPUT)0;

	output.Pos = mul(float4(input.Pos, 1.0f), World);
	output.Tex = input.Tex;

	return output;
}