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
#include "Score.h"
#include "CaptureArea.h"
//#include "GroundScore.h"

class Map : public Scene
{
protected:
	float m_AccumTime = 0;
	Score* score;
	Vector3 spawnPositions[4];
	vector<CaptureArea*> captureAreas; //TODO move this to wherever is best, used in minion class (ClosestCaptureArea)
	static int mapIndex; // Controls which map will be loaded

	//--------------------------------------------------------------------------------------------//
	// UI Elements in the scene
	//--------------------------------------------------------------------------------------------//
	//CEGUI::ProgressBar* energyBar;

	//--------------------------------------------------------------------------------------------//
	// Score Related Variables
	//--------------------------------------------------------------------------------------------//
	int xDimension = 40;
	int yDimension = 40;
	int groundScoreAccuracy = 15;

	//pickup stuff
	uint npickup;
	Pickup** pickup;
	//capture areas for minimap
	uint ncapture;
	CaptureArea** capture;
public:
	//--------------------------------------------------------------------------------------------//
	// Initialization
	//--------------------------------------------------------------------------------------------//
	Map(const std::string& friendly_name) : Scene(friendly_name) {}
	~Map() {
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

	virtual void LoadTextures();
	virtual void AddObjects() {};
	virtual void SetSpawnLocations();
	virtual void InitializeScores();

	void AddCaptureArea(CaptureArea * ca) {
		captureAreas.push_back(ca);
	}
	CaptureArea * GetCaptureArea(int i) { return captureAreas[i]; }
	vector<CaptureArea*> GetCaptureAreaVector() { return captureAreas; }

	static int GetMapIndex() { return mapIndex; }
	void SetMapIndex(int mapIndx); 
	//--------------------------------------------------------------------------------------------//
	// Updating Avatars and Scores
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	inline Score * GetScore() { return score; }

	//phil 21/02/2018 for minimap
	inline int GetXDimension() { return xDimension; }
	inline int GetYDimension() { return yDimension; }

	inline uint GetNPickup() { return npickup; }
	inline Pickup** GetPickups() { return pickup; }
	inline uint GetNCapture() { return ncapture; }
	inline CaptureArea** GetCaptureAreas() { return capture; }
};

