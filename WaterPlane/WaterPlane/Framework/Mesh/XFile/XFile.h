#pragma once

//include
#include <stdint.h>
#include <string>
#include <d3dx9.h>
#include <random>
#include "../../../System/Utility/NonCopyable.h"
//#include "../OBB/OBBCollision.h"

namespace XFile 
{

	const DWORD c_VertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;	
	const DWORD c_PrimitiveVertexFVF = D3DFVF_XYZ | D3DFVF_NORMAL;

	typedef uint32_t VertexNum;//頂点数
	struct Vertex
	{
		D3DXVECTOR3 m_Pos;//位置
		D3DXVECTOR3 m_Normal;//法線
		D3DXVECTOR2 m_tex;//テクスチャ座標
	};

	typedef uint32_t FaceNum;//面の数

	typedef uint32_t IndexNum;//インデックスの数
	typedef uint32_t Index;//インデックス

	typedef uint32_t MaterialNum;//マテリアルの数
	struct Material
	{
		float m_Alpha;//透明度
		D3DXVECTOR3 m_Diffuse;//拡散反射光
		D3DXVECTOR3 m_Specular;//鏡面反射光
		float m_SpecularPower;//鏡面反射の強さ
		D3DXVECTOR3 m_Ambient;//環境光
		D3DXVECTOR3 m_Emissive;//放射光

		Material()//コンストラクタ
			:m_Alpha(0),
			m_Diffuse(D3DXVECTOR3(0,0,0)),
			m_Specular(D3DXVECTOR3(0,0,0)),
			m_SpecularPower(0),
			m_Ambient(D3DXVECTOR3(0,0,0)),
			m_Emissive(D3DXVECTOR3(0,0,0))
		{}
	};

	typedef uint32_t SubsetNum;//サブセットの数
	struct Subset
	{
		//マテリアルテーブルのインデックス。使わない場合は-1
		int32_t m_MaterialIndex;//マテリアルのインデックス
		int32_t m_VertexNum;//頂点数
		DWORD m_FaceStart;//開始面
		DWORD m_FaceCount;//面数
		DWORD m_VertexStart;//開始頂点
		DWORD m_VertexCount;//頂点の数

		Subset()
			:m_MaterialIndex(-1),
			m_VertexNum(0)
		{}
	};

	//結線情報
	struct Connect
	{
		WORD index[3];
	};

	//ばらばらにした三角形の構造体
	struct Triangle
	{
		XFile::Vertex vertex[3];//三角形の頂点
		D3DCOLOR color[3];//各頂点の色

		D3DXVECTOR3 angle;//回転角度
		D3DXVECTOR3 derivative_angle;//回転角度増分
		D3DXVECTOR3 center_position;//重心座標
		D3DXVECTOR3 normal;//法線ベクトル

		D3DXMATRIX matrix;//変換行列
		Triangle()
		{
			std::random_device rnd;
			std::mt19937 mt(rnd());
			std::uniform_int_distribution<> rand(0, RAND_MAX);
			angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			derivative_angle = D3DXVECTOR3(
				((float)rand(mt) / (float)RAND_MAX),
				((float)rand(mt) / (float)RAND_MAX),
				((float)rand(mt) / (float)RAND_MAX));

			D3DXMatrixIdentity(&matrix);
		}
	};

	//受け渡す頂点データ
	struct TriangleVertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR4 Color;
		D3DXVECTOR2 Tex;
	};

	struct TrianglePlane
	{
		TriangleVertex vertex[3];
	};
}


class CXFile :private NonCopyable
{
private:

	XFile::VertexNum m_VertexNum;
	XFile::Vertex* m_Vertex;

	XFile::FaceNum m_FaceNum;
	
	XFile::IndexNum m_IndexNum;
	XFile::Index* m_Index;

	XFile::MaterialNum m_MaterialNum;
	XFile::Material* m_Material;
	std::string* m_DecalTex;//マテリアル配列と対応

	XFile::SubsetNum m_SubsetNum;
	XFile::Subset* m_Subset;

	XFile::Connect* m_connect;


	XFile::Triangle* m_pTriangle;
	XFile::TrianglePlane* m_pTrianglePlane;

	//HITDATA m_hitdata;

	//void CalculateBoundingSphere(const LPD3DXMESH mesh, const LPDIRECT3DDEVICE9 device);

public:

	explicit CXFile(const char* name,LPDIRECT3DDEVICE9 device);
	CXFile(LPDIRECT3DDEVICE9 device);
	~CXFile();

	//void ExplosionInit();
	void TraiangleTransforms(const D3DXMATRIX& mat);
	void ExplosionUpdate();

	XFile::VertexNum GetVertexNum() { return m_VertexNum; };
	XFile::Vertex* GetVertices() { return m_Vertex; };
	XFile::Vertex& GetVertex(int index) { return m_Vertex[index]; };
	
	XFile::FaceNum GetFaceNum() { return m_FaceNum; };

	XFile::IndexNum GetIndexNum() { return m_IndexNum; };
	XFile::Index* GetIndex() { return m_Index; };
	
	XFile::MaterialNum GetMaterialNum() { return m_MaterialNum; };
	XFile::Material* GetMaterial() { return m_Material; };
	XFile::Material& GetMaterial(int index) { return m_Material[index]; };
	std::string* GetDecal() { return m_DecalTex; };
	std::string& GetDecal(int index) { return m_DecalTex[index]; };

	XFile::SubsetNum GetSubsetNum() { return m_SubsetNum; };
	XFile::Subset* GetSubset() { return m_Subset; };
	XFile::Subset& GetSubset(int index) { return m_Subset[index]; };

	XFile::Triangle* GetTriangle() { return m_pTriangle; };
	XFile::Triangle& GetTriangle(int index) { return m_pTriangle[index]; };
	XFile::TrianglePlane* GetTrianglePlane() { return m_pTrianglePlane; };
	XFile::TriangleVertex* GetTrianglePlane(int index) { return m_pTrianglePlane[index].vertex; };

	//HITDATA& GetHitData() { return m_hitdata; };
	//float& GetRadius() { return m_hitdata.r; };
};