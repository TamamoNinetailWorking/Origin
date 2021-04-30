#include "ShaderCast.h"

#include <d3dcompiler.h>
#include <locale.h>
#include <vector>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

namespace NShaderCast
{
	//=====================================================================
	//
	//@brief : When we create shader from "File",we must use this function.
	//
	//=====================================================================
	static HRESULT CompileShaderFromFile(char* szFilename, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
	{
		ID3DBlob* p1 = nullptr;
		HRESULT hr = S_OK;
		WCHAR filename[512];
		unsigned int wLen = 0;
		int err = 0;

		//convert char->wchar
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

	//=====================================================================
	//
	//@brief : When we create shader reflector,we must use this function.
	//
	//=====================================================================
	static DXGI_FORMAT GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask)
	{
		if (mask == 0x0F)
		{
			//xyzw
			switch (type)
			{
				case D3D10_REGISTER_COMPONENT_FLOAT32:
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
		}

		if (mask == 0x07)
		{
			//xyz
			switch (type)
			{
				case D3D10_REGISTER_COMPONENT_FLOAT32:
					return DXGI_FORMAT_R32G32B32_FLOAT;
			}
		}

		if (mask == 0x03)
		{
			//xy
			switch (type)
			{
				case D3D10_REGISTER_COMPONENT_FLOAT32:
					return DXGI_FORMAT_R32G32_FLOAT;
			}
		}

		if (mask == 0x01)
		{
			//x
			switch (type)
			{
				case D3D10_REGISTER_COMPONENT_FLOAT32:
					return DXGI_FORMAT_R32_FLOAT;
			}
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	//=====================================================================
	//
	//@brief : Create "Vertex"Shader
	//
	//=====================================================================
	bool CreateVertexShader(ID3D11Device*& device,char* szFileName, LPCSTR szFuncName, D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements, ID3D11VertexShader*& pVertexShader,ID3D11InputLayout*& pInputLayout)
	{
		ID3DBlob* pBlob = nullptr;
		LPCSTR shaderModel = "vs_5_0";

		HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
		if (FAILED(hr))
		{
			return false;
		}

		//Create Vertex"Shader"
		hr = device->CreateVertexShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pVertexShader);
		if (FAILED(hr))
		{
			pBlob->Release();
			return false;
		}

		//Create Vertex"InputLayout"
		hr = device->CreateInputLayout(
			layout,
			numElements,
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "CreateInputLayout error", "error", MB_OK);
			pBlob->Release();
			return false;
		}
		pBlob->Release();
		return true;
	}

	//=====================================================================
	//
	//@brief : Create "Vertex"Shader to Use Reflector
	//
	//=====================================================================
	bool CreateVertexShader(ID3D11Device*& device, char* szFileName, LPCSTR szFuncName, ID3D11VertexShader*& pVertexShader, ID3D11InputLayout*& pInputLayout)
	{
		ID3DBlob* pBlob = nullptr;
		LPCSTR shaderModel = "vs_5_0";

		HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
		if (FAILED(hr))
		{
			return false;
		}

		//Create Vertex"Shader"
		hr = device->CreateVertexShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pVertexShader);
		if (FAILED(hr))
		{
			pBlob->Release();
			return false;
		}

		//Create Reflector
		ID3D11ShaderReflection* pReflector;
		hr = D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
		if (FAILED(hr))
		{
			pReflector->Release();
			pBlob->Release();
			return false;
		}

		D3D11_SHADER_DESC shaderdesc;
		pReflector->GetDesc(&shaderdesc);

		std::vector<D3D11_INPUT_ELEMENT_DESC> vElements;
		for (UINT i = 0; i < shaderdesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC sigdesc;
			pReflector->GetInputParameterDesc(i, &sigdesc);

			auto format = GetDxgiFormat(sigdesc.ComponentType, sigdesc.Mask);

			D3D11_INPUT_ELEMENT_DESC eledesc = {
				sigdesc.SemanticName,//This valiable is Pointer. Because it was invalid when that reference is Deleted.
				sigdesc.SemanticIndex,
				format,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0};

			vElements.push_back(eledesc);
		}

		if (!vElements.empty())
		{
			//Create Vertex"InputLayout"
			hr = device->CreateInputLayout(
				&vElements[0],
				vElements.size(),
				pBlob->GetBufferPointer(),
				pBlob->GetBufferSize(),
				&pInputLayout);

			if (FAILED(hr))
			{
				MessageBox(nullptr, "CreateInputLayout error", "error", MB_OK);
				pReflector->Release();
				pBlob->Release();
				return false;
			}
			pReflector->Release();
			pBlob->Release();
		}
		else
		{
			pReflector->Release();
			pBlob->Release();
			MessageBox(nullptr, "D3D11_ELEMENT_DESC is Empty.(We tried to create InputLayout using reflector.)", "error", MB_OK);
			return false;
		}
		return true;
	}

	//=====================================================================
	//
	//@brief : Create "Pixel"Shader
	//
	//=====================================================================
	bool CreatePixelShader(ID3D11Device*& device,char*szFileName, LPCSTR szFuncName,ID3D11PixelShader*& pPixelShader)
	{
		ID3DBlob* pBlob = nullptr;
		LPCSTR shaderModel = "ps_5_0";

		HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
		if (FAILED(hr))
		{
			return false;
		}

		hr = device->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pPixelShader);
		if (FAILED(hr))
		{
			pBlob->Release();
			return false;
		}
		pBlob->Release();
		return true;
	}

