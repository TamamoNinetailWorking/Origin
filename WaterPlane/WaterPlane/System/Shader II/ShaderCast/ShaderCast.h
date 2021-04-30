#pragma once

#include <d3d11.h>

//Template CreateFunction 
namespace NShaderCast
{
	bool CreateVertexShader(
		ID3D11Device*& device, 
		char* szFileName, 
		LPCSTR szFuncName, 
		D3D11_INPUT_ELEMENT_DESC* layout, 
		unsigned int numElements, 
		ID3D11VertexShader*& pVertexShader, 
		ID3D11InputLayout*& pInputLayout);

	bool CreateVertexShader(
		ID3D11Device*& device,
		char* szFileName,
		LPCSTR szFuncName,
		ID3D11VertexShader*& pVertexShader,
		ID3D11InputLayout*& pInputLayout);

	bool CreatePixelShader(
		ID3D11Device*& device, 
		char*szFileName, 
		LPCSTR szFuncName, 
		ID3D11PixelShader*& pPixelShader);

	bool CreateGeometryShader(
		ID3D11Device*& device, 
		char*szFileName, 
		LPCSTR szFuncName, 
		ID3D11GeometryShader*& pGeometryShader);

	bool CreateHullShader(
		ID3D11Device*& device,
		char* szFileName,
		LPCSTR szFuncName,
		ID3D11HullShader*& pHullShader);

	bool CreateDomainShader(
		ID3D11Device*& device,
		char* szFileName,
		LPCSTR szFuncName,
		ID3D11DomainShader*& pDomainShader);

	bool CreateVertexBuffer(
		ID3D11Device*& device,
		UINT stride, 
		UINT VertexNum, 
		void* pVertices, 
		ID3D11Buffer*& pVertexBuffer);

	bool CreateIndexBuffer(
		ID3D11Device*& device,
		UINT IndexNum, 
		void* IndexData, 
		ID3D11Buffer*& pIndexBuffer);

	bool CreateConstantBuffer(
		ID3D11Device*& device,
		UINT ByteSize, 
		ID3D11Buffer*& pConstantBuffer);
	
	bool CreateSamplerState(
		ID3D11Device*& device,
		ID3D11SamplerState*& pSamplerState);

	bool CreateShaderResourceView(
		ID3D11Device*& device,
		ID3D11DeviceContext*& deviceContext,
		LPCSTR filename, 
		ID3D11ShaderResourceView*& pSRV);

}