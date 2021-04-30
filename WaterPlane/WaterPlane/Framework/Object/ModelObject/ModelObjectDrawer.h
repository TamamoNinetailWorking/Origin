#pragma once

#include "../../Drawer/3D/D3DDrawer3D.h"

class CXFile;
class CShaderSet;

class CModelObjectDrawer : public CD3DDrawer3D
{
private:

	struct ConstantBufferWorld
	{
		D3DXMATRIX world;
		D3DXVECTOR4 diffuse;
	};

	enum class ConstantBufferType
	{
		World = 1,

		Max,
	};

	enum class ShaderType
	{
		Default = 0,

		Max,
		NotUse,
	};

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	LPDIRECT3DDEVICE9 m_nines;

	CXFile* m_pMesh;
	CShaderSet* m_pShader;

	bool meshInit();
	bool meshInit(const char* _modelFile);
	bool shaderInit();
	bool vertexshaderInit(const char* _file, const char* _func);
	bool pixelshaderInit(const char* _file, const char* _func);
	bool constantBufferInit();
	bool samplerInit();

	void updateBuffer(const D3DXMATRIX& matrix);
	void setBufferToShaders(ConstantBufferType type);
	void setVerticesAndIndex();

public:

	CModelObjectDrawer() {};
	virtual ~CModelObjectDrawer();

	bool Init(
		ID3D11Device* dev, 
		ID3D11DeviceContext* devCon, 
		LPDIRECT3DDEVICE9 ni);

	bool FileInit(
		ID3D11Device* dev, 
		ID3D11DeviceContext* devCon, 
		LPDIRECT3DDEVICE9 ni,
		const char* _modelFile, 
		const char* _VertexShaderFile, 
		const char* _VertexFunction, 
		const char* _PixelShaderFile, 
		const char* _PixelFunction);

	void Draw(const D3DXMATRIX& matrix) override;

	void Exit();

};