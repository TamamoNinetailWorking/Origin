#pragma once

#include "../../Drawer/3D/D3DDrawer3D.h"


class CXFile;
class CShaderSet;

class CSkydomeDrawer : public CD3DDrawer3D
{
private:

	struct ConstantBufferWorld
	{
		D3DXMATRIX world;
	};

	enum class ConstantBufferType
	{
		World = 1,//0 is ViewAndProjectionBuffer.

		Max,
	};

	enum class ShaderType
	{
		Default = 0,
		CubeMap,

		Max,
		NotUse,
	};

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	LPDIRECT3DDEVICE9 m_nines;

	CXFile * m_pMesh;
	CShaderSet* m_pShader;

	bool meshInit();
	bool shaderInit();
	bool constantBufferInit();
	bool samplerInit();

	void updateBuffer(const D3DXMATRIX& matrix);
	void setBufferToShaders(ConstantBufferType type);
	void setVerticesAndIndex();

public:

	CSkydomeDrawer() {};
	virtual ~CSkydomeDrawer();

	bool Init(ID3D11Device* dev, ID3D11DeviceContext* devCon,LPDIRECT3DDEVICE9 ni);

	void Draw(const D3DXMATRIX& matrix) override;

	void DrawAsCubeMap(const D3DXMATRIX& matrix);

	void Exit();

};