#include "Shader.h"
#include "../Dependence.h"

bool CShader::CreateVertexShader(char* szFileName, LPCSTR szFuncName, D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements)
{
	ID3DBlob* pBlob = nullptr;
	LPCSTR shaderModel = "vs_5_0";

	HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
	if (FAILED(hr))
	{
		return false;
	}

	//頂点シェーダーを作成
	hr = GetDevice11()->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&m_pVertexShader);
	if (FAILED(hr))
	{
		pBlob->Release();
		return false;
	}

	//頂点データ定義生成
	hr = GetDevice11()->CreateInputLayout(
		layout,
		numElements,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateInputLayout error", "error", MB_OK);
		pBlob->Release();
		return false;
	}
	pBlob->Release();
	return true;
}


bool CShader::CreatePixelShader(char*szFileName, LPCSTR szFuncName)
{
	ID3DBlob* pBlob = nullptr;
	LPCSTR shaderModel = "ps_5_0";

	HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
	if (FAILED(hr))
	{
		return false;
	}

	hr = GetDevice11()->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&m_pPixelShader);
	if (FAILED(hr))
	{
		pBlob->Release();
		return false;
	}
	pBlob->Release();
	return true;
}

bool CShader::CreateGeometryShader(char* szFileName, LPCSTR szFuncName)
{
	ID3DBlob* pBlob = nullptr;
	LPCSTR shaderModel = "gs_5_0";

	HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
	if (FAILED(hr))
	{
		return false;
	}

	hr = GetDevice11()->CreateGeometryShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&m_pGeometryShader);
	if (FAILED(hr))
	{
		pBlob->Release();
		return false;
	}
	pBlob->Release();
	return true;
}