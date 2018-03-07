// Fragkas Nikolaos
// Date 06/03/2018
// The 4th map

#pragma once

#include "Map.h"
#include "MinionCamp.h"

class StageFourth : public Map
{
private:
	float m_AccumTime = 0;
	Vector2 dimensions;
	Map* map;


	//--------------------------------------------------------------------------------------------//
	// Special objects in the map
	//--------------------------------------------------------------------------------------------//
	MinionCamp * mc;
	MinionCamp * mc2;

public:
	//--------------------------------------------------------------------------------------------//
	// Initialisation and Cleanup
	//--------------------------------------------------------------------------------------------//
	StageFourth(const std::string& friendly_name) :
		Map(friendly_name)
	{
	}

	~StageFourth() {
		delete mc;
		delete mc2;
	}

	virtual void OnInitializeScene() override;
	virtual void SetSpawnLocations() override;
	virtual void AddObjects() override;

	//--------------------------------------------------------------------------------------------//
	// Special Object udpates (e.g. Pickups)
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	//--------------------------------------------------------------------------------------------//
	// Utility. TODO: can probably (re)move this
	//--------------------------------------------------------------------------------------------//
	void onButtonClicked();
};
