#include "ShaderSet.h"
#include "../Utility/Memory.h"
#include "ShaderManager/ShaderManager.h"

template<class Tempus>
static Tempus TemplateGetter(std::vector<Tempus> substance,UINT index)
{
	if (substance.size() > index)
	{
		return substance[index];
	}
	else
	{
		//Index Out of Range
		return nullptr;
	}
}

ID3D11VertexShader* CShaderSet::GetVertexShader(UINT index)
{
	return TemplateGetter<ID3D11VertexShader*>(m_vVertexShader, index);
}

ID3D11InputLayout* CShaderSet::GetInputLayout(UINT index)
{
	return TemplateGetter<ID3D11InputLayout*>(m_vInputLayout, index);
}

ID3D11PixelShader* CShaderSet::GetPixelShader(UINT index)
{
	return TemplateGetter<ID3D11PixelShader*>(m_vPixelShader, index);
}

ID3D11GeometryShader* CShaderSet::GetGeometryShader(UINT index)
{
	return TemplateGetter<ID3D11GeometryShader*>(m_vGeometryShader, index);
}

ID3D11HullShader* CShaderSet::GetHullShader(UINT index)
{
	return TemplateGetter<ID3D11HullShader*>(m_vHullShader, index);
}

ID3D11DomainShader* CShaderSet::GetDomainShader(UINT index)
{
	return TemplateGetter<ID3D11DomainShader*>(m_vDomainShader, index);
}


bool CShaderSet::CreateVertexShader(ID3D11Device*& device, const char* file_name, const char* func_name)
{
	ID3D11VertexShader* vertex_shader;
	ID3D11InputLayout* input_layout;
	if (CShaderManager::GetInstance().GetVertexShader(device, file_name, func_name, vertex_shader, input_layout))
	{
		//Created.
		m_vVertexShader.push_back(vertex_shader);
		m_vInputLayout.push_back(input_layout);
		return true;
	}
	else
	{
		MessageBox(NULL, "FileName or FuncName is a fault.", "CShaderSet CreateVertexShader", MB_OK); 
		return false;
	}
}

bool CShaderSet::CreatePixelShader(ID3D11Device*& device, const char* file_name, const char* func_name)
{
	ID3D11PixelShader* pixel_shader;
	if (CShaderManager::GetInstance().GetPixelShader(device, file_name, func_name, pixel_shader))
	{
		//Created.
		m_vPixelShader.push_back(pixel_shader);
		return true;
	}
	else
	{
		MessageBox(NULL, "FileName or FuncName is a fault.", "CShaderSet CreatePixelShader", MB_OK);
		return false;
	}
}

bool CShaderSet::CreateGeometryShader(ID3D11Device*& device, const char* file_name, const char* func_name)
{
	ID3D11GeometryShader* geometry_shader;
	if (CShaderManager::GetInstance().GetGeometryShader(device, file_name, func_name, geometry_shader))
	{
		//Created.
		m_vGeometryShader.push_back(geometry_shader);
		return true;
	}
	else
	{
		MessageBox(NULL, "FileName or FuncName is a fault.", "CShaderSet CreateGeometryShader", MB_OK);
		return false;
	}
}

bool CShaderSet::CreateHullShader(ID3D11Device*& device, const char* file_name, const char* func_name)
{
	ID3D11HullShader* hull_shader;
	if (CShaderManager::GetInstance().GetHullShader(device, file_name, func_name, hull_shader))
	{
		//Created.
		m_vHullShader.push_back(hull_shader);
		return true;
	}
	else
	{
		MessageBox(NULL, "FileName or FuncName is a fault.", "CShaderSet CreateHullShader", MB_OK);
		return false;
	}
}

bool CShaderSet::CreateDomainShader(ID3D11Device*& device, const char* file_name, const char* func_name)
{
	ID3D11DomainShader* domain_shader;
	if (CShaderManager::GetInstance().GetDomainShader(device, file_name, func_name, domain_shader))
	{
		//Created.
		m_vDomainShader.push_back(domain_shader);
		return true;
	}
	else
	{
		MessageBox(NULL, "FileName or FuncName is a fault.", "CShaderSet CreateDomainShader", MB_OK);
		return false;
	}
}

void CShaderSet::SetVertexShader(ID3D11DeviceContext*& deviceContext, UINT index)
{
	deviceContext->VSSetShader(GetVertexShader(index), NULL, 0);
	deviceContext->IASetInputLayout(GetInputLayout(index));
}

void CShaderSet::SetPixelShader(ID3D11DeviceContext*& deviceContext, UINT index)
{
	deviceContext->PSSetShader(GetPixelShader(index), NULL, 0);
}

void CShaderSet::SetGeometryShader(ID3D11DeviceContext*& deviceContext, UINT index)
{
	deviceContext->GSSetShader(GetGeometryShader(index), NULL, 0);
}

void CShaderSet::SetHullShader(ID3D11DeviceContext*& deviceContext, UINT index)
{
	deviceContext->HSSetShader(GetHullShader(index), NULL, 0);
}

void CShaderSet::SetDomainShader(ID3D11DeviceContext*& deviceContext, UINT index)
{
	deviceContext->DSSetShader(GetDomainShader(index), NULL, 0);
}

void CShaderSet::Exit()
{
	ReleaseVector<ID3D11VertexShader*>(m_vVertexShader);
	ReleaseVector<ID3D11InputLayout*>(m_vInputLayout);
	ReleaseVector<ID3D11PixelShader*>(m_vPixelShader);
	ReleaseVector<ID3D11GeometryShader*>(m_vGeometryShader);
	ReleaseVector<ID3D11HullShader*>(m_vHullShader);
	ReleaseVector<ID3D11DomainShader*>(m_vDomainShader);
}