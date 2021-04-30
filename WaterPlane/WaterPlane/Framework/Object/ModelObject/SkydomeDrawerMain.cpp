#include "SkydomeDrawer.h"
#include "../../Mesh/XFile/XFile.h"
#include "../../../System/Shader II/ShaderSet.h"
#include "../../../System/Shader II/ShaderCast/ShaderCast.h"
#include "../../../System/DirectoryManager/DirectoryManager.h"
#include "../../../System/Utility/Memory.h"

void CSkydomeDrawer::Exit()
{
	m_pShader->Exit();
	Delete(m_pShader);
	Delete(m_pMesh);
}

CSkydomeDrawer::~CSkydomeDrawer()
{
	Exit();
}

bool CSkydomeDrawer::meshInit()
{
	CDirectoryManager::GetInstance().SetDefaultDirectory();

	//Mesh Init
	CDirectoryManager::GetInstance().SetDirectory("Assets/mesh/skydome/");
	m_pMesh = new CXFile("skydome.x", m_nines);

	//Decal Init
	for (UINT i = 0; i < m_pMesh->GetMaterialNum(); i++)
	{
		ID3D11ShaderResourceView* p_srv;
		if (m_pMesh->GetDecal(i) != "")
		{
			if (NShaderCast::CreateShaderResourceView(m_device, m_deviceContext, m_pMesh->GetDecal(i).c_str(), p_srv))
			{
				m_pSRVContainer.push_back(p_srv);
			}
			else
			{
				MessageBox(NULL, "Decal CreateFailed.", "CSkydomeDrawer meshInit", MB_OK);
				return false;
			}
		}
		else
		{
			//This material Doesn't has decal.
			p_srv = nullptr;
			m_pSRVContainer.push_back(p_srv);
		}
	}

	//VertexBufferInit
	if (!(NShaderCast::CreateVertexBuffer(
		m_device,
		sizeof(XFile::Vertex),
		m_pMesh->GetVertexNum(),
		m_pMesh->GetVertices(),
		m_pVertexBuffer)))
	{
		MessageBox(NULL, "VertexBuffer CreateFailed.", "CSkydomeDrawer meshInit", MB_OK);
		return false;
	}

	//IndexBuffer Init
	if (!(NShaderCast::CreateIndexBuffer(
		m_device,
		m_pMesh->GetIndexNum(),
		m_pMesh->GetIndex(),
		m_pIndexBuffer)))
	{
		MessageBox(NULL, "VertexBuffer CreateFailed.", "CSkydomeDrawer meshInit", MB_OK);
		return false;
	}

	CDirectoryManager::GetInstance().SetDefaultDirectory();
	return true;
}

bool CSkydomeDrawer::shaderInit()
{
	CDirectoryManager::GetInstance().SetDefaultDirectory();

	m_pShader = new CShaderSet();

	//Default Shader
	CDirectoryManager::GetInstance().SetDirectory("HLSL/Skydome/");
	bool sts;
	sts = m_pShader->CreateVertexShader(
		m_device,
		"SkydomeVertex.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "VertexShader CreateFailed.", "CSkydomeDrawer shaderInit", MB_OK);
		return false;
	}

	sts = m_pShader->CreatePixelShader(
		m_device,
		"SkydomePixel.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "PixelShader CreateFailed.", "CSkydomeDrawer shaderInit", MB_OK);
		return false;
	}

	CDirectoryManager::GetInstance().SetDefaultDirectory();

	//CubeMap Shader
	CDirectoryManager::GetInstance().SetDirectory("HLSL/CubeMap/");
	sts = m_pShader->CreateVertexShader(
		m_device,
		"CubeMapVertex.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "VertexShader CreateFailed.", "CSkydomeDrawer shaderInit", MB_OK);
		return false;
	}

	sts = m_pShader->CreateGeometryShader(
		m_device,
		"CubeMapGeometry.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "GeometryShader CreateFailed.", "CSkydomeDrawer shaderInit", MB_OK);
		return false;
	}

	sts = m_pShader->CreatePixelShader(
		m_device,
		"CubeMapPixel.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "PixelShader CreateFailed.", "CSkydomeDrawer shaderInit", MB_OK);
		return false;
	}

	CDirectoryManager::GetInstance().SetDefaultDirectory();

	return true;
}

bool CSkydomeDrawer::constantBufferInit()
{
	
	ID3D11Buffer* buffer;

	m_pBufferContainer.resize((int)ConstantBufferType::Max);

	if (NShaderCast::CreateConstantBuffer(m_device, sizeof(ConstantBufferWorld), buffer))
	{
		m_pBufferContainer[(int)ConstantBufferType::World] = buffer;
		return true;
	}
	else
	{
		MessageBox(NULL, "constantBuffer CreateFailed", "CSkydomeDrawer constantBufferInit", MB_OK);
		return false;
	}
}

