#pragma once

#include "Map.h"
#include "MinionCamp.h"

class SimpleGamePlay : public Map
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
	SimpleGamePlay(const std::string& friendly_name) :
		Map(friendly_name)
	{
		SetNumOfPickups(5);
		pickup = new Pickup*[npickup];
		SetNumOfCaptureAreas(2);
		capture = new CaptureArea*[ncapture];
	}

	~SimpleGamePlay() {
		delete[] pickup;
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
