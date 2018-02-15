#pragma once

// Daniel Burns [12/02/2018] Testing Map

#include <nclgl\NCLDebug.h>
#include <ncltech\Scene.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\GameObject.h>
#include <ncltech\TextureManager.h> 
#include "GamePlay.h"
#include "Avatar.h"
#include "Pickup.h"
#include "WeaponPickup.h"
#include "Game.h"



class TestMap : public Scene
{
private:

	float m_AccumTime = 0;
	//Avatar* player;
	Pickup* pickup_SpeedBoost;
	Pickup* pickup_JumpBoost;
	Pickup* pickup_Weapon;
	WeaponPickup* weaponPickup;
	CEGUI::ProgressBar* energyBar;

	GameObject* object;

	Colour colour;
	PickupType type;

	bool active;
	float respawnTime;
	float currentRespawnTimer;

public:
	TestMap(const std::string& friendly_name)
		: Scene(friendly_name) {}

	~TestMap()
	{
		TextureManager::Instance()->RemoveAllTexture();
		//delete player;
		delete pickup_JumpBoost;
		delete pickup_SpeedBoost;
		delete pickup_Weapon;
	}

	virtual void OnInitializeScene() override;

	void onConnectToScene() override;

	virtual void OnUpdateScene(float dt) override;
	
	virtual void OnInitializeGUI() override;

	void OnCleanUpScene();

	void CreateEnvironment();

	void GameplayTesting();

	void PaintEnvironment();

	void OnButtonClicked();
};