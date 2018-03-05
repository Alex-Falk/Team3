#pragma once

#include "Map.h"
#include "MinionCamp.h"

class SimpleGamePlay : public Map
{
private:
	float m_AccumTime = 0;
	Vector2 dimensions;

public:
	//--------------------------------------------------------------------------------------------//
	// Initialisation and Cleanup
	//--------------------------------------------------------------------------------------------//
	SimpleGamePlay(const std::string& friendly_name) :
		Map(friendly_name)
	{
	}

	~SimpleGamePlay() {
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
