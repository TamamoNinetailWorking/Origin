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

	typedef uint32_t VertexNum;//���_��
	struct Vertex
	{
		D3DXVECTOR3 m_Pos;//�ʒu
		D3DXVECTOR3 m_Normal;//�@��
		D3DXVECTOR2 m_tex;//�e�N�X�`�����W
	};

	typedef uint32_t FaceNum;//�ʂ̐�

	typedef uint32_t IndexNum;//�C���f�b�N�X�̐�
	typedef uint32_t Index;//�C���f�b�N�X

	typedef uint32_t MaterialNum;//�}�e���A���̐�
	struct Material
	{
		float m_Alpha;//�����x
		D3DXVECTOR3 m_Diffuse;//�g�U���ˌ�
		D3DXVECTOR3 m_Specular;//���ʔ��ˌ�
		float m_SpecularPower;//���ʔ��˂̋���
		D3DXVECTOR3 m_Ambient;//����
		D3DXVECTOR3 m_Emissive;//���ˌ�

		Material()//�R���X�g���N�^
			:m_Alpha(0),
			m_Diffuse(D3DXVECTOR3(0,0,0)),
			m_Specular(D3DXVECTOR3(0,0,0)),
			m_SpecularPower(0),
			m_Ambient(D3DXVECTOR3(0,0,0)),
			m_Emissive(D3DXVECTOR3(0,0,0))
		{}
	};

	typedef uint32_t SubsetNum;//�T�u�Z�b�g�̐�
	struct Subset
	{
		//�}�e���A���e�[�u���̃C���f�b�N�X�B�g��Ȃ��ꍇ��-1
		int32_t m_MaterialIndex;//�}�e���A���̃C���f�b�N�X
		int32_t m_VertexNum;//���_��
		DWORD m_FaceStart;//�J�n��
		DWORD m_FaceCount;//�ʐ�
		DWORD m_VertexStart;//�J�n���_
		DWORD m_VertexCount;//���_�̐�

		Subset()
			:m_MaterialIndex(-1),
			m_VertexNum(0)
		{}
	};

	//�������
	struct Connect
	{
		WORD index[3];
	};

	//�΂�΂�ɂ����O�p�`�̍\����
	struct Triangle
	{
		XFile::Vertex vertex[3];//�O�p�`�̒��_
		D3DCOLOR color[3];//�e���_�̐F

		D3DXVECTOR3 angle;//��]�p�x
		D3DXVECTOR3 derivative_angle;//��]�p�x����
		D3DXVECTOR3 center_position;//�d�S���W
		D3DXVECTOR3 normal;//�@���x�N�g��

		D3DXMATRIX matrix;//�ϊ��s��
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

	//�󂯓n�����_�f�[�^
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
	std::string* m_DecalTex;//�}�e���A���z��ƑΉ�

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