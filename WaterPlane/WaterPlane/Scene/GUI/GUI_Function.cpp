#include "GUIPackage.h"

#include "../../System/IMGUI/imgui.h"
#include "../../System/IMGUI/imgui_impl_dx11.h"
#include "../../System/IMGUI/imgui_impl_win32.h"
#include "../../System/DirectXGraphics/CDirectGraphics.h"
#include <iostream>

#include "../../Framework/Object/WaterPlane/WaterPlaneObject.h"
#include "../../Framework/Object/ModelObject/SkydomeObject.h"

extern CWaterPlaneObject* m_cWaterPlane;
extern CSkydomeObject* m_cSkydome;


void CGUIPackage::fpsCalculator()
{
	ImGui::SetNextWindowSize(ImVec2(300,60), ImGuiSetCond_Once);
	ImGui::Begin("FrameRate");

	ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

void CGUIPackage::fillmodeChange()
{

	static bool fillModeChange = false;
	char str[21];
	
	if (fillModeChange){
		strcpy_s(str, "FillMode(WireFrame)");
	}else{
		strcpy_s(str, "FillMode(Solid)    ");
	}

	ImGui::SetNextWindowSize(ImVec2(300, 60), ImGuiSetCond_Once);
	ImGui::Begin("FillMode");

	ImGui::Text(str);
	ImGui::SameLine();

	if (ImGui::Checkbox("", &fillModeChange))
	{
		if (fillModeChange)
		{
			CDirectGraphics::GetInstance().Mode_WireFrame();
			//std::cout << "Wire" << std::endl;
		}
		else
		{
			CDirectGraphics::GetInstance().Mode_Solid();
			//std::cout << "Solid" << std::endl;
		}
	}

	ImGui::End();
}

void CGUIPackage::surfaceParameterSlider()
{
	const float distortion_first = 0.3f;
	static float distortion = distortion_first;
	const float dist_min = 0.0f;
	const float dist_max = 2.0f;

	const float speed_first = 3.5f;
	static float speed = speed_first;
	const float spd_min = 0.0f;
	const float spd_max = 10.0f;

	const float power_first = 0.8f;
	static float power = power_first;
	const float pow_min = 0.0f;
	const float pow_max = 5.0f;

	const float refractio_first = 0.4f;
	static float refractio = refractio_first;
	const float ref_min = 0.0f;
	const float ref_max = 1.0f;

	static float color[3] = { 1.0f,1.0f,1.0f };
	const float color_min = 0.0f;
	const float color_max = 1.0f;

	//Header
	{
		ImGui::SetNextWindowSize(ImVec2(300, 60), ImGuiSetCond_Once);
		ImGui::Begin("Surface");
		ImGui::Text("SurfaceParameter");
		ImGui::SameLine();
		if (ImGui::Button("Initialize"))
		{
			distortion = distortion_first;
			speed = speed_first;
			power = power_first;
			refractio = refractio_first;
			color[0] = color[1] = color[2] = color[3] = 1.0f;
		}
		
		/*ID3D11ShaderResourceView* srv = m_cWaterPlane->GetSRV();
		ImGui::Image((void*)m_Map, ImVec2(40, 40));*/
		ImGui::End();
	}

	//Distortion
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiSetCond_Once);
		ImGui::Begin("Distortion");
		ImGui::Text("Distortion");
		ImGui::SliderFloat("", &distortion, dist_min, dist_max);
		ImGui::End();
	}

	//Speed
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiSetCond_Once);
		ImGui::Begin("Speed");
		ImGui::Text("Speed");
		ImGui::SliderFloat("", &speed, spd_min, spd_max);
		ImGui::End();
	}

	//Power
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiSetCond_Once);
		ImGui::Begin("Resistance");
		ImGui::Text("Resistance");
		ImGui::SliderFloat("", &power, pow_min, pow_max);
		ImGui::End();
	}

	//Refractio
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiSetCond_Once);
		ImGui::Begin("Refractio");
		ImGui::Text("Refractio");
		ImGui::SliderFloat("", &refractio, ref_min, ref_max);
		ImGui::End();
	}

	//Color
	{
		ImGui::SetNextWindowSize(ImVec2(300, 60), ImGuiSetCond_Once);
		ImGui::Begin("Color");
		//ImGui::Text("R      G      B      A");
		ImGui::ColorEdit3("", color);
		//ImGui::SliderFloat4("", color, color_min, color_max);
		ImGui::End();
	}

	//Value To WaterObject
	m_cWaterPlane->SetSurfaceParameter(distortion,speed,power,refractio);
	m_cWaterPlane->SetDiffuse(color[0], color[1], color[2], 1.0f);
}

