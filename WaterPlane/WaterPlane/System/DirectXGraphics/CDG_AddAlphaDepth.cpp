#include "CDirectGraphics.h"

HRESULT CDirectGraphics::CreateDefaultBlendState(bool AlphaToCoverageEnable)
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

	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;// TRUEの場合、マルチレンダーターゲットで各レンダーターゲットのブレンドステートの設定を個別に設定できる// FALSEの場合、0番目のみが使用される

	BlendDesc.IndependentBlendEnable = true;

	memcpy_s((void*)&BlendDesc.RenderTarget[0], sizeof(BlendDesc.RenderTarget[0]), (void*)&renderTarget, sizeof(renderTarget));

	if (FAILED(m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState))) 
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CDirectGraphics::CreateAddBlendState(bool AlphaToCoverageEnable)
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

	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;// TRUEの場合、マルチレンダーターゲットで各レンダーターゲットのブレンドステートの設定を個別に設定できる// FALSEの場合、0番目のみが使用される

	BlendDesc.IndependentBlendEnable = true;

	memcpy_s((void*)&BlendDesc.RenderTarget[0], sizeof(BlendDesc.RenderTarget[0]), (void*)&renderTarget, sizeof(renderTarget));

	if (FAILED(m_pDevice->CreateBlendState(&BlendDesc, &m_pAddBlendState)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CDirectGraphics::CreateAlignmentBlendState(bool AlphaToCoverageEnable)
{
	D3D11_RENDER_TARGET_BLEND_DESC renderTarget;
	renderTarget.BlendEnable = TRUE;
	renderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	renderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	renderTarget.BlendOp = D3D11_BLEND_OP_ADD;
	renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	renderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;// TRUEの場合、マルチレンダーターゲットで各レンダーターゲットのブレンドステートの設定を個別に設定できる// FALSEの場合、0番目のみが使用される

	BlendDesc.IndependentBlendEnable = false;

	memcpy_s((void*)&BlendDesc.RenderTarget[0], sizeof(BlendDesc.RenderTarget[0]), (void*)&renderTarget, sizeof(renderTarget));

	if (FAILED(m_pDevice->CreateBlendState(&BlendDesc, &m_pAlignmentBlendState)))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CDirectGraphics::OMSetBlendState(BlendState kind)
{
	UINT mask = 0xffffffff;
	switch (kind)
	{
		case Default:

			m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, mask);

			break;

		case Add:

			m_pDeviceContext->OMSetBlendState(m_pAddBlendState, NULL, mask);

			break;

		case Alignment:

			m_pDeviceContext->OMSetBlendState(m_pAlignmentBlendState, NULL, mask);

			break;
	}
}

void CDirectGraphics::BlendStateSwitch(BlendState state)
{
	OMSetBlendState(state);
}

HRESULT CDirectGraphics::CreateDepthStencilStateOFF()
{
	//ステンシルステート作成
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = false;
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
	HRESULT hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilStateOff);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CDirectGraphics::OMSetDepthStencil(bool state)
{
	//デバイスコンテキストへセット
	if (state)
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	}
	else
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilStateOff, 1);
	}
}

void CDirectGraphics::DepthStencilSwitch(bool state)
{
	OMSetDepthStencil(state);
}