#include "Framework/Object/ModelObject/SkydomeDrawer.h"
#include "System/Dependence.h"

CSkydomeDrawer* m_Drawer;

void Test4Init()
{
	m_Drawer = new CSkydomeDrawer();

	m_Drawer->Init(GetDevice11(), GetImmediateContext(), GetDevice9());

}

void Test4Draw()
{
	DefaultBlendState();
	DepthStencilSwitch(true);

	D3DXMATRIX dummy;
	D3DXMatrixIdentity(&dummy);

	m_Drawer->Draw(dummy);
}