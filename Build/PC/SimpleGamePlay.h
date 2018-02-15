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
#include "MinionCamp.h"


// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions
//



class SimpleGamePlay : public Scene
{
private:

	float m_AccumTime = 0;
	Avatar* player;
	MinionCamp * minionCamp;
	Pickup* pickup;
	CEGUI::ProgressBar* energyBar;

public:
	SimpleGamePlay(const std::string& friendly_name)
		: Scene(friendly_name) {}

	~SimpleGamePlay()
	{
		TextureManager::Instance()->RemoteAllTexture();
		delete player;
		delete pickup;
		delete minionCamp;
	}

	void OnCleanupScene();

	virtual void OnInitializeScene() override;
	
	virtual void OnUpdateScene(float dt) override;

	virtual void OnInitializeGUI() override;

	void onButtonClicked();
};