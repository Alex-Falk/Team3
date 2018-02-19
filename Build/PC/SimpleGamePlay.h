#pragma once

#include "Map.h"



// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions
//



class SimpleGamePlay : public Map
{
private:

	float m_AccumTime = 0;
	CEGUI::ProgressBar* energyBar;

	//--------------------------------------------------------------------------------------------//
	// Special objects in the map
	//--------------------------------------------------------------------------------------------//
	static const uint npickup = 2;
	Pickup* pickup[npickup];

	//--------------------------------------------------------------------------------------------//
	// Setting up map specific variables for score
	//--------------------------------------------------------------------------------------------//
	static const int xDimension = 40;
	static const int yDimension = 40;

	static const int groundScoreAccuracy = 15;
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
	// Utility. TODO: can probably (re)move this
	//--------------------------------------------------------------------------------------------//
	void onButtonClicked();
};
