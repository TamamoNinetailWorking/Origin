#include "System/Shader/Shader.h"
#include "System/Dependence.h"
#include "Framework/Camera/Camera.h"
#include "System/WINDOW/CWindow.h"
#include "System/Input/CDirectInput.h"

#include "Framework/Drawer/3D/D3DDrawer3D.h"
#include "Framework/Mesh/XFile/XFile.h"

#include "Framework/Camera/CameraObject.h"

#include "System/Shader II/ShaderManager/ShaderManager.h"

#include <iostream>
#include <list>
#include <vector>

struct ConstantBuffer_W
{
	D3DXMATRIX world;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 emissive;
};

struct ConstantBuffer_C
{
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 ambient;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 emissive;
	D3DXVECTOR4 WaterHeight;
};

struct ConstantBuffer_L
{
	D3DXVECTOR4 Light;
};

const UINT m_directory_name_size = 1024;
extern char m_default_current_directory[m_directory_name_size];

CShader* m_kShader;
CShader* m_knotShader;
CShader* m_kNotTexShader;
CShader* m_kNotTexnotShader;
CShader* m_kLineShader;
CXFile* m_kModel;
CD3DDrawer3D* m_kDrawer;
CCameraObject* m_kCamera;

ID3D11Buffer* m_kWorld;
ID3D11Buffer* m_kColor;
ID3D11Buffer* m_kLight;

ID3D11ShaderResourceView* m_kSRV;
ID3D11ShaderResourceView* m_kToon;
ID3D11SamplerState* m_kSampler;

std::vector<ID3D11ShaderResourceView*> m_kSRVList;

float c_kscale = 1.0f;
float c_kturn = 1000.0f;

D3DXVECTOR4 mat_kambient = D3DXVECTOR4(0.5, 0.5, 0.5, 1);
D3DXVECTOR4 mat_kdiffuse = D3DXVECTOR4(0.8, 0.8, 0.8, 1);
D3DXVECTOR4 mat_kspecular = D3DXVECTOR4(1, 1, 1, 1);
D3DXVECTOR4 mat_kemissive = D3DXVECTOR4(0.1, 0.1, 0.1, 1);

D3DXVECTOR4 c_kambient = D3DXVECTOR4(1, 1, 1, 1);
D3DXVECTOR4 c_kdiffuse = D3DXVECTOR4(1,1, 1, 1);
D3DXVECTOR4 c_kspecular = D3DXVECTOR4(1, 1, 1, 1);
D3DXVECTOR4 c_kemissive = D3DXVECTOR4(1, 1, 1, 1);

D3DXVECTOR4 c_klight = D3DXVECTOR4(0, 10, 0, 0);

ID3D11VertexShader* m_vs;
ID3D11InputLayout* m_il;
ID3D11PixelShader* m_ps;

