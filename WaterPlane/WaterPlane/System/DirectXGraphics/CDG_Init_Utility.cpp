#include "CDirectGraphics.h"

HRESULT CDirectGraphics::device_and_swapchain_Init(HWND hwnd,int width,int height)
{
	//デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC swapchain_desc;
	ZeroMemory(&swapchain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapchain_desc.BufferCount = 1;
	swapchain_desc.BufferDesc.Width = width;
	swapchain_desc.BufferDesc.Height = height;
	swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.OutputWindow = hwnd;
	swapchain_desc.SampleDesc.Count = 1;
	swapchain_desc.SampleDesc.Quality = 0;
	swapchain_desc.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatures = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &pFeatures, 1, D3D11_SDK_VERSION, &swapchain_desc, &m_pSwapChain, &m_pDevice, pFeatureLevel, &m_pDeviceContext)))
	{
		MessageBox(hwnd, "Device Error", "Error", S_OK);
		return FALSE;
	}

	return S_OK;
}

void CDirectGraphics::get_backbuffer_texture()
{
	//バックバッファテクスチャを取得
	ID3D11Texture2D* pBackBuffer_Texture;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Texture);
	//そのテクスチャに対してレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView(pBackBuffer_Texture, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Texture);
	return;
}

void CDirectGraphics::create_depth_stencil_texture(int width, int height)
{
	//デプスステンシルビュー用のテクスチャを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pBackBuffer_DSTex);

	return;
}

void CDirectGraphics::depth_stencil_config(bool stencil_enable)
{
	//深度テスト
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = stencil_enable;

	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	return;
}

void CDirectGraphics::view_port(int width,int height)
{
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &viewport);
	return;
}

void CDirectGraphics::rasterise()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;//カリングの有無
	rdc.FillMode = D3D11_FILL_SOLID;//モデルデータの指定
	rdc.FrontCounterClockwise = TRUE;
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);
	return;
}

HRESULT CDirectGraphics::SettingAlphaBlend()
{
	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
	blend_desc.IndependentBlendEnable = true;
	blend_desc.AlphaToCoverageEnable = true;
	blend_desc.RenderTarget[0].BlendEnable = true;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return SetBlendState(blend_desc);
}

HRESULT CDirectGraphics::SetBlendState(D3D11_BLEND_DESC& blendstate)
{
	if (m_pBlendState != nullptr) {
		m_pBlendState->Release();
	}

	if (FAILED(
		m_pDevice->CreateBlendState(
			&blendstate,
			&m_pBlendState)
	)
		)
	{
		return E_FAIL;
	}

	UINT mask = 0xffffffff;
	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, mask);
	return S_OK;
}

HRESULT CDirectGraphics::SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC* blendDesc, UINT NumBlendState, bool AlphaToCoverageEnable,bool IndependentBlendEnable)
{
	if (NumBlendState > 8)
	{
		return E_FAIL;
	}

	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;// TRUEの場合、マルチレンダーターゲットで各レンダーターゲットのブレンドステートの設定を個別に設定できる// FALSEの場合、0番目のみが使用される

	BlendDesc.IndependentBlendEnable = IndependentBlendEnable;

	for (UINT i = 0; i < NumBlendState; i++)
	{
		memcpy_s((void*)&BlendDesc.RenderTarget[i], sizeof(BlendDesc.RenderTarget[i]), (void*)&blendDesc[i], sizeof(blendDesc[i]));
	}

	return SetBlendState(BlendDesc);
}

HRESULT CDirectGraphics::SetDefaultBlend()
{
	D3D11_RENDER_TARGET_BLEND_DESC renderTarget;
	renderTarget.BlendEnable = FALSE;
	renderTarget.SrcBlend = D3D11_BLEND_ONE;
	renderTarget.DestBlend = D3D11_BLEND_ZERO;
	renderTarget.BlendOp = D3D11_BLEND_OP_ADD;
	renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	//renderTarget.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return SetBlendState(&renderTarget, 1, false,false);
}

HRESULT CDirectGraphics::SetAddBlend()
{
	D3D11_RENDER_TARGET_BLEND_DESC renderTarget;
	renderTarget.BlendEnable = TRUE;
	renderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	renderTarget.DestBlend = D3D11_BLEND_ONE;
	renderTarget.BlendOp = D3D11_BLEND_OP_ADD;
	renderTarget.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	renderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTarget.BlendOpAlpha = D3D11_BLEND_OP_MAX;
	renderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return SetBlendState(&renderTarget, 1, false,true);
}

HRESULT CDirectGraphics::SetAligmentBlend()
{
	D3D11_RENDER_TARGET_BLEND_DESC renderTarget;
	ZeroMemory(&renderTarget, sizeof(renderTarget));
	renderTarget.BlendEnable = TRUE;
	renderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	renderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	renderTarget.BlendOp = D3D11_BLEND_OP_ADD;
	renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return SetBlendState(&renderTarget, 1, false,false);
}

void CDirectGraphics::ChangeBlendState(CDirectGraphics::BlendState blendstate)
{
	switch (blendstate)
	{
		case Default:

			SetDefaultBlend();

			break;

		case Add:

			SetAddBlend();

			break;

		case Alignment:

			SetAligmentBlend();

			break;

		default:
			break;
	}
}