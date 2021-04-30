#pragma once
#include "../../System/Utility/NonCopyable.h"

class CDirectGraphics;
class CDirectX9;
class CDirectInput;


class CAllManager : public NonCopyable
{
private:



public:

	static CAllManager& GetInstance()
	{
		static CAllManager instance;
		return instance;
	}

	CDirectGraphics& GetDirectX11();
	CDirectX9& GetDirectX9();
	CDirectInput& GetDirectInput();

};