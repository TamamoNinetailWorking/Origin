#include "WaterPlaneObject.h"
#include "WaterPlaneDrawer.h"

#include "../../../System/Dependence.h"

#include <iostream>



void CWaterPlaneObject::Start()
{
	m_cDrawer = new CWaterPlaneDrawer();

	if (!m_cDrawer->Init(GetDevice11(), GetImmediateContext()))
	{
		std::cout << "CWaterPlaneObject Init Failed." << std::endl;
		return;
	}
	float scale = 200;
	m_cTransform.SetPosition(D3DXVECTOR3(0, 0.8f, 0));
	m_cTransform.SetScaling(D3DXVECTOR3(scale, scale, scale));
	D3DXQUATERNION qt;
	D3DXQuaternionIdentity(&qt);
	//D3DXQuaternionRotationYawPitchRoll(&qt, D3DXToRadian(90), 0,0);
	m_cTransform.SetRotation(qt);
	m_bStartFlag = true;
	m_bActivate = true;
}

void CWaterPlaneObject::updateProcess()
{
	//m_cDrawer->SetParameter(0.3f, 3.5f, 0.8f, 0,0.4f);
	static float angle = 0.0f;
	D3DXQUATERNION qt;
	D3DXQuaternionRotationYawPitchRoll(&qt, D3DXToRadian(angle), 0, 0);
	m_cTransform.SetRotation(qt);
	angle += D3DXToRadian(20.0f);
}

void CWaterPlaneObject::SetSurfaceParameter(const float dist, const float speed, const float pow, const float refrac)
{
	m_cDrawer->SetParameter(dist, speed, pow, refrac);
}

void CWaterPlaneObject::SetTessCoeficient(const float tess, const float in_tess)
{
	m_cDrawer->SetTessCoeficient(tess, in_tess);
}

void CWaterPlaneObject::SetWaveParameter(const float amplitude, const float length, const float cycle, const float edge)
{
	m_cDrawer->SetWaveParameter(amplitude, length, cycle, edge);
}

void CWaterPlaneObject::SetDiffuse(const float r, const float g, const float b, const float a)
{
	m_cDrawer->SetDiffuse(r, g, b, a);
}

void CWaterPlaneObject::Exit()
{
	Delete(m_cDrawer);
}

void CWaterPlaneObject::Update()
{
	if (!m_bStartFlag)
	{
		Start();
	}

	if (m_bActivate)
	{
		updateProcess();
	}
}

void CWaterPlaneObject::Draw()
{
	m_cDrawer->Draw(m_cTransform.GetMatrix());
}