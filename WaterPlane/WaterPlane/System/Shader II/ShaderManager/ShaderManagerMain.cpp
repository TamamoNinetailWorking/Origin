#include "ShaderManager.h"
#include "../ShaderCast/ShaderCast.h"

using NKeyStruct = NShaderManager::KeyStruct;
using NVertexAndInput = NShaderManager::VertexShaderAndInputLayout;

using VertexMap = NShaderManager::VertexShaderMap;
using PixelMap = NShaderManager::PixelShaderMap;
using GeometryMap = NShaderManager::GeometryShaderMap;

template <typename Shader>
static bool ShaderList_Check(
	std::map<NKeyStruct, Shader>& map, 
	const char* file, 
	const char* func)
{
	if (map.count(NKeyStruct(file, func)))
	{
		return true;//Exists
	}
	else
	{
		return false;//Not Exists
	}

}

template <typename Shader>
static void ShaderList_Add(
	std::map<NKeyStruct, Shader>& map,
	Shader& pointer, const char* file_name, const char* func_name)
{
	map[NKeyStruct(file_name, func_name)] = pointer;
	return;
}

template <typename Shader>
static Shader ShaderList_Get(
	std::map<NKeyStruct, Shader>& map,const char* file_name, const char* func_name)
{
	return map[NKeyStruct(file_name, func_name)];
}

CShaderManager::CShaderManager()
{
	AllClear();
}

CShaderManager::~CShaderManager()
{
	AllClear();
}

void CShaderManager::AllClear()
{
	m_pVertexShaderMap.clear();
	m_pInputLayoutMap.clear();
	m_pPixelShaderMap.clear();
	m_pGeometryShaderMap.clear();
	m_pHullShaderMap.clear();
	m_pDomainShaderMap.clear();
}

bool CShaderManager::CheckWhetherVertexShaderIsAbleToChatch(const char* file_name, const char* func_name)
{
	return ShaderList_Check<ID3D11VertexShader*>(m_pVertexShaderMap, file_name, func_name);
}

bool CShaderManager::CheckWhetherInputLayoutIsAbleToChatch(const char* file_name, const char* func_name)
{
	return ShaderList_Check<ID3D11InputLayout*>(m_pInputLayoutMap, file_name, func_name);
}

bool CShaderManager::CheckWhetherPixelShaderIsAbleToChatch(const char* file_name,const char* func_name)
{
	return ShaderList_Check<ID3D11PixelShader*>(m_pPixelShaderMap, file_name, func_name);
}

bool CShaderManager::CheckWhetherGeometryShaderIsAbleToChatch(const char* file_name,const char* func_name)
{
	return ShaderList_Check<ID3D11GeometryShader*>(m_pGeometryShaderMap, file_name, func_name);
}

bool CShaderManager::CheckWhetherHullShaderIsAbleToChatch(const char* file_name, const char* func_name)
{
	return ShaderList_Check<ID3D11HullShader*>(m_pHullShaderMap, file_name, func_name);
}

bool CShaderManager::CheckWhetherDomainShaderIsAbleToChatch(const char* file_name, const char* func_name)
{
	return ShaderList_Check<ID3D11DomainShader*>(m_pDomainShaderMap, file_name, func_name);
}

bool CShaderManager::GetVertexShader(ID3D11Device* device, const char* file_name, const char* func_name, ID3D11VertexShader*& pVertexShader, ID3D11InputLayout*& pInputLayout)
{
	if (CheckWhetherVertexShaderIsAbleToChatch(file_name, func_name))
	{
		//Exists.
		//VAI = ShaderList_Get<NVertexAndInput>(m_pVertexShaderMap, file_name, func_name);
		//VAI = m_pVertexShaderMap[NKeyStruct(file_name, func_name)];
		pVertexShader = ShaderList_Get<ID3D11VertexShader*>(m_pVertexShaderMap,file_name,func_name);
		pInputLayout = ShaderList_Get<ID3D11InputLayout*>(m_pInputLayoutMap,file_name,func_name);
		return true;
	}
	else
	{
		//Not Exists.
		if (NShaderCast::CreateVertexShader(device, (char*)file_name, func_name, pVertexShader, pInputLayout))
		{
			ShaderList_Add <ID3D11VertexShader*>(m_pVertexShaderMap, pVertexShader, file_name, func_name);
			ShaderList_Add<ID3D11InputLayout*>(m_pInputLayoutMap, pInputLayout, file_name, func_name);
			return true;
		}
		else
		{
			//Targeted file is not exists.
			return false;
		}
	}
}

