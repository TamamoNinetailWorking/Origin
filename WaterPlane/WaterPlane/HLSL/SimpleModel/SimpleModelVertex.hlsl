#include "SimpleModel.hlsli"

VS_OUTPUT main( 
	float3 pos : POSITION ,
	float3 normal : NORMAL,
	float2 tex : TEXCOORD)
{
	float4 out_pos;
	out_pos = mul(float4(pos,1.0f), world);
	float4 P = out_pos;//World Position
	
	out_pos = mul(out_pos, View);
	out_pos = mul(out_pos, Projection);
	VS_OUTPUT outer;
	outer.Pos = out_pos;
	outer.Tex = tex;

	float4 norm = float4(normal.xyz, 0.0f);
	float4 N =mul(norm, world);
	N = normalize(N);
	//float4 L = -Light;
	float4 L = Light - P;
	L = normalize(L);
	float4 V = CameraPos- P;
	V = normalize(V);
	float4 H = L + V;
	H = normalize(H);


	float4 amb = mat_ambient * ambient;
	float4 dif = mat_diffuse * diffuse *  max(0.0, dot(L, N));
	outer.Diffuse = amb + dif;
	//outer.Diffuse = dif;
	outer.Specular = mat_specular * specular * pow(max(0, dot(N, H)), 50);

	return outer;
}

PPLVS_OUTPUT pplmain(
	float3 pos : POSITION,
	float3 normal : NORMAL,
	float2 tex : TEXCOORD)
{
	float4 out_pos;
	PPLVS_OUTPUT outer;
	out_pos = mul(float4(pos,1.0f), world);
	outer.WPos = out_pos;//World Position

	out_pos = mul(out_pos, View);
	out_pos = mul(out_pos, Projection);

	outer.Pos = out_pos;
	outer.Tex = tex;

	//normal = float4(normal.xyz, 0.0f);
	float4 N = mul(float4(normal,0), world);
	outer.WNormal = normalize(N);
	if (outer.WPos.y >=  WaterHeight.y)
	{
		outer.Color = float4(1, 1, 1, 0);
	}
	else
	{
		outer.Color = float4(1, 1, 1, 1);
	}
	return outer;
	//float4 L = -Light;
	//float4 L = Light - P;
	//L = normalize(L);
	//float4 V = CameraPos - P;
	//V = normalize(V);
	//float4 H = L + V;
	//H = normalize(H);

}

VS_OUTPUT LineVS(
	float4 pos : POSITION,
	float4 normal : NORMAL,
	float2 tex : TEXCOORD)
{
	float4 out_pos = pos;
	VS_OUTPUT outer;
	normal = normalize(normal);
	out_pos = pos + normal * 0.05;

	out_pos = mul(out_pos, world);

	//float4 n = mul(normal, world);
	//out_pos = out_pos + n;
	//outer.WPos = out_pos;//World Position

	out_pos = mul(out_pos, View);
	out_pos = mul(out_pos, Projection);

	outer.Pos = out_pos;
	outer.Diffuse = float4(0, 0, 0, 1);
	outer.Specular = float4(0, 0, 0, 1);
	outer.Tex = tex;
	
	return outer;
}