bool CSkydomeDrawer::samplerInit()
{
	if (NShaderCast::CreateSamplerState(m_device, m_pSamplerState))
	{
		return true;
	}
	else
	{
		MessageBox(NULL, "samplerstate CreateFailed", "CSkydomeDrawer samplerInit", MB_OK);
		return false;
	}
}

bool CSkydomeDrawer::Init(
	ID3D11Device* dev, 
	ID3D11DeviceContext* devCon, 
	LPDIRECT3DDEVICE9 ni)
{
	m_device = dev;
	m_deviceContext = devCon;
	m_nines = ni;

	if (!meshInit())
	{
		MessageBox(NULL, "meshInitFailed.", "CSkydomeDrawer Init", MB_OK);
		return false;
	}

	if (!shaderInit())
	{
		MessageBox(NULL, "shaderInitFailed", "CSkydomeDrawer Init", MB_OK);
		return false;
	}

	if (!constantBufferInit())
	{
		MessageBox(NULL, "constantBufferInitFailed", "CSkydomeDrawer Init", MB_OK);
		return false;
	}

	if (!samplerInit())
	{
		MessageBox(NULL, "samplerInitFailed", "CSkydomeDrawer Init", MB_OK);
		return false;
	}

	return true;
}

void CSkydomeDrawer::updateBuffer(const D3DXMATRIX& matrix)
{
	ConstantBufferWorld cb;
	::ZeroMemory(&cb, sizeof(ConstantBufferWorld));
	D3DXMatrixTranspose(&cb.world, &matrix);
	m_deviceContext->UpdateSubresource(
		m_pBufferContainer[(int)ConstantBufferType::World], 0, NULL, &cb, 0, 0);
}

void CSkydomeDrawer::setBufferToShaders(ConstantBufferType type)
{
	int num = (int)type;
	//ID3D11Buffer* temp = m_pBufferContainer[num];
	m_deviceContext->VSSetConstantBuffers(num, 1, &m_pBufferContainer[num]);
	m_deviceContext->GSSetConstantBuffers(num, 1, &m_pBufferContainer[num]);
	m_deviceContext->PSSetConstantBuffers(num, 1, &m_pBufferContainer[num]);
}

void CSkydomeDrawer::setVerticesAndIndex()
{
	UINT stride = sizeof(XFile::Vertex);
	UINT offset = 0;

	m_deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,&stride,&offset);
	
	m_deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

	for (UINT i = 0; i < m_pMesh->GetMaterialNum(); i++)
	{
		int subsetindex = -1;
		UINT num = m_pMesh->GetMaterialNum();

		m_deviceContext->PSSetShaderResources(0, 1, &m_pSRVContainer[i]);
		for (UINT j = 0; j < m_pMesh->GetSubsetNum(); j++)
		{
			if (i == m_pMesh->GetSubset(j).m_MaterialIndex)
			{
				subsetindex = j;
				auto obj = m_pMesh->GetSubset(subsetindex).m_VertexNum;
				auto work = m_pMesh->GetSubset(subsetindex).m_FaceStart * 3;
				m_deviceContext->DrawIndexed(obj, work, 0);
			}
		}
	}

}

void CSkydomeDrawer::Draw(const D3DXMATRIX& matrix)
{
	updateBuffer(matrix);
	
	setBufferToShaders(ConstantBufferType::World);

	m_pShader->SetVertexShader(m_deviceContext, (int)ShaderType::Default);
	m_pShader->SetHullShader(m_deviceContext, (int)ShaderType::NotUse);//Not Use.
	m_pShader->SetDomainShader(m_deviceContext, (int)ShaderType::NotUse);//Not Use.
	m_pShader->SetGeometryShader(m_deviceContext, (int)ShaderType::NotUse);//Not Use.
	m_pShader->SetPixelShader(m_deviceContext, (int)ShaderType::Default);

	setVerticesAndIndex();
}

void CSkydomeDrawer::DrawAsCubeMap(const D3DXMATRIX& matrix)
{
	updateBuffer(matrix);

	setBufferToShaders(ConstantBufferType::World);

	m_pShader->SetVertexShader(m_deviceContext, (int)ShaderType::CubeMap);
	m_pShader->SetHullShader(m_deviceContext, (int)ShaderType::NotUse);//Not Use.
	m_pShader->SetDomainShader(m_deviceContext, (int)ShaderType::NotUse);//Not Use.
	m_pShader->SetGeometryShader(m_deviceContext, (int)ShaderType::Default);
	m_pShader->SetPixelShader(m_deviceContext, (int)ShaderType::CubeMap);

	setVerticesAndIndex();
}