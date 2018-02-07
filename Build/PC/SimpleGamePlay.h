#pragma once

#include <nclgl\NCLDebug.h>
#include <ncltech\Scene.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\CommonUtils.h>
#include "GamePlay.h"
#include "Player.h"

// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions




class SimpleGamePlay : public Scene
{
private:

	float m_AccumTime = 0;
	Player* player;

public:
	SimpleGamePlay(const std::string& friendly_name)
		: Scene(friendly_name) {}

	virtual void OnInitializeScene() override;
	
	virtual void OnUpdateScene(float dt) override;
};