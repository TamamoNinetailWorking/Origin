#include "System/Shader/Shader.h"
#include "System/Dependence.h"
#include "Framework/Camera/Camera.h"
#include "System/WINDOW/CWindow.h"
#include "System/Input/CDirectInput.h"

#include "Framework/Drawer/3D/D3DDrawer3D.h"

#include "System/Utility/MathUtility.h"

#include <iostream>
#include <random>

//デフォルトカレントディレクトリ用
const UINT m_name_size = 1024;
char m_default_directory[m_name_size];

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
};

struct DistortionBuffer
{
	float Time;
	float Speed;
	float Power;
	float Distortion;
	float RefractiveRatio;
	D3DXVECTOR3 padding;
};

//頂点レイアウト
struct SimpleVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};

struct SimPolygonVertex
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 uv;
};

CShader* m_shader;
CShader* m_sepiaShader;
CShader* m_FresnelShader;
CShader* m_ReflectShader;

//頂点バッファ
ID3D11Buffer* m_VertexBuffer;
//ビュー、プロジェクションのバッファ
ID3D11Buffer* m_ViewAndProjectionBuffer;
//モデル毎のバッファ
ID3D11Buffer* m_WorldBuffer;
ID3D11Buffer* m_WorldTestBuffer;

//テクスチャサンプラ
ID3D11SamplerState* m_TextureSampler;
//テクスチャ
ID3D11ShaderResourceView* m_Texture;
ID3D11ShaderResourceView* m_NormalTexture;
extern ID3D11ShaderResourceView* m_pSRV;
extern ID3D11ShaderResourceView* m_pReflectSRV;

CD3DDrawer3D* m_Drawer;

float c_wScale = 200.0f;
//Distortion
ID3D11Buffer* m_DistBuffer;
float m_time = 0;

extern D3DXVECTOR3 m_WaterHeight;

