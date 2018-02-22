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

class Map : public Scene
{
protected:
	float m_AccumTime = 0;
	Score* score;
	Vector3 spawnPositions[4];
	static int mapIndex; // Controls which map will be loaded

	//--------------------------------------------------------------------------------------------//
	// UI Elements in the scene
	//--------------------------------------------------------------------------------------------//
	CEGUI::ProgressBar* energyBar;

	//--------------------------------------------------------------------------------------------//
	// Score Related Variables
	//--------------------------------------------------------------------------------------------//
	int groundTeamScore[5];
	int teamScores[5];

public:
	//--------------------------------------------------------------------------------------------//
	// Initialization
	//--------------------------------------------------------------------------------------------//
	Map(const std::string& friendly_name) : Scene(friendly_name) {}
	~Map() {
		TextureManager::Instance()->RemoveAllTexture();
	};


	void onConnectToScene() override;
	virtual void OnInitializeScene() override;
	virtual void OnInitializeGUI() override;

	static int GetMapIndex() { return mapIndex; }
	void SetMapIndex(int mapIndx); 

	//--------------------------------------------------------------------------------------------//
	// Updating Avatars and Scores
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	inline Score * GetScore() { return score; }

};

