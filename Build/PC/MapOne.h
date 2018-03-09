#pragma once

#include "Map.h"
#include "MinionCaptureArea.h"

class MapOne : public Map
{
private:
	float m_AccumTime = 0;
	Vector2 dimensions;

public:
	//--------------------------------------------------------------------------------------------//
	// Initialisation and Cleanup
	//--------------------------------------------------------------------------------------------//
	MapOne(const std::string& friendly_name) :
		Map(friendly_name)
	{
	}

	~MapOne() {
	}

	void CreateEnvironment();
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