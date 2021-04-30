//*****************************************************************************
//!	@file	CDirectXGraphics.h
//!	@brief	
//!	@note	CDirectXGraphicsクラス
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

#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント使用に必要

#endif

//=============================================================================
//!	@class	CDirectXGraphics
//!	@brief	CDirectXGraphicsクラス
//=============================================================================
class CDirectX9{
private:
	LPDIRECT3D9					m_lpd3d;			// DIRECT3D8オブジェクト
	LPDIRECT3DDEVICE9			m_lpd3ddevice;		// DIRECT3DDEVICE8オブジェクト
	D3DPRESENT_PARAMETERS		m_d3dpp;			// プレゼンテーションパラメータ
	D3DDISPLAYMODE				m_disp;				// ディスプレイモード
	int							m_adapter;			// ディスプレイアダプタ番号
	int							m_width;			// バックバッファＸサイズ
	int							m_height;			// バックバッファＹサイズ
	
	CDirectX9(){								// コンストラクタ
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

	bool	Init(HWND hwnd,bool fullscreenflag,int width,int height);	// 初期処理
	void	Exit();														// 終了処理
	LPDIRECT3DDEVICE9	GetDXDevice() const {							// デバイスオブジェクトＧＥＴ
		return m_lpd3ddevice;
	}
	D3DPRESENT_PARAMETERS GetDXD3dpp() const{							// プレゼンテーションパラメータＧＥＴ
		return m_d3dpp;
	}
};

//外部参照
//extern CDirectXGraphics* g_pd9;
//inline LPDIRECT3DDEVICE9 GetDevice9(){ return g_pd9->GetDXDevice(); }
inline LPDIRECT3DDEVICE9 GetDevice9()
{
	return CDirectX9::GetInstance().GetDXDevice();
}


//******************************************************************************
//	End of file.
//******************************************************************************