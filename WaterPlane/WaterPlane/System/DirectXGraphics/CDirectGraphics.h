#pragma once

//include header
#pragma warning(push)
#pragma warning(disable:4005)
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <D3DCompiler.h>
#pragma warning(pop)

//link library
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//Suppression warning
#pragma warning(disable:4305)

//Macro Define
#define SAFE_RELEASE(p) {if(p){p->Release(); p = nullptr;}}

class CDirectGraphics
{
public:

	//アルファブレンドステート
	enum BlendState
	{
		Default,
		Add,
		Alignment,
	};

private:

	//ウィンドウハンドル受け取り用
	HWND m_hwnd;
	//バックバッファサイズ
	int m_width, m_height;
	bool m_full_screen;

	/* デバイス */

	//ドライバタイプ
	D3D_DRIVER_TYPE m_DriverType;
	//機能レベル
	D3D_FEATURE_LEVEL m_FeatureLevel;

	//デバイス
	ID3D11Device * m_pDevice;
	//デバイスコンテキスト
	ID3D11DeviceContext* m_pDeviceContext;
	//スワップチェイン
	IDXGISwapChain* m_pSwapChain;

	//レンダーターゲットビュー
	ID3D11Texture2D* m_pRTV_Tex;
	ID3D11ShaderResourceView* m_pRTV_SRV;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	//デプスステンシルバッファ
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	//デプスステンシルビュー
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	//ラスターステータス
	ID3D11RasterizerState* m_rasterStateCullNone;
	ID3D11RasterizerState* m_rasterStateFrontCull;
	ID3D11RasterizerState* m_rasterStateBackCull;


	//デプスステンシルステート
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilState* m_pDepthStencilStateOff;

	//アルファブレンド
	ID3D11BlendState* m_pBlendState;
	ID3D11BlendState* m_pAddBlendState;
	ID3D11BlendState* m_pAlignmentBlendState;

	//ビューポート
	D3D11_VIEWPORT m_ViewPort;

	//簡易版
	//------------------------------------------------------------------
	//デバイスとスワップチェーンを作成
	HRESULT device_and_swapchain_Init(HWND hwnd, int width, int height);
	//バックバッファテクスチャを取得し、レンダーターゲットビューを作成
	void get_backbuffer_texture();
	//デプスステンシルビュー用のテクスチャを作成
	void create_depth_stencil_texture(int width,int height);
	//深度テスト
	void depth_stencil_config(bool stencil_enable);
	//ビューポートの設定
	void view_port(int width,int height);
	//ラスタライズ設定
	void rasterise();
	//アルファブレンド設定
	HRESULT SettingAlphaBlend();
	//------------------------------------------------------------------

	//詳細版
	//------------------------------------------------------------------
	HRESULT Create_Device_And_SwapChain();
	HRESULT RenderTargetSetting();
	HRESULT CreateDepthBuffer();
	HRESULT CreateStencilState();
	HRESULT RasterizeSetting();
	void ViewPortSetting();
	//------------------------------------------------------------------

	//ブレンドステートのセット
	HRESULT SetBlendState(D3D11_BLEND_DESC& blendstate);
	HRESULT SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC* blendDesc, UINT NumBlendState, bool AlphaToCoverageEnable, bool IndependentBlendEnable);
	HRESULT SetDefaultBlend();
	HRESULT SetAddBlend();
	HRESULT SetAligmentBlend();

	HRESULT CreateDefaultBlendState(bool AlphaToCoverageEnable);
	HRESULT CreateAddBlendState(bool AlphaToCoverageEnable);
	HRESULT CreateAlignmentBlendState(bool AlphaToCoverageEnable);
	void OMSetBlendState(BlendState kind);

	//デプスステンシルのセット
	HRESULT CreateDepthStencilStateOFF();
	void OMSetDepthStencil(bool state);

	//フィルモード変更
	HRESULT CreateRasteRizer(ID3D11RasterizerState*& rasterizer, D3D11_CULL_MODE cull_mode);
	bool FillModeChange_KindRasterizer(ID3D11RasterizerState*& rasterizer, D3D11_FILL_MODE fill_mode);
	bool Change_FillMode(D3D11_FILL_MODE fill_mode);

	//Shader作成
	HRESULT MakeShader(LPCSTR szFileName, LPCSTR szFuncName, LPCSTR sProfileName, void**ppShader, ID3DBlob** ppBlob);


	//コンストラクタ
	CDirectGraphics() {
		m_pDevice = nullptr;
		m_pDeviceContext = nullptr;
		m_pSwapChain = nullptr;
		m_pBackBuffer_TexRTV = nullptr;
		m_pBackBuffer_DSTexDSV = nullptr;
		m_pDepthStencilState = nullptr;
		m_pBackBuffer_DSTex = nullptr;
	};

	CDirectGraphics(CDirectGraphics& _Device) = delete;
	CDirectGraphics& operator=(CDirectGraphics& _Device) = delete;

