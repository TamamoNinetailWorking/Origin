#include "CDirectGraphics.h"
#include <iostream>

void CDirectGraphics::Clear()
{
	//画面クリア
	float ClearColor[4] = { 0, 0, 0, 1 };//クリアカラー RGBA

	//カラーバッファクリア
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);

	//デプスステンシルバッファクリア
	m_pDeviceContext->ClearDepthStencilView
	(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return;
}

void CDirectGraphics::Present()
{
	//画面更新(バックバッファをフロントバッファに
	m_pSwapChain->Present(0, 0);
	return;
}

bool CDirectGraphics::FillModeChange_KindRasterizer(ID3D11RasterizerState*& rasterizer,D3D11_FILL_MODE fill_mode)
{
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterizer->GetDesc(&rasterDesc);
	rasterDesc.FillMode = fill_mode;

	rasterizer->Release();
	HRESULT hr;
	hr = m_pDevice->CreateRasterizerState(&rasterDesc, &rasterizer);
	if (!hr)
	{
		return false;
	}

	return true;
}

bool CDirectGraphics::Change_FillMode(D3D11_FILL_MODE fill_mode)
{
	/*D3D11_RASTERIZER_DESC rasterDesc;

	m_rasterStateCullNone->GetDesc(&rasterDesc);
	rasterDesc.FillMode = fill_mode;

	m_rasterStateCullNone->Release();
	m_pDevice->CreateRasterizerState(&rasterDesc, &m_rasterStateCullNone);*/

	if (FillModeChange_KindRasterizer(m_rasterStateCullNone, fill_mode))
	{
		std::cout << "CullNoneState Change Failed." << std::endl;
		return false;
	}

	if (FillModeChange_KindRasterizer(m_rasterStateFrontCull, fill_mode))
	{
		std::cout << "CullFrontState Change Failed." << std::endl;
		return false;
	}

	if (FillModeChange_KindRasterizer(m_rasterStateBackCull, fill_mode))
	{
		std::cout << "CullBackState Change Failed." << std::endl;
		return false;
	}

	//暫定
	m_pDeviceContext->RSSetState(m_rasterStateCullNone);
	return true;

}

void CDirectGraphics::Mode_Solid()
{
	Change_FillMode(D3D11_FILL_SOLID);
	return;
}

void CDirectGraphics::Mode_WireFrame()
{
	Change_FillMode(D3D11_FILL_WIREFRAME);
	return;
}

void CDirectGraphics::SetRenderTarget(ID3D11RenderTargetView*& RTV)
{
	m_pDeviceContext->OMSetRenderTargets(1, &RTV, m_pBackBuffer_DSTexDSV);
}

void CDirectGraphics::SetRenderTarget(ID3D11RenderTargetView*& RTV, ID3D11DepthStencilView*& DSV)
{
	m_pDeviceContext->OMSetRenderTargets(1, &RTV, DSV);
}

void CDirectGraphics::SetDefaultRenderTarget()
{
	SetRenderTarget(m_pBackBuffer_TexRTV);
}

void CDirectGraphics::SetRenderTargetAndUAV(ID3D11RenderTargetView*& RTV, ID3D11UnorderedAccessView*& UAV)
{
	m_pDeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(
		1, 
		&RTV, 
		m_pBackBuffer_DSTexDSV, 
		1, 
		1, 
		&UAV,
		0);
}

void CDirectGraphics::SetDefaultRenderTargetAndUAV(ID3D11UnorderedAccessView*& UAV)
{
	m_pDeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV,
		1,
		1,
		&UAV,
		0);
}

void CDirectGraphics::SetDefaultViewPort()
{
	m_pDeviceContext->RSSetViewports(1, &m_ViewPort);
}

void CDirectGraphics::SetViewPort(float width, float height, float minDepth, float maxDepth, float topLeftX, float topLeftY)
{
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = minDepth;
	vp.MaxDepth = maxDepth;
	vp.TopLeftX = topLeftX;
	vp.TopLeftY = topLeftY;
	m_pDeviceContext->RSSetViewports(1, &m_ViewPort);
}