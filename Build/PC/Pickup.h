//Name: Nick Bedford
//Date: 06/02/2018
//Simple class to build on for pickup functionality

#pragma once

#include <ncltech\GameObject.h>
#include <ncltech\CommonUtils.h>
#include <nclgl\NCLDebug.h>
#include "GamePlay.h"

class Pickup : GameObject
{
public:
	Pickup();
	Pickup(Vector3 pos, PickupType type, float respawnTime = 30.0);

	void Update(float dt);
	GameObject* GetObj() { return object; }
	
	//Example of member callback ('this' parameter is bound at bind time)
	bool PickupCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject)
	{
		if (collidingObject->getName() == "Player")
		{
			this->active = false;

			NCLDebug::Log(Vector3(1.0f, 0.0f, 0.0f), "Pickup collision");
		}

		//Return true to enable collision resolution, for Pickup just return false so we can drop the collision pair from the system
		return false;
	}


	~Pickup();

private:

	GameObject* object;
	
	PickupType type;

	bool active;
	float respawnTime;
	float currentRespawnTimer;
};

