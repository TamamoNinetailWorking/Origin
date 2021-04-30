
//Global
cbuffer view_projection  : register(b0)
{
	matrix View;
	matrix Projection;
	float4 CameraPos;
};