#include "CDirectGraphics.h"

#pragma comment(lib,"dxgi.lib")

HRESULT CDirectGraphics::Create_Device_And_SwapChain()
{
	HRESULT hr = S_OK;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes;
	unsigned int numerator = 60;
	unsigned int denominator = 1;
	DXGI_MODE_DESC* displayModeList;

	//ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	unsigned int numDriverTypes = ARRAYSIZE(driverTypes);
	unsigned int createDeviceFlags = 0;

	//機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// Create a DirectX graphics interface factory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// use the factory to create an adpter for the primary graphics interface(video card)
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// enumerrate primary adapter output(monitor)
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// get the number of modes that fit the DXGI_FORMAT_R8G8B8_UNORM display format forthe adapter output(monitor)
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// create alist to hold all possible display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) 
	{
		return E_FAIL;
	}

	// now fill the display mode list structures
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// now go through all the display modes and find the one that matches the screen width and height
	// when a match is found store the numerator and denominator of the refresh rate for that monitor
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == m_width)
		{
			if (displayModeList[i].Height == m_height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;


	//スワップチェインの設定(ダブルバッファの管理)
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_width;
	sd.BufferDesc.Height = m_height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = numerator;
	sd.BufferDesc.RefreshRate.Denominator = denominator;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (m_full_screen)
	{
		sd.Windowed = false;
	}
	else
	{
		sd.Windowed = TRUE;
	}
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			m_DriverType,
			NULL,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&m_pSwapChain,
			&m_pDevice,
			&m_FeatureLevel,
			&m_pDeviceContext);
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	else
	{
		return S_OK;
	}

}

HRESULT CDirectGraphics::RenderTargetSetting()
{
	//レンダリングターゲットを作成
	HRESULT hr;
	
#if FALSE
	ID3D11Texture2D* pTex2D = nullptr;
	D3D11_TEXTURE2D_DESC Tex2DDesc;
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;

	::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	Tex2DDesc.ArraySize = 1;
	Tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Tex2DDesc.Width = m_width;
	Tex2DDesc.Height = m_height;
	Tex2DDesc.MipLevels = 1;
	Tex2DDesc.SampleDesc.Count = 1;

	::ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = Tex2DDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	hr = GetDevice()->CreateTexture2D(&Tex2DDesc, NULL, &m_pRTV_Tex);
	if (FAILED(hr))
	{
		MessageBox(m_hwnd,"CreateTexture2D Error", "Error", MB_OK);
		return E_FAIL;
	}

	hr = GetDevice()->CreateRenderTargetView(m_pRTV_Tex, &RTVDesc, &m_pBackBuffer_TexRTV);
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, "CreateRenderTargetView Error", "Error", MB_OK);
		return E_FAIL;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	::ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = RTVDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = GetDevice()->CreateShaderResourceView(m_pRTV_Tex, &srvDesc, &m_pRTV_SRV);
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, "CreateRenderTargetShaderResource Error", "Error", MB_OK);
		return E_FAIL;
	}

	//pTex2D->Release();
	//pTex2D = nullptr;
#else if

	ID3D11Texture2D* pBackBuffer = nullptr;

	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	//バックバッファへのポインタを指定してレンダーターゲットビューを生成
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBuffer_TexRTV);
	pBackBuffer->Release();
	pBackBuffer = nullptr;
	if (FAILED(hr))
	{
		return E_FAIL;
	}
#endif
	return S_OK;
}

