#pragma once

// Daniel Burns [12/02/2018] Testing Map

#include "Map.h"



class TestMap : public Map
{
private:

	float m_AccumTime = 0;

	//--------------------------------------------------------------------------------------------//
	// Special objects in the map
	//--------------------------------------------------------------------------------------------//
	static const uint npickup = 3;
	Pickup* pickup[npickup];
	Vector3 pickupTextOffset[npickup];

	Pickup* pickupSpeedBoost;
	Pickup* pickupJumpBoost;
	Pickup* pickupPaintSpray;
	Pickup* pickupPaintPistol;
	Pickup* pickupAutoPaintLauncher;
	Pickup* pickupPaintRocket;
	Pickup* pickupPool;

	GameObject* object;

public:
	//--------------------------------------------------------------------------------------------//
	// Initialisation and Cleanup
	//--------------------------------------------------------------------------------------------//
	TestMap(const std::string& friendly_name)
		: Map(friendly_name) {}

	~TestMap()
	{
		delete pickupJumpBoost;
		delete pickupSpeedBoost;
		delete pickupPaintSpray;
		delete pickupPaintPistol;
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
	void GameplayTesting();

};