#include "Shader.h"
#include "../DirectXGraphics/CDirectGraphics.h"

#include <locale.h>
#include <memory>
#include "../DirectxTex/DirectXTex.h"
#include "../DirectxTex/WICTextureLoader.h"
#pragma comment(lib,"System/DirectxTex/lib/DirectXTex.lib")


bool CShader::CreateSamplerState(ID3D11SamplerState*& pSamplerState)
{
	HRESULT hr;
	D3D11_SAMPLER_DESC SamDesc;
	::ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = GetDevice11()->CreateSamplerState(&SamDesc, &pSamplerState);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool CShader::CreateShaderResourceView(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRV)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	pBuffer->GetDesc(&bd);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;

	if (bd.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		srvDesc.BufferEx.NumElements = bd.ByteWidth / 4;
	}
	else if (bd.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements = bd.ByteWidth / bd.StructureByteStride;
	}
	else
	{
		return false;
	}

	HRESULT hr;
	hr = GetDevice11()->CreateShaderResourceView(pBuffer, &srvDesc, ppSRV);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateShaderResourceView error", "Error", MB_OK);
		return false;
	}

	return true;
}

bool CShader::CreateSRVfromTGAFile(const char* filename, ID3D11ShaderResourceView** ppSRV)
{

	HRESULT hr;
	wchar_t ws[512];
	size_t ret;

	setlocale(LC_CTYPE, "jpn");
	mbstowcs_s(&ret, ws, 512, filename, _TRUNCATE);

	DirectX::TexMetadata meta;
	DirectX::GetMetadataFromTGAFile(ws, meta);

	typename std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);
	hr = LoadFromTGAFile(ws, &meta, *image);
	if (FAILED(hr))
	{
		return false;
	}

	//シェーダーリソースビューの作成
	hr = DirectX::CreateShaderResourceView(GetDevice11(), image->GetImages(), image->GetImageCount(), meta, ppSRV);
	if (FAILED(hr)) 
	{
		return false;
	}

	return true;
}


bool CShader::CreateSRVfromWICFile(const char* filename, ID3D11ShaderResourceView** ppSRV)
{

	ID3D11Resource* texr = nullptr;//テクスチャリソース

	wchar_t ws[512];
	size_t ret;

	setlocale(LC_CTYPE, "jpn");
	mbstowcs_s(&ret, ws, 512, filename, _TRUNCATE);

	//テクスチャ読み込み
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		GetDevice11(),
		GetImmediateContext(),
		ws,
		&texr,
		ppSRV);

	if (FAILED(hr))
	{
		//MessageBox(NULL, "CreateWICTextureFromFile", "Error", MB_OK);
		return false;
	}

	return true;
}