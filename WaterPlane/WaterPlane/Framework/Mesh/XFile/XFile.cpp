//#include "../Direct9/CDirectxGraphics.h"
#include "XFile.h"
#include "../../../System/Utility/Exception.h"
#include "../../../System/Utility/Memory.h"
#include <iostream>

//Sphere
CXFile::CXFile(LPDIRECT3DDEVICE9 device)
	: m_VertexNum(0), m_Vertex(nullptr),
	m_IndexNum(0), m_Index(nullptr),
	m_MaterialNum(0), m_Material(nullptr), m_DecalTex(nullptr),
	m_SubsetNum(0), m_Subset(nullptr)
{
	HRESULT hr;
	LPD3DXMESH mesh = nullptr;
	LPD3DXMESH meshOpt = nullptr;
	LPD3DXBUFFER material = nullptr;
	DWORD materialNum = 0;
	DWORD subsetNum = 0;

	//インデックスを32bitにコンバート
	hr = D3DXCreateSphere(device, 0.1f, 10, 10, &mesh, NULL);
	if (FAILED(hr))
	{
		//エラー処理を投げる
		throw Exception(ErrorCode::Fatal,
			"XFile::new() D3DXCreateSphere is failed.");
	}

	//CalculateBoundingSphere(mesh, device);

	////並びを最適化、属性テーブル（サブセット）を作成する
	//mesh->Optimize(D3DXMESHOPT_ATTRSORT,
	//	NULL, NULL, NULL, NULL, &meshOpt);
	//mesh->Release();

	//属性テーブルの取得
	//メッシュの属性テーブルに格納されているエントリ数を取得
	mesh->GetAttributeTable(NULL, &subsetNum);
	D3DXATTRIBUTERANGE* subset = new D3DXATTRIBUTERANGE[subsetNum];
	mesh->GetAttributeTable(subset, &subsetNum);

	//指定子したFVFでメッシュを作り直す
	hr = mesh->CloneMeshFVF(
		mesh->GetOptions(),
		XFile::c_VertexFVF,
		device,
		&meshOpt);
	if (FAILED(hr))
	{
		throw Exception(ErrorCode::Fatal,
			"XFile::new() CloneMeshFVF is failed.");
	}
	mesh->Release();
	D3DXComputeNormals(meshOpt, NULL);//法線の計算

								   //マテリアル数とサブセット数は一致するとは限らないので注意
	m_VertexNum = meshOpt->GetNumVertices();
	m_FaceNum = meshOpt->GetNumFaces();
	m_IndexNum = meshOpt->GetNumFaces() * 3;
	m_MaterialNum = materialNum;
	m_SubsetNum = subsetNum;

	m_Vertex = new XFile::Vertex[m_VertexNum];
	m_Index = new XFile::Index[m_IndexNum];
	m_Material = new XFile::Material[m_MaterialNum];
	m_DecalTex = new std::string[m_MaterialNum];
	m_Subset = new XFile::Subset[m_SubsetNum];


	//頂点データの取得
	XFile::Vertex* srcVertex = nullptr;
	LPDIRECT3DVERTEXBUFFER9 vb = nullptr;
	meshOpt->GetVertexBuffer(&vb);
	vb->Lock(0, 0, (void**)&srcVertex, 0);
	memcpy_s(m_Vertex, sizeof(XFile::Vertex) * m_VertexNum, srcVertex, sizeof(XFile::Vertex) * m_VertexNum);
	//memcpy(m_Vertex, srcVertex, sizeof(XFile::Vertex) * m_VertexNum);
	vb->Unlock();
	vb->Release();

	//インデックスの取得
	XFile::Index* srcIndex = nullptr;
	LPDIRECT3DINDEXBUFFER9 ib = nullptr;
	meshOpt->GetIndexBuffer(&ib);
	ib->Lock(0, 0, (void**)&srcIndex, 0);
	memcpy_s(m_Index, sizeof(XFile::Index)* m_IndexNum, srcIndex, sizeof(XFile::Index) * m_IndexNum);
	//memcpy(m_Index, srcIndex, sizeof(XFile::Index) * m_IndexNum);
	//結線情報の取得
	m_connect = new XFile::Connect[m_FaceNum];
	for (unsigned int count = 0; count < m_FaceNum; count++)
	{
		m_connect[count].index[0] = *srcIndex++;
		m_connect[count].index[1] = *srcIndex++;
		m_connect[count].index[2] = *srcIndex++;
	}
	ib->Unlock();
	ib->Release();

	meshOpt->Release();
}

