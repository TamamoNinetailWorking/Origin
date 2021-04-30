#pragma once

#include "../../Drawer/3D/D3DDrawer3D.h"


class CXFile;
class CShaderSet;

class CWaterPlaneDrawer : public CD3DDrawer3D
{
private:
	
	//Buffer
	//------------------------------------------
	struct ConstantBufferWorld
	{
		D3DXMATRIX world;
		D3DXVECTOR4 diffuse;
	};

	struct DistortionBuffer
	{
		float Time;
		float Speed;
		float Power;
		float Distortion;
		float RefractiveRatio;
		D3DXVECTOR3 padding;
	};

	struct TessellationBuffer
	{
		float TessFactor;
		float InsideTessFactor;
		D3DXVECTOR2 padding;
	};

	struct WaveBuffer
	{
		D3DXVECTOR3 WaveCenter;
		float padding;

		float Amplitude;
		float WaveLength;
		float WaveCycle;
		float Time;
		float WaveEdge;
		D3DXVECTOR3 padding2;
	};
	//------------------------------------------

	//Vertices info
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 uv;
	};

	//Used index of 'm_pBufferContainer'
	enum class ConstantBufferType
	{
		World = 1,//0 is ViewAndProjectionBuffer.
		Distortion = 2,
		TessBuffer = 3,
		WaveBuffer = 4,

		Max,
	};

	//Used index of ShaderSetType
	enum class ShaderType
	{
		Default = 0,

		Max,
		NotUse,
	};

	//Used index of 'm_pSRVContainer'
	enum class TextureType
	{
		Default = 0,
		Normal = 1,

		Max,
	};

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	CShaderSet* m_pShader;//ShaderPackage

	//Water Surf Parameter
	float m_fdistortion;
	float m_fspeed;
	float m_fpower;
	float m_ftime;
	float m_frefractratio;
	D3DXVECTOR4 m_vdiffuse;

	//Subdivision Coeficient
	float m_ftessFactor;
	float m_finsideTessFactor;

	//Wave Coeficient
	D3DXVECTOR3 m_vwaveCenter;
	float m_famplitude;
	float m_fwavelength;
	float m_fwavecycle;
	float m_fwavetime;
	float m_fwaveedge;


	//-- Functions --

	bool meshInit();
	bool decalInit();
	bool shaderInit();
	bool constantBufferInit();
	bool samplerInit();

	void updateWorldBuffer(const D3DXMATRIX& matrix);
	void updateDistortionWorld(const float dist, const float speed, const float power, const float time, const float refract);
	void updateTessellationBuffer(const float tess, const float in_tess);
	void updateWaveBuffer(const float amplitude, const float wavelength, const float wavecycle, const float waveedge);
	void setBufferToShaders(ConstantBufferType type);
	void setVerticesAndIndex();

public:

	CWaterPlaneDrawer() {};
	virtual ~CWaterPlaneDrawer();

	bool Init(ID3D11Device* dev, ID3D11DeviceContext* devCon);
	
	void SetParameter(const float dist, const float speed, const float pow, const float refract);
	void SetTessCoeficient(const float tess, const float in_tess);
	void SetDiffuse(const float r, const float g, const float b, const float a);
	void SetWaveParameter(const float amplitude, const float length, const float cycle, const float edge);

	void Draw(const D3DXMATRIX& matrix) override;

	void Exit();

};