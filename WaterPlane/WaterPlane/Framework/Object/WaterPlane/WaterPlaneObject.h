#pragma once

#include "../GameObject/D3DGameObject.h"

class CWaterPlaneDrawer;
struct ID3D11ShaderResourceView;

class CWaterPlaneObject : public CD3DGameObject
{
private:

	CWaterPlaneDrawer * m_cDrawer;
	bool m_bStartFlag = false;

	void updateProcess();

public:

	void Start() override;

	void Update() override;

	void Draw() override;

	void Exit() override;

	void SetSurfaceParameter(const float dist, const float speed, const float pow, const float refract);

	void SetTessCoeficient(const float tess, const float in_tess);

	void SetWaveParameter(const float amplitude, const float length, const float cycle, const float edge);

	void SetDiffuse(const float r, const float g, const float b, const float a);

};