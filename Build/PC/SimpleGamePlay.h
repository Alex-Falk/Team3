//#pragma once
//
//#include "Map.h"
//
//class SimpleGamePlay : public Map
//{
//private:
//	float m_AccumTime = 0;
//	Vector2 dimensions;
//	Map* map;
//
//
//	//--------------------------------------------------------------------------------------------//
//	// Special objects in the map
//	//--------------------------------------------------------------------------------------------//
//	CaptureArea * mc;
//	CaptureArea * mc2;
//
//public:
//	//--------------------------------------------------------------------------------------------//
//	// Initialisation and Cleanup
//	//--------------------------------------------------------------------------------------------//
//	SimpleGamePlay(const std::string& friendly_name) :
//		Map(friendly_name)
//	{
//	}
//
//	~SimpleGamePlay() {
//		delete mc;
//		delete mc2;
//	}
//
//	virtual void OnInitializeScene() override;
//	virtual void SetSpawnLocations() override;
//	virtual void AddObjects() override;
//
//	//--------------------------------------------------------------------------------------------//
//	// Special Object udpates (e.g. Pickups)
//	//--------------------------------------------------------------------------------------------//
//	virtual void OnUpdateScene(float dt) override;
//
//	//--------------------------------------------------------------------------------------------//
//	// Utility. TODO: can probably (re)move this
//	//--------------------------------------------------------------------------------------------//
//	void onButtonClicked();
//};

#include "Map.h"
#include "MinionCaptureArea.h"

class SimpleGamePlay : public Map
{
private:
	float m_AccumTime = 0;
	Vector2 dimensions;
	float dicks;
public:
	//--------------------------------------------------------------------------------------------//
	// Initialisation and Cleanup
	//--------------------------------------------------------------------------------------------//
	SimpleGamePlay(const std::string& friendly_name, const std::string& mapName) :
		Map(friendly_name, mapName)
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
