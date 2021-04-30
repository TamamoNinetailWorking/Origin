//*****************************************************************************
//!	@file	CDirectXGraphics.h
//!	@brief	
//!	@note	CDirectXGraphics�N���X
//!	@author
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<d3d9.h>
#include	<d3dx9.h>
#include	"../Utility/NonCopyable.h"

#if TRUE

#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g�g�p�ɕK�v

#endif

//=============================================================================
//!	@class	CDirectXGraphics
//!	@brief	CDirectXGraphics�N���X
//=============================================================================
class CDirectX9{
private:
	LPDIRECT3D9					m_lpd3d;			// DIRECT3D8�I�u�W�F�N�g
	LPDIRECT3DDEVICE9			m_lpd3ddevice;		// DIRECT3DDEVICE8�I�u�W�F�N�g
	D3DPRESENT_PARAMETERS		m_d3dpp;			// �v���[���e�[�V�����p�����[�^
	D3DDISPLAYMODE				m_disp;				// �f�B�X�v���C���[�h
	int							m_adapter;			// �f�B�X�v���C�A�_�v�^�ԍ�
	int							m_width;			// �o�b�N�o�b�t�@�w�T�C�Y
	int							m_height;			// �o�b�N�o�b�t�@�x�T�C�Y
	
	CDirectX9(){								// �R���X�g���N�^
		m_lpd3d = NULL;
		m_lpd3ddevice = NULL;
		m_adapter = 0;
		m_height = 0;
		m_width = 0;
	}
	CDirectX9(CDirectX9& _Device) = delete;
	CDirectX9& operator=(CDirectX9& _Device) = delete;
public:

	static CDirectX9& GetInstance()
	{
		static CDirectX9 instance;
		return instance;
	}

	bool	Init(HWND hwnd,bool fullscreenflag,int width,int height);	// ��������
	void	Exit();														// �I������
	LPDIRECT3DDEVICE9	GetDXDevice() const {							// �f�o�C�X�I�u�W�F�N�g�f�d�s
		return m_lpd3ddevice;
	}
	D3DPRESENT_PARAMETERS GetDXD3dpp() const{							// �v���[���e�[�V�����p�����[�^�f�d�s
		return m_d3dpp;
	}
};

//�O���Q��
//extern CDirectXGraphics* g_pd9;
//inline LPDIRECT3DDEVICE9 GetDevice9(){ return g_pd9->GetDXDevice(); }
inline LPDIRECT3DDEVICE9 GetDevice9()
{
	return CDirectX9::GetInstance().GetDXDevice();
}


//******************************************************************************
//	End of file.
//******************************************************************************