public:

	//デストラクタ
	~CDirectGraphics() {};

	static CDirectGraphics& GetInstance() {
		static CDirectGraphics instance;
		return instance;
	}

	bool Init(HWND hwnd, bool full_screen_flag, int width, int height);
	bool Init_Detail(HWND hwnd, bool full_screen_flag, int width, int height);
	void Exit();
	void ExitDetail();

	ID3D11Device* GetDevice() const { return m_pDevice; };
	ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext; };
	IDXGISwapChain* GetSwapChain()const {
		return m_pSwapChain;};
	ID3D11RenderTargetView* GetRenderTargetView() const {
		return m_pBackBuffer_TexRTV;};
	ID3D11DepthStencilView* GetDepthStencilView() const {
		return m_pBackBuffer_DSTexDSV;};

	void SetRenderTarget(ID3D11RenderTargetView*& RTV);
	void SetRenderTarget(ID3D11RenderTargetView*& RTV, ID3D11DepthStencilView*& DSV);
	void SetDefaultRenderTarget();

	void SetDefaultViewPort();
	void SetViewPort(
		float width,
		float height,
		float minDepth = 0.0f,
		float maxDepth = 1.0f,
		float topLeftX = 0,
		float topLeftY = 0);

	//Set UnorderedAccessView To PixelShader
	void SetRenderTargetAndUAV(ID3D11RenderTargetView*& RTV, ID3D11UnorderedAccessView*& UAV);
	void SetDefaultRenderTargetAndUAV(ID3D11UnorderedAccessView*& UAV);

	//ブレンドのモードチェンジ
	void ChangeBlendState(BlendState state);
	void BlendStateSwitch(BlendState state);

	//デプスバッファのチェンジ
	void DepthStencilSwitch(bool state);

	//ラスタライズモード
	void ChangeMode_Culling(D3D11_CULL_MODE cull_mode);
	void Mode_Solid();
	void Mode_WireFrame();

	//画面クリア
	void Clear();
	//バックバッファをフロントバッファに
	void Present();
};

//外部参照
//extern CDirectGraphics* g_pDGraphics;

//あまりにもよく使う子達をinline関数に
inline ID3D11Device* GetDevice11() 
{
	//return g_pDGraphics->GetDevice(); 
	return CDirectGraphics::GetInstance().GetDevice();
}
inline ID3D11DeviceContext* GetImmediateContext() 
{ 
	//return g_pDGraphics->GetDeviceContext(); 
	return CDirectGraphics::GetInstance().GetDeviceContext();
};

inline void DefaultBlendState()
{
	//g_pDGraphics->BlendStateSwitch(CDirectGraphics::BlendState::Default);
	CDirectGraphics::GetInstance().BlendStateSwitch(CDirectGraphics::BlendState::Default);
}

inline void AddOnMaxBlendState()
{
	//g_pDGraphics->BlendStateSwitch(CDirectGraphics::BlendState::Add);
	CDirectGraphics::GetInstance().BlendStateSwitch(CDirectGraphics::BlendState::Add);
}

inline void AlignmentBlendState()
{
	CDirectGraphics::GetInstance().BlendStateSwitch(CDirectGraphics::BlendState::Alignment);
}

inline void DepthStencilSwitch(bool flag)
{
	CDirectGraphics::GetInstance().DepthStencilSwitch(flag);
}