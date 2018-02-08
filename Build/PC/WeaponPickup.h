//Name: Nick Bedford
//Date: 07/02/2018
//Simple class to build on for weapon pickup functionality

#pragma once

#include "Pickup.h"

class WeaponPickup : public Pickup 
{
public:
	WeaponPickup();
	WeaponPickup(Vector3 pos, WeaponType type, float respawnTime = 30.0);

	/*void Update(float dt);*/

	void Reactivate();

	WeaponType GetType() { return weaponType; }
	
	void SetType(WeaponType newType) { weaponType = newType; }

	~WeaponPickup();

private:

	WeaponType weaponType;
	
};

