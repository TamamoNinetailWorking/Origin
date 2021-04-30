#include "WaterPlaneDrawer.h"
#include "../../../System/Shader II/ShaderSet.h"
#include "../../../System/Shader II/ShaderCast/ShaderCast.h"
#include "../../../System/DirectoryManager/DirectoryManager.h"
#include "../../../System/Utility/Memory.h"

#include <iostream>

CWaterPlaneDrawer::~CWaterPlaneDrawer()
{
	Exit();
}

void CWaterPlaneDrawer::Exit()
{
	m_pShader->Exit();
	Delete(m_pShader);
}

bool CWaterPlaneDrawer::meshInit()
{
	//VertexBufferInit

	Vertex vertices[] =
	{
		D3DXVECTOR3(-0.5f,0.0f,0.5f),D3DXVECTOR3(0,1,0),D3DXVECTOR2(0,0),
		D3DXVECTOR3(0.5f,0.0f,0.5f),D3DXVECTOR3(0,1,0),D3DXVECTOR2(1,0),
		D3DXVECTOR3(-0.5f,-0.0f,-0.5f),D3DXVECTOR3(0,1,0),D3DXVECTOR2(0,1),
		D3DXVECTOR3(0.5f,-0.0f,-0.5f),D3DXVECTOR3(0,1,0),D3DXVECTOR2(1,1),
	};

	int vertices_num = ARRAYSIZE(vertices);

	for (int i = 0; i < vertices_num; i++)
	{
		vertices[i].pos *= 3;
	}

	if (!(NShaderCast::CreateVertexBuffer(
		m_device,
		sizeof(Vertex),
		vertices_num,
		vertices,
		m_pVertexBuffer)))
	{
		MessageBox(NULL, "VertexBuffer CreateFailed.", "CSkydomeDrawer meshInit", MB_OK);
		return false;
	}

	return true;
}

bool CWaterPlaneDrawer::decalInit()
{
	CDirectoryManager::GetInstance().SetDefaultDirectory();

	//Decal Init
	CDirectoryManager::GetInstance().SetDirectory("Assets/image/");

	ID3D11ShaderResourceView* p_srv;
	if (NShaderCast::CreateShaderResourceView(m_device, m_deviceContext, "WaterBasicDaytimeGradient.tga", p_srv))
	{
		m_pSRVContainer.push_back(p_srv);
	}
	else
	{
		MessageBox(NULL, "Decal CreateFailed.", "CWaterDrawer meshInit", MB_OK);
		return false;
	}

	if (NShaderCast::CreateShaderResourceView(m_device, m_deviceContext, "stones_NM_height.tga", p_srv))
	{
		m_pSRVContainer.push_back(p_srv);
	}
	else
	{
		MessageBox(NULL, "NormalMap CreateFailed.", "CWaterDrawer meshInit", MB_OK);
		return false;
	}

	return true;
}

bool CWaterPlaneDrawer::shaderInit()
{
	CDirectoryManager::GetInstance().SetDefaultDirectory();

	m_pShader = new CShaderSet();

	/*CDirectoryManager::GetInstance().SetDirectory("HLSL/Water/");

	bool sts;
	sts = m_pShader->CreateVertexShader(
		m_device,
		"WaterVertex.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "VertexShader CreateFailed.", "CWaterPlaneDrawer shaderInit", MB_OK);
		return false;
	}

	sts = m_pShader->CreatePixelShader(
		m_device,
		"WaterPixel.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "PixelShader CreateFailed.", "CWaterPlaneDrawer shaderInit", MB_OK);
		return false;
	}*/

	CDirectoryManager::GetInstance().SetDirectory("HLSL/Tessellation/");

	bool sts;
	sts = m_pShader->CreateVertexShader(
	m_device,
	"WaveVertex.hlsl",
	"main");
	if (!sts)
	{
		MessageBox(NULL, "VertexShader CreateFailed.", "CWaterPlaneDrawer shaderInit", MB_OK);
		return false;
	}

	sts = m_pShader->CreatePixelShader(
	m_device,
	"WavePixel.hlsl",
	"main");
	if (!sts)
	{
		MessageBox(NULL, "PixelShader CreateFailed.", "CWaterPlaneDrawer shaderInit", MB_OK);
		return false;
	}

	sts = m_pShader->CreateHullShader(
		m_device,
		"WaveHullShader.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "HullShader CreateFailed.", "CWaterPlaneDrawer shaderInit", MB_OK);
		return false;
	}

	sts = m_pShader->CreateDomainShader(
		m_device,
		"WaveDomainShader.hlsl",
		"main");
	if (!sts)
	{
		MessageBox(NULL, "HullShader CreateFailed.", "CWaterPlaneDrawer shaderInit", MB_OK);
		return false;
	}

	CDirectoryManager::GetInstance().SetDefaultDirectory();

	return true;
}

