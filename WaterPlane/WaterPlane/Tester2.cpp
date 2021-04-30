#include "System/Shader/Shader.h"
#include "System/Dependence.h"
#include "Framework/Camera/Camera.h"
#include "System/WINDOW/CWindow.h"
#include "System/Input/CDirectInput.h"

#include "Framework/Drawer/3D/D3DDrawer3D.h"
#include "Framework/Mesh/XFile/XFile.h"

#include "Framework/Camera/CameraObject.h"

#include "System/Shader II/ShaderSet.h"
#include "Framework/Object/ModelObject/SkydomeObject.h"

#include <iostream>

//コンスタントバッファ
struct ConstantBuffer_ViewProj
{
	D3DXMATRIX view;
	D3DXMATRIX proj;
};

struct ConstantBuffer_World
{
	D3DXMATRIX world;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 emissive;
};

struct ConstantBuffer_Color
{
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 emissive;
	D3DXVECTOR4 WaterHeight;
};

struct ConstantBuffer_Light
{
	D3DXVECTOR4 Light;
};

//CurrentDirectory
const UINT m_directory_name_size = 1024;
char m_default_current_directory[m_directory_name_size];

CShader* m_TesterShader;
CShader* m_TesterSubShader;
CXFile* m_SphereTester;
CD3DDrawer3D* m_TesterDrawer;
CCameraObject* m_Camera;

CShaderSet* m_ShaderII;

CSkydomeObject* m_Skydome;

CCameraObject* m_SubCamera;

ID3D11Buffer* m_TestWorld;
ID3D11Buffer* m_TestColor;
ID3D11Buffer* m_TestLight;
ID3D11ShaderResourceView* m_StageTexture;
ID3D11SamplerState* m_TesterSampler;

ID3D11RenderTargetView* m_pTargetView;
ID3D11ShaderResourceView* m_pSRV;

ID3D11RenderTargetView* m_pReflectView;
ID3D11ShaderResourceView* m_pReflectSRV;

float c_scale = 0.1f;
float c_turn = 500.0f;

D3DXVECTOR4 mat_ambient = D3DXVECTOR4(0.5, 0.5, 0.5, 1);
D3DXVECTOR4 mat_diffuse = D3DXVECTOR4(0.8, 0.8, 0.8, 1);
D3DXVECTOR4 mat_specular = D3DXVECTOR4(0.1, 0.1, 0.1, 1);
D3DXVECTOR4 mat_emissive = D3DXVECTOR4(0.1, 0.1, 0.1, 1);

D3DXVECTOR4 c_ambient = D3DXVECTOR4(0.8, 0.4, 0.4, 1);
D3DXVECTOR4 c_diffuse = D3DXVECTOR4(0.2, 0.5, 0.8, 1);
D3DXVECTOR4 c_specular = D3DXVECTOR4(1, 1, 1, 1);
D3DXVECTOR4 c_emissive = D3DXVECTOR4(1, 1, 1, 1);

D3DXVECTOR4 c_light = D3DXVECTOR4(0, 10, 0, 0);
//サンプラー
//ID3D11SamplerState* m_TesterTextureSampler;
//テクスチャ
//ID3D11ShaderResourceView* m_NormalTexture;

D3DXVECTOR3 m_WaterHeight = D3DXVECTOR3(0, 0, 0);

