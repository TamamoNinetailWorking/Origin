#include "SkydomeObject.h"
#include "SkydomeDrawer.h"

#include "../../../System/Dependence.h"

#include <iostream>

void CSkydomeObject::Start()
{
	m_cDrawer = new CSkydomeDrawer();

	m_cDrawer->Init(GetDevice11(), GetImmediateContext(), GetDevice9());

	m_cTransform.SetPosition(D3DXVECTOR3(0, 0, 0));
	m_cTransform.SetScaling(D3DXVECTOR3(1, 1, 1));
	D3DXQUATERNION qt;
	D3DXQuaternionIdentity(&qt);
	m_cTransform.SetRotation(qt);

	m_bStartFlag = true;
	m_bActivate = true;
}

D3DXMATRIX CSkydomeObject::GetMatrix()
{
	D3DXMATRIX matrix = m_cTransform.GetMatrix();
	return matrix;
}

void CSkydomeObject::updateProcess()
{

}

void CSkydomeObject::Exit()
{
	Delete(m_cDrawer);
}

void CSkydomeObject::Update()
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

void CSkydomeObject::Draw()
{
	if (m_bActivate) 
	{
		m_cDrawer->Draw(m_cTransform.GetMatrix());
	}
}

void CSkydomeObject::DrawAsCubeMap()
{
	if (m_bActivate)
	{
		m_cDrawer->DrawAsCubeMap(m_cTransform.GetMatrix());
	}
}