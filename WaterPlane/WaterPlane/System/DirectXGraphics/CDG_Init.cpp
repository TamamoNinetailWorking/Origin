#include "CDirectGraphics.h"
#include <stdio.h>

//CDirectGraphics* g_pDGraphics = nullptr;

bool CDirectGraphics::Init(HWND hwnd, bool full_screen_flag, int width, int height)
{
	//�E�B���h�E�n���h���󂯎��
	m_hwnd = hwnd;

	//�f�o�C�X�ƃX���b�v�`�F�[�����쐬
	if (FAILED(device_and_swapchain_Init(hwnd, width, height)))
	{
		return FALSE;
	}

	//�e��e�N�X�`���[�Ƃ���ɕt�т���e��r���[�̍쐬

	//�o�b�N�o�b�t�@�e�N�X�`�����擾
	get_backbuffer_texture();

	//�f�v�X�X�e���V���r���[�p�̃e�N�X�`�����쐬
	create_depth_stencil_texture(width, height);

	//���̃e�N�X�`���ɑ΂��ăf�v�X�X�e���V���r���[���쐬
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//�����_�[�^�[�Q�b�g�r���[�g�f�v�X�X�e���V���r���[���p�C�v���C���ɃZ�b�g
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);

	//�[�x�e�X�g
	//��X�͗L���ɂ��Ă������ق����ǂ���������Ȃ�
	depth_stencil_config(false);

	//�r���[�|�[�g�̐ݒ�
	view_port(width, height);

	//���X�^���C�Y�ݒ�
	rasterise();

	//�A���t�@�u�����h�ݒ�
	if (FAILED(SettingAlphaBlend()))
	{
		return E_FAIL;
	}

	//�����I��
	return TRUE;
}

bool CDirectGraphics::Init_Detail(HWND hwnd, bool full_screen_flag, int width, int height)
{

	m_hwnd = hwnd;
	m_width = width;
	m_height = height;
	m_full_screen = full_screen_flag;

	//�@�\���x���̐ݒ�
	HRESULT hr;
	hr = Create_Device_And_SwapChain();
	if (FAILED(hr))
	{
		printf("Device and SwapChain Create Failed.\n");
		return false;
	}

	//�����_�[�^�[�Q�b�g�̐ݒ�
	hr = RenderTargetSetting();
	if (FAILED(hr))
	{
		printf("RednerTargetSetting Failed.\n");
		return false;
	}

	//�f�v�X�o�b�t�@
	hr = CreateDepthBuffer();
	if (FAILED(hr))
	{
		printf("DepthBuffer Create Failed.\n");
		return false;
	}

	//�X�e���V���X�e�[�g�̍쐬
	hr = CreateStencilState();
	if (FAILED(hr))
	{
		printf("StencilState Create Failed.\n");
		return false;
	}

	//���X�^���C�Y�̐ݒ�
	hr = RasterizeSetting();
	if (FAILED(hr))
	{
		printf("Rasterize Setting Failed.\n");
		return false;
	}

	//�r���[�|�[�g��ݒ�
	ViewPortSetting();

	//�A���t�@�u�����h�ݒ�
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