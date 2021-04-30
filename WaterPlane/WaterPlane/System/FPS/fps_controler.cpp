#include "fps_controler.h"

//環境に応じてここを変化させる
//
#include "../Config.h"
//
//スタティックメンバ変数
CFPS_CONTROLER* CFPS_CONTROLER::fps_controler = NULL;


void CFPS_CONTROLER::Begin(UINT period)
{
	resolution = period;
	timeBeginPeriod(resolution);
	execLastTime = fpsLastTime = timeGetTime();
	return;
}

void CFPS_CONTROLER::End()
{
	timeEndPeriod(resolution);
	return;
}

CFPS_CONTROLER::CFPS_CONTROLER(HWND hwnd,UINT period = 1)
:second(1000),FPS(static_cast<float>(second / FrameRate)), measure_point(1000),countFPS(0.0f)
{
	m_hwnd = hwnd;
	Begin(period);
	return;
}

CFPS_CONTROLER::CFPS_CONTROLER(HWND hwnd)
:second(1000), FPS(static_cast<float>(second / FrameRate)), measure_point(1000), countFPS(0.0f)
{
	m_hwnd = hwnd;
	Begin(1);
	return;
}

void CFPS_CONTROLER::AutoFPSControler(HWND hwnd,UINT period = 1)
{
	if (fps_controler == NULL)
	{
		fps_controler = new CFPS_CONTROLER(hwnd,period);
	}
	return;
}

void CFPS_CONTROLER::AutoFPSControler(HWND hwnd)
{
	{
		if (fps_controler == NULL)
		{
			fps_controler = new CFPS_CONTROLER(hwnd,1);
		}
		return;
	}
}

CFPS_CONTROLER::~CFPS_CONTROLER()
{
	End();
	return;
}

void CFPS_CONTROLER::ReleaseFPSControler()
{
	if (fps_controler != NULL)
	{
		delete fps_controler;
		fps_controler = NULL;
	}
	return;
}

CFPS_CONTROLER* CFPS_CONTROLER::GetFPSControler()
{
	return fps_controler;
}

void CFPS_CONTROLER::getTime()
{
	currentTime = timeGetTime();
	return;
}

bool CFPS_CONTROLER::checkTime()
{
	return (currentTime - execLastTime >= FPS);
}

void CFPS_CONTROLER::updateTime()
{
	countFrame++;
	execLastTime = timeGetTime();
	return;
}

void CFPS_CONTROLER::Measurement_FPS()
{
	if (currentTime - fpsLastTime >= measure_point)
	{
		countFPS = static_cast<float> ((countFrame * second) / (currentTime - fpsLastTime));
		fpsLastTime = currentTime;
		countFrame = 0;
	}

	return;
}

void CFPS_CONTROLER::Draw()
{
	char str[256];
	sprintf_s(str, "%s	FPS:%.2f\n",WindowName, countFPS);

#ifdef _SCREEN_WRITE
	dx_font.Draw(str,0,0);
#else
	SetWindowText(m_hwnd, (LPSTR)str);
#endif

	return;
}