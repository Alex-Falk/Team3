#pragma once

// Daniel Burns [12/02/2018] Testing Map

#include "Map.h"

class level2 : public Map
{
private:

	float m_AccumTime = 0;

	//--------------------------------------------------------------------------------------------//
	// Special objects in the map
	//--------------------------------------------------------------------------------------------//
	static const uint npickup = 3;
	Pickup* pickup[npickup];

	Pickup* pickupSpeedBoost;
	Pickup* pickupJumpBoost;
	Pickup* pickupWeapon;
	Pickup* pickupPool;

	GameObject* object;

public:
	//--------------------------------------------------------------------------------------------//
	// Initialisation and Cleanup
	//--------------------------------------------------------------------------------------------//
	level2(const std::string& friendly_name)
		: Map(friendly_name) {}

	~level2()
	{
		delete pickupJumpBoost;
		delete pickupSpeedBoost;
		delete pickupWeapon;
	}

	virtual void OnInitializeScene() override;
	virtual void SetSpawnLocations() override;

	//--------------------------------------------------------------------------------------------//
	// Special Object udpates (e.g. Pickups)
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//
	void CreateEnvironment();
	//void GameplayTesting();

};