bool CWaterPlaneDrawer::constantBufferInit()
{

	m_pBufferContainer.resize((int)ConstantBufferType::Max);

	//World Buffer
	if (NShaderCast::CreateConstantBuffer(m_device, sizeof(ConstantBufferWorld), m_pBufferContainer[(int)ConstantBufferType::World]))
	{
		//m_pBufferContainer[(int)ConstantBufferType::World] = buffer;
		//memcpy_s(m_pBufferContainer[(int)ConstantBufferType::World], sizeof(ConstantBufferWorld), buffer, sizeof(ConstantBufferWorld));
	}
	else
	{
		MessageBox(NULL, "constantBuffer World CreateFailed", "CWaterPlaneDrawer constantBufferInit", MB_OK);
		return false;
	}

	//Distorion Buffer
	if (NShaderCast::CreateConstantBuffer(m_device, sizeof(DistortionBuffer), m_pBufferContainer[(int)ConstantBufferType::Distortion]))
	{
		//m_pBufferContainer[(int)ConstantBufferType::Distortion] = distbuffer;
	}
	else
	{
		MessageBox(NULL, "constantBuffer Distortion CreateFailed", "CWaterPlaneDrawer constantBufferInit", MB_OK);
		return false;
	}

	//TessellationBuffer
	if (NShaderCast::CreateConstantBuffer(m_device, sizeof(TessellationBuffer), m_pBufferContainer[(int)ConstantBufferType::TessBuffer]))
	{
		//return true;
	}
	else
	{
		MessageBox(NULL, "constantBuffer Distortion CreateFailed", "CWaterPlaneDrawer constantBufferInit", MB_OK);
		return false;
	}

	//WaveBuffer
	if (NShaderCast::CreateConstantBuffer(m_device, sizeof(WaveBuffer), m_pBufferContainer[(int)ConstantBufferType::WaveBuffer]))
	{
		return true;
	}
	else
	{
		MessageBox(NULL, "constantBuffer Wave CreateFailed", "CWaterPlaneDrawer constantBufferInit", MB_OK);
		return false;
	}

}


bool CWaterPlaneDrawer::samplerInit()
{
	if (NShaderCast::CreateSamplerState(m_device, m_pSamplerState))
	{
		return true;
	}
	else
	{
		MessageBox(NULL, "samplerstate CreateFailed", "CWaterPlaneDrawer samplerInit", MB_OK);
		return false;
	}
}

bool CWaterPlaneDrawer::Init(
	ID3D11Device* dev,
	ID3D11DeviceContext* devCon)
{
	m_device = dev;
	m_deviceContext = devCon;

	if (!meshInit())
	{
		MessageBox(NULL, "meshInitFailed.", "CWaterPlaneDrawer Init", MB_OK);
		return false;
	}

	if (!decalInit())
	{
		MessageBox(NULL, "decalInitFailed.", "CWaterPlaneDrawer Init", MB_OK);
		return false;
	}

	if (!shaderInit())
	{
		MessageBox(NULL, "shaderInitFailed", "CWaterPlaneDrawer Init", MB_OK);
		return false;
	}

	if (!constantBufferInit())
	{
		MessageBox(NULL, "constantBufferInitFailed", "CWaterPlaneDrawer Init", MB_OK);
		return false;
	}

	if (!samplerInit())
	{
		MessageBox(NULL, "samplerInitFailed", "CWaterPlaneDrawer Init", MB_OK);
		return false;
	}

	m_fdistortion = 0.3f;
	m_fspeed = 3.5f;
	m_fpower = 0.8f;
	m_ftime = 0.0f;
	m_frefractratio = 0.4f;
	m_vdiffuse = D3DXVECTOR4(1, 1, 1, 1);

	m_ftessFactor = 200.0f;
	m_finsideTessFactor = 200.0f;

	m_vwaveCenter = D3DXVECTOR3(0,0,0);
	m_famplitude = 0.01f;
	m_fwavelength = -0.1f;
	m_fwavecycle = 70.0f;
	m_fwavetime = 0.0f;
	m_fwaveedge = 10.0f;

	return true;

}

void CWaterPlaneDrawer::updateWorldBuffer(const D3DXMATRIX& matrix)
{
	ConstantBufferWorld cb;
	::ZeroMemory(&cb, sizeof(ConstantBufferWorld));
	D3DXMatrixTranspose(&cb.world, &matrix);
	D3DXVECTOR4 diffuse = m_vdiffuse;
	cb.diffuse = diffuse;
	m_deviceContext->UpdateSubresource(
		m_pBufferContainer[(int)ConstantBufferType::World], 0, NULL, &cb, 0, 0);
}

void CWaterPlaneDrawer::SetParameter(
	const float dist, 
	const float speed, 
	const float pow, 
	const float refract)
{
	m_fdistortion = dist;
	m_fspeed = speed;
	m_fpower = pow;
	m_frefractratio = refract;
}

void CWaterPlaneDrawer::SetTessCoeficient(const float tess, const float in_tess)
{
	m_ftessFactor = tess;
	m_finsideTessFactor = in_tess;
}

