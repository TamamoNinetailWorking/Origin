#pragma once

#include <d3dx9.h>
#include <d3d11.h>

#include "../Object/GameObject/D3DGameObject.h"

//using namespace DirectX;

class CCamera
{
private:

	D3DXMATRIX m_Projection;
	D3DXMATRIX m_Camera;

	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_Lookat;
	D3DXVECTOR3 m_Up;

	float m_near;
	float m_far;
	float m_Aspect;
	float m_Fov;

	CCamera() {};

	void CreateCameraMatrix();
	void CreateProjectionMatrix();

public:

	//コピーを禁止するための=delete定義
	//=deleteが恐らく使用禁止の意味になると思われる
	CCamera(const CCamera&) = delete;
	CCamera& operator= (const CCamera&) = delete;
	//右辺参照を禁止
	CCamera(CCamera&&) = delete;
	CCamera& operator=(CCamera&&) = delete;

	static CCamera* GetInstance()
	{
		static CCamera instance;
		return &instance;
	}

	void Init(float nearclip, float farclip, float fov, float width,float height,const D3DXVECTOR3& eye,const D3DXVECTOR3& lookat,const D3DXVECTOR3& up);

	void SetNear(float nearclip);
	void SetFar(float farclip);
	void SetFov(float fov);
	void SetAspect(float width, float height);

	void SetEye(const D3DXVECTOR3& eye);
	void SetLookat(const D3DXVECTOR3& lookat);
	void SetUp(const D3DXVECTOR3& up);

	void SetProjection(float nearclip, float farclip, float fov, float width,float height);
	void SetCamera(const D3DXVECTOR3& eye, const D3DXVECTOR3& lookat, const D3DXVECTOR3& up);

	const D3DXMATRIX& GetGameraMatrix() const 
	{ return m_Camera; };

	const D3DXMATRIX& GetProjectionMatrix() const 
	{ return m_Projection; };
	
	float GetFov() const { return m_Fov; };

	const D3DXVECTOR3& GetEye() const{ return m_Eye; };
	const D3DXVECTOR3& GetLookat() const { return m_Lookat; };
	const D3DXVECTOR3& GetUp() const { return m_Up; };
};