#pragma once

#include <d3dx9.h>
#include <d3d11.h>

#include "../Object/GameObject/D3DGameObject.h"

class CCameraObject : public CD3DGameObject
{
public:

	struct ConstantBuffer
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
		D3DXVECTOR4 CameraPos;
	};

private:

	D3DXMATRIX m_matProjection;
	D3DXMATRIX m_matView;
	
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vLookat;
	D3DXVECTOR3 m_vUp;

	float m_fnear;
	float m_ffar;
	float m_faspect;
	float m_ffield_of_view;

	ID3D11Buffer* m_pViewProjectionBuffer;
	void CreateCameraTransform();
	void CreateProjectionMatrix();

public:

	CCameraObject() : CD3DGameObject(std::string("Camera"))
	{
		//CGameObject(std::string("Camera"));
	}

	virtual ~CCameraObject() override {};

	void UpdateBuffer(ID3D11DeviceContext* context);

	void Update()override;

	void Update(ID3D11DeviceContext* context);

	void Init(float nearclip, float farclip, float fov, float width, float height, const D3DXVECTOR3& eye, const D3DXVECTOR3& lookat, const D3DXVECTOR3& up);

	void SetNear(float nearclip);
	void SetFar(float farclip);
	void SetFov(float fov);
	void SetAspect(float width, float height);

	void SetEye(const D3DXVECTOR3& eye);
	void SetLookat(const D3DXVECTOR3& lookat);
	void SetUp(const D3DXVECTOR3& up);

	void SetProjection(float nearclip, float farclip, float fov, float width, float height);
	void SetCamera(const D3DXVECTOR3& eye, const D3DXVECTOR3& lookat, const D3DXVECTOR3& up);

	D3DXMATRIX GetViewMatrix() const
	{
		return m_matView;
	}

	D3DXMATRIX GetCameraMatrix()
	{
		return GetTransform().GetMatrix();
	};

	D3DXMATRIX GetTransposeCameraMatrix()
	{
		return GetTransform().GetMatrixTranspose();
	}

	const D3DXMATRIX GetProjectionMatrix() const
	{
		return m_matProjection;
	};

	float GetFov() const { return m_ffield_of_view; };

	const D3DXVECTOR3& GetEye() const { return m_vEye; };
	const D3DXVECTOR3& GetLookat() const { return m_vLookat; };
	const D3DXVECTOR3& GetUp() const { return m_vUp; };

	ID3D11Buffer*& GetCameraBuffer(){ return m_pViewProjectionBuffer; };

};