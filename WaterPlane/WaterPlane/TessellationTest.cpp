#include "System/Dependence.h"
#include "Framework/Camera/CameraObject.h"

#include "Framework/Object/WaterPlane/WaterPlaneObject.h"
#include "Framework/Object/ModelObject/SkydomeObject.h"

#include "System/Shader II/ShaderCast/ShaderCast.h"

extern CCameraObject* m_Camera;

CWaterPlaneObject* m_cWaterPlane = nullptr;
CSkydomeObject* m_cSkydome = nullptr;

extern ID3D11ShaderResourceView* m_pCubeReflectTextureSRV;

ID3D11ShaderResourceView* m_ParticleTest;

D3DXVECTOR3 m_vLightDir = D3DXVECTOR3(0, 50, 0);

struct LightBuffer
{
	D3DXVECTOR3 LightDir;
	float padding;
};

ID3D11Buffer* m_LightBuffer;

bool TessellationInit()
{
	m_cSkydome = new CSkydomeObject();
	m_cWaterPlane = new CWaterPlaneObject();

	ID3D11Device* device = GetDevice11();
	if (!NShaderCast::CreateConstantBuffer(device, sizeof(LightBuffer), m_LightBuffer) )
	{
		MessageBox(nullptr, "CreateFailed ConstantBuffer.", "LightBuffer", MB_OK);
		return false;
	}
	
	ID3D11DeviceContext* context = GetImmediateContext();
	if (!NShaderCast::CreateShaderResourceView(device, context, "Assets/image/particle.png", m_ParticleTest))
	{
		MessageBox(nullptr, "CreateFailed ConstantBuffer.", "LightBuffer", MB_OK);
		return false;
	}

	return true;
}

void TessellationUpdate()
{
	m_cSkydome->Update();
	m_cWaterPlane->Update();

	LightBuffer lb;
	lb.LightDir = m_vLightDir;
	CDirectGraphics::GetInstance().GetDeviceContext()->UpdateSubresource(m_LightBuffer, 0, nullptr, &lb, 0, 0);

	CDirectGraphics::GetInstance().GetDeviceContext()->PSSetConstantBuffers(5, 1, &m_LightBuffer);

}

void TessellationDraw()
{
	DefaultBlendState();
	DepthStencilSwitch(true);

	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	GetImmediateContext()->GSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	GetImmediateContext()->DSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());

	m_cSkydome->Draw();

	GetImmediateContext()->PSSetShaderResources(2, 1, &m_pCubeReflectTextureSRV);

	GetImmediateContext()->PSSetShaderResources(3, 1, &m_ParticleTest);

	AlignmentBlendState();

	m_cWaterPlane->Draw();

	DefaultBlendState();
	DepthStencilSwitch(true);
}