bool TesterInit()
{
	//ディレクトリ取得
	GetCurrentDirectory(m_directory_name_size, m_default_current_directory);

	m_TesterShader = new CShader(GetDevice11(), GetImmediateContext());
	m_TesterSubShader = new CShader(GetDevice11(), GetImmediateContext());
	m_TesterDrawer = new CD3DDrawer3D();

	m_ShaderII = new CShaderSet();

	//ディレクトリ移動
	SetCurrentDirectory("Assets/mesh/");
	m_SphereTester = new CXFile("stage1.x",GetDevice9());
	SetCurrentDirectory(m_default_current_directory);

	bool sts;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },

		{ "NORMAL" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },

		{ "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};

	UINT numElements = ARRAYSIZE(layout);
	//ディレクトリ移動
	SetCurrentDirectory("HLSL/SimpleModel/");

	//頂点シェーダーの作成
	sts = m_TesterShader->CreateVertexShader(
		(char*)"SimpleModelVertex.hlsl",
		(char*)"pplmain", layout, numElements);
	if (!sts)
	{
		MessageBox(nullptr, "VertexShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	//ピクセルシェーダーの作成
	sts = m_TesterShader->CreatePixelShader(
		(char*)"SimpleModelPixel.hlsl",
		(char*)"pplmain");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	ID3D11Device* device = GetDevice11();
	sts = m_ShaderII->CreatePixelShader(device, "SimpleModelPixel.hlsl", "pplmain");
	if(!sts)
	{
		MessageBox(nullptr, "PixelShader.", "Error", MB_OK);
		return false;
	}

	sts = m_ShaderII->CreateVertexShader(device, "SimpleModelVertex.hlsl",
		"pplmain");
	if (!sts)
	{
		MessageBox(nullptr, "VertexShader.", "Error", MB_OK);
		return false;
	}

	sts = m_TesterSubShader->CreatePixelShader(
		(char*)"SimpleModelPixel.hlsl",
		(char*)"CullingPPLMain");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	//頂点バッファ
	sts = m_TesterShader->CreateVertexBuffer(
		sizeof(XFile::Vertex),
		m_SphereTester->GetVertexNum(),
		m_SphereTester->GetVertices(),
		&m_TesterDrawer->GetVertexBuffer());
	if (!sts)
	{
		MessageBox(nullptr, "VertexBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	//インデックスバッファ
	sts = m_TesterShader->CreateIndexBuffer(
		m_SphereTester->GetIndexNum(),
		m_SphereTester->GetIndex(),
		&m_TesterDrawer->GetIndexBuffer());
	if (!sts)
	{
		MessageBox(nullptr, "IndexBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = CShader::CreateConstantBuffer(sizeof(ConstantBuffer_World), &m_TestWorld);
	if (!sts)
	{
		MessageBox(nullptr, "ConstantBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = CShader::CreateConstantBuffer(sizeof(ConstantBuffer_Color), &m_TestColor);
	if (!sts)
	{
		MessageBox(nullptr, "ConstantBufferColor CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = CShader::CreateConstantBuffer(sizeof(ConstantBuffer_Light), &m_TestLight);
	if (!sts)
	{
		MessageBox(nullptr, "ConstantBufferLight CreateFailed.", "Error", MB_OK);
		return false;
	}

	////ディレクトリ移動
	SetCurrentDirectory(m_default_current_directory);
	//SetCurrentDirectory("Assets/image/");
	SetCurrentDirectory("Assets/mesh/");
	sts = CShader::CreateSRVfromWICFile(m_SphereTester->GetDecal()->c_str(), &m_StageTexture);	
	if (!sts)
	{
		sts = CShader::CreateSRVfromTGAFile(m_SphereTester->GetDecal()->c_str(), &m_StageTexture);
		if (!sts)
		{
			std::cout << m_SphereTester->GetDecal()->c_str() << std::endl;
			MessageBox(nullptr, "Decal CreateFailed.", "Error", MB_OK);
			return false;
		}
	}

	SetCurrentDirectory(m_default_current_directory);

	sts = CShader::CreateSamplerState(m_TesterSampler);
	if (!sts)
	{
		MessageBox(nullptr, "Sampler CreateFailed.", "Error", MB_OK);
		return false;
	}

	CDirectInput::GetInstance().Init(g_pWindow->GetHinstance(), g_pWindow->GetHWND(), g_pWindow->GetWindowWidth(), g_pWindow->GetWindowHeight());

	m_Camera = new CCameraObject();

	m_Camera->Init(
		0.1f,
		10000.0f,
		D3DX_PI / 4,
		g_pWindow->GetWindowWidth(),
		g_pWindow->GetWindowHeight(),
		D3DXVECTOR3(0, 150, -300),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 1, 0));
	sts = m_TesterShader->CreateConstantBuffer(sizeof(CCameraObject::ConstantBuffer), &m_Camera->GetCameraBuffer());
	if (!sts)
	{
		MessageBox(nullptr, "Buffer CreateFailed.", "Error", MB_OK);
		return false;
	}
	m_Camera->UpdateBuffer(GetImmediateContext());
	
	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());

	m_SubCamera = new CCameraObject();

	//D3DXVECTOR3(0, 250, -20),
	//	D3DXVECTOR3(0, 10, 1000),
	//	D3DXVECTOR3(0, 1, -0.5)

	m_SubCamera->Init(
		0.1f,
		10000.0f,
		D3DX_PI / 4,
		g_pWindow->GetWindowWidth(),
		g_pWindow->GetWindowHeight(),
		D3DXVECTOR3(0, 5, -10),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 1, 0));

	sts = m_TesterShader->CreateConstantBuffer(sizeof(CCameraObject::ConstantBuffer), &m_SubCamera->GetCameraBuffer());
	if (!sts)
	{
		MessageBox(nullptr, "Buffer CreateFailed.", "Error", MB_OK);
		return false;
	}
	m_SubCamera->UpdateBuffer(GetImmediateContext());

	//GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	//GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());

	D3DXVECTOR3 pos = m_Camera->GetTransform().GetPosition();

	//std::cout << pos.x << pos.y << pos.z << std::endl;

	D3DXMATRIX view = m_Camera->GetCameraMatrix();
	
	std::cout << "CameraObject" << std::endl;
	std::cout << view._11 << " " << view._12 << " " << view._13 << " " << view._14 << std::endl;
	std::cout << view._21 << " " << view._22 << " " << view._23 << " " << view._24 << std::endl;
	std::cout << view._31 << " " << view._32 << " " << view._33 << " " << view._34 << std::endl;
	std::cout << view._41 << " " << view._42 << " " << view._43 << " " << view._44 << std::endl << std::endl;

	m_Skydome = new CSkydomeObject();
	m_Skydome->Start();

	return true;
}

void TesterUpdate()
{

	CDirectInput::GetInstance().GetKeyBuffer();

	m_Camera->Update(GetImmediateContext());

	D3DXVECTOR3 Direction = D3DXVECTOR3(0, 0, 0) - m_Camera->GetEye();
	D3DXVECTOR3 Normal = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 Reflection = Direction - 2 * D3DXVec3Dot(&Direction, &Normal) * Normal;

	D3DXVECTOR3 Up;
	D3DXVECTOR3 right = D3DXVECTOR3(1, 0, 0);
	D3DXVec3Cross(&Up, &right , &Reflection);

	D3DXVECTOR3 Pos = m_Camera->GetEye();
	Pos.y = -10;
	m_SubCamera->SetCamera(Pos, D3DXVECTOR3(0, 0, 0), Up);
	m_SubCamera->UpdateBuffer(GetImmediateContext());


	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	GetImmediateContext()->DSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	//GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_SubCamera->GetCameraBuffer());
	//GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_SubCamera->GetCameraBuffer());

	//if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_TAB))
	//{
	//	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_SubCamera->GetCameraBuffer());
	//	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_SubCamera->GetCameraBuffer());
	//}
	//else
	//{
	//	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	//	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	//}
}

void TesterDraw()
{
	//CDirectGraphics::GetInstance().Clear();

	m_Skydome->Draw();

	//DefaultBlendState();
	//DepthStencilSwitch(true);

	//ConstantBuffer_Color cc;
	//cc.ambient = c_ambient;
	//cc.diffuse = c_diffuse;
	//cc.emissive = c_emissive;
	//cc.specular = c_specular;
	//cc.WaterHeight = D3DXVECTOR4(m_WaterHeight, 0.0f);
	//GetImmediateContext()->UpdateSubresource(m_TestColor, 0, NULL, &cc, 0, 0);
	//GetImmediateContext()->VSSetConstantBuffers(2, 1, &m_TestColor);
	//GetImmediateContext()->PSSetConstantBuffers(2, 1, &m_TestColor);

	//ConstantBuffer_Light cl;
	//cl.Light = c_light;
	//GetImmediateContext()->UpdateSubresource(m_TestLight, 0, NULL, &cl, 0, 0);
	//GetImmediateContext()->VSSetConstantBuffers(3, 1, &m_TestLight);
	//GetImmediateContext()->PSSetConstantBuffers(3, 1, &m_TestLight);

	////m_Camera->Update(GetImmediateContext());

	//static bool change = false;
	//if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
	//{
	//	change = !change;
	//}

	//if (change)
	//{
	//	CDirectGraphics::GetInstance().Mode_WireFrame();
	//}
	//else
	//{
	//	CDirectGraphics::GetInstance().Mode_Solid();
	//}

	//D3DXMATRIX world,rot;
	//ConstantBuffer_World cw;
	//D3DXMatrixIdentity(&world);
	//D3DXMatrixIdentity(&rot);
	//D3DXMatrixScaling(&world, c_scale,c_scale,c_scale);
	////D3DXMatrixRotationYawPitchRoll(&rot,timeGetTime() / c_turn, timeGetTime() / c_turn, 0);
	//world = world * rot;
	//D3DXMatrixTranspose(&cw.world, &world);
	//cw.diffuse = mat_diffuse;
	//cw.ambient = mat_ambient;
	//cw.specular = mat_specular;
	//cw.emissive = mat_emissive;
	//GetImmediateContext()->UpdateSubresource(m_TestWorld, 0, NULL, &cw, 0, 0);

	////GetImmediateContext()->VSSetShader(m_TesterShader->GetVertexShader(), NULL, 0);
	////GetImmediateContext()->IASetInputLayout(m_TesterShader->GetVertexLayout());
	//GetImmediateContext()->VSSetShader(m_ShaderII->GetVertexShader(0), NULL, 0);
	//GetImmediateContext()->IASetInputLayout(m_ShaderII->GetInputLayout(0));
	//ID3D11DeviceContext* devCon = GetImmediateContext();
	////m_ShaderII->SetVertexShader(devCon, 0);
	////GetImmediateContext()->PSSetShader(m_TesterShader->GetPixelShader(), NULL, 0);
	//GetImmediateContext()->PSSetShader(m_ShaderII->GetPixelShader(0), NULL, 0);


	//GetImmediateContext()->VSSetConstantBuffers(1, 1, &m_TestWorld);
	//GetImmediateContext()->PSSetConstantBuffers(1, 1, &m_TestWorld);

	//UINT stride = sizeof(XFile::Vertex);
	//UINT offset = 0;

	//GetImmediateContext()->IASetVertexBuffers(0, 1, &m_TesterDrawer->GetVertexBuffer(), &stride, &offset);

	///*int indexnum = 0;
	//int indexstart = 0;
	//indexstart = indexstart + indexnum;

	//for (UINT cnt = 0; cnt < m_SphereTester->GetMaterialNum(); cnt++)
	//{
	//	int subseeindex = -1;
	//	UINT num = m_SphereTester->GetMaterialNum();
	//	GetImmediateContext()->PSSetShaderResources(0, 1, &m_StageTexture);
	//	
	//	for (UINT j = 0; j < m_SphereTester->GetSubsetNum(); j++)
	//	{
	//		UINT work = m_SphereTester->GetSubsetNum();
	//		short stop = 0;

	//		if (cnt == m_SphereTester->GetSubset(j).m_MaterialIndex)
	//		{

	//		}

	//	}


	//}*/

	//GetImmediateContext()->PSSetShaderResources(0, 1, &m_StageTexture);
	//GetImmediateContext()->PSSetSamplers(0, 1, &m_TesterSampler);
	//GetImmediateContext()->IASetIndexBuffer(m_TesterDrawer->GetIndexBuffer(),DXGI_FORMAT_R32_UINT,0);

	//GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//auto obj = m_SphereTester->GetIndexNum();

	//GetImmediateContext()->DrawIndexed(obj, 0, 0);

	////CDirectGraphics::GetInstance().Present();
}

void TesterDrawBackRender()
{

	ConstantBuffer_Color cc;
	cc.ambient = c_ambient;
	cc.diffuse = c_diffuse;
	cc.emissive = c_emissive;
	cc.specular = c_specular;
	cc.WaterHeight = D3DXVECTOR4(m_WaterHeight,0.0f);
	GetImmediateContext()->UpdateSubresource(m_TestColor, 0, NULL, &cc, 0, 0);
	GetImmediateContext()->VSSetConstantBuffers(2, 1, &m_TestColor);
	GetImmediateContext()->PSSetConstantBuffers(2, 1, &m_TestColor);

	ConstantBuffer_Light cl;
	cl.Light = c_light;
	GetImmediateContext()->UpdateSubresource(m_TestLight, 0, NULL, &cl, 0, 0);
	GetImmediateContext()->VSSetConstantBuffers(3, 1, &m_TestLight);
	GetImmediateContext()->PSSetConstantBuffers(3, 1, &m_TestLight);

	D3DXMATRIX world,rot;
	ConstantBuffer_World cw;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixScaling(&world, c_scale, c_scale, c_scale);
	//D3DXMatrixRotationYawPitchRoll(&rot, timeGetTime() / c_turn, timeGetTime() / c_turn, 0);
	world = world * rot;
	D3DXMatrixTranspose(&cw.world, &world);
	cw.diffuse = mat_diffuse;
	cw.ambient = mat_ambient;
	cw.specular = mat_specular;
	cw.emissive = mat_emissive;
	GetImmediateContext()->UpdateSubresource(m_TestWorld, 0, NULL, &cw, 0, 0);

	GetImmediateContext()->VSSetShader(m_TesterShader->GetVertexShader(), NULL, 0);
	GetImmediateContext()->PSSetShader(m_TesterSubShader->GetPixelShader(), NULL, 0);

	GetImmediateContext()->VSSetConstantBuffers(1, 1, &m_TestWorld);
	GetImmediateContext()->PSSetConstantBuffers(1, 1, &m_TestWorld);

	UINT stride = sizeof(XFile::Vertex);
	UINT offset = 0;

	GetImmediateContext()->IASetVertexBuffers(0, 1, &m_TesterDrawer->GetVertexBuffer(), &stride, &offset);
	GetImmediateContext()->IASetInputLayout(m_TesterShader->GetVertexLayout());

	GetImmediateContext()->PSSetShaderResources(0, 1, &m_StageTexture);
	GetImmediateContext()->PSSetSamplers(0, 1, &m_TesterSampler);
	GetImmediateContext()->IASetIndexBuffer(m_TesterDrawer->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto obj = m_SphereTester->GetIndexNum();

	GetImmediateContext()->DrawIndexed(obj, 0, 0);
}

//最初に書くRenderTargetView用のもの
void TesterTargetDraw()
{
	HRESULT hr;

	ID3D11Texture2D* pTex2D = nullptr;
	D3D11_TEXTURE2D_DESC Tex2DDesc;
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;

	::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	Tex2DDesc.ArraySize = 1;
	Tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Tex2DDesc.Width = g_pWindow->GetWindowWidth();
	Tex2DDesc.Height = g_pWindow->GetWindowHeight();
	Tex2DDesc.MipLevels = 1;
	Tex2DDesc.SampleDesc.Count = 1;

	::ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = Tex2DDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	hr = GetDevice11()->CreateTexture2D(&Tex2DDesc, NULL, &pTex2D);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateTexture2D Error", "Error", MB_OK);
		return;
	}

	hr = GetDevice11()->CreateRenderTargetView(pTex2D, &RTVDesc, &m_pTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateRenderTargetView Error", "Error", MB_OK);
		return;
	}
	Release(pTex2D);

	//仮ターゲットに書き込み
	CDirectGraphics::GetInstance().SetRenderTarget(m_pTargetView);
	float ClearColor[4] = { 0, 0, 0, 1 };//クリアカラー RGBA
	GetImmediateContext()->ClearRenderTargetView(m_pTargetView, ClearColor);

	m_Skydome->Draw();

	TesterDrawBackRender();

	//一度書き込んでからRenderTargetViewを取り出す
	ID3D11Resource* pResource = nullptr;
	m_pTargetView->GetResource(&pResource);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	::ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = RTVDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = GetDevice11()->CreateShaderResourceView(pResource, &srvDesc, &m_pSRV);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateRenderTargetShaderResource Error", "Error", MB_OK);
		return;
	}
	Release(pResource);
	Release(m_pTargetView);

	CDirectGraphics::GetInstance().SetDefaultRenderTarget();
	CDirectGraphics::GetInstance().Clear();
}

void ReflectView()
{
	HRESULT hr;

	ID3D11Texture2D* pTex2D = nullptr;
	D3D11_TEXTURE2D_DESC Tex2DDesc;
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;

	::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	Tex2DDesc.ArraySize = 1;
	Tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Tex2DDesc.Width = g_pWindow->GetWindowWidth();
	Tex2DDesc.Height = g_pWindow->GetWindowHeight();
	Tex2DDesc.MipLevels = 1;
	Tex2DDesc.SampleDesc.Count = 1;

	::ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = Tex2DDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	hr = GetDevice11()->CreateTexture2D(&Tex2DDesc, NULL, &pTex2D);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateTexture2D Error", "Error", MB_OK);
		return;
	}

	hr = GetDevice11()->CreateRenderTargetView(pTex2D, &RTVDesc, &m_pReflectView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateRenderTargetView Error", "Error", MB_OK);
		return;
	}
	Release(pTex2D);

	//仮ターゲットに書き込み
	CDirectGraphics::GetInstance().SetRenderTarget(m_pReflectView);
	float ClearColor[4] = { 0, 0, 0, 1 };//クリアカラー RGBA
	GetImmediateContext()->ClearRenderTargetView(m_pReflectView, ClearColor);

	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_SubCamera->GetCameraBuffer());
	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_SubCamera->GetCameraBuffer());
	m_Skydome->Draw();

	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());
	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_Camera->GetCameraBuffer());

	//一度書き込んでからRenderTargetViewを取り出す
	ID3D11Resource* pResource = nullptr;
	m_pReflectView->GetResource(&pResource);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	::ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = RTVDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = GetDevice11()->CreateShaderResourceView(pResource, &srvDesc, &m_pReflectSRV);
	if (FAILED(hr))
	{
		MessageBox(NULL, "CreateRenderTargetShaderResource Error", "Error", MB_OK);
		return;
	}
	Release(pResource);
	Release(m_pReflectView);

	CDirectGraphics::GetInstance().SetDefaultRenderTarget();
	CDirectGraphics::GetInstance().Clear();
}