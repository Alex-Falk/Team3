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

//#include "GroundScore.h"

class Map : public Scene
{
protected:
	float m_AccumTime = 0;
	Score* score;
	Vector3 spawnPositions[4];

	//--------------------------------------------------------------------------------------------//
	// UI Elements in the scene
	//--------------------------------------------------------------------------------------------//
	CEGUI::ProgressBar* energyBar;

	//--------------------------------------------------------------------------------------------//
	// Score Related Variables
	//--------------------------------------------------------------------------------------------//
	int xDimension = 40;
	int yDimension = 40;

	int groundScoreAccuracy = 15;

public:
	//--------------------------------------------------------------------------------------------//
	// Initialization
	//--------------------------------------------------------------------------------------------//
	Map(const std::string& friendly_name) : Scene(friendly_name) {}
	~Map() {
		TextureManager::Instance()->RemoveAllTexture();
	};

	virtual void OnCleanupScene();

	void onConnectToScene() override;
	virtual void OnInitializeScene() override;
	virtual void OnInitializeGUI() override;

	virtual void LoadTextures();
	virtual void AddObjects() {};
	virtual void SetSpawnLocations();
	virtual void InitializeScores();

	//--------------------------------------------------------------------------------------------//
	// Updating Avatars and Scores
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	inline Score * GetScore() { return score; }

};

