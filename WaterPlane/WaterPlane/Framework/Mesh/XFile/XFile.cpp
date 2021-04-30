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

	//�C���f�b�N�X��32bit�ɃR���o�[�g
	hr = D3DXCreateSphere(device, 0.1f, 10, 10, &mesh, NULL);
	if (FAILED(hr))
	{
		//�G���[�����𓊂���
		throw Exception(ErrorCode::Fatal,
			"XFile::new() D3DXCreateSphere is failed.");
	}

	//CalculateBoundingSphere(mesh, device);

	////���т��œK���A�����e�[�u���i�T�u�Z�b�g�j���쐬����
	//mesh->Optimize(D3DXMESHOPT_ATTRSORT,
	//	NULL, NULL, NULL, NULL, &meshOpt);
	//mesh->Release();

	//�����e�[�u���̎擾
	//���b�V���̑����e�[�u���Ɋi�[����Ă���G���g�������擾
	mesh->GetAttributeTable(NULL, &subsetNum);
	D3DXATTRIBUTERANGE* subset = new D3DXATTRIBUTERANGE[subsetNum];
	mesh->GetAttributeTable(subset, &subsetNum);

	//�w��q����FVF�Ń��b�V������蒼��
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
	D3DXComputeNormals(meshOpt, NULL);//�@���̌v�Z

								   //�}�e���A�����ƃT�u�Z�b�g���͈�v����Ƃ͌���Ȃ��̂Œ���
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


	//���_�f�[�^�̎擾
	XFile::Vertex* srcVertex = nullptr;
	LPDIRECT3DVERTEXBUFFER9 vb = nullptr;
	meshOpt->GetVertexBuffer(&vb);
	vb->Lock(0, 0, (void**)&srcVertex, 0);
	memcpy_s(m_Vertex, sizeof(XFile::Vertex) * m_VertexNum, srcVertex, sizeof(XFile::Vertex) * m_VertexNum);
	//memcpy(m_Vertex, srcVertex, sizeof(XFile::Vertex) * m_VertexNum);
	vb->Unlock();
	vb->Release();

	//�C���f�b�N�X�̎擾
	XFile::Index* srcIndex = nullptr;
	LPDIRECT3DINDEXBUFFER9 ib = nullptr;
	meshOpt->GetIndexBuffer(&ib);
	ib->Lock(0, 0, (void**)&srcIndex, 0);
	memcpy_s(m_Index, sizeof(XFile::Index)* m_IndexNum, srcIndex, sizeof(XFile::Index) * m_IndexNum);
	//memcpy(m_Index, srcIndex, sizeof(XFile::Index) * m_IndexNum);
	//�������̎擾
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

	//D3DXMESH����K�v�ȃf�[�^�����o��

	HRESULT hr;
	LPD3DXMESH mesh = nullptr;
	LPD3DXMESH meshOpt = nullptr;
	LPD3DXBUFFER material = nullptr;
	DWORD materialNum = 0;
	DWORD subsetNum = 0;

	//�C���f�b�N�X��32bit�ɃR���o�[�g
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
		//�G���[�����𓊂���
		throw Exception(ErrorCode::Fatal,
			"XFile::new() D3DXLoadMeshFromX() is failed.");
	}
	
	//CalculateBoundingSphere(mesh, device);

	//���т��œK���A�����e�[�u���i�T�u�Z�b�g�j���쐬����
	mesh->Optimize(D3DXMESHOPT_ATTRSORT,
		NULL, NULL, NULL, NULL, &meshOpt);
	mesh->Release();

	//�����e�[�u���̎擾
	//���b�V���̑����e�[�u���Ɋi�[����Ă���G���g�������擾
	meshOpt->GetAttributeTable(NULL, &subsetNum);
	D3DXATTRIBUTERANGE* subset = new D3DXATTRIBUTERANGE[subsetNum];
	meshOpt->GetAttributeTable(subset, &subsetNum);

	//�w��q����FVF�Ń��b�V������蒼��
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
	hr = D3DXComputeNormals(mesh, NULL);//�@���̌v�Z

	//�}�e���A�����ƃT�u�Z�b�g���͈�v����Ƃ͌���Ȃ��̂Œ���
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


	//���_�f�[�^�̎擾
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

	//�C���f�b�N�X�̎擾
	XFile::Index* srcIndex = nullptr;
	LPDIRECT3DINDEXBUFFER9 ib = nullptr;
	mesh->GetIndexBuffer(&ib);
	ib->Lock(0, 0, (void**)&srcIndex, 0);
	memcpy_s(m_Index, sizeof(XFile::Index)* m_IndexNum, srcIndex, sizeof(XFile::Index) * m_IndexNum);
	//memcpy(m_Index, srcIndex, sizeof(XFile::Index) * m_IndexNum);
	//�������̎擾
	/*m_connect = new XFile::Connect[m_FaceNum];
	for (int count = 0; count < m_FaceNum; count++)
	{
		m_connect[count].index[0] = *srcIndex++;
		m_connect[count].index[1] = *srcIndex++;
		m_connect[count].index[2] = *srcIndex++;
	}*/
	ib->Unlock();
	ib->Release();

	//�}�e���A���ƃe�N�X�`�����̎擾
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

		//null�ɂȂ��Ă��邱�Ƃ�����
		//null�Ȃ�󕶎��������
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


	//�T�u�Z�b�g�̎擾
	//�C���f�b�N�X�o�b�t�@�̐擪���珇�ɎQ�Ƃ��Ă���͂�
	//FaceStart�𒲂ׂ�΂킩��
	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		auto& dest = m_Subset[i];
		auto& src = subset[i];

		//0�Ԗڂ̃T�u�Z�b�g��0�Ԗڂ̃}�e���A�����Q�Ƃ���Ƃ͌���Ȃ�
		//�����A����ID���}�e���A���e�[�u���̃C���f�b�N�X
		dest.m_MaterialIndex = src.AttribId;
		dest.m_FaceStart = src.FaceStart;//�J�n��
		dest.m_FaceCount = src.FaceCount;//�ʐ�
		dest.m_VertexStart = src.VertexStart;//�J�n���_
		dest.m_VertexCount = src.VertexCount;//���_�̐�

		//VertexCount��FaceCount*3�ł͂Ȃ��̂Œ���
		dest.m_VertexNum = src.FaceCount * 3;
	}
	DeleteArray(subset);

	mesh->Release();

	//�ǂݍ��݉񐔂����O�Ŋm�F���邽�߂̃f�o�b�O�\��
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