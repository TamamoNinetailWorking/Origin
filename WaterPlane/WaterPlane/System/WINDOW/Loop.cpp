#include "CWindow.h"
#include "../FPS/fps_controler.h"
#include "../Dependence.h"//外部参照用

void CWINDOW::Loop()
{
	m_msg = { 0 };

	//Xファイル用DirectX9
	//g_pd9 = new CDirectXGraphics;
	if (!CDirectX9::GetInstance().Init(m_hWnd, false, m_width, m_height))
	{
		//g_pd9->Exit();
		//delete g_pd9;
		CDirectX9::GetInstance().Exit();
		return;
	}

	//FPS制御用クラスの生成
	CFPS_CONTROLER::AutoFPSControler(m_hWnd);

	//DirectInputの初期化
	CDirectInput::GetInstance().Init(
		GetHinstance(),
		GetHWND(),
		GetWindowWidth(),
		GetWindowHeight());

	//DirectoryManager Init
	CDirectoryManager::GetInstance().Init();

	//メッセージループ
	while (m_msg.message != WM_QUIT)
	{
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
		else
		{
			FPS_GetTime();
			if (FPS_CheckTime())
			{
				FPS_UpdateTime();
				FPS_Draw();
				//メイン処理
				App();//メインのアプリケーション処理
			}
			FPS_GetTime();
			FPS_Measurement();
		}
	}

	//FPS制御クラスの解放
	CFPS_CONTROLER::ReleaseFPSControler();

	//g_pd9->Exit();
	CDirectX9::GetInstance().Exit();

	return;
}