	//=====================================================================
	//
	//@brief : Create "Geometry"Shader
	//
	//=====================================================================
	bool CreateGeometryShader(ID3D11Device*& device, char*szFileName, LPCSTR szFuncName, ID3D11GeometryShader*& pGeometryShader)
	{
		ID3DBlob* pBlob = nullptr;
		LPCSTR shaderModel = "gs_5_0";

		HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
		if (FAILED(hr))
		{
			return false;
		}

		hr = device->CreateGeometryShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pGeometryShader);
		if (FAILED(hr))
		{
			pBlob->Release();
			return false;
		}
		pBlob->Release();
		return true;
	}

	//=====================================================================
	//
	//@brief : Create "Hull"Shader
	//
	//=====================================================================
	bool CreateHullShader(ID3D11Device*& device, char* szFileName, LPCSTR szFuncName, ID3D11HullShader*& pHullShader)
	{
		ID3DBlob* pBlob = nullptr;
		LPCSTR shaderModel = "hs_5_0";

		HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
		if (FAILED(hr))
		{
			return false;
		}

		hr = device->CreateHullShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pHullShader);
		if (FAILED(hr))
		{
			pBlob->Release();
			return false;
		}
		pBlob->Release();
		return true;
	}

	//=====================================================================
	//
	//@brief : Create "Domain"Shader
	//
	//=====================================================================
	bool CreateDomainShader(ID3D11Device*& device, char* szFileName, LPCSTR szFuncName, ID3D11DomainShader*& pDomainShader)
	{
		ID3DBlob* pBlob = nullptr;
		LPCSTR shaderModel = "ds_5_0";

		HRESULT hr = CompileShaderFromFile(szFileName, szFuncName, shaderModel, &pBlob);
		if (FAILED(hr))
		{
			return false;
		}

		hr = device->CreateDomainShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pDomainShader);
		if (FAILED(hr))
		{
			pBlob->Release();
			return false;
		}
		pBlob->Release();
		return true;
	}

	//=====================================================================
	//
	//@brief : Create "Vertex"Buffer
	//
	//=====================================================================
	bool CreateVertexBuffer(ID3D11Device*& device,UINT stride, UINT VertexNum, void* pVertices, ID3D11Buffer*& pVertexBuffer)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = stride * VertexNum;//Buffer Size
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//Use as "Vertex"Buffer
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pVertices;

		hr = device->CreateBuffer(&bd, &InitData, &pVertexBuffer);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
			return false;
		}

		return true;
	}

	//=====================================================================
	//
	//@brief : Create "Index"Buffer
	//
	//=====================================================================
	bool CreateIndexBuffer(ID3D11Device*& device,UINT IndexNum,void* IndexData,ID3D11Buffer*& pIndexBuffer)
	{

		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA InitData;

		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * IndexNum;//Buffer Size
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;//Use as "Index"Buffer
		bd.CPUAccessFlags = 0;//Don't Permission to access from CPU.

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = IndexData;

		HRESULT hr = device->CreateBuffer(&bd, &InitData, &pIndexBuffer);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "CreateBuffer(index buffer) error", "Error", MB_OK);
			return false;
		}

		return true;
	}

	//=====================================================================
	//
	//@brief : Create "Constant"Buffer
	//
	//=====================================================================
	bool CreateConstantBuffer(ID3D11Device*& device,UINT ByteSize,ID3D11Buffer*& pConstantBuffer)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = ByteSize;//BufferSize
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//Use as "Constant"Buffer
		bd.CPUAccessFlags = 0;//CPUアクセス不要

		HRESULT hr = device->CreateBuffer(&bd, nullptr, &pConstantBuffer);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "CreateBuffer(constant buffer) error", "Error", MB_OK);
			return false;
		}

		return true;
	}

	//=====================================================================
	//
	//@brief : Create Sampler State
	//
	//=====================================================================
	bool CreateSamplerState(
		ID3D11Device*& device,
		ID3D11SamplerState*& pSamplerState)
	{
		HRESULT hr;
		D3D11_SAMPLER_DESC SamDesc;
		::ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
		SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = device->CreateSamplerState(&SamDesc, &pSamplerState);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

}