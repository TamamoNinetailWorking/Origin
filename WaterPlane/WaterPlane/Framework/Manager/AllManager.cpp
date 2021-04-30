#include "AllManager.h"
#include "../../System/DirectXGraphics/CDirectGraphics.h"
#include "../../System/Direct9/CDirectxGraphics.h"
#include "../../System/Input/CDirectInput.h"

CDirectGraphics& CAllManager::GetDirectX11()
{
	return CDirectGraphics::GetInstance();
}

CDirectX9& CAllManager::GetDirectX9()
{
	return CDirectX9::GetInstance();
}

CDirectInput& CAllManager::GetDirectInput()
{
	return CDirectInput::GetInstance();
}