#include "CWindow.h"
#include "../FPS/fps_controler.h"
#include "../Dependence.h"//�O���Q�Ɨp

void CWINDOW::Loop()
{
	m_msg = { 0 };

	//X�t�@�C���pDirectX9
	//g_pd9 = new CDirectXGraphics;
	if (!CDirectX9::GetInstance().Init(m_hWnd, false, m_width, m_height))
	{
		//g_pd9->Exit();
		//delete g_pd9;
		CDirectX9::GetInstance().Exit();
		return;
	}

	//FPS����p�N���X�̐���
	CFPS_CONTROLER::AutoFPSControler(m_hWnd);

	//DirectInput�̏�����
	CDirectInput::GetInstance().Init(
		GetHinstance(),
		GetHWND(),
		GetWindowWidth(),
		GetWindowHeight());

	//DirectoryManager Init
	CDirectoryManager::GetInstance().Init();

	//���b�Z�[�W���[�v
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
				//���C������
				App();//���C���̃A�v���P�[�V��������
			}
			FPS_GetTime();
			FPS_Measurement();
		}
	}

	//FPS����N���X�̉��
	CFPS_CONTROLER::ReleaseFPSControler();

	//g_pd9->Exit();
	CDirectX9::GetInstance().Exit();

	return;
}