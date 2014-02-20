/*
================================
 position.h

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "position.h"

/*
================
 Position::Position
================
*/
Position::Position(void) :
	m_posX             (0.0f),
	m_posY             (0.0f),
	m_posZ             (0.0f),
	m_rotationX        (0.0f),
	m_rotationY        (0.0f),
	m_rotationZ        (0.0f),
	m_frameTime        (0.0f),	
	m_leftTurnSpeed    (0.0f),
	m_rightTurnSpeed   (0.0f),
	m_forwardSpeed     (0.0f),
	m_backwardSpeed    (0.0f),
	m_strafeRightSpeed (0.0f),
	m_strafeLeftSpeed  (0.0f)
{
	
}

/*
================
 Position::~Position
================
*/
Position::~Position(void)
{

}

/*
================
 Position::setFrameTime
================
*/
void Position::setFrameTime( float time )
{
	m_frameTime = time;
}

/*
================
 Position::turnLeft
================
*/
void Position::turnLeft( bool keydown )
{
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{	
		m_leftTurnSpeed = m_frameTime * 0.005f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}		
	}

	m_rotationY -= m_leftTurnSpeed;
	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

}

/*
================
 Position::turnRight
================
*/
void Position::turnRight( bool keydown )
{
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{	
		m_rightTurnSpeed = m_frameTime * 0.005f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}		
	}

	m_rotationY += m_rightTurnSpeed;
	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}
}

/*
================
 Position::setPosition
================
*/
void Position::setPosition( float x, float y, float z )
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}

/*
================
 Position::setRotation
================
*/
void Position::setRotation( float x, float y, float z )
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

/*
================
 Position::getPosition
================
*/
void Position::getPosition( float& x, float& y, float& z )
{
	x = m_posX;
	y = m_posY;
	z = m_posZ;
}

/*
================
 Position::getRotation
================
*/
void Position::getRotation( float& x, float& y, float& z )
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
}

/*
================
 Position::moveForward
================
*/
void Position::moveForward( bool keydown )
{
	if (keydown)
	{
		m_forwardSpeed += m_frameTime * 0.001f;

		if (m_forwardSpeed > (m_frameTime * 0.03f))
		{
			m_forwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.0007f;

		if (m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	float radians = m_rotationY * 0.0174532925f;
	float radiansz = m_rotationX * 0.0174532925f;

	m_posX += sinf(radians) * m_forwardSpeed;
	m_posY += -sinf(radiansz) * m_forwardSpeed;
	m_posZ += cosf(radians) * m_forwardSpeed;
}

/*
================
 Position::moveBack
================
*/
void Position::moveBack( bool keydown )
{
	if (keydown)
	{
		m_backwardSpeed += m_frameTime * 0.001f;

		if (m_backwardSpeed > (m_frameTime * 0.03f))
		{
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.0007f;

		if (m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	float radians = m_rotationY * 0.0174532925f;
	float radiansz = m_rotationX * 0.0174532925f;

	m_posX -= sinf(radians) * m_backwardSpeed;
	m_posY -= -sinf(radiansz) * m_backwardSpeed;
	m_posZ -= cosf(radians) * m_backwardSpeed;
}

/*
================
 Position::strafeLeft
================
*/
void Position::strafeLeft( bool keydown )
{
	if (keydown)
	{
		m_strafeLeftSpeed += m_frameTime * 0.001f;

		if (m_strafeLeftSpeed > (m_frameTime * 0.03f))
		{
			m_strafeLeftSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_strafeLeftSpeed -= m_frameTime * 0.0007f;

		if (m_strafeLeftSpeed < 0.0f)
		{
			m_strafeLeftSpeed = 0.0f;
		}
	}

	float radians = (m_rotationY + 90) * 0.0174532925f;

	m_posX -= sinf(radians) * m_strafeLeftSpeed;
	m_posY -= cosf(radians) * m_strafeLeftSpeed;
}

/*
================
 Position::strafeRight
================
*/
void Position::strafeRight( bool keydown )
{
	if(keydown)
	{
		m_strafeRightSpeed += m_frameTime * 0.001f;

		if(m_strafeRightSpeed > (m_frameTime * 0.03f))
		{
			m_strafeRightSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_strafeRightSpeed -= m_frameTime * 0.0007f;

		if(m_strafeRightSpeed < 0.0f)
		{
			m_strafeRightSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	float radians = (m_rotationY + 90) * 0.0174532925f;

	// Update the position.
	m_posX += sinf(radians) * m_strafeRightSpeed;
	m_posY += cosf(radians) * m_strafeRightSpeed;
}

/*
================
 Position::getYRotation
================
*/
void Position::getYRotation( float& rotation)
{
	rotation = m_rotationY;
}
