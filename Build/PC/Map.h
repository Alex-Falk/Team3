#pragma once
#include <nclgl\NCLDebug.h>
#include <ncltech\Scene.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\TextureManager.h> 
#include "GamePlay.h"
#include "Pickup.h"
#include "Avatar.h"
#include "ControllableAvatar.h"
#include "Game.h"
#include "GamePlay.h"
#include "WeaponPickup.h"
#include "CaptureArea.h"

class Map : public Scene
{
protected:
	float m_AccumTime = 0;
	Vector3 spawnPositions[4];
	
	static int mapIndex; // Controls which map will be loaded

	
	//--------------------------------------------------------------------------------------------//
	// UI Elements in the scene
	//--------------------------------------------------------------------------------------------//
	CEGUI::ProgressBar* lifeBar;

	//--------------------------------------------------------------------------------------------//
	// Map Size
	//--------------------------------------------------------------------------------------------//
	Vector2 dimensions;
	inline void SetMapDimensions(Vector2 dimens) { dimensions = dimens; }
	inline Vector2 GetMapDimensions() { return dimensions; }


	//pickup stuff
	uint npickup;
	vector<Pickup*> pickups;
	//capture areas for minimap
	uint ncapture;
	vector<CaptureArea*> captureAreas; //TODO move this to wherever is best, used in minion class (ClosestCaptureArea)


public:
	//--------------------------------------------------------------------------------------------//
	// Initialization
	//--------------------------------------------------------------------------------------------//
	Map(const std::string& friendly_name) : Scene(friendly_name) {}
	virtual ~Map() {
		TextureManager::Instance()->RemoveAllTexture();
		for (auto itr = pickups.begin(); itr != pickups.end(); ++itr)
		{
			delete (*itr);
		}
		pickups.clear();

		for (auto itr = captureAreas.begin(); itr != captureAreas.end(); ++itr)
		{
			delete (*itr);
		}
		captureAreas.clear();

	};

	virtual void OnCleanupScene();

	void onConnectToScene() override;
	virtual void OnInitializeScene() override;
	virtual void OnInitializeGUI() override;

	void BuildGround(Vector2 Dimensions);
	virtual void LoadTextures();
	virtual void AddObjects() {};
	virtual void SetSpawnLocations();

	void AddPickup(Pickup * p) {
		pickups.push_back(p);
	}

	Pickup * GetPickup(int i) { return pickups[i]; }
	vector<Pickup*> GetPickups() { return pickups; }


	void AddCaptureArea(CaptureArea * ca) {
		captureAreas.push_back(ca);
	}

	CaptureArea * GetCaptureArea(int i) { return captureAreas[i]; }
	vector<CaptureArea*> GetCaptureAreaVector() { return captureAreas; }

	static int GetMapIndex() { return mapIndex; }
	void SetMapIndex(int mapIndx);
	//--------------------------------------------------------------------------------------------//
	// Updating Avatars
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	//phil 21/02/2018 for minimap
	inline int GetXDimension() { return dimensions.x; }
	inline int GetYDimension() { return dimensions.y; }


};

