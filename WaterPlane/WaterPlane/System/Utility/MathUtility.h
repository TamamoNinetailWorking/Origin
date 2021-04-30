#pragma once

#include <d3dx9.h>
#include <math.h>
#include <random>
#include "NonCopyable.h"


namespace MathUtility
{
	//std::random_device m_rnd;
	//std::mt19937 m_mt = std::mt19937(m_rnd());
	//extern std::mt19937 m_mt;

	//template <typename Integer>
	//Integer RandomRangeInteger(Integer min, Integer max)
	//{
	//	std::uniform_int_distribution<Integer> dst = std::uniform_int_distribution<Integer>(min, max);
	//	return dst(m_mt);
	//}

	//template <typename Real>
	//Real RandomRangeReal(Real min, Real max)
	//{
	//	std::uniform_real_distribution<Real> dst = std::uniform_real_distribution<Real>(min, max);
	//	return dst(m_mt);
	//}

	class CRandom : public NonCopyable
	{
	private:

		std::random_device m_rnd;
		std::mt19937 m_mt;

		CRandom() {
			m_mt = std::mt19937(m_rnd());
		}

	public:

		static CRandom& GetInstance()
		{
			static CRandom instance;
			return instance;
		}

		template <typename Integer>
		Integer RandomRangeInteger(Integer min, Integer max)
		{
			std::uniform_int_distribution<Integer> dst = std::uniform_int_distribution<Integer>(min, max);
			return dst(m_mt);
		}

		template <typename Real>
		Real RandomRangeReal(Real min, Real max)
		{
			std::uniform_real_distribution<Real> dst = std::uniform_real_distribution<Real>(min, max);
			return dst(m_mt);
		}

	};


	template <typename Number>
	Number Clamp(Number target, Number min, Number max)
	{
		Number returner = target;

		if (target > max)
		{
			returner = max;
		}

		if (target < min)
		{
			returner = min;
		}

		return returner;
	}

	float VectorDistance(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);

	D3DXQUATERNION CreateQuaternionFromVector(const D3DXVECTOR3 vec0, const D3DXVECTOR3 vec1);

	void MakeWorldMatrix(D3DXMATRIX& mat, D3DXMATRIX& mattotal, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans);

	void MakeNextQuaternion(D3DXQUATERNION &qt, D3DXQUATERNION& qt_total, const D3DXVECTOR3& trans);

}