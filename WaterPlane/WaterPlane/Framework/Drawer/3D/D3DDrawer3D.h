#pragma once

#include<d3dx9.h>
//include header
#pragma warning(push)
#pragma warning(disable:4005)
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>
#include <D3DCompiler.h>
#include "../../../System/Utility/Memory.h"
#pragma warning(pop)

//link library
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//Suppression warning
#pragma warning(disable:4305)

#include <vector>
#include "Drawer3D.h"


class CD3DDrawer3D : public CDrawer3D<ID3D11Buffer, ID3D11Buffer, ID3D11Buffer, ID3D11Buffer, D3DXMATRIX>
{
protected:

	std::vector<ID3D11Buffer*> m_pBufferContainer;
	std::vector<ID3D11ShaderResourceView*> m_pSRVContainer;
	
	ID3D11SamplerState* m_pSamplerState;

public:

	virtual ~CD3DDrawer3D()
	{
		Exit();
	}

	void Exit()
	{
		ReleaseVector<ID3D11Buffer*>(m_pBufferContainer);
		ReleaseVector<ID3D11ShaderResourceView*>(m_pSRVContainer);
		Release(m_pSamplerState);
	}

	ID3D11Buffer*& GetConstantBuffer(UINT index)
	{
		if (index >= m_pBufferContainer.size())
		{
			return m_pBufferContainer[0];
		}
		else
		{
			return m_pBufferContainer[index];
		}
	}

	ID3D11ShaderResourceView*& GetShaderResourceView(UINT index)
	{
		if (index >= m_pSRVContainer.size())
		{
			return m_pSRVContainer[0];
		}
		else
		{
			return m_pSRVContainer[index];
		} 
	}

};