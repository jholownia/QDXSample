/*
================================
 light.cpp

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "light.h"

/*
================
 Light::Light
================
*/
Light::Light(void)
{

}

/*
================
 Light::~Light
================
*/
Light::~Light(void)
{

}

/*
================
 Light::setDiffuseColor
================
*/
void Light::setDiffuseColor( float red, float green, float blue, float alpha )
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
}

/*
================
 Light::getDiffuseColor
================
*/
D3DXVECTOR4 Light::getDiffuseColor() const
{
	return m_diffuseColor;
}

/*
================
 Light::getAmbientColor
================
*/
D3DXVECTOR4 Light::getAmbientColor() const
{
	return m_ambientColor;
}

/*
================
 Light::setAmbientColor
================
*/
void Light::setAmbientColor( float red, float green, float blue, float alpha )
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::setPosition( float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
}

void Light::setLookAt( float x, float y, float z)
{
	m_lookAt.x = x;
	m_lookAt.y = y;
	m_lookAt.z = z;
}

void Light::generateViewMatrix()
{
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_lookAt, &up);
}

void Light::generateProjectionMatrix(float screenDepth, float screenNear)
{
	float fov = (float)D3DX_PI * 0.5f;
	float screenAspect = 1.0f;

	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fov, screenAspect, screenNear, screenDepth);
}

void Light::getViewMatrix( D3DXMATRIX& viewMatrix )
{
	viewMatrix = m_viewMatrix;
}

void Light::getProjectionMatrix( D3DXMATRIX& projectionMatrix )
{
	projectionMatrix = m_projectionMatrix;
}

D3DXVECTOR3 Light::getPosition() const
{
	return m_position;
}

void Light::setDirection( float x, float y, float z )
{
	m_direction = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 Light::getDirection() const
{
	return m_direction;
}

void Light::setSpecularColor( float red, float green, float blue, float alpha)
{
	m_specularColor = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::setSpecularPower( float power )
{
	m_specularPower = power;
}

D3DXVECTOR4 Light::getSpecularColor() const
{
	return m_specularColor;
}

float Light::getSpecularPower() const
{
	return m_specularPower;
}

void Light::setSpecularIntensity( float intensity )
{
	m_specularIntensity = intensity;
}

float Light::getSpecularIntensity() const
{
	return m_specularIntensity;
}


