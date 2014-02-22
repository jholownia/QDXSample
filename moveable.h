/*
================================
 moveable.h
 
 Created on: 29 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once
#pragma warning( disable : 4005 )

#include <D3DX10math.h>


/*
================================
 
 Moveable

 Stores position and rotation of a moveable object.
 
================================
*/
class Moveable
{
public:
	Moveable(void);
	virtual ~Moveable(void);

	void setPosition(float x, float y, float z);
	void setRotation(float rx, float ry, float rz);

	D3DXVECTOR3 getPosition() const;
	D3DXVECTOR3 getRotation() const;

protected:
	float m_x;
	float m_y;
	float m_z;
	float m_rx;
	float m_ry;
	float m_rz;
};

