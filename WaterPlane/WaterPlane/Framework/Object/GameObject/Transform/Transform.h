#pragma once

template <typename VECTOR,typename ROTATION,typename MATRIX>
class CTransform
{
protected:

	//位置と姿勢を保持。必要時に行列へ変換

	VECTOR m_vPosition;
	VECTOR m_vScaling;
	ROTATION m_qRotation;

public:

	CTransform() {};

	CTransform(CTransform& _Transform) {
		m_vPosition = _Transform.GetPosition();
		m_vScaling = _Transform.GetScaling();
		m_qRotation = _Transform.GetRotation();
	}

	CTransform& operator=(const CTransform& _Transform)
	{
		m_vPosition = _Transform.GetPosition();
		m_vScaling = _Transform.GetScaling();
		m_qRotation = _Transform.GetRotation();
		return this;
	}

	VECTOR GetPosition() { return m_vPosition; };
	VECTOR GetScaling() { return m_vScaling; };
	ROTATION GetRotation() { return m_qRotation; };

	void SetPosition(VECTOR _Position) {
		m_vPosition = _Position;
	};
	void SetScaling(VECTOR _Scaling) {
		m_vScaling = _Scaling;
	}
	void SetRotation(ROTATION _Rotation) {
		m_qRotation = _Rotation;
	};

	virtual VECTOR GetForward()=0;
	virtual VECTOR GetUpward()=0;
	virtual VECTOR GetRight()=0;

	virtual MATRIX GetMatrix()=0;
	virtual MATRIX GetMatrixTranspose()=0;

};