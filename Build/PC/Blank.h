#pragma once

#include <nclgl\NCLDebug.h>
#include <ncltech\Scene.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\CommonUtils.h>
#include "GamePlay.h"
#include "Player.h"
#include "PaintableGameObject.h"


// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions

GameObject* player;
Player* p1;

class Blank : public Scene
{
public:
	Blank(const std::string& friendly_name)
		: Scene(friendly_name)
	{
	}

	const Vector3 ss_pos = Vector3(-5.5f, 1.5f, -5.0f);
	const Vector3 sc_pos = Vector3(4.5f, 1.5f, -5.0f);
	const Vector3 cc_pos = Vector3(-0.5f, 1.5f, 5.0f);

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

<<<<<<< Updated upstream
		this->AddGameObject(ground);

		p1 = new Player(Vector3(0.0, 1.0, 0.0), Green, 1.0f);



		player = p1->GetObj();

		this->AddGameObject(player);


		GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->Physics());
		GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);

=======
		PaintableGameObject pgo(ground, 1, true);
		
		Player* p1 = new Player(Vector3(0.0, 5.0, 0.0), Green, 1.0f);
		pgo.CollisionOccured(p1);

		this->AddGameObject(ground);
	
		
		//Sphere-Sphere
		{

			this->AddGameObject(CommonUtils::BuildSphereObject("ss1",
				ss_pos + Vector3(0.0f, 5.0f, 0.0f),		//Position leading to 0.25 meter overlap between spheres
				0.5f,									//Radius
				true,									//Has Physics Object
				1.0f,
				true,									//Has Collision Shape
				true,									//Dragable by the user
				CommonUtils::GenColor(0.45f, 0.5f)));	//Color

			this->AddGameObject(CommonUtils::BuildSphereObject("ss2",
				ss_pos,									//Position
				0.5f,									//Radius
				true,									//Has Physics Object
				1.0f,
				true,									//Has Collision Shape
				true,									//Dragable by the user
				CommonUtils::GenColor(0.5f, 1.0f)));	//Color

		}

		//Sphere-Cuboid
		{
			this->AddGameObject(CommonUtils::BuildCuboidObject("sc1",
				sc_pos,									//Position
				Vector3(0.5f, 0.5f, 0.5f),				//Half dimensions
				true,									//Has Physics Object
				1.0f,									//Infinite Mass
				true,									//Has Collision Shape
				true,									//Dragable by the user
				CommonUtils::GenColor(0.5f, 1.0f)));	//Color

			this->AddGameObject(CommonUtils::BuildSphereObject("sc2",
				sc_pos + Vector3(0.0f, 5.0f, 0.0f),		//Position leading to 0.1 meter overlap on faces, and more on diagonals
				0.5f,									//Radius
				true,									//Has Physics Object
				1.0f,									//Infinite Mass
				true,									//Has Collision Shape
				true,									//Dragable by the user
				CommonUtils::GenColor(0.45f, 0.5f)));	//Color
		}

		//Cuboid-Cuboid
		{

			this->AddGameObject(CommonUtils::BuildCuboidObject("cc1",
				cc_pos + Vector3(0.0f, 5.0f, 0.0f),	//Position leading to 0.25 meter overlap on faces, and more on diagonals
				Vector3(0.5f, 0.5f, 0.5f),				//Half dimensions
				true,									//Has Physics Object
				1.0f,									//Infinite Mass
				true,									//Has Collision Shape
				true,									//Dragable by the user
				CommonUtils::GenColor(0.45f, 0.5f)));	//Color

			this->AddGameObject(CommonUtils::BuildCuboidObject("cc2",
				cc_pos,									//Position
				Vector3(0.5f, 0.5f, 0.5f),				//Half dimensions
				true,									//Has Physics Object
				1.0f,									//Infinite Mass
				true,									//Has Collision Shape
				true,									//Dragable by the user
				CommonUtils::GenColor(0.5f, 1.0f)));	//Color
		}

		//TEMP Camera Center
		{
			GameObject* center = CommonUtils::BuildSphereObject(
				"Center",
				Vector3(-2.5f, 1.5f, -2.0f),			//Position
				0.5,									//Half dimensions
				true,									//Has Physics Object
				1.0f,									//Infinite Mass
				true,									//Has Collision Shape
				true,									//Dragable by the user
				Vector4(1.0,0.0,0.0,1.0));				//Color

			GraphicsPipeline::Instance()->GetCamera()->SetCenter(center->Physics());
			this->AddGameObject(center);
		}
>>>>>>> Stashed changes

		
		Scene::OnInitializeScene();
	}

	float m_AccumTime = 0;
	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);
		m_AccumTime += dt;

		p1->Input(dt);




		uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	}
};