void CWaterPlaneDrawer::SetDiffuse(const float r, const float g, const float b, const float a)
{
	m_vdiffuse = D3DXVECTOR4(r, g, b, a);
}

void CWaterPlaneDrawer::SetWaveParameter(const float amplitude, const float length, const float cycle, const float edge)
{
	m_famplitude = amplitude;
	m_fwavelength = length;
	m_fwavecycle = cycle;
	m_fwaveedge = edge;
}

void CWaterPlaneDrawer::updateDistortionWorld(
	const float dist, 
	const float speed, 
	const float power, 
	const float time, 
	const float refract)
{
	DistortionBuffer distBuffer;
	distBuffer.Distortion = dist;
	distBuffer.Speed = speed;
	distBuffer.Power = power;
	distBuffer.Time = m_ftime;
	distBuffer.RefractiveRatio = refract;
	m_deviceContext->UpdateSubresource(
		m_pBufferContainer[(int)ConstantBufferType::Distortion], 0, NULL, &distBuffer, 0, 0);
	m_ftime += D3DXToRadian(1);
	if (m_ftime >= D3DXToRadian(720))
	{
		m_ftime = 0;
	}
}

void CWaterPlaneDrawer::updateTessellationBuffer(const float tess, const float in_tess)
{
	TessellationBuffer tessBuffer;
	tessBuffer.TessFactor = tess;
	tessBuffer.InsideTessFactor = in_tess;
	m_deviceContext->UpdateSubresource(
		m_pBufferContainer[(int)ConstantBufferType::TessBuffer], 0, NULL, &tessBuffer, 0, 0);

}

void CWaterPlaneDrawer::updateWaveBuffer(const float amplitude, const float wavelength, const float wavecycle,const float waveedge)
{
	WaveBuffer waveBuffer;
	waveBuffer.WaveCenter = m_vwaveCenter;
	waveBuffer.Amplitude = amplitude;
	waveBuffer.WaveLength = wavelength;
	waveBuffer.WaveCycle = wavecycle;
	waveBuffer.Time = m_fwavetime;
	waveBuffer.WaveEdge = waveedge;
	m_deviceContext->UpdateSubresource(
		m_pBufferContainer[(int)ConstantBufferType::WaveBuffer], 0, NULL, &waveBuffer, 0, 0);

	m_fwavetime += 1.0f;
	//if (m_fwavetime >= m_fwavecycle)
	//{
	//	m_fwavetime = 0;
	//}

}

void CWaterPlaneDrawer::setBufferToShaders(ConstantBufferType type)
{
	int num = (int)type;
	m_deviceContext->VSSetConstantBuffers(num, 1, &m_pBufferContainer[num]);
	m_deviceContext->PSSetConstantBuffers(num, 1, &m_pBufferContainer[num]);
	m_deviceContext->GSSetConstantBuffers(num, 1, &m_pBufferContainer[num]);
	m_deviceContext->HSSetConstantBuffers(num, 1, &m_pBufferContainer[num]);
	m_deviceContext->DSSetConstantBuffers(num, 1, &m_pBufferContainer[num]);
}

void CWaterPlaneDrawer::setVerticesAndIndex()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	m_deviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

	m_deviceContext->PSSetShaderResources((int)TextureType::Default, 1, &m_pSRVContainer[(int)TextureType::Default]);
	m_deviceContext->PSSetShaderResources((int)TextureType::Normal, 1, &m_pSRVContainer[(int)TextureType::Normal]);

	m_deviceContext->Draw(4, 0);

}

void CWaterPlaneDrawer::Draw(const D3DXMATRIX& matrix)
{
	updateWorldBuffer(matrix);
	updateDistortionWorld(
		m_fdistortion,
		m_fspeed,
		m_fpower,
		m_ftime,
		m_frefractratio);
	updateTessellationBuffer(m_ftessFactor, m_finsideTessFactor);
	updateWaveBuffer(m_famplitude, m_fwavelength, m_fwavecycle,m_fwaveedge);

	setBufferToShaders(ConstantBufferType::World);
	setBufferToShaders(ConstantBufferType::Distortion);
	setBufferToShaders(ConstantBufferType::TessBuffer);
	setBufferToShaders(ConstantBufferType::WaveBuffer);

	m_pShader->SetVertexShader(m_deviceContext, (int)ShaderType::Default);
	m_pShader->SetHullShader(m_deviceContext, (int)ShaderType::Default);
	m_pShader->SetDomainShader(m_deviceContext, (int)ShaderType::Default);
	m_pShader->SetGeometryShader(m_deviceContext, (int)ShaderType::NotUse);//Not Use.
	m_pShader->SetPixelShader(m_deviceContext, (int)ShaderType::Default);

	setVerticesAndIndex();

	m_pShader->SetHullShader(m_deviceContext, (int)ShaderType::NotUse);
	m_pShader->SetDomainShader(m_deviceContext, (int)ShaderType::NotUse);
}