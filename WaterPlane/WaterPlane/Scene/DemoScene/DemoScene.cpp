#include "DemoScene.h"

#include "../GUI/GUIPackage.h"

bool Process_Init();
void Process_Draw();

bool TesterInit();
void TesterUpdate();
void TesterDraw();
void TesterTargetDraw();
void ReflectView();

void Test4Init();
void Test4Draw();

bool TessellationInit();
void TessellationUpdate();
void TessellationDraw();

bool CubeMapInit();
void CubeMapUpdate();
void CubeMapDraw();

bool CDemoScene::Init()
{

	Process_Init();
	TesterInit();

	CubeMapInit();
	
	TessellationInit();

	if (CGUIPackage::GetInstance().Init())
	{
		return false;
	}

	return true;
}


void CDemoScene::Update()
{
	TesterUpdate();

	CubeMapUpdate();
	
	TessellationUpdate();

	CGUIPackage::GetInstance().Update();
}


void CDemoScene::Render()
{
	//TesterTargetDraw();
	//ReflectView();
	//TesterDraw();
	//Process_Draw();

	CubeMapDraw();

	TessellationDraw();

	CGUIPackage::GetInstance().Draw();
}