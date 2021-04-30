#include "Wave.hlsli"

VS_CONTROL_POINT_OUTPUT main(	float3 pos : POSITION,
								float3 normal : NORMAL,
								float2 tex : TEXCOORD)
{
	VS_CONTROL_POINT_OUTPUT output;

	output.vPosition = pos;
	output.Normal = normal;
	output.Tex = tex;

	return output;
}