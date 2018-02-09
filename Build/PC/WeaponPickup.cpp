//Name: Nick Bedford
//Date: 07/02/2018
//Simple class to build on for weapon pickup functionality

#include "WeaponPickup.h"
#include <string.h>

WeaponPickup::WeaponPickup()
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
		Vector4(0.0f, 1.0f, 0.0f, 1.0f));	        //Color
}

WeaponPickup::WeaponPickup(Vector3 pos, WeaponType type, float respawnTime)
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
		Vector4(0.0f, 1.0f, 0.0f, 1.0f));		//Colour

	object->Physics()->SetOnCollisionCallback(
		std::bind(&Pickup::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

void WeaponPickup::Reactivate()
{
	object->Render()->SetChildBaseColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	active = true;
	currentRespawnTimer = 0;
	weaponType = static_cast<WeaponType>(rand() % NUM_OF_WEAPONS);
	cout << weaponType << endl;
}

WeaponPickup::~WeaponPickup()
{
}