#include "../System/Dependence.h"
#include "../Framework/Camera/CameraObject.h"

#include "../Framework/Object/WaterPlane/WaterPlaneObject.h"
#include "../Framework/Object/ModelObject/SkydomeObject.h"

#include "../System/Shader II/ShaderCast/ShaderCast.h"
#include "../System/Shader II/ShaderSet.h"
#include "../System/DirectoryManager/DirectoryManager.h"

#include "../System/WINDOW/CWindow.h"


ID3D11Texture2D* m_pCubeReflectTexture;
ID3D11ShaderResourceView* m_pCubeReflectTextureSRV;
ID3D11RenderTargetView* m_pCubeRTV;
ID3D11Texture2D* m_pCubeDS;
ID3D11DepthStencilView* m_pCubeDSV;

CShaderSet* m_pShader;

extern CWINDOW* g_pWindow;
extern CCameraObject* m_Camera;
extern CSkydomeObject* m_Skydome;

struct CubeMapBuffer
{
	D3DXMATRIX View[6];
	D3DXMATRIX Projection;
};

ID3D11Buffer* m_pCubeMapBuffer;

static bool shaderInit(ID3D11Device* device)
{
	m_pShader = new CShaderSet();

	CDirectoryManager::GetInstance().SetDefaultDirectory();
	CDirectoryManager::GetInstance().SetDirectory("HLSL/CubeMap/");

	bool status;
	
	status = m_pShader->CreateVertexShader(device, "CubeMapVertex.hlsl", "main");
	if (!status)
	{
		MessageBox(nullptr, "CreateCubeMapVertex Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	status = m_pShader->CreateGeometryShader(device, "CubeMapGeometry.hlsl", "main");
	if (!status)
	{
		MessageBox(nullptr, "CreateCubeMapGeometry Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	status = m_pShader->CreatePixelShader(device, "CubeMapPixel.hlsl", "main");
	if (!status)
	{
		MessageBox(nullptr, "CreateCubeMapPixel Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	CDirectoryManager::GetInstance().SetDefaultDirectory();

	return true;
}

static bool constantBufferInit(ID3D11Device* device)
{
	if (!NShaderCast::CreateConstantBuffer(device, sizeof(CubeMapBuffer), m_pCubeMapBuffer))
	{
		MessageBox(nullptr, "CreateCubeMapConstantBuffer Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	return true;
}

bool CubeMapInit()
{
	HRESULT hr;
	//Create ReflectionCubeMap Texture
	D3D11_TEXTURE2D_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.ArraySize = 6;
	Desc.Width = 1024;
	Desc.Height = 1024;
	Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.CPUAccessFlags = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
	Desc.MipLevels = 1;

	hr = GetDevice11()->CreateTexture2D(&Desc, NULL, &m_pCubeReflectTexture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateReflectionCubeMapTexture Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	//Create ReflectionCubeMap ShaderResourceView
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = Desc.Format;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	descSRV.TextureCube.MipLevels = Desc.MipLevels;
	descSRV.TextureCube.MostDetailedMip = 0;

	hr = GetDevice11()->CreateShaderResourceView(m_pCubeReflectTexture, &descSRV, &m_pCubeReflectTextureSRV);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateReflectionCubeMapShaderResourceView Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	//Create ReflectionCubeMap RenderTargetView
	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	renderDesc.Format = Desc.Format;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	renderDesc.Texture2DArray.ArraySize = 6;
	renderDesc.Texture2DArray.FirstArraySlice = 0;
	renderDesc.Texture2DArray.MipSlice = 0;

	hr = GetDevice11()->CreateRenderTargetView(m_pCubeReflectTexture, &renderDesc, &m_pCubeRTV);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateReflectionCubeMapRenderTargetView Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	//Create ReflectionCubeMap DepthStencil Texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = 1024;
	descDepth.Height = 1024;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 6;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	
	hr = GetDevice11()->CreateTexture2D(&descDepth, NULL, &m_pCubeDS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateReflectionCubeMapDepthStencilTexture Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	//Create ReflectionCubeMap DepthStencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	descDSV.Texture2DArray.FirstArraySlice = 0;
	descDSV.Texture2DArray.ArraySize = 6;
	descDSV.Texture2DArray.MipSlice = 0;
	descDSV.Flags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	hr = GetDevice11()->CreateDepthStencilView(m_pCubeDS, NULL, &m_pCubeDSV);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateReflectionCubeMapDepthStencilView Failed.", "CubeMapInit", MB_OK);
		return false;
	}

	if (!shaderInit(GetDevice11()))
	{
		return false;
	}

	if (!constantBufferInit(GetDevice11()))
	{
		return false;
	}

	return true;
}


void CubeMapUpdate()
{
	D3DXMATRIX view[6];
	D3DXVECTOR3 Eye;
	D3DXVECTOR3 Lookat;
	D3DXVECTOR3 Up;



	//+X
	//Eye = m_Camera->GetLookat();
	Eye = D3DXVECTOR3(0, 0, 0);
	Lookat = D3DXVECTOR3(1, 0, 0);
	Up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&view[0], &Eye, &Lookat, &Up);
	
	//-X
	//Eye = D3DXVECTOR3(0, 0, 0);
	Lookat = D3DXVECTOR3(-1, 0, 0);
	Up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&view[1], &Eye, &Lookat, &Up);

	//+Y
	//Eye = D3DXVECTOR3(0, 0, 0);
	Lookat = D3DXVECTOR3(0, 1, 0);
	Up = D3DXVECTOR3(0, 0, -1);
	D3DXMatrixLookAtLH(&view[2], &Eye, &Lookat, &Up);

	//-Y
	//Eye = D3DXVECTOR3(0, 0, 0);
	Lookat = D3DXVECTOR3(0, -1, 0);
	Up = D3DXVECTOR3(0, 0, 1);
	D3DXMatrixLookAtLH(&view[3], &Eye, &Lookat, &Up);

	//+Z
	//Eye = D3DXVECTOR3(0, 0, 0);
	Lookat = D3DXVECTOR3(0, 0, 1);
	Up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&view[4], &Eye, &Lookat, &Up);

	//-Z
	//Eye = D3DXVECTOR3(0, 0, 0);
	Lookat = D3DXVECTOR3(0, 0, -1);
	Up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&view[5], &Eye, &Lookat, &Up);

	float aspect = (FLOAT)g_pWindow->GetWindowWidth() / (FLOAT)g_pWindow->GetWindowHeight();
	D3DXMATRIX Projection;
	D3DXMatrixPerspectiveFovLH(
		&Projection, 
		D3DX_PI / 2, 
		aspect, 
		0.01f, 
		10000.0f);

	CubeMapBuffer cmb;
	ZeroMemory(&cmb, sizeof(CubeMapBuffer));
	for (int i = 0; i < 6; i++)
	{
		D3DXMatrixTranspose(&cmb.View[i], &view[i]);
	}
	//D3DXMATRIX projection = m_Camera->GetProjectionMatrix();
	D3DXMatrixTranspose(&cmb.Projection, &Projection);

	GetImmediateContext()->UpdateSubresource(
		m_pCubeMapBuffer, 0, NULL, &cmb, 0, 0);

}

void CubeMapDraw()
{
	float ClearColor[4] = { 1,1,1,1 };
	CDirectGraphics::GetInstance().SetRenderTarget(m_pCubeRTV,m_pCubeDSV);
	CDirectGraphics::GetInstance().SetViewPort(1024, 1024);
	GetImmediateContext()->ClearRenderTargetView(m_pCubeRTV, ClearColor);
	GetImmediateContext()->ClearDepthStencilView(m_pCubeDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_pCubeMapBuffer);
	GetImmediateContext()->GSSetConstantBuffers(0, 1, &m_pCubeMapBuffer);
	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_pCubeMapBuffer);

	m_Skydome->DrawAsCubeMap();

	CDirectGraphics::GetInstance().SetDefaultViewPort();
	CDirectGraphics::GetInstance().SetDefaultRenderTarget();
}