CXFile::CXFile(const char* name, LPDIRECT3DDEVICE9 device)
	:	m_VertexNum(0),		m_Vertex(nullptr),
		m_IndexNum(0),		m_Index(nullptr),
		m_MaterialNum(0),	m_Material(nullptr),	m_DecalTex(nullptr),
		m_SubsetNum(0),		m_Subset(nullptr)
{

	//D3DXMESHから必要なデータを取り出す

	HRESULT hr;
	LPD3DXMESH mesh = nullptr;
	LPD3DXMESH meshOpt = nullptr;
	LPD3DXBUFFER material = nullptr;
	DWORD materialNum = 0;
	DWORD subsetNum = 0;

	//インデックスを32bitにコンバート
	hr = D3DXLoadMeshFromX(
		name,
		//D3DXMESH_MANAGED | D3DXMESH_32BIT,
		D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
		device,
		NULL,
		&material,
		NULL,
		&materialNum,
		&mesh);

	if (FAILED(hr))
	{
		//エラー処理を投げる
		throw Exception(ErrorCode::Fatal,
			"XFile::new() D3DXLoadMeshFromX() is failed.");
	}
	
	//CalculateBoundingSphere(mesh, device);

	//並びを最適化、属性テーブル（サブセット）を作成する
	mesh->Optimize(D3DXMESHOPT_ATTRSORT,
		NULL, NULL, NULL, NULL, &meshOpt);
	mesh->Release();

	//属性テーブルの取得
	//メッシュの属性テーブルに格納されているエントリ数を取得
	meshOpt->GetAttributeTable(NULL, &subsetNum);
	D3DXATTRIBUTERANGE* subset = new D3DXATTRIBUTERANGE[subsetNum];
	meshOpt->GetAttributeTable(subset, &subsetNum);

	//指定子したFVFでメッシュを作り直す
	hr = meshOpt->CloneMeshFVF(
		meshOpt->GetOptions(),
		XFile::c_VertexFVF,
		device,
		&mesh);
	if (FAILED(hr))
	{
		throw Exception(ErrorCode::Fatal,
			"XFile::new() CloneMeshFVF is failed.");
	}
	meshOpt->Release();
	hr = D3DXComputeNormals(mesh, NULL);//法線の計算

	//マテリアル数とサブセット数は一致するとは限らないので注意
	m_VertexNum = mesh->GetNumVertices();
	m_FaceNum = mesh->GetNumFaces();
	m_IndexNum = mesh->GetNumFaces() * 3;
	m_MaterialNum = materialNum;
	m_SubsetNum = subsetNum;

	m_Vertex = new XFile::Vertex[m_VertexNum];
	m_Index = new XFile::Index[m_IndexNum];
	m_Material = new XFile::Material[m_MaterialNum];
	m_DecalTex = new std::string[m_MaterialNum];
	m_Subset = new XFile::Subset[m_SubsetNum];


	//頂点データの取得
	XFile::Vertex* srcVertex = nullptr;
	LPDIRECT3DVERTEXBUFFER9 vb = nullptr;
	mesh->GetVertexBuffer(&vb);
	vb->Lock(0, 0, (void**)&srcVertex, 0);
	memcpy_s(m_Vertex, sizeof(XFile::Vertex) * m_VertexNum, srcVertex, sizeof(XFile::Vertex) * m_VertexNum);
	for (unsigned int i = 10 ; i < m_VertexNum; i++)
	{
		std::cout << m_Vertex[i].m_Pos.x << " " << m_Vertex[i].m_Pos.y << " " << m_Vertex[i].m_Pos.z << std::endl;
		std::cout << m_Vertex[i].m_Normal.x << " " << m_Vertex[i].m_Normal.y << " " << m_Vertex[i].m_Normal.z << std::endl;
		std::cout << m_Vertex[i].m_tex.x << " " << m_Vertex[i].m_tex.y << std::endl;
		break;
	}
	//memcpy(m_Vertex, srcVertex, sizeof(XFile::Vertex) * m_VertexNum);
	vb->Unlock();
	vb->Release();

	//インデックスの取得
	XFile::Index* srcIndex = nullptr;
	LPDIRECT3DINDEXBUFFER9 ib = nullptr;
	mesh->GetIndexBuffer(&ib);
	ib->Lock(0, 0, (void**)&srcIndex, 0);
	memcpy_s(m_Index, sizeof(XFile::Index)* m_IndexNum, srcIndex, sizeof(XFile::Index) * m_IndexNum);
	//memcpy(m_Index, srcIndex, sizeof(XFile::Index) * m_IndexNum);
	//結線情報の取得
	/*m_connect = new XFile::Connect[m_FaceNum];
	for (int count = 0; count < m_FaceNum; count++)
	{
		m_connect[count].index[0] = *srcIndex++;
		m_connect[count].index[1] = *srcIndex++;
		m_connect[count].index[2] = *srcIndex++;
	}*/
	ib->Unlock();
	ib->Release();

	//マテリアルとテクスチャ名の取得
	D3DXMATERIAL* dxMat = (D3DXMATERIAL*)material->GetBufferPointer();

	for (unsigned int i = 0; i < m_MaterialNum; i++)
	{
		auto& dist = m_Material[i];
		auto& src = dxMat[i].MatD3D;

		dist.m_Alpha = src.Diffuse.a;
		dist.m_Diffuse.x = src.Diffuse.r;
		dist.m_Diffuse.y = src.Diffuse.g;
		dist.m_Diffuse.z = src.Diffuse.b;

		dist.m_Specular.x = src.Specular.r;
		dist.m_Specular.y = src.Specular.g;
		dist.m_Specular.z = src.Specular.b;

		dist.m_SpecularPower = src.Power;

		dist.m_Ambient.x = src.Ambient.r;
		dist.m_Ambient.y = src.Ambient.g;
		dist.m_Ambient.z = src.Ambient.b;

		dist.m_Emissive.x = src.Emissive.r;
		dist.m_Emissive.y = src.Emissive.g;
		dist.m_Emissive.z = src.Emissive.b;

		//nullになっていることがある
		//nullなら空文字列を入れる
		if (dxMat[i].pTextureFilename)
		{
			m_DecalTex[i] = dxMat[i].pTextureFilename;
		}
		else
		{
			m_DecalTex[i] = "";
		}
	}
	material->Release();


	//サブセットの取得
	//インデックスバッファの先頭から順に参照しているはず
	//FaceStartを調べればわかる
	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		auto& dest = m_Subset[i];
		auto& src = subset[i];

		//0番目のサブセットが0番目のマテリアルを参照するとは限らない
		//多分、属性IDがマテリアルテーブルのインデックス
		dest.m_MaterialIndex = src.AttribId;
		dest.m_FaceStart = src.FaceStart;//開始面
		dest.m_FaceCount = src.FaceCount;//面数
		dest.m_VertexStart = src.VertexStart;//開始頂点
		dest.m_VertexCount = src.VertexCount;//頂点の数

		//VertexCountはFaceCount*3ではないので注意
		dest.m_VertexNum = src.FaceCount * 3;
	}
	DeleteArray(subset);

	mesh->Release();

	//読み込み回数をログで確認するためのデバッグ表示
	std::cout << name << "Init" << std::endl;
}

CXFile::~CXFile()
{
	DeleteArray(m_pTriangle);

	DeleteArray(m_Subset);
	DeleteArray(m_DecalTex);
	DeleteArray(m_Material);
	DeleteArray(m_Index);
	DeleteArray(m_Vertex);

	return;
}