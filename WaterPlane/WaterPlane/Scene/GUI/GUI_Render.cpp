#include "GUIPackage.h"

#include "../../System/IMGUI/imgui.h"
#include "../../System/IMGUI/imgui_impl_dx11.h"
#include "../../System/IMGUI/imgui_impl_win32.h"
#include "../../System/WINDOW/CWindow.h"
#include "../../System/DirectXGraphics/CDirectGraphics.h"
#include <iostream>


bool CGUIPackage::Init(float width,float height)
{
	bool sts = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	sts = ImGui_ImplWin32_Init(g_pWindow->GetHWND());
	if (!sts)
		return sts;

	sts = ImGui_ImplDX11_Init(
		CDirectGraphics::GetInstance().GetDevice(),
		CDirectGraphics::GetInstance().GetDeviceContext());
	if (!sts)
		return sts;

	m_fWidth = width;
	m_fHeight = height;

	return true;
}

void CGUIPackage::Update()
{

}

void CGUIPackage::Draw()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	{
		fpsCalculator();

		fillmodeChange();

		surfaceParameterSlider();

		tessellationSlider();

		waveParameterSlider();

		objectFrame();

		cubemapFrame();
	}

	

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


}

void CGUIPackage::Exit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}