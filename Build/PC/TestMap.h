//#pragma once
//
//// Daniel Burns [12/02/2018] Testing Map
//
//#include "Map.h"
//
//
//
//class TestMap : public Map
//{
//private:
//
//	float m_AccumTime = 0;
//
//	//--------------------------------------------------------------------------------------------//
//	// Special objects in the map
//	//--------------------------------------------------------------------------------------------//
//	Vector3* pickupTextOffset;
//
//	Pickup* pickupSpeedBoost;
//	Pickup* pickupJumpBoost;
//	Pickup* pickupWeapon;
//
//	GameObject* object;
//
//public:
//	//--------------------------------------------------------------------------------------------//
//	// Initialisation and Cleanup
//	//--------------------------------------------------------------------------------------------//
//	TestMap(const std::string& friendly_name)
//		: Map(friendly_name) {
//		npickup = 3;
//		pickup = new Pickup*[npickup];
//		pickupTextOffset = new Vector3[npickup];
//	}
//
//	~TestMap()
//	{
//		delete pickupJumpBoost;
//		delete pickupSpeedBoost;
//		delete pickupWeapon;
//
//		delete[] pickupTextOffset;
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
//	// Utility
//	//--------------------------------------------------------------------------------------------//
//	void CreateEnvironment();
//
//	void GameplayTesting();
//};