#include "CameraClass.h"

////////////////////////////////////////////////////////////
//		Constructors And Destructors
//

CameraClass::CameraClass()
{

}

CameraClass::~CameraClass()
{

}

////////////////////////////////////////////////////////////
//		Public Functions
//
void CameraClass::MoveCamera(float move, float strafe)
{
	XMMATRIX RotMatrix = XMMatrixRotationRollPitchYaw(m_RotateX, m_RotateY, m_RotateZ);
	XMVECTOR PosVector = XMLoadFloat3(&m_Pos);
	XMVECTOR FrontDirection = XMLoadFloat3(&m_LookAtFrontDirection);
	XMVECTOR LeftDirection = XMLoadFloat3(&m_LookAtLeftDirection);

	FrontDirection = XMVector3TransformCoord(FrontDirection, RotMatrix);
	LeftDirection = XMVector3TransformCoord(LeftDirection, RotMatrix);
	PosVector += move * FrontDirection;
	PosVector += strafe * LeftDirection;

	XMStoreFloat3(&m_Pos, PosVector);
}

void CameraClass::RotateCamera(float RotateX, float RotateY, float RotateZ)
{
	m_RotateX += RotateX * 0.0174532925f; // (rotX*180°)/pi
	m_RotateY += RotateY * 0.0174532925f;
	m_RotateZ += RotateZ * 0.0174532925f;
}

void CameraClass::UpdateCamera()
{
	XMMATRIX RotMatrix = XMMatrixRotationRollPitchYaw(m_RotateX, m_RotateY, m_RotateZ);
	XMVECTOR LookAt = XMLoadFloat3(&m_LookAt);
	XMVECTOR LookAtBase = XMLoadFloat3(&m_LookAtFrontDirection);
	XMVECTOR UpVector = XMLoadFloat3(&m_UpVector);
	XMVECTOR PosVector = XMLoadFloat3(&m_Pos);
	
	LookAt = XMVector3TransformCoord(LookAtBase, RotMatrix);
	UpVector = XMVector3TransformCoord(UpVector, RotMatrix);
	LookAt += PosVector;

	m_camera = XMMatrixLookAtLH(PosVector, LookAt, UpVector);
}

void CameraClass::SetCameraDefaultValue(XMFLOAT3 EyePos, XMFLOAT3 LookAt, XMFLOAT3 UpVector)
{
	XMVECTOR Eye = XMLoadFloat3(&EyePos);
	XMVECTOR Look = XMLoadFloat3(&LookAt);
	XMVECTOR LookAtDirection = Look - Eye;
	XMVECTOR UpDirection = XMLoadFloat3(&UpVector);

	m_RotateX = 0.f;
	m_RotateY = 0.f;
	m_RotateZ = 0.f;
	m_Pos = EyePos;

	XMStoreFloat3(&m_LookAt, Look);

	//Save original front and left direction for later use
	XMStoreFloat3(&m_LookAtFrontDirection, XMVector3Normalize(LookAtDirection));
	XMStoreFloat3(&m_LookAtLeftDirection, XMVector3Normalize(XMVector3Cross(LookAtDirection, UpDirection)));

	m_UpVector = UpVector;

	m_camera = XMMatrixLookAtLH(Eye, Look, UpDirection);
}

CXMMATRIX CameraClass::GetCamera()
{
	return m_camera;
}

