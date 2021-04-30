#pragma once

#include "../../System/Utility/NonCopyable.h"

class CCameraObject;

class CDemoScene : public NonCopyable
{
private:

	CCameraObject * m_pMainCamera;

public:

	static CDemoScene& GetInstance()
	{
		static CDemoScene instance;
		return instance;
	}

	bool Init();
	void Update();
	void Render();

};