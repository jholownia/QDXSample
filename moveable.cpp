/*
================================
 moveable.cpp

 Created on: 29 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "moveable.h"

/*
================
 Moveable::Moveable
================
*/
Moveable::Moveable(void) :
	m_x  (0.0f),
	m_y  (0.0f),
	m_z  (0.0f),
	m_rx (0.0f),
	m_ry (0.0f),
	m_rz (0.0f)	
{

}

/*
================
 Moveable::~Moveable
================
*/
Moveable::~Moveable(void)
{

}

/*
================
 Moveable::setPosition
================
*/
void Moveable::setPosition( float x, float y, float z )
{
	m_x = x;
	m_y = y;
	m_z = z;
}

/*
================
 Moveable::setRotation
================
*/
void Moveable::setRotation( float rx, float ry, float rz )
{
	m_rx = rx;
	m_ry = ry;
	m_rz = rz;
}

/*
================
 Moveable::getPosition
================
*/
D3DXVECTOR3 Moveable::getPosition() const
{
	return D3DXVECTOR3(m_x, m_y, m_z);
}

/*
================
 Moveable::getRotation
================
*/
D3DXVECTOR3 Moveable::getRotation() const
{
	return D3DXVECTOR3(m_rx, m_ry, m_rz);
}
