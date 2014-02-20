/*
================================
 camera.cpp
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "camera.h"


/*
================
 Camera::Camera
================
*/
Camera::Camera()
{

}

/*
================
 Camera::~Camera
================
*/
Camera::~Camera()
{

}

/*
================
 Camera::render

 Creates view matrix based on position and rotation.
================
*/
void Camera::render()
{
	
	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;	

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_x;
	position.y = m_y;
	position.z = m_z;
			
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// radians
	float pitch = m_rx * 0.0174532925f;
	float yaw = m_ry * 0.0174532925f;
	float roll = m_rz * 0.0174532925f;

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Create view matrix
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	
}

/*
================
 Camera::getViewMatrix
================
*/
void Camera::getViewMatrix( D3DXMATRIX& viewMatrix ) const
{
	viewMatrix = m_viewMatrix;
}

/*
================
 Camera::renderReflection

 This method builds a reflection view matrix. It takes the height of the object to be reflected as a parameter
 and uses it to invert the y position of the reflection.
================
*/
void Camera::renderReflection(float height)
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;	

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup camera position with inverted Y
	position.x = m_x;
	position.y = -m_y + (height * 2.0f);
	position.z = m_z;

	
	// Calculate rotation
	float radians = m_ry * 0.0174532925f;

	// Where the camera is looking
	lookAt.x = sinf(radians) + m_x;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + m_z;
	
	// Create reflection matrix
	D3DXMatrixLookAtLH(&m_reflectionMatrix, &position, &lookAt, &up);	
}

/*
================
 Camera::getReflectionViewMatrix
================
*/
D3DXMATRIX Camera::getReflectionViewMatrix() const
{
	return m_reflectionMatrix;
}
