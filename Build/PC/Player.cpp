#include "Player.h"
#include <string.h>

Player::Player()
{
	CommonUtils::BuildSphereObject("Player1" ,
		Vector3(0.0f, 1.0f, 0.0f),
		1.0f,									//Radius
		true,									//Has Physics Object
		1.0f,
		true,									//Has Collision Shape
		true,									//Dragable by the user
		Vector4(0.5, 0.5, 0.5, 1.0));	//Color
}

Player::~Player()
{
}

Player::Player(Vector3 pos, Colour c, float s)
{
	Vector4 colour;

	switch (c)
	{
	case Green: 
	{
		colour=	Vector4(0.0, 1.0, 0.0, 1.0);
	}
		break;
	case Blue:
	{
		colour = Vector4(0.0, 0.0, 1.0, 1.0);
	}
		break;
	case Red:
	{
		colour = Vector4(1.0, 0.0, 0.0, 1.0);
	}
		break;
	case Pink:
	{
		colour = Vector4(1.0, 2.0, 1.0, 1.0);
	}
		break;
	default:
	{
		colour = Vector4(0.5, 0.5, 0.5, 1.0);
	}
		break;
	}

	CommonUtils::BuildSphereObject("Player" + to_string(c),
		Vector3(0.0f, 1.0f, 0.0f) + pos,
		1.0f * s,								//Radius
		true,									//Has Physics Object
		1.0f / s,
		true,									//Has Collision Shape
		true,									//Dragable by the user
		colour);								//Colour
}