bool Process_Init()
{
	//シェーダー管理クラスの作成
	m_shader = new CShader(GetDevice11(),GetImmediateContext());

	m_sepiaShader = new CShader(GetDevice11(), GetImmediateContext());

	m_FresnelShader = new CShader(GetDevice11(), GetImmediateContext());

	m_ReflectShader = new CShader(GetDevice11(), GetImmediateContext());

	m_Drawer = new CD3DDrawer3D();

	bool sts;

	//頂点レイアウトの作成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },

		{ "NORMAL" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },

		{ "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },

	};

	/*D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },

	{ "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },

	};*/

	UINT numElements = ARRAYSIZE(layout);

	//ディレクトリ取得
	GetCurrentDirectory(m_name_size, m_default_directory);
	//ディレクトリ移動
	SetCurrentDirectory("HLSL/Square/");


	//頂点シェーダーの作成
	sts = m_shader->CreateVertexShader(
		(char*)"SquareVertex.hlsl",
		(char*)"main", layout, numElements);
	if (!sts)
	{
		MessageBox(nullptr, "VertexShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	//ピクセルシェーダーの作成
	sts = m_shader->CreatePixelShader(
		(char*)"SquarePixel.hlsl",
		(char*)"Surf");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = m_FresnelShader->CreatePixelShader(
		(char*)"SquarePixel.hlsl",
		(char*)"Bottom");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = m_ReflectShader->CreatePixelShader(
		(char*)"SquarePixel.hlsl",
		(char*)"Reflect");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	//ディレクトリを元に戻す
	SetCurrentDirectory(m_default_directory);

	//頂点バッファの作成
	SimpleVertex vertices[] = 
	{
		D3DXVECTOR3(-0.5f,0.5f,0.0f),D3DXVECTOR3(0,1,0),D3DXVECTOR2(0,0),
		D3DXVECTOR3(0.5f,0.5f,0.0f),D3DXVECTOR3(0,1,0),D3DXVECTOR2(1,0),
		D3DXVECTOR3(-0.5f,-0.5f,0.0f),D3DXVECTOR3(0,1,0),D3DXVECTOR2(0,1),
		D3DXVECTOR3(0.5f,-0.5f,0.0f),D3DXVECTOR3(0,1,0),D3DXVECTOR2(1,1),
	};

	int vertices_num = ARRAYSIZE(vertices);

	SimPolygonVertex vertices2d[] =
	{
		D3DXVECTOR2(0,50),D3DXVECTOR2(0,0),
		D3DXVECTOR2(50,50),D3DXVECTOR2(1,0),
		D3DXVECTOR2(0,100),D3DXVECTOR2(0,1),
		D3DXVECTOR2(50,100),D3DXVECTOR2(1,1),
	};

	int vertices2d_num = ARRAYSIZE(vertices2d);

	sts = m_shader->CreateVertexBuffer(
		sizeof(SimpleVertex), vertices_num, (void*)vertices, &m_Drawer->GetVertexBuffer());
	if (!sts)
	{
		MessageBox(nullptr, "VertexBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	SetCurrentDirectory("Assets/image/");

	//テクスチャ情報の作成
	sts = m_shader->CreateSRVfromTGAFile("WaterBasicDaytimeGradient.tga", &m_Texture);
	if (!sts)
	{
		MessageBox(nullptr, "Texture CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = CShader::CreateSRVfromTGAFile("stones_NM_height.tga", &m_NormalTexture);
	if (!sts)
	{
		MessageBox(nullptr, "Normal Texture CreateFailed.", "Error", MB_OK);
		return false;
	}

	//ディレクトリを元に戻す
	SetCurrentDirectory(m_default_directory);

	//テクスチャサンプラーの作成
	sts = m_shader->CreateSamplerState(m_TextureSampler);
	if (!sts)
	{
		MessageBox(nullptr, "TextureSampler CreateFailed.", "Error", MB_OK);
		return false;
	}


	//コンスタントバッファの作成
	//sts = m_shader->CreateConstantBuffer(sizeof(ConstantBuffer_ViewProj), &m_ViewAndProjectionBuffer);
	//if (!sts)
	//{
	//	MessageBox(nullptr, "ViewProjectionBuffer CreateFailed.", "Error", MB_OK);
	//	return false;
	//}
	
	sts = m_shader->CreateConstantBuffer(sizeof(ConstantBuffer_World), &m_WorldBuffer);
	if (!sts)
	{
		MessageBox(nullptr, "WorldBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = CShader::CreateConstantBuffer(sizeof(DistortionBuffer), &m_DistBuffer);
	if (!sts)
	{
		MessageBox(nullptr, "WorldBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	//入力周りの初期化
	//CDirectInput::GetInstance().Init(g_pWindow->GetHinstance(), g_pWindow->GetHWND(), g_pWindow->GetWindowWidth(), g_pWindow->GetWindowHeight());

	/*D3DXMATRIX view, proj;
	ConstantBuffer_ViewProj cbvp;
	view = CCamera::GetInstance()->GetGameraMatrix();
	D3DXMatrixTranspose(&cbvp.view, &view);
	proj = CCamera::GetInstance()->GetProjectionMatrix();
	D3DXMatrixTranspose(&cbvp.proj, &proj);

	GetImmediateContext()->UpdateSubresource(m_ViewAndProjectionBuffer, 0, NULL, &cbvp, 0, 0);

	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_ViewAndProjectionBuffer);
	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_ViewAndProjectionBuffer);*/

	return true;
}

void Process_Draw()
{
	//g_pDGraphics->Clear();
	//CDirectGraphics::GetInstance().Clear();

	//AddOnMaxBlendState();
	DefaultBlendState();
	DepthStencilSwitch(true);

	//レンダーターゲットからシェーダーリソースの作成
	/*ID3D11Resource* pResource = nullptr;
	ID3D11ShaderResourceView* pSRV = nullptr;
	CDirectGraphics::GetInstance().GetRenderTargetView()->GetResource(&pResource);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	HRESULT hr = GetDevice11()->CreateShaderResourceView(pResource, &srvDesc, &pSRV);
	Release(pResource);*/


	D3DXMATRIX world,scale,trans;
	D3DXVECTOR4 diffuse = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
	ConstantBuffer_World cm;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&trans);
	D3DXMatrixScaling(&scale, c_wScale, c_wScale, c_wScale);
	D3DXMatrixTranslation(&trans, 0, 0.8f, 0);
	D3DXMatrixRotationX(&world, D3DXToRadian(90));
	world = world * scale * trans;
	D3DXMatrixTranspose(&cm.world, &world);
	cm.diffuse = diffuse;

	D3DXVECTOR3 vec = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	
	//べくとると行列の掛け算をここで行う
	D3DXVec3TransformCoord(&m_WaterHeight, &vec, &world);

	//CDirectInput::GetInstance().GetKeyBuffer();

	/*static bool change = false;
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
	{
		change = !change;
		std::cout << "Space Push" << std::endl;
	}*/


	GetImmediateContext()->UpdateSubresource(m_WorldBuffer, 0, NULL, &cm, 0, 0);

	DistortionBuffer distBuffer;
	//distBuffer.Distortion = MathUtility::CRandom::GetInstance().RandomRangeReal<float>(-1.0f, 1.0f);
	distBuffer.Distortion = 0.3f;
	//distBuffer.Speed = MathUtility::CRandom::GetInstance().RandomRangeInteger<int>(1, 60);
	distBuffer.Speed = 3.5f;
	//distBuffer.Power = MathUtility::CRandom::GetInstance().RandomRangeReal<float>(0.1f, 1.0f);
	distBuffer.Power = 0.8f;
	distBuffer.Time = m_time;
	distBuffer.RefractiveRatio = 0.4f;
	GetImmediateContext()->UpdateSubresource(m_DistBuffer, 0, NULL, &distBuffer, 0, 0);
	m_time += D3DXToRadian(1);
	if (m_time >= D3DXToRadian(720))
	{
		m_time = 0;
	}
	//m_time += 1.0f;

	GetImmediateContext()->VSSetShader(m_shader->GetVertexShader(), NULL, 0);

	/*if (!change) 
	{

	}
	else
	{
		GetImmediateContext()->PSSetShader(m_sepiaShader->GetPixelShader(), NULL, 0);
	}*/

	GetImmediateContext()->PSSetShader(m_shader->GetPixelShader(), NULL, 0);

	GetImmediateContext()->VSSetConstantBuffers(1, 1, &m_WorldBuffer);
	GetImmediateContext()->PSSetConstantBuffers(1, 1, &m_WorldBuffer);

	GetImmediateContext()->VSSetConstantBuffers(2, 1, &m_DistBuffer);
	GetImmediateContext()->PSSetConstantBuffers(2, 1, &m_DistBuffer);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	GetImmediateContext()->IASetVertexBuffers(0, 1, &m_Drawer->GetVertexBuffer(), &stride, &offset);
	GetImmediateContext()->IASetInputLayout(m_shader->GetVertexLayout());

	GetImmediateContext()->PSSetSamplers(0, 1, &m_TextureSampler);
	GetImmediateContext()->PSSetShaderResources(0, 1, &m_Texture);
	GetImmediateContext()->PSSetShaderResources(1, 1, &m_NormalTexture);
	GetImmediateContext()->PSSetShaderResources(2, 1, &m_pSRV);
	//Release(pSRV);


	GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	GetImmediateContext()->Draw(4, 0);



	//Refraction
	//AddOnMaxBlendState();
	//DepthStencilSwitch(false);
	//GetImmediateContext()->PSSetShader(m_FresnelShader->GetPixelShader(), NULL, 0);
	//GetImmediateContext()->PSSetShaderResources(0, 1, &m_pSRV);
	//GetImmediateContext()->Draw(4, 0);

	////Reflection
	//AddOnMaxBlendState();
	//DepthStencilSwitch(false);
	//GetImmediateContext()->PSSetShader(m_ReflectShader->GetPixelShader(), NULL, 0);
	//GetImmediateContext()->PSSetShaderResources(0, 1, &m_pReflectSRV);
	//GetImmediateContext()->Draw(4, 0);

	//Release(m_pSRV);
	//Release(m_pReflectSRV);
	//g_pDGraphics->Present();
	//CDirectGraphics::GetInstance().Present();
}