/*
================================
 camera.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma warning( disable : 4005 )

#include "moveable.h"


/*
================================
 
 Camera

 A simple camera class. Holds a view matrix and reflection view matrix.
 
================================
*/
class Camera : public Moveable
{
public:
	Camera();
	~Camera();	

	void render();
	void getViewMatrix(D3DXMATRIX& viewMatrix) const;

	void renderReflection(float);
	D3DXMATRIX getReflectionViewMatrix() const;

protected:	
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_reflectionMatrix;
};

