#include "CameraObject.h"
#include "../../System/Utility/MathUtility.h"
#include "../../System/Input/CDirectInput.h"
#include "../../System/Utility/MathUtility.h"

void CCameraObject::SetNear(float nearclip)
{
	m_fnear = nearclip;
}

void CCameraObject::SetFar(float farclip)
{
	m_ffar = farclip;
}

void CCameraObject::SetFov(float fov)
{
	m_ffield_of_view = fov;
}

void CCameraObject::SetAspect(float width, float height)
{
	m_faspect = width / height;
}

void CCameraObject::SetEye(const D3DXVECTOR3& eye)
{
	m_vEye = eye;
}

void CCameraObject::SetLookat(const D3DXVECTOR3& lookat)
{
	m_vLookat = lookat;
}

void CCameraObject::SetUp(const D3DXVECTOR3& up)
{
	m_vUp = up;
}

void CCameraObject::SetProjection(float nearclip, float farclip, float fov, float width, float height)
{
	SetNear(nearclip);
	SetFar(farclip);
	SetFov(fov);
	SetAspect(width, height);
	CreateProjectionMatrix();
}

void CCameraObject::SetCamera(const D3DXVECTOR3& eye, const D3DXVECTOR3& lookat, const D3DXVECTOR3& up)
{
	SetEye(eye);
	SetLookat(lookat);
	SetUp(up);
	CreateCameraTransform();
}

void CCameraObject::Init(float nearclip, float farclip, float fov, float width, float height, const D3DXVECTOR3& eye, const D3DXVECTOR3& lookat, const D3DXVECTOR3& up)
{
	SetProjection(nearclip, farclip, fov, width, height);
	SetCamera(eye, lookat, up);
}

void CCameraObject::CreateCameraTransform()
{
	D3DXMATRIX camera;
	D3DXMatrixLookAtLH(&camera, &m_vEye, &m_vLookat, &m_vUp);
	m_matView = camera;
	D3DXQUATERNION qt;
	qt = MathUtility::CreateQuaternionFromVector(m_vLookat, m_vUp);
	
	D3DXVECTOR3 pos = m_vEye;

	m_cTransform.SetPosition(pos);
	m_cTransform.SetRotation(qt);
}

void CCameraObject::CreateProjectionMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_matProjection, m_ffield_of_view, m_faspect, m_fnear, m_ffar);
}

void CCameraObject::UpdateBuffer(ID3D11DeviceContext* context)
{
	ConstantBuffer cb;
	D3DXMATRIX view = GetViewMatrix();
	D3DXMatrixTranspose(&cb.View, &view);
	D3DXMATRIX proj = GetProjectionMatrix();
	D3DXMatrixTranspose(&cb.Projection, &proj);
	cb.CameraPos = D3DXVECTOR4(GetEye(), 1.0f);
	context->UpdateSubresource(m_pViewProjectionBuffer, 0, NULL,&cb, 0, 0);
}

#include <iostream>

void CCameraObject::Update()
{
	D3DXVECTOR3 trans = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 angle = D3DXVECTOR3(0,0,0);
	const float mover = 1.0f;
	const float angler = 5.0f;

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
	{
		trans.z -= mover;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
	{
		trans.z += mover;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
	{
		trans.x += mover;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		trans.x -=mover;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_R))
	{
		trans.y -= mover;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F))
	{
		trans.y += mover;
	}


	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP))
	{
		angle.y += angler;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN))
	{
		angle.y -= angler;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT))
	{
		angle.x += angler;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT))
	{
		angle.x -= angler;
	}
	
	SetLookat(GetLookat() + trans);
	SetEye(GetEye() + trans);
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXMATRIX total = GetViewMatrix();
	MathUtility::MakeWorldMatrix(mat, total, angle, trans);
	m_matView = total;

	D3DXQUATERNION qt;
	D3DXQUATERNION qt_total = m_cTransform.GetRotation();
	MathUtility::MakeNextQuaternion(qt, qt_total, angle);
	m_cTransform.SetRotation(qt_total);
	m_cTransform.SetScaling(D3DXVECTOR3(1, 1, 1));
	m_cTransform.SetPosition(GetEye());

}

void CCameraObject::Update(ID3D11DeviceContext* context)
{
	Update();
	UpdateBuffer(context);
}