#include "Camera.h"

void CCamera::SetNear(float nearclip)
{
	m_near = nearclip;
}

void CCamera::SetFar(float farclip)
{
	m_far = farclip;
}

void CCamera::SetFov(float fov)
{
	m_Fov = fov;
}

void CCamera::SetAspect(float width, float height)
{
	m_Aspect = width / height;
}

void CCamera::SetEye(const D3DXVECTOR3& eye)
{
	m_Eye = eye;
}

void CCamera::SetLookat(const D3DXVECTOR3& lookat)
{
	m_Lookat = lookat;
}

void CCamera::SetUp(const D3DXVECTOR3& up)
{
	m_Up = up;
}

void CCamera::CreateCameraMatrix()
{
	D3DXMatrixLookAtLH(&m_Camera,&m_Eye, &m_Lookat, &m_Up);
}

void CCamera::CreateProjectionMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_Projection,m_Fov, m_Aspect, m_near, m_far);
}

void CCamera::SetProjection(float nearclip, float farclip, float fov, float width, float height)
{
	SetNear(nearclip);
	SetFar(farclip);
	SetFov(fov);
	SetAspect(width, height);
	CreateProjectionMatrix();
}

void CCamera::SetCamera(const D3DXVECTOR3& eye, const D3DXVECTOR3& lookat, const D3DXVECTOR3& up)
{
	SetEye(eye);
	SetLookat(lookat);
	SetUp(up);
	CreateCameraMatrix();
}

void CCamera::Init(float nearclip, float farclip, float fov, float width, float height, const D3DXVECTOR3& eye, const D3DXVECTOR3& lookat, const D3DXVECTOR3& up)
{
	SetProjection(nearclip, farclip, fov, width, height);
	SetCamera(eye, lookat, up);
}