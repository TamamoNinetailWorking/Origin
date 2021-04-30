#pragma once

#include <d3dx9.h>
#include "Transform.h"

class CD3DTransform
	: public CTransform<D3DXVECTOR3, D3DXQUATERNION, D3DXMATRIX>
{
public:

	D3DXMATRIX GetMatrix() override
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 sca = GetScaling();
		D3DXQUATERNION qt = GetRotation();
		D3DXMATRIX matrix,sca_mat;
		D3DXMatrixIdentity(&matrix);
		D3DXMatrixRotationQuaternion(&matrix, &qt);
		D3DXMatrixIdentity(&sca_mat);
		D3DXMatrixScaling(&sca_mat, sca.x, sca.y, sca.z);
		matrix = sca_mat * matrix ;
		matrix._41 = pos.x;
		matrix._42 = pos.y;
		matrix._43 = pos.z;

		return matrix;
	};

	D3DXMATRIX GetMatrixTranspose() override
	{
		D3DXMATRIX matrix;
		D3DXMatrixTranspose(&matrix, &GetMatrix());
		return matrix;
	}

	D3DXVECTOR3 GetForward() override
	{
		D3DXMATRIX matrix = GetMatrix();
		D3DXVECTOR3 forward = D3DXVECTOR3(matrix._31, matrix._32, matrix._33);
		D3DXVec3Normalize(&forward, &forward);
		return forward;
	};

	D3DXVECTOR3 GetUpward() override
	{
		D3DXMATRIX matrix = GetMatrix();
		D3DXVECTOR3 upward = D3DXVECTOR3(matrix._21, matrix._22, matrix._23);
		D3DXVec3Normalize(&upward, &upward);
		return upward;
	}

	D3DXVECTOR3 GetRight() override
	{
		D3DXMATRIX matrix = GetMatrix();
		D3DXVECTOR3 right = D3DXVECTOR3(matrix._11, matrix._12, matrix._13);
		D3DXVec3Normalize(&right, &right);
		return right;
	}

};