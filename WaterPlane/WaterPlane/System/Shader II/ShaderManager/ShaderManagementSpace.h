#pragma once

#include <map>
#include <string>
#include <d3d11.h>

namespace NShaderManager
{
	struct KeyStruct
	{
		std::string FileName;
		std::string FuncName;

		KeyStruct() { };
		KeyStruct(const char* file, const char* func)
		{
			FileName = std::string(file);
			FuncName = std::string(func);
		}
		bool operator< (const KeyStruct& value) const
		{
			return std::tie(FileName, FuncName) < std::tie(value.FileName, value.FuncName);
		}
	};

	struct VertexShaderAndInputLayout
	{
		ID3D11VertexShader* vertex_shader;
		ID3D11InputLayout* input_layout;
		VertexShaderAndInputLayout()
			:vertex_shader(nullptr),input_layout(nullptr){}
		~VertexShaderAndInputLayout()
		{
			if (vertex_shader)
			{
				vertex_shader->Release();
			}
			vertex_shader = nullptr;
			
			if (input_layout)
			{
				input_layout->Release();
			}
			input_layout = nullptr;
		}
	};

	//typedef std::map<KeyStruct, VertexShaderAndInputLayout> VertexShaderMap;
	typedef std::map<KeyStruct, ID3D11VertexShader*> VertexShaderMap;
	typedef std::map<KeyStruct, ID3D11InputLayout*> InputLayoutMap;
	typedef std::map<KeyStruct, ID3D11PixelShader*> PixelShaderMap;
	typedef std::map<KeyStruct, ID3D11GeometryShader*> GeometryShaderMap;
	typedef std::map<KeyStruct, ID3D11HullShader*> HullShaderMap;
	typedef std::map<KeyStruct, ID3D11DomainShader*> DomainShaderMap;

}