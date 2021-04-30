#include "Wave.hlsli"

// �p�b�`�萔�֐�
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	// �����ɃR�[�h��}�����ďo�͂��v�Z���܂�
	Output.EdgeTessFactor[0] = 
		Output.EdgeTessFactor[1] = 
		Output.EdgeTessFactor[2] = 
		Output.EdgeTessFactor[3] = TessFactor; // ���Ƃ��΁A����ɓ��I�e�Z���[�V�����W�����v�Z�ł��܂�
	Output.InsideTessFactor[0] = Output.InsideTessFactor[1] = InsideTessFactor;

	return Output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// �����ɃR�[�h��}�����ďo�͂��v�Z���܂�
	Output.pos = ip[i].vPosition;
	Output.normal = ip[i].Normal;
	Output.tex = ip[i].Tex;

	return Output;
}
