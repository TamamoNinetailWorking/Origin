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

	//�A���t�@�u�����h�X�e�[�g
	enum BlendState
	{
		Default,
		Add,
		Alignment,
	};

private:

	//�E�B���h�E�n���h���󂯎��p
	HWND m_hwnd;
	//�o�b�N�o�b�t�@�T�C�Y
	int m_width, m_height;
	bool m_full_screen;

	/* �f�o�C�X */

	//�h���C�o�^�C�v
	D3D_DRIVER_TYPE m_DriverType;
	//�@�\���x��
	D3D_FEATURE_LEVEL m_FeatureLevel;

	//�f�o�C�X
	ID3D11Device * m_pDevice;
	//�f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_pDeviceContext;
	//�X���b�v�`�F�C��
	IDXGISwapChain* m_pSwapChain;

	//�����_�[�^�[�Q�b�g�r���[
	ID3D11Texture2D* m_pRTV_Tex;
	ID3D11ShaderResourceView* m_pRTV_SRV;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	//�f�v�X�X�e���V���o�b�t�@
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	//�f�v�X�X�e���V���r���[
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	//���X�^�[�X�e�[�^�X
	ID3D11RasterizerState* m_rasterStateCullNone;
	ID3D11RasterizerState* m_rasterStateFrontCull;
	ID3D11RasterizerState* m_rasterStateBackCull;


	//�f�v�X�X�e���V���X�e�[�g
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilState* m_pDepthStencilStateOff;

	//�A���t�@�u�����h
	ID3D11BlendState* m_pBlendState;
	ID3D11BlendState* m_pAddBlendState;
	ID3D11BlendState* m_pAlignmentBlendState;

	//�r���[�|�[�g
	D3D11_VIEWPORT m_ViewPort;

	//�ȈՔ�
	//------------------------------------------------------------------
	//�f�o�C�X�ƃX���b�v�`�F�[�����쐬
	HRESULT device_and_swapchain_Init(HWND hwnd, int width, int height);
	//�o�b�N�o�b�t�@�e�N�X�`�����擾���A�����_�[�^�[�Q�b�g�r���[���쐬
	void get_backbuffer_texture();
	//�f�v�X�X�e���V���r���[�p�̃e�N�X�`�����쐬
	void create_depth_stencil_texture(int width,int height);
	//�[�x�e�X�g
	void depth_stencil_config(bool stencil_enable);
	//�r���[�|�[�g�̐ݒ�
	void view_port(int width,int height);
	//���X�^���C�Y�ݒ�
	void rasterise();
	//�A���t�@�u�����h�ݒ�
	HRESULT SettingAlphaBlend();
	//------------------------------------------------------------------

	//�ڍה�
	//------------------------------------------------------------------
	HRESULT Create_Device_And_SwapChain();
	HRESULT RenderTargetSetting();
	HRESULT CreateDepthBuffer();
	HRESULT CreateStencilState();
	HRESULT RasterizeSetting();
	void ViewPortSetting();
	//------------------------------------------------------------------

	//�u�����h�X�e�[�g�̃Z�b�g
	HRESULT SetBlendState(D3D11_BLEND_DESC& blendstate);
	HRESULT SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC* blendDesc, UINT NumBlendState, bool AlphaToCoverageEnable, bool IndependentBlendEnable);
	HRESULT SetDefaultBlend();
	HRESULT SetAddBlend();
	HRESULT SetAligmentBlend();

	HRESULT CreateDefaultBlendState(bool AlphaToCoverageEnable);
	HRESULT CreateAddBlendState(bool AlphaToCoverageEnable);
	HRESULT CreateAlignmentBlendState(bool AlphaToCoverageEnable);
	void OMSetBlendState(BlendState kind);

	//�f�v�X�X�e���V���̃Z�b�g
	HRESULT CreateDepthStencilStateOFF();
	void OMSetDepthStencil(bool state);

	//�t�B�����[�h�ύX
	HRESULT CreateRasteRizer(ID3D11RasterizerState*& rasterizer, D3D11_CULL_MODE cull_mode);
	bool FillModeChange_KindRasterizer(ID3D11RasterizerState*& rasterizer, D3D11_FILL_MODE fill_mode);
	bool Change_FillMode(D3D11_FILL_MODE fill_mode);

	//Shader�쐬
	HRESULT MakeShader(LPCSTR szFileName, LPCSTR szFuncName, LPCSTR sProfileName, void**ppShader, ID3DBlob** ppBlob);


	//�R���X�g���N�^
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

	//�f�X�g���N�^
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

	//�u�����h�̃��[�h�`�F���W
	void ChangeBlendState(BlendState state);
	void BlendStateSwitch(BlendState state);

	//�f�v�X�o�b�t�@�̃`�F���W
	void DepthStencilSwitch(bool state);

	//���X�^���C�Y���[�h
	void ChangeMode_Culling(D3D11_CULL_MODE cull_mode);
	void Mode_Solid();
	void Mode_WireFrame();

	//��ʃN���A
	void Clear();
	//�o�b�N�o�b�t�@���t�����g�o�b�t�@��
	void Present();
};

//�O���Q��
//extern CDirectGraphics* g_pDGraphics;

//���܂�ɂ��悭�g���q�B��inline�֐���
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