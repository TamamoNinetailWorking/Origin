#include "Shader.h"
#include "../DirectXGraphics/CDirectGraphics.h"

bool CShader::CreateConstantBuffer(unsigned int ByteSize,ID3D11Buffer** ppConstantBuffer)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;//バッファ使用方法
	bd.ByteWidth = ByteSize;//バッファの大きさ
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//コンスタントバッファ
	bd.CPUAccessFlags = 0;//CPUアクセス不要

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
	bd.ByteWidth = sizeof(unsigned int) * IndexNum;//バッファ大きさ
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;//インデックスバッファ
	bd.CPUAccessFlags = 0;//CPUアクセス不要

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
	bd.ByteWidth = stride * VertexNum;//バッファの大きさ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点バッファ
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