#include "CubeMap.hlsli"

[maxvertexcount(18)]
void main(
	triangle GS_INPUT input[3], 
	inout TriangleStream< PS_INPUT > Stream
)
{
	for (uint face = 0; face < 6; face++)
	{
		PS_INPUT output;
		output.RenderTargetIndex = face;
		for (uint vertex = 0; vertex < 3; vertex++)
		{
			output.Pos = mul(input[vertex].Pos, View[face]);
			output.Pos = mul(output.Pos, Projection);
			output.Tex = input[vertex].Tex;
			Stream.Append(output);
		}
		Stream.RestartStrip();
	}
}