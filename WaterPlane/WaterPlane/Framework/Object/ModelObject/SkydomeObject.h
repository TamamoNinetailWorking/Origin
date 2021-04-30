#pragma once

#include "../GameObject/D3DGameObject.h"
class CSkydomeDrawer;

class CSkydomeObject : public CD3DGameObject
{
private:

	CSkydomeDrawer * m_cDrawer;
	bool m_bStartFlag = false;

	void updateProcess();

public:

	void Start() override;

	void Update() override;

	void Draw() override;

	void DrawAsCubeMap();

	void Exit() override;

	D3DXMATRIX GetMatrix();

};