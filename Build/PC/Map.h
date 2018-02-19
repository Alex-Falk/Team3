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

	Vector3 spawnPositions[4];

	//--------------------------------------------------------------------------------------------//
	// UI Elements in the scene
	//--------------------------------------------------------------------------------------------//
	CEGUI::ProgressBar* energyBar;

	//--------------------------------------------------------------------------------------------//
	// Score Related Variables
	//--------------------------------------------------------------------------------------------//
	int groundTeamScore[5];
	int teamScores[5];

	static const int groundScoreAccuracy = 100;
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

	//--------------------------------------------------------------------------------------------//
	// Updating Avatars and Scores
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	//--------------------------------------------------------------------------------------------//
	// Score Related Functions
	//--------------------------------------------------------------------------------------------//
	virtual void BuildGroundScore() = 0; //Builds the array for the ground score
	virtual void UpdateGroundScore(Avatar* player) = 0; //Updates the ground cells 
	void ChangeGridScore(Colour teamToDecrease, Colour teamToIncrease); // updates the score
	void PrintScore(int x); // debug
};

