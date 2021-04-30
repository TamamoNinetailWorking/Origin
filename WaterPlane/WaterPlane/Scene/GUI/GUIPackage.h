#pragma once

#include "../../System/Utility/NonCopyable.h"

class CGUIPackage : public NonCopyable
{
private:

	float m_fWidth;
	float m_fHeight;



	void fpsCalculator();
	void fillmodeChange();
	void surfaceParameterSlider();
	void tessellationSlider();
	void waveParameterSlider();
	void objectFrame();
	void cubemapFrame();

public:

	static CGUIPackage& GetInstance()
	{
		static CGUIPackage instance;
		return instance;
	}

	bool Init(float width = 400,float height = 600);

	void Update();

	void Draw();

	void Exit();

};