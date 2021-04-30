#pragma once

#include <d3d11.h>
#include <vector>

class CShaderSet
{
private:

	//VertexShaderSet
	std::vector<ID3D11VertexShader*> m_vVertexShader;
	//InputLayoutSet
	std::vector<ID3D11InputLayout*> m_vInputLayout;
	//PixelShaderSet
	std::vector<ID3D11PixelShader*> m_vPixelShader;
	//GeometryShaderSet
	std::vector<ID3D11GeometryShader*> m_vGeometryShader;
	//HullShaderSet
	std::vector<ID3D11HullShader*> m_vHullShader;
	//DomainShaderSet
	std::vector<ID3D11DomainShader*> m_vDomainShader;


public:

	//Creator
	bool CreateVertexShader(
		ID3D11Device*& device, 
		const char* file_name, 
		const char* func_name);

	bool CreatePixelShader(
		ID3D11Device*& device, 
		const char* file_name, 
		const char* func_name);

	bool CreateGeometryShader(
		ID3D11Device*& device, 
		const char* file_name, 
		const char* func_name);

	bool CreateHullShader(
		ID3D11Device*& device,
		const char* file_name,
		const char* func_name);

	bool CreateDomainShader(
		ID3D11Device*& device,
		const char* file_name,
		const char* func_name);

	//Accessor
	ID3D11VertexShader* GetVertexShader(UINT index);
	ID3D11InputLayout* GetInputLayout(UINT index);
	ID3D11PixelShader* GetPixelShader(UINT index);
	ID3D11GeometryShader* GetGeometryShader(UINT index);
	ID3D11HullShader* GetHullShader(UINT index);
	ID3D11DomainShader* GetDomainShader(UINT index);
	
	void SetVertexShader(ID3D11DeviceContext*& deviceContext,UINT index);
	void SetPixelShader(ID3D11DeviceContext*& deviceContext, UINT index);
	void SetGeometryShader(ID3D11DeviceContext*& deviceContext, UINT index);
	void SetHullShader(ID3D11DeviceContext*& deviceContext, UINT index);
	void SetDomainShader(ID3D11DeviceContext*& deviceContext, UINT index);

	void Exit();

};