bool KadaiInit()
{
	GetCurrentDirectory(m_directory_name_size, m_default_current_directory);

	m_kShader = new CShader(GetDevice11(), GetImmediateContext());
	m_knotShader = new CShader(GetDevice11(), GetImmediateContext());
	m_kNotTexShader = new CShader(GetDevice11(),GetImmediateContext());
	m_kNotTexnotShader = new CShader(GetDevice11(), GetImmediateContext());
	m_kLineShader = new CShader(GetDevice11(), GetImmediateContext());
	m_kDrawer = new CD3DDrawer3D();

	//SetCurrentDirectory("Assets/mesh/");
	SetCurrentDirectory("Assets/mesh/Maid/");
	//m_kModel = new CXFile("f1.x",GetDevice9());
	m_kModel = new CXFile("maid_pose.x",GetDevice9());
	SetCurrentDirectory(m_default_current_directory);

	bool sts;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ 
			"POSITION" ,
			0 ,
			DXGI_FORMAT_R32G32B32_FLOAT , 
			0 , 
			D3D11_APPEND_ALIGNED_ELEMENT , 
			D3D11_INPUT_PER_VERTEX_DATA , 
			0 },

		{ "NORMAL" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },

		{ "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 ,	D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};

	UINT numElements = ARRAYSIZE(layout);
	//ディレクトリ移動
	SetCurrentDirectory("HLSL/SimpleModel/");

	//頂点シェーダーの作成
	sts = m_kShader->CreateVertexShader(
		(char*)"SimpleModelVertex.hlsl",
		(char*)"pplmain", layout, numElements);
	if (!sts)
	{
		MessageBox(nullptr, "VertexShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	//Hello
	//-----------------------------------------------------------------------------
	//sts = CShaderManager::GetInstance().CheckWhetherVertexShaderIsAbleToChatch("Tester.hlsl", "main");
	//if (!sts)
	//{
	//	//MessageBox(nullptr, "VertexShader Not Catch", "Error", MB_OK);
	//	sts = CShaderManager::GetInstance().GetVertexShader(GetDevice11(), "Tester.hlsl", "main", m_vs, m_il);
	//	if (!sts)
	//	{
	//		MessageBox(nullptr, "VertexShader Not Catch", "Error", MB_OK);
	//	}
	//}

	//sts = CShaderManager::GetInstance().CheckWhetherVertexShaderIsAbleToChatch("Tester.hlsl", "main");
	//if (!sts)
	//{
	//	MessageBox(nullptr, "VertexShader Not Catch", "Error", MB_OK);
	//	return false;
	//}

	//sts = CShaderManager::GetInstance().GetVertexShader(GetDevice11(), "Tester.hlsl", "main", m_vs, m_il);
	//if (!sts)
	//{
	//	MessageBox(nullptr, "VertexShader Not Catch", "Error", MB_OK);
	//}

	

	//-----------------------------------------------------------------------------

	sts = m_knotShader->CreateVertexShader(
		(char*)"SimpleModelVertex.hlsl",
		(char*)"main", layout, numElements);
	if (!sts)
	{
		MessageBox(nullptr, "VertexShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = m_kLineShader->CreateVertexShader(
		(char*)"SimpleModelVertex.hlsl",
		(char*)"LineVS",layout, numElements);
	if (!sts)
	{
		MessageBox(nullptr, "VertexShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	//ピクセルシェーダーの作成
	sts = m_kShader->CreatePixelShader(
		(char*)"SimpleModelPixel.hlsl",
		(char*)"pplmain");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = m_knotShader->CreatePixelShader(
		(char*)"SimpleModelPixel.hlsl",
		(char*)"main");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = m_kNotTexShader->CreatePixelShader(
		(char*)"SimpleModelPixel.hlsl",
		(char*)"pplsub");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = m_kNotTexnotShader->CreatePixelShader(
		(char*)"SimpleModelPixel.hlsl",
		(char*)"sub");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = m_kLineShader->CreatePixelShader(
		(char*)"SimpleModelPixel.hlsl",
		(char*)"Line");
	if (!sts)
	{
		MessageBox(nullptr, "PixelShader CreateFailed.", "Error", MB_OK);
		return false;
	}

	SetCurrentDirectory(m_default_current_directory);

	//Vertex Buffer
	sts = m_kShader->CreateVertexBuffer(
		sizeof(XFile::Vertex),
		m_kModel->GetVertexNum(),
		m_kModel->GetVertices(),
		&m_kDrawer->GetVertexBuffer());
	if (!sts)
	{
		MessageBox(nullptr, "VertexBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	//インデックスバッファ
	sts = m_kShader->CreateIndexBuffer(
		m_kModel->GetIndexNum(),
		m_kModel->GetIndex(),
		&m_kDrawer->GetIndexBuffer());
	if (!sts)
	{
		MessageBox(nullptr, "IndexBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = CShader::CreateConstantBuffer(sizeof(ConstantBuffer_W), &m_kWorld);
	if (!sts)
	{
		MessageBox(nullptr, "ConstantBuffer CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = CShader::CreateConstantBuffer(sizeof(ConstantBuffer_C), &m_kColor);
	if (!sts)
	{
		MessageBox(nullptr, "ConstantBufferColor CreateFailed.", "Error", MB_OK);
		return false;
	}

	sts = CShader::CreateConstantBuffer(sizeof(ConstantBuffer_L), &m_kLight);
	if (!sts)
	{
		MessageBox(nullptr, "ConstantBufferLight CreateFailed.", "Error", MB_OK);
		return false;
	}

	////ディレクトリ移動
	SetCurrentDirectory(m_default_current_directory);
	//SetCurrentDirectory("Assets/mesh/");
	SetCurrentDirectory("Assets/mesh/Maid/");

	for (int i = 0; i < m_kModel->GetMaterialNum();i++)
	{
		ID3D11ShaderResourceView* p_srv;
		if (m_kModel->GetDecal(i) != "")
		{
			sts = CShader::CreateSRVfromWICFile(m_kModel->GetDecal(i).c_str(), &p_srv);
			if (!sts)
			{
				sts = CShader::CreateSRVfromTGAFile(m_kModel->GetDecal(i).c_str(), &p_srv);
				if (!sts)
				{
					std::cout << m_kModel->GetDecal(i).c_str() << std::endl;
					MessageBox(nullptr, "Decal CreateFailed.", "Error", MB_OK);
					//return false;
				}
				else
				{
					m_kSRVList.push_back(p_srv);//リスト登録
				}
			}
			else
			{
				m_kSRVList.push_back(p_srv);//リスト登録
			}
		}
		else
		{
			p_srv = nullptr;
			m_kSRVList.push_back(p_srv);
		}
	}

	sts = CShader::CreateSRVfromWICFile("toonpaint.bmp", &m_kToon);
	if (!sts)
	{
		sts = CShader::CreateSRVfromTGAFile("toonpaint.bmp", &m_kToon);
		if (!sts)
		{
			std::cout << "toonpaint.bmp" << std::endl;
			MessageBox(nullptr, "Decal CreateFailed.", "Error", MB_OK);
			//return false;
		}
	}

	SetCurrentDirectory(m_default_current_directory);

	sts = CShader::CreateSamplerState(m_kSampler);
	if (!sts)
	{
		MessageBox(nullptr, "Sampler CreateFailed.", "Error", MB_OK);
		return false;
	}

	CDirectInput::GetInstance().Init(g_pWindow->GetHinstance(), g_pWindow->GetHWND(), g_pWindow->GetWindowWidth(), g_pWindow->GetWindowHeight());

	m_kCamera = new CCameraObject();

	m_kCamera->Init(
		0.1f,
		1000.0f,
		D3DX_PI / 4,
		g_pWindow->GetWindowWidth(),
		g_pWindow->GetWindowHeight(),
		D3DXVECTOR3(0, 5, -10),
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(0, 1, 0));
	sts = m_kShader->CreateConstantBuffer(sizeof(CCameraObject::ConstantBuffer), &m_kCamera->GetCameraBuffer());
	if (!sts)
	{
		MessageBox(nullptr, "Buffer CreateFailed.", "Error", MB_OK);
		return false;
	}
	m_kCamera->UpdateBuffer(GetImmediateContext());

	GetImmediateContext()->VSSetConstantBuffers(0, 1, &m_kCamera->GetCameraBuffer());
	GetImmediateContext()->PSSetConstantBuffers(0, 1, &m_kCamera->GetCameraBuffer());

	return true;
}

void LineDraw()
{
	DefaultBlendState();
	DepthStencilSwitch(true);

	CDirectGraphics::GetInstance().ChangeMode_Culling(D3D11_CULL_FRONT);

	ConstantBuffer_C cc;
	cc.ambient = c_kambient;
	cc.diffuse = c_kdiffuse;
	cc.emissive = c_kemissive;
	cc.specular = c_kspecular;
	cc.WaterHeight = D3DXVECTOR4(0, 0, 0, 0.0f);
	GetImmediateContext()->UpdateSubresource(m_kColor, 0, NULL, &cc, 0, 0);
	GetImmediateContext()->VSSetConstantBuffers(2, 1, &m_kColor);
	GetImmediateContext()->PSSetConstantBuffers(2, 1, &m_kColor);

	ConstantBuffer_L cl;
	cl.Light = c_klight;
	GetImmediateContext()->UpdateSubresource(m_kLight, 0, NULL, &cl, 0, 0);
	GetImmediateContext()->VSSetConstantBuffers(3, 1, &m_kLight);
	GetImmediateContext()->PSSetConstantBuffers(3, 1, &m_kLight);

	//CDirectInput::GetInstance().GetKeyBuffer();

	m_kCamera->Update(GetImmediateContext());

	static bool change = false;
	static bool flag = false;
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
	{
		change = !change;
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
	{
		flag = !flag;
	}

	//if (change)
	//{
	//	CDirectGraphics::GetInstance().Mode_WireFrame();
	//}
	//else
	//{
	//	CDirectGraphics::GetInstance().Mode_Solid();
	//}

	static D3DXVECTOR3 vector = D3DXVECTOR3(0, 0, 0);
	const float mover = 0.1f;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP))
	{
		vector.x += mover;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN))
	{
		vector.x -= mover;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT))
	{
		vector.y -= mover;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT))
	{
		vector.y += mover;
	}

	D3DXMATRIX world, rot;
	ConstantBuffer_W cw;
	::ZeroMemory(&cw, sizeof(ConstantBuffer_W));
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixScaling(&world, c_kscale, c_kscale, c_kscale);
	D3DXMatrixRotationYawPitchRoll(&rot, timeGetTime() / c_kturn, timeGetTime() / c_kturn, timeGetTime() / c_kturn);
	//D3DXMatrixRotationYawPitchRoll(&rot, D3DXToRadian(vector.x), D3DXToRadian(vector.y), D3DXToRadian(vector.z));
	world = world * rot;
	//world = rot;
	D3DXMatrixTranspose(&cw.world, &world);
	cw.diffuse = mat_kdiffuse;
	cw.ambient = mat_kambient;
	cw.specular = mat_kspecular;
	cw.emissive = mat_kemissive;
	GetImmediateContext()->UpdateSubresource(m_kWorld, 0, NULL, &cw, 0, 0);

	GetImmediateContext()->VSSetShader(m_kLineShader->GetVertexShader(), NULL, 0);
	GetImmediateContext()->PSSetShader(m_kLineShader->GetPixelShader(), NULL, 0);

	GetImmediateContext()->VSSetConstantBuffers(1, 1, &m_kWorld);
	GetImmediateContext()->PSSetConstantBuffers(1, 1, &m_kWorld);

	UINT stride = sizeof(XFile::Vertex);
	UINT offset = 0;

	GetImmediateContext()->IASetVertexBuffers(0, 1, &m_kDrawer->GetVertexBuffer(), &stride, &offset);
	GetImmediateContext()->IASetInputLayout(m_kShader->GetVertexLayout());

	//GetImmediateContext()->PSSetShaderResources(0, 1, &m_kSRV);
	GetImmediateContext()->PSSetSamplers(0, 1, &m_kSampler);
	GetImmediateContext()->IASetIndexBuffer(m_kDrawer->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	auto obj = m_kModel->GetIndexNum();

	GetImmediateContext()->DrawIndexed(obj, 0, 0);
}

void KadaiDraw()
{

	LineDraw();

	DefaultBlendState();
	DepthStencilSwitch(true);

	CDirectGraphics::GetInstance().ChangeMode_Culling(D3D11_CULL_BACK);

	ConstantBuffer_C cc;
	//cc.ambient = c_kambient;
	cc.diffuse = c_kdiffuse;
	//cc.emissive = c_kemissive;
	cc.specular = c_kspecular;
	cc.WaterHeight = D3DXVECTOR4(0,0,0, 0.0f);
	//GetImmediateContext()->UpdateSubresource(m_kColor, 0, NULL, &cc, 0, 0);
	//GetImmediateContext()->VSSetConstantBuffers(2, 1, &m_kColor);
	//GetImmediateContext()->PSSetConstantBuffers(2, 1, &m_kColor);

	ConstantBuffer_L cl;
	cl.Light = c_klight;
	GetImmediateContext()->UpdateSubresource(m_kLight, 0, NULL, &cl, 0, 0);
	GetImmediateContext()->VSSetConstantBuffers(3, 1, &m_kLight);
	GetImmediateContext()->PSSetConstantBuffers(3, 1, &m_kLight);

	CDirectInput::GetInstance().GetKeyBuffer();

	m_kCamera->Update(GetImmediateContext());

	static bool change = false;
	static bool flag = false;
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
	{
		change = !change;
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
	{
		flag = !flag;
	}

	//if (change)
	//{
	//	CDirectGraphics::GetInstance().Mode_WireFrame();
	//}
	//else
	//{
	//	CDirectGraphics::GetInstance().Mode_Solid();
	//}

	static D3DXVECTOR3 vector = D3DXVECTOR3(0,0,0);
	const float mover = 0.1f;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP))
	{
		vector.x += mover;
	}
	if(CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN))
	{
		vector.x -= mover;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT))
	{
		vector.y -= mover;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT))
	{
		vector.y +=mover;
	}

	D3DXMATRIX world, rot;
	ConstantBuffer_W cw;
	::ZeroMemory(&cw, sizeof(ConstantBuffer_W));
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rot);
	D3DXMatrixScaling(&world, c_kscale, c_kscale, c_kscale);
	D3DXMatrixRotationYawPitchRoll(&rot,timeGetTime() / c_kturn, timeGetTime() / c_kturn, timeGetTime() / c_kturn);
	//D3DXMatrixRotationYawPitchRoll(&rot, D3DXToRadian(vector.x), D3DXToRadian(vector.y), D3DXToRadian(vector.z));
	world = world * rot;
	//world = rot;
	D3DXMatrixTranspose(&cw.world, &world);
	cw.diffuse = mat_kdiffuse;
	cw.ambient = mat_kambient;
	cw.specular = mat_kspecular;
	cw.emissive = mat_kemissive;
	GetImmediateContext()->UpdateSubresource(m_kWorld, 0, NULL, &cw, 0, 0);

	if (!flag)
	{
		GetImmediateContext()->VSSetShader(m_kShader->GetVertexShader(), NULL, 0);
		//GetImmediateContext()->VSSetShader(m_vs, NULL, 0);
		//GetImmediateContext()->PSSetShader(m_kShader->GetPixelShader(), NULL, 0);
	}
	else
	{
		GetImmediateContext()->VSSetShader(m_knotShader->GetVertexShader(), NULL, 0);
		//GetImmediateContext()->VSSetShader(m_vs, NULL, 0);
		//GetImmediateContext()->PSSetShader(m_knotShader->GetPixelShader(), NULL, 0);
	}

	GetImmediateContext()->VSSetConstantBuffers(1, 1, &m_kWorld);
	GetImmediateContext()->PSSetConstantBuffers(1, 1, &m_kWorld);

	UINT stride = sizeof(XFile::Vertex);
	UINT offset = 0;

	GetImmediateContext()->IASetVertexBuffers(0, 1, &m_kDrawer->GetVertexBuffer(), &stride, &offset);
	
	
	//GetImmediateContext()->IASetInputLayout(m_kShader->GetVertexLayout());
	GetImmediateContext()->IASetInputLayout(m_il);

	
	
	//GetImmediateContext()->PSSetShaderResources(0, 1, &m_kSRV);
	GetImmediateContext()->PSSetSamplers(0, 1, &m_kSampler);
	GetImmediateContext()->IASetIndexBuffer(m_kDrawer->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GetImmediateContext()->PSSetShaderResources(1, 1, &m_kToon);

	for (UINT i = 0; i < m_kModel->GetMaterialNum(); i++)
	{
		int subsetindex = -1;
		unsigned int num = m_kModel->GetMaterialNum();

		GetImmediateContext()->PSSetShaderResources(0, 1, &m_kSRVList[i]);
		if (m_kSRVList[i] != nullptr)
		{
			if (!change)
			{
				GetImmediateContext()->PSSetShader(m_kShader->GetPixelShader(), NULL, 0);
			}
			else
			{
				GetImmediateContext()->PSSetShader(m_knotShader->GetPixelShader(), NULL, 0);
			}
		}
		else
		{
			if (!change)
			{
				GetImmediateContext()->PSSetShader(m_kNotTexShader->GetPixelShader(), NULL, 0);
			}
			else
			{
				GetImmediateContext()->PSSetShader(m_kNotTexnotShader->GetPixelShader(), NULL, 0);
			}
		}

		for (UINT j = 0; j < m_kModel->GetSubsetNum(); j++)
		{
			if (i == m_kModel->GetSubset(j).m_MaterialIndex)
			{
				cc.ambient = D3DXVECTOR4(m_kModel->GetMaterial(i).m_Ambient,1.0f);

				cc.diffuse = D3DXVECTOR4(m_kModel->GetMaterial(i).m_Diffuse,1.0f);

				cc.specular = D3DXVECTOR4(m_kModel->GetMaterial(i).m_Specular, 1.0f);

				cc.emissive = D3DXVECTOR4(m_kModel->GetMaterial(i).m_Emissive, 1.0f);


				//定数バッファ更新
				GetImmediateContext()->UpdateSubresource(
					m_kColor,
					0,					//コピー先サブリソース
					NULL,				//サブリソースを定義するボックス
					&cc,	//コピー元
					0,					//１行のサイズ
					0);					//１深度スライスのサイズ

										//定数バッファを頂点シェーダーへセット
				GetImmediateContext()->VSSetConstantBuffers(
					2,//スタートスロット
					1,//個数
					&m_kColor);
				//定数バッファをピクセルシェーダーへセット
				GetImmediateContext()->PSSetConstantBuffers(
					2,//スタートスロット
					1,//個数
					&m_kColor);

				subsetindex = j;

				auto obj = m_kModel->GetSubset(subsetindex).m_VertexNum;
				auto work = m_kModel->GetSubset(subsetindex).m_FaceStart * 3;

				/*GetImmediateContext()->DrawIndexed(
				m_xfile->GetSubset(subsetidx).m_VertexNum,//描画するインデックス数(面数*3)
				m_xfile->GetSubset(subsetidx).m_FaceStart * 3,//最初のインデックスバッファの位置
				0);//頂点バッファの最初から使用*/

				GetImmediateContext()->DrawIndexed(
					obj,//描画するインデックス数(面数*3)
					work,//最初のインデックスバッファの位置
					0);
			}
		}

	}

	//auto obj = m_kModel->GetIndexNum();

	//GetImmediateContext()->DrawIndexed(obj, 0, 0);
}