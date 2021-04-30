#include "CDirectGraphics.h"

HRESULT CDirectGraphics::MakeShader(LPCSTR szFileName,LPCSTR szFuncName,LPCSTR szProfileName,void** ppShader,ID3DBlob** ppBlob)
{
	ID3DBlob* pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors,NULL)))
	{
		char* p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		SAFE_RELEASE(pErrors);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//�ǂ̃V�F�[�_�[�Ȃ̂��̎��ʎq����A�擪�Q�����𒊏o
	char szProfile[3] = { 0 };
	memcpy(szProfile,szProfileName,2);

	//VertexShader�̐���
	if (strcmp(szProfile, "vs") == 0)
	{
		if (FAILED(
			m_pDevice->CreateVertexShader(
			(*ppBlob)->GetBufferPointer(),
			(*ppBlob)->GetBufferSize(),
			NULL,
			(ID3D11VertexShader**)ppShader)
		)//CreateVertexShader
			)//FAILED Macro
		{
			MessageBox(m_hwnd, "VertexShader Error", "Error", S_OK);
			return E_FAIL;
		}
	}//VertexShader�̐����I���

	//PixelShader�̐���
	if (strcmp(szProfile, "ps") == 0)
	{
		if (FAILED(
			m_pDevice->CreatePixelShader(
				(*ppBlob)->GetBufferPointer(),
				(*ppBlob)->GetBufferSize(),
				NULL,
				(ID3D11PixelShader**)ppShader)
		)//CreatePixelShader
			) //FAILED Macro
		{
			MessageBox(m_hwnd, "PixelShader Error", "Error", S_OK);
			return E_FAIL;
		}
	}//PixelShader�̐����I���

	//GeometryShader�̐���
	if (strcmp(szProfile, "gs") == 0)
	{
		if (FAILED(
			m_pDevice->CreateGeometryShader(
				(*ppBlob)->GetBufferPointer(),
				(*ppBlob)->GetBufferSize(),
				NULL,
				(ID3D11GeometryShader**)ppShader)
		)
			)
		{
			MessageBox(m_hwnd, "GeometryShader Error", "Error", S_OK);
			return E_FAIL;
		}
	}//GetometryShader�̐����I���

	//HullShader�̐���
	if (strcmp(szProfile, "hs") == 0)
	{
		if (FAILED(
			m_pDevice->CreateHullShader(
				(*ppBlob)->GetBufferPointer(),
				(*ppBlob)->GetBufferSize(),
				NULL,
				(ID3D11HullShader**)ppShader)
		)
			)
		{
			MessageBox(m_hwnd, "HullShader Error", "Error", S_OK);
			return E_FAIL;
		}
	}//HullShader�̐����I���

	//DomainShader�̐���
	if (strcmp(szProfile, "ds") == 0)
	{
		if (FAILED(
			m_pDevice->CreateDomainShader(
				(*ppBlob)->GetBufferPointer(), 
				(*ppBlob)->GetBufferSize(),
				NULL, 
				(ID3D11DomainShader**)ppShader)
		)
			)
		{

			MessageBox(m_hwnd, "DomainShader Error", "Error", S_OK);
			return E_FAIL;
		}
	}//DomainShader�̐����I���

	//ComputeShader�̐���
	if (strcmp(szProfile, "cs") == 0)
	{
		if (FAILED(
			m_pDevice->CreateComputeShader(
				(*ppBlob)->GetBufferPointer(), 
				(*ppBlob)->GetBufferSize(),
				NULL, 
				(ID3D11ComputeShader**)ppShader)
		)
			)
		{

			MessageBox(m_hwnd, "ComputeShader Error", "Error", S_OK);
			return E_FAIL;
		}
	}//ComuteShader�̐����I���

	return S_OK;
}