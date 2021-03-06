////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}
// z축 이동
void CameraClass::MoveZ(float direct)
{
	SetPosition(m_positionX, m_positionY, m_positionZ+direct);
	return;
}

// z축 이동
void CameraClass::MoveX(float direct)
{
	SetPosition(m_positionX + direct, m_positionY, m_positionZ);
	return;
}

// z축 이동
void CameraClass::MoveY(float direct)
{
	SetPosition(m_positionX, m_positionY+ direct, m_positionZ );
	return;
}

// Y축 회전
void CameraClass::RotateY(float direct) 
{
	SetRotation(m_rotationX, m_rotationY+direct, m_rotationZ);
	return;
}

//X축 회전
void CameraClass::RotateX(float direct)
{
	SetRotation(m_rotationX + direct, m_rotationY, m_rotationZ);
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

D3DXVECTOR3 CameraClass::GetLookAt()
{
	return lookAt;
}

void CameraClass::setLookAt(D3DXVECTOR3 current)
{
	lookAt = current;
}

void CameraClass::LookAtChange(int x, int y)
{
	lookAt.x += (float)x/100;
	lookAt.y += (float)y/100;
	lookAt.z = 1.0f;
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = -0.2f;
	lookAt.z = 1.0f;

	setLookAt(lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}



void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}