void CGUIPackage::tessellationSlider()
{

	static int tessFactor = 64;
	static int insideFactor = 64;
	
	const int slider_start = 1;
	const int slider_end = 64;

	static bool together = true;

	bool changer = false;

	//Header
	ImGui::SetNextWindowSize(ImVec2(300, 140), ImGuiSetCond_Once);
	ImGui::Begin("Tessellation");
	ImGui::Text("TessellationCoeficient");

	//SliderChanger
	ImGui::Text("Together?");
	ImGui::SameLine();
	ImGui::Checkbox("", &together);
	ImGui::NewLine();

	//SliderMain
	if (together)
	{
		ImGui::Text("Subdivision");
		ImGui::SameLine();
		changer = ImGui::SliderInt("", &tessFactor, slider_start, slider_end);
		insideFactor = tessFactor;
	}
	else
	{
		ImGui::Text("Factor & Insider");
;		int vec[2] = { tessFactor,insideFactor };
		changer = ImGui::SliderInt2("", vec, slider_start, slider_end);
		tessFactor = vec[0];
		insideFactor = vec[1];
	}

	

	ImGui::End();

	//Value To WaterObject
	if (!changer) {
		//CreateVertexMap(tessFactor, tessFactor);
		m_cWaterPlane->SetTessCoeficient((float)tessFactor, (float)insideFactor);
	}
}

void CGUIPackage::waveParameterSlider()
{
	const float amplitude_first = 0.035f;
	static float amplitude = amplitude_first;
	const float amplitude_min = 0.001f;
	const float amplitude_max = 0.1f;

	const float length_first = 0.1f;
	static float length = length_first;
	const float length_min = 0.02f;
	const float length_max = 0.5f;

	const float cycle_first = 16.0f;
	static float cycle = cycle_first;
	const float cycle_min = 1.0f;
	const float cycle_max = 100.0f;

	const float edge_first = 1.0f;
	static float edge = edge_first;
	const float edge_min = 0.1f;
	const float edge_max = 2.0f;

	//Header
	{
		ImGui::SetNextWindowSize(ImVec2(300, 60), ImGuiSetCond_Once);
		ImGui::Begin("Wave");
		ImGui::Text("WaveParameter");
		ImGui::SameLine();
		if (ImGui::Button("Initialize"))
		{
			amplitude = amplitude_first;
			length = length_first;
			cycle = cycle_first;
			edge = edge_first;
		}

		/*ID3D11ShaderResourceView* srv = m_cWaterPlane->GetSRV();
		ImGui::Image((void*)m_Map, ImVec2(40, 40));
		*/
		ImGui::End();
	}

	//Amplitude
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiSetCond_Once);
		ImGui::Begin("Amplitude");
		ImGui::Text("Amplitude");
		ImGui::SliderFloat("", &amplitude , amplitude_min, amplitude_max);
		ImGui::End();
	}

	//WaveLength
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiSetCond_Once);
		ImGui::Begin("WaveLength");
		ImGui::Text("WaveLength");
		ImGui::SliderFloat("", &length, length_min, length_max);
		ImGui::End();
	}

	//WaveCycle
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiSetCond_Once);
		ImGui::Begin("WaveCycle");
		ImGui::Text("WaveCycle");
		ImGui::SliderFloat("", &cycle, cycle_min, cycle_max);
		ImGui::End();
	}

	//Edge
	{
		ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiSetCond_Once);
		ImGui::Begin("Edge");
		ImGui::Text("Edge");
		ImGui::SliderFloat("", &edge, edge_min, edge_max);
		ImGui::End();
	}

	//Value To WaterObject
	m_cWaterPlane->SetWaveParameter(amplitude, length * (-1), cycle, edge);
}

void CGUIPackage::objectFrame()
{
	//Header
	ImGui::SetNextWindowSize(ImVec2(300, 60), ImGuiSetCond_Once);
	ImGui::Begin("ObjectFrame");

	static bool skydomeRender = true;
	char str[21];

	if (skydomeRender) {
		strcpy_s(str, "Skydome(On) ");
	}
	else {
		strcpy_s(str, "Skydome(Off)");
	}

	ImGui::Text(str);
	ImGui::SameLine();

	if (ImGui::Checkbox("", &skydomeRender))
	{
		m_cSkydome->SetActivation(skydomeRender);
	}

	ImGui::End();
}

void CGUIPackage::cubemapFrame()
{

}