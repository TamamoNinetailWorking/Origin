#include "CDirectGraphics.h"
#include <stdio.h>

//CDirectGraphics* g_pDGraphics = nullptr;

bool CDirectGraphics::Init(HWND hwnd, bool full_screen_flag, int width, int height)
{
	//ウィンドウハンドル受け取り
	m_hwnd = hwnd;

	//デバイスとスワップチェーンを作成
	if (FAILED(device_and_swapchain_Init(hwnd, width, height)))
	{
		return FALSE;
	}

	//各種テクスチャーとそれに付帯する各種ビューの作成

	//バックバッファテクスチャを取得
	get_backbuffer_texture();

	//デプスステンシルビュー用のテクスチャを作成
	create_depth_stencil_texture(width, height);

	//そのテクスチャに対してデプスステンシルビューを作成
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//レンダーターゲットビュートデプスステンシルビューをパイプラインにセット
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);

	//深度テスト
	//後々は有効にしておいたほうが良いかもしれない
	depth_stencil_config(false);

	//ビューポートの設定
	view_port(width, height);

	//ラスタライズ設定
	rasterise();

	//アルファブレンド設定
	if (FAILED(SettingAlphaBlend()))
	{
		return E_FAIL;
	}

	//無事終了
	return TRUE;
}

bool CDirectGraphics::Init_Detail(HWND hwnd, bool full_screen_flag, int width, int height)
{

	m_hwnd = hwnd;
	m_width = width;
	m_height = height;
	m_full_screen = full_screen_flag;

	//機能レベルの設定
	HRESULT hr;
	hr = Create_Device_And_SwapChain();
	if (FAILED(hr))
	{
		printf("Device and SwapChain Create Failed.\n");
		return false;
	}

	//レンダーターゲットの設定
	hr = RenderTargetSetting();
	if (FAILED(hr))
	{
		printf("RednerTargetSetting Failed.\n");
		return false;
	}

	//デプスバッファ
	hr = CreateDepthBuffer();
	if (FAILED(hr))
	{
		printf("DepthBuffer Create Failed.\n");
		return false;
	}

	//ステンシルステートの作成
	hr = CreateStencilState();
	if (FAILED(hr))
	{
		printf("StencilState Create Failed.\n");
		return false;
	}

	//ラスタライズの設定
	hr = RasterizeSetting();
	if (FAILED(hr))
	{
		printf("Rasterize Setting Failed.\n");
		return false;
	}

	//ビューポートを設定
	ViewPortSetting();

	//アルファブレンド設定
	hr = CreateDefaultBlendState(false);
	if (FAILED(hr))
	{
		printf("DefaultBlendState Failed.\n");
		return false;
	}

	hr = CreateAddBlendState(false);
	if (FAILED(hr))
	{
		printf("AddBlendState Failed.\n");
		return false;
	}

	hr = CreateAlignmentBlendState(false);
	if (FAILED(hr))
	{
		printf("AlignmentBlendState Failed.\n");
		return false;
	}

	hr = CreateDepthStencilStateOFF();
	if (FAILED(hr))
	{
		printf("StencilState Create Failed.\n");
		return false;
	}

	OMSetDepthStencil(false);

	//OMSetBlendState(Default);

	return true;
}