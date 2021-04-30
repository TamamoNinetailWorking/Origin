#include "CDirectGraphics.h"
#include "../Utility/Memory.h"

void CDirectGraphics::Exit()
{
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
	return;
}

void CDirectGraphics::ExitDetail()
{
	if (m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
	}
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}

	Release(m_rasterStateBackCull);
	Release(m_rasterStateFrontCull);
	Release(m_rasterStateCullNone);
	Release(m_pBackBuffer_DSTexDSV);
	Release(m_pDepthStencilState);
	Release(m_pBackBuffer_DSTex);
	Release(m_pBackBuffer_TexRTV);

	Release(m_pDeviceContext);
	Release(m_pDevice);
	Release(m_pSwapChain);

	return;
}