HRESULT CDirectGraphics::CreateDepthBuffer()
{
	//ZBuffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = m_width;
	depthBufferDesc.Height = m_height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	//depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HRESULT hr = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pBackBuffer_DSTex);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CDirectGraphics::CreateStencilState()
{
	//ステンシルステート作成
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//set up the description of the stencil state
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//stencil operation if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//stencil opeartion if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create the depth stencil state
	HRESULT hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	//デバイスコンテキストへセット
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

#if TRUE
	//デプスステンシルビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	//set up the depth stencil view description
	//depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthStencilViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//create the depth stencil view
	hr = m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, &depthStencilViewDesc, &m_pBackBuffer_DSTexDSV);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

#else if

	//ID3D11Texture2D* pTex2D;
	D3D11_TEXTURE2D_DESC Tex2DDesc;
	
	::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	Tex2DDesc.ArraySize = 1;
	Tex2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.Format = DXGI_FORMAT_D32_FLOAT;
	Tex2DDesc.Width = m_width;
	Tex2DDesc.Height = m_height;
	Tex2DDesc.MipLevels = 1;
	Tex2DDesc.SampleDesc.Count = 1;

	D3D11_DEPTH_STENCIL_VIEW_DESC DescDS;
	::ZeroMemory(&DescDS, sizeof(DescDS));
	DescDS.Format = Tex2DDesc.Format;
	DescDS.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = GetDevice11()->CreateTexture2D(&Tex2DDesc, NULL, &m_pBackBuffer_DSTex);
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, "DepthTexture CreateFailed.", "Error", MB_OK);
		return E_FAIL;
	}

	hr = GetDevice11()->CreateDepthStencilView(m_pBackBuffer_DSTex, &DescDS, &m_pBackBuffer_DSTexDSV);
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, "DepthView CreateFailed.", "Error", MB_OK);
		return E_FAIL;
	}


#endif
	//bind the render target view and depth stencil buffer to the output render pipeline
	//m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	SetDefaultRenderTarget();

	return S_OK;
}

HRESULT CDirectGraphics::CreateRasteRizer(ID3D11RasterizerState*& rasterizer, D3D11_CULL_MODE cull_mode)
{
	D3D11_RASTERIZER_DESC rasterDesc;

	//setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = cull_mode;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//create the rasterrizer state from the description we just filled out
	HRESULT hr = m_pDevice->CreateRasterizerState(&rasterDesc, &rasterizer);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CDirectGraphics::RasterizeSetting()
{
	//D3D11_RASTERIZER_DESC rasterDesc;

	////setup the raster description which will determine how and what polygons will be drawn
	//rasterDesc.AntialiasedLineEnable = false;
	//rasterDesc.CullMode = D3D11_CULL_NONE;
	//rasterDesc.DepthBias = 0;
	//rasterDesc.DepthBiasClamp = 0.0f;
	//rasterDesc.DepthClipEnable = true;
	////rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	//rasterDesc.FillMode = D3D11_FILL_SOLID;
	//rasterDesc.FrontCounterClockwise = false;
	//rasterDesc.MultisampleEnable = false;
	//rasterDesc.ScissorEnable = false;
	//rasterDesc.SlopeScaledDepthBias = 0.0f;

	////create the rasterrizer state from the description we just filled out
	//HRESULT hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_rasterStateCullNone);
	//if (FAILED(hr))
	//{
	//	return E_FAIL;
	//}

	m_rasterStateCullNone = nullptr;
	m_rasterStateFrontCull = nullptr;
	m_rasterStateBackCull = nullptr;

	HRESULT hr = CreateRasteRizer(m_rasterStateCullNone, D3D11_CULL_NONE);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	hr = CreateRasteRizer(m_rasterStateFrontCull, D3D11_CULL_FRONT);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	hr = CreateRasteRizer(m_rasterStateBackCull, D3D11_CULL_BACK);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	//set the rasterizer state
	m_pDeviceContext->RSSetState(m_rasterStateCullNone);

	return S_OK;
}

void CDirectGraphics::ChangeMode_Culling(D3D11_CULL_MODE cull_mode)
{
	switch (cull_mode)
	{
		case D3D11_CULL_NONE:
			m_pDeviceContext->RSSetState(m_rasterStateCullNone);
			break;

		case D3D11_CULL_FRONT:
			m_pDeviceContext->RSSetState(m_rasterStateFrontCull);
			break;

		case D3D11_CULL_BACK:
			m_pDeviceContext->RSSetState(m_rasterStateBackCull);
			break;
	}
}

void CDirectGraphics::ViewPortSetting()
{
	//ビューポートを設定
	//D3D11_VIEWPORT vp;
	m_ViewPort.Width = (FLOAT)m_width;
	m_ViewPort.Height = (FLOAT)m_height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &m_ViewPort);

	return;
}