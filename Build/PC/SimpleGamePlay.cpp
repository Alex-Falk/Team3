#include "WeaponPickup.h"
#include "SimpleGamePlay.h"
#include "Game.h"

void SimpleGamePlay::OnInitializeScene() {
	GraphicsPipeline::Instance()->SetIsMainMenu(false);

	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

	//Create Ground (..everybody loves finding some common ground)
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(40.0f, 1.0f, 40.0f),
		true,
		0.0f,
		true,
		false,
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),
		MATERIALTYPE::Ground);

	this->AddGameObject(ground);

	//player = new Player(Vector3(0.0, 1.0, 0.0), DEFAULT_COLOUR, 0, 1.0f);
	for (uint i = 0; i < 4; i++) {
		if (Game::Instance()->GetUser())
		{
			Avatar * p = nullptr;
			if (i == Game::Instance()->getUserID())
			{
				p = new ControllableAvatar(Vector3(i * 3, 1.0, 0.0), Colour(i), i, 1.0f);
			}
			else
			{
				p = new Avatar(Vector3(i * 3, 1.0, 0.0), Colour(i), i, 1.0f);
			}

			this->AddGameObject(p->GetGameObject());
			Game::Instance()->SetAvatar(i, p);
		}
	}
	//this->AddGameObject(player->GetGameObject());

	pickup = new Pickup(Vector3(0, 3, 0), SPEED_BOOST);

	this->AddGameObject(pickup->GetObj());

	//GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
	if (Game::Instance()->GetUser())
	{
		GraphicsPipeline::Instance()->GetCamera()->SetCenter(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetGameObject()->Physics());
		GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);

		GraphicsPipeline::Instance()->InitPath(Vector2(40, 40));
		GraphicsPipeline::Instance()->AddPlayerRenderNode(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetGameObject()->Render());
	}

	OnInitializeGUI();
	Scene::OnInitializeScene();
}

void SimpleGamePlay::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	//player->OnPlayerUpdate(dt);
	for (uint i = 0; i < 4; i++) {
		if (Game::Instance()->GetPlayer(i))
			Game::Instance()->GetPlayer(i)->OnAvatarUpdate(dt);
	}

	if (pickup)
	{
		pickup->Update(dt);
	}

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	if (Game::Instance()->GetUser())
		energyBar->setProgress(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetLife()/100.0f);
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

	if (Game::Instance()->GetUser())
		energyBar->setProgress(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetLife()/100.0f);
}

void SimpleGamePlay::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}