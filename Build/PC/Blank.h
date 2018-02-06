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


Player* p1ayer;

class Blank : public Scene
{
public:
	Blank(const std::string& friendly_name)
		: Scene(friendly_name)
	{
	}

	virtual void OnInitializeScene() override
	{
		//Create Ground (..everybody loves finding some common ground)
		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(20.0f, 1.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		this->AddGameObject(ground);

		p1ayer = new Player(Vector3(0.0, 1.0, 0.0),PINK, 1.0f);

		this->AddGameObject(p1ayer->GetGameObject());

		GraphicsPipeline::Instance()->GetCamera()->SetCenter(p1ayer->GetGameObject()->Physics());
		GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);


		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"box",
			Vector3(-4.0f, 4.5f, -4.0f),
			Vector3(3.0f, 3.0f, 3.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

		Scene::OnInitializeScene();
	}

	float m_AccumTime=0;
	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);
		m_AccumTime += dt;

		p1ayer->Input(dt);
		



		uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	}
};

