#pragma once

#include <d3dx9.h>

//ÏZs—ñ
inline void MakeWorldMatrix(D3DXMATRIX& mat, D3DXMATRIX& mattotal,const D3DXVECTOR3& angle, const D3DXVECTOR3& trans)
{
	D3DXMATRIX matrot, mattrans;
	D3DXMatrixRotationYawPitchRoll(&matrot, D3DXToRadian(angle.x), D3DXToRadian(angle.y), D3DXToRadian(angle.z));
	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);
	mat = matrot * mattrans;
	mattotal = mat * mattotal;
	return;
}

//‹——£‚ğ‹‚ß‚é
inline float ResolveLength(D3DXVECTOR3& vec1,D3DXVECTOR3& vec2)
{
	float x = vec2.x - vec1.x;
	float y = vec2.y - vec1.y;
	float z = vec2.z - vec1.z;

	float length = x * x + y * y + z * z;
	length = sqrtf(length);

	return length;
}