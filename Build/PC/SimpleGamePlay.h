#pragma once

#include "Map.h"



// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions
//



class SimpleGamePlay : public Map
{
private:

	float m_AccumTime = 0;

	//--------------------------------------------------------------------------------------------//
	// Special objects in the map
	//--------------------------------------------------------------------------------------------//
	Pickup* pickup;

	//--------------------------------------------------------------------------------------------//
	// Setting up map specific variables for score
	//--------------------------------------------------------------------------------------------//
	static const int xDimension = 40;
	static const int yDimension = 40;

	static const int xOnGrid = 2 * (xDimension + 10)*groundScoreAccuracy; //Array cordinates for the x position of the player on the grid
	static const int yOnGrid = 2 * (yDimension + 10)*groundScoreAccuracy; //Array cordinates for the y position of the player on the grid

	Colour ground[xOnGrid][yOnGrid];

public:
	//--------------------------------------------------------------------------------------------//
	// Initialisation and Cleanup
	//--------------------------------------------------------------------------------------------//
	SimpleGamePlay(const std::string& friendly_name) :
		Map(friendly_name)
	{}

	~SimpleGamePlay() {
		delete pickup;
	}

	void OnCleanupScene();

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
	// Utility. TODO: can probably (re)move this
	//--------------------------------------------------------------------------------------------//
	void onButtonClicked();
};
