#include "WeaponPickup.h"
#include "SimpleGamePlay.h"
#include "GamePlay.h"


void SimpleGamePlay::OnInitializeScene() {
	GraphicsPipeline::Instance()->SetIsMainMenu(false);

	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

	//Create Ground (..everybody loves finding some common ground)
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(DIMENSION_X, 1.0f, DIMENSION_Y),
		true,
		0.0f,
		true,
		false,
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),
		MATERIALTYPE::Ground);

	this->AddGameObject(ground);

	//BuildGroundScore();

	player = new ControllableAvatar(Vector3(4.0, 7.0, 300.0), START_COLOUR, 0, 1.0f);

	this->AddGameObject(player->GetGameObject());

	pickup = new Pickup(Vector3(0, 3, 0), JUMP_BOOST);

	this->AddGameObject(pickup->GetObj());

	GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);

	GraphicsPipeline::Instance()->InitPath(Vector2(DIMENSION_X, DIMENSION_Y));
	GraphicsPipeline::Instance()->AddPlayerRenderNode(player->GetGameObject()->Render());

	OnInitializeGUI();
	Scene::OnInitializeScene();
}

void SimpleGamePlay::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	player->OnAvatarUpdate(dt);

	if (pickup)
	{
		pickup->Update(dt);
	}

	//UpdateGroundScore(groundScore, player->GetPosition(),player->GetSize());

	//PrintScore();

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	energyBar->setProgress(player->GetLife()/100.0f);
}

void SimpleGamePlay::OnCleanupScene()
{
	DeleteAllGameObjects();
	TextureManager::Instance()->RemoteAllTexture();
	GraphicsPipeline::Instance()->RemoteAllPlayerRenderNode();
};

void SimpleGamePlay::OnInitializeGUI()
{
	//Call initi-function for gui
	sceneGUI = new GUI();
	sceneGUI->Init(CEGUIDIR);

	//Load Scheme - Which actually means UI style - notice that multiple Scheme could be load at once
	sceneGUI->LoadScheme("TaharezLook.scheme");
	sceneGUI->LoadScheme("AlfiskoSkin.scheme");

	//Set Font sytle
	sceneGUI->SetFont("DejaVuSans-10");

	//SetMouseCursor
	sceneGUI->SetMouseCursor("TaharezLook/MouseArrow");
	sceneGUI->ShowMouseCursor();

	//Create Push Button handle
	energyBar = static_cast<CEGUI::ProgressBar*>(
		sceneGUI->createWidget("TaharezLook/ProgressBar",
			Vector4(0.40f, 0.9f, 0.2f, 0.03f),
			Vector4(),
			"energyBar"
		));

	energyBar->setProgress(player->GetLife()/100.0f);
}

void SimpleGamePlay::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}