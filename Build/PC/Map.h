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
#include "MultiPaintPool.h"

class Map : public Scene
{
protected:
	float m_AccumTime = 0;
	float updatePerSecond = 0;
	Vector3 spawnPositions[4];
	vector<CaptureArea*> captureAreas; //TODO move this to wherever is best, used in minion class (ClosestCaptureArea)
	static int mapIndex; // Controls which map will be loaded


	//--------------------------------------------------------------------------------------------//
	// UI Elements in the scene
	CEGUI::ProgressBar* lifeBar;
	CEGUI::Titlebar* timer;
	//--------------------------------------------------------------------------------------------//

	//--------------------------------------------------------------------------------------------//
	// Map Size
	//--------------------------------------------------------------------------------------------//
	Vector2 dimensions;
	inline void SetMapDimensions(Vector2 dimens) { dimensions = dimens; }
	inline Vector2 GetMapDimensions() { return dimensions; }

	//Scoring
	void UpdateCaptureAreas();			

	//pickup stuff
	void SetNumOfPickups(uint x) { npickup = x; }
	uint npickup;
	Pickup** pickup;
	//capture areas for minimap
	void SetNumOfCaptureAreas(uint x) { ncapture = x; }
	uint ncapture;
	CaptureArea** capture;

public:
	//--------------------------------------------------------------------------------------------//
	// Initialization
	//--------------------------------------------------------------------------------------------//
	Map(const std::string& friendly_name) : Scene(friendly_name) {}
	virtual ~Map() {
		TextureManager::Instance()->RemoveAllTexture();
		//delete the array of pickups
		if (pickup) {
			delete[] pickup;
		}
		if (capture) {
			delete[] capture;
		}
	};

	virtual void OnCleanupScene();

	void onConnectToScene() override;
	virtual void OnInitializeScene() override;
	virtual void OnInitializeGUI() override;

	void BuildGround(Vector2 Dimensions);
	virtual void LoadTextures();
	virtual void AddObjects() {};
	virtual void SetSpawnLocations();

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

	//Jeffery 06/03/2018 for timer GUI
	void TransferAndUpdateTimer();

	//Jeffery 06/03/2018 for updating playername and position for GUI rendering
	void UpdateGUI(float dt);

	//phil 21/02/2018 for minimap
	inline int GetXDimension() { return dimensions.x; }
	inline int GetYDimension() { return dimensions.y; }

	inline uint GetNPickup() { return npickup; }
	inline Pickup** GetPickups() { return pickup; }
	inline uint GetNCapture() { return ncapture; }
	inline CaptureArea** GetCaptureAreas() { return capture; }

	float temp_fps = 0;
	bool isLoading = false;
};

