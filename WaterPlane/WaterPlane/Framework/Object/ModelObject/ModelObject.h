#pragma once

#include "../GameObject/D3DGameObject.h"

class CModelObjectDrawer;

class CModelObject : public CD3DGameObject
{
private:

	CModelObjectDrawer * m_cDrawer;
	bool m_bStartFlag = false;

	void updateProcess();

public:

	void Start() override;

	void Update() override;

	void Draw() override;

	void Exit() override;

};