bool CShaderManager::GetPixelShader(ID3D11Device* device, const char* file_name, const char* func_name, ID3D11PixelShader*& pPixelShader)
{
	ID3D11PixelShader* PS;

	if (CheckWhetherPixelShaderIsAbleToChatch(file_name, func_name))
	{
		//Exists.
		PS = ShaderList_Get<ID3D11PixelShader*>(m_pPixelShaderMap, file_name, func_name);
		pPixelShader = PS;
		return true;
	}
	else
	{
		//Not Exists.
		if (NShaderCast::CreatePixelShader(device, (char*)file_name, func_name, pPixelShader))
		{
			PS = pPixelShader;
			ShaderList_Add < ID3D11PixelShader*> (m_pPixelShaderMap, PS, file_name, func_name);
			return true;
		}
		else
		{
			//Targeted file is not exists.
			return false;
		}
	}
}

bool CShaderManager::GetGeometryShader(ID3D11Device* device, const char* file_name, const char* func_name, ID3D11GeometryShader*& pGeometryShader)
{
	ID3D11GeometryShader* GS;

	if (CheckWhetherGeometryShaderIsAbleToChatch(file_name, func_name))
	{
		//Exists.
		GS = ShaderList_Get<ID3D11GeometryShader*>(m_pGeometryShaderMap, file_name, func_name);
		pGeometryShader = GS;
		return true;
	}
	else
	{
		//Not Exists.
		if (NShaderCast::CreateGeometryShader(device, (char*)file_name, func_name, pGeometryShader))
		{
			GS = pGeometryShader;
			ShaderList_Add < ID3D11GeometryShader*>(m_pGeometryShaderMap, GS, file_name, func_name);
			return true;
		}
		else
		{
			//Targeted file is not exists.
			return false;
		}
	}
}

bool CShaderManager::GetHullShader(ID3D11Device* device, const char* file_name, const char* func_name, ID3D11HullShader*& pHullShader)
{
	ID3D11HullShader* HS;

	if (CheckWhetherHullShaderIsAbleToChatch(file_name, func_name))
	{
		//Exists.
		HS = ShaderList_Get<ID3D11HullShader*>(m_pHullShaderMap, file_name, func_name);
		pHullShader = HS;
		return true;
	}
	else
	{
		//Not Exists.
		if (NShaderCast::CreateHullShader(device, (char*)file_name, func_name, pHullShader))
		{
			HS = pHullShader;
			ShaderList_Add < ID3D11HullShader*>(m_pHullShaderMap, HS, file_name, func_name);
			return true;
		}
		else
		{
			//Targeted file is not exists.
			return false;
		}
	}
}

bool CShaderManager::GetDomainShader(ID3D11Device* device, const char* file_name, const char* func_name, ID3D11DomainShader*& pDomainShader)
{
	ID3D11DomainShader* DS;

	if (CheckWhetherDomainShaderIsAbleToChatch(file_name, func_name))
	{
		//Exists.
		DS = ShaderList_Get<ID3D11DomainShader*>(m_pDomainShaderMap, file_name, func_name);
		pDomainShader = DS;
		return true;
	}
	else
	{
		//Not Exists.
		if (NShaderCast::CreateDomainShader(device, (char*)file_name, func_name, pDomainShader))
		{
			DS = pDomainShader;
			ShaderList_Add < ID3D11DomainShader*>(m_pDomainShaderMap, DS, file_name, func_name);
			return true;
		}
		else
		{
			//Targeted file is not exists.
			return false;
		}
	}
}