//Name: Nick Bedford
//Date: 06/02/2018
//Simple class to build on for pickup functionality

#include "Pickup.h"
#include <string.h>

Pickup::Pickup()
{
	active = true;
	respawnTime = 30.0;
	currentRespawnTimer = 0.0;
	object = CommonUtils::BuildCuboidObject("Pickup",
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.5f, 0.5f, 0.5f),
		true,									//Has Physics Object
		0.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PICKUP,
		Vector4(0.0f, 1.0f, 0.0f, 1.0f));	        //Color
}

Pickup::Pickup(Vector3 pos, PickupType type, float respawnTime)
{
	active = true;
	this->respawnTime = respawnTime;
	currentRespawnTimer = 0.0;

	object = CommonUtils::BuildCuboidObject("Pickup",
		Vector3(0.0f, 1.0, 0.0f) + pos,
		Vector3(0.5f, 0.5f, 0.5f),				//Radius
		true,									//Has Physics Object
		0.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PICKUP,
		Vector4(0.0f, 1.0f, 0.0f, 1.0f));		//Colour

	object->Physics()->SetOnCollisionCallback(
		std::bind(&Pickup::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

void Pickup::Reactivate()
{
	object->Render()->SetChildBaseColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	active = true;
	currentRespawnTimer = 0;
}

void Pickup::Update(float dt)
{
	if (!active)
	{
		object->Render()->SetChildBaseColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		
		object->Physics()->SetAngularVelocity(Vector3(0.1f, 0.4f, 0.3f));

		currentRespawnTimer += dt;
		if (currentRespawnTimer >= respawnTime)
		{
			Reactivate();
		}
	}
	else
	{
		object->Physics()->SetAngularVelocity(Vector3(0.3f, 1.2f, 0.9f));
	}
}

Pickup::~Pickup()
{
}