#include "Shader.h"
#include "../DirectXGraphics/CDirectGraphics.h"

bool CShader::CreateConstantBuffer(unsigned int ByteSize,ID3D11Buffer** ppConstantBuffer)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;//�o�b�t�@�g�p���@
	bd.ByteWidth = ByteSize;//�o�b�t�@�̑傫��
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�R���X�^���g�o�b�t�@
	bd.CPUAccessFlags = 0;//CPU�A�N�Z�X�s�v

	HRESULT hr = GetDevice11()->CreateBuffer(&bd, nullptr, ppConstantBuffer);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}

bool CShader::CreateIndexBuffer(unsigned int IndexNum, void* IndexData, ID3D11Buffer** ppIndexBuffer)
{

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * IndexNum;//�o�b�t�@�傫��
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;//�C���f�b�N�X�o�b�t�@
	bd.CPUAccessFlags = 0;//CPU�A�N�Z�X�s�v

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = IndexData;

	HRESULT hr = GetDevice11()->CreateBuffer(&bd, &InitData, ppIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}


bool CShader::CreateVertexBuffer(unsigned int stride, unsigned int VertexNum, void* pVertex, ID3D11Buffer** ppVertexBuffer)
{

	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = stride * VertexNum;//�o�b�t�@�̑傫��
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�o�b�t�@
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = pVertex;

	hr = GetDevice11()->CreateBuffer(&bd, &InitData, ppVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	return true;
}