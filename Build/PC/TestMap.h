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

	static const int xOnGrid = 2 * (xDimension + 10)*groundScoreAccuracy; //Array cordinates for the x position of the player on the grid
	static const int yOnGrid = 2 * (yDimension + 10)*groundScoreAccuracy; //Array cordinates for the y position of the player on the grid

	Colour ground[xOnGrid][yOnGrid];

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
	// Score Related Functions
	//--------------------------------------------------------------------------------------------//
	virtual void BuildGroundScore(); //Builds the array for the ground score
	virtual void UpdateGroundScore(Avatar* player); //Updates the ground cells 

	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//
	void CreateEnvironment();

	void GameplayTesting();
};