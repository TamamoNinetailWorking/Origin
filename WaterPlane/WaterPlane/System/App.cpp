#include "WINDOW/CWindow.h"
#include "DirectXGraphics/CDirectGraphics.h"
#include "../Scene/DemoScene/DemoScene.h"
#include "../Scene/GUI/GUIPackage.h"

//bool Process_Init();
//void Process_Draw();
//
//bool TesterInit();
//void TesterUpdate();
//void TesterDraw();
//void TesterTargetDraw();
//void ReflectView();
//
//bool KadaiInit();
//void KadaiDraw();
//
//void Test4Init();
//void Test4Draw();


//実際のアプリケーション処理部分
void CWINDOW::App()
{

	static int scene = 0;

	switch (scene)
	{
		case 0:

			//Init

			//Test4Init();

			//Process_Init();
			//TesterInit();

			///*if (!KadaiInit())
			//{
			//	return;
			//}*/

			//CGUIPackage::GetInstance().Init();

			CDemoScene::GetInstance().Init();

			scene = 1;

			break;


		case 1:

			CDirectGraphics::GetInstance().Clear();

			//Update & Draw
			
			//Test4Draw();

			//TesterUpdate();

			//TesterTargetDraw();
			//ReflectView();
			//TesterDraw();
			//Process_Draw();

			////KadaiDraw();

			//CGUIPackage::GetInstance().Draw();

			CDemoScene::GetInstance().Update();
			CDemoScene::GetInstance().Render();

			CDirectGraphics::GetInstance().Present();

			break;
	}


	return;
}