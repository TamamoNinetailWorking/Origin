#include <Windows.h>
#include "CWindow.h"
#include "../DirectXGraphics/CDirectGraphics.h"

CWINDOW* g_pWindow = nullptr;

//Entry Point
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmd)
{

	g_pWindow = new CWINDOW;
	if (g_pWindow != nullptr)
	{
		//ウィンドウの初期化
		if (SUCCEEDED(g_pWindow->Create_Window(hInstance, cmd)))
		{
			//g_pDGraphics = new CDirectGraphics;
			//DirectXの初期化
			if (CDirectGraphics::GetInstance().Init_Detail(
								g_pWindow->GetHWND(),
								false,
								g_pWindow->GetWindowWidth(),
								g_pWindow->GetWindowHeight())
				)
			{
				//メインループ
				g_pWindow->Loop();
			}
		}
		CDirectGraphics::GetInstance().ExitDetail();
		//delete g_pDGraphics;
	}

	return (int)g_pWindow->GetMSG().wParam;
}

//Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pWindow->MsgProc(hWnd, uMsg, wParam, lParam);
}