#include "ShaderCast.h"

#include <locale.h>
#include <memory>
#include "../../DirectxTex/DirectXTex.h"
#include "../../DirectxTex/WICTextureLoader.h"
#pragma comment(lib,"System/DirectxTex/lib/DirectXTex.lib")

namespace NShaderCast
{

	static bool CreateSRVfromTGAFile(ID3D11Device*& device,const char* filename, ID3D11ShaderResourceView** ppSRV)
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

		//Create Shader ResourceView from Texture File
		hr = DirectX::CreateShaderResourceView(device, image->GetImages(), image->GetImageCount(), meta, ppSRV);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	static bool CreateSRVfromWICFile(ID3D11Device*& device,ID3D11DeviceContext*& deviceContext,const char* filename, ID3D11ShaderResourceView** ppSRV)
	{

		ID3D11Resource* texr = nullptr;

		wchar_t ws[512];
		size_t ret;

		setlocale(LC_CTYPE, "jpn");
		mbstowcs_s(&ret, ws, 512, filename, _TRUNCATE);

		//Read Texture from File
		HRESULT hr = DirectX::CreateWICTextureFromFile(
			device,
			deviceContext,
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


	bool CreateShaderResourceView(
		ID3D11Device*& device,
		ID3D11DeviceContext*& deviceContext,
		LPCSTR filename,
		ID3D11ShaderResourceView*& pSRV)
	{
		bool sts = false;
		
		sts = CreateSRVfromWICFile(device, deviceContext, filename, &pSRV);

		if (!sts)
		{
			//Failed to read texture from "WIC"File
			sts = CreateSRVfromTGAFile(device, filename, &pSRV);
			if (!sts)
			{
				MessageBox(NULL, "Failed to create texture from file.", "Error", MB_OK);
				return false;
			}
		}

		return true;
	}
}