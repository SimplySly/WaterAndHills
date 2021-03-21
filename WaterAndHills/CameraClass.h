#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include "InclFiles.h"

class CameraClass
{
protected:
	XMMATRIX m_camera;
	XMFLOAT3 m_Pos;
	float m_RotateX, m_RotateY, m_RotateZ;
	XMFLOAT3 m_LookAt, m_LookAtFrontDirection, m_LookAtLeftDirection;  
	XMFLOAT3 m_UpVector;
public:
	CameraClass();
	~CameraClass();

	void RotateCamera(float RotateX, float RotateY, float RotateZ);
	void MoveCamera(float Move, float Strafe);
	void UpdateCamera();

	void SetCameraDefaultValue(XMFLOAT3 EyePos, XMFLOAT3 LookAt, XMFLOAT3 UpVector);
	CXMMATRIX GetCamera();
};

#endif //_CAMERACLASS_H_