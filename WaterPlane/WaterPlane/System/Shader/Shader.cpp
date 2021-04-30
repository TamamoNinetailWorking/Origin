#include "Shader.h"
#include "../Utility/Memory.h"

#include <d3dcompiler.h>
#include <locale.h>

HRESULT CShader::CompileShaderFromFile(char* szFilename, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	ID3DBlob* p1 = nullptr;
	HRESULT hr = S_OK;
	WCHAR filename[512];
	unsigned int wLen = 0;
	int err = 0;

	//char->wchar‚É•ÏŠ·
	setlocale(LC_ALL, "japanese");
	err = mbstowcs_s(&wLen, filename, 512, szFilename, _TRUNCATE);

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined ( _DEBUG )

	dwShaderFlags |= D3DCOMPILE_DEBUG;

#endif

	ID3DBlob* pErrorBlob = nullptr;

	hr = D3DCompileFromFile(
		filename,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		szEntryPoint,
		szShaderModel,
		dwShaderFlags,
		0,
		ppBlobOut,
		&pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr)
		{
			MessageBox(NULL,
				(char*)pErrorBlob->GetBufferPointer(),
				"Error", MB_OK);
		}
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void CShader::Exit()
{
	Release(m_pGeometryShader);
	Release(m_pPixelShader);
	Release(m_pVertexLayout);
	Release(m_pVertexShader);
	return;
}