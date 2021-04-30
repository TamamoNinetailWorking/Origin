#include "MathUtility.h"


namespace MathUtility
{

	float VectorDistance(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2)
	{
		float x_d = pow((vec2.x - vec1.x), 2);
		float y_d = pow((vec2.y - vec2.y), 2);
		float z_d = pow((vec2.z - vec2.z), 2);

		return sqrtf(x_d + y_d + z_d);
	}

	D3DXQUATERNION CreateQuaternionFromVector(const D3DXVECTOR3 vec0, const D3DXVECTOR3 vec1)
	{
		D3DXVECTOR3 Axis;
		D3DXQUATERNION qt;
		float dot;

		D3DXVec3Cross(&Axis, &vec0, &vec1);

		dot = D3DXVec3Dot(&vec0, &vec1);

		dot = Clamp<float>(dot, -1, 1);

		float s = (float)sqrt((1 + dot) * 2);
		qt.x = Axis.x / s;
		qt.y = Axis.y / s;
		qt.z = Axis.z / s;
		qt.w = s / 2;

		return qt;
	}

	//積算行列
	void MakeWorldMatrix(D3DXMATRIX& mat, D3DXMATRIX& mattotal, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans)
	{
		D3DXMATRIX matrot, mattrans;
		D3DXMatrixRotationYawPitchRoll(&matrot, D3DXToRadian(angle.x), D3DXToRadian(angle.y), D3DXToRadian(angle.z));
		D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);
		mat = matrot * mattrans;
		mattotal = mattotal * mat;
		return;
	}

	//積算クォータニオン
	void MakeNextQuaternion(D3DXQUATERNION &qt, D3DXQUATERNION& qt_total, const D3DXVECTOR3& trans)
	{
		D3DXQUATERNION rot;
		D3DXQuaternionRotationYawPitchRoll(
			&rot,
			D3DXToRadian(trans.x),
			D3DXToRadian(trans.y),
			D3DXToRadian(trans.z));

		qt = rot;
		qt_total = qt_total * qt;
		return;
	}

}