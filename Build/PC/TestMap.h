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
	Pickup* pickupSpeedBoost;
	Pickup* pickupJumpBoost;
	Pickup* pickupWeapon;

	GameObject* object;

	//--------------------------------------------------------------------------------------------//
	// Setting up map specific variables for score
	//--------------------------------------------------------------------------------------------//
	static const int xDimension = 30;
	static const int yDimension = 30;

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
		delete pickupWeapon;
	}

	void OnCleanUpScene();

	virtual void OnInitializeScene() override;

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