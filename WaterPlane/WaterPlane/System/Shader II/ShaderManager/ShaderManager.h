#pragma once

#include <map>

#include "ShaderManagementSpace.h"
#include "../../Utility/NonCopyable.h"

//Singleton
class CShaderManager : public NonCopyable
{
private:

	NShaderManager::VertexShaderMap m_pVertexShaderMap;
	NShaderManager::InputLayoutMap m_pInputLayoutMap;
	NShaderManager::PixelShaderMap m_pPixelShaderMap;
	NShaderManager::GeometryShaderMap m_pGeometryShaderMap;
	NShaderManager::HullShaderMap m_pHullShaderMap;
	NShaderManager::DomainShaderMap m_pDomainShaderMap;

	void AllClear();

	CShaderManager();
	virtual ~CShaderManager();

public:

	static CShaderManager& GetInstance()
	{
		static CShaderManager instance;
		return instance;
	}

	bool CheckWhetherVertexShaderIsAbleToChatch(
		const char* file_name,
		const char* func_name);

	bool CheckWhetherInputLayoutIsAbleToChatch(
		const char* file_name,
		const char* func_name);

	bool CheckWhetherPixelShaderIsAbleToChatch(
		const char* file_name,
		const char* func_name);

	bool CheckWhetherGeometryShaderIsAbleToChatch(
		const char* file_name,
		const char* func_name);

	bool CheckWhetherHullShaderIsAbleToChatch(
		const char* file_name,
		const char* func_name);

	bool CheckWhetherDomainShaderIsAbleToChatch(
		const char* file_name,
		const char* func_name);

	bool GetVertexShader(
		ID3D11Device* device,
		const char* file_name, 
		const char* func_name, 
		ID3D11VertexShader*& pVertexShader, 
		ID3D11InputLayout*& pInputLayout);

	bool GetPixelShader(
		ID3D11Device* device,
		const char* file_name, 
		const char* func_name, 
		ID3D11PixelShader*& pPixelShader);

	bool GetGeometryShader(
		ID3D11Device* device, 
		const char* file_name, 
		const char* func_name, 
		ID3D11GeometryShader*& pGeometryShader);

	bool GetHullShader(
		ID3D11Device* device,
		const char* file_name,
		const char* func_name,
		ID3D11HullShader*& pHullShader);

	bool GetDomainShader(
		ID3D11Device* device,
		const char* file_name,
		const char* func_name,
		ID3D11DomainShader*& pDomainShader);


};