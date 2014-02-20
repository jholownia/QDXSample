/*
================================
 light.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma warning( disable : 4005 )

#include <D3DX10math.h>

/*
================================
 
 Light

 Class to hold light information.
 
================================
*/
class Light
{
public:
	Light();
	virtual ~Light();

	void setAmbientColor(float red, float green, float blue, float alpha);
	void setDiffuseColor(float red, float green, float blue, float alpha);		
	void setPosition(float x, float y, float z);
	void setLookAt(float x, float y, float z);
		
	void setDirection(float x, float y, float z);

	void setSpecularColor(float red, float green, float blue, float alpha);
	void setSpecularPower(float power);
	void setSpecularIntensity(float intensity);

	D3DXVECTOR4 getAmbientColor() const;
	D3DXVECTOR4 getDiffuseColor() const;
	D3DXVECTOR3 getPosition() const;

	D3DXVECTOR3 getDirection() const;

	D3DXVECTOR4 getSpecularColor() const;
	float getSpecularPower() const;
	float getSpecularIntensity() const;

	void generateViewMatrix();
	void generateProjectionMatrix(float screenDepth, float screenNear);

	void getViewMatrix(D3DXMATRIX&);
	void getProjectionMatrix(D3DXMATRIX&);

private:
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_lookAt;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;

	D3DXVECTOR3 m_direction;

	D3DXVECTOR4 m_specularColor;
	float m_specularPower;
	float m_specularIntensity;
};

