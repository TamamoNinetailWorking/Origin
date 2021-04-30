#include "Square.hlsli"

Texture2D g_Texture : register(t0);
Texture2D g_NormalTexture : register(t1);
Texture2D g_RenderTarget : register(t2);
SamplerState g_Sampler: register(s0);


float4 grayScale(VS_OUTPUT input)
{
	float4 color = g_Texture.Sample(g_Sampler, input.Tex)/* * diffuse*/;

	float gray = (color.r * 2 + color.g * 4 + color.b) / 7;

	return gray;
}

float wave(float2 uv, float2 emitter, float speed, float phase)
{
	float dst = distance(uv, emitter);
	float temp = 0.5 + 0.5 * sin(dst * phase - Time * speed);
	return pow(temp, 2.0f);
}

float Fresnel(float4 ViewVec,float4 NormalVec)
{
	float A = RefractiveRatio;
	float B = dot(-normalize(ViewVec), normalize(NormalVec));
	float C = sqrt(1.0f - A * A * (1 - B * B));
	float Rs = (A* B - C) * (A*B - C) / ((A*B + C) * (A*B + C));
	float Rp = (A*C - B) * (A*C - B) / ((A*C + B) * (A*C + B));
	float alpha = (Rs + Rp) / 2.0f;
	alpha = min(alpha + 0.02f, 1.0f);
	return max(alpha,0.0f);
}

float4 Surf(VS_OUTPUT input): SV_TARGET
{
	float2 position = input.Tex;

	float w = wave(position, float2(0.6, 0.11), Speed, 20);
	w += wave(position, float2(0.9, 0.6), Speed, 90);
	w += wave(position, float2(0.1, 0.84), Speed, 150.0);
	w += wave(position, float2(0.32, 0.81), Speed, 150.0);
	w += wave(position, float2(0.16, 0.211), Speed, 150.0);
	w += wave(position, float2(0.39, 0.46), Speed, 150.0);
	w += wave(position, float2(0.51, 0.484), Speed, 150.0);
	w += wave(position, float2(0.732, 0.91), Speed, 150.0);
	w *= 0.116 * Power;
	//input.Tex += w;
	float2 tex = input.Tex;
	tex += w;

	//float2 rtv_pos = input.Pos.xy;
	//rtv_pos = (rtv_pos + 1.0f) / 2.0f;
	//rtv_pos.y = (rtv_pos.y - 1.0f) * -1.0f;
	//float4 rtv = g_RenderTarget.Sample(g_Sampler, rtv_pos);
	//rtv_pos += w;
	//float4 rtv = g_RenderTarget.Sample(g_Sampler, input.RTV_Tex);
	
	float2 rtv_pos = input.RTV_Tex.xy / input.RTV_Tex.w;
	rtv_pos = (rtv_pos + 1.0f) / 2.0f;
	rtv_pos.y = (rtv_pos.y - 1.0f) * -1.0f;
	float4 rtv = g_RenderTarget.Sample(g_Sampler, rtv_pos);
	float4 WNorm = mul(input.Normal, world);
	float4 ViewVec = CameraPos - input.WPosVec;
	//rtv.a = Fresnel(ViewVec,WNorm);

	float3 norm = normalize(g_NormalTexture.Sample(g_Sampler, tex).rgb * 2.0f - 1.0f);
	float2 uv = input.Tex + float2(norm.r,norm.b) * Distortion;
	uv.y = uv.y * (-1) + 1;
	//float2 uv = input.Tex + 0.1f;
	//float4 back = g_Texture.Sample(g_Sampler, uv);
	
	//float water_per = 0.4f;
	
	float4 w_color = g_Texture.Sample(g_Sampler, uv);
	//float4 w_color = g_RenderTarget.Sample(g_Sampler, uv);
	//w_color.a = 0.4f;

	//float4 color = w_color * water_per + rtv * (1.0f - water_per);
	//color.a = w_color.a * rtv.a;

	float4 color = w_color;
	return color;
}

float4 Bottom(VS_OUTPUT input) : SV_TARGET
{
	float2 position = input.Tex;
	float w = wave(position, float2(0.6, 0.11), Speed, 20);
	w += wave(position, float2(0.9, 0.6), Speed, 90);
	w += wave(position, float2(0.1, 0.84), Speed, 150.0);
	w += wave(position, float2(0.32, 0.81), Speed, 150.0);
	w += wave(position, float2(0.16, 0.211), Speed, 150.0);
	w += wave(position, float2(0.39, 0.46), Speed, 150.0);
	w += wave(position, float2(0.51, 0.484), Speed, 150.0);
	w += wave(position, float2(0.732, 0.91), Speed, 150.0);
	w *= 0.116 * Power;

	float2 rtv_pos = input.RTV_Tex.xy / input.RTV_Tex.w;
	rtv_pos = (rtv_pos + 1.0f) / 2.0f;
	rtv_pos.y = (rtv_pos.y - 1.0f) * -1.0f;
	//rtv_pos += w;
	float4 rtv = g_Texture.Sample(g_Sampler, rtv_pos);
	float4 WNorm = mul(input.Normal, world);
	float4 ViewVec = CameraPos - input.WPosVec;
	rtv.a = clamp(Fresnel(ViewVec,WNorm) - 0.9f,0,1);
	return rtv;
}

float4 Reflect(VS_OUTPUT input) : SV_TARGET
{
	float2 position = input.Tex;
	float w = wave(position, float2(0.6, 0.11), Speed, 20);
	w += wave(position, float2(0.9, 0.6), Speed, 90);
	w += wave(position, float2(0.1, 0.84), Speed, 150.0);
	w += wave(position, float2(0.32, 0.81), Speed, 150.0);
	w += wave(position, float2(0.16, 0.211), Speed, 150.0);
	w += wave(position, float2(0.39, 0.46), Speed, 150.0);
	w += wave(position, float2(0.51, 0.484), Speed, 150.0);
	w += wave(position, float2(0.732, 0.91), Speed, 150.0);
	w *= 0.116 * Power;

	float2 rtv_pos = input.RTV_Tex.xy / input.RTV_Tex.w;
	rtv_pos = (rtv_pos + 1.0f) / 2.0f;
	rtv_pos.y = (rtv_pos.y - 1.0f) * -1.0f;
	//rtv_pos += w;
	float4 rtv = g_Texture.Sample(g_Sampler, rtv_pos);
	float4 WNorm = mul(input.Normal, world);
	float4 ViewVec = CameraPos - input.WPosVec;
	rtv.a = 0.6f;

	return rtv;
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
	//return grayScale(input) * diffuse;
	return g_Texture.Sample(g_Sampler,input.Tex) * diffuse;
}

float4 SepiaPS(VS_OUTPUT input) : SV_TARGET
{
	float4 col = grayScale(input);
	
	float4 sepia = float4(col.r, col.g * 74 / 107, col.b * 43 / 107, 1.0f);
	return sepia * diffuse;
}