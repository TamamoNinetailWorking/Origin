#include "Wave.hlsli"

float WaveFunction(
	float3 Emitter,
	float3 localPos,
	float Amplitude,
	float WaveLength,
	float Angle)
{
	Emitter.y = localPos.y = 0;
	float len = length(Emitter - localPos);
	float lengthRate = len / WaveLength;
	float Wavemain = sin(lengthRate + Angle);
	return Amplitude * pow((Wavemain + 1.0f), WaveEdge);
	//return Amplitude * Wavemain;
}

float3 CrossVector(float3 origin, float3 vecRight, float3 vecLeft)
{
	float3 vec1 = vecRight - origin;
	float3 vec2 = vecLeft - origin;

	return cross(vec1, vec2);
}

float3 ProcessGetNormal(float3 pos)
{
	float3 normaldesc[4];
	
	float offset = 0.001f;

	float2 texel[4] = {
		{ 0,offset },
		{ offset,0 },
		{ -offset,0 },
		{ 0,-offset } };


	for (uint i = 0; i < 4; i++)
	{
		normaldesc[i] = pos;
		
		normaldesc[i].x += texel[i].x;
		normaldesc[i].z += texel[i].y;

		normaldesc[i].y = WaveFunction(
			WaveCenter,
			normaldesc[i],
			Amplitude,
			WaveLength,
			WaveTime / WaveCycle);
	}

	float3 pointnormal[4];
	pointnormal[0] = CrossVector(pos, normaldesc[1], normaldesc[0]);
	pointnormal[1] = CrossVector(pos, normaldesc[0], normaldesc[2]);
	pointnormal[2] = CrossVector(pos, normaldesc[2], normaldesc[3]);
	pointnormal[3] = CrossVector(pos, normaldesc[3], normaldesc[1]);

	return (pointnormal[0] + pointnormal[1] + pointnormal[2] + pointnormal[3]) * (-0.25f);
}

[domain("quad")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	float3 p1 = lerp(patch[1].pos, patch[0].pos, domain.x);
	float3 p2 = lerp(patch[3].pos, patch[2].pos, domain.x);
	float3 p3 = lerp(p1, p2, domain.y);

	float3 norm_pos = p3;
	p3.y = WaveFunction(
		WaveCenter, 
		p3,
		Amplitude, 
		WaveLength, 
		WaveTime / WaveCycle);

	float3 normal = ProcessGetNormal(p3);

	Output.Pos = mul(float4(p3, 1.0f), world);
	Output.WPos = Output.Pos;
	Output.Pos = mul(Output.Pos, View);
	Output.Pos = mul(Output.Pos, Projection);

	Output.Normal = float4(normal, 0.0f);
	Output.Normal = mul(Output.Normal, world);

	float2 t1 = lerp(patch[1].tex, patch[0].tex, domain.x);
	float2 t2 = lerp(patch[3].tex, patch[2].tex, domain.x);
	float2 t3 = lerp(t1, t2, domain.y);

	Output.Tex = t3